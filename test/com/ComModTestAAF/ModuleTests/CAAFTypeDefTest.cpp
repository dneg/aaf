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

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

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
  ProductInfo.productID = NIL_UID;
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
