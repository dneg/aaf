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

#ifndef __CAAFHeader_h__
#include "CAAFHeader.h"
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

static aafWChar *slotName = L"SLOT1";

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

	//Make the MOB
	IAAFMob			*pMob;

	hr = CoCreateInstance(CLSID_AAFMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);


	if (AAFRESULT_SUCCESS != hr)
		return hr;

	newUID = DDEF_Video;   // Could have been any other value !
	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;



	// Close file, clean-up and return
	hr = pFile->Close();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
 	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

//	return AAFRESULT_TEST_PARTIAL_SUCCESS;
	return AAFRESULT_SUCCESS;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
#ifdef FULL_TOOLKIT
	IAAFMob						*pMob;
#endif
	aafUID_t					checkUID;

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
	if (numMobs == 1)
	{
		checkUID = DDEF_Video;   // Could have been any other value !
#ifdef FULL_TOOLKIT
		hr = pHeader->LookupMob(&checkUID, &pMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
#endif 

	}
	hr = pFile->Close();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
}
 


HRESULT CAAFHeader::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"HeaderTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );

		hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFHeader::test...Caught general C++"
		" exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

  // Cleanup our object if it exists.
  return hr;
}































































