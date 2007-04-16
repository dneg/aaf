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
        IAAFTypeDefVariableArray* pVarArrayType = 0;
        IAAFTypeDefInt* pIntType = 0;
        IAAFPropertyValue* pIntValue = 0;

        try
        {
            printf("    * UInt8: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafUInt8 value;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, &value, 1));
            localPassed = localPassed && value == 0;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, &value, 1));
            localPassed = localPassed && value == 255;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, &value, 1));
            localPassed = localPassed && value == 0xFF;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Int8: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafInt8 value;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 1));
            localPassed = localPassed && value == -128;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 1));
            localPassed = localPassed && value == 127;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 1));
            localPassed = localPassed && (aafUInt8)value == 0xFF;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * UInt16: ");

            const aafUID_t propId = 
                {0x20000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafUInt16 value;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 2));
            localPassed = localPassed && value == 0;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 2));
            localPassed = localPassed && value == 65535;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 2));
            localPassed = localPassed && value == 0xFFFF;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Int16: ");

            const aafUID_t propId = 
                {0x30000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafInt16 value;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 2));
            localPassed = localPassed && value == -32768;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 2));
            localPassed = localPassed && value == 32767;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 2));
            localPassed = localPassed && (aafUInt16)value == 0xFFFF;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * UInt32: ");

            const aafUID_t propId = 
                {0x40000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafUInt32 value;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 4));
            localPassed = localPassed && value == 0;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 4));
            localPassed = localPassed && value == 0xFFFFFFFF;//4294967295;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 4));
            localPassed = localPassed && value == 0xFFFFFFFF;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Int32: ");

            const aafUID_t propId = 
                {0x50000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafInt32 value;
            aafUInt32 maxValue;
            memset(&maxValue, 0xFF, sizeof(aafUInt32));

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 4));
            localPassed = localPassed && value == (aafInt32)(maxValue / 2 + 1);

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 4));
            localPassed = localPassed && value == (aafInt32)(maxValue / 2);

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 4));
            localPassed = localPassed && (aafUInt32)value == 0xFFFFFFFF;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * UInt64: ");

            const aafUID_t propId = 
                {0x60000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafUInt64 value;
            aafUInt64 maxValue;
            memset(&maxValue, 0xFF, sizeof(aafUInt64));

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 8));
            localPassed = localPassed && value == 0;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 8));
            localPassed = localPassed && value == maxValue;//18446744073709551615;

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 8));
            localPassed = localPassed && value == maxValue;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Int64: ");

            const aafUID_t propId = 
                {0x70000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&pVarArrayType));
            release(pType);
            
            checkResult(pVarArrayType->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));

            bool localPassed = true;
            aafInt64 value;
            aafUInt64 maxValue;
            memset(&maxValue, 0xFF, sizeof(aafUInt64));

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 0, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 8));
            localPassed = localPassed && value == (aafInt64)(maxValue / 2 + 1);

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 1, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 8));
            localPassed = localPassed && value == (aafInt64)(maxValue / 2);

            checkResult(pVarArrayType->GetElementValue(pPropertyValue, 2, &pIntValue));
            checkResult(pIntType->GetInteger(pIntValue, (aafUInt8*)&value, 8));
            localPassed = localPassed && (aafUInt64)value == maxValue;

            if (localPassed)
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
        release(pIntValue);
        release(pIntType);
        release(pVarArrayType);
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


bool testInteger()
{
    printf("Rule 5.6: Integer\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testInteger.xml") && passed;
        
        printf("b) Roundtrip\n");
        saveCopy(L"input/testInteger.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testInteger.xml", L"tmp.xml");
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


