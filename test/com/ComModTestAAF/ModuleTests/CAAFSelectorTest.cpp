// @doc INTERNAL
// @com This file implements the module test for CAAFSelector
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "AAF.h"

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>


static const	aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x5ed723c8, 0x0404, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

static const	aafMobID_t	TEST_referencedMobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x643df7b0, 0x0404, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFSelector; // generated

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



static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   // IAAFSession**		ppSession,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFSelector Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kAAFMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		if (*ppFile)
		{
			(*ppFile)->Release();
			*ppFile = NULL;
		}
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMob*			pMob = NULL;
	IAAFTimelineMobSlot*		pMobSlot = NULL;
	IAAFMob*			pReferencedMob = NULL;
	IAAFSourceClip*		pSourceClip = NULL;
	IAAFFiller*			pFiller = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFSelector*		pSelector = NULL;
	IAAFCompositionMob*	pCompMob = NULL;
	aafSourceRef_t		sourceRef; 
	aafLength_t			fillerLength = 3200;
	aafInt32			numAlternates;
	IAAFComponent*		pComponent = NULL;

	HRESULT				hr = AAFRESULT_SUCCESS;

	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
		// Create a mob to be referenced by the source clip
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pReferencedMob));
		checkResult(pReferencedMob->SetMobID(TEST_referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));
		checkResult(pHeader->AddMob(pReferencedMob));
		pReferencedMob->Release();
		pReferencedMob = NULL;

		// Create a Composition Mob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFCompositionMob, 
								   (IUnknown **)&pCompMob));

	    // get a IAAFMob interface
		checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AAFSelectorTest"));
	  
		// Create a Source clip 
 		checkResult(defs.cdSourceClip()->
					CreateInstance(IID_IAAFSourceClip, 
								   (IUnknown **)&pSourceClip));		
		 checkResult(pSourceClip->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		pComponent->Release();
		pComponent = NULL;

		// Set the properties for the SourceClip
		sourceRef.sourceID = TEST_referencedMobID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		checkResult(pSourceClip->SetSourceReference(sourceRef));

		// create a filler 
	    checkResult(defs.cdFiller()->
					CreateInstance(IID_IAAFFiller, 
								   (IUnknown **)&pFiller));
		// Set its properties.
	    checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));

		// Now create a selector 
	    checkResult(defs.cdSelector()->
					CreateInstance(IID_IAAFSelector, 
								   (IUnknown **)&pSelector));
		 checkResult(pSelector->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddkAAFPicture()));
		pComponent->Release();
		pComponent = NULL;

		// Get a segment interface from the source clip
		checkResult(pSourceClip->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		// -----------------------------------------------------------------
		// Set all properties on the Selector
		//	Set the selected segment on the Selector
		checkResult(pSelector->SetSelectedSegment(pSegment));
		// Release the intreface so we can reuse the pointer
		pSegment->Release();
		checkResult(pFiller->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
		checkResult(pSelector->AppendAlternateSegment(pSegment));
		// Release the intreface so we can reuse the pointer
		pSegment->Release();
		pSegment = NULL;
		pFiller->Release();
		pFiller = NULL;

		// create another filler, add it as an alternate, count two alternates, 
		// then delete and check for only one alternate
	    checkResult(defs.cdFiller()->
					CreateInstance(IID_IAAFFiller, 
								   (IUnknown **)&pFiller));
	    checkResult(pFiller->Initialize(defs.ddkAAFPicture(), fillerLength));
		checkResult(pFiller->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
		checkResult(pSelector->AppendAlternateSegment(pSegment));
		checkResult(pSelector->GetNumAlternateSegments (&numAlternates));
		checkExpression(2 == numAlternates, AAFRESULT_TEST_FAILED);
		checkResult(pSelector->RemoveAlternateSegment (pSegment));
		checkResult(pSelector->GetNumAlternateSegments (&numAlternates));
		checkExpression(1 == numAlternates, AAFRESULT_TEST_FAILED);
		pSegment->Release();
		pSegment = NULL;
		pFiller->Release();
		pFiller = NULL;


		checkResult(pSelector->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
	    // append the Selector to the MOB tree
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot(editRate,
												pSegment,
												1,
												L"SelectorSlot",
												0,
												&pMobSlot)); 
		
		// Add the composition mob to the file
		pHeader->AddMob(pMob);

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pMobSlot)
		pMobSlot->Release();

	if (pComponent)
		pComponent->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pFiller)
		pFiller->Release();

	if (pSegment)
		pSegment->Release();

	if (pSelector)
		pSelector->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pMob)
		pMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IEnumAAFMobs*		pMobIter = NULL;
	IAAFMob*			pMob = NULL;
	IEnumAAFMobSlots*	pSlotIter = NULL;
	IAAFMobSlot*		pSlot = NULL;
	IAAFCompositionMob*	pCompMob = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFSegment*		pSelSegment = NULL;
	IAAFSegment*		pAltSegment = NULL;
	IAAFFiller*			pFiller = NULL;
	IAAFSelector*		pSelector = NULL;
	IAAFSourceClip*		pSourceClip = NULL;
	IEnumAAFSegments*	pAlternateSegIter = NULL;

	aafNumSlots_t		numMobs;
	aafInt32			numAlternateSegs;

	aafSearchCrit_t		criteria;
	HRESULT				hr = AAFRESULT_SUCCESS;


	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));

		// Validate that there is only one composition mob.
		checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over Composition MOBs
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = kAAFCompMob;
		checkResult(pHeader->GetMobs(&criteria, &pMobIter));
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{
			aafNumSlots_t		numSlots = 0;

			checkResult(pMob->CountSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

			// Enumerate over all MOB slots for this MOB
			checkResult(pMob->GetSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				checkResult(pSlot->GetSegment(&pSegment));
				// See if it is a Selector
				checkResult(pSegment->QueryInterface(IID_IAAFSelector, (void **) &pSelector));

				// -----------------------------------------------------------
				// Get Selector Properties
				checkResult(pSelector->GetNumAlternateSegments(&numAlternateSegs));
				checkExpression(1 == numAlternateSegs, AAFRESULT_TEST_FAILED);

				checkResult(pSelector->GetSelectedSegment(&pSelSegment));
				// Make sure it is a Source clip
				checkResult(pSelSegment->QueryInterface(IID_IAAFSourceClip, (void **)&pSourceClip));
				
				// Enumerate alternates
				checkResult(pSelector->EnumAlternateSegments(&pAlternateSegIter));
				while (pAlternateSegIter && pAlternateSegIter->NextOne(&pAltSegment) == AAFRESULT_SUCCESS)
				{
					// Make sure Alternate segment is a filler
					checkResult(pAltSegment->QueryInterface(IID_IAAFFiller, (void **)&pFiller));
					pFiller->Release();
					pFiller = NULL;
				}
				pAlternateSegIter->Release();
				pAlternateSegIter = NULL;

				pSourceClip->Release();
				pSourceClip = NULL;

				pSelSegment->Release();
				pSelSegment = NULL;

				pSelector->Release();
				pSelector = NULL;

				pSegment->Release();
				pSegment = NULL;

				pSlot->Release();
				pSlot = NULL;
			}

			pSlotIter->Release();
			pSlotIter = NULL;
		}
		pMobIter->Release();
		pMobIter = NULL;
	}
	catch (HRESULT& rResult)
	{
	    hr = rResult;
	}

	// Cleanup object references
	if (pSourceClip)
		pSourceClip->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pSegment)
		pSegment->Release();

	if (pSelSegment)
		pSelSegment->Release();

	if (pAltSegment)
		pAltSegment->Release();

	if (pSlot)
		pSlot->Release();

	if (pSlotIter)
		pSlotIter->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pAlternateSegIter)
		pAlternateSegIter->Release();

	if (pFiller)
		pFiller->Release();

	if (pHeader) pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}


	return 	hr;
}

extern "C" HRESULT CAAFSelector_test(testMode_t mode);
extern "C" HRESULT CAAFSelector_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFSelectorTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
		
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFSelector_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
