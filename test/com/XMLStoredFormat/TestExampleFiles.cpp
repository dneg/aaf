//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "XMLStoredFormatTest.h"


bool testExampleFiles()
{
    printf("\nExample Files:\n\n");
    
    const wchar_t* exampleFiles[2] = 
    {
        L"input/axExample.xml",
        L"input/AVID_Newscutter_sequence.xml"
    };
    
    bool passed = true;
    try
    {
        for (unsigned int i = 0; i < 2; i++)
        {
            printf("%ls:\n", exampleFiles[i]);

            bool thisFilePassed = true;
            
            printf("a) Roundtrip\n");
            saveCopy(exampleFiles[i], L"tmp.xml");
            if (diff(exampleFiles[i], L"tmp.xml"))
            {
                printf("FAILED\n");
                thisFilePassed = false;
            }
            else
            {
                printf("passed\n");
            }
    
            printf("b) Modify\n");
            copyAndTouch(exampleFiles[i], L"tmp.xml");
            if (diff(exampleFiles[i], L"tmp.xml"))
            {
                printf("FAILED\n");
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
    
    return passed;
}


