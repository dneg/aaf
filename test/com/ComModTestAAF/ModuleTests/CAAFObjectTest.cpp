// @doc INTERNAL
// @com This file implements the module test for CAAFObject
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
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>


static aafUID_t    fillerUID = DDEF_Timecode;
static aafLength_t  fillerLength = 3200;


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

static wchar_t testFileName[] = L"AAFObjectTest.aaf";


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


static HRESULT ObjectTest ()
{
  HRESULT hr = AAFRESULT_SUCCESS;

  aafProductIdentification_t ProductInfo;
  IAAFFile* pFile = NULL;
  IAAFHeader * pHeader = NULL;
  IAAFDictionary * pDict = NULL;
  IAAFCompositionMob * pCMob = NULL;
  IAAFMob * pMob = NULL;
  IAAFObject * pObj = NULL;
  IEnumAAFProperties * pEnum = NULL;

  try
	{
	  ProductInfo.companyName = L"AAF Developers Desk";
	  ProductInfo.productName = L"AAFObject Test";
	  ProductInfo.productVersion.major = 1;
	  ProductInfo.productVersion.minor = 0;
	  ProductInfo.productVersion.tertiary = 0;
	  ProductInfo.productVersion.patchLevel = 0;
	  ProductInfo.productVersion.type = kVersionUnknown;
	  ProductInfo.productVersionString = NULL;
	  ProductInfo.productID = UnitTestProductID;
	  ProductInfo.platform = NULL;

	  RemoveTestFile (testFileName);
	  checkResult (AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile));
	  assert (pFile);
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);
	  checkResult (pHeader->GetDictionary (&pDict));
	  assert (pDict);

	  checkResult (pDict->CreateInstance (&AUID_AAFCompositionMob,
										  IID_IAAFCompositionMob,
										  (IUnknown **) &pCMob));
	  assert (pCMob);
	  checkResult (pCMob->Initialize (L"TestMob"));
	  
	  checkResult (pCMob->QueryInterface (IID_IAAFMob,
										  (void **) &pMob));
	  assert (pMob);
	  checkResult (pHeader->AppendMob (pMob));

	  checkResult (pCMob->QueryInterface (IID_IAAFObject,
										  (void **) &pObj));
	  assert (pObj);

	  aafUInt32 propCount = 0;
	  checkResult (pObj->CountProperties (&propCount));
	  // checkExpression(9 == propCount, AAFRESULT_TEST_FAILED);

	  checkResult (pObj->GetProperties (&pEnum));
	  checkExpression (pEnum != 0, AAFRESULT_TEST_FAILED);
	  while (propCount--)
		{
		  IAAFProperty * pProp = NULL;
		  checkResult (pEnum->NextOne (&pProp));
		  checkExpression (pProp != 0, AAFRESULT_TEST_FAILED);
		  pProp->Release();
		}
	}
  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pEnum) pEnum->Release();
  if (pCMob) pCMob->Release();
  if (pMob) pCMob->Release();
  if (pObj) pObj->Release();
  if (pDict) pDict->Release();
  if (pHeader) pHeader->Release();
  if (pFile)
	{
	  AAFRESULT temphr = pFile->Save();
	  if (! SUCCEEDED (temphr)) return temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
	  pFile->Release();
	}
  
  return hr;
}


extern "C" HRESULT CAAFObject_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  hr = ObjectTest ();

	  if (FAILED(hr))
		{
		  cerr << "CAAFObject_test...FAILED!" << endl;
		  return hr;
		}

	}
  catch (...)
	{
	  cerr << "CAAFObject_test...Caught general C++"
		" exception!" << endl; 
	}

  return hr;
}
