 

// @doc INTERNAL
// @com This file implements the module test for CAAFEffectDef

/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/

#include "CAAFEffectDef.h"
#include "CAAFEffectDef.h"
#ifndef __CAAFEffectDef_h__
#error - improperly defined include guard
#endif

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEffectDef; // generated


#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "aafUtils.h"

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

#define TEST_NUM_INPUTS		1
#define TEST_CATEGORY		L"Test Effects"
#define TEST_BYPASS			1

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMasterMob Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	if(mode == kMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);

	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFEffectDef*		pEffectDef = NULL;
	IAAFPluggableDef*	pPlugDef = NULL;
	IAAFParameterDef*	pParamDef = NULL;	
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUID_t			testDataDef = DDEF_Video;
/*	long				test;
*/

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
    
		checkResult(pDictionary->CreateInstance(&AUID_AAFEffectDef,
							  IID_IAAFEffectDef, 
							  (IUnknown **)&pEffectDef));
    
		checkResult(pDictionary->CreateInstance(&AUID_AAFParameterDef,
							  IID_IAAFParameterDef, 
							  (IUnknown **)&pParamDef));

		checkResult(pDictionary->RegisterEffectDefinition(pEffectDef));
		checkResult(pParamDef->QueryInterface(IID_IAAFPluggableDef, (void **)&pPlugDef));
		checkResult(pDictionary->RegisterPluggableDefinition(pPlugDef));
		pPlugDef->Release();
		pPlugDef = NULL;

		checkResult(pEffectDef->SetDataDefinitionID (&testDataDef));
		checkResult(pEffectDef->SetIsTimeWarp (AAFFalse));
		checkResult(pEffectDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pEffectDef->SetCategory (TEST_CATEGORY));
		checkResult(pEffectDef->AddParameterDefs (pParamDef));
		checkResult(pEffectDef->SetBypass (TEST_BYPASS));
#if 0
		checkResult(pEffectDef->PrependDegradeToEffects (ImplAAFEffectDef  *pEffectDef));
		checkResult(pEffectDef->AppendDegradeToEffects (ImplAAFEffectDef  *pEffectDef));
#endif
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


	// Cleanup and return
	if (pEffectDef)
		pEffectDef->Release();

	if (pPlugDef)
		pPlugDef->Release();

	if (pParamDef)
		pParamDef->Release();

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
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFEffectDefs *pEffectEnum = NULL;
	IAAFEffectDef		*pEffectDef = NULL;
	bool				bFileOpen = false;
	aafUID_t			readDataDef, checkDataDef = DDEF_Video;
	aafBool				readIsTimeWarp;
	aafInt32			catLen, testLen, checkNumInputs;
	aafUInt32			checkBypass;
	HRESULT				hr = S_OK;
	wchar_t				checkCat[256];

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetEffectDefinitions(&pEffectEnum));
		checkResult(pEffectEnum->NextOne (&pEffectDef));
		checkResult(pEffectDef->GetDataDefinitionID(&readDataDef));
		checkExpression(EqualAUID(&readDataDef, &checkDataDef) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pEffectDef->IsTimeWarp (&readIsTimeWarp));
		checkExpression(readIsTimeWarp == AAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pEffectDef->GetCategoryBufLen (&catLen));
		testLen = wcslen(TEST_CATEGORY);
		checkResult(pEffectDef->GetCategory (checkCat, sizeof(checkCat)));
		checkExpression(wcscmp(checkCat, TEST_CATEGORY) == 0, AAFRESULT_TEST_FAILED);
		checkExpression(testLen == wcslen(checkCat), AAFRESULT_TEST_FAILED);
		checkResult(pEffectDef->GetBypass (&checkBypass));
		checkExpression(checkBypass == TEST_BYPASS, AAFRESULT_TEST_FAILED);
		checkResult(pEffectDef->GetNumberInputs (&checkNumInputs));
		checkExpression(checkNumInputs == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
		
#if 0
    ImplAAFEffectDef::GetParameterDefinitions (ImplEnumAAFParameterDefs **ppEnum)
    ImplAAFEffectDef::GetDegradeToEffects (ImplEnumAAFEffectDefs  **ppEnum)
#endif
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pHeader)
		pHeader->Release();
      
	if (pDictionary)
		pDictionary->Release();
      
	if (pEffectEnum)
		pEffectEnum->Release();
      
	if (pEffectDef)
		pEffectDef->Release();
      
	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

HRESULT CAAFEffectDef::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EffectDefTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFEffectDef::test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following IAAFEffectDef methods have not been implemented:" << endl; 
		cout << "     PrependDegradeToEffects" << endl; 
		cout << "     AppendDegradeToEffects" << endl; 
		cout << "     GetDegradeToEffects" << endl; 
		cout << "     GetParameterDefinitions" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}
























































