// @doc INTERNAL
// @com This file implements the module test for CAAFParameterDef

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
#include "AAFTypeDefUIDs.h"

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
#define TEST_EFFECT_NAME	L"A TestEffect"
#define TEST_EFFECT_DESC	L"A longer description of the TestEffect"
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
#define TEST_PARAM_UNITS	L"Furlongs per Fortnight"
static aafUID_t	checkTypeID = kAAFTypeID_UInt8;

#define TEST_EFFECT_LEN		60

const aafUID_t kTestEffectID = { 0xD15E7611, 0xFE40, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kTestParmID = { 0xC7265931, 0xFE57, 0x11d2, { 0x80, 0xA5, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafRational_t kTestLevel = { 1, 2 };

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
	ProductInfo.productName = L"AAFParameterDef Test";
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
	IAAFDefObject*		pDefObject = NULL;
	IAAFTypeDef*		pTypeDef = NULL;
	aafUID_t			effectID = kTestEffectID;
	aafUID_t			parmID = kTestParmID;
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
    
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));

		checkResult(pOperationDef->Initialize (effectID, TEST_EFFECT_NAME, TEST_EFFECT_DESC));
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));

 		checkResult(pDictionary->LookupTypeDef (checkTypeID , &pTypeDef));
    checkResult(pParamDef->Initialize (parmID, TEST_PARAM_NAME, TEST_PARAM_DESC, pTypeDef));
		pTypeDef->Release();
		pTypeDef = NULL;
   
		checkResult(pDictionary->RegisterParameterDef(pParamDef));

		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->SetName (TEST_EFFECT_NAME));
		checkResult(pDefObject->SetDescription (TEST_EFFECT_DESC));
		pDefObject->Release();
		pDefObject = NULL;

//!!!Not testing the Init() on AAFDefObject
		checkResult(pOperationDef->SetDataDef (defs.ddPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));

		checkResult(pParamDef->SetDisplayUnits(TEST_PARAM_UNITS));

    checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->SetName (TEST_PARAM_NAME));
		checkResult(pDefObject->SetDescription (TEST_PARAM_DESC));
		pDefObject->Release();
		pDefObject = NULL;
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

	if (pTypeDef)
		pTypeDef->Release();

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
	IAAFOperationDef		*pOperationDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFDefObject*		pDefObject = NULL;
  IAAFMetaDefinition *pMetaDefinition = NULL;
	IAAFTypeDef*		pTypeDef = NULL;
	bool				bFileOpen = false;
	aafBool				readIsTimeWarp;
	aafInt32			checkNumInputs;
	aafUInt32			checkBypass;
	HRESULT				hr = S_OK;
	wchar_t				checkName[256];
	aafUID_t			checkCat;
	aafUID_t			testAUID;
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
		checkResult (pReadDataDef->IsDataDefOf(defs.ddPicture(), &bResult));
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
		checkResult(pParmDefEnum->NextOne (&pParmDef));

		checkResult(pParmDef->GetTypeDefinition(&pTypeDef));
		checkResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition, (void **) &pMetaDefinition));
		checkResult(pMetaDefinition->GetAUID(&testAUID));
		checkExpression(memcmp(&testAUID, &checkTypeID, sizeof(testAUID)) == 0, AAFRESULT_TEST_FAILED);
		pMetaDefinition->Release();
		pMetaDefinition = NULL;
		pTypeDef->Release();
		pTypeDef = NULL;
		checkResult(pParmDef->GetDisplayUnits (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_UNITS) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pParmDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
		checkResult(pDefObject->GetName (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_NAME) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pDefObject->GetDescription (checkName, sizeof(checkName)));
		checkExpression(wcscmp(checkName, TEST_PARAM_DESC) == 0, AAFRESULT_TEST_FAILED);
		pDefObject->Release();
		pDefObject = NULL;

#if 0
    ImplAAFOperationDef::GetDegradeToOperations (ImplEnumAAFOperationDefs  **ppEnum)
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

	if (pParmDefEnum)
		pParmDefEnum->Release();
      
	if (pOperationDef)
		pOperationDef->Release();
      
	if (pDefObject)
		pDefObject->Release();

  if (pMetaDefinition)
    pMetaDefinition->Release();

	if (pTypeDef)
		pTypeDef->Release();

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
 

extern "C" HRESULT CAAFParameterDef_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFParameterDefTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFParameterDef_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFParameterDef methods have not been implemented:" << endl; 
//		cout << "     SetTypeDef" << endl; 
//		cout << "     GetTypeDefinition" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
