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

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __CAAFFiller_h__
#include "CAAFFiller.h"
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"


static aafUID_t		fillerUID = DDEF_Timecode;
static aafLength_t	fillerLength = 3200;

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

	IAAFCompositionMob*			pCompMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;

	aafProductIdentification_t	ProductInfo;
	aafUID_t					newMobID;
	HRESULT						hr = AAFRESULT_SUCCESS;


	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"Make AVR Example";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	/*
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession);
	*/
	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)&pFile);

	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls create the AAF file
		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
	    hr = pFile->Initialize();
		hr = pFile->OpenNewModify(pFileName, 0, &ProductInfo);
		hr = pFile->GetHeader(&pHeader);

		// Create a Composition mob - it should work !!
		hr = CoCreateInstance(CLSID_AAFCompositionMob,
							  NULL, 
							  CLSCTX_INPROC_SERVER, 
							  IID_IAAFCompositionMob, 
							  (void **)&pCompMob);
		// get a IAAFMob interface
		hr = pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		// Initialize the CompMob
		CoCreateGuid((GUID *)&newMobID);
		hr = pMob->SetMobID(&newMobID);
		hr = pMob->SetName(L"AAFFillerTest");

		// Create a AAFFiller - since it is the first time we will check the error code
		hr = CoCreateInstance(CLSID_AAFFiller,
							  NULL, 
							  CLSCTX_INPROC_SERVER, 
							  IID_IAAFFiller, 
							  (void **)&pFiller);
		if (AAFRESULT_SUCCESS == hr)
		{
			// Get a IAAFSegment interface for it
			hr = pFiller->QueryInterface (IID_IAAFSegment, (void **)&pSegment);
			if (AAFRESULT_SUCCESS == hr)
			{
				// Set filler properties
				hr = pFiller->Initialize( &fillerUID, fillerLength);
				if (AAFRESULT_SUCCESS == hr)
				{
					// append the filler to the MOB tree
					hr = pMob->AppendNewSlot(pSegment, 1, L"FillerSlot", &pSlot); 
					if (AAFRESULT_SUCCESS == hr)
					{
						pSlot->Release();
						pSlot = NULL;
					}
				}

				pSegment->Release();
				pSegment = NULL;
			}

			pFiller->Release();
			pFiller = NULL;
		}
	}

	// Add the Mob to the file
	if (AAFRESULT_SUCCESS == hr)
		hr = pHeader->AppendMob(pMob);



	// Cleanup and return
	if (pSegment)
		pSegment->Release();

	if (pSlot)
		pSlot->Release();

	if (pFiller)
		pFiller->Release();

	if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	/*
	if (pSession)
	{
		pSession->EndSession();
		pSession->Release();
	}
	*/

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

//	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFFiller*					pFiller = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFComponent*				pComponent = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs, numSlots;
	aafSearchCrit_t				criteria;
	aafUID_t					readFillerUID;
	aafLength_t					readFillerLength;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"Make AVR Example";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	/*
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession);

	*/
	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)&pFile);

	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
		// hr = pSession->SetDefaultIdentification(&ProductInfo);
	    // hr = pSession->OpenReadFile(pFileName, &pFile);
	    hr = pFile->Initialize();
		hr = pFile->OpenExistingRead(pFileName, 0);
		hr = pFile->GetHeader(&pHeader);

		// Get the number of mobs in the file (should be one)
		hr = pHeader->GetNumMobs(kAllMob, &numMobs);
		if (1 == numMobs )
		{
			// Enumerate over all Composition Mobs
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kCompMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
			{
				pMob->GetNumSlots(&numSlots);
				if (1 == numSlots)
				{
					hr = pMob->EnumAAFAllMobSlots(&pSlotIter);
					while (pSlotIter && (pSlotIter->NextOne(&pSlot) == AAFRESULT_SUCCESS))
					{
						hr = pSlot->GetSegment(&pSegment);
						hr = pSegment->QueryInterface(IID_IAAFFiller, (void **) &pFiller);
						// Make sure the segment is a AAFFiller !!
						if (AAFRESULT_SUCCESS == hr)
						{
							// Get The component interface
							hr = pSegment->QueryInterface(IID_IAAFComponent, (void **) &pComponent);
							// retrieve properties
							hr = pComponent->GetLength( &readFillerLength);
							hr = pComponent->GetDataDef( &readFillerUID);
							// Compare results
							if ( (readFillerLength != fillerLength) ||
								 (memcmp(&readFillerUID, &fillerUID, sizeof(fillerUID)) != 0))
							{
								 hr = AAFRESULT_TEST_FAILED;
							}

							pComponent->Release();
							pComponent = NULL;
						}
						else
						{
							hr = AAFRESULT_TEST_FAILED;
						}

						pSegment->Release();
						pSegment = NULL;

						pSlot->Release();
						pSlot = NULL;
					}

					if (pSlotIter)
					{
						pSlotIter->Release();
						pSlotIter = NULL;
					}
				}
				else
				{
					hr = AAFRESULT_TEST_FAILED;
				}

				pMob->Release();
				pMob = NULL;
			}

			if (pMobIter)
			{
				pMobIter->Release();
				pMobIter = NULL;
			}

		}
		else
		{
			hr = AAFRESULT_TEST_FAILED;
		}
	}

	// Cleanup and return

	if (pComponent)
		pComponent->Release();

	if (pSegment)
		pSegment->Release();

	if (pSlot)
		pSlot->Release();

	if (pFiller)
		pFiller->Release();

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
		pFile->Close();
		pFile->Release();
	}

	/*
	if (pSession)
	{
		pSession->EndSession();
		pSession->Release();
	}
	*/

//	if (pSourceMob)
//		pSourceMob->Release();


	

	return hr;
}

HRESULT CAAFFiller::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"FillerTest.aaf";

	try
	{
		hr = CreateAAFFile( pFileName );
		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFFiller::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  return hr;
}





