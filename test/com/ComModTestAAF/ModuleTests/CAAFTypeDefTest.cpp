// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDef
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

#include "AAF.h"

#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#include <iostream>
using namespace std;
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDictionary> IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>       IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>     IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFTypeDef>    IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt> IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IUnknown>       IUnknownSP;

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

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT TestTypeDef (testMode_t mode)
{
  HRESULT hr = E_FAIL;
  aafProductIdentification_t ProductInfo;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFTypeDef Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = NIL_UID;
  ProductInfo.platform = NULL;

  IAAFFileSP pFile;
  if(mode == kAAFUnitTestReadWrite)
  {
 	 RemoveTestFile (testFileName);
  	hr = AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile);
  }
  else
  {
  	hr = AAFFileOpenExistingRead(testFileName, 0, &pFile);
  }
  
  if (! SUCCEEDED (hr)) return hr;

  IAAFHeaderSP pHeader;
  hr = pFile->GetHeader (&pHeader);
  if (! SUCCEEDED (hr)) return hr;
  assert (pHeader);

  IAAFDictionarySP pDict;
  hr = pHeader->GetDictionary (&pDict);
  if (! SUCCEEDED (hr)) return hr;
  assert (pDict);

  CAAFBuiltinDefs defs (pDict);

  // Let's try to do something interesting with a type definition
  IAAFTypeDefIntSP pTypeDefInt;
  hr = defs.tdInt32()->QueryInterface (IID_IAAFTypeDefInt, (void **) &pTypeDefInt);
  if (! SUCCEEDED (hr)) return hr;
  assert (pTypeDefInt);

  IAAFTypeDefSP pTypeDef;
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

  // Test for RawAccessType().  It should be array of unsigned chars.
  IAAFTypeDefSP pRawType;
  hr = pTypeDef->RawAccessType (&pRawType);
  if (! SUCCEEDED (hr)) return hr;

  IUnknownSP    pUnkRawType;
  hr = pRawType->QueryInterface(IID_IUnknown, (void **)&pUnkRawType);
  if (! SUCCEEDED (hr)) return hr;

  IAAFTypeDefSP pUInt8ArrayType;
  hr = pDict->LookupTypeDef (kAAFTypeID_UInt8Array, &pUInt8ArrayType);
  if (! SUCCEEDED (hr)) return hr;

  IUnknownSP    pUnkUInt8Array;
  hr = pUInt8ArrayType->QueryInterface(IID_IUnknown, (void **)&pUnkUInt8Array);
  if (! SUCCEEDED (hr)) return hr;

  if (pUnkUInt8Array != pUnkRawType)
	return AAFRESULT_TEST_FAILED;


  // that's all we can test for.  Clean up.
  if(mode == kAAFUnitTestReadWrite)
  {
  	hr = pFile->Save();
 	 if (! SUCCEEDED (hr)) return hr;
 }
  hr = pFile->Close();
  if (! SUCCEEDED (hr)) return hr;

  return AAFRESULT_SUCCESS;
}


extern "C" HRESULT CAAFTypeDef_test(testMode_t mode);
extern "C" HRESULT CAAFTypeDef_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
    {
      // Attempt to create an AAFTypeDef.
      hr =  TestTypeDef(mode);
	}
  catch (...)
    {
      cerr << "CAAFTypeDef_test...Caught general C++"
		   << " exception!" << endl;
	  hr = AAFRESULT_TEST_FAILED;
    }

  return hr;
}
