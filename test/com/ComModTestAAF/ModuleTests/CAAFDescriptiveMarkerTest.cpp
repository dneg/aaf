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
using namespace std;

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


    // FIXME - test framework set here.
#if 0
    IAAFSmartPointer<IAAFDescriptiveFramework> pDescFramework;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFDescriptiveFramework,
							   IID_IAAFDescriptiveFramework,
							   (IUnknown**)&pDescFramework ) );
    CheckResult( pDescMarker->SetDescriptiveFramework( pDescFramework ) );
#endif

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
    
    // FIXME - Test framework get here.

    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

// Required function prototype.
extern "C" HRESULT CAAFDescriptiveMarker_test(testMode_t mode);
HRESULT CAAFDescriptiveMarker_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  aafCharacter* pFileName = L"AAFDescriptiveMarkerTest.aaf";

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
    cerr << "CAAFDescriptiveMarker_test...Caught general C++"
	 << " exception!" << endl; 
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}
