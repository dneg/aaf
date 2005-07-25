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
#include "AAFPluginDefs.h"

#include <iostream>
using namespace std;


//static wchar_t *manuf1URL = L"www.microsoft.com";
static wchar_t *manuf2URL = L"www.avid.com";

#include <iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"
#include "AAFCodecDefs.h"

#include "CAAFBuiltinDefs.h"

const aafUID_t ID_MANUFACTURER = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expPixelFormat */
//static aafVersionType_t samplePluginVersion = { 0, 0 };//, 0, 0, kVersionReleased };
static aafVersionType_t sampleMinPlatformVersion = { 1, 2 }; //, 3, 4, kVersionDebug };
static aafVersionType_t sampleMinEngineVersion = { 5, 6 }; //7, 9, kVersionPatched };
static aafVersionType_t sampleMinAPIVersion = { 10, 11 };//, 12, 13, kAAFVersionBeta };
static aafVersionType_t sampleMaxPlatformVersion = { 31, 32 };//3, 34, kVersionDebug };
static aafVersionType_t sampleMaxEngineVersion = { 35, 36 };//, 37, 39, kVersionPatched };
static aafVersionType_t sampleMaxAPIVersion = { 40, 41 };//, 42, 43, kAAFVersionBeta };

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		3

static wchar_t *manufName[] = { L"A test plugin manufacturer", L"Another test plugin manufacturer" };
static wchar_t *manufRev = L"Rev0.0.0a0";


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

