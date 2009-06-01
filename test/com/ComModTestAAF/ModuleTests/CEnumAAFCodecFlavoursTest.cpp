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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFCodecDefs.h"

#include "CAAFBuiltinDefs.h"

static aafBool  EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2)
{
    return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? kAAFTrue : kAAFFalse);
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

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFCodecDef*	pCodecDef = NULL;
	IAAFDefObject	*pDef = NULL;
	IAAFDataDef		*pDataDef = NULL;
	IAAFPluginManager *mgr = NULL;
	bool bFileOpen = false;
	HRESULT			hr = S_OK;
/*	long			test;
*/

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
    
		checkResult(AAFGetPluginManager(&mgr));
		checkResult(mgr->CreatePluginDefinition (kAAFCodecWAVE, pDictionary, &pDef));

		checkResult(pDef->QueryInterface(IID_IAAFCodecDef, (void **)&pCodecDef));
		checkResult(pCodecDef->AddEssenceKind (defs.ddkAAFSound()));
		checkResult(pDictionary->RegisterCodecDef(pCodecDef));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (pDef)
    pDef->Release();

  if (mgr)
    mgr->Release();

  if (pDataDef)
    pDataDef->Release();

  if (pCodecDef)
    pCodecDef->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
  {  // Close file
    if (bFileOpen)
	  {
		  pFile->Save();
		  pFile->Close();
	  }
    pFile->Release();
  }

  return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFCodecDef	*pCodec = NULL;
	IAAFDataDef		*pDataDef = NULL;
	IEnumAAFCodecFlavours	*pEnum = NULL;
	IEnumAAFCodecFlavours	*pCloneEnum = NULL;
	bool bFileOpen = false;
	aafBool			testResult;
	aafUID_t		codecID = kAAFCodecWAVE;
	// aafUID_t		testMatte = kAAFDataDef_Matte;
	aafUID_t		checkFlavour = kAAFNilCodecFlavour;
	aafUID_t		testFlavour;
	HRESULT			hr = S_OK;

	try
	{
	  // Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		checkResult(pDictionary->LookupCodecDef(codecID, &pCodec));

		checkResult(pCodec->IsEssenceKindSupported (defs.ddkAAFPicture(), &testResult));
		checkExpression (testResult == kAAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pCodec->EnumCodecFlavours (&pEnum));
		checkResult(pEnum->NextOne (&testFlavour));
		checkExpression (EqualAUID(&testFlavour, &checkFlavour) ? true : false,
						 AAFRESULT_TEST_FAILED);
		checkResult(pEnum->Reset ());
		checkResult(pEnum->NextOne (&testFlavour));
		checkExpression (EqualAUID(&testFlavour, &checkFlavour) ? true : false,
						 AAFRESULT_TEST_FAILED);

		// Test for correct "off end" behavior
		checkExpression(pEnum->NextOne (&testFlavour) == AAFRESULT_NO_MORE_FLAVOURS, AAFRESULT_TEST_FAILED);

		// Skip1 should also put us off of the end
		checkResult(pEnum->Reset ());
		checkExpression(pEnum->Skip (1) == AAFRESULT_NO_MORE_FLAVOURS, AAFRESULT_TEST_FAILED);

		// Check with Next()
		checkResult(pEnum->Reset ());
		checkResult(pEnum->Next(1,&testFlavour,NULL));
		checkExpression (EqualAUID(&testFlavour, &checkFlavour) ? true : false,
						 AAFRESULT_TEST_FAILED);
		
		// Check out clones version
		checkResult(pEnum->Reset ());
		checkResult(pEnum->Clone (&pCloneEnum));
		checkResult(pCloneEnum->Next(1,&testFlavour,NULL));
		checkExpression (EqualAUID(&testFlavour, &checkFlavour) ? true : false,
						 AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pCloneEnum)
		pCloneEnum->Release();
	if (pEnum)
		pEnum->Release();
	if (pDataDef)
		pDataDef->Release();
	if (pCodec)
		pCodec->Release();
	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CEnumAAFCodecFlavours_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFCodecFlavours_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CEnumAAFCodecFlavours_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFCodecFlavours tests have not been implemented:" << endl; 
//		cout << "     Next" << endl; 
//		cout << "     Clone" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}

