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

#include "CEnumAAFMobs.h"
#include "CEnumAAFMobs.h"
#ifndef __CEnumAAFMobs_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include "AAFResult.h"

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
	//	return hr;

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

	// Create a FileMob
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

	hr = CoCreateGuid((GUID *)&newUID); // hack: we need a utility function.
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	hr = pMob->SetName(L"File Mob");
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
 	hr = CoCreateInstance(CLSID_AAFFileDescriptor,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFFileDescriptor, 
							(void **)&edesc);		
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
 	hr = pSourceMob->SetEssenceDescriptor (edesc);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Create a MasterMob
	hr = CoCreateInstance(CLSID_AAFMasterMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);


	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = CoCreateGuid((GUID *)&newUID); // hack: we need a utility function.
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	hr = pMob->SetName(L"Master Mob");
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Create a CompositionMob
	hr = CoCreateInstance(CLSID_AAFCompositionMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFMob, 
							(void **)&pMob);


	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = CoCreateGuid((GUID *)&newUID); // hack: we need a utility function.
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pMob->SetMobID(&newUID);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	
	hr = pMob->SetName(L"Composition Mob");
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	hr = pHeader->AppendMob(pMob);
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Close the file and get out of here
	hr = pFile->Close();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// hr = pSession->EndSession();
 	// if (AAFRESULT_SUCCESS != hr)
	// 	return hr;

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
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs;
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
	//	return hr;

	// hr = pSession->OpenReadFile(pFileName, &pFile);
	hr = pFile->OpenExistingRead(pFileName, 0);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
  
  	hr = pFile->GetHeader(&pHeader);
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	// Make sure that we have one master, one file, and one composition (three total)
	hr = pHeader->GetNumMobs(kAllMob, &numMobs);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (3 != numMobs )
		return AAFRESULT_TEST_FAILED;

	hr = pHeader->GetNumMobs(kMasterMob, &numMobs);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (1 != numMobs )
		return AAFRESULT_TEST_FAILED;

	hr = pHeader->GetNumMobs(kFileMob, &numMobs);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (1 != numMobs )
		return AAFRESULT_TEST_FAILED;

	hr = pHeader->GetNumMobs(kCompMob, &numMobs);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
	if (1 != numMobs )
		return AAFRESULT_TEST_FAILED;

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
 
HRESULT CEnumAAFMobs::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFSourceMob *pObject = NULL;
 	aafWChar * pFileName = L"EnumMOBTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(AAFRESULT_SUCCESS == hr)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFSourceMob::test...Caught general C++"
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














