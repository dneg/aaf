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


#ifndef __CAAFMob_h__
#include "CAAFMob.h"
#endif

#include <iostream.h>
#include "AAFResult.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
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

	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pSession->SetDefaultIdentification(&ProductInfo);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
  
  	hr = pFile->GetHeader(&pHeader);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
 	
//Make the first mob
	IAAFMob						*pMob;
	long	test;
	IAAFMobSlot		*newSlot;
	IAAFSegment		*seg;
	IAAFSourceClip	*sclp;
	aafRational_t	audioRate = { 44100, 1 };

	// Create a Mob
	hr = CoCreateInstance(CLSID_AAFMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);


	if (AAFRESULT_SUCCESS != hr)
		return hr;

	newUID.Data1 = 0;
	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pMob->SetName(L"MOBTest");
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	// Add some slots
	for(test = 0; test < 5; test++)
	{
		hr = CoCreateInstance(CLSID_AAFMobSlot,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFMobSlot, 
								(void **)&newSlot);
 		if (AAFRESULT_SUCCESS != hr)
			return hr;

 		hr = CoCreateInstance(CLSID_AAFSourceClip,
							   NULL, 
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFSourceClip, 
							   (void **)&sclp);		
 		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = sclp->QueryInterface (IID_IAAFSegment, (void **)&seg);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = pMob->AppendNewSlot (seg, test+1, slotNames[test], &newSlot);
 		if (AAFRESULT_SUCCESS != hr)
			return hr;

		seg->Release();
		newSlot->Release();
	}
	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pFile->Close();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pSession->EndSession();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	pMob->Release();
	if (pFile) pFile->Release();
	if (pSession) pSession->Release();

	return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
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
	  
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pSession->SetDefaultIdentification(&ProductInfo);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pSession->OpenReadFile(pFileName, &pFile);
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
		aafWChar		name[500], slotName[500];
		aafNumSlots_t	numSlots;
		IEnumAAFMobSlots	*slotIter;
		IAAFMobSlot		*slot;
		aafUID_t		mobID;
		aafSlotID_t		trackID;

		hr = mobIter->NextOne (&aMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = aMob->GetName (name);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = aMob->GetMobID (&mobID);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = aMob->GetNumSlots (&numSlots);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		if (5 != numSlots)
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
				hr = slot->GetName (slotName);
				if (AAFRESULT_SUCCESS != hr)
					return hr;
				hr = slot->GetSlotID(&trackID);
				if (AAFRESULT_SUCCESS != hr)
					return hr;
//				if (memcmp(slotName, slotNames[s], 10) != 0) 
				if (wcscmp(slotName, slotNames[s]) != 0) 
					return AAFRESULT_TEST_FAILED;
			}
		}
	}

	//!!! Problem deleting, let it leak -- 	delete mobIter;
	hr = pFile->Close();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pSession->EndSession();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
}
 

HRESULT CAAFMob::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFMob *pObject = NULL;
 	aafWChar * pFileName = L"MOBTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );

		hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFMob::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}






























































////////////////////////////////////////////////////////////////////////////////
