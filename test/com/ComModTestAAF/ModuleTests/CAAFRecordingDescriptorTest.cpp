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

#include <iostream>
using namespace std;

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

    IAAFSmartPointer<IAAFSourceMob> spSourceMob = AddChainedSourceMob( &filePointers );

    // Finally... add the recording descriptor.
    IAAFSmartPointer<IAAFRecordingDescriptor> pRecDesc;
    CheckResult( filePointers.pDictionary->CreateInstance( AUID_AAFRecordingDescriptor,
							   IID_IAAFRecordingDescriptor,
							   (IUnknown**)&pRecDesc ) );
    CheckResult( pRecDesc->Initialize() );

    IAAFSmartPointer<IAAFEssenceDescriptor> pDesc;
    CheckResult( pRecDesc->QueryInterface( IID_IAAFEssenceDescriptor, (void**)&pDesc ) );
    CheckResult( spSourceMob->SetEssenceDescriptor( pDesc ) );

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

    // Get the source mob, and check that the RecordingDescriptor is
    // attached.
    IAAFSmartPointer<IAAFSourceClip> pSourceClip
      = GetSourceClipFromSlot( filePointers.pReferencedMasterMob, 1 );

    IAAFSmartPointer<IAAFMob> pMob;
    CheckResult( pSourceClip->ResolveRef( &pMob ) );
    
    IAAFSmartPointer<IAAFSourceMob> pSourceMob;
    CheckResult( pMob->QueryInterface( IID_IAAFSourceMob, (void**)&pSourceMob ) );

    IAAFSmartPointer<IAAFEssenceDescriptor> pDesc;
    CheckResult( pSourceMob->GetEssenceDescriptor( &pDesc ) );

    // Finally...
    IAAFSmartPointer<IAAFRecordingDescriptor> pRecDesc;
    CheckResult( pDesc->QueryInterface( IID_IAAFRecordingDescriptor, (void**)&pRecDesc ));

    CheckResult( filePointers.pFile->Close() );
  }
  catch( const AAFRESULT& hr ) {
    cout << "failed hr = " << hr << endl;
    return hr;
  }

  return AAFRESULT_SUCCESS;
}

// Required function prototype.
extern "C" HRESULT CAAFRecordingDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFRecordingDescriptor_test(
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
		cerr << "CAAFRecordingDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
