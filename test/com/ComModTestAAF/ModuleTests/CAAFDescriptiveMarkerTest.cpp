// @doc INTERNAL
// @com This file implements the module test for CAAFDescriptiveMarker
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

#include <string.h>

using namespace std;

const aafUID_t TestDesciptiveFrameworkClassID = 
{ 0x023a1cae, 0xdc16, 0x4db9, { 0x95, 0xf9, 0x43, 0xd0, 0x56, 0xca, 0xd3, 0x78 } };

static void RegisterDescriptiveTestFramework( IAAFSmartPointer<IAAFDictionary>& pDict )
{
  using namespace mtc;

  // Create a new class definition for the TestDescriptiveFramework.

  IAAFSmartPointer<IAAFClassDef> pClassDef;
  CheckResult( pDict->CreateMetaInstance( AUID_AAFClassDef, IID_IAAFClassDef,
					  (IUnknown**)&pClassDef ) );

  IAAFSmartPointer<IAAFClassDef> pBaseClassDef;
  CheckResult( pDict->LookupClassDef( AUID_AAFDescriptiveFramework, &pBaseClassDef ) );

  CheckResult( pClassDef->Initialize( TestDesciptiveFrameworkClassID,
				      pBaseClassDef,
				      L"TestDescriptiveFramework",
				      kAAFTrue ) );

  CheckResult( pDict->RegisterClassDef( pClassDef ) );
}


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
  try {
    using namespace mtc;

    IAAFSmartPointer<IAAFHeader> pHeader;
    IAAFSmartPointer<IAAFDictionary> pDict;
    SimpleFilePointers filePointers;
    CreateSimpleAAFFile( pFileName, 
			 L"DescriptiveMarker",
			 &filePointers );

    // Add add a timeline, and add a DescriptiveMarker to the
    // composition mob.
    IAAFSmartPointer<IAAFDescriptiveMarker> pDescMarker;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFDescriptiveMarker,
							   IID_IAAFDescriptiveMarker,
							   (IUnknown**)&pDescMarker ));

    CheckResult( pDescMarker->Initialize() );

    IAAFSmartPointer<IAAFSegment> pSeg;
    CheckResult(pDescMarker->QueryInterface( IID_IAAFSegment, (void**)&pSeg ));

    IAAFSmartPointer<IAAFComponent> pComp;
    CheckResult( pDescMarker->QueryInterface( IID_IAAFComponent, (void**)&pComp ));
    CheckResult( pComp->SetDataDef( filePointers.pDataDef ));
	
    IAAFSmartPointer<IAAFTimelineMobSlot> pNewSlot;
    CheckResult( filePointers.pCompositionMob->AppendNewTimelineSlot(TEST_EditRate,
								     pSeg,
								     2,
								     L"Descriptive Content",
								     0,
								     &pNewSlot ));

    // Attach a (concrete) descriptive framework object to the marker.

    RegisterDescriptiveTestFramework( filePointers.pDictionary );

    IAAFSmartPointer<IAAFDescriptiveFramework> pDescFramework;
    CheckResult( filePointers.pDictionary->CreateInstance( TestDesciptiveFrameworkClassID,
							   IID_IAAFDescriptiveFramework,
							   (IUnknown**)&pDescFramework ) );
    CheckResult( pDescMarker->SetDescriptiveFramework( pDescFramework ) );


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

    // Get slot 2 from the composition and verify that that attached
    // segment is a DescriptiveMarker.
    
    IAAFSmartPointer<IAAFMobSlot> pSlot = GetSlotById( filePointers.pCompositionMob, 2 );

    IAAFSmartPointer<IAAFSegment> pSeg;
    CheckResult( pSlot->GetSegment( &pSeg ) );

    IAAFSmartPointer<IAAFDescriptiveMarker> pDescMarker;
    CheckResult( pSeg->QueryInterface( IID_IAAFDescriptiveMarker, (void**)&pDescMarker ) );
    
    IAAFSmartPointer<IAAFDescriptiveFramework> pDescFramework;
    CheckResult( pDescMarker->GetDescriptiveFramework( &pDescFramework ) );

    IAAFSmartPointer<IAAFObject> pDescFrameworkAsObj;
    CheckResult( pDescFramework->QueryInterface( IID_IAAFObject, (void**)&pDescFrameworkAsObj ) );
    
    IAAFSmartPointer<IAAFClassDef> pClassDef;
    CheckResult( pDescFrameworkAsObj->GetDefinition( &pClassDef ) );

    IAAFSmartPointer<IAAFMetaDefinition> pMetaDef;
    CheckResult( pClassDef->QueryInterface( IID_IAAFMetaDefinition, (void**)&pMetaDef ) );

    aafUID_t auid;

    CheckResult( pMetaDef->GetAUID( &auid ) );

    CheckExpression( memcmp( &auid, &TestDesciptiveFrameworkClassID, sizeof(auid) ) == 0,
		     AAFRESULT_TEST_FAILED );

    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}


HRESULT DescriptiveMarkerAndFrameworkTest( aafCharacter* pFileName,
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
extern "C" HRESULT CAAFDescriptiveMarker_test(testMode_t mode);
HRESULT CAAFDescriptiveMarker_test(testMode_t mode)
{
  return DescriptiveMarkerAndFrameworkTest( L"AAFDescriptiveMarkerTest.aaf",
					    L"CAAFDescriptiveMarker_test",
					    mode );
}
