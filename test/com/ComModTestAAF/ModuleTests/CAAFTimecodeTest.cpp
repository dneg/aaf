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




#include "CAAFTimecode.h"
#include "CAAFTimecode.h"
#ifndef __CAAFTimecode_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include "aafCvt.h"
#include "AAFResult.h"

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr;
	aafLength_t					zero;
	aafTimecode_t				startTC;

	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob						*pMob;
	IAAFMobSlot					*newSlot;
	IAAFTimecode				*timecode;
	IAAFSegment					*seg;

	CvtInt32toLength(0, zero);
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
	// Create a CompositionMob
	hr = CoCreateInstance(CLSID_AAFCompositionMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFCompositionMob, 
							(void **)&pCompMob);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	hr = pCompMob->SetInitialValues(L"COMPMOB01");
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Get a MOB interface
	hr = pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = CoCreateInstance(CLSID_AAFMobSlot,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMobSlot, 
							(void **)&newSlot);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = CoCreateInstance(CLSID_AAFTimecode,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFTimecode, 
						   (void **)&timecode);		
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	startTC.startFrame = 108000;	// One hour
	startTC.drop = kTcNonDrop;
	startTC.fps = 30;
	hr = timecode->InitTimecode (zero, &startTC);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = timecode->QueryInterface (IID_IAAFSegment, (void **)&seg);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pMob->AppendNewSlot (seg, 0, L"timecode", &newSlot);
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

	pCompMob->Release();
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
	aafNumSlots_t				numMobs, n;
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

	hr = pHeader->GetNumMobs( kAllMob, &numMobs );
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	if (numMobs > 0)
	{
		IEnumAAFMobs*		mobIter;
		IEnumAAFMobSlots	*pEnum;
		IAAFMob*			pMob = NULL;
		IAAFCompositionMob*	pCompMob = NULL;
		IAAFMobSlot			*pMobSlot;
		IAAFSegment			*pSeg;
		IAAFTimecode		*pTimecode;
		aafTimecode_t		startTC;

		hr = pHeader->EnumAAFAllMobs( NULL, &mobIter );
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		for (n = 0; n < numMobs; n++)
		{
			hr = mobIter->NextOne( &pMob );
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			hr = pMob->EnumAAFAllMobSlots (&pEnum);
			if (AAFRESULT_SUCCESS != hr)
				return hr;
			
			hr = pEnum->NextOne (&pMobSlot);
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			hr = pMobSlot->GetSegment (&pSeg);
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			hr = pSeg->QueryInterface (IID_IAAFTimecode, (void **)&pTimecode);
 			if (AAFRESULT_SUCCESS != hr)
				return hr;

			hr = pTimecode->GetTimecode (&startTC);
 			if (AAFRESULT_SUCCESS != hr)
				return hr;

			if(startTC.startFrame != 108000)
				return AAFRESULT_TEST_FAILED;
			if(startTC.drop != kTcNonDrop)
				return AAFRESULT_TEST_FAILED;
			if(startTC.fps != 30)
				return AAFRESULT_TEST_FAILED;
		}
	}


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

HRESULT CAAFTimecode::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"TimecodeTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTimecodeMob::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;

	// Cleanup our object if it exists.
	return hr;
}


