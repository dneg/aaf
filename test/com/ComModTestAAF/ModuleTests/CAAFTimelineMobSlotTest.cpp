// @doc INTERNAL
// @com This file implements the module test for CAAFTimelineMobSlot
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x81e5b6d0, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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

static const aafRational_t	checkEditRate = { 30000, 1001 };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *				pFile = NULL;
	bool bFileOpen =		false;
	IAAFHeader *			pHeader = NULL;
	IAAFDictionary*			pDictionary = NULL;
	IAAFMob					*pMob = NULL;
	IAAFMobSlot				*newSlot = NULL;
	IAAFTimelineMobSlot		*timelineSlot = NULL;
	IAAFSegment				*seg = NULL;
	IAAFSourceClip			*sclp = NULL;
	IAAFComponent*		pComponent = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTimelineMobSlot Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		// Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		//Make the first mob
		long	test;
		// audioRate not used
		// aafRational_t	audioRate = { 44100, 1 };
		
		// Create a concrete subclass of Mob
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"MOBTest"));
		
		// Add some slots
		for(test = 0; test < 5; test++)
		{
			checkResult(defs.cdSourceClip()->
						CreateInstance(IID_IAAFSourceClip, 
									   (IUnknown **)&sclp));		
		 checkResult(sclp->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddPicture()));
		pComponent->Release();
		pComponent = NULL;
			
			checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
			
			checkResult(defs.cdTimelineMobSlot()->
						CreateInstance(IID_IAAFTimelineMobSlot, 
									   (IUnknown **)&timelineSlot));		
			checkResult(timelineSlot->SetEditRate (checkEditRate));
			checkResult(timelineSlot->SetOrigin (0));
			checkResult(timelineSlot->QueryInterface (IID_IAAFMobSlot, (void **)&newSlot));
			checkResult(newSlot->SetSegment(seg));
			checkResult(newSlot->SetSlotID(test+1));
			checkResult(newSlot->SetName(slotNames[test]));
			checkResult(pMob->AppendSlot (newSlot));
			
			timelineSlot->Release();
			timelineSlot = NULL;
			newSlot->Release();
			newSlot = NULL;
			
			seg->Release();
			seg = NULL;
			
			sclp->Release();
			sclp = NULL;
		}
		
		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (newSlot)
		newSlot->Release();
	
	if (seg)
		seg->Release();
	
	if (pComponent)
		pComponent->Release();

	if (sclp)
		sclp->Release();
	
	if (pMob)
		pMob->Release();
	
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
	IAAFFile *				pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *			pHeader = NULL;
	IEnumAAFMobs			*mobIter = NULL;
	IAAFMob					*aMob = NULL;
	IEnumAAFMobSlots		*slotIter = NULL;
	IAAFMobSlot				*slot = NULL;
	IAAFTimelineMobSlot		*timelineSlot = NULL;
	aafNumSlots_t			numMobs, n, s;
	aafPosition_t			testOrigin;
	aafRational_t			testRate;
	HRESULT					hr = S_OK;
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		
		aafSearchCrit_t		criteria;
		criteria.searchTag = kAAFNoSearch;
		checkResult(pHeader->GetMobs (&criteria, &mobIter));
		
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500], slotName[500];
			aafNumSlots_t	numSlots;
			aafMobID_t		mobID;
			aafSlotID_t		trackID;
			
			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));
			
			checkResult(aMob->CountSlots (&numSlots));
			checkExpression(5 == numSlots, AAFRESULT_TEST_FAILED);
			
			checkResult(aMob->GetSlots(&slotIter));
			

			for(s = 0; s < numSlots; s++)
			{
				checkResult(slotIter->NextOne (&slot));
				checkResult(slot->GetName (slotName, sizeof(slotName)));
				checkResult(slot->GetSlotID(&trackID));
				checkExpression (wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);
				checkResult(slot->QueryInterface(IID_IAAFTimelineMobSlot, (void **) &timelineSlot));
				checkResult(timelineSlot->GetEditRate (&testRate));
				checkExpression (testRate.numerator == checkEditRate.numerator, AAFRESULT_TEST_FAILED);
				checkExpression (testRate.denominator == checkEditRate.denominator, AAFRESULT_TEST_FAILED);
				checkResult(timelineSlot->GetOrigin (&testOrigin));
 				checkExpression (testOrigin == 0, AAFRESULT_TEST_FAILED);
				
				slot->Release();
				slot = NULL;
				timelineSlot->Release();
				timelineSlot = NULL;
			}
			
			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (slot)
		slot->Release();
	
	if (timelineSlot)
		timelineSlot->Release();
	
	if (slotIter)
		slotIter->Release();
	
	if (aMob)
		aMob->Release();
	
	if (mobIter)
		mobIter->Release();
	
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
 

extern "C" HRESULT CAAFTimelineMobSlot_test(testMode_t mode);
extern "C" HRESULT CAAFTimelineMobSlot_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFTimelineMobSlotTest.aaf";
	
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
		cerr << "CAAFTimelineMobSlot_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	// Cleanup our object if it exists.
	
	// When all of the functionality of this class is tested, we can return success
//!!!	if(hr == AAFRESULT_SUCCESS)
//	{
//		cout << "The following IAAFTimelineMobSlot methods have not been tested:" << endl; 
//		cout << "     SetEditRate" << endl; 
//		cout << "     SetOrigin" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}

