// @doc INTERNAL
// @com This file implements the module test for CAAFInterpolationDef
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

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFInterpolatorDefs.h"

#include "CAAFBuiltinDefs.h"

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFInterpolationDef; // generated

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
	ProductInfo.productName = L"AAFInterpolationDef Test";
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
	IAAFFile*		pFile = NULL;
  IAAFHeader *        pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
  IAAFInterpolationDef*	pInterpolationDef = NULL;
  bool bFileOpen = false;
	HRESULT			hr = S_OK;
/*	long			test;
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
		checkResult(defs.cdInterpolationDefinition()->
					CreateInstance(IID_IAAFInterpolationDef, 
								   (IUnknown **)&pInterpolationDef));
    
		checkResult(pInterpolationDef->Initialize(NoInterpolator, L"Test", L"Test"));
		checkResult(pDictionary->RegisterInterpolationDef(pInterpolationDef));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
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
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IEnumAAFInterpolationDefs *pPlug = NULL;
	IAAFInterpolationDef		*pPlugDef = NULL;
	IAAFInterpolationDef		*pInterpolationDef = NULL;
	bool bFileOpen = false;
	HRESULT			hr = S_OK;

	try
	{
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetInterpolationDefs(&pPlug));
		checkResult(pPlug->NextOne (&pPlugDef));
		checkResult(pPlugDef->QueryInterface (IID_IAAFInterpolationDef, (void **)&pInterpolationDef));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pInterpolationDef)
		pInterpolationDef->Release();

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
 

extern "C" HRESULT CAAFInterpolationDef_test(testMode_t mode);
extern "C" HRESULT CAAFInterpolationDef_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFInterpolationDefTest.aaf";

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
		cerr << "CAAFInterpolationDef_test..."
			 <<" Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}





































