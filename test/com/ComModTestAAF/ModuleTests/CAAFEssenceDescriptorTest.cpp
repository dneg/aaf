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

#include "CAAFEssenceDescriptor.h"
#include "CAAFEssenceDescriptor.h"
#ifndef __CAAFEssenceDescriptor_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

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


  //@comm The number of locators may be zero if the essence is in the current file.


  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a new primary location).


  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a secondary location for the essence).




  //@comm The number of locators may be zero if the essence is in the current file.


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFLocator	*				pLocator = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	aafRational_t	audioRate = { 44100, 1 };
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	aafInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
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

	try 
	{
		/*
		hr = CoCreateInstance(CLSID_AAFSession,
								 NULL, 
								 CLSCTX_INPROC_SERVER, 
								 IID_IAAFSession, 
								 (void **)&pSession);

		hr = pSession->SetDefaultIdentification(&ProductInfo);
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		*/
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
 		
		//Make the first mob
		// Create a Mob
		CHECK(CoCreateInstance(CLSID_AAFSourceMob,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFSourceMob, 
								(void **)&pSourceMob));
		
		// Initialize mob properties:
		CHECK(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		CHECK(CoCreateGuid((GUID *)&newUID));
		CHECK(pMob->SetMobID(&newUID));
		CHECK(pMob->SetName(L"EssenceDescriptorTest"));
		
		// Create the descriptor:
		CHECK(CoCreateInstance(CLSID_AAFEssenceDescriptor,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFEssenceDescriptor, 
								(void **)&edesc));		
 		CHECK(pSourceMob->SetEssenceDescriptor (edesc));

			// Verify that there are no locators
		CHECK(edesc->GetNumLocators(&numLocators));
		if (0 != numLocators)
			CHECK(AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		CHECK(CoCreateInstance(CLSID_AAFLocator,
								NULL, 
								CLSCTX_INPROC_SERVER, 
								IID_IAAFLocator, 
								(void **)&pLocator));		

		CHECK(edesc->AppendLocator(pLocator));

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

	// Cleanup object references
	if (pLocator)
		pLocator->Release();

	if (edesc)
		edesc->Release();

	if (pMob)
		pMob->Release();

	if (pSourceMob)
		pSourceMob->Release();
	
	if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	/*
	hr = pSession->EndSession();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
	*/

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFMob			*aMob = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IEnumAAFLocators *			pEnum = NULL;
	IAAFLocator	*				pLocator = NULL;
	aafInt32					numLocators;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
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

			// This should run off the end
			pLocator->Release();
			pLocator = NULL;

			hr = pEnum->NextOne(&pLocator);
			if (AAFRESULT_NO_MORE_OBJECTS != hr)
				CHECK(AAFRESULT_TEST_FAILED);
			else
				hr = AAFRESULT_SUCCESS; // reset result
			

			pEnum->Release();
			pEnum = NULL;

			pEdesc->Release();
			pEdesc = NULL;

			pSourceMob->Release();
			pSourceMob = NULL;

			aMob->Release();
			aMob = NULL;
		}
	}
	catch (...)
	{
	}

	// Cleanup object references
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
	{	// Close file, clean-up and return
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	/*
	hr = pSession->EndSession();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
	*/

	return hr;
}
 
HRESULT CAAFEssenceDescriptor::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEssenceDescriptor *pObject = NULL;
  aafWChar * pFileName = L"EssenceDescTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFEssenceDescriptor::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;

	return hr;
}
