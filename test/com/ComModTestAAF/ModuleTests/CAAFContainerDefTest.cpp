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
#include "AAFDefUIDs.h"
#include "AAFContainerDefs.h"

#include "CAAFBuiltinDefs.h"

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}

// {4E84045D-0F29-11d4-A359-009027DFCA6A}
static const aafUID_t testUID = 
{ 0x4e84045d, 0xf29, 0x11d4, { 0xa3, 0x59, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

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
	IAAFContainerDef*	pContainerDef = NULL;
	bool bFileOpen = false;
	aafUID_t		uid = testUID;
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

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
    
		CAAFBuiltinDefs defs (pDictionary);
		checkResult(defs.cdContainerDef()->
					CreateInstance(IID_IAAFContainerDef, 
								   (IUnknown **)&pContainerDef));
    
		checkResult(pContainerDef->Initialize(uid, L"Test Container", L"Test Container Definition"));

		checkResult(pContainerDef->SetEssenceIsIdentified (kAAFTrue));

		checkResult(pDictionary->RegisterContainerDef(pContainerDef));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (pContainerDef)
    pContainerDef->Release();

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
	IEnumAAFContainerDefs *pPlug = NULL;
	IAAFContainerDef		*pPlugDef = NULL;
	IAAFDefObject		*pDef = NULL;
	bool bFileOpen = false;
	aafBool			testBool;
	aafUID_t		readUID;
	HRESULT			hr = S_OK;

	try
	{
	  // Open the AAF file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetContainerDefs(&pPlug));
		while(pPlug->NextOne (&pPlugDef) == AAFRESULT_SUCCESS)
		{
			checkResult(pPlugDef->QueryInterface (IID_IAAFDefObject, (void **)&pDef));
			checkResult(pDef->GetAUID(&readUID));
			if(memcmp(&readUID, &testUID, sizeof(aafUID_t)) == 0)
			{
				checkResult(pPlugDef->EssenceIsIdentified (&testBool));
				checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
				break;
			}
			pDef->Release();
			pDef = NULL;
			pPlugDef->Release();
			pPlugDef = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pDef)
		pDef->Release();

	if (pPlugDef)
		pPlugDef->Release();

	if (pPlug)
		pPlug->Release();

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
 

extern "C" HRESULT CAAFContainerDef_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFContainerDef_test(
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
		cerr << "CAAFContainerDef_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}



































