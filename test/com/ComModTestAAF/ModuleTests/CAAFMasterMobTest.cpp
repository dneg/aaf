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

#include "CAAFMasterMob.h"
#include "CAAFMasterMob.h"
#ifndef __CAAFMasterMob_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include "AAFResult.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };
static GUID		NewMobID;	// NOTE: this should really be aafUID_t, but problems w/ IsEqualGUID()

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		5

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
	if (!SUCCEEDED(hr))
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

	if (!SUCCEEDED(hr))
	{
		(*ppSession)->Release();
		*ppSession = NULL;
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (!SUCCEEDED(hr))
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
	IAAFMasterMob*	pMasterMob = NULL;
	HRESULT			hr;
	long			test;

	// Create the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenAppend, &pSession, &pFile, &pHeader);
	if (!SUCCEEDED(hr))
		return hr;

	// Create a Master Mob
	hr = CoCreateInstance(CLSID_AAFMasterMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);
	if (!SUCCEEDED(hr))
		goto Cleanup;

	CoCreateGuid((GUID *)&NewMobID);
	hr = pMob->SetMobID((aafUID_t *)&NewMobID);
	if (!SUCCEEDED(hr))
		goto Cleanup;

	hr = pMob->SetName(MobName);
	if (!SUCCEEDED(hr))
		goto Cleanup;
	
	// Add some slots
	for (test = 0; test < NumMobSlots; test++)
	{
		IAAFMobSlot*	pNewSlot = NULL;
		IAAFSegment*	pSegment = NULL;
		IAAFSourceClip* pSrcClip = NULL;

		hr = CoCreateInstance(CLSID_AAFSourceClip,
							   NULL, 
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFSourceClip, 
							   (void **)&pSrcClip);		
		if (SUCCEEDED(hr))
		{
			hr = pSrcClip->QueryInterface (IID_IAAFSegment, (void **)&pSegment);
			if (SUCCEEDED(hr))
			{
				hr = pMob->AppendNewSlot (pSegment, test+1, slotNames[test], &pNewSlot);
				if (SUCCEEDED(hr))
				{
					pNewSlot->Release();
				}
				pSegment->Release();
			}
			pSrcClip->Release();
		}

		if (!SUCCEEDED(hr))
			goto Cleanup;
	}

	// TODO: Test Master MOB specific methods here
	hr = pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob);
	if (!SUCCEEDED(hr))
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}
	pMasterMob->Release();

	// Add the master mob to the file and cleanup
	hr = pHeader->AppendMob(pMob);

Cleanup:
	pFile->Close();
	pSession->EndSession();

	if (pMob) pMob->Release();
	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	if (pSession) pSession->Release();

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
	if (!SUCCEEDED(hr))
		return hr;

	pHeader->GetNumMobs(kAllMob, &numMobs);
	if (1 != numMobs )
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Enumerate over all MOBs
	criteria.searchTag = kNoSearch;
    hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
	while (pMobIter && pMobIter->NextOne(&pMob) != AAFRESULT_NO_MORE_MOBS)
	{
		IAAFMasterMob*		pMasterMob = NULL;
		IEnumAAFMobSlots*	pSlotIter = NULL;
		IAAFMobSlot*		pSlot;
		aafWChar			name[500];
		aafNumSlots_t		numSlots = 0;
		GUID				mobID;

		pMob->GetName(name, sizeof(name));
		if (wcscmp(name, MobName) != 0) 
			hr = AAFRESULT_TEST_FAILED;

		pMob->GetMobID((aafUID_t *)&mobID);
		if (!IsEqualGUID(mobID, NewMobID))
			hr = AAFRESULT_TEST_FAILED;

		pMob->GetNumSlots(&numSlots);
		if (NumMobSlots != numSlots)
			hr = AAFRESULT_TEST_FAILED;

		// TODO: Test Master MOB specific methods here
		hr = pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob);
		if (!SUCCEEDED(hr))
		{
			hr = AAFRESULT_TEST_FAILED;
			goto Cleanup;
		}
		pMasterMob->Release();

		if (SUCCEEDED(hr))
		{
			unsigned long	s = 0;

			// Enumerate over all MOB slots for this MOB
			hr = pMob->EnumAAFAllMobSlots(&pSlotIter);
			while (pSlotIter && pSlotIter->NextOne(&pSlot) != AAFRESULT_NO_MORE_OBJECTS)
			{
				aafWChar			slotName[500];
				aafSlotID_t			slotID;

				pSlot->GetName(slotName, sizeof(slotName));
				if (wcscmp(slotName, slotNames[s]) != 0) 
					hr = AAFRESULT_TEST_FAILED;

				pSlot->GetSlotID(&slotID);
				if (slotID != s+1)
					hr = AAFRESULT_TEST_FAILED;

				pSlot->Release();
				s++;

				if (!SUCCEEDED(hr))
					break;
			}

			if (pSlotIter) pSlotIter->Release();
		}

		if (pMob) pMob->Release();

		if (!SUCCEEDED(hr))
			break;
	}

	if (pMobIter) pMobIter->Release();

Cleanup:
	pFile->Close();
	pSession->EndSession();

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
}
 

HRESULT CAAFMasterMob::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"MasterMOBTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFMasterMob::test...Caught general C++ exception!" << endl; 
	}

	return hr;
}
























