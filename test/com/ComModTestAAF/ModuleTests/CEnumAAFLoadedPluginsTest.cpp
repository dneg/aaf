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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include <iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>

#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"

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

extern "C" HRESULT CEnumAAFLoadedPlugins_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFLoadedPlugins_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IEnumAAFLoadedPlugins	*pEnum = NULL, *pCloneEnum = NULL;
	IAAFPluginManager		*pMgr = NULL;
	aafUID_t				testUID;
	IAAFFile*		pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFDefObject	*pPluginDef = NULL;
	aafInt32		numPlugins, checkNumPlugins;

	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode != kAAFUnitTestReadWrite)
			return AAFRESULT_SUCCESS;		// Can't run this test read-only

		// Remove the previous test file if any.
		RemoveTestFile(pFileName);		
		
		checkResult(AAFGetPluginManager (&pMgr));

		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		checkResult(pMgr->EnumLoadedPlugins (AUID_AAFCodecDef, &pEnum));
		numPlugins = 0;
		while(pEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkResult(pMgr->CreatePluginDefinition (testUID, pDictionary, &pPluginDef));
			pPluginDef->Release();
			pPluginDef = NULL;
			numPlugins++;
		}
		
		// Test reset
		checkResult(pEnum->Reset ());
		checkNumPlugins = 0;
		while(pEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkNumPlugins++;
		}
		checkExpression(numPlugins == checkNumPlugins, AAFRESULT_TEST_FAILED);
		
		// Test Next
		checkResult(pEnum->Reset ());
		checkNumPlugins = 0;
		while(pEnum->Next (1, &testUID, NULL) == AAFRESULT_SUCCESS)
		{
			checkNumPlugins++;
		}
		checkExpression(numPlugins == checkNumPlugins, AAFRESULT_TEST_FAILED);
		
		// Test skip
		checkResult(pEnum->Reset ());
		checkResult(pEnum->Skip (1));
		checkNumPlugins = 0;
		while(pEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkNumPlugins++;
		}
		checkExpression((numPlugins-1) == checkNumPlugins, AAFRESULT_TEST_FAILED);

		// Test Clone
		checkResult(pEnum->Reset ());
		checkResult(pEnum->Clone (&pCloneEnum));
		checkNumPlugins = 0;
		while(pCloneEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkNumPlugins++;
		}
		checkExpression(numPlugins == checkNumPlugins, AAFRESULT_TEST_FAILED);
	}
	catch (...)
	{
		cerr << "CEnumAAFLoadedPlugins_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	if (pCloneEnum)
		pCloneEnum->Release();


	if (pPluginDef)
		pPluginDef->Release();
	
	if (pEnum)
		pEnum->Release();
	
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

	if (pMgr)
		pMgr->Release();
	
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFLoadedPlugins tests have not been implemented:" << endl; 
//		cout << "     Next" << endl; 
//		cout << "     Skip" << endl; 
//		cout << "     Reset" << endl; 
///		cout << "     Clone" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
