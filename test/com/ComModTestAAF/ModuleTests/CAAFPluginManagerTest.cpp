// @doc INTERNAL
// @com This file implements the module test for CAAFPluginManager

/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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



#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>

#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"
#include "AAF.h"
#include "AAFPlugin.h"
#include "AAFPlugin_i.c"

const CLSID CLSID_AAFBasicInterp = { 0x5B6C85A1, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

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
	ProductInfo.productName = L"AAFPluginManager Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kAAFMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

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

extern "C" HRESULT CAAFPluginManager_test(testMode_t );
extern "C" HRESULT CAAFPluginManager_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	IEnumAAFLoadedPlugins	*pEnum = NULL;
	IAAFPluginManager		*pMgr = NULL;
	aafUID_t				testUID;
	IAAFFile*		pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	aafWChar * pFileName = L"AAFPluginManagerTest.aaf";
	IAAFDefObject	*pPluginDef = NULL;
	IAAFPlugin	*plugin			= NULL;

	try
	{
		if(mode != kAAFUnitTestReadWrite)
			return AAFRESULT_SUCCESS;			// Can't run this test read-only

		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		
		checkResult(AAFGetPluginManager (&pMgr));
		
		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));
		bFileOpen = true;
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));

		checkResult(pMgr->EnumLoadedPlugins (AUID_AAFCodecDef, &pEnum));
		while(pEnum->NextOne (&testUID) == AAFRESULT_SUCCESS)
		{
			checkResult(pMgr->CreatePluginDefinition (testUID, pDictionary, &pPluginDef));
			pPluginDef->Release();
			pPluginDef = NULL;
		}
		hr = AAFRESULT_SUCCESS;

		checkResult(pMgr->CreateInstance(CLSID_AAFBasicInterp,
			NULL, 
			IID_IAAFPlugin, 
			(void **)&plugin));
	}
	catch (HRESULT& rhr)
	{
		hr = rhr;
	}

	if (plugin)
		plugin->Release();

	if (pEnum)
		pEnum->Release();

	if (pMgr)
		pMgr->Release();

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
	
	// JeffB: RegisterPluginDirectory and RegisterPluginFile have their implementations tested
	// by the RegisterStandard() function, and will require a special local plugin to test.  Therefore
	// I'm pushing the test off post 1.0 and declaring success here.
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFPluginManager tests have not been implemented:" << endl; 
////!!!		cout << "     RegisterPluginDirectory" << endl; 
////!!!		cout << "     RegisterPluginFile" << endl; 
////		cout << "     EnumLoadedPlugins" << endl; 
//		cout << "     CreateInstance" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}







