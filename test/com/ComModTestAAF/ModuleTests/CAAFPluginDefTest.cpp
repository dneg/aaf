// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/




#include "CAAFPluginDescriptor.h"
#include "CAAFPluginDescriptor.h"
#ifndef __CAAFPluginDescriptor_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFPluginDescriptor; // generated
extern "C" const aafUID_t AUID_AAFPluggableDesc; // generated

static wchar_t *manuf1URL = L"www.microsoft.com";
static wchar_t *manuf2URL = L"www.avid.com";

#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "aafUtils.h"

const aafUID_t MANUF_JEFFS_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expPixelFormat */
aafVersionType_t samplePluginVersion = { 0, 0 };//, 0, 0, kVersionReleased };
aafVersionType_t sampleMinPlatformVersion = { 1, 2 }; //, 3, 4, kVersionDebug };
aafVersionType_t sampleMinEngineVersion = { 5, 6 }; //7, 9, kVersionPatched };
aafVersionType_t sampleMinAPIVersion = { 10, 11 };//, 12, 13, kVersionBeta };
aafVersionType_t sampleMaxPlatformVersion = { 31, 32 };//3, 34, kVersionDebug };
aafVersionType_t sampleMaxEngineVersion = { 35, 36 };//, 37, 39, kVersionPatched };
aafVersionType_t sampleMaxAPIVersion = { 40, 41 };//, 42, 43, kVersionBeta };

#define	MobName			L"MasterMOBTest"
#define	NumMobSlots		3

static wchar_t *manufName = L"Jeff's Plugin-O-Rama";
static wchar_t *manufRev = L"Rev0.0.0a0";

aafBool	EqualVersion(aafVersionType_t *vers1, aafVersionType_t *vers2)
{
	return(memcmp((char *)vers1, (char *)vers2, sizeof(aafVersionType_t)) == 0 ? AAFTrue : AAFFalse);
}

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
	ProductInfo.productName = L"AAFMasterMob Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	if(mode == kMediaOpenAppend)
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
	else
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);

	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
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
  IAAFPluggableDef*	pPlugDef;
  IAAFPluginDescriptor *pDesc;
  IAAFNetworkLocator *pNetLoc, *pNetLoc2, *pNetLoc3;
  IAAFLocator *pLoc, *pLoc2, *pLoc3;
  aafUID_t			category = AUID_AAFPluggableDefinition, manufacturer = MANUF_JEFFS_PLUGINS;
  bool bFileOpen = false;
	HRESULT			hr = S_OK;
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
    
	checkResult(pDictionary->CreateInstance(&AUID_AAFPluggableDef,
							  IID_IAAFPluggableDef, 
							  (IUnknown **)&pPlugDef));
    
	checkResult(pDictionary->CreateInstance(&AUID_AAFPluginDescriptor,
							  IID_IAAFPluginDescriptor, 
							  (IUnknown **)&pDesc));
	checkResult(pDictionary->CreateInstance(&AUID_AAFNetworkLocator,
							  IID_IAAFNetworkLocator, 
							  (IUnknown **)&pNetLoc));
	checkResult(pNetLoc->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc));
	checkResult(pLoc->SetPath (manuf2URL));

	checkResult(pDesc->SetCategoryClass(&category));
	checkResult(pDesc->SetPluginVersionString(manufRev));
    checkResult(pDesc->SetManufacturerInfo(pNetLoc));
    checkResult(pDesc->SetManufacturerID(&manufacturer));
    checkResult(pDesc->SetPluginManufacturerName(manufName));
    checkResult(pDesc->SetIsSoftwareOnly(AAFTrue));
    checkResult(pDesc->SetIsAccelerated(AAFFalse));
    checkResult(pDesc->SetSupportsAuthentication(AAFFalse));
 
