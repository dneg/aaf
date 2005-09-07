//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ModuleTestsCommon.h"

#include "ModuleTest.h"

#include "AAF.h"
#include "AAFResult.h"
#include "AAFTypes.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#include <iostream>

using namespace std;

static const aafUID_t KLVDataDefTestIDs[2] = {  
  {0xde7a80eb, 0x0bf3, 0x41c4, {0xb8, 0x25, 0x36, 0xf2, 0x3d, 0xca, 0x95, 0xcb}},
  {0xa47cf6fd, 0x8dbe, 0x4b64, {0x81, 0xa4, 0x5e, 0x2b, 0x1f, 0xd8, 0x5b, 0x56}}
};

static const aafUID_t* KLVTypeDefTestIDs[2] = {
  &kAAFTypeID_UInt32,
  &kAAFTypeID_String
};

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  try {
    using namespace mtc;

    IAAFSmartPointer<IAAFHeader> pHeader;
    IAAFSmartPointer<IAAFDictionary> pDict;
    SimpleFilePointers filePointers;
    CreateSimpleAAFFile( pFileName,
			 fileKind,
			 rawStorageType,
			 productID,
			 &filePointers );


    IAAFSmartPointer<IAAFDictionary2> pDict2;
    CheckResult( filePointers.pDictionary->QueryInterface( IID_IAAFDictionary2, 
							   (void**)&pDict2 ) );



    // Create KLVDataDef objects and add them to the dictionary.

    IAAFSmartPointer<IAAFKLVDataDefinition> pKLVDataDef;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFKLVDataDefinition,
							   IID_IAAFKLVDataDefinition,
							   (IUnknown**)&pKLVDataDef ) );
    CheckResult( pKLVDataDef->Initialize( KLVDataDefTestIDs[0],
					 L"Test KLVData Def",
					 L"KLVData definition used by the AAFKLVDataDefinition module test." ) );
    CheckResult( pDict2->RegisterKLVDataDef( pKLVDataDef ) );

    IAAFSmartPointer<IAAFKLVDataDefinition> pKLVDataDef2;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFKLVDataDefinition,
							   IID_IAAFKLVDataDefinition,
							   (IUnknown**)&pKLVDataDef2 ) );
    CheckResult( pKLVDataDef2->Initialize( KLVDataDefTestIDs[1],
					 L"Test KLVData Def Two",
					 L"Second KLVData definition used by the AAFKLVDataDefinition module test." ) );
    CheckResult( pDict2->RegisterKLVDataDef( pKLVDataDef2 ) );

    
    // Add type def references to the data defs.
    IAAFSmartPointer<IAAFTypeDef> pTypeDef1;
    CheckResult( filePointers.pDictionary->LookupTypeDef( *KLVTypeDefTestIDs[0], &pTypeDef1 ) );
    CheckResult( pKLVDataDef->SetKLVDataType( pTypeDef1 ) );

    IAAFSmartPointer<IAAFTypeDef> pTypeDef2;
    CheckResult( filePointers.pDictionary->LookupTypeDef( *KLVTypeDefTestIDs[1], &pTypeDef2 ) );
    CheckResult( pKLVDataDef2->SetKLVDataType( pTypeDef2 ) );


    // FIXME - When weak references to property definitions are working
    // add a reference to a KLVData object added to one of the mobs in
    // the test aaf file.

    CheckResult( filePointers.pFile->Save() );
    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  try {
    using namespace mtc;

    SimpleFilePointers filePointers;
    ReadSimpleAAFFile( pFileName, &filePointers );

    IAAFSmartPointer<IAAFDictionary2> pDict2;
    CheckResult( filePointers.pDictionary->QueryInterface( IID_IAAFDictionary2, 
							   (void**)&pDict2 ) );

    aafUInt32 defCount = 0;
    CheckResult( pDict2->CountKLVDataDefs( &defCount ) );
    CheckExpression( 2 == defCount, AAFRESULT_TEST_FAILED );

    IAAFSmartPointer<IEnumAAFKLVDataDefs> pEnumDefs;
    CheckResult( pDict2->GetKLVDataDefs( &pEnumDefs ) );
    IAAFSmartPointer<IAAFKLVDataDefinition> pKLVDataDef;
    HRESULT hr;
    int enumCount;

    bool idFoundMap[2];
    idFoundMap[0] = false;
    idFoundMap[1] = false;

    aafUID_t typeIDs[2];

    for( enumCount = 0, hr = pEnumDefs->NextOne( &pKLVDataDef );
	 AAFRESULT_SUCCESS == hr;
	 enumCount++, hr = pEnumDefs->NextOne( &pKLVDataDef ) ) {
      
      aafUID_t id;
      IAAFSmartPointer<IAAFDefObject> pDefObj;
      CheckResult( pKLVDataDef->QueryInterface( IID_IAAFDefObject, (void**)&pDefObj ) );
      CheckResult( pDefObj->GetAUID( &id ) );

      IAAFSmartPointer<IAAFTypeDef> pTypeDef;
      CheckResult( pKLVDataDef->GetKLVDataType( &pTypeDef ) );
      IAAFSmartPointer<IAAFMetaDefinition> pTypeMetaDef;
      CheckResult( pTypeDef->QueryInterface( IID_IAAFMetaDefinition, (void**)&pTypeMetaDef ) );
      aafUID_t typeID;
      CheckResult( pTypeMetaDef->GetAUID( &typeID ) );

      if ( 0 == memcmp( &id, &KLVDataDefTestIDs[0], sizeof(id) ) ) {
	idFoundMap[0] = true;
	typeIDs[0] = typeID;
      }
      else if ( 0 == memcmp( &id, &KLVDataDefTestIDs[1], sizeof(id) ) ) {
	idFoundMap[1] = true;
	typeIDs[1] = typeID;
      }
    }

    CheckExpression( AAFRESULT_NO_MORE_OBJECTS == hr, AAFRESULT_TEST_FAILED );
    CheckExpression( 2 == enumCount, AAFRESULT_TEST_FAILED );
    CheckExpression( idFoundMap[0], AAFRESULT_TEST_FAILED );
    CheckExpression( idFoundMap[1], AAFRESULT_TEST_FAILED );
    CheckExpression( 0 == memcmp( &typeIDs[0], KLVTypeDefTestIDs[0], sizeof(typeIDs[0]) ), AAFRESULT_TEST_FAILED );
    CheckExpression( 0 == memcmp( &typeIDs[1], KLVTypeDefTestIDs[1], sizeof(typeIDs[1]) ), AAFRESULT_TEST_FAILED );

    // FIXME - When weak references to property definitions are working
    // verify the poperty definition reference that was added to the
    // KLVData definition.


    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

// Required function prototype.
extern "C" HRESULT CAAFKLVDataDefinition_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFKLVDataDefinition_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
		
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFKLVDataDefinition_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	if ( SUCCEEDED(hr) )
	{
		cout << "The following AAFKLVDataDef tests have not been implemented:" << endl;
		cout << "    {Add,Get,Count,Remove}ParentProperty" << endl;
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}
