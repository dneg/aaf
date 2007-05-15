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


static bool testRestore(wchar_t* fileName)
{
    bool passed = true;
    
    IAAFFile* pFile = 0;
    IAAFHeader* pHeader = 0;
    IAAFDictionary* pDictionary = 0;
    IAAFContentStorage* pStorage = 0;
    IEnumAAFMobs* pMobs = 0;
    IAAFMob* pMob = 0;
    
    try
    {
        pFile = openFileForReading(fileName);
    }
    catch (...)
    {
        return false;
    }
    
    try
    {
        // get the Mob containing the test data
        checkResult(pFile->GetHeader(&pHeader));
        checkResult(pHeader->GetDictionary(&pDictionary));
        checkResult(pHeader->GetContentStorage(&pStorage));
        aafSearchCrit_t searchCrit;
        searchCrit.searchTag = kAAFByMobKind;
        searchCrit.tags.mobKind = kAAFAllMob;
        checkResult(pStorage->GetMobs(&searchCrit, &pMobs));
        checkResult(pMobs->NextOne(&pMob));
        
        IAAFObject* pObject = 0;
        IAAFClassDef* pClassDef = 0;
        IAAFPropertyDef* pPropertyDef = 0;
        IAAFPropertyValue* pPropertyValue = 0;
        IAAFTypeDef* pType = 0;
        IAAFTypeDefStream* pStreamType = 0;

        try
        {
            printf("    * Simple: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUInt8 testValue[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', '\n'};
            eAAFByteOrder_t testByteOrderValue = kAAFByteOrderBig;
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefStream, (void **)&pStreamType));
            release(pType);
            
            eAAFByteOrder_t byteOrderValue;
            checkResult(pStreamType->GetStoredByteOrder(pPropertyValue, &byteOrderValue));
            
            aafInt64 size;
            checkResult(pStreamType->GetSize(pPropertyValue, &size));
            
            aafUInt8 value[256];
            aafUInt32 numRead;
            checkResult(pStreamType->Read(pPropertyValue, static_cast<aafUInt32>(size), value, &numRead));

            if (byteOrderValue == testByteOrderValue &&
                size == 8 &&
                memcmp(value, testValue, (size_t)size) == 0)
            {
                printf("passed\n");
            }
            else
            {
                printf("FAILED\n");
                passed = false;
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pStreamType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Empty: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            HRESULT hr = pObject->GetPropertyValue(pPropertyDef, &pPropertyValue);
            if (hr == AAFRESULT_PROP_NOT_PRESENT)
            {
                // Note: A DataValue is represented by a OMArrayProperty (OMDataVector)
                // and when cloning the destination will get no elements appended and
                // therefore isPresent() will be false
                printf("passed (property not present)\n");
            }
            else
            {
                checkResult(hr);
                
                checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
                checkResult(pPropertyValue->GetType(&pType));
                checkResult(pType->QueryInterface(IID_IAAFTypeDefStream, (void **)&pStreamType));
                release(pType);
                
                aafBoolean_t hasByteOrderValue;
                checkResult(pStreamType->HasStoredByteOrder(pPropertyValue, &hasByteOrderValue));
                
                aafInt64 size;
                checkResult(pStreamType->GetSize(pPropertyValue, &size));
                
                if (hasByteOrderValue == kAAFFalse && size == 0)
                {
                    printf("passed\n");
                }
                else
                {
                    printf("FAILED\n");
                    passed = false;
                }
            }
        }
        catch (...)
        {
            printf("FAILED\n");
            passed = false;
        }
        release(pStreamType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);


    }
    catch (...)
    {
        passed = false;
    }

    // cleanup    
    release(pMob);
    release(pMobs);
    release(pStorage);
    release(pDictionary);
    release(pHeader);
    checkResult(pFile->Close());
    release(pFile);

    report(passed);
    
    return passed;
}


bool testStream()
{
    printf("Rule 5.11: Stream\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testStream.xml") && passed;
        
        printf("b) Roundtrip\n");
        saveCopy(L"input/testStream.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testStream.xml", L"input/tmp.xml");
        passed = testRestore(L"input/tmp.xml") && passed;
    }
    catch (...)
    {
        passed = false;
        printf("FAILED: Exception thrown\n");
    }

    printf("\n");
    
    return passed;
}


