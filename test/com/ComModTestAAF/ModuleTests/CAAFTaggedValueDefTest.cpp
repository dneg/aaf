// @doc INTERNAL
// @com This file implements the module test for CAAFTaggedValueDefinition
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "ModuleTestsCommon.h"

#include <ModuleTest.h>

#include <AAF.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFStoredObjectIDs.h>

#include <iostream>
using namespace std;

#include <string.h>

static const aafUID_t TaggedValueDefTestIDs[2] = {  
  {0x27060b8e, 0xd496, 0x4790, {0x82, 0x00, 0xa1, 0xe0, 0x41, 0x94, 0xa7, 0xbb}},
  {0xb1d22937, 0xc0cb, 0x4ec0, {0x98, 0xa4, 0x30, 0x63, 0x29, 0xee, 0x22, 0x55}} };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
  try {
    using namespace mtc;

    IAAFSmartPointer<IAAFHeader> pHeader;
    IAAFSmartPointer<IAAFDictionary> pDict;
    SimpleFilePointers filePointers;
    CreateSimpleAAFFile( pFileName, 
			 L"TaggedValueDataDefTest",
			 &filePointers );

    IAAFSmartPointer<IAAFDictionary2> pDict2;
    CheckResult( filePointers.pDictionary->QueryInterface( IID_IAAFDictionary2, 
							   (void**)&pDict2 ) );

    // Create a TaggedValueDef objects and add them to the dictionary.
    IAAFSmartPointer<IAAFTaggedValueDefinition> pTagValDef;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFTaggedValueDefinition,
							   IID_IAAFTaggedValueDefinition,
							   (IUnknown**)&pTagValDef ) );
    CheckResult( pTagValDef->Initialize( TaggedValueDefTestIDs[0],
					 L"Test Tagged Val Def",
					 L"Tagged value definition used by the AAFTaggedValueDefinition module test." ) );
    CheckResult( pDict2->RegisterTaggedValueDef( pTagValDef ) );


    IAAFSmartPointer<IAAFTaggedValueDefinition> pTagValDef2;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFTaggedValueDefinition,
							   IID_IAAFTaggedValueDefinition,
							   (IUnknown**)&pTagValDef2 ) );
    CheckResult( pTagValDef2->Initialize( TaggedValueDefTestIDs[1],
					  L"Test Tagged Val Def Two",
					  L"Second Tagged value definition used by the AAFTaggedValueDefinition module test." ) );
    CheckResult( pDict2->RegisterTaggedValueDef( pTagValDef2 ) );
    
    // FIXME - When weak references to property definitions are working
    // add a reference to a tagged value added to one of the mobs in
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

    // We should find a two TaggedValueDef objects in the dictionary.
    aafUInt32 defCount = 0;
    CheckResult( pDict2->CountTaggedValueDefs( &defCount ) );
    CheckExpression( 2 == defCount, AAFRESULT_TEST_FAILED );

    IAAFSmartPointer<IEnumAAFTaggedValueDefs> pEnumDefs;
    CheckResult( pDict2->GetTaggedValueDefs( &pEnumDefs ) );
    IAAFSmartPointer<IAAFTaggedValueDefinition> pTagValDef;
    HRESULT hr;
    int enumCount;

    // The def objects are not necessarilly going to come back in in
    // the same order they were put in.  And we can't use an stl::map
    // to order them (aarrrgggg...), so we use this foolish little map
    // to be sure all are found.
    bool idFoundMap[2];
    idFoundMap[0] = false;
    idFoundMap[1] = false;

    for( enumCount = 0, hr = pEnumDefs->NextOne( &pTagValDef );
	 AAFRESULT_SUCCESS == hr;
	 enumCount++, hr = pEnumDefs->NextOne( &pTagValDef ) ) {
      
      aafUID_t id;
      IAAFSmartPointer<IAAFDefObject> pDefObj;
      CheckResult( pTagValDef->QueryInterface( IID_IAAFDefObject, (void**)&pDefObj ) );
      CheckResult( pDefObj->GetAUID( &id ) );

      if ( 0 == memcmp( &id, &TaggedValueDefTestIDs[0], sizeof(id) ) ) {
	idFoundMap[0] = true;
      }
      else if ( 0 == memcmp( &id, &TaggedValueDefTestIDs[1], sizeof(id) ) ) {
	idFoundMap[1] = true;
      }
    }

    CheckExpression( AAFRESULT_NO_MORE_OBJECTS == hr, AAFRESULT_TEST_FAILED );
    CheckExpression( 2 == enumCount, AAFRESULT_TEST_FAILED );
    CheckExpression( idFoundMap[0], AAFRESULT_TEST_FAILED );
    CheckExpression( idFoundMap[1], AAFRESULT_TEST_FAILED );

    // FIXME - When weak references to property definitions are working
    // verify the poperty definition reference that was added to the
    // tagged value definition.

    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

HRESULT TaggedValueDefinitionTest( aafCharacter* pFileName,
				   aafCharacter* pTestName,
				   testMode_t mode )
{
  HRESULT hr = AAFRESULT_SUCCESS;

  try {
    if ( kAAFUnitTestReadWrite == mode ) {
      hr = CreateAAFFile(pFileName);
    }
    else {
      hr = AAFRESULT_SUCCESS;
    }

    if ( AAFRESULT_SUCCESS == hr ) {
      hr = ReadAAFFile(pFileName);
    }
  }
  catch (...) {
    cerr << pTestName << L"...Caught general C++"
	 << " exception!" << endl; 
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}

// Required function prototype.
extern "C" HRESULT CAAFTaggedValueDefinition_test(testMode_t mode);

HRESULT CAAFTaggedValueDefinition_test(testMode_t mode)
{
  HRESULT hr = TaggedValueDefinitionTest( L"AAFTaggedValueDefTest.aaf", 
					  L"CAAFTaggedValueDef_test",
					  mode );

  if ( SUCCEEDED(hr) ) {
    cout << "The following AAFTaggedValueDataDef tests have not been implemented:" << endl;
    cout << "    {Add,Get,Count,Remove}ParentProperty" << endl;
    hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
  }

  return hr;
}
