// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDef
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

#include "AAFResult.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#include <iostream.h>
#include <assert.h>
#include <stdio.h>

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

static HRESULT TestTypeDef ()
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
  RemoveTestFile (testFileName);
  hr = AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile);
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
  hr = pFile->Save();
  if (! SUCCEEDED (hr)) return hr;
  hr = pFile->Close();
  if (! SUCCEEDED (hr)) return hr;

  return AAFRESULT_SUCCESS;
}


extern "C" HRESULT CAAFTypeDef_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
    {
      // Attempt to create an AAFTypeDef.
      hr =  TestTypeDef();
	}
  catch (...)
    {
      cerr << "CAAFTypeDef_test...Caught general C++"
		   << " exception!" << endl;
	  hr = AAFRESULT_TEST_FAILED;
    }

  return hr;
}
