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
        IAAFTypeDefRecord* pRecordType = 0;
        IAAFPropertyValue* pMemberPropertyValue = 0;
        IAAFTypeDefInt* pIntType = 0;
        IAAFTypeDefEnum* pEnumType = 0;

        try
        {
            printf("    * Baseline: ");

            const aafUID_t propId = 
                {0x00000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafProductVersion_t testValue = {0,1,0,0,kAAFVersionUnknown};
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafProductVersion_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafProductVersion_t)));

            if (memcmp(&testValue, &value, sizeof(aafProductVersion_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * Non-baseline: ");

            const aafUID_t propId = 
                {0x10000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUInt8 testLoginIDValue = 1;
            aafUInt8 testUserIDValue = 5;
            aafBool testIsAdminValue = kAAFFalse;
            
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));

            aafUInt8 loginIDValue = 0;
            aafUInt8 userIDValue = 0;
            aafInt64 isAdminValue = kAAFTrue;
            
            release(pType);            
            checkResult(pRecordType->GetValue(pPropertyValue, 0, &pMemberPropertyValue));
            checkResult(pMemberPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));
            checkResult(pIntType->GetInteger(pMemberPropertyValue, (aafUInt8*)&loginIDValue, 1));
            
            release(pType);            
            release(pIntType);            
            checkResult(pRecordType->GetValue(pPropertyValue, 1, &pMemberPropertyValue));
            checkResult(pMemberPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefInt, (void **)&pIntType));
            checkResult(pIntType->GetInteger(pMemberPropertyValue, (aafUInt8*)&userIDValue, 1));
            
            release(pType);            
            checkResult(pRecordType->GetValue(pPropertyValue, 2, &pMemberPropertyValue));
            checkResult(pMemberPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefEnum, (void **)&pEnumType));
            checkResult(pEnumType->GetIntegerValue(pMemberPropertyValue, &isAdminValue));
            

            if (loginIDValue == testLoginIDValue &&
                userIDValue == testUserIDValue &&
                isAdminValue == (aafInt64)testIsAdminValue)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * UUID AUID: ");

            const aafUID_t propId = 
                {0x20000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUID_t testValue =
                {0xd3ec8680,0x1e79,0x48f9,{0x99,0x41,0x0a,0x10,0xa1,0x68,0x86,0xaf}};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafUID_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafUID_t)));

            if (memcmp(&testValue, &value, sizeof(aafUID_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * UL AUID: ");

            const aafUID_t propId = 
                {0x30000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafUID_t testValue =
                {0x01030202,0x0200,0x0000,{0x06,0x0e,0x2b,0x34,0x04,0x01,0x01,0x01}};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafUID_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafUID_t)));

            if (memcmp(&testValue, &value, sizeof(aafUID_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * DateStruct: ");

            const aafUID_t propId = 
                {0x40000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafDateStruct_t testValue = {2005,01,05};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafDateStruct_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafDateStruct_t)));

            if (memcmp(&testValue, &value, sizeof(aafDateStruct_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * MobID: ");

            const aafUID_t propId = 
                {0x50000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafMobID_t testValue =
            {{0x06,0x0a,0x2b,0x34,0x01,0x01,0x01,0x01,0x01,0x01,0x0f,0x02},
                0x13,
                0x00,0x00,0x00,
                {0xf4304fc3,0x1231,0x41ed,{0xbe,0xe5,0xd2,0xde,0x30,0x67,0xf0,0xcb}}};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafMobID_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafMobID_t)));

            if (memcmp(&testValue, &value, sizeof(aafMobID_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * OMF MobID: ");

            const aafUID_t propId = 
                {0x60000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafMobID_t testValue =
            {{0x06,0x0a,0x2b,0x34,0x01,0x01,0x01,0x01,0x01,0x01,0x0f,0x00},
                0x13,
                0x00,0x00,0x00,
                {0x4108d0ec,0xda35,0x0007,{0x06,0x0e,0x2b,0x34,0x7f,0x7f,0x2a,0x80}}};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafMobID_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafMobID_t)));

            if (memcmp(&testValue, &value, sizeof(aafMobID_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * Rational: ");

            const aafUID_t propId = 
                {0x70000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafRational_t testValue = {25,1};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafRational_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafRational_t)));

            if (memcmp(&testValue, &value, sizeof(aafRational_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);

        try
        {
            printf("    * TimeStamp: ");

            const aafUID_t propId = 
                {0x80000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafTimeStamp_t testValue = {{2005,06,02},{11,40,40,00}};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafTimeStamp_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafTimeStamp_t)));

            if (memcmp(&testValue, &value, sizeof(aafTimeStamp_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * TimeStruct: ");

            const aafUID_t propId = 
                {0x90000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafTimeStruct_t testValue = {10,41,42,11};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafTimeStruct_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafTimeStruct_t)));

            if (memcmp(&testValue, &value, sizeof(aafTimeStruct_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
        release(pType);
        release(pPropertyValue);
        release(pPropertyDef);
        release(pClassDef);
        release(pObject);
        
        try
        {
            printf("    * VersionType: ");

            const aafUID_t propId = 
                {0x01000000,0x0000,0x0000,{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
            aafVersionType_t testValue = {0,9};
                
            checkResult(pMob->QueryInterface(IID_IAAFObject, (void **)&pObject));
            
            checkResult(pDictionary->LookupClassDef(kAAFClassID_Mob, &pClassDef));
            checkResult(pClassDef->LookupPropertyDef(propId, &pPropertyDef));
            
            checkResult(pObject->GetPropertyValue(pPropertyDef, &pPropertyValue));
            checkResult(pPropertyValue->GetType(&pType));
            checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
            
            aafVersionType_t value;
            checkResult(pRecordType->GetStruct(pPropertyValue, (aafUInt8*)&value, sizeof(aafVersionType_t)));

            if (memcmp(&testValue, &value, sizeof(aafVersionType_t)) == 0)
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
        release(pEnumType);
        release(pIntType);
        release(pMemberPropertyValue);
        release(pRecordType);
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


bool testRecord()
{
    printf("Rule 5.8: Record\n");
    
    bool passed = true;
    try
    {
        printf("a) Restore\n");
        passed = testRestore(L"input/testRecord.xml") && passed;
        
        printf("b) Roundtrip\n");
        saveCopy(L"input/testRecord.xml", L"tmp.xml");
        passed = testRestore(L"tmp.xml") && passed;

        printf("c) Modify\n");
        copyAndTouch(L"input/testRecord.xml", L"tmp.xml");
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


