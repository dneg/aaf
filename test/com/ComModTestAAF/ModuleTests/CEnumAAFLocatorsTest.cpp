// @doc INTERNAL
// @com This file implements the module test for CEnumAAFLocators
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


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
	aafRational_t	audioRate = { 44100, 1 };
	aafProductIdentification_t	ProductInfo;
	aafMobID_t					newMobID;
	aafUInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
//	aafUID_t					ddef = DDEF_Sound;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFLocators Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
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
		checkResult(pDictionary->CreateInstance(defs.cdSourceMob(),
								IID_IAAFSourceMob, 
								(IUnknown **)&pSourceMob));
		
		// Initialize mob properties:
		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(newMobID));
		checkResult(pMob->SetName(L"EssenceDescriptorTest"));
		
		// Create the descriptor:
		checkResult(pDictionary->CreateInstance(defs.cdEssenceDescriptor(),
								IID_IAAFEssenceDescriptor, 
								(IUnknown **)&edesc));		
 		checkResult(pSourceMob->SetEssenceDescriptor (edesc));

			// Verify that there are no locators
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(0 == numLocators, AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		checkResult(pDictionary->CreateInstance(defs.cdNetworkLocator(),
								IID_IAAFLocator, 
								(IUnknown **)&pLocator));		
		checkResult(pLocator->SetPath (locator1));
		checkResult(edesc->AppendLocator(pLocator));
		pLocator->Release();
		pLocator = NULL;

		// Verify that there is now one locator
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(1 == numLocators, AAFRESULT_TEST_FAILED);

		// Make a second ocator, and attach it to the EssenceDescriptor
		checkResult(pDictionary->CreateInstance(defs.cdNetworkLocator(),
								IID_IAAFLocator, 
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
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
	wchar_t						testname[256];
	IAAFLocator				*	pArray[2] = { NULL, NULL };
	IAAFLocator				**	pArrayPoint = pArray;
	aafUInt32			resultCount;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"EnumAAFLocators Test. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;


	try
	{	
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAllMob, &numMobs));
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
 
extern "C" HRESULT CEnumAAFLocators_test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  aafWChar * pFileName = L"EnumAAFLocators.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CEnumAAFLocators_test...Caught general C++"
		" exception!" << endl; 
	}

	return hr;
}
