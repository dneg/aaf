// @doc INTERNAL
// @com This file implements the module test for CAAFNetworkLocator
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=




#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#define TEST_PATH	L"AnotherFile.aaf"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xe4b74fbe, 0x0403, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}


static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFLocator	*				pLocator = NULL;
	IAAFNetworkLocator *		pNetLocator = NULL;
	aafProductIdentification_t	ProductInfo;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	aafUInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;


	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFNetworkLocator Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;


	try
	{
	    // Remove the previous test file if any.
	    RemoveTestFile(pFileName);


		// Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
 
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
		//Make the first mob
		// Create a Mob
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));

		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"SourceMOBTest"));
		
		// Create a concrete subclass of EssenceDescriptor
		checkResult(defs.cdAIFCDescriptor()->
					CreateInstance(IID_IAAFEssenceDescriptor, 
								   (IUnknown **)&edesc));
										

		IAAFAIFCDescriptor*			pAIFCDesc = NULL;
		checkResult(edesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
		pAIFCDesc->Release();
		pAIFCDesc = NULL;

 		checkResult(pSourceMob->SetEssenceDescriptor(edesc));

			// Verify that there are no locators
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(0 == numLocators, AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		checkResult(defs.cdNetworkLocator()->
					CreateInstance(IID_IAAFNetworkLocator, 
								   (IUnknown **)&pNetLocator));		
		checkResult(pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));

		checkResult(pLocator->SetPath (TEST_PATH));
		
		checkResult(edesc->AppendLocator(pLocator));
 		checkResult(pSourceMob->SetEssenceDescriptor (edesc));

		// Verify that there is now one locator
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(1 == numLocators, AAFRESULT_TEST_FAILED);

		// Add the source mob into the tree
		checkResult(pHeader->AddMob(pMob));

	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
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

  if (pDictionary)
    pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		if (bFileOpen)
		  {
			pFile->Save();
			pFile->Close();
		  }
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
	aafUInt32					numLocators;
	aafUInt32					readLen;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafWChar					readBuf[1024];
	bool bFileOpen = false;


	try
	{	  
    // Open the file.
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

  	checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		if (1 != numMobs )
			checkResult(AAFRESULT_TEST_FAILED);


	//!!!	aafSearchCrit_t		criteria;
	//!!!	criteria.searchTag = kAAFNoSearch;

		checkResult(pHeader->GetMobs (NULL, &mobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafMobID_t		mobID;

			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));

			checkResult(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));

			// Verify that there is now one locator
			checkResult(pEdesc->CountLocators(&numLocators));
			if (1 != numLocators)
				checkResult(AAFRESULT_TEST_FAILED);
		
			checkResult(pEdesc->GetLocators(&pEnum));

			// This should read the one real locator
			checkResult(pEnum->NextOne(&pLocator));

			checkResult(pLocator->GetPathBufLen (&readLen));
	//		if(readLen != strlen(TEST_PATH))
				
			checkResult(pLocator->GetPath (readBuf, readLen));

		// This should run off the end
			pLocator->Release();
			pLocator = NULL;
			hr = pEnum->NextOne(&pLocator);
			if (AAFRESULT_NO_MORE_OBJECTS != hr)
				checkResult(hr);
			else
				hr = AAFRESULT_SUCCESS; // reset result
		}


	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
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
 
extern "C" HRESULT CAAFNetworkLocator_test(testMode_t mode);
extern "C" HRESULT CAAFNetworkLocator_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar * pFileName = L"AAFNetworkLocatorTest.aaf";

  try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFNetworkLocator_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


  return hr;
}









