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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFMob			*pMob = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;

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

		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// if (AAFRESULT_SUCCESS != hr)
		// 	return hr;

		CHECK(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		CHECK(pFile->Initialize());

		// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
		CHECK(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
		bFileOpen = true;
  
		CHECK(pFile->GetHeader(&pHeader));

		//Make the MOB
		CHECK(CoCreateInstance(CLSID_AAFMob,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFMob, 
								(void **)&pMob));

		newUID = DDEF_Video;   // Could have been any other value !
		CHECK(pMob->SetMobID(&newUID));
		CHECK(pHeader->AppendMob(pMob));
	}
	catch (...)
	{
	}

	// Cleanup object references
	if (pMob)
		pMob->Release();
	
	if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	// hr = pSession->EndSession();
 	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;
	// if (pSession) pSession->Release();


	return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	IAAFMob						*pMob = NULL;
	aafUID_t					checkUID;
	bool bFileOpen = false;
	HRESULT						hr = AAFRESULT_SUCCESS;

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
		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// if (AAFRESULT_SUCCESS != hr)
		// 	return hr;

		CHECK(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		CHECK(pFile->Initialize());

		// hr = pSession->OpenReadFile(pFileName, &pFile);
		CHECK(pFile->OpenExistingRead(pFileName, 0));
		bFileOpen = true;
  
  	CHECK(pFile->GetHeader(&pHeader));

		CHECK(pHeader->GetNumMobs(kAllMob, &numMobs));
		if (numMobs != 1)
			CHECK(AAFRESULT_TEST_FAILED);

		checkUID = DDEF_Video;   // Could have been any other value !
		CHECK(pHeader->LookupMob(&checkUID, &pMob));
	}
	catch (...)
	{
	}

	// Cleanup object references
	if (pMob)
		pMob->Release();
	
	if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	//hr = pSession->EndSession();
	//if (AAFRESULT_SUCCESS != hr)
	//	return hr;
	//if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
}
 


HRESULT CAAFHeader::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"HeaderTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if (AAFRESULT_SUCCESS == hr)
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































































