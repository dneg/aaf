// @doc INTERNAL
// @com This file implements the module test for CAAFContainerDef
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
#include "AAFContainerDefs.h"

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

// {4E84045D-0F29-11d4-A359-009027DFCA6A}
static const aafUID_t testUID = 
{ 0x4e84045d, 0xf29, 0x11d4, { 0xa3, 0x59, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

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
	ProductInfo.productName = L"AAFContainerDef Test";
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
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;

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
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

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
 

extern "C" HRESULT CAAFContainerDef_test(testMode_t mode);
extern "C" HRESULT CAAFContainerDef_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFContainerDefTest.aaf";

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
		cerr << "CAAFContainerDef_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}



































