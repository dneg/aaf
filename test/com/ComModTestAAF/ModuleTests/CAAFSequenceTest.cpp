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









#include "CAAFSequence.h"
#include "CAAFSequence.h"
#ifndef __CAAFSequence_h__
#error - improperly defined include guard
#endif

#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include <iostream.h>

#define kNumComponents	5

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFSession**		ppSession,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
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

	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)ppSession);
	if (FAILED(hr))
		return hr;

	hr = (*ppSession)->SetDefaultIdentification(&ProductInfo);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = (*ppSession)->OpenReadFile(pFileName, ppFile);
		break;

	case kMediaOpenAppend:
		hr = (*ppSession)->CreateFile(pFileName, kAAFRev1, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		(*ppSession)->Release();
		*ppSession = NULL;
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppSession)->Release();
		*ppSession = NULL;
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFSession*	pSession = NULL;
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFMob*		pMob = NULL;
	IAAFMobSlot*	pMobSlot;
	IAAFSequence*	pSequence;
	IAAFSegment*	pSegment;
	aafUID_t		NewMobID;
	int				i;
	HRESULT			hr;

	// Create the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenAppend, &pSession, &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

	// Create a Composition Mob
	hr = CoCreateInstance(CLSID_AAFCompositionMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);
	if (FAILED(hr))
		goto Cleanup;

	CoCreateGuid((GUID *)&NewMobID);
	hr = pMob->SetMobID(&NewMobID);
	hr = pMob->SetName(L"AAFSequenceTest");
	
	// Add mob slot w/ sequence
 	hr = CoCreateInstance(CLSID_AAFSequence,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSequence, 
						   (void **)&pSequence);		
 	if (AAFRESULT_SUCCESS != hr)
		goto Cleanup;

	pSequence->SetInitialValue((aafUID_t*)&DDEF_Audio);

	//
	//	Add some segments.  Need to test failure conditions
	//	(i.e. starting/ending w/ transition, two trans back
	//	to bacl).
	//
	for(i = 0; i < kNumComponents; i++)
	{
		IAAFComponent*	pComponent;
		aafLength_t		len = 10;

		hr = CoCreateInstance(CLSID_AAFFiller,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFComponent, 
								(void **)&pComponent);
 		if (FAILED(hr))
			break;

		pComponent->SetDataDef((aafUID_t*)&DDEF_Audio);
		pComponent->SetLength(&len);
		hr = pSequence->AppendCpnt(pComponent);

		pComponent->Release();

		if (FAILED(hr))
			break;
	}

	if (SUCCEEDED(hr))
	{
		hr = pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment);
		if (FAILED(hr))
			goto Cleanup;

		pMob->AppendNewSlot(pSegment, 1, L"AAF Test Sequence", &pMobSlot);
		if (pMobSlot) pMobSlot->Release();

		pSegment->Release();

		// Add the master mob to the file and cleanup
		pHeader->AppendMob(pMob);
	}

Cleanup:
	if (pSequence) pSequence->Release();
	if (pMob) pMob->Release();
	if (pHeader) pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	if (pSession)
	{
		pSession->EndSession();
		pSession->Release();
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFSession*	pSession = NULL;
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr;

	// Open the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenReadOnly, &pSession, &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

	pHeader->GetNumMobs(kCompMob, &numMobs);
	if (1 != numMobs )
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Enumerate over Composition MOBs
	criteria.searchTag = kByMobKind;
	criteria.tags.mobKind = kCompMob;
    hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
	while (pMobIter && pMobIter->NextOne(&pMob) != AAFRESULT_NO_MORE_MOBS)
	{
		IAAFMobSlot*		pSlot;
		aafNumSlots_t		numSlots = 0;
		IEnumAAFMobSlots*	pSlotIter = NULL;

		pMob->GetNumSlots(&numSlots);
		if (1 != numSlots)
		{
			hr = AAFRESULT_TEST_FAILED;
		}
		else
		{
			// Enumerate over all MOB slots for this MOB
			hr = pMob->EnumAAFAllMobSlots(&pSlotIter);
			while (pSlotIter && pSlotIter->NextOne(&pSlot) != AAFRESULT_NO_MORE_OBJECTS)
			{
				IAAFComponent*		pComp;
				IAAFSegment*		pSegment;
				IAAFSequence*		pSequence;
				IEnumAAFComponents*	pCompIter;
				aafInt32			numCpnts;

				pSlot->GetSegment(&pSegment);
				pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence);
				pSegment->Release();

				pSequence->GetNumCpnts(&numCpnts);
				if (numCpnts != kNumComponents)
				{
					hr = AAFRESULT_TEST_FAILED;
				}
				else
				{
					pSequence->EnumComponents(&pCompIter);
					numCpnts = 0;
					while (pCompIter && pCompIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
					{
						aafLength_t	len;
						aafUID_t	dataDef;

						numCpnts++;

						pComp->GetDataDef(&dataDef);
						if (memcmp(&DDEF_Audio, &dataDef, sizeof(aafUID_t)) != 0)
							hr = AAFRESULT_TEST_FAILED;

						pComp->GetLength(&len);
						if (len != 10)
							hr = AAFRESULT_TEST_FAILED;

						pComp->Release();
					}

					if (pCompIter) pCompIter->Release();

					if (numCpnts != kNumComponents)
						hr = AAFRESULT_TEST_FAILED;
				}

				pSequence->Release();
				pSlot->Release();

				if (FAILED(hr))
					break;
			}

			if (pSlotIter) pSlotIter->Release();
		}

		pMob->Release();

		if (FAILED(hr))
			break;
	}

	if (pMobIter)
		pMobIter->Release();

Cleanup:

	if (pHeader) pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	if (pSession)
	{
		pSession->EndSession();
		pSession->Release();
	}

	return 	hr;
}

HRESULT CAAFSequence::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFSequenceTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFSequence::test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success
	if (SUCCEEDED(hr))
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;

	return hr;
}



















