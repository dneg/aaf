// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __CAAFTransition_h__
#include "CAAFTransition.h"
#endif


#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

// This values are used for testing purposes
static aafUID_t    fillerUID = DDEF_Video;
static aafLength_t  fillerLength = 3200;

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


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pNewSlot = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFGroup*					pGroup = NULL;
	IAAFGroup*					pGroupCopy = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFSequence*				pSequence = NULL;
	
	aafUID_t					newMobID;
	aafUID_t					datadef = DDEF_Video;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint = 0;

	CvtInt32toLength(100, transitionLength);
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTransition Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;


	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the file
		checkResult(CoCreateInstance(CLSID_AAFFile,
									 NULL, 
									 CLSCTX_INPROC_SERVER, 
									 IID_IAAFFile, 
									 (void **)&pFile));
		checkResult(pFile->Initialize());
		checkResult(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		// Get the number of mobs to force creation of the content storage.
		// This is temporary as the content storage should be created by
		// the call to OpenNewModify above.
		aafNumSlots_t n;
		checkResult(pHeader->GetNumMobs(kAllMob, &n));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
 		
		// ------------------------------------------------------------
		//	To test a Transition we need to create a Sequence which will 
		//	a Filler, a transition and another Filler. I know this is not 
		//  very interesting, but it will let us test the Transition 
		//  interface with the least amount of other stuff.
		// ------------------------------------------------------------
		//
		// Create a CompositionMob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
							IID_IAAFCompositionMob, 
							(IUnknown **)&pCompMob));

		checkResult(pCompMob->Initialize(L"Transition Test"));
		// Get a MOB interface
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(&newMobID));

		// Create a Sequence
		checkResult(pDictionary->CreateInstance(&AUID_AAFSequence,
												IID_IAAFSequence,
												(IUnknown **) &pSequence));

		// Get a Segment interface
		checkResult(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
		// Get a component interface and 
		checkResult(pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		// set the Data definition for it !
		checkResult(pComponent->SetDataDef(&fillerUID));
		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Create a new Mob Slot that will contain the sequence
		checkResult(pMob->AppendNewSlot(pSegment, 1, L"Transition", &pNewSlot));


		// Create a Filler
		checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
												IID_IAAFFiller,
												(IUnknown **) &pFiller));

		// Get a component interface
		checkResult(pFiller->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
		// Set values for the filler
	    checkResult(pFiller->Initialize( &fillerUID, fillerLength));
		// append the filler to the sequence
		checkResult(pSequence->AppendComponent(pComponent));

		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Also release the filler
		pFiller->Release();
		pFiller = NULL;
		
	    checkResult(pDictionary->CreateInstance(&AUID_AAFTransition,
												IID_IAAFTransition, 
												(IUnknown **)&pTransition));
		// Create an empty Effect object !!
		checkResult(pDictionary->CreateInstance(&AUID_AAFGroup,
												IID_IAAFGroup,
												(IUnknown **)&pGroup));

		checkResult(pTransition->Create (&datadef, transitionLength, cutPoint, pGroup));
		checkResult(pGroup->Initialize(&datadef, transitionLength, NULL));
		checkResult(pTransition->QueryInterface (IID_IAAFComponent, (void **)&pComponent));

		// now append the transition
		checkResult(pSequence->AppendComponent(pComponent));

		// Release the component - because we need to reuse the pointer later
		pComponent->Release();
		pComponent = NULL;

		// Create the second filler 
		checkResult(pDictionary->CreateInstance(&AUID_AAFFiller,
												IID_IAAFFiller,
												(IUnknown **) &pFiller));

		checkResult(pFiller->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
		// Set values for the filler
	    checkResult(pFiller->Initialize( &fillerUID, fillerLength));
		// append the filler to the sequence
		checkResult(pSequence->AppendComponent(pComponent));

		// Now, we append the composition mob to the file	
		checkResult(pHeader->AppendMob(pMob));
		// and we are done !
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	

	// Cleanup and return
	if (pNewSlot)
		pNewSlot->Release();

	if (pSegment)
		pSegment->Release();

	if (pSequence)
		pSequence->Release();

	if (pFiller)
		pFiller->Release();


	if (pGroup)
		pGroup->Release();

	if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();


	if (pTransition)
		pTransition->Release();

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
    // IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IEnumAAFMobSlots*			pEnum = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFTransition*				pTransition = NULL;
	IAAFComponent*				pComponent = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFGroup*					pGroup = NULL;
	IEnumAAFComponents*			pCompIter = NULL;
//	aafUID_t					datadef ;
	aafLength_t					transitionLength;
	aafPosition_t				cutPoint;

	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	aafInt32					numComponents = 0;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"Make AVR Example. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;


	try
	{
		// Open the file
		checkResult(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		checkResult(pFile->Initialize());
		checkResult(pFile->OpenExistingRead(pFileName, 0));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->EnumAAFAllMobs( NULL, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->EnumAAFAllMobSlots (&pEnum));

			while (AAFRESULT_SUCCESS == pEnum->NextOne (&pMobSlot))
			{
				checkResult(pMobSlot->GetSegment (&pSegment));
				// Check to see if Segment is a Sequence
				checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
				// It is, so get a Component Iterator
				checkResult(pSequence->GetNumComponents(&numComponents));
				// Verify that all 3 components(Filler, Transition, Filler) are present
				checkExpression(numComponents == 3,  AAFRESULT_TEST_FAILED);
				checkResult(pSequence->EnumComponents(&pCompIter));
				// Now visit each and every one of the components.
				while(AAFRESULT_SUCCESS == pCompIter->NextOne(&pComponent))
				{
					// Find out what kind of segment we have
					if ((pComponent->QueryInterface(IID_IAAFTransition, (void **)&pTransition)) == AAFRESULT_SUCCESS)
					{
						// This is the transition 
						checkResult(pTransition->GetCutPoint (&cutPoint));
						checkResult(pComponent->GetLength(&transitionLength));
						checkResult(pTransition->GetEffect(&pGroup));
						// Check results !!
						checkExpression(cutPoint == 0, AAFRESULT_TEST_FAILED);
						checkExpression(transitionLength == 100, AAFRESULT_TEST_FAILED);
					}
					else
					{
						// validate that the other segments are Fillers
						checkResult(pComponent->QueryInterface(IID_IAAFFiller, (void **)&pFiller));
					}
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
	if (pTransition)
		pTransition->Release();

	if (pGroup)
		pGroup->Release();

	if (pComponent)
		pComponent->Release();

	if (pSegment)
		pSegment->Release();

	if (pMobSlot)
		pMobSlot->Release();

	if (pSequence)
		pSequence->Release();

	if (pCompIter)
		pCompIter->Release();

	if (pEnum)
		pEnum->Release();

	if (pFiller)
		pFiller->Release();

	if (pMob)
		pMob->Release();

	if (pDictionary)
		pDictionary->Release();

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

HRESULT CAAFTransition::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"TransitionTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTransition::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}



	 


	//@comm Replaces part of omfsTransitionGetInfo


	//@comm Replaces part of omfsTransitionGetInfo







