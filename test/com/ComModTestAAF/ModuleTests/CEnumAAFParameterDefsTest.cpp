// @doc INTERNAL
// @com This file implements the module test for CEnumAAFParameterDefs

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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
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
static const aafUID_t TEST_CATEGORY = 
{ 0x9f0e730c, 0xbf8, 0x11d4, { 0xa3, 0x58, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };
#define TEST_BYPASS			1
static const aafUID_t TEST_EFFECT_AUID = 
{ 0x12684769, 0xcd02, 0x11d3, { 0xa3, 0xf4, 0x0, 0x4, 0xac, 0x96, 0xa9, 0x37 } };
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME1	L"A TestEffect parameter"
#define TEST_PARAM_DESC1	L"A longer description of the TestEffect parameter"
#define TEST_PARAM_NAME2	L"Another TestEffect parameter"
#define TEST_PARAM_DESC2	L"Another longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"

aafUID_t TestParamUID1 = { 0x75546871, 0x28CC, 0x11d3, { 0x80, 0xAE, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
aafUID_t TestParamUID2 = { 0x75546872, 0x28CC, 0x11d3, { 0x80, 0xAE, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

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
	ProductInfo.productName = L"EnumAAFParameterDefs Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	if(mode == kAAFMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);

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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFOperationDef*		pOperationDef = NULL;
	IAAFParameterDef*	pParamDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
/*	long				test;
*/

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
    
		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));
		checkResult(pOperationDef->Initialize(TEST_EFFECT_AUID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
  		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pOperationDef->SetDataDef (defs.ddPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));

		// Add two parameter defs, so that we can test all functions
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

		checkResult(pParamDef->Initialize (TestParamUID1, TEST_PARAM_NAME1, TEST_PARAM_DESC1, defs.tdRational()));
		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		pParamDef->Release();
		pParamDef = NULL;

		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pParamDef->Initialize (TestParamUID2, TEST_PARAM_NAME2, TEST_PARAM_DESC2, defs.tdRational()));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		pParamDef->Release();
		pParamDef = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


	// Cleanup and return
	if (pOperationDef)
		pOperationDef->Release();

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
	IEnumAAFOperationDefs *pEffectEnum = NULL;
	IEnumAAFParameterDefs *pParmDefEnum = NULL;
	IEnumAAFParameterDefs *pCloneEnum = NULL;
	IAAFOperationDef		*pOperationDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFParameterDef*	pArrayDef[2] = { NULL, NULL };
	IAAFDefObject*		pDefObject = NULL;
	bool				bFileOpen = false;
	aafBool				readIsTimeWarp;
	aafInt32			checkNumInputs;
	aafUInt32			checkBypass;
	HRESULT				hr = S_OK;
	wchar_t				checkName[256];
	aafUID_t			checkCat;
	wchar_t				testString[256];
	aafUInt32			resultCount;
	IAAFDataDefSP		pReadDataDef;

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
	
		checkResult(pDictionary->GetOperationDefs(&pEffectEnum));
		checkResult(pEffectEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->GetDataDef(&pReadDataDef));

		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_EFFECT_NAME) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_EFFECT_DESC) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		
		aafBool bResult = kAAFFalse;
		checkResult(pReadDataDef->IsDataDefOf(defs.ddPicture(), &bResult));
		checkExpression(bResult == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->IsTimeWarp (&readIsTimeWarp));
		checkExpression(readIsTimeWarp == kAAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetCategory (&checkCat));
		checkExpression(memcmp(&checkCat, &TEST_CATEGORY, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetBypass (&checkBypass));
		checkExpression(checkBypass == TEST_BYPASS, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetNumberInputs (&checkNumInputs));
		checkExpression(checkNumInputs == TEST_NUM_INPUTS, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetParameterDefs (&pParmDefEnum));

		/* Read and check the first element */
		checkResult(pParmDefEnum->NextOne (&pParmDef));
		checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		pParmDef->Release();
		pParmDef = NULL;
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME1) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC1) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;

		/* Read and check the second element */
		checkResult(pParmDefEnum->NextOne (&pParmDef));
		checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		pParmDef->Release();
		pParmDef = NULL;
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME2) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC2) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;

		/* Reset, and check the first element again*/
		checkResult(pParmDefEnum->Reset());
		checkResult(pParmDefEnum->NextOne (&pParmDef));
		checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		pParmDef->Release();
		pParmDef = NULL;
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME1) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC1) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;

		/* Reset, Skip, and check the second element again*/
		checkResult(pParmDefEnum->Reset());
		checkResult(pParmDefEnum->Skip(1));
		checkResult(pParmDefEnum->NextOne (&pParmDef));
		checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		pParmDef->Release();
		pParmDef = NULL;
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME2) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC2) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		/* Reset, and read both elements */
		checkResult(pParmDefEnum->Reset());
		checkResult(pParmDefEnum->Next (2, (IAAFParameterDef **)&pArrayDef, &resultCount));
		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		checkResult(pArrayDef[0]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDefObject));

		checkResult(pDefObject->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, TEST_PARAM_NAME1) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		checkResult(pArrayDef[1]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDefObject));

		checkResult(pDefObject->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, TEST_PARAM_NAME2) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		pArrayDef[0]->Release();
		pArrayDef[0] = NULL;
		pArrayDef[1]->Release();
		pArrayDef[1] = NULL;

		/* Read one past to make sure that it fails */
		checkExpression(pParmDefEnum->NextOne(&pParmDef) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		/* Clone the enumerator, and read one element */
		checkResult(pParmDefEnum->Clone(&pCloneEnum));
		checkResult(pCloneEnum->Reset());
		checkResult(pCloneEnum->NextOne (&pParmDef));
		checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		pParmDef->Release();
		pParmDef = NULL;
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME1) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC1) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
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

	if (pCloneEnum)
		pCloneEnum->Release();

	if (pParmDefEnum)
		pParmDefEnum->Release();
      
	if (pOperationDef)
		pOperationDef->Release();
      
	if (pDefObject)
		pDefObject->Release();

	if (pParmDef)
		pParmDef->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CEnumAAFParameterDefs_test(testMode_t mode);
extern "C" HRESULT CEnumAAFParameterDefs_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFParameterDefsTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CEnumAAFParameterDefs_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//!!!	if (SUCCEEDED(hr))
//	{
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
