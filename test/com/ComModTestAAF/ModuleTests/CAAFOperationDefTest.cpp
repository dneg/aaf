// @doc INTERNAL
// @com This file implements the module test for CAAFOperationDef

/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\************************************************/

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
#define TEST_EFFECT_NAME1	L"A TestEffect"
#define TEST_EFFECT_DESC1	L"A longer description of the TestEffect"
#define TEST_EFFECT_NAME2	L"An appended TestEffect"
#define TEST_EFFECT_DESC2	L"A longer description of the appended TestEffect"
#define TEST_EFFECT_NAME3	L"A prepended TestEffect"
#define TEST_EFFECT_DESC3	L"A longer description of the prepended TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"

const aafUID_t kParmID = { 0x88928EB4, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static wchar_t *effectNames[] = { TEST_EFFECT_NAME1, TEST_EFFECT_NAME2, TEST_EFFECT_NAME3 };
static wchar_t *effectDesc[] = { TEST_EFFECT_DESC1, TEST_EFFECT_DESC2, TEST_EFFECT_DESC3 };
static aafUID_t effectID[3] =
	{ { 0x88928EB1, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } },
	{ 0x88928EB2, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } },
	{ 0x88928EB3, 0x18FA, 0x11d3, { 0x80, 0xAB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } } };

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFOperationDef Test";
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
	IAAFParameterDef*	pParamDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUID_t			testDataDef = DDEF_Picture, testParmID = kParmID;
	aafInt32			index;
	IAAFOperationDef	*pOperationDef = NULL, *defResults[3] = { NULL, NULL, NULL };

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
    
 		checkResult(pDictionary->CreateInstance(&AUID_AAFParameterDef,
						  IID_IAAFParameterDef, 
						  (IUnknown **)&pParamDef));
		checkResult(pDictionary->RegisterParameterDefinition(pParamDef));
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->Init (&testParmID, TEST_PARAM_NAME, TEST_PARAM_DESC));
		pDefObject->Release();
		pDefObject = NULL;

		for(index = 0; index < 3; index++)
		{
			checkResult(pDictionary->CreateInstance(&AUID_AAFOperationDef,
				IID_IAAFOperationDef, 
				(IUnknown **)&pOperationDef));
			checkResult(pDictionary->RegisterOperationDefinition(pOperationDef));
			
			checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
			checkResult(pDefObject->Init (effectID+index, effectNames[index], effectDesc[index]));
			pDefObject->Release();
			pDefObject = NULL;
			
			checkResult(pOperationDef->SetDataDefinitionID (&testDataDef));
			checkResult(pOperationDef->SetIsTimeWarp (AAFFalse));
			checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
			checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
			checkResult(pOperationDef->AddParameterDefs (pParamDef));
			checkResult(pOperationDef->SetBypass (TEST_BYPASS));
			
			defResults[index] = pOperationDef;
			pOperationDef = NULL;
		}

		// !!!Added circular definitions because we don't have optional properties
		checkResult(defResults[1]->AppendDegradeToOperations (defResults[1]));
		checkResult(defResults[2]->AppendDegradeToOperations (defResults[2]));
		checkResult(defResults[0]->AppendDegradeToOperations (defResults[1]));
		checkResult(defResults[0]->PrependDegradeToOperations (defResults[2]));
		for(index = 0; index < 3; index++)
		{
			defResults[index]->Release();
			defResults[index] = NULL;
		}
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
	IEnumAAFOperationDefs *pDegradeEnum = NULL;
	IEnumAAFParameterDefs *pParmDefEnum = NULL;
	IAAFOperationDef		*pOperationDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
	bool				bFileOpen = false;
	aafUID_t			readDataDef, checkDataDef = DDEF_Picture;
	aafBool				readIsTimeWarp;
	aafInt32			catLen, checkNumInputs;
	aafUInt32			checkBypass, testLen;
	HRESULT				hr = S_OK;
	wchar_t				checkCat[256], checkName[256];

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetOperationDefinitions(&pEffectEnum));
		checkResult(pEffectEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->GetDataDefinitionID(&readDataDef));

		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectNames[0]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectDesc[0]) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		
		checkExpression(EqualAUID(&readDataDef, &checkDataDef) == AAFTrue, AAFRESULT_TEST_FAILED);
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
		checkResult(pOperationDef->GetParameterDefinitions (&pParmDefEnum));
		checkResult(pParmDefEnum->NextOne (&pParmDef));

		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;

		checkResult(pOperationDef->GetDegradeToOperations (&pDegradeEnum));
		pOperationDef->Release();
		pOperationDef = NULL;

		// Check for prepended one first
		checkResult(pDegradeEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectNames[2]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectDesc[2]) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		pOperationDef->Release();
		pOperationDef = NULL;

		// Check for appended one second
		checkResult(pDegradeEnum->NextOne (&pOperationDef));
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectNames[1]) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, effectDesc[1]) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;
		pOperationDef->Release();
		pOperationDef = NULL;
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

	if (pDegradeEnum)
		pDegradeEnum->Release();

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
 

extern "C" HRESULT CAAFOperationDef_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFOperationDefTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFOperationDef_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFOperationDef methods have not been implemented:" << endl; 
//		cout << "     GetDegradeToOperations" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
























































