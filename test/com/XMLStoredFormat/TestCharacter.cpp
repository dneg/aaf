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
        IAAFTypeDefCharacter* pCharacterType = 0;

        // test simple
        try
        {
            printf("    * Simple: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            wchar_t testCharacter = L'A';
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&pCharacterType));
            
            aafCharacter value;
            checkResult(pCharacterType->GetCharacter(pPropertyValue, &value));

            if (value == testCharacter)
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
        release(pCharacterType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        // test XML escaped
        try
        {
            printf("    * XML escaped: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            wchar_t testCharacter = L'\r';
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&pCharacterType));
            
            aafCharacter value;
            checkResult(pCharacterType->GetCharacter(pPropertyValue, &value));

            if (value == testCharacter)
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
        release(pCharacterType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        
        // test AAF escaped
        try
        {
            printf("    * AAF escaped: ");

            const aafUID_t propId = 
                {0x20000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            wchar_t testCharacter = 0x0;
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&pCharacterType));
            
            aafCharacter value;
            checkResult(pCharacterType->GetCharacter(pPropertyValue, &value));

            if (value == testCharacter)
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
        release(pCharacterType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        // test AAF escaped of invalid UNICODE character
        try
        {
            printf("    * AAF escaped invalid Unicode character: ");

            const aafUID_t propId = 
                {0x30000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            wchar_t testCharacter = 0xFFFF;
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&pCharacterType));
            
            aafCharacter value;
            checkResult(pCharacterType->GetCharacter(pPropertyValue, &value));

            if (value == testCharacter)
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
        release(pCharacterType);
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


bool testCharacter()
{
    printf("Rule 5.1: Character\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testCharacter.xml") && passed;
        
        printf("b) Roundtrip\n");
        saveCopy(L"input/testCharacter.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testCharacter.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;
    }
    catch (...)
    {
        passed = false;
        printf("FAILED: Exception thrown\n");
    }

    printf("\n");
    
    return passed;
}


