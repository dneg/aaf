// @doc INTERNAL
// @com This file implements the module test for CAAFPluginManager

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



#include <iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>

#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"
#include "AAF.h"
#include "AAFPlugin.h"
#include "AAFPlugin_i.c"

const CLSID CLSID_AAFBasicInterp = { 0x5B6C85A1, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

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

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFPluginManager Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kAAFMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		if (*ppFile)
		{
			(*ppFile)->Release();
			*ppFile = NULL;
		}
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

extern "C" HRESULT CAAFPluginManager_test(testMode_t );
extern "C" HRESULT CAAFPluginManager_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IEnumAAFLoadedPlugins	*pEnum = NULL;
	IAAFPluginManager		*pMgr = NULL;
	aafUID_t				testUID;
	IAAFFile*		pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	aafWChar * pFileName = L"AAFPluginManagerTest.aaf";
	IAAFDefObject	*pPluginDef = NULL;
	IAAFPlugin	*plugin			= NULL;

	try
	{
		if(mode != kAAFUnitTestReadWrite)
			return AAFRESULT_SUCCESS;			// Can't run this test read-only

		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		
		checkResult(AAFGetPluginManager (&pMgr));
		
		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));
		bFileOpen = true;
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));

		checkResult(pMgr->EnumLoadedPlugins (AUID_AAFCodecDef, &pEnum));
		while(pEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkResult(pMgr->CreatePluginDefinition (testUID, pDictionary, &pPluginDef));
			pPluginDef->Release();
			pPluginDef = NULL;
		}
		hr = AAFRESULT_SUCCESS;

		checkResult(pMgr->CreateInstance(CLSID_AAFBasicInterp,
			NULL, 
			IID_IAAFPlugin, 
			(void **)&plugin));
	}
	catch (HRESULT& rhr)
	{
		hr = rhr;
	}

	if (plugin)
		plugin->Release();

	if (pEnum)
		pEnum->Release();

	if (pMgr)
		pMgr->Release();

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
	
	// JeffB: RegisterPluginDirectory and RegisterPluginFile have their implementations tested
	// by the RegisterStandard() function, and will require a special local plugin to test.  Therefore
	// I'm pushing the test off post 1.0 and declaring success here.
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFPluginManager tests have not been implemented:" << endl; 
////!!!		cout << "     RegisterPluginDirectory" << endl; 
////!!!		cout << "     RegisterPluginFile" << endl; 
////		cout << "     EnumLoadedPlugins" << endl; 
//		cout << "     CreateInstance" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}







