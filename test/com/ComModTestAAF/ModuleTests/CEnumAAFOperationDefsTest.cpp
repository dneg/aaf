// @doc INTERNAL
// @com This file implements the module test for CEnumAAFContainerDefs object
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
#define TEST_CATEGORY		L"Test Effects"
#define TEST_BYPASS			1
#define TEST_PARAM_NAME		L"A TestEffect parameter"
#define TEST_PARAM_DESC		L"A longer description of the TestEffect parameter"
static wchar_t *sName[2] = { L"Test Descriptor Name1", L"Test Descriptor Name2" };
static wchar_t *sDescription[2] = { L"Test Descriptor Description1", L"Test Descriptor Description2" };

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFOperationDefs Test";
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
  IAAFHeader *		pHeader = NULL;
  IAAFDictionary*		pDictionary = NULL;
  IAAFDefObject*		pDef = NULL;
  IAAFOperationDef*	pOperationDef = NULL;
  IAAFParameterDef*	pParamDef = NULL;
  bool				bFileOpen = false;
  HRESULT				hr = S_OK;
  long				n;

  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


	// Create the AAF file
	checkResult(OpenAAFFile(pFileName,
							kMediaOpenAppend,
							&pFile,
							&pHeader));
    bFileOpen = true;

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
	CAAFBuiltinDefs defs (pDictionary);
    
	for(n = 0; n < 2; n++)
	{
		checkResult(defs.cdOperationDef()->
					CreateInstance(IID_IAAFOperationDef, 
								   (IUnknown **)&pOperationDef));    
		checkResult(defs.cdParameterDef()->
					CreateInstance(IID_IAAFParameterDef, 
								   (IUnknown **)&pParamDef));
		
		checkResult(pDictionary->RegisterOperationDef(pOperationDef));
		checkResult(pDictionary->RegisterParameterDef(pParamDef));
		
		checkResult(pOperationDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef));
		checkResult(pDef->SetName (sName[n]));
		checkResult(pDef->SetDescription (sDescription[n]));
		pDef->Release();
		pDef = NULL;
		
		//!!!Not testing the INIT on AAFDefObject
		checkResult(pOperationDef->SetDataDef (defs.ddPicture()));
		checkResult(pOperationDef->SetIsTimeWarp (AAFFalse));
		checkResult(pOperationDef->SetNumberInputs (TEST_NUM_INPUTS));
		checkResult(pOperationDef->SetCategory (TEST_CATEGORY));
		checkResult(pOperationDef->AddParameterDef (pParamDef));
		checkResult(pOperationDef->SetBypass (TEST_BYPASS));
		// !!!Added circular definitions because we don't have optional properties
		checkResult(pOperationDef->AppendDegradeToOperation (pOperationDef));
		
		checkResult(pParamDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef));
		checkResult(pDef->SetName (TEST_PARAM_NAME));
		checkResult(pDef->SetDescription (TEST_PARAM_DESC));
		pDef->Release();
		pDef = NULL;
		pParamDef->Release();
		pParamDef = NULL;
		
		pOperationDef->Release();
		pOperationDef = NULL;
	}
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pDef)
    pDef->Release();

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
	IEnumAAFOperationDefs *pPlug = NULL;
	IEnumAAFOperationDefs *pClonePlug = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFOperationDef*	pOperationDef = NULL;
	IAAFOperationDef*	pArray[2] = { NULL, NULL };
	IAAFOperationDef**	pArrayDef = pArray;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	wchar_t				testString[256];
	aafUInt32			resultCount;

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetOperationDefs(&pPlug));
		/* Read and check the first element */
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[0]) == 0, AAFRESULT_TEST_FAILED);
		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;

		/* Read and check the second element */
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[1]) == 0, AAFRESULT_TEST_FAILED);
		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, and check the first element again*/
		checkResult(pPlug->Reset());
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[0]) == 0, AAFRESULT_TEST_FAILED);
		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, Skip, and check the second element again*/
		checkResult(pPlug->Reset());
		checkResult(pPlug->Skip(1));
		checkResult(pPlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[1]) == 0, AAFRESULT_TEST_FAILED);
		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
		/* Reset, and read both elements */
		checkResult(pPlug->Reset());
		checkResult(pPlug->Next (2, (IAAFOperationDef **)&pArray, &resultCount));
		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		checkResult(pArrayDef[0]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[0]) == 0, AAFRESULT_TEST_FAILED);
		pDef->Release();
		pDef = NULL;
		checkResult(pArrayDef[1]->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[1]) == 0, AAFRESULT_TEST_FAILED);
		pDef->Release();
		pDef = NULL;
		/* Read one past to make sure that it fails */
		checkExpression(pPlug->NextOne(&pOperationDef) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		/* Clone the enumerator, and read one element */
		checkResult(pPlug->Clone(&pClonePlug));
		checkResult(pClonePlug->Reset());
		checkResult(pClonePlug->NextOne(&pOperationDef));
		checkResult(pOperationDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

		checkResult(pDef->GetName (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, sName[0]) == 0, AAFRESULT_TEST_FAILED);
		pOperationDef->Release();
		pOperationDef = NULL;
		pDef->Release();
		pDef = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pHeader)
		pHeader->Release();
      
	if (pPlug)
		pPlug->Release();

	if (pClonePlug)
		pClonePlug->Release();

	if (pDef)
		pDef->Release();
      
	if (pOperationDef)
		pOperationDef->Release();
            
	if (pArray[0])
		pArray[0]->Release();
	if (pArray[1])
		pArray[1]->Release();
            
	if (pDictionary)
		pDictionary->Release();

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 

extern "C" HRESULT CEnumAAFOperationDefs_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFOperationDefsTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CEnumAAFOperationDefs_test...Caught general C++ exception!" << endl; 
	}
	return hr;
}
