// @doc INTERNAL
// @com This file implements the module test for CAAFPropertyValue
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
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include <iostream.h>
#include <assert.h>
#include <stdio.h>

// {C3930DD1-E603-11d2-842A-00600832ACB8}
static aafUID_t TypeID_LocalInt32 = 
{ 0xc3930dd1, 0xe603, 0x11d2, { 0x84, 0x2a, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };





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

static wchar_t testFileName[] = L"AAFPropertyValueTest.aaf";


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


static HRESULT TestPropertyValue ()
{
  // HRESULT hr = E_FAIL;
  long hr = E_FAIL;
  aafProductIdentification_t ProductInfo;

  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFPropertyValue Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;

  IAAFFile* pFile = NULL;
  RemoveTestFile (testFileName);
  checkResult (AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile));
  assert (pFile);

  IAAFHeader * pHeader = NULL;
  hr = pFile->GetHeader (&pHeader);
  if (! SUCCEEDED (hr)) return hr;
  assert (pHeader);

  IAAFDictionary * pDict = NULL;
  hr = pHeader->GetDictionary (&pDict);
  if (! SUCCEEDED (hr)) return hr;
  assert (pDict);

  CAAFBuiltinDefs defs (pDict);

  // Let's try to do something interesting with a type definition
  IAAFTypeDefInt * pTypeDef = NULL;
  hr = defs.cdTypeDefInt()->CreateInstance (IID_IAAFTypeDefInt,
											(IUnknown **) &pTypeDef);
  if (! SUCCEEDED (hr)) return hr;
  assert (pTypeDef);

  hr = pTypeDef->Initialize (TypeID_LocalInt32,
							 4,        // 4-byte (32-bit) int
							 AAFTrue,  // signed
							 L"Local 32-bit int");
  if (! SUCCEEDED (hr)) return hr;

  // Now attempt to create invalid property values; check for errors.
  const aafInt32 forty_two = 42;
  IAAFPropertyValue * pv = NULL;
  // This is what a correct one would look like; we're not ready for that yet:
  // hr = pTypeDef->CreateValue (&forty_two, 4, &pv);

  // set this to -1 to see if it gets modified
  pv = (IAAFPropertyValue *) (-1);

  // Try null pVal
  hr = pTypeDef->CreateValue (NULL, 4, &pv);
  if (AAFRESULT_NULL_PARAM != hr)
	return AAFRESULT_TEST_FAILED;
  if ((IAAFPropertyValue *)(-1) != pv)
	return AAFRESULT_TEST_FAILED;

  // Try valSize too large
  hr = pTypeDef->CreateValue ((aafMemPtr_t) &forty_two, 8, &pv);
  if (AAFRESULT_BAD_SIZE != hr)
	return AAFRESULT_TEST_FAILED;
  if ((IAAFPropertyValue *)(-1) != pv)
	return AAFRESULT_TEST_FAILED;

  // Now try correct one
  pv = NULL;
  hr = pTypeDef->CreateValue ((aafMemPtr_t) &forty_two, 4, &pv);
  if (! SUCCEEDED (hr)) return hr;
  if (! pv)
	return AAFRESULT_TEST_FAILED;

  // That one worked; let's try one with a smaller init size (should
  // also work)
  pv->Release();
  pv = NULL;
  const aafInt16 fifty_seven = 57;
  hr = pTypeDef->CreateValue ((aafMemPtr_t) &fifty_seven, 2, &pv);
  if (! SUCCEEDED (hr)) return hr;
  if (! pv)
	return AAFRESULT_TEST_FAILED;

  // cool.  Now we should have a good property value whose value is 57.
  // check GetType() for error condition
  hr = pv->GetType (NULL);
  if (AAFRESULT_NULL_PARAM != hr)
	return AAFRESULT_TEST_FAILED;

  // this GetType() call should work, and return the original type def.
  IAAFTypeDef * propType = NULL;
  hr = pv->GetType (&propType);
  if (! SUCCEEDED (hr)) return hr;
  if (! propType)
	return AAFRESULT_TEST_FAILED;

  // Convert both to IUnknown for comparison
  IUnknown * pPropUnknown = NULL;
  IUnknown * pTypeDefUnknown = NULL;
  hr = propType->QueryInterface(IID_IUnknown, (void **)&pPropUnknown);
  if (! SUCCEEDED (hr)) return hr;
  assert (pPropUnknown);
  hr = pTypeDef->QueryInterface(IID_IUnknown, (void **)&pTypeDefUnknown);
  if (! SUCCEEDED (hr)) return hr;
  assert (pTypeDefUnknown);

  if (pPropUnknown != pTypeDefUnknown)
	return AAFRESULT_TEST_FAILED;

  // Test IsDefinedType ()
  // (Currently only returns true.)
  aafBool b = AAFFalse;
  hr = pv->IsDefinedType (&b);
	if (! SUCCEEDED (hr)) return hr;
  if (AAFTrue != b)
	return AAFRESULT_TEST_FAILED;

  pTypeDefUnknown->Release();
  pPropUnknown->Release();
  pTypeDef->Release();
  propType->Release();
  pv->Release();
  pDict->Release();
  pHeader->Release();
  hr = pFile->Save();
  if (! SUCCEEDED (hr)) return hr;
  hr = pFile->Close();
  if (! SUCCEEDED (hr)) return hr;
  pFile->Release();

  return AAFRESULT_SUCCESS;
}



extern "C" HRESULT CAAFPropertyValue_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  hr = TestPropertyValue();
	}
  catch (...)
	{
	  cerr << "CAAFPropertyValue_test...Caught general C++ exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
