// @doc INTERNAL
// @com This file implements the module test for CAAFCodecDef
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
 *  prior written permission of Avid Technology, Inc.
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

#include "AAFClassDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFCodecDefs.h"

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

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFCodecDef Test";
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
	IAAFFile*		pFile = NULL;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFCodecDef*	pPlugDef = NULL;
	IAAFDefObject	*pDef = NULL;
	IAAFDataDef		*pDataDef = NULL;
	IAAFClassDef	*classDef = NULL;
	bool bFileOpen = false;
	HRESULT			hr = S_OK;
	aafUID_t		uid;
/*	long			test;
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
    
		checkResult(pDictionary->CreateInstance(AUID_AAFCodecDef,
							  IID_IAAFCodecDef, 
							  (IUnknown **)&pPlugDef));
    
		checkResult(pPlugDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef));
		uid = CodecWave;
		checkResult(pDef->Initialize (uid, L"TestCodec", L"TestCodecDescription"));

		checkResult(pPlugDef->AddEssenceKind (DDEF_Matte));
		checkResult(pDictionary->RegisterCodecDef(pPlugDef));
		uid = kAAFClassID_WAVEDescriptor;
//		checkResult(pDictionary->LookupClass(&uid, &classDef));
//		checkResult(pPlugDef->SetFileDescriptorClass (classDef));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


  // Cleanup and return
  if (pDef)
    pDef->Release();

  if (classDef)
    classDef->Release();

  if (pDataDef)
    pDataDef->Release();

  if (pPlugDef)
    pPlugDef->Release();

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
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IAAFDictionary*			 pDictionary = NULL;
	IAAFCodecDef			*pCodec = NULL;
	IAAFDataDef				*pDataDef = NULL;
	IEnumAAFCodecFlavours	*pEnum = NULL;
	bool					bFileOpen = false;
	aafBool					testResult;
	aafUID_t				codecID = CodecWave;
	aafUID_t				readFlavour, checkFlavour = NilCodecFlavour;
	HRESULT					hr = S_OK;

	try
	{
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
		checkResult(pDictionary->LookupCodecDef(codecID, &pCodec));

		checkResult(pCodec->IsEssenceKindSupported (DDEF_Matte, &testResult));
		checkExpression (testResult == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pCodec->IsEssenceKindSupported (DDEF_Picture, &testResult));
		checkExpression (testResult == AAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pCodec->EnumCodecFlavours (&pEnum));
		checkResult(pEnum->NextOne (&readFlavour));
		checkExpression (memcmp(&readFlavour, &checkFlavour, sizeof(checkFlavour)) == 0,
						 AAFRESULT_TEST_FAILED);
		checkResult(pCodec->AreThereFlavours (&testResult));
		checkExpression (AAFFalse == testResult,
						 AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pEnum)
		pEnum->Release();
	if (pDataDef)
		pDataDef->Release();
	if (pCodec)
		pCodec->Release();
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
 

extern "C" HRESULT CAAFCodecDef_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFCodecDefTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFCodecDef_test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following IAAFCodecDef methods have not been tested:" << endl; 
		cout << "     SetFileDescriptorClass" << endl; 
		cout << "     GetFileDescriptorClass" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}

























