//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "XMLStoredFormatTest.h"


int testExampleFiles()
{
    printf("\nExample Files:\n\n");
    
    const wchar_t* exampleFilesLE[2] = 
    {
        L"input/axExample_le.xml",
        L"input/AVID_Newscutter_sequence_le.xml"
    };
    const wchar_t* exampleFilesBE[2] = 
    {
        L"input/axExample_be.xml",
        L"input/AVID_Newscutter_sequence_be.xml"
    };
    const wchar_t** exampleFiles;
    
    // the examples file are chosen based on the host byte order because
    // machines with different host byte orders were found to output the 
    // AAF XML elements in a different order and this caused the basic
    // diff function below to fail
    aafUInt32 longWord = 0x12345678;
    aafUInt8 byte = *((aafUInt8*)&longWord);
    if (byte == 0x12) 
    {
        exampleFiles = exampleFilesBE;
    }
    else
    {
        exampleFiles = exampleFilesLE;
    }
    
    
    bool passed = true;
    try
    {
        for (unsigned int i = 0; i < 2; i++)
        {
            printf("%ls:\n", exampleFiles[i]);

            bool thisFilePassed = true;
            
            printf("a) Roundtrip\n");

			wchar_t tmpFile[100];
			wcscpy(tmpFile, exampleFiles[i]);
			wcscat(tmpFile, L"_roundtrip_tmp.xml");

            saveCopy(exampleFiles[i], tmpFile);
            if (diff(exampleFiles[i], tmpFile))
            {
                printf("FAILED diff(%ls, %ls)\n", exampleFiles[i], tmpFile);
                thisFilePassed = false;
            }
            else
            {
                printf("passed\n");
            }
    
            printf("b) Modify\n");

			wcscpy(tmpFile, exampleFiles[i]);
			wcscat(tmpFile, L"_modify_tmp.xml");

            copyAndTouch(exampleFiles[i], tmpFile);
            if (diff(exampleFiles[i], tmpFile))
            {
                printf("FAILED diff(%ls, %ls)\n", exampleFiles[i], tmpFile);
                thisFilePassed = false;
            }
            else
            {
                printf("passed\n");
            }
            
            report(thisFilePassed);
            printf("\n");
            
            passed = passed && thisFilePassed;
        }
    }
    catch (const char* ex)
    {
        passed = false;
        printf("FAILED: Exception thrown: %s\n", ex);
    }
    catch (...)
    {
        passed = false;
        printf("FAILED: Exception thrown\n");
    }

    printf("\n");
    
    return passed ? 0 : 2;
}