//!!!	aafProductVersion_t samplePluginVersion = { 0, 0, 0, 0, kVersionReleased };
    /**/
    checkResult(pDesc->SetHardwarePlatform(kAAFPlatformIndependant));
    checkResult(pDesc->SetPlatformMinimumVersion(&sampleMinPlatformVersion));
    checkResult(pDesc->SetPlatformMaximumVersion(&sampleMaxPlatformVersion));
    /**/
 	checkResult(pDesc->SetEngine(kAAFNoEngine));
    checkResult(pDesc->SetEngineMinimumVersion(&sampleMinEngineVersion));
    checkResult(pDesc->SetEngineMaximumVersion(&sampleMaxEngineVersion));
    /**/
	checkResult(pDesc->SetPluginAPI(kAAFEssencePluginAPI));
    checkResult(pDesc->SetPluginAPIMinimumVersion(&sampleMinAPIVersion));
    checkResult(pDesc->SetPluginAPIMaximumVersion(&sampleMaxAPIVersion));

	/**/
	checkResult(pDictionary->CreateInstance(&AUID_AAFNetworkLocator,
							  IID_IAAFNetworkLocator, 
							  (IUnknown **)&pNetLoc2));
	checkResult(pNetLoc2->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc2));
	checkResult(pLoc2->SetPath (manuf2URL));
    checkResult(pDesc->AppendLocator(pLoc2));
	/**/
	checkResult(pPlugDef->AppendPluginDescriptor(pDesc));

	checkResult(pDictionary->RegisterPluggableDefinition(pPlugDef));
	/**/
	checkResult(pDictionary->CreateInstance(&AUID_AAFNetworkLocator,
							  IID_IAAFNetworkLocator, 
							  (IUnknown **)&pNetLoc3));
	checkResult(pNetLoc3->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc3));
	checkResult(pLoc3->SetPath (manuf1URL));
    checkResult(pDesc->PrependLocator(pLoc3));

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


  // Cleanup and return
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
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IEnumAAFPluggableDefs *pEnumPluggable = NULL;
	IAAFPluggableDef *pPluggable = NULL;
	IEnumAAFPluginDescriptors *pEnumDesc;
	IAAFPluginDescriptor *pPlugin = NULL;
	IAAFNetworkLocator	*pNetLoc = NULL;
	IAAFLocator			*pLoc = NULL;
	IEnumAAFPluginLocators *pEnumLoc = NULL;

  bool bFileOpen = false;
	HRESULT			hr = S_OK;
	aafUID_t		testUID;
	aafInt32		testInt32;
	aafUInt32		count;
	wchar_t			testString[256];
	aafBool			testBool;
	aafHardwarePlatform_t testPlatform;
	aafPluginAPI_t	testAPI;
	aafEngine_t		testEngine;
	aafVersionType_t testMinVersion, testMaxVersion;
  aafUID_t			category = AUID_AAFPluggableDefinition, manufacturer = MANUF_JEFFS_PLUGINS;

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;

		checkResult(pHeader->GetDictionary(&pDictionary));
	
		checkResult(pDictionary->GetPluggableDefinitions(&pEnumPluggable));
		checkResult(pEnumPluggable->NextOne (&pPluggable));
		checkResult(pPluggable->EnumPluginDescriptors (&pEnumDesc));
		checkResult(pEnumDesc->NextOne (&pPlugin));

	  
		checkResult(pPlugin->GetCategoryClass(&testUID));
		checkExpression(EqualAUID(&testUID, &category) == AAFTrue, AAFRESULT_TEST_FAILED);

//	checkResult(pPlugin->GetPluginVersion(aafProductVersion_t *  pVersion));
		checkResult(pPlugin->GetPluginVersionString(testString, sizeof(testString)));
		checkExpression (wcscmp(testString, manufRev) == 0, AAFRESULT_TEST_FAILED);

//	checkResult(pPlugin->GetProductVersionStringLen(aafInt32 *  pLen));
		checkResult(pPlugin->GetPluginManufacturerName(testString, sizeof(testString)));
		checkExpression (wcscmp(testString, manufName) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetProductManufacturerNameLen(&testInt32));
