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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFLocator	*				pLocator;
	IAAFNetworkLocator *		pNetLocator;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	aafInt32					numLocators;
	HRESULT						hr;
	aafUID_t					ddef = DDEF_Audio;

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
 	
//Make the first mob
	IAAFSourceMob	*pSourceMob;
	IAAFMob			*pMob;
	IAAFEssenceDescriptor *edesc;

	aafRational_t	audioRate = { 44100, 1 };

	// Create a Mob
	hr = CoCreateInstance(CLSID_AAFSourceMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFSourceMob, 
							(void **)&pSourceMob);


	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	newUID.Data1 = 0;
	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pMob->SetName(L"SourceMOBTest");
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	hr = CoCreateInstance(CLSID_AAFEssenceDescriptor,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFEssenceDescriptor, 
							(void **)&edesc);		
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
 	hr = pSourceMob->SetEssenceDescription (edesc);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

    // Verify that there are no locators
	hr = edesc->GetNumLocators(&numLocators);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (0 != numLocators)
		return AAFRESULT_TEST_FAILED;

  
	// Make a locator, and attach it to the EssenceDescriptor
	hr = CoCreateInstance(CLSID_AAFNetworkLocator,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFNetworkLocator, 
							(void **)&pNetLocator);		
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
	hr = pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pLocator->SetPath (TEST_PATH);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	hr = edesc->AppendLocator(pLocator);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Verify that there is now one locator
	hr = edesc->GetNumLocators(&numLocators);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (1 != numLocators)
		return AAFRESULT_TEST_FAILED;

	// Add the source mob into the tree
	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pFile->Close();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
 	// if (AAFRESULT_SUCCESS != hr)
	//	return hr;

	pMob->Release();
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFEssenceDescriptor		*pEdesc;
	IAAFSourceMob				*pSourceMob;
	IEnumAAFLocators *			pEnum;
	IAAFLocator	*				pLocator;
	aafInt32					numLocators, readLen;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr;
	aafWChar					readBuf[1024];

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
	//	return hr;

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
		aafWChar		name[500];
		aafUID_t		mobID;

		hr = mobIter->NextOne (&aMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = aMob->GetName (name, sizeof(name));
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = aMob->GetMobID (&mobID);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = pSourceMob->GetEssenceDescription (&pEdesc);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		// Verify that there is now one locator
		hr = pEdesc->GetNumLocators(&numLocators);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		if (1 != numLocators)
			return AAFRESULT_TEST_FAILED;
	
		hr = pEdesc->EnumAAFAllLocators(&pEnum);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		// This should read the one real locator
		hr = pEnum->NextOne(&pLocator);
		if (AAFRESULT_SUCCESS != hr)
			return hr;

		hr = pLocator->GetPathBufLen (&readLen);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
//		if(readLen != strlen(TEST_PATH))
			
		hr = pLocator->GetPath (readBuf, readLen);
		if (AAFRESULT_SUCCESS != hr)
			return hr;


	// This should run off the end
		hr = pEnum->NextOne(&pLocator);
		if (AAFRESULT_NO_MORE_OBJECTS != hr)
			return hr;
}

	//!!! Problem deleting, let it leak -- 	delete mobIter;
	hr = pFile->Close();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
	// if (AAFRESULT_SUCCESS != hr)
	//	return hr;

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	// if (pSession) pSession->Release();

	return 	AAFRESULT_SUCCESS;
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









