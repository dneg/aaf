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
#include "AAFUtils.h"

static aafWChar *slotName = L"SLOT1";
static aafInt32 fadeInLen  = 1000;
static aafInt32 fadeOutLen = 2000;
static aafFadeType_t fadeInType = kFadeLinearAmp;
static aafFadeType_t fadeOutType = kFadeLinearPower;
static aafSourceRef_t sourceRef; 

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFSession*				pSession = NULL;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IAAFMobSlot*				newSlot = NULL;
	IAAFSegment*				seg = NULL;
	IAAFSourceClip*				sclp = NULL;
	aafRational_t				audioRate = { 44100, 1 };

	aafProductIdentification_t	ProductInfo;
	aafUID_t					newMobID, referencedMobID;
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
	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls create the AAF file
		hr = pSession->SetDefaultIdentification(&ProductInfo);
		hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
	  	hr = pFile->GetHeader(&pHeader);

		//Make the MOB to be referenced
		hr = CoCreateInstance( CLSID_AAFMasterMob,
							   NULL, 			  
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFMob, 
							   (void **)&pReferencedMob);


		CoCreateGuid((GUID *)&referencedMobID);
		hr = pReferencedMob->SetMobID(&referencedMobID);
		hr = pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob");

		// Create a Mob
		hr = CoCreateInstance( CLSID_AAFCompositionMob,
							   NULL, 			  
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFMob, 
							   (void **)&pMob);


		CoCreateGuid((GUID *)&newMobID);
		hr = pMob->SetMobID(&newMobID);
		hr = pMob->SetName(L"AAFSourceClipTest");

		// Create a SourceClip
		hr = CoCreateInstance( CLSID_AAFSourceClip,
							   NULL, 
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFSourceClip, 
							   (void **)&sclp);		
 		if (AAFRESULT_SUCCESS == hr)
		{			
			// Set the properties for the SourceClip
			hr = sclp->SetFade( fadeInLen, fadeInType, fadeOutLen, fadeOutType);
			if (AAFRESULT_SUCCESS == hr)
			{
				sourceRef.sourceID = referencedMobID;
				sourceRef.sourceSlotID = 0;
				sourceRef.startTime = 0;
				hr = sclp->SetRef(sourceRef);
				if (AAFRESULT_SUCCESS == hr)
				{
					hr = sclp->QueryInterface (IID_IAAFSegment, (void **)&seg);
					if (AAFRESULT_SUCCESS == hr)
					{
						hr = pMob->AppendNewSlot (seg, 1, slotName, &newSlot);
					}
				}
			}
		}
	}

	if (AAFRESULT_SUCCESS == hr)
		hr = pHeader->AppendMob(pMob);
	if (AAFRESULT_SUCCESS == hr)
		hr = pHeader->AppendMob(pReferencedMob);

	// Cleanup and return
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

	if (pHeader)
		pHeader->Release();

	if (pMob)
		pMob->Release();

	if (newSlot)
		newSlot->Release();

	if (seg)
		seg->Release();

	if (sclp)
		sclp->Release();

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSourceClip*				pSourceClip = NULL;

	aafProductIdentification_t	ProductInfo;
	aafSearchCrit_t				criteria;
	aafNumSlots_t				numMobs, numSlots;
	aafInt32					rFadeInLen;
	aafInt32					rFadeOutLen;
	aafFadeType_t				rFadeInType;
	aafFadeType_t				rFadeOutType;
	aafSourceRef_t				rSourceRef; 
	aafBool						fadeInPresent;
	aafBool						fadeOutPresent;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafUID_t					rReferencedMobID;

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
	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
		hr = pSession->SetDefaultIdentification(&ProductInfo);
		hr = pSession->OpenReadFile(pFileName, &pFile);
	  	hr = pFile->GetHeader(&pHeader);

		// Get the number of mobs in the file (should be one)
		hr = pHeader->GetNumMobs(kAllMob, &numMobs);
		if (2 == numMobs )
		{
			// Enumerate over all Composition Mobs
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kCompMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			while (pMobIter && pMobIter->NextOne(&pMob) !=AAFRESULT_NO_MORE_MOBS)
			{
				pMob->GetNumSlots(&numSlots);
				if (1 == numSlots)
				{
					hr = pMob->EnumAAFAllMobSlots(&pSlotIter);
					while (pSlotIter && pSlotIter->NextOne(&pSlot) != AAFRESULT_NO_MORE_OBJECTS)
					{
						hr = pSlot->GetSegment(&pSegment);
						hr = pSegment->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
		
						hr = pSourceClip->GetFade( &rFadeInLen, &rFadeInType, &fadeInPresent, 
											&rFadeOutLen, &rFadeOutType, &fadeOutPresent );
						hr = pSourceClip->GetRef( &rSourceRef); 
						// verify that we read exactly the same thing as we wrote to the file !!
						if (fadeInPresent)
						{
							if (rFadeInLen != fadeInLen ||
								rFadeInType != fadeInType)
							{
								hr = AAFRESULT_TEST_FAILED;
							}
						}
						else
						{
							hr = AAFRESULT_TEST_FAILED;
						}
						if (fadeOutPresent)
						{
							if (rFadeOutLen != fadeOutLen ||
								rFadeOutType != fadeOutType)
							{
								hr = AAFRESULT_TEST_FAILED;
							}
						}
						else
						{
							hr = AAFRESULT_TEST_FAILED;
						}
						if (memcmp(&(rSourceRef.sourceID), &(sourceRef.sourceID), sizeof(sourceRef.sourceID)) != 0) 
						{
							hr = AAFRESULT_TEST_FAILED;
						}
						if (rSourceRef.sourceSlotID != sourceRef.sourceSlotID ||
							rSourceRef.startTime != sourceRef.startTime)
						{
							hr = AAFRESULT_TEST_FAILED;
						}

						hr = pSourceClip->ResolveRef(&pReferencedMob);
						if(hr == AAFRESULT_SUCCESS)
						{
							pReferencedMob->GetMobID(&rReferencedMobID);
							if(!EqualAUID(&rReferencedMobID, &sourceRef.sourceID))
								hr = AAFRESULT_TEST_FAILED;
						}
					}
				}
				else
				{
					hr = AAFRESULT_TEST_FAILED;
				}
			}
		}
		else
		{
			hr = AAFRESULT_TEST_FAILED;
		}
	}

	// Cleanup and return
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

	if (pHeader)
		pHeader->Release();

	if (pMob)
		pMob->Release();

	if (pSlot)
		pSlot->Release();

	if (pSegment)
		pSegment->Release();

	if (pSourceClip)
		pSourceClip->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pSlotIter)
		pSlotIter->Release();

	return hr;
}
 


HRESULT CAAFSourceClip::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFMob *pObject = NULL;
 	aafWChar * pFileName = L"SourceClipTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
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






















