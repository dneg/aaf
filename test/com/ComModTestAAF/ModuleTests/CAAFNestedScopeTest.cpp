// @doc INTERNAL
// @com This file implements the module test for CAAFNestedScope
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



#include "AAF.h"

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"


#include <iostream.h>
#include <stdio.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFNestedScope; // generated

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

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFNestedScope Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kMediaOpenAppend:
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
	IAAFMobSlot*		pMobSlot = NULL;
	IAAFMob*			pReferencedMob = NULL;
	IAAFSourceClip*		pSourceClip = NULL;
	IAAFFiller*			pFiller = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFNestedScope*		pNestedScope = NULL;
	IAAFCompositionMob*	pCompMob = NULL;
	aafUID_t			NewMobID, referencedMobID;
	aafInt32			fadeInLen  = 1000;
	aafInt32			fadeOutLen = 2000;
	aafFadeType_t		fadeInType = kFadeLinearAmp;
	aafFadeType_t		fadeOutType = kFadeLinearPower;
	aafSourceRef_t		sourceRef; 
	aafUID_t			fillerUID = DDEF_Picture;
	aafLength_t			fillerLength = 3200;

	HRESULT				hr = AAFRESULT_SUCCESS;

	try
	{  
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
 		
		// Create a mob to be referenced by the source clip
		checkResult(pDictionary->CreateInstance(&AUID_AAFMasterMob,
								 IID_IAAFMob, 
								 (IUnknown **)&pReferencedMob));
		checkResult(CoCreateGuid((GUID *)&referencedMobID));
		checkResult(pReferencedMob->SetMobID(&referencedMobID));
		checkResult(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));
		checkResult(pHeader->AppendMob(pReferencedMob));
		pReferencedMob->Release();
		pReferencedMob = NULL;

		// Create a Composition Mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
											  IID_IAAFCompositionMob, 
											  (IUnknown **)&pCompMob));

	    // get a IAAFMob interface
		checkResult(pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&NewMobID));
		checkResult(pMob->SetMobID(&NewMobID));
		checkResult(pMob->SetName(L"AAFNestedScopeTest"));
	  
		// Create a Source clip 
 		checkResult(pDictionary->CreateInstance(&AUID_AAFSourceClip,
						     IID_IAAFSourceClip, 
						     (IUnknown **)&pSourceClip));		

		// Set the properties for the SourceClip
		checkResult(pSourceClip->SetFade( fadeInLen, fadeInType, fadeOutLen, fadeOutType));
		sourceRef.sourceID = referencedMobID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		checkResult(pSourceClip->SetSourceReference(sourceRef));

		// create a filler 
	    checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
												IID_IAAFFiller, 
												(IUnknown **)&pFiller));
		// Set its properties.
	    checkResult(pFiller->Initialize( &fillerUID, fillerLength));

		// Now create a selector 
	    checkResult(pDictionary->CreateInstance(&AUID_AAFNestedScope,
												IID_IAAFNestedScope, 
												(IUnknown **)&pNestedScope));

		// Get a segment interface from the source clip
		checkResult(pSourceClip->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		// -----------------------------------------------------------------
		// Set all properties on the Selector
		//	Set the selected segment on the Selector
		checkResult(pNestedScope->AppendSegment(pSegment));
		// Release the intreface so we can reuse the pointer
		pSegment->Release();
		pSegment = NULL;
		checkResult(pFiller->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
		checkResult(pNestedScope->AppendSegment(pSegment));
		// Release the intreface so we can reuse the pointer
		pSegment->Release();
		pSegment = NULL;
		checkResult(pNestedScope->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
	    // append the Selector to the MOB tree
		checkResult(pMob->AppendNewSlot(pSegment, 1, L"SelectorSlot", &pMobSlot)); 
		
		// Add the composition mob to the file
		pHeader->AppendMob(pMob);

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pMobSlot)
		pMobSlot->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pFiller)
		pFiller->Release();

	if (pSegment)
		pSegment->Release();

	if (pNestedScope)
		pNestedScope->Release();

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
	IAAFFiller*			pFiller = NULL;
	IAAFNestedScope*		pNestedScope = NULL;
	IAAFSourceClip*		pSourceClip = NULL;
	IEnumAAFSegments*	pSegIter = NULL;

	aafNumSlots_t		numMobs;

	aafSearchCrit_t		criteria;
	HRESULT				hr = AAFRESULT_SUCCESS;


	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));

		// Validate that there is only one composition mob.
		checkResult(pHeader->GetNumMobs(kCompMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		// Enumerate over Composition MOBs
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kCompMob;
		checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{
			aafNumSlots_t		numSlots = 0;

			checkResult(pMob->GetNumSlots(&numSlots));
			checkExpression(1 == numSlots, AAFRESULT_TEST_FAILED);

			// Enumerate over all MOB slots for this MOB
			checkResult(pMob->EnumAAFAllMobSlots(&pSlotIter));
			while (pSlotIter && pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS)
			{
				checkResult(pSlot->GetSegment(&pSegment));
				// See if it is a Selector
				checkResult(pSegment->QueryInterface(IID_IAAFNestedScope, (void **) &pNestedScope));
				pSegment->Release();
				pSegment = NULL;

				// -----------------------------------------------------------				
				// Enumerate slots
				checkResult(pNestedScope->GetSlots(&pSegIter));
				checkResult(pSegIter->NextOne(&pSegment));
				checkResult(pSegment->QueryInterface(IID_IAAFSourceClip, (void **)&pSourceClip));
				pSegment->Release();
				pSegment = NULL;

				while (pSegIter && pSegIter->NextOne(&pSegment) == AAFRESULT_SUCCESS)
				{
					// Make sure further segmenta are filler
					checkResult(pSegment->QueryInterface(IID_IAAFFiller, (void **)&pFiller));
					pSegment->Release();
					pSegment = NULL;
					pFiller->Release();
					pFiller = NULL;
				}

				pSegIter->Release();
				pSegIter = NULL;

				pSourceClip->Release();
				pSourceClip = NULL;

				pNestedScope->Release();
				pNestedScope = NULL;

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

	if (pSlot)
		pSlot->Release();

	if (pSlotIter)
		pSlotIter->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pSegIter)
		pSegIter->Release();

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

extern "C" HRESULT CAAFNestedScope_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFNestedScopeTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFNestedScope_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following AAFNestedScope tests have not been implemented:" << endl; 
//		cout << "     RemoveSegment" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}
	return hr;
}
















