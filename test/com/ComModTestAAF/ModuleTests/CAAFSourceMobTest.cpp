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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <stdio.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xb6cb63f0, 0x0404, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static aafUInt8 TEST_AIFC_Summary[] = "TEST";

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
	IAAFHeader *				pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
  IAAFEssenceDescriptor *edesc = NULL;

	HRESULT						hr = S_OK;

  try
  {
      // Remove the previous test file if any.
      RemoveTestFile(pFileName);


	  // Create the file
	  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
	  bFileOpen = true;
 
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
	  CAAFBuiltinDefs defs (pDictionary);
	 		
	  //Make the first mob
	  long			test;

	  aafRational_t	audioRate = { 44100, 1 };

	  // Create a Mob
	  checkResult(defs.cdSourceMob()->
				  CreateInstance(IID_IAAFSourceMob, 
								 (IUnknown **)&pSourceMob));

	  checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));

	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"SourceMOBTest"));
	  
	  // Add some slots
	  for(test = 0; test < 2; test++)
	  {
		  checkResult(pSourceMob->AddNilReference (test+1, 0, defs.ddkAAFSound(), audioRate));
	  }

	  // Test AppendTimecodeSlot()
	  aafRational_t videoRate = { 30000, 1001 };
	  aafTimecode_t timeCode = { 108000, kAAFTcNonDrop, 30 };
	  checkResult(pSourceMob->AppendTimecodeSlot(videoRate, 3, timeCode, 60*60*30));

	  // Create a concrete subclass of EssenceDescriptor
 	  checkResult(defs.cdAIFCDescriptor()->
				  CreateInstance(IID_IAAFEssenceDescriptor, 
								 (IUnknown **)&edesc));		

		IAAFAIFCDescriptor*			pAIFCDesc = NULL;
		checkResult(edesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		checkResult(pAIFCDesc->SetSummary (sizeof(TEST_AIFC_Summary), TEST_AIFC_Summary));
		pAIFCDesc->Release();
		pAIFCDesc = NULL;

 	  checkResult(pSourceMob->SetEssenceDescriptor (edesc));

	  checkResult(pHeader->AddMob(pMob));

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	
  if (edesc)
	  edesc->Release();

  if (pMob)
    pMob->Release();

  if (pSourceMob)
    pSourceMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob			*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot		*slot = NULL;
	aafNumSlots_t	numMobs, n, s;
	HRESULT						hr = S_OK;

	try
	{ 
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);


    checkResult(pHeader->GetMobs (NULL, &mobIter));
	  for(n = 0; n < numMobs; n++)
	  {
		  aafWChar		name[500];
		  aafNumSlots_t	numSlots;
		  aafMobID_t		mobID;
		  aafSlotID_t		trackID;

		  checkResult(mobIter->NextOne (&aMob));
		  checkResult(aMob->GetName (name, sizeof(name)));
		  checkResult(aMob->GetMobID (&mobID));

		  checkResult(aMob->CountSlots (&numSlots));
		  if (3 != numSlots)
			  return AAFRESULT_TEST_FAILED;
		  if(numSlots != 0)
		  {
			  checkResult(aMob->GetSlots(&slotIter));

			  for(s = 0; s < numSlots; s++)
			  {
				  checkResult(slotIter->NextOne (&slot));
				  checkResult(slot->GetSlotID(&trackID));

				  slot->Release();
				  slot = NULL;
			  }
		  }
		  checkResult(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
		  checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));

		  aafUInt8 summary[5];
		  IAAFAIFCDescriptor* pAIFCDesc = NULL;
		  checkResult(pEdesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		  checkResult(pAIFCDesc->GetSummary (sizeof(summary), summary));
		  checkExpression(memcmp(summary, TEST_AIFC_Summary, sizeof(TEST_AIFC_Summary)) == 0, AAFRESULT_TEST_FAILED);
		  pAIFCDesc->Release();
		  pAIFCDesc = NULL;

		  pEdesc->Release();
		  pEdesc = NULL;

		  pSourceMob->Release();
		  pSourceMob = NULL;

		  aMob->Release();
		  aMob = NULL;
    }
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  
  // Cleanup and return
  if (slot)
    slot->Release();

  if (slotIter)
    slotIter->Release();

  if (pEdesc)
    pEdesc->Release();

  if (pSourceMob)
    pSourceMob->Release();

  if (aMob)
    aMob->Release();

	if(mobIter)
		mobIter->Release();

	if (pHeader)
    pHeader->Release();

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 
extern "C" HRESULT CAAFSourceMob_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFSourceMob_test(
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
	  cerr << "CAAFSourceMob_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFSourceMob methods have not been implemented:" << endl; 
		cout << "     Initialize" << endl; 
		cout << "     AppendEdgecodeSlot" << endl; 
		cout << "     AppendPhysSourceRef - needs unit test" << endl; 
		cout << "     SpecifyValidCodeRange" << endl; 
		cout << "     NewPhysSourceRef" << endl; 
		cout << "     AddPulldownRef - needs unit test" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}
	  
	return hr;
}
