// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceGroup
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFUtils.h"

#include "CAAFBuiltinDefs.h"

static aafWChar *slotName = L"SLOT1";
//static aafInt32 fadeInLen  = 1000;
//static aafInt32 fadeOutLen = 2000;
//static aafFadeType_t fadeInType = kAAFFadeLinearAmp;
//static aafFadeType_t fadeOutType = kAAFFadeLinearPower;
//!!!static aafSourceRef_t sourceRef; 



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

#define SUBSEG_LENGTH	60

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession*				pSession = NULL;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IAAFTimelineMobSlot*		newSlot = NULL;
	IAAFEssenceGroup*			essenceGroup = NULL;
	IAAFSegment*				pSeg = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFComponent*				pComponent = NULL;
	bool bFileOpen = false;
	aafProductIdentification_t	ProductInfo;
	aafMobID_t					newMobID, referencedMobID;
	aafLength_t					stillLength = 1, segLen = SUBSEG_LENGTH;
	AAFRESULT					hr = AAFRESULT_SUCCESS;
	aafUInt32					numChoices;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFEssenceGroup Test";
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
		checkResult(CoCreateGuid((GUID *)&referencedMobID));
		checkResult(pReferencedMob->SetMobID(referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));

		// Create a Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(newMobID));
		checkResult(pMob->SetName(L"AAFSourceClipTest"));

		// Create an EssenceGroup
		checkResult(defs.cdEssenceGroup()->
					CreateInstance(IID_IAAFEssenceGroup, 
								   (IUnknown **)&essenceGroup));
		checkResult(essenceGroup->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
		checkResult(pComponent->SetDataDef (defs.ddSound()));
		checkResult(pComponent->SetLength (segLen));
		pComponent->Release();
		pComponent = NULL;
		// Add a source clip still frame
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceClip, 
								   (IUnknown **)&pSourceClip));
		checkResult(pSourceClip->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
		checkResult(pComponent->SetDataDef (defs.ddSound()));
		checkResult(pComponent->SetLength (stillLength));
		checkResult(essenceGroup->SetStillFrame(pSourceClip)); 
		pComponent->Release();
		pComponent = NULL;
		pSourceClip->Release();
		pSourceClip = NULL;
		// Add a source clip alternate
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceClip, 
								   (IUnknown **)&pSourceClip));
		checkResult(pSourceClip->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
		checkResult(pComponent->SetDataDef (defs.ddSound()));
		checkResult(pComponent->SetLength (segLen));
		checkResult(pSourceClip->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		checkResult(essenceGroup->AppendChoice(pSegment)); 
		pSegment->Release();
		pSegment = NULL;
		pComponent->Release();
		pComponent = NULL;
		pSourceClip->Release();
		pSourceClip = NULL;
		checkResult(essenceGroup->CountChoices(&numChoices));
		checkExpression(1 == numChoices, AAFRESULT_TEST_FAILED);
		// Add another source clip alternate
		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceClip, 
								   (IUnknown **)&pSourceClip));
		checkResult(pSourceClip->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
		checkResult(pComponent->SetDataDef (defs.ddSound()));
		checkResult(pComponent->SetLength (segLen));
		checkResult(pSourceClip->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		checkResult(essenceGroup->AppendChoice(pSegment)); 
		pSegment->Release();
		pSegment = NULL;
		pComponent->Release();
		pComponent = NULL;
		pSourceClip->Release();
		pSourceClip = NULL;
		checkResult(essenceGroup->CountChoices(&numChoices));
		checkExpression(2 == numChoices, AAFRESULT_TEST_FAILED);
		// Remove the second alternate, check for count 1 once again
		checkResult(essenceGroup->RemoveChoiceAt(1));
		checkResult(essenceGroup->CountChoices(&numChoices));
		checkExpression(1 == numChoices, AAFRESULT_TEST_FAILED);

		checkResult(essenceGroup->QueryInterface (IID_IAAFSegment, (void **)&pSeg));
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot (editRate,
												 pSeg,
												 1,
												 slotName,
												 0,
												 &newSlot));
		pSeg->Release();
		pSeg = NULL;

		checkResult(pHeader->AddMob(pMob));
		checkResult(pHeader->AddMob(pReferencedMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  // Cleanup and return
  if (pSourceClip)
	{
	  pSourceClip->Release();
	  pSourceClip = 0;
	}

  if (pSeg)
	{
	  pSeg->Release();
	  pSeg = 0;
	}

  if (pComponent)
	{
	  pComponent->Release();
	  pComponent = 0;
	}

  if (newSlot)
	{
	  newSlot->Release();
	  newSlot = 0;
	}

  if (essenceGroup)
	{
	  essenceGroup->Release();
	  essenceGroup = 0;
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
	IAAFEssenceGroup*			pEssenceGroup = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	IAAFComponent*				pComponent = NULL;
	aafLength_t					readLength;
	bool bFileOpen = false;
	aafProductIdentification_t	ProductInfo;
	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	aafUInt32					readNumChoices;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"AAFEssenceGroup Test. NOT!";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;

	try
	{ 
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be two)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(2 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

			checkResult(pMob->GetSlots(&pSlotIter));
			while (AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
			{
				// The segment should be a source clip...
				checkResult(pSlot->GetSegment(&pSegment));
				checkResult(pSegment->QueryInterface (IID_IAAFEssenceGroup, (void **)&pEssenceGroup));
				pSegment->Release();
				pSegment = NULL;
				/***/
				checkResult(pEssenceGroup->GetStillFrame(&pSourceClip));
				checkResult(pSourceClip->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
				checkResult(pComponent->GetLength(&readLength));
				checkExpression(1 == readLength, AAFRESULT_TEST_FAILED);
				pSourceClip->Release();
				pSourceClip = NULL;
				pComponent->Release();
				pComponent = NULL;
				/***/
				checkResult(pEssenceGroup->CountChoices(&readNumChoices));
				checkExpression(1 == readNumChoices, AAFRESULT_TEST_FAILED);
				/***/
				checkResult(pEssenceGroup->GetChoiceAt (0, &pSegment));
				checkResult(pSegment->QueryInterface (IID_IAAFComponent, (void **)&pComponent));
				checkResult(pComponent->GetLength(&readLength));
				checkExpression(SUBSEG_LENGTH == readLength, AAFRESULT_TEST_FAILED);
				pSegment->Release();
				pSegment = NULL;
				pComponent->Release();
				pComponent = NULL;

				pEssenceGroup->Release();
				pEssenceGroup = NULL;
				pSlot->Release();
				pSlot = NULL;
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
		pReferencedMob->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pComponent)
		pComponent->Release();

	if (pEssenceGroup)
		pEssenceGroup->Release();

	if (pSlot)
		pSlot->Release();

	if (pSlotIter)
		pSlotIter->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

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
 


extern "C" HRESULT CAAFEssenceGroup_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"AAFEssenceGroupTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFEssenceGroup_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following AAFEssenceGroup tests have not been implemented:" << endl; 
//		cout << "     GetStillFrame" << endl; 
//		cout << "     GetNumChoices" << endl; 
//		cout << "     GetIndexedChoice" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
