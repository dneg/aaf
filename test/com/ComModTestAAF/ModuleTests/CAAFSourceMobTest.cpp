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

#include "CAAFSourceMob.h"
#include "CAAFSourceMob.h"
#ifndef __CAAFSourceMob_h__
#error - improperly defined include guard
#endif

#include <stdio.h>
#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr;

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
	if (AAFRESULT_SUCCESS != hr)
		return hr;
    hr = pFile->Initialize();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->SetDefaultIdentification(&ProductInfo);
	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

	// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
	hr = pFile->OpenNewModify(pFileName, 0, &ProductInfo);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
  
  	hr = pFile->GetHeader(&pHeader);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
 	
//Make the first mob
	IAAFSourceMob	*pSourceMob;
	IAAFMob			*pMob;
	long			test;
	IAAFEssenceDescriptor *edesc;
	aafUID_t		ddef = DDEF_Audio;

	aafRational_t	audioRate = { 44100, 1 };

	// Create a Mob
	hr = CoCreateInstance(CLSID_AAFSourceMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFSourceMob, 
							(void **)&pSourceMob);


	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	newUID.Data1 = 0;
	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pMob->SetName(L"SourceMOBTest");
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	// Add some slots
	for(test = 0; test < 2; test++)
	{
		hr = pSourceMob->AddNilReference (test+1, 0, &ddef, audioRate);
 		if (AAFRESULT_SUCCESS != hr)
			return hr;
	}

 	hr = CoCreateInstance(CLSID_AAFEssenceDescriptor,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFEssenceDescriptor, 
							(void **)&edesc);		
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
 	hr = pSourceMob->SetEssenceDescription (edesc);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	edesc->Release();
	edesc = NULL;
	pMob->Release();
	pMob = NULL;
	
	hr = pFile->Close();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
 	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFEssenceDescriptor		*pEdesc;
	IAAFSourceMob				*pSourceMob;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n, s;
	HRESULT						hr;

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
	if (AAFRESULT_SUCCESS != hr)
		return hr;
    hr = pFile->Initialize();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->SetDefaultIdentification(&ProductInfo);
	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

	// hr = pSession->OpenReadFile(pFileName, &pFile);
	hr = pFile->OpenExistingRead(pFileName, 0);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
  
  	hr = pFile->GetHeader(&pHeader);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pHeader->GetNumMobs(kAllMob, &numMobs);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (1 != numMobs )
		return AAFRESULT_TEST_FAILED;

	IEnumAAFMobs *mobIter;

//!!!	aafSearchCrit_t		criteria;
//!!!	criteria.searchTag = kNoSearch;

    hr = pHeader->EnumAAFAllMobs (NULL, &mobIter);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	for(n = 0; n < numMobs; n++)
	{
		IAAFMob			*aMob;
		aafWChar		name[500];
		aafNumSlots_t	numSlots;
		IEnumAAFMobSlots	*slotIter;
		IAAFMobSlot		*slot;
		aafUID_t		mobID;
		aafSlotID_t		trackID;

		hr = mobIter->NextOne (&aMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = aMob->GetName (name, sizeof(name));
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = aMob->GetMobID (&mobID);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = aMob->GetNumSlots (&numSlots);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		if (2 != numSlots)
			return AAFRESULT_TEST_FAILED;
		if(numSlots != 0)
		{
			hr = aMob->EnumAAFAllMobSlots(&slotIter);
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			for(s = 0; s < numSlots; s++)
			{
				hr = slotIter->NextOne (&slot);
				if (AAFRESULT_SUCCESS != hr)
					return hr;
				hr = slot->GetSlotID(&trackID);
				if (AAFRESULT_SUCCESS != hr)
					return hr;

				slot->Release();
				slot = NULL;
			}
		}
		hr = aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = pSourceMob->GetEssenceDescription (&pEdesc);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		aMob->Release();
		aMob = NULL;
		pEdesc->Release();
		pEdesc = NULL;
	}

	if(mobIter != NULL)
	{
		mobIter->Release();
		mobIter = NULL;
	}
	hr = pFile->Close();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
}
 
HRESULT CAAFSourceMob::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFSourceMob *pObject = NULL;
 	aafWChar * pFileName = L"SourceMOBTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFSourceMob::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	  
	return hr;
}
