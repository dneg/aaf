// @doc INTERNAL
// @com This file implements the module test for CAAFDefObject
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

/*************************************************************************
 * 
 * @module AAFDefObject | AAFDefObject is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFDefObject specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/




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
#include "AAFContainerDefs.h"

#include "CAAFBuiltinDefs.h"

// Some handy smart pointers
#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDefObject>             IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>        IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFPluginDef>				IAAFPluginDescriptorSP;
typedef IAAFSmartPointer<IAAFTypeDef>               IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>            IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IEnumAAFPluginDefs>		IEnumAAFPluginDescriptorsSP;


//
// Some guids
//

// {005DFB21-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDescID1 = 
{ 0x5dfb21, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {005DFB22-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDescID2 = 
{ 0x5dfb22, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {005DFB23-7B5B-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDescID3 = 
{ 0x5dfb23, 0x7b5b, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


// {4E84045D-0F29-11d4-A359-009027DFCA6A}
static const aafUID_t testUID = 
{ 0x4e84045d, 0xf29, 0x11d4, { 0xa3, 0x59, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

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

static wchar_t *sName = L"Test Descriptor Name";
static wchar_t *sDescription = L"Test Descriptor Description";


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
	ProductInfo.productName = L"AAFDefObject Test";
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
	IAAFHeader *		pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFContainerDef*	pContainerDef = NULL;
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

	checkResult(defs.cdContainerDef()->
				CreateInstance(IID_IAAFContainerDef, 
							   (IUnknown **)&pContainerDef));
    
	checkResult(pContainerDef->QueryInterface (IID_IAAFDefObject,
                                          (void **)&pDef));

	checkResult(pContainerDef->Initialize (testUID, sName, sDescription));
	checkResult(pDictionary->RegisterContainerDef(pContainerDef));


	//
	// test Append, Prepend, and enum plugin descriptor using same type def
	//
	IAAFPluginDescriptorSP pd1;
	checkResult (defs.cdPluginDef()->
				 CreateInstance (IID_IAAFPluginDef,
								 (IUnknown **)&pd1));
	checkResult (pd1->Initialize (kTestPluginDescID1,
							L"PluginDesc1",
							L"Plugin Descriptor 1 description"));
	checkResult(pd1->SetDefinitionObjectID(kTestPluginDescID1));
		checkResult (pDictionary->RegisterPluginDef (pd1));

	IAAFPluginDescriptorSP pd2;
	checkResult (defs.cdPluginDef()->
				 CreateInstance (IID_IAAFPluginDef,
								 (IUnknown **)&pd2));
	checkResult (pd2->Initialize (kTestPluginDescID2,
							L"PluginDesc2",
							L"Plugin Descriptor 2 description"));
	checkResult(pd2->SetDefinitionObjectID(kTestPluginDescID2));
	checkResult (pDictionary->RegisterPluginDef (pd2));

	IAAFPluginDescriptorSP pd3;
	checkResult (defs.cdPluginDef()->
				 CreateInstance (IID_IAAFPluginDef,
								 (IUnknown **)&pd3));
	checkResult (pd3->Initialize (kTestPluginDescID3,
							L"PluginDesc3",
							L"Plugin Descriptor 3 description"));
	checkResult(pd3->SetDefinitionObjectID(kTestPluginDescID3));
	checkResult (pDictionary->RegisterPluginDef (pd3));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pDef)
    pDef->Release();

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
	IAAFFile*			pFile = NULL;
	IAAFHeader*			pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IEnumAAFContainerDefs *pPlug = NULL;
	IAAFDefObject*		pDef = NULL;
	IAAFContainerDef*	pContainerDef = NULL;
	bool				bFileOpen = false;
	HRESULT				hr = S_OK;
	aafUID_t			readUID;
	wchar_t				testString[256];

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetContainerDefs(&pPlug));
		while(pPlug->NextOne (&pContainerDef) == AAFRESULT_SUCCESS)
		{
			checkResult(pContainerDef->QueryInterface (IID_IAAFDefObject, (void **)&pDef));
			checkResult(pDef->GetAUID(&readUID));
			if(memcmp(&readUID, &testUID, sizeof(aafUID_t)) == 0)
			{
				checkResult(pDef->GetName (testString, sizeof(testString)));
				checkExpression (wcscmp(testString, sName) == 0, AAFRESULT_TEST_FAILED);

				aafUInt32 nameLen;
				checkResult (pDef->GetNameBufLen (&nameLen));
				checkExpression (((wcslen (sName)+1) * sizeof (aafCharacter) == nameLen),
						 AAFRESULT_TEST_FAILED);

				checkResult(pDef->GetDescription (testString, sizeof(testString)));
				checkExpression (wcscmp(testString, sDescription) == 0, AAFRESULT_TEST_FAILED);

				checkResult (pDef->GetDescriptionBufLen (&nameLen));
				checkExpression (((wcslen (sDescription)+1) * sizeof (aafCharacter) == nameLen),
						 AAFRESULT_TEST_FAILED);

				break;
			}

                        pDef->Release();
                        pDef = NULL;
                        pContainerDef->Release();
                        pContainerDef = NULL;

		}//		checkResult(pPlug->NextOne(&pContainerDef));


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

	if (pDef)
		pDef->Release();
      
	if (pContainerDef)
		pContainerDef->Release();
            
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
 

extern "C" HRESULT CAAFDefObject_test(testMode_t mode);
extern "C" HRESULT CAAFDefObject_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFDefObjectTest.aaf";

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
		cerr << "CAAFDefObject_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}



