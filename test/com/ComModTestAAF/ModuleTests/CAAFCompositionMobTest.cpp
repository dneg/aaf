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

	IAAFCompositionMob*			pCompMob = NULL;
	IAAFMob*					pMob = NULL;

	aafUID_t					newMobID;
	aafProductIdentification_t	ProductInfo;
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
		// The next 3 function calls open the AAF file
		hr = pSession->SetDefaultIdentification(&ProductInfo);
		hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
	  	hr = pFile->GetHeader(&pHeader);

		// Create a CompositionMob
		hr = CoCreateInstance(CLSID_AAFCompositionMob,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFCompositionMob, 
								(void **)&pCompMob);
		if (AAFRESULT_SUCCESS == hr)
		{
			// Get a MOB Interface 
			hr = pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
			CoCreateGuid((GUID *)&newMobID);
			hr = pMob->SetMobID(&newMobID);

			hr = pCompMob->SetInitialValues(L"COMPMOB01", AAFFalse);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pCompMob->SetDefaultFade(fadeInLen, fadeInType, fadeInEditUnit);
			}	

			hr = pHeader->AppendMob(pMob);
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

	if (pCompMob)
		pCompMob->Release();

	if (pMob)
		pMob->Release();

	return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

	IEnumAAFMobs*				pMobIter = NULL;
	IAAFMob*					pMob = NULL;
	IAAFCompositionMob*			pCompMob = NULL;

	aafSearchCrit_t				criteria;
	aafDefaultFade_t			defaultFade;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
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
	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
		hr = pSession->SetDefaultIdentification(&ProductInfo);
		hr = pSession->OpenReadFile(pFileName, &pFile);
	  	hr = pFile->GetHeader(&pHeader);

		// Get the number of mobs in the file (should be one)
		hr = pHeader->GetNumMobs( kAllMob, &numMobs );
		if ( 1 == numMobs )
		{
			// Enumerate over all Composition Mobs
			criteria.searchTag = kByMobKind;
			criteria.tags.mobKind = kCompMob;
			hr = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
			while (pMobIter && pMobIter->NextOne(&pMob) !=AAFRESULT_NO_MORE_MOBS)
			{
				// Get A CompositionMob Interface 
				hr = pMob->QueryInterface(IID_IAAFCompositionMob,(void **)&pCompMob);
				if (AAFRESULT_SUCCESS == hr)
				{
					hr = pCompMob->GetDefaultFade( &defaultFade );
					if ( (defaultFade.fadeLength != fadeInLen) ||
						 (defaultFade.fadeType != fadeInType) ||
						 (memcmp( &( defaultFade.fadeEditUnit), &fadeInEditUnit, sizeof( fadeInEditUnit ))!= 0) ||
						 (defaultFade.valid != AAFTrue)
						)
					{
						hr = AAFRESULT_TEST_FAILED;
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

	if (pCompMob)
		pCompMob->Release();

	return hr;
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


