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


// Handle macro for error handling...
#ifdef CHECK
#undef CHECK
#endif
#define CHECK(result)\
do\
{\
	hr = result;\
	if (AAFRESULT_SUCCESS != hr)\
		throw hr;\
} while (false)



static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession*				pSession = NULL;
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IAAFMobSlot*				newSlot = NULL;
	IAAFSegment*				seg = NULL;
	IAAFSourceClip*				sclp = NULL;
	aafRational_t				audioRate = { 44100, 1 };
	bool bFileOpen = false;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newMobID, referencedMobID;
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

	try
	{
		/*
		hr = CoCreateInstance(CLSID_AAFSession,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFSession, 
								 (void **)&pSession);
		*/
		CHECK(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		// We assume the following functions have been tested and they do work
		// The next 3 function calls create the AAF file
		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
		CHECK(pFile->Initialize());
		CHECK(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
		bFileOpen = true;

		CHECK(pFile->GetHeader(&pHeader));

		//Make the MOB to be referenced
		CHECK(CoCreateInstance( CLSID_AAFMasterMob,
								 NULL, 			  
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFMob, 
								 (void **)&pReferencedMob));
		CHECK(CoCreateGuid((GUID *)&referencedMobID));
		CHECK(pReferencedMob->SetMobID(&referencedMobID));
		CHECK(pReferencedMob->SetName(L"AAFSourceClipTest::ReferencedMob"));

		// Create a Mob
		CHECK(CoCreateInstance( CLSID_AAFCompositionMob,
								 NULL, 			  
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFMob, 
								 (void **)&pMob));
		CHECK(CoCreateGuid((GUID *)&newMobID));
		CHECK(pMob->SetMobID(&newMobID));
		CHECK(pMob->SetName(L"AAFSourceClipTest"));

		// Create a SourceClip
		CHECK(CoCreateInstance( CLSID_AAFSourceClip,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFSourceClip, 
								 (void **)&sclp));
								 		
		// Set the properties for the SourceClip
		CHECK(sclp->SetFade( fadeInLen, fadeInType, fadeOutLen, fadeOutType));
		sourceRef.sourceID = referencedMobID;
		sourceRef.sourceSlotID = 0;
		sourceRef.startTime = 0;
		CHECK(sclp->SetSourceReference(sourceRef));

		CHECK(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
		CHECK(pMob->AppendNewSlot (seg, 1, slotName, &newSlot));

		CHECK(pHeader->AppendMob(pMob));
		CHECK(pHeader->AppendMob(pReferencedMob));
	}
	catch (...)
	{
	}

	// Cleanup and return
	if (newSlot)
		newSlot->Release();

	if (seg)
		seg->Release();

	if (sclp)
		sclp->Release();

	if (pMob)
		pMob->Release();

	if (pReferencedMob)
		pReferencedMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		if (bFileOpen)
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
	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pReferencedMob = NULL;
	IEnumAAFMobSlots*			pSlotIter = NULL;
	IAAFMobSlot*				pSlot = NULL;
	IAAFSegment*				pSegment = NULL;
	IAAFSourceClip*				pSourceClip = NULL;
	bool bFileOpen = false;
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

	try
	{  
		/*
		hr = CoCreateInstance(CLSID_AAFSession,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFSession, 
								 (void **)&pSession);
		*/
		CHECK(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));

		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// hr = pSession->OpenReadFile(pFileName, &pFile);
		CHECK(pFile->Initialize());
		CHECK(pFile->OpenExistingRead(pFileName, 0));
		bFileOpen = true;

		CHECK(pFile->GetHeader(&pHeader));

		// Get the number of mobs in the file (should be one)
		CHECK(pHeader->GetNumMobs(kAllMob, &numMobs));
		if (2 != numMobs )
			CHECK(AAFRESULT_TEST_FAILED);

		// Enumerate over all Composition Mobs
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = kCompMob;
		CHECK(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			CHECK(pMob->GetNumSlots(&numSlots));
			if (1 != numSlots)
				CHECK(AAFRESULT_TEST_FAILED);

			CHECK(pMob->EnumAAFAllMobSlots(&pSlotIter));
			while (AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
			{
				// The segment should be a source clip...
				CHECK(pSlot->GetSegment(&pSegment));
				CHECK(pSegment->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip));

				CHECK(pSourceClip->GetFade( &rFadeInLen, &rFadeInType, &fadeInPresent, 
									&rFadeOutLen, &rFadeOutType, &fadeOutPresent ));
				CHECK(pSourceClip->GetSourceReference( &rSourceRef)); 
				// verify that we read exactly the same thing as we wrote to the file !!
				if (!fadeInPresent)
					CHECK(AAFRESULT_TEST_FAILED);
				if (rFadeInLen != fadeInLen || rFadeInType != fadeInType)
					CHECK(AAFRESULT_TEST_FAILED);
				if (!fadeOutPresent)
					CHECK(AAFRESULT_TEST_FAILED);
				if (rFadeOutLen != fadeOutLen || rFadeOutType != fadeOutType)
					CHECK(AAFRESULT_TEST_FAILED);
				if (memcmp(&(rSourceRef.sourceID), &(sourceRef.sourceID), sizeof(sourceRef.sourceID)) != 0) 
					CHECK(AAFRESULT_TEST_FAILED);
				if (rSourceRef.sourceSlotID != sourceRef.sourceSlotID ||
					rSourceRef.startTime != sourceRef.startTime)
					CHECK(AAFRESULT_TEST_FAILED);

				CHECK(pSourceClip->ResolveRef(&pReferencedMob));
				CHECK(pReferencedMob->GetMobID(&rReferencedMobID));
				if(!EqualAUID(&rReferencedMobID, &sourceRef.sourceID))
					CHECK(AAFRESULT_TEST_FAILED);

				pSlot->Release();
				pSlot = NULL;
			}

			pMob->Release();
			pMob = NULL;
		}

	}
	catch (...)
	{
	}

	// Cleanup and return
	if (pReferencedMob)
		pReferencedMob->Release();

	if (pSourceClip)
		pSourceClip->Release();

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

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		if (bFileOpen)
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






