static aafUID_t TestPluginDesc = { 0x7C77C181, 0x2283, 0x11d2, { 0x80, 0xAD, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
static aafUID_t TestPluginDesc2 = { 0x816DA15C, 0x33BD, 0x11d3, { 0x80, 0xB3, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expPixelFormat */

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
	ProductInfo.productName = L"EnumAAFPluginDefs Test";
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
  IAAFHeader *      pHeader = NULL;
  IAAFDictionary*	pDictionary = NULL;
  IAAFDefObject*	pPlugDef = NULL;
  IAAFCodecDef*		pCodecDef = NULL;
  IAAFClassDef*		pClassDef = NULL;
  IAAFPluginDef *pDesc;
  IAAFNetworkLocator *pNetLoc, *pNetLoc2;
  IAAFLocator		*pLoc, *pLoc2;
  aafUID_t			category = AUID_AAFDefObject, manufacturer = ID_MANUFACTURER;
  bool				bFileOpen = false;
  aafUID_t			*uidPtr, uid;
	HRESULT			hr = S_OK;
	aafInt32		n;


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
    
	checkResult(defs.cdCodecDef()->
				CreateInstance(IID_IAAFDefObject, 
							   (IUnknown **)&pPlugDef));
    
	for(n = 0; n < 2; n++)
	{
		checkResult(defs.cdPluginDef()->
					CreateInstance(IID_IAAFPluginDef, 
								   (IUnknown **)&pDesc));
		checkResult(defs.cdNetworkLocator()->
					CreateInstance(IID_IAAFNetworkLocator, 
								   (IUnknown **)&pNetLoc));
		checkResult(pNetLoc->QueryInterface (IID_IAAFLocator,
			(void **)&pLoc));
		checkResult(pLoc->SetPath (manuf2URL));
		
		if(n == 0)
			uidPtr = &TestPluginDesc;
		else if(n == 1)
			uidPtr = &TestPluginDesc2;
		else
			return AAFRESULT_TEST_FAILED;	// Assert
		checkResult(pDesc->Initialize (*uidPtr, L"Test Plugin", L"TestPlugin Description"));
		checkResult(pDesc->SetCategoryClass(category));
		checkResult(pDesc->SetPluginVersionString(manufRev));
		checkResult(pDesc->SetManufacturerInfo(pNetLoc));
		checkResult(pDesc->SetManufacturerID(manufacturer));
		checkResult(pDesc->SetPluginManufacturerName(manufName[n]));
		checkResult(pDesc->SetIsSoftwareOnly(kAAFTrue));
		checkResult(pDesc->SetIsAccelerated(kAAFFalse));
		checkResult(pDesc->SetSupportsAuthentication(kAAFFalse));
		
		//!!!	aafProductVersion_t samplePluginVersion = { 0, 0, 0, 0, kVersionReleased };
		/**/
		checkResult(pDesc->SetHardwarePlatform(kAAFPlatformIndependant));
		checkResult(pDesc->SetPlatformMinimumVersion(sampleMinPlatformVersion));
		checkResult(pDesc->SetPlatformMaximumVersion(sampleMaxPlatformVersion));
		/**/
		checkResult(pDesc->SetEngine(kAAFNoEngine));
		checkResult(pDesc->SetEngineMinimumVersion(sampleMinEngineVersion));
		checkResult(pDesc->SetEngineMaximumVersion(sampleMaxEngineVersion));
		/**/
		checkResult(pDesc->SetPluginAPI(kAAFEssencePluginAPI));
		checkResult(pDesc->SetPluginAPIMinimumVersion(sampleMinAPIVersion));
		checkResult(pDesc->SetPluginAPIMaximumVersion(sampleMaxAPIVersion));
		
		checkResult(pDictionary->RegisterPluginDef (	pDesc));
		
		/**/
		checkResult( defs.cdNetworkLocator()->
					 CreateInstance(IID_IAAFNetworkLocator, 
									(IUnknown **)&pNetLoc2));
		checkResult(pNetLoc2->QueryInterface (IID_IAAFLocator,
			(void **)&pLoc2));
		checkResult(pLoc2->SetPath (manuf2URL));
		checkResult(pDesc->AppendLocator(pLoc2));
		/**/
		checkResult(pDesc->SetDefinitionObjectID(kAAFNoCodec));
		pNetLoc->Release();
		pNetLoc = NULL;
		pNetLoc2->Release();
		pNetLoc2 = NULL;
		pLoc->Release();
		pLoc = NULL;
		pLoc2->Release();
		pLoc2 = NULL;
		pDesc->Release();
		pDesc = NULL;
	}
	
	checkResult(pPlugDef->QueryInterface (IID_IAAFCodecDef,
                                          (void **)&pCodecDef));
	checkResult(pCodecDef->AddEssenceKind (defs.ddkAAFMatte()));
	checkResult(pCodecDef->Initialize (kAAFNoCodec, L"TestCodec", L"Just a test"));
	uid = kAAFClassID_WAVEDescriptor;
	checkResult(pDictionary->LookupClassDef(uid, &pClassDef));
	checkResult(pCodecDef->SetFileDescriptorClass (pClassDef));
	checkResult(pDictionary->RegisterCodecDef(pCodecDef));
	pCodecDef->Release();
	pCodecDef = NULL;
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
  if (pPlugDef)
    pPlugDef->Release();

  if (pCodecDef)
    pCodecDef->Release();
  if (pClassDef)
    pClassDef->Release();

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
	IAAFDefObject *pDefObj = NULL;
	IAAFDefObject *pDef2 = NULL;
	IEnumAAFPluginDefs *pEnumDesc = NULL;
	IEnumAAFPluginDefs *pClonePlug = NULL;
	IAAFPluginDef *pPlugin = NULL;
	IAAFNetworkLocator	*pNetLoc = NULL;
	IAAFLocator			*pLoc = NULL;
	IEnumAAFPluginLocators *pEnumLoc = NULL;
	IAAFPluginDef*	pArray[2] = { NULL, NULL };
	IAAFPluginDef**	pArrayDef = pArray;
	
	bool bFileOpen = false;
	HRESULT			hr = S_OK;
	aafUID_t		testUID;
	aafUInt32		resultCount;
	
	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;
		
		checkResult(pHeader->GetDictionary(&pDictionary));
		

		checkResult(pDictionary->GetPluginDefs (&pEnumDesc));
		/* Read and check the first element */
		checkResult(pEnumDesc->NextOne (&pPlugin));
		checkResult(pPlugin->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;
//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc) == kAAFTrue, AAFRESULT_TEST_FAILED);
		pPlugin->Release();
		pPlugin = NULL;
		/* Read and check the second element */
		checkResult(pEnumDesc->NextOne (&pPlugin));
		checkResult(pPlugin->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;

//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc2) == kAAFTrue, AAFRESULT_TEST_FAILED);
		pPlugin->Release();
		pPlugin = NULL;
		/* Reset, and check the first element again*/
		checkResult(pEnumDesc->Reset());
		checkResult(pEnumDesc->NextOne (&pPlugin));
		checkResult(pPlugin->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;
//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc) == kAAFTrue, AAFRESULT_TEST_FAILED);
		pPlugin->Release();
		pPlugin = NULL;
		/* Reset, Skip, and check the second element again*/
		checkResult(pEnumDesc->Reset());
		checkResult(pEnumDesc->Skip(1));
		checkResult(pEnumDesc->NextOne (&pPlugin));
		checkResult(pPlugin->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;
//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc2) == kAAFTrue, AAFRESULT_TEST_FAILED);
		pPlugin->Release();
		pPlugin = NULL;
		/* Reset, and read both elements */
		checkResult(pEnumDesc->Reset());
		checkResult(pEnumDesc->Next (2, (IAAFPluginDef **)&pArray, &resultCount));
//!!!		checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
		checkResult(pArrayDef[0]->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;
//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc) == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pArrayDef[1]->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;
//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc2) == kAAFTrue, AAFRESULT_TEST_FAILED);
		pArrayDef[0]->Release();
		pArrayDef[0] = NULL;
		pArrayDef[1]->Release();
		pArrayDef[1] = NULL;
		
		/* Read one past to make sure that it fails */
		checkExpression(pEnumDesc->NextOne(&pPlugin) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
		/* Clone the enumerator, and read one element */
		checkResult(pEnumDesc->Clone(&pClonePlug));
		checkResult(pClonePlug->Reset());
		checkResult(pClonePlug->NextOne (&pPlugin));
		checkResult(pPlugin->QueryInterface (IID_IAAFDefObject, (void **)&pDef2));
		checkResult(pDef2->GetAUID(&testUID));
		pDef2->Release();
		pDef2 = NULL;
//!!!		checkExpression(EqualAUID(&testUID, &TestPluginDesc) == kAAFTrue, AAFRESULT_TEST_FAILED);

		pEnumDesc->Release();
		pEnumDesc = NULL;
		pClonePlug->Release();
		pClonePlug = NULL;
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup and return
	if (pEnumLoc)
		pEnumLoc->Release();
		
	if (pEnumDesc)
		pEnumDesc->Release();
	
	if (pClonePlug)
		pClonePlug->Release();
	
	if (pLoc)
		pLoc->Release();
	
	if (pNetLoc)
		pNetLoc->Release();
	
	if (pPlugin)
		pPlugin->Release();
		
	if (pDefObj)
		pDefObj->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pDef2)
		pDef2->Release();
	
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
 

extern "C" HRESULT CEnumAAFPluginDefs_test(testMode_t mode);
extern "C" HRESULT CEnumAAFPluginDefs_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"EnumAAFPluginDescTest.aaf";

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
		cerr << "CEnumAAFPluginDef_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFPluginDef methods have not been tested:" << endl;       
//		cout << "     Next" << endl; 
//		cout << "     Skip" << endl; 
//		cout << "     Reset" << endl; 
//		cout << "     Clone" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}
