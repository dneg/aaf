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
#include "AAFDefUIDs.h"

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		3

static aafWChar *		slotNames[NumMobSlots] = { L"VIDEO SLOT", L"AUDIO SLOT1", L"AUDIO SLOT2"};
static const aafUID_t *	slotDDefs[NumMobSlots] = {&DDEF_Video, &DDEF_Audio, &DDEF_Audio};
static aafRational_t	slotRates[NumMobSlots] = { {297,1}, {44100, 1}, {44100, 1}};

static GUID		NewMobID;	// NOTE: this should really be aafUID_t, but problems w/ IsEqualGUID()


static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   // IAAFSession**		ppSession,
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

	/*
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)ppSession);
	*/
	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)ppFile);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
    hr = (*ppFile)->Initialize();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = (*ppSession)->SetDefaultIdentification(&ProductInfo);
	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		// hr = (*ppSession)->OpenReadFile(pFileName, ppFile);
		hr = (*ppFile)->OpenExistingRead(pFileName, 0);
		break;

	case kMediaOpenAppend:
		// hr = (*ppSession)->CreateFile(pFileName, kAAFRev1, ppFile);
		hr = (*ppFile)->OpenNewModify(pFileName, 0, &ProductInfo);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		// (*ppSession)->Release();
		// *ppSession = NULL;
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		// (*ppSession)->Release();
	    // *ppSession = NULL;
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession*	pSession = NULL;
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFMob*		pMob = NULL;
	IAAFMasterMob*	pMasterMob = NULL;
	HRESULT			hr;
	long			test;

	// Create the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

	// Create a Master Mob
	hr = CoCreateInstance(CLSID_AAFMasterMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);
	if (FAILED(hr))
		goto Cleanup;

	CoCreateGuid((GUID *)&NewMobID);
	hr = pMob->SetMobID((aafUID_t *)&NewMobID);
	if (FAILED(hr))
		goto Cleanup;

	hr = pMob->SetName(MobName);
	if (FAILED(hr))
		goto Cleanup;
	
	hr = pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob);
	if (FAILED(hr))
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Add some slots
	for (test = 0; test < NumMobSlots; test++)
	{
		IAAFSourceMob* pSrcMob;

		hr = CoCreateInstance(CLSID_AAFSourceMob,
							   NULL, 
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFSourceMob, 
							   (void **)&pSrcMob);		
		if (SUCCEEDED(hr))
		{
			hr = pSrcMob->AddNilReference (1, 0, (aafUID_t *)slotDDefs[test], slotRates[test]);
			if (SUCCEEDED(hr))
			{
				aafUID_t				TempUID;
				IAAFMob*				pTempMob;
				IAAFEssenceDescriptor*	pDesc;

 				hr = CoCreateInstance(CLSID_AAFEssenceDescriptor,
										NULL, 
										CLSCTX_INPROC_SERVER, 
										IID_IAAFEssenceDescriptor, 
										(void **)&pDesc);		
 				hr = pSrcMob->SetEssenceDescriptor(pDesc);
				pDesc->Release();

				// Append source MOB to header
				pSrcMob->QueryInterface(IID_IAAFMob, (void **) &pTempMob);
				CoCreateGuid((GUID *)&TempUID);
				hr = pTempMob->SetMobID(&TempUID);
				hr = pTempMob->SetName(L"source mob");
				pHeader->AppendMob(pTempMob);
				pTempMob->Release();

				hr = pMasterMob->AddMasterSlot((aafUID_t *)slotDDefs[test], 1, pSrcMob, test+1, slotNames[test]);
			}
			pSrcMob->Release();
		}

		if (FAILED(hr))
			goto Cleanup;
	}

	// Add the master mob to the file and cleanup
	hr = pHeader->AppendMob(pMob);

Cleanup:
	pFile->Close();
	// pSession->EndSession();

	if (pMasterMob) pMasterMob->Release();
	if (pMob) pMob->Release();
	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	// IAAFSession*	pSession = NULL;
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*		pMob;
	aafNumSlots_t	numMobs;
	aafSearchCrit_t	criteria;
	HRESULT			hr;

	// Open the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenReadOnly, /*&pSession,*/ &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

	pHeader->GetNumMobs(kMasterMob, &numMobs);
	if (1 != numMobs )
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Enumerate over Master MOBs
	criteria.searchTag = kByMobKind;
	criteria.tags.mobKind = kMasterMob;
    hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
	while (pMobIter && pMobIter->NextOne(&pMob) != AAFRESULT_NO_MORE_MOBS)
	{
		IAAFMasterMob*		pMasterMob = NULL;
		IEnumAAFMobSlots*	pSlotIter = NULL;
		IAAFMobSlot*		pSlot;
		aafWChar			name[500];
		aafNumSlots_t		numSlots = 0;
		GUID				mobID;

		// TODO: Test Master MOB specific methods here
		hr = pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob);
		if (FAILED(hr))
		{
			hr = AAFRESULT_TEST_FAILED;
			goto Cleanup;
		}

		pMob->GetName(name, sizeof(name));
		if (wcscmp(name, MobName) != 0) 
			hr = AAFRESULT_TEST_FAILED;

		pMob->GetMobID((aafUID_t *)&mobID);
		if (!IsEqualGUID(mobID, NewMobID))
			hr = AAFRESULT_TEST_FAILED;

		pMob->GetNumSlots(&numSlots);
		if (NumMobSlots != numSlots)
			hr = AAFRESULT_TEST_FAILED;

		if (SUCCEEDED(hr))
		{
			unsigned long	s = 0;

			// Enumerate over all MOB slots for this MOB
			hr = pMob->EnumAAFAllMobSlots(&pSlotIter);
			while (pSlotIter && pSlotIter->NextOne(&pSlot) != AAFRESULT_NO_MORE_OBJECTS)
			{
				aafWChar			slotName[500];
				aafSlotID_t			slotID;
				aafNumSlots_t		numReps;
				aafWChar*			pTapeName = NULL;
				aafInt32			bufSize = 0;

				pSlot->GetName(slotName, sizeof(slotName));
				if (wcscmp(slotName, slotNames[s]) != 0) 
					hr = AAFRESULT_TEST_FAILED;

				pSlot->GetSlotID(&slotID);
				if (slotID != s+1)
					hr = AAFRESULT_TEST_FAILED;

				pMasterMob->GetTapeNameBufLen(slotID, &bufSize);
				if (bufSize)
				{
					pTapeName = new aafWChar [bufSize];
					if (pTapeName)
					{
						pMasterMob->GetTapeName(slotID, pTapeName, bufSize);
						delete [] pTapeName;
					}
				}

				pMasterMob->GetNumRepresentations(slotID, &numReps);
				if (numReps != 1)
					hr = AAFRESULT_TEST_FAILED;

				pSlot->Release();
				s++;

				if (FAILED(hr))
					break;
			}

			if (pSlotIter) pSlotIter->Release();
		}

		if (pMasterMob) pMasterMob->Release();
		if (pMob) pMob->Release();

		if (FAILED(hr))
			break;
	}

	if (pMobIter) pMobIter->Release();

Cleanup:
	pFile->Close();
	// pSession->EndSession();

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
}
 

HRESULT CAAFMasterMob::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"MasterMOBTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFMasterMob::test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following IAAFMasterMob methods have not been implemented:" << endl; 
		cout << "     GetTapeName" << endl; 
		cout << "     GetTapeNameBufLen" << endl; 
		cout << "     GetRepresentationSourceClip" << endl; 
		cout << "     GetCriteriaSourceClip" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}
























