// @doc INTERNAL
// @com This file implements the module test for CAAFTextLocator interface
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
 *  prior written permission of Avid Technology, Inc.
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

#define TEST_NAME	L"This is a text locator"

static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafTapeCaseType_t FormFactor = kVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kPALSignal;
static aafTapeFormatType_t TapeFormat = kVHSFormat;
static aafLength_t TapeLength = 3200 ;

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTextLocator; // generated

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
	IAAFFile*					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				 pDictionary = NULL;
	IAAFLocator*				pLocator = NULL;
	IAAFTextLocator*			pTextLocator = NULL;
	aafProductIdentification_t	ProductInfo;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob						*pMob = NULL;
	IAAFEssenceDescriptor		*edesc = NULL;
	IAAFTapeDescriptor*			pTapeDescriptor = NULL;
	aafUID_t					newUID;
	aafInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;
	aafRational_t	audioRate = { 44100, 1 };


	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTextLocator Test";
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
 		
		//Make the first mob
		// Create a Mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFSourceMob,
								IID_IAAFSourceMob, 
								(IUnknown **)&pSourceMob));

		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newUID));
		checkResult(pMob->SetMobID(&newUID));
		checkResult(pMob->SetName(L"TextLocatorTestSourceMOB"));
		
		checkResult(pDictionary->CreateInstance(&AUID_AAFTapeDescriptor,
								IID_IAAFTapeDescriptor, 
								(IUnknown **)&pTapeDescriptor));
		
		checkResult(pTapeDescriptor->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&edesc));
		// Set tape properties
		checkResult(pTapeDescriptor->SetTapeManufacturer( Manufacturer ));
		checkResult(pTapeDescriptor->SetTapeModel( Model ));
		checkResult(pTapeDescriptor->SetTapeFormFactor( FormFactor ));
		checkResult(pTapeDescriptor->SetSignalType( VideoSignalType ));
		checkResult(pTapeDescriptor->SetTapeFormat( TapeFormat ));
		checkResult(pTapeDescriptor->SetTapeLength( TapeLength ));

 		checkResult(pSourceMob->SetEssenceDescriptor(edesc));

			// Verify that there are no locators
		checkResult(edesc->GetNumLocators(&numLocators));
		checkExpression(0 == numLocators, AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		checkResult(pDictionary->CreateInstance(&AUID_AAFTextLocator,
								IID_IAAFTextLocator, 
								(IUnknown **)&pTextLocator));		
		checkResult(pTextLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));

		checkResult(pTextLocator->SetName (TEST_NAME));
		
		checkResult(edesc->AppendLocator(pLocator));
 		checkResult(pSourceMob->SetEssenceDescriptor (edesc));

		// Verify that there is now one locator
		checkResult(edesc->GetNumLocators(&numLocators));
		checkExpression(1 == numLocators, AAFRESULT_TEST_FAILED);

		// Add the source mob into the tree
		checkResult(pHeader->AppendMob(pMob));

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}	

	// cleanup
	if (pLocator)
		pLocator->Release();

	if (pTextLocator)
		pTextLocator->Release();

	if (pTapeDescriptor)
		pTapeDescriptor->Release();
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
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IAAFEssenceDescriptor*	pEdesc = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IEnumAAFLocators*		pEnum = NULL;
	IAAFLocator*			pLocator = NULL;
	IAAFTextLocator*		pTextLocator = NULL;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	aafInt32				numLocators, readLen;
	aafNumSlots_t			numMobs, n;

	HRESULT					hr = AAFRESULT_SUCCESS;
	aafWChar					readBuf[1024];
	bool bFileOpen = false;


	try
	{	  
	    // Open the file.
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

  		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		if (1 != numMobs )
			checkResult(AAFRESULT_TEST_FAILED);


		checkResult(pHeader->EnumAAFAllMobs (NULL, &pMobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafUID_t		mobID;

			checkResult(pMobIter->NextOne (&pMob));
			checkResult(pMob->GetName (name, sizeof(name)));
			checkResult(pMob->GetMobID (&mobID));

			checkResult(pMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));

			// Verify that there is now one locator
			checkResult(pEdesc->GetNumLocators(&numLocators));
			if (1 != numLocators)
				checkResult(AAFRESULT_TEST_FAILED);
		
			checkResult(pEdesc->EnumAAFAllLocators(&pEnum));

			// This should read the one real locator
			checkResult(pEnum->NextOne(&pLocator));
			checkResult(pLocator->QueryInterface(IID_IAAFTextLocator, (void **)&pTextLocator));
			checkResult(pTextLocator->GetNameBufLen (&readLen));
	//		if(readLen != strlen(TEST_PATH))
				
			checkResult(pTextLocator->GetName (readBuf, readLen));

		// This should run off the end
			pLocator->Release();
			pLocator = NULL;
			pTextLocator->Release();
			pTextLocator = NULL;
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

	if (pTextLocator)
		pTextLocator->Release();

	if (pEnum)
		pEnum->Release();

	if (pEdesc)
		pEdesc->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

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
 
extern "C" HRESULT CAAFTextLocator_test()
{
  HRESULT	hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar*	pFileName = L"AAFTextLocatorTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFTextLocator_test...Caught general C++"
		" exception!" << endl; 
	}


  return hr;
}














