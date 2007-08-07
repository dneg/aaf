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
        IAAFTypeDefExtEnum* pExtEnumType = 0;

        // test baseline ext. enum
        try
        {
            printf("    * Baseline: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUID_t testValue = 
                {0x0D010102,0x0101,0x0500,{0x06,0x0E,0x2B,0x34,0x04,0x01,0x01,0x01}};
            const wchar_t* testName = L"Usage_SubClip";
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefExtEnum, (void **)&pExtEnumType));
            
            aafUID_t value;
            aafCharacter name[256];
            checkResult(pExtEnumType->GetAUIDValue(pPropertyValue, &value));
            checkResult(pExtEnumType->GetNameFromValue(pPropertyValue, name, 256));

            if (memcmp(&value, &testValue, sizeof(aafUID_t)) ==0 && wcscmp(name, testName) == 0)
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
        release(pExtEnumType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        // test Int8 enum
        try
        {
            printf("    * Non-baseline Ext Enum: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUID_t testValue = 
                {0x6f9685a4,0x0a0d,0x447b,{0x89,0xf3,0x1b,0x75,0x0b,0xc5,0xc7,0xde}};
            const wchar_t* testName = L"AAA";
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefExtEnum, (void **)&pExtEnumType));
            
            aafUID_t value;
            aafCharacter name[256];
            checkResult(pExtEnumType->GetAUIDValue(pPropertyValue, &value));
            checkResult(pExtEnumType->GetNameFromValue(pPropertyValue, name, 256));

            if (memcmp(&value, &testValue, sizeof(aafUID_t)) ==0 && wcscmp(name, testName) == 0)
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
        release(pExtEnumType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        // test baseline extension
        try
        {
            printf("    * Baseline extension: ");

            const aafUID_t propId = 
                {0x20000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUID_t testValue = 
                {0x197e78ac,0xb464,0x4bd1,{0x86,0x11,0xb6,0xf2,0xdf,0x0d,0x03,0xfe}};
            const wchar_t* testName = L"MyUsage";
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefExtEnum, (void **)&pExtEnumType));
            
            aafUID_t value;
            aafCharacter name[256];
            checkResult(pExtEnumType->GetAUIDValue(pPropertyValue, &value));
            checkResult(pExtEnumType->GetNameFromValue(pPropertyValue, name, 256));

            if (memcmp(&value, &testValue, sizeof(aafUID_t)) ==0 && wcscmp(name, testName) == 0)
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
        release(pExtEnumType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        // test non-baseline extension
        try
        {
            printf("    * Non-baseline extension: ");

            const aafUID_t propId = 
                {0x30000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUID_t testValue = 
                {0xe54f0efa,0x15c1,0x468f,{0xb7,0x43,0x9b,0xfd,0x12,0x94,0x04,0x49}};
            const wchar_t* testName = L"CCC";
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefExtEnum, (void **)&pExtEnumType));
            
            aafUID_t value;
            aafCharacter name[256];
            checkResult(pExtEnumType->GetAUIDValue(pPropertyValue, &value));
            checkResult(pExtEnumType->GetNameFromValue(pPropertyValue, name, 256));

            if (memcmp(&value, &testValue, sizeof(aafUID_t)) ==0 && wcscmp(name, testName) == 0)
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
        release(pExtEnumType);
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


int testExtEnum()
{
    printf("Rule 5.3: Extendible Enumeration\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testExtEnum.xml") && passed;
        
        printf("b) Roundtrip\n");
        printf("FAILED (not tested): Ext enums are not cloned properly - fix due for v1.2\n");
        //saveCopy(L"input/testExtEnum.xml", L"tmp.xml");
        //passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testExtEnum.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;
    }
    catch (...)
    {
        passed = false;
        printf("FAILED: Exception thrown\n");
    }

    printf("\n");
    
    return passed ? 1 : 2; // 1 -> known failures
}


