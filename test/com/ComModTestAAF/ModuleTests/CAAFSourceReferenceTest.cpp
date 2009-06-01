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
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc1f142c2, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}



static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFSourceReference			*pSourceReference = NULL;
	IAAFSourceReference2		*pSrcRef2 = NULL;
	aafMobID_t					outSourceID;
	aafUInt32 inMobSlotID, outMobSlotID;
	HRESULT						hr = S_OK;


	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the file.
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);

		// Create a concrete subclass of an Abstract SourceReference
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceReference, (IUnknown **)&pSourceReference));
		checkResult(pSourceReference->
					QueryInterface(IID_IAAFSourceReference2, (void **)&pSrcRef2));

		// module-specific tests go here
		//		Set Values.	
		checkResult(pSourceReference->SetSourceID( TEST_MobID));
		
		inMobSlotID = 100;   // Could have been any other value !
		checkResult(pSourceReference->SetSourceMobSlotID( inMobSlotID));

		//	Now Get Values
		checkResult(pSourceReference->GetSourceID( &outSourceID));

		// Compare value with the one we set
		checkExpression(memcmp(&TEST_MobID, &outSourceID, sizeof(TEST_MobID)) == 0, AAFRESULT_TEST_FAILED);

		checkResult(pSourceReference->GetSourceMobSlotID( &outMobSlotID));

		// Compare value with the one we set
		checkExpression(inMobSlotID == outMobSlotID, AAFRESULT_TEST_FAILED);


		// Test SetChannelIDs(), GetChannelIDsSize(), GetChannelIDs()
		aafUInt32 size;
		aafUInt32 ChannelIDs[3] = { 1234, 4567, 7890 }, *pChan = ChannelIDs;
		aafUInt32 ChannelOutIDs[3], *pOutChan = ChannelOutIDs;

		checkResult(pSrcRef2->SetChannelIDs(1, pChan));
		checkResult(pSrcRef2->GetChannelIDs(1, pOutChan));
		checkResult(pSrcRef2->GetChannelIDsSize(&size));
		checkExpression(size == (1 * sizeof(*pChan)), AAFRESULT_TEST_FAILED);
		checkExpression(pChan[0] == pOutChan[0], AAFRESULT_TEST_FAILED);

		checkResult(pSrcRef2->SetChannelIDs(3, pChan));
		checkExpression(pSrcRef2->GetChannelIDs(2, pOutChan) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		checkResult(pSrcRef2->GetChannelIDs(3, pOutChan));
		checkResult(pSrcRef2->GetChannelIDsSize(&size));
		checkExpression(size == (3 * sizeof(*pChan)), AAFRESULT_TEST_FAILED);
		checkExpression(memcmp(pChan, pOutChan, 3 * sizeof(*pChan)) == 0, AAFRESULT_TEST_FAILED);

		// Test SetMonoSourceSlotIDs(), GetMonoSourceSlotIDsSize(), GetMonoSourceSlotIDs()
		aafUInt32 MonoSourceSlotIDs[2] = { 55555, 666666 }, *pMono = MonoSourceSlotIDs;
		aafUInt32 MonoOutIDs[2], *pOutMono = MonoOutIDs;

		checkResult(pSrcRef2->SetMonoSourceSlotIDs(1, pMono));
		checkResult(pSrcRef2->GetMonoSourceSlotIDs(1, pOutMono));
		checkResult(pSrcRef2->GetMonoSourceSlotIDsSize(&size));
		checkExpression(size == (1 * sizeof(*pMono)), AAFRESULT_TEST_FAILED);
		checkExpression(pMono[0] == pOutMono[0], AAFRESULT_TEST_FAILED);

		checkResult(pSrcRef2->SetMonoSourceSlotIDs(2, pMono));
		checkExpression(pSrcRef2->GetMonoSourceSlotIDs(1, pOutMono) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		checkResult(pSrcRef2->GetMonoSourceSlotIDs(2, pOutMono));
		checkResult(pSrcRef2->GetMonoSourceSlotIDsSize(&size));
		checkExpression(size == (2 * sizeof(*pMono)), AAFRESULT_TEST_FAILED);
		checkExpression(memcmp(pMono, pOutMono, 2 * sizeof(*pChan)) == 0, AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (pSourceReference)
    pSourceReference->Release();

  if (pSrcRef2)
    pSrcRef2->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
	  {
		  pFile->Save();
		  pFile->Close();
	  }
    pFile->Release();
  }

  return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	HRESULT						hr = S_OK;


	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}

	// Cleanup object references
  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
      pFile->Close();
     pFile->Release();
  }

  return hr;
}
 

extern "C" HRESULT CAAFSourceReference_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFSourceReference_test(
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
			
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CSourceReferences_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
