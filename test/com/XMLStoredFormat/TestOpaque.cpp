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


static bool testRestore(const wchar_t* fileName)
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
        IAAFTypeDefOpaque* pOpaqueType = 0;

        try
        {
            printf("    * Baseline: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            const aafUID_t testTypeId = 
                {0x01010600,0x0000,0x0000,{0x06,0x0E,0x2B,0x34,0x01,0x04,0x01,0x01}};
            aafInt8 testValue[2] = {0x7F,00};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefOpaque, (void **)&pOpaqueType));
            
            aafUID_t actualTypeId;
            checkResult(pOpaqueType->GetActualTypeID(pPropertyValue, &actualTypeId));
            
            aafUInt8 buffer[256];
            aafUInt32 numRead;
            checkResult(pOpaqueType->GetHandle(pPropertyValue, 256, buffer, &numRead));

            aafUInt32 expectedNumRead = 1 /* opaque handle version */ + 
                4 /* 'opaq' signature */ + 
                1 /* byte order */ +
                16 /*type id*/ + 
                2 /*value*/ +
                4 /* 'opaq' signature */; 
            if (memcmp(&testTypeId, &actualTypeId, sizeof(aafUID_t)) == 0 &&
                numRead == expectedNumRead &&
                memcmp(testValue, &buffer[1 + 4 + 1 + 16], 2) == 0)
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
        release(pOpaqueType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * Unknown: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            const aafUID_t testTypeId = 
                {0x00000000,0x1000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            const char* testValue = "a string\na string";
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefOpaque, (void **)&pOpaqueType));
            
            aafUID_t actualTypeId;
            checkResult(pOpaqueType->GetActualTypeID(pPropertyValue, &actualTypeId));
            
            aafUInt8 buffer[256];
            aafUInt32 numRead;
            checkResult(pOpaqueType->GetHandle(pPropertyValue, 256, buffer, &numRead));

            aafUInt32 expectedNumRead = 1 /* opaque handle version */ + 
                4 /* 'opaq' signature */ + 
                1 /* byte order */ +
                16 /*type id*/ + 
                strlen(testValue) + 1 /*value*/ +
                4 /* 'opaq' signature */; 
            if (memcmp(&testTypeId, &actualTypeId, sizeof(aafUID_t)) == 0 &&
                numRead == expectedNumRead &&
                memcmp(testValue, &buffer[1 + 4 + 1 + 16], strlen(testValue) + 1) == 0)
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
        release(pOpaqueType);
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


int testOpaque()
{
    printf("Rule 5.7: Opaque\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testOpaque.xml") && passed;
        
        printf("b) Roundtrip\n");
        saveCopy(L"input/testOpaque.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testOpaque.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;
    }
    catch (...)
    {
        passed = false;
        printf("FAILED: Exception thrown\n");
    }

    printf("\n");
    
    return passed ? 0 : 2;
}


