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

#ifndef __CAAFSourceClip_h__
#include "CAAFSourceClip.h"
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

static aafWChar *slotName = L"SLOT1";
static aafInt32 fadeInLen  = 1000;
static aafInt32 fadeOutLen = 2000;
static aafFadeType_t fadeInType = kFadeLinearAmp;
static aafFadeType_t fadeOutType = kFadeLinearPower;
static aafSourceRef_t sourceRef; 

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
 	
	//Make the MOB
	IAAFMob			*pMob;
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
	
	hr = CoCreateInstance(CLSID_AAFSourceClip,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceClip, 
						   (void **)&sclp);		
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Set the properties for the SourceClip
	hr = sclp->SetFade( fadeInLen, fadeInType, fadeOutLen, fadeOutType);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	sourceRef.sourceID = NilMOBID;
	sourceRef.sourceSlotID = 0;
	sourceRef.startTime = 0;
	hr = sclp->SetRef(sourceRef);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = sclp->QueryInterface (IID_IAAFSegment, (void **)&seg);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pMob->AppendNewSlot (seg, 1, slotName, &newSlot);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	seg->Release();
	newSlot->Release();

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

	IEnumAAFMobs *mobIter;

//!!!	aafSearchCrit_t		criteria;
//!!!	criteria.searchTag = kNoSearch;

    hr = pHeader->EnumAAFAllMobs (NULL, &mobIter);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	for(n = 0; n < numMobs; n++)
	{
		IAAFMob			*aMob;
		aafNumSlots_t	numSlots;
		IEnumAAFMobSlots	*slotIter;
		IAAFMobSlot		*slot;
		IAAFSegment		*seg = NULL;
		IAAFSourceClip	*sclp = NULL;

		hr = mobIter->NextOne (&aMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = aMob->GetNumSlots (&numSlots);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		if(numSlots != 0)
		{
			hr = aMob->EnumAAFAllMobSlots(&slotIter);
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			for(s = 0; s < numSlots; s++)
			{
				aafInt32		rFadeInLen;
				aafInt32		rFadeOutLen;
				aafFadeType_t	rFadeInType;
				aafFadeType_t	rFadeOutType;
				aafSourceRef_t	rSourceRef; 
				aafBool			fadeInPresent;
				aafBool			fadeOutPresent;
				
				hr = slotIter->NextOne (&slot);
				if (AAFRESULT_SUCCESS != hr)
					return hr;

				hr = slot->GetSegment(&seg);
				if (AAFRESULT_SUCCESS != hr)
					return hr;

				hr = seg->QueryInterface(IID_IAAFSourceClip,(void **)&sclp);
				if (AAFRESULT_SUCCESS != hr)
					return hr;
		 
				hr = sclp->GetFade( &rFadeInLen, &rFadeInType, &fadeInPresent, 
									&rFadeOutLen, &rFadeOutType, &fadeOutPresent );
				if (AAFRESULT_SUCCESS != hr)
					return hr;
				// verify that we read exactly the same thing as we wrote to the file !!
				if (fadeInPresent)
				{
					if (rFadeInLen != fadeInLen ||
						rFadeInType != fadeInType)
					{
						hr = AAFRESULT_TEST_FAILED;
						return hr;
					}
				}
				else
				{
					hr = AAFRESULT_TEST_FAILED;
					return hr;
				}
				if (fadeOutPresent)
				{
					if (rFadeOutLen != fadeOutLen ||
						rFadeOutType != fadeOutType)
					{
						hr = AAFRESULT_TEST_FAILED;
						return hr;
					}
				}
				else
				{
					hr = AAFRESULT_TEST_FAILED;
					return hr;
				}
				
				hr = sclp->GetRef( &rSourceRef); 
				if (AAFRESULT_SUCCESS != hr)
					return hr;

				if (memcmp(&(rSourceRef.sourceID), &(sourceRef.sourceID), sizeof(sourceRef.sourceID)) != 0) 
				{
					hr = AAFRESULT_TEST_FAILED;
					return hr;
				}
				if (rSourceRef.sourceSlotID != sourceRef.sourceSlotID ||
					rSourceRef.startTime != sourceRef.startTime)
				{
					hr = AAFRESULT_TEST_FAILED;
					return hr;
				}
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
 


HRESULT CAAFSourceClip::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFMob *pObject = NULL;
 	aafWChar * pFileName = L"SourceClipTest.aaf";

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






















