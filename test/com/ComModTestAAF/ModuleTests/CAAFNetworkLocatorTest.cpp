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




#include "CAAFNetworkLocator.h"
#include "CAAFNetworkLocator.h"
#ifndef __CAAFNetworkLocator_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#define TEST_PATH	L"AnotherFile.aaf"

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
	IAAFLocator	*				pLocator = NULL;
	IAAFNetworkLocator *		pNetLocator = NULL;
	aafProductIdentification_t	ProductInfo;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	aafUID_t					newUID;
	aafInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafUID_t					ddef = DDEF_Audio;
	aafRational_t	audioRate = { 44100, 1 };
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
		CHECK(CoCreateInstance(CLSID_AAFFile,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFFile, 
								 (void **)&pFile));
		CHECK(pFile->Initialize());

		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// if (AAFRESULT_SUCCESS != hr)
		// 	return hr;

		// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
		CHECK(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
		bFileOpen = true;

  	CHECK(pFile->GetHeader(&pHeader));
 		
	//Make the first mob


		// Create a Mob
		CHECK(CoCreateInstance(CLSID_AAFSourceMob,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFSourceMob, 
								(void **)&pSourceMob));

		CHECK(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		CHECK(CoCreateGuid((GUID *)&newUID));
		CHECK(pMob->SetMobID(&newUID));
		CHECK(pMob->SetName(L"SourceMOBTest"));
		
		CHECK(CoCreateInstance(CLSID_AAFEssenceDescriptor,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFEssenceDescriptor, 
								(void **)&edesc));
										
 		CHECK(pSourceMob->SetEssenceDescriptor(edesc));

			// Verify that there are no locators
		CHECK(edesc->GetNumLocators(&numLocators));
		if (0 != numLocators)
			CHECK(AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		CHECK(CoCreateInstance(CLSID_AAFNetworkLocator,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFNetworkLocator, 
								(void **)&pNetLocator));		
		CHECK(pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));

		CHECK(pLocator->SetPath (TEST_PATH));
		
		CHECK(edesc->AppendLocator(pLocator));
 		CHECK(pSourceMob->SetEssenceDescriptor (edesc));

		// Verify that there is now one locator
		CHECK(edesc->GetNumLocators(&numLocators));
		if (1 != numLocators)
			CHECK(AAFRESULT_TEST_FAILED);

		// Add the source mob into the tree
		CHECK(pHeader->AppendMob(pMob));

	}
	catch (...)
	{
	}

	// cleanup
	if (pLocator)
		pLocator->Release();

	if (pNetLocator)
		pNetLocator->Release();

	if (edesc)
		edesc->Release();

	if (pMob)
		pMob->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	// hr = pSession->EndSession();
 	// if (AAFRESULT_SUCCESS != hr)
	//	return hr;

	// if (pSession) pSession->Release();


	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IEnumAAFLocators *			pEnum = NULL;
	IAAFLocator	*				pLocator = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFMob			*aMob = NULL;
	aafInt32					numLocators, readLen;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafWChar					readBuf[1024];
	bool bFileOpen = false;

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
		CHECK(pFile->Initialize());

		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// if (AAFRESULT_SUCCESS != hr)
		//	return hr;

		// hr = pSession->OpenReadFile(pFileName, &pFile);
		CHECK(pFile->OpenExistingRead(pFileName, 0));
		bFileOpen = true;

  	CHECK(pFile->GetHeader(&pHeader));

		CHECK(pHeader->GetNumMobs(kAllMob, &numMobs));
		if (1 != numMobs )
			CHECK(AAFRESULT_TEST_FAILED);


	//!!!	aafSearchCrit_t		criteria;
	//!!!	criteria.searchTag = kNoSearch;

		CHECK(pHeader->EnumAAFAllMobs (NULL, &mobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafUID_t		mobID;

			CHECK(mobIter->NextOne (&aMob));
			CHECK(aMob->GetName (name, sizeof(name)));
			CHECK(aMob->GetMobID (&mobID));

			CHECK(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			CHECK(pSourceMob->GetEssenceDescriptor (&pEdesc));

			// Verify that there is now one locator
			CHECK(pEdesc->GetNumLocators(&numLocators));
			if (1 != numLocators)
				CHECK(AAFRESULT_TEST_FAILED);
		
			CHECK(pEdesc->EnumAAFAllLocators(&pEnum));

			// This should read the one real locator
			CHECK(pEnum->NextOne(&pLocator));

			CHECK(pLocator->GetPathBufLen (&readLen));
	//		if(readLen != strlen(TEST_PATH))
				
			CHECK(pLocator->GetPath (readBuf, readLen));

		// This should run off the end
			pLocator->Release();
			pLocator = NULL;
			hr = pEnum->NextOne(&pLocator);
			if (AAFRESULT_NO_MORE_OBJECTS != hr)
				CHECK(hr);
			else
				hr = AAFRESULT_SUCCESS; // reset result
		}


	}
	catch (...)
	{
	}

	
	// Cleanup...
	if (pLocator)
		pLocator->Release();

	if (pEnum)
		pEnum->Release();

	if (pEdesc)
		pEdesc->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (aMob)
		aMob->Release();

	if (mobIter)
		mobIter->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	// hr = pSession->EndSession();
	// if (AAFRESULT_SUCCESS != hr)
	//	return hr;

	// if (pSession) pSession->Release();
	
	return 	hr;
}
 
HRESULT CAAFNetworkLocator::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFNetworkLocator *pObject = NULL;
  aafWChar * pFileName = L"NetworkLocatorTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFNetworkLocator::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
		pObject->Release();

  return hr;
}









