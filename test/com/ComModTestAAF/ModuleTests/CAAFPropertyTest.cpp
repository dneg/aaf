// @doc INTERNAL
// @com This file implements the module test for CAAFProperty
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
#include <stdio.h>
#include <assert.h>



// {669349B1-EBC3-11d2-842C-00600832ACB8}
static aafUID_t PropID_Local1 = 
{ 0x669349b1, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B2-EBC3-11d2-842C-00600832ACB8}
static aafUID_t TypeID_Local1 = 
{ 0x669349b2, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };



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

static wchar_t testFileName[] = L"AAFPropertyTest.aaf";


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


// not ready for prime time
#if 0

static HRESULT ObjectTest ()
{
  HRESULT hr = AAFRESULT_SUCCESS;

  aafProductIdentification_t ProductInfo;
  IAAFFile* pFile = NULL;
  IAAFHeader * pHeader = NULL;
  IAAFDictionary * pDict = NULL;
  IAAFPropertyDef * pPropDef = NULL;
  IAAFPropertyValue * pPropVal = NULL;
  IAAFProperty * pProp = NULL;

  try
	{
	  ProductInfo.companyName = L"AAF Developers Desk";
	  ProductInfo.productName = L"AAFProperty Test";
	  ProductInfo.productVersion.major = 1;
	  ProductInfo.productVersion.minor = 0;
	  ProductInfo.productVersion.tertiary = 0;
	  ProductInfo.productVersion.patchLevel = 0;
	  ProductInfo.productVersion.type = kVersionUnknown;
	  ProductInfo.productVersionString = NULL;
	  ProductInfo.productID = -1;
	  ProductInfo.platform = NULL;

	  RemoveTestFile (testFileName);
	  checkResult (AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile));
	  assert (pFile);
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);
	  checkResult (pHeader->GetDictionary (&pDict));
	  assert (pDict);
	  CAAFBuiltinDefs defs (pDict);

	  checkResult (pDict->CreateInstance (AUID_IAAFTypeDefInt,
										  IID_IAAFTypeDefInt,
										  (IUnknown **) &pTypeDefInt));
	  assert (pTypeDefInt);
	  checkResult (pTypeDefInt->Initialize (&TypeID_Local1,
											1,
											AAFTrue,
											L"Fake local 1-byte signed int type");
	  
	  checkResult (pDict->CreateInstance (defs.cdPropertyDef(),
										  IID_IAAFPropertyDef,
										  (IUnknown **) &pPropDef));
	  assert (pPropDef);
	  checkResult (pPropDef->Initialize (&PropID_Local1,
										 pTypeDefInt,
										 L"Fake local property def");										 
	  checkResult (pDict->CreateInstance (defs.cdProperty(),
										  IID_IAAFProperty,
										  (IUnknown **) &pProp));
	  assert (pProp);
	}
  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pProp) pProp->Release ();
  if (pPropVal) pPropVal->Release ();
  if (pPropDef) pPropVal->Release ();
  if (pDict) pDict->Release ();
  if (pHeader) pHeader->Release ();
  if (pFile)
	{
	  AAFRESULT temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
	  pFile->Release();
	}
  
  return hr;
}
#endif // 0

extern "C" HRESULT CAAFProperty_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
    {
	  // hr = PropertyTest ();

      // if (FAILED(hr))
      // {
      //  cerr << "CAAFProperty_test...FAILED!";
      //  return hr;
      // }

      return AAFRESULT_NOT_IMPLEMENTED;

    }
  catch (...)
    {
      cerr << "CAAFProperty_test...Caught general C++"
        " exception!" << endl; 
    }

  return hr;
}