//		checkExpression(testInt32 == (aafInt32)wcslen(testString), AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetManufacturerInfo(&pNetLoc));
		checkResult(pNetLoc->QueryInterface (IID_IAAFLocator,
                                          (void **)&pLoc));
		checkResult(pLoc->GetPath (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, manuf2URL) == 0, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetManufacturerID(&testUID));
		checkExpression(EqualAUID(&testUID, &manufacturer) == AAFTrue, AAFRESULT_TEST_FAILED);

		/**/
		checkResult(pPlugin->GetHardwarePlatform(&testPlatform));
		checkExpression(EqualAUID(&testPlatform, &kAAFPlatformIndependant) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetPlatformVersionRange(&testMinVersion, &testMaxVersion));
		checkExpression(EqualVersion(&testMinVersion, &sampleMinPlatformVersion) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkExpression(EqualVersion(&testMaxVersion, &sampleMaxPlatformVersion) == AAFTrue, AAFRESULT_TEST_FAILED);

		/**/
 		checkResult(pPlugin->GetEngine(&testEngine));
		checkExpression(EqualAUID(&testEngine, &kAAFNoEngine) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetEngineVersionRange(&testMinVersion, &testMaxVersion));
		checkExpression(EqualVersion(&testMinVersion, &sampleMinEngineVersion) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkExpression(EqualVersion(&testMaxVersion, &sampleMaxEngineVersion) == AAFTrue, AAFRESULT_TEST_FAILED);

		/**/
		checkResult(pPlugin->GetPluginAPI(&testAPI));
		checkExpression(EqualAUID(&testAPI, &kAAFEssencePluginAPI) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->GetPluginAPIVersionRange(&testMinVersion, &testMaxVersion));
		checkExpression(EqualVersion(&testMinVersion, &sampleMinAPIVersion) == AAFTrue, AAFRESULT_TEST_FAILED);
		checkExpression(EqualVersion(&testMaxVersion, &sampleMaxAPIVersion) == AAFTrue, AAFRESULT_TEST_FAILED);

		/**/
		checkResult(pPlugin->GetNumLocators(&count));
		checkExpression (count == 2, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->EnumPluginLocators(&pEnumLoc));

    pLoc->Release(); // this local variable was already has a reference that must be released!
    pLoc = NULL;
		checkResult(pEnumLoc->NextOne (&pLoc));
 		checkResult(pLoc->GetPath (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, manuf1URL) == 0, AAFRESULT_TEST_FAILED);

    pLoc->Release(); // this local variable was already has a reference that must be released!
    pLoc = NULL;
		checkResult(pEnumLoc->NextOne (&pLoc));
 		checkResult(pLoc->GetPath (testString, sizeof(testString)));
		checkExpression (wcscmp(testString, manuf2URL) == 0, AAFRESULT_TEST_FAILED);
		
		checkResult(pPlugin->IsSoftwareOnly(&testBool));
 		checkExpression(testBool == AAFTrue, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->IsAccelerated(&testBool));
 		checkExpression(testBool == AAFFalse, AAFRESULT_TEST_FAILED);
		checkResult(pPlugin->SupportsAuthentication(&testBool));
		checkExpression(testBool == AAFFalse, AAFRESULT_TEST_FAILED);

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup and return
	if (pEnumLoc)
		pEnumLoc->Release();

	if (pLoc)
		pLoc->Release();

  if (pNetLoc)
		pNetLoc->Release();

	if (pPlugin)
		pPlugin->Release();

	if (pEnumDesc)
		pEnumDesc->Release();

	if (pPluggable)
		pPluggable->Release();

	if (pEnumPluggable)
		pEnumPluggable->Release();

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
 

HRESULT CAAFPluginDescriptor::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFPluginDescriptorTest.aaf";

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFPluginDescriptor::test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following IAAFPluginDescriptor methods have not been implemented:" << endl;       
		cout << "     GetPluggableCode" << endl; 
		cout << "     IsPluginLocal" << endl; 
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

	return hr;
}

#if 0
  //****************
  // GetPluggableCode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluggableCode
        // @parm [out] An interface pointer to the pluggable code object
        (ImplAAFPluggableCode ** pCode);




  //****************
  // IsPluginLocal()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsPluginLocal
        // @parm [out] Returns AAFTrue if the plugin is local
        (aafBool *  pIsLocal); 
#endif