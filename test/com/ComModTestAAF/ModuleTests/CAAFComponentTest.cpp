// @doc INTERNAL
// @com This file implements the module test for CAAFSegment
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafWChar *slotName = L"SLOT1";

static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xb9768af0, 0x03fd, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

static const	aafMobID_t	TEST_referencedMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xc2fff2f0, 0x03fd, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

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

#define TEST_DDEF	DDEF_Sound
#define TEST_LENGTH	42L

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession*				pSession = NULL;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IAAFTimelineMobSlot*		newSlot = NULL;
	IAAFComponent*				comp = NULL;
	IAAFSegment*				seg = NULL;
	IAAFDataDef *               pDataDef = 0;
	aafLength_t					testLength = TEST_LENGTH;
	bool bFileOpen = false;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					dataDef = TEST_DDEF;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFComponent Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	try
	{
	    // Remove the previous test file if any.
	    RemoveTestFile(pFileName);

		// Create the file
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
 		
		CAAFBuiltinDefs defs (pDictionary);

		//Make the MOB to be referenced
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pReferencedMob));
		checkResult(pReferencedMob->SetMobID(TEST_referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));

		// Create a Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob,
								   (IUnknown **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFSourceClipTest"));

		// Create a SourceClip
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSegment, 
								   (IUnknown **)&seg));
		checkResult(seg->QueryInterface (IID_IAAFComponent,
                                          (void **)&comp));
		checkResult(pDictionary->LookupDataDef(dataDef, &pDataDef));
		checkResult(comp->SetDataDef (pDataDef));
		pDataDef->Release ();
		pDataDef = 0;
		checkResult(comp->SetLength (testLength));
		comp->Release();
		comp = NULL;
								 		
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot (editRate,
												 seg,
												 1,
												 slotName,
												 0,
												 &newSlot));

		checkResult(pHeader->AddMob(pMob));
		checkResult(pHeader->AddMob(pReferencedMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  // Cleanup and return
	if (newSlot)
	  {
		newSlot->Release();
		newSlot = 0;
	  }

	if (seg)
	  {
		seg->Release();
		seg = 0;
	  }

	if (comp)
	  {
		comp->Release();
		comp = 0;
	  }

	if (pMob)
	  {
		pMob->Release();
		pMob = 0;
	  }

	if (pReferencedMob)
	  {
		pReferencedMob->Release();
		pReferencedMob = 0;
	  }

	if (pDictionary)
	  {
		pDictionary->Release();
		pDictionary = 0;
	  }

	if (pHeader)
	  {
		pHeader->Release();
		pHeader = 0;
	  }

	if (pDataDef)
	  {
		pDataDef->Release ();
		pDataDef = 0;
	  }

	if (pFile) 
	{
		if (bFileOpen)
		  {
			pFile->Save();
			pFile->Close();
		  }
		pFile->Release();
		pFile = 0;
	}


	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFComponent*				comp = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFDataDef *               pDataDef = 0;
	IAAFDefObject *             pDefObj = 0;
	aafLength_t					testLength;
	aafUID_t					testUID, checkUID = TEST_DDEF;
	bool bFileOpen = false;
	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	HRESULT						hr = AAFRESULT_SUCCESS;

	try
	{ 
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(2 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			aafMobID_t	debugMobID;

			checkResult(pMob->GetMobID(&debugMobID));
			checkResult(pMob->CountSlots(&numSlots));
			if(0 != numSlots)	// numSlots == 0 on referenced mob
			{
				checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);
				
				checkResult(pMob->GetSlots(&pSlotIter));
				while (AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
				{
					// The segment should be a source clip...
					checkResult(pSlot->GetSegment(&pSegment));
					checkResult(pSegment->QueryInterface (IID_IAAFComponent,
						(void **)&comp));
					checkResult(comp->GetDataDef (&pDataDef));
					checkResult(pDataDef->QueryInterface (IID_IAAFDefObject,
						(void **)&pDefObj));
					checkResult(pDefObj->GetAUID (&testUID));
					pDataDef->Release ();
					pDataDef = 0;
					pDefObj->Release ();
					pDefObj = 0;
					checkExpression(memcmp(&testUID, &checkUID, sizeof(testUID)) == 0, AAFRESULT_TEST_FAILED);
					checkResult(comp->GetLength (&testLength));
					checkExpression(TEST_LENGTH == testLength, AAFRESULT_TEST_FAILED);
					comp->Release();
					comp = NULL;
					pSegment->Release();
					pSegment = NULL;
					
					pSlot->Release();
					pSlot = NULL;
				}
			}

			pMob->Release();
			pMob = NULL;
		}

	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

	// Cleanup and return
	if (pReferencedMob)
	  {
		pReferencedMob->Release();
		pReferencedMob = 0;
	  }

	if (pSourceClip)
	  {
		pSourceClip->Release();
		pSourceClip = 0;
	  }

	if (comp)
	  {
		comp->Release();
		comp = 0;
	  }

	if (pSegment)
	  {
		pSegment->Release();
		pSegment = 0;
	  }

	if (pSlot)
	  {
		pSlot->Release();
		pSlot = 0;
	  }

	if (pSlotIter)
	  {
		pSlotIter->Release();
		pSlotIter = 0;
	  }

	if (pMob)
	  {
		pMob->Release();
		pMob = 0;
	  }

	if (pMobIter)
	  {
		pMobIter->Release();
		pMobIter = 0;
	  }

	if (pHeader)
	  {
		pHeader->Release();
		pHeader = 0;
	  }

	if (pDataDef)
	  {
		pDataDef->Release ();
		pDataDef = 0;
	  }

	if (pDefObj)
	  {
		pDefObj->Release ();
		pDefObj = 0;
	  }

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
		pFile = 0;
	}

	return hr;
}
 


extern "C" HRESULT CAAFComponent_test(testMode_t mode);
extern "C" HRESULT CAAFComponent_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"AAFComponentTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
			
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFComponent_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


	return hr;
}






















