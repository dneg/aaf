// @doc INTERNAL
// @com This file implements the module test for CEnumAAFLocators
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
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
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x53c55746, 0x0406, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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

wchar_t *locator1 = L"First Locator";
wchar_t *locator2 = L"Second Locator";

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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFLocator	*				pLocator = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
//	aafUID_t					ddef = DDEF_Sound;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFLocators Test";
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
		
		// Initialize mob properties:
		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"EssenceDescriptorTest"));
		
		// Create a concrete subclass of the descriptor:
		checkResult(defs.cdAIFCDescriptor()->
					CreateInstance(IID_IAAFEssenceDescriptor, 
								   (IUnknown **)&edesc));		

		IAAFAIFCDescriptor*			pAIFCDesc = NULL;
		checkResult(edesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
		pAIFCDesc->Release();
		pAIFCDesc = NULL;

 		checkResult(pSourceMob->SetEssenceDescriptor (edesc));

			// Verify that there are no locators
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(0 == numLocators, AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		checkResult(defs.cdNetworkLocator()->
					CreateInstance(IID_IAAFLocator, 
								   (IUnknown **)&pLocator));		
		checkResult(pLocator->SetPath (locator1));
		checkResult(edesc->AppendLocator(pLocator));
		pLocator->Release();
		pLocator = NULL;

		// Verify that there is now one locator
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(1 == numLocators, AAFRESULT_TEST_FAILED);

		// Make a second ocator, and attach it to the EssenceDescriptor
		checkResult(defs.cdNetworkLocator()->
					CreateInstance(IID_IAAFLocator, 
								   (IUnknown **)&pLocator));		
		checkResult(pLocator->SetPath (locator2));
		checkResult(edesc->AppendLocator(pLocator));
		pLocator->Release();
		pLocator = NULL;

		// Add the source mob into the tree
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
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
	
	if (pDictionary)
    pDictionary->Release();

  if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
		if (bFileOpen)
		  {
			pFile->Save();
			pFile->Close();
		  }
 		pFile->Release();
	}

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
	IEnumAAFLocators *			pCloneEnum = NULL;
	IAAFLocator	*				pLocator = NULL;
	aafUInt32					numLocators;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
	wchar_t						testname[256];
	IAAFLocator				*	pArray[2] = { NULL, NULL };
	IAAFLocator				**	pArrayPoint = pArray;
	aafUInt32			resultCount;


	try
	{	
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

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

			// Verify that there is now two locators
			checkResult(pEdesc->CountLocators(&numLocators));
		  checkExpression(2 == numLocators, AAFRESULT_TEST_FAILED);
		
			checkResult(pEdesc->GetLocators(&pEnum));

			/* Read and check the first element */
			checkResult(pEnum->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator1) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;

			/**/
			/* Read and check the second element */
			checkResult(pEnum->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator2) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;
			/*****/
			
			/* Reset, and check the first element again*/
			checkResult(pEnum->Reset());
			checkResult(pEnum->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator1) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;
			
			/* Reset, Skip, and check the second element again*/
			checkResult(pEnum->Reset());
			checkResult(pEnum->Skip(1));
			checkResult(pEnum->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator2) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;

			/* Reset, and read both elements */
			checkResult(pEnum->Reset());
			checkResult(pEnum->Next (2, (IAAFLocator **)&pArray, &resultCount));
			checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
			checkResult(pArrayPoint[0]->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator1) == 0, AAFRESULT_TEST_FAILED);
			pArrayPoint[0]->Release();
			pArrayPoint[0] = NULL;
			
			checkResult(pArrayPoint[1]->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator2) == 0, AAFRESULT_TEST_FAILED);
			pArrayPoint[1]->Release();
			pArrayPoint[1] = NULL;
			
			/* Read one past to make sure that it fails */
			checkExpression(pEnum->NextOne(&pLocator) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
			/* Clone the enumerator, and read one element */
			checkResult(pEnum->Clone(&pCloneEnum));
			checkResult(pCloneEnum->Reset());
			checkResult(pCloneEnum->NextOne(&pLocator));
			checkResult(pLocator->GetPath (testname, sizeof(testname)));
			checkExpression(wcscmp(testname, locator1) == 0, AAFRESULT_TEST_FAILED);
			pLocator->Release();
			pLocator = NULL;

			pEnum->Release();
			pEnum = NULL;

			pEdesc->Release();
			pEdesc = NULL;

			pSourceMob->Release();
			pSourceMob = NULL;

			aMob->Release();
			aMob = NULL;
			pCloneEnum->Release();
			pCloneEnum = NULL;

		}
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
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
 
extern "C" HRESULT CEnumAAFLocators_test(testMode_t mode);
extern "C" HRESULT CEnumAAFLocators_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  aafWChar * pFileName = L"EnumAAFLocators.aaf";

  try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CEnumAAFLocators_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
