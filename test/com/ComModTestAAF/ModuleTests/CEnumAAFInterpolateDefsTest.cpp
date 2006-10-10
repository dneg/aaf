
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
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

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

static wchar_t *sName1 = L"Test Descriptor Name1";
static wchar_t *sDescription1 = L"Test Descriptor Description1";
static wchar_t *sName2 = L"Test Descriptor Name2";
static wchar_t *sDescription2 = L"Test Descriptor Description2";

// {81831636-EDF4-11d3-A353-009027DFCA6A}
static const aafUID_t InterpDef1 = 
{ 0x81831636, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };
// {81831637-EDF4-11d3-A353-009027DFCA6A}
static const aafUID_t InterpDef2 = 
{ 0x81831637, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

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
	ProductInfo.productName = L"EnumAAFInterpolationDefs Test";
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

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*			pFile = NULL;
	IAAFHeader *		pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFInterpolationDef*	pInterpolationDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
/*	long				test;
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
    
	checkResult(defs.cdInterpolationDefinition()->
				CreateInstance(IID_IAAFInterpolationDef, 
							   (IUnknown **)&pInterpolationDef));
    
	checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

	checkResult(pInterpolationDef->Initialize(InterpDef1, sName1, sDescription1));
	checkResult(pDictionary->RegisterInterpolationDef(pInterpolationDef));
	pDef->Release();
	pDef = NULL;
	pInterpolationDef->Release();
	pInterpolationDef = NULL;
	checkResult(defs.cdInterpolationDefinition()->
				CreateInstance(IID_IAAFInterpolationDef, 
							   (IUnknown **)&pInterpolationDef));
    
	checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

	checkResult(pInterpolationDef->Initialize(InterpDef2, sName2, sDescription2));

	checkResult(pDictionary->RegisterInterpolationDef(pInterpolationDef));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pDef)
    pDef->Release();

  if (pInterpolationDef)
    pInterpolationDef->Release();

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
	IEnumAAFInterpolationDefs *pPlug = NULL;
	IEnumAAFInterpolationDefs *pClonePlug = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFInterpolationDef*	pInterpolationDef = NULL;
	IAAFInterpolationDef*	pArray[2] = { NULL, NULL };
	IAAFInterpolationDef**	pArrayDef = pArray;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	wchar_t				testString[256];
	aafUInt32			resultCount;

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetInterpolationDefs(&pPlug));
		/* Read and check the first element */
		checkResult(pPlug->NextOne(&pInterpolationDef));
		checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName1) == 0, AAFRESULT_TEST_FAILED);
		pInterpolationDef->Release();
		pInterpolationDef = NULL;
		pDef->Release();
		pDef = NULL;

		/* Read and check the second element */
		checkResult(pPlug->NextOne(&pInterpolationDef));
		checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName2) == 0, AAFRESULT_TEST_FAILED);
		pInterpolationDef->Release();
		pInterpolationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, and check the first element again*/
		checkResult(pPlug->Reset());
		checkResult(pPlug->NextOne(&pInterpolationDef));
		checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName1) == 0, AAFRESULT_TEST_FAILED);
		pInterpolationDef->Release();
		pInterpolationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, Skip, and check the second element again*/
		checkResult(pPlug->Reset());
		checkResult(pPlug->Skip(1));
		checkResult(pPlug->NextOne(&pInterpolationDef));
		checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName2) == 0, AAFRESULT_TEST_FAILED);
		pInterpolationDef->Release();
		pInterpolationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, and read both elements */
		checkResult(pPlug->Reset());
		checkResult(pPlug->Next (2, (IAAFInterpolationDef **)&pArray, &resultCount));
		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		checkResult(pArrayDef[0]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName1) == 0, AAFRESULT_TEST_FAILED);
		pDef->Release();
		pDef = NULL;
		checkResult(pArrayDef[1]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName2) == 0, AAFRESULT_TEST_FAILED);
		pDef->Release();
		pDef = NULL;
		/* Read one past to make sure that it fails */
		checkExpression(pPlug->NextOne(&pInterpolationDef) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		/* Clone the enumerator, and read one element */
		checkResult(pPlug->Clone(&pClonePlug));
		checkResult(pClonePlug->Reset());
		checkResult(pClonePlug->NextOne(&pInterpolationDef));
		checkResult(pInterpolationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName1) == 0, AAFRESULT_TEST_FAILED);
		pInterpolationDef->Release();
		pInterpolationDef = NULL;
		pDef->Release();
		pDef = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pClonePlug)
		pClonePlug->Release();

	if (pDef)
		pDef->Release();

	if (pInterpolationDef)
		pInterpolationDef->Release();

	if (pArray[0])
		pArray[0]->Release();
	if (pArray[1])
		pArray[1]->Release();

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
 

extern "C" HRESULT CEnumAAFInterpolationDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFInterpolationDefs_test(
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
		cerr << "CEnumAAFInterpolationDefs_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	return hr;
}
