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

#include "CAAFTypeDef.h"
#include "CAAFTypeDef.h"
#ifndef __CAAFTypeDef_h__
#error - improperly defined include guard
#endif

#include "AAFResult.h"
#include "AAFStoredObjectIDs.h"

#include <iostream.h>
#include <assert.h>
#include <stdio.h>


// {C3930DD6-E603-11d2-842A-00600832ACB8}
static aafUID_t TypeID_LocalInt32 = 
{ 0xc3930dd6, 0xe603, 0x11d2, { 0x84, 0x2a, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

static wchar_t testFileName[] = L"AAFTypeDefTest.aaf";


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


static HRESULT TestTypeDef ()
{
  HRESULT hr = E_FAIL;
  aafProductIdentification_t ProductInfo;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTypeDef Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = -1;
  ProductInfo.platform = NULL;

  IAAFFile* pFile = NULL;
  RemoveTestFile (testFileName);
  hr = AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile);
  if (! SUCCEEDED (hr)) return hr;

  IAAFHeader * pHeader = NULL;
  hr = pFile->GetHeader (&pHeader);
  if (! SUCCEEDED (hr)) return hr;
  assert (pHeader);

  IAAFDictionary * pDict = NULL;
  hr = pHeader->GetDictionary (&pDict);
  if (! SUCCEEDED (hr)) return hr;
  assert (pDict);

  // Let's try to do something interesting with a type definition
  IAAFTypeDefInt * pTypeDefInt = NULL;
  hr = pDict->CreateInstance (&AUID_AAFTypeDefInt,
							  IID_IAAFTypeDefInt,
							  (IUnknown **) &pTypeDefInt);
  if (! SUCCEEDED (hr)) return hr;
  assert (pTypeDefInt);

  hr = pTypeDefInt->Initialize (&TypeID_LocalInt32,
								4,        // 4-byte (32-bit) int
								AAFTrue,  // signed
								L"Local 32-bit int");
  if (! SUCCEEDED (hr)) return hr;

  IAAFTypeDef * pTypeDef = NULL;
  hr = pTypeDefInt->QueryInterface(IID_IAAFTypeDef, (void **)&pTypeDef);
  if (! SUCCEEDED (hr)) return hr;
  assert (pTypeDef);

  //
  // Test GetTypeCategory() method
  //

  // first, test error conditions
  hr = pTypeDef->GetTypeCategory(NULL);
  if (AAFRESULT_NULL_PARAM != hr)
	return AAFRESULT_TEST_FAILED;

  // now, test for valid value.  Our type def is integral.
  eAAFTypeCategory_t typeCat = kAAFTypeCatUnknown;
  hr = pTypeDef->GetTypeCategory(&typeCat);
  if (! SUCCEEDED (hr)) return hr;
  if (kAAFTypeCatInt != typeCat)
	return AAFRESULT_TEST_FAILED;

  // that's all we can test for.  Clean up.
  pTypeDef->Release();
  pTypeDefInt->Release();
  pDict->Release();
  pHeader->Release();
  hr = pFile->Save();
  if (! SUCCEEDED (hr)) return hr;
  hr = pFile->Close();
  if (! SUCCEEDED (hr)) return hr;
  pFile->Release();

  return AAFRESULT_SUCCESS;
}


HRESULT CAAFTypeDef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
    {
      // Attempt to create an AAFTypeDef.
      hr =  TestTypeDef();
	}
  catch (...)
    {
      cerr << "CAAFTypeDef::test...Caught general C++"
        " exception!" << endl; 
    }

  // When all of the functionality of this class is tested, we can return success.
  // When a method and its unit test have been implemented, remove it from the list.
  if (SUCCEEDED(hr))
	{
	  cout << "The following AAFTypeDef methods have not been implemented:" << endl; 
	  cout << "     RawAccessType - needs unit test" << endl;
	  cout << "     GetMinVersion - needs unit test" << endl;
	  cout << "     SetMinVersion - needs unit test" << endl;
	  cout << "     GetMaxVersion - needs unit test" << endl;
	  cout << "     SetMaxVersion - needs unit test" << endl;
	  cout << "     GetSwapNeeded - needs unit test" << endl;
	  cout << "     SetSwapNeeded - needs unit test" << endl;
	  cout << "     GetRefValues  - needs unit test" << endl;
	  hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
	}

  return hr;
}
