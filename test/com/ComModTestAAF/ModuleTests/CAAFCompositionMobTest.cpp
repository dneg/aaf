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



#ifndef __CAAFCompositionMob_h__
#include "CAAFCompositionMob.h"
#endif

#include <iostream.h>
#include "AAFResult.h"


static aafInt32 fadeInLen  = 1000;
static aafFadeType_t fadeInType = kFadeLinearAmp;
static aafRational_t fadeInEditUnit = { 25, 1};

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr;

	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob						*pMob;

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
	
	hr = pCompMob->SetInitialValues(L"COMPMOB01", AAFFalse);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pCompMob->SetDefaultFade(fadeInLen, fadeInType, fadeInEditUnit);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	// Get a MOB interface
	hr = pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

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
		IAAFMob*			pMob = NULL;
		IAAFCompositionMob*	pCompMob = NULL;
		aafDefaultFade_t	defaultFade;

		hr = pHeader->EnumAAFAllMobs( NULL, &mobIter );
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		for (n = 0; n < numMobs; n++)
		{
			hr = mobIter->NextOne( &pMob );
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			hr = pMob->QueryInterface(IID_IAAFCompositionMob,(void **)&pCompMob);
			if (AAFRESULT_SUCCESS != hr)
				return hr;

			pCompMob->GetDefaultFade( &defaultFade );
			if ( (defaultFade.fadeLength != fadeInLen) ||
				 (defaultFade.fadeType != fadeInType) ||
				 (memcmp( &( defaultFade.fadeEditUnit), &fadeInEditUnit, sizeof( fadeInEditUnit ))!= 0) ||
				 (defaultFade.valid != AAFTrue)
				)
			{
				hr = AAFRESULT_TEST_FAILED;
				return hr;
			}
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

HRESULT CAAFCompositionMob::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"CompMobTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFCompositionMob::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.
	return hr;
}


  //@access Public Members

  //@rdesc Error code [see below].


  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to omfiSourceClipGetFade() in all cases.
  //@comm Maps to omfiMobGetDefaultFade
	


  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade


