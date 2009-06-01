//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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

#define TEST_NAME	L"This is a text locator"

static const aafWChar* Manufacturer = L"Sony";
static const aafWChar* Model = L"MyModel";
static aafTapeCaseType_t FormFactor = kAAFVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kAAFPALSignal;
static aafTapeFormatType_t TapeFormat = kAAFVHSFormat;
static aafUInt32 TapeLength = 3200 ;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x4c649116, 0x0405, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTextLocator; // generated

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

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*					pFile = NULL;
	bool						bFileOpen = false;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*				 pDictionary = NULL;
	IAAFLocator*				pLocator = NULL;
	IAAFTextLocator*			pTextLocator = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob						*pMob = NULL;
	IAAFEssenceDescriptor		*edesc = NULL;
	IAAFTapeDescriptor*			pTapeDescriptor = NULL;
	aafUInt32					numLocators;
	HRESULT						hr = AAFRESULT_SUCCESS;


	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the file.
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
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
		checkResult(pMob->SetName(L"TextLocatorTestSourceMOB"));
		
		checkResult(defs.cdTapeDescriptor()->
					CreateInstance(IID_IAAFTapeDescriptor, 
								   (IUnknown **)&pTapeDescriptor));
		
		checkResult(pTapeDescriptor->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&edesc));
		// Set tape properties
		checkResult(pTapeDescriptor->SetTapeManufacturer( Manufacturer ));
		checkResult(pTapeDescriptor->SetTapeModel( Model ));
		checkResult(pTapeDescriptor->SetTapeFormFactor( FormFactor ));
		checkResult(pTapeDescriptor->SetSignalType( VideoSignalType ));
		checkResult(pTapeDescriptor->SetTapeFormat( TapeFormat ));
		checkResult(pTapeDescriptor->SetTapeLength( TapeLength ));

			// Verify that there are no locators
		checkResult(edesc->CountLocators(&numLocators));
		checkExpression(0 == numLocators, AAFRESULT_TEST_FAILED);

  
		// Make a locator, and attach it to the EssenceDescriptor
		checkResult(defs.cdTextLocator()->
					CreateInstance(IID_IAAFTextLocator, 
								   (IUnknown **)&pTextLocator));		
		checkResult(pTextLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));

		checkResult(pTextLocator->SetName (TEST_NAME));
		
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
	aafUInt32				numLocators;
	aafUInt32				readLen;
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

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		if (1 != numMobs )
			checkResult(AAFRESULT_TEST_FAILED);


		checkResult(pHeader->GetMobs (NULL, &pMobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafMobID_t		mobID;

			checkResult(pMobIter->NextOne (&pMob));
			checkResult(pMob->GetName (name, sizeof(name)));
			checkResult(pMob->GetMobID (&mobID));

			checkResult(pMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));

			// Verify that there is now one locator
			checkResult(pEdesc->CountLocators(&numLocators));
			if (1 != numLocators)
				checkResult(AAFRESULT_TEST_FAILED);
		
			checkResult(pEdesc->GetLocators(&pEnum));

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
 
extern "C" HRESULT CAAFTextLocator_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTextLocator_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT	hr = AAFRESULT_NOT_IMPLEMENTED;
  const size_t fileNameBufLen = 128;
  aafWChar pFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

  try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if (AAFRESULT_SUCCESS != hr)
			return hr;
		hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFTextLocator_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


  return hr;
}














