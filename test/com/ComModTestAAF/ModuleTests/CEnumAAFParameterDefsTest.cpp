// @doc INTERNAL
// @com This file implements the module test for CEnumAAFParameterDefs

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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "aafUtils.h"
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
#define TEST_CATEGORY		L"Test Parameters"
#define TEST_BYPASS			1
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

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFParameterDefs Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	if(mode == kMediaOpenAppend)
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
	IAAFDefObject*		pDefObject = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
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
		CAAFBuiltinDefs defs (pDictionary);
    
		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));
  		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pOperationDef->SetDataDef (defs.ddPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (AAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->SetName (TEST_EFFECT_NAME));
		checkResult(pDefObject->SetDescription (TEST_EFFECT_DESC));
		pDefObject->Release();
		pDefObject = NULL;

		// !!!Added circular definitions because we don't have optional properties
		checkResult(pOperationDef->AppendDegradeToOperation (pOperationDef));

		// Add two parameter defs, so that we can test all functions
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Initialize (TestParamUID1, TEST_PARAM_NAME1, TEST_PARAM_DESC1));
		pDefObject->Release();
		pDefObject = NULL;
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		pParamDef->Release();
		pParamDef = NULL;

		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Initialize (TestParamUID2, TEST_PARAM_NAME2, TEST_PARAM_DESC2));
		pDefObject->Release();
		pDefObject = NULL;
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
	if (pDefObject)
		pDefObject->Release();

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
	aafUInt32			catLen;
	aafInt32			checkNumInputs;
	aafUInt32			checkBypass, testLen;
	HRESULT				hr = S_OK;
	wchar_t				checkCat[256], checkName[256];
	wchar_t				testString[256];
	aafUInt32			resultCount;
	IAAFDataDefSP		pReadDataDef;

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
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
		
		aafBool bResult = AAFFalse;
		checkResult(pReadDataDef->IsDataDefOf(defs.ddPicture(), &bResult));
		checkExpression(bResult == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->IsTimeWarp (&readIsTimeWarp));
		checkExpression(readIsTimeWarp == AAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pOperationDef->GetCategoryBufLen (&catLen));
		testLen = wcslen(TEST_CATEGORY);
		checkResult(pOperationDef->GetCategory (checkCat, sizeof(checkCat)));
		checkExpression(wcscmp(checkCat, TEST_CATEGORY) == 0, AAFRESULT_TEST_FAILED);
		checkExpression(testLen == wcslen(checkCat), AAFRESULT_TEST_FAILED);
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
 

extern "C" HRESULT CEnumAAFParameterDefs_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFParameterDefsTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CEnumAAFParameterDefs_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//!!!	if (SUCCEEDED(hr))
//	{
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
