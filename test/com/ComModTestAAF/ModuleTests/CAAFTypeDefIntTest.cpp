// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefInt
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

#include <iostream>
using namespace std;
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

// Foward declaration.
extern "C" HRESULT CAAFTypeDefInt_test(testMode_t mode);

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

static wchar_t testFileName[] = L"AAFTypeDefIntTest.aaf";


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


static HRESULT GetOneTypeDef (IAAFDictionary *  pDict,
								 const aafUID_t &  id,
								 IAAFTypeDefInt ** ppTD)
{
  assert (pDict);
  assert (ppTD);

  HRESULT hr = S_OK;
  IAAFTypeDef * pTypeDef = NULL;

  hr = pDict->LookupTypeDef(id, &pTypeDef);
  if (SUCCEEDED(hr))
  {
    hr = pTypeDef->QueryInterface (IID_IAAFTypeDefInt, (void **) ppTD);
    pTypeDef->Release();
  }

  return hr;
} 


//
// We're given:
// - a typedef of a certain size
// - the size of the typedef
// - the desired 'set' data size
// - the desired 'get' data size
// - the desired 'set' data
//
// And we're supposed to:
// - create a prop val
// - get its value
// - set its value
// - get its value
//
// At each step test to see if
// - it *should* succeed
// - it *does* succeed
// - gotten data matches set data
//
static HRESULT TestOneValue (aafUInt32 setDataSize,
							 aafInt64  setData,

							 aafUInt32 getDataSize,

							 aafUInt32 pvSize,	// desired pv size
							 aafBool   isSigned,
							 IAAFTypeDefInt * ptd)  // should match pvSize
{
  // The idea is that success should happen when
  // setDataSize<=pvSize<=getDataSize.
  //
  // If pvSize isn't one of (1, 2, 4, 8) it should also fail.
  //
  // Also if setDataSize<pvSize or pvSize<getDataSize, sign-extension
  // or zero-fill should happen.

  IAAFPropertyValue * ppv = NULL;
  HRESULT hr = E_FAIL;

  assert (ptd);

  if (((1 != setDataSize) &&
	   (2 != setDataSize) &&
	   (4 != setDataSize) &&
	   (8 != setDataSize)) ||
	  (setDataSize > pvSize))
	{
	  // This thing's gonna fail.  Let's try to create one, and see
	  // what happens.
	  hr = ptd->CreateValue ((aafMemPtr_t) &setData, setDataSize, &ppv);
	  if (AAFRESULT_BAD_SIZE != hr)
		{
		  if (ppv)
			ppv->Release();
		  return AAFRESULT_TEST_FAILED;
		}
	  // good, the creation failed.  Let's let's create one that
	  // *will* work, and make sure it fails when we try to 'set' it.
	  hr = ptd->CreateValue ((aafMemPtr_t) &setData, 1, &ppv);
	  if (! SUCCEEDED (hr))
		{
		  if (ppv)
			ppv->Release();
		  return AAFRESULT_TEST_FAILED;
		}
      assert (ppv);

	  hr = ptd->SetInteger (ppv, (aafMemPtr_t) &setData, setDataSize);
	  if (AAFRESULT_BAD_SIZE != hr)
		{
		  if (ppv)
			ppv->Release();
		  return AAFRESULT_TEST_FAILED;
		}

	  // OK, it failed where it should have.  Make sure we return the
	  // 'bad size' err code
	  if (ppv)
		ppv->Release();
	  return AAFRESULT_BAD_SIZE;
	}

  // OK, the creation should succeed.
  aafInt8  v1;
  aafInt16 v2;
  aafInt32 v4;
  aafInt64 v8;

  switch (setDataSize)
	{
	case 1:
	  v1 = (aafInt8) setData;
	  hr = ptd->CreateValue ((aafMemPtr_t)&v1, 1, &ppv);
	  break;
	case 2:
	  v2 = (aafInt16) setData;
	  hr = ptd->CreateValue ((aafMemPtr_t) &v2, 2, &ppv);
	  break;
	case 4:
	  v4 = (aafInt32) setData;
	  hr = ptd->CreateValue ((aafMemPtr_t) &v4, 4, &ppv);
	  break;
	case 8:
	  v8 = setData;
	  hr = ptd->CreateValue ((aafMemPtr_t) &v8, 8, &ppv);
	  break;
	default:
	  assert (0);
	}
  
  if (! SUCCEEDED (hr))
	{
	  if (ppv)
		ppv->Release();
	  return hr;
	}

  // OK, ppv is created.  Let's try to get the value.
  if (((1 != getDataSize) &&
	   (2 != getDataSize) &&
	   (4 != getDataSize) &&
	   (8 != getDataSize)) ||
	  (getDataSize < pvSize))
	{
	  // It should fail.
	  aafInt64 junk;
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &junk, getDataSize);
	  if (AAFRESULT_BAD_SIZE != hr)
		{
		  if (ppv)
			ppv->Release();
		  return AAFRESULT_TEST_FAILED;
		}

	  // this is what we were expecting.
	  if (ppv)
		ppv->Release();
	  return AAFRESULT_BAD_SIZE;
	}

  v8 = v4 = v2 = v1 = -1;
  aafInt64 getVal = -1;
  // OK, it should succeed.
  switch (getDataSize)
	{
	case 1:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v1, 1);
	  if (! isSigned)
		getVal = (aafUInt8) v1;
	  else
		getVal = v1;
	  break;
	case 2:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v2, 2);
	  if (! isSigned)
		getVal = (aafUInt16) v2;
	  else
		getVal = v2;
	  break;
	case 4:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v4, 4);
	  if (! isSigned)
		getVal = (aafUInt32) v4;
	  else
		getVal = v4;
	  break;
	case 8:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v8, 8);
	  getVal = v8;
	  break;
	default:
	  assert (0);
	}
  if (! SUCCEEDED (hr))
	{
	  if (ppv)
		ppv->Release();
	  return hr;
	}

  aafUInt32 compareSize =
	getDataSize < setDataSize ? getDataSize : setDataSize;
  aafInt64 compareMask = ~0;
  if (compareSize < 8)
	compareMask = (((aafInt64)1) << (compareSize*8))-1;
  if ((getVal&compareMask) != (setData&compareMask))
	{
	  if (ppv)
		ppv->Release();
	  return AAFRESULT_TEST_FAILED;
	}

  // Now let's try a set/get cycle again.

  switch (setDataSize)
	{
	case 1:
	  if (! isSigned)
		v1 = (aafUInt8) setData;
	  else
		v1 = (aafInt8) setData;
	  hr = ptd->SetInteger (ppv, (aafMemPtr_t) &v1, 1);
	  break;
	case 2:
	  if (! isSigned)
		v2 = (aafUInt16) setData;
	  else
		v2 = (aafInt16) setData;
	  hr = ptd->SetInteger (ppv, (aafMemPtr_t) &v2, 2);
	  break;
	case 4:
	  if (! isSigned)
		v4 = (aafUInt32) setData;
	  else
		v4 = (aafInt32) setData;
	  hr = ptd->SetInteger (ppv, (aafMemPtr_t) &v4, 4);
	  break;
	case 8:
	  v8 = setData;
	  hr = ptd->SetInteger (ppv, (aafMemPtr_t) &v8, 8);
	  break;
	default:
	  assert (0);
	}
  if (! SUCCEEDED (hr))
	{
	  if (ppv)
		ppv->Release();
	  return hr;
	}

  v8 = v4 = v2 = v1 = -1;
  switch (getDataSize)
	{
	case 1:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v1, 1);
	  if (! isSigned)
		getVal = (aafUInt8) v1;
	  else
		getVal = v1;
	  break;
	case 2:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v2, 2);
	  if (! isSigned)
		getVal = (aafUInt16) v2;
	  else
		getVal = v2;
	  break;
	case 4:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v4, 4);
	  if (! isSigned)
		getVal = (aafUInt32) v4;
	  else
		getVal = v4;
	  break;
	case 8:
	  hr = ptd->GetInteger (ppv, (aafMemPtr_t) &v8, 8);
	  getVal = v8;
	  break;
	default:
	  assert (0);
	}

  // don't need this anymore
  if (ppv)
	ppv->Release();

  checkResult (hr);

  if ((getVal&compareMask) != (setData&compareMask))
	return AAFRESULT_TEST_FAILED;

  return AAFRESULT_SUCCESS;
}



static HRESULT TestTypeDefInt (testMode_t mode)
{
  HRESULT hr = E_FAIL;
  HRESULT caughtHr = AAFRESULT_SUCCESS;
  aafProductIdentification_t ProductInfo;

  IAAFFile* pFile = NULL;
  IAAFHeader * pHeader = NULL;
  IAAFDictionary * pDict = NULL;
  IAAFTypeDefInt * ptds8 = NULL;
  IAAFTypeDefInt * ptds16 = NULL;
  IAAFTypeDefInt * ptds32 = NULL;
  IAAFTypeDefInt * ptds64 = NULL;
  IAAFTypeDefInt * ptdu8 = NULL;
  IAAFTypeDefInt * ptdu16 = NULL;
  IAAFTypeDefInt * ptdu32 = NULL;
  IAAFTypeDefInt * ptdu64 = NULL;

  try
	{
	  aafProductVersion_t v;
	  v.major = 1;
	  v.minor = 0;
	  v.tertiary = 0;
	  v.patchLevel = 0;
	  v.type = kAAFVersionUnknown;

	  ProductInfo.companyName = L"AAF Developers Desk";
	  ProductInfo.productName = L"AAFTypeDefInt Test";
	  ProductInfo.productVersion = &v;
	  ProductInfo.productVersionString = NULL;
	  ProductInfo.productID = UnitTestProductID;
	  ProductInfo.platform = NULL;

	  if(mode == kAAFUnitTestReadWrite)
	  {
		  RemoveTestFile (testFileName);
	  	 checkResult (AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile));
	  }
	  else
	  {
	  	 checkResult (AAFFileOpenExistingRead(testFileName, 0, &pFile));
	  }
	  
      assert (pFile);
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);

	  checkResult (pHeader->GetDictionary (&pDict));
	  assert (pDict);

	  // Let's create some typedef ints and try to initialize them; also
	  // test GetSize() and IsSigned()

	  // 1-byte signed
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_Int8,  &ptds8));
	  assert (ptds8);

  // 2-byte signed
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_Int16, &ptds16));
	  assert (ptds16);

  // 4-byte signed
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_Int32, &ptds32));
	  assert (ptds32);

  // 8-byte signed
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_Int64, &ptds64));
	  assert (ptds64);

  // 1-byte unsigned
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_UInt8, &ptdu8));
	  assert (ptdu8);

  // 2-byte unsigned
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_UInt16, &ptdu16));
	  assert (ptdu16);

  // 4-byte unsigned
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_UInt32, &ptdu32));
	  assert (ptdu32);

  // 8-byte unsigned
	  checkResult (GetOneTypeDef (pDict, kAAFTypeID_UInt64, &ptdu64));
	  assert (ptdu64);

	  const aafUInt32 sizeTable[4] = {1, 2, 4, 8};
	  IAAFTypeDefInt *typeDefTable[4][2] =
	  {
		ptdu8, ptds8,
		ptdu16, ptds16,
		ptdu32, ptds32,
		ptdu64, ptds64
	  };

	  aafUInt32  setSize;
	  aafUInt32 getSize;
	  aafUInt32 pvIndex;
	  aafUInt32 pvSize;
	  aafUInt8  sign;
	  aafUInt8  negVal;

	  for (sign = 0; sign <= 1; sign++)
		for (getSize = 0; getSize <10; getSize++)
		  for (setSize = 0; setSize <10; setSize++)
			for (pvIndex = 0; pvIndex <= 3; pvIndex++)
			  for (negVal = 0; negVal <= 1; negVal++)
				{
				  IAAFTypeDefInt * td;

				  pvSize = sizeTable[pvIndex];

				  if ((1 == pvSize) ||
					  (2 == pvSize) ||
					  (4 == pvSize) ||
					  (8 == pvSize))
					{
					  td = typeDefTable[pvIndex][sign];
					}
				  else
					continue;	// skip rest of the loop

				  aafInt64 setData = negVal ? -(aafInt32)setSize : setSize;

				  hr = TestOneValue (setSize,
									 setData,
									 getSize,
									 pvSize,
									 sign? kAAFTrue : kAAFFalse,
									 td);

				  //
				  // first test for expected failures
				  //
				  if ((setSize != 1) &&
					  (setSize != 2) &&
					  (setSize != 4) &&
					  (setSize != 8))
					{
					  if (AAFRESULT_BAD_SIZE != hr)
						throw AAFRESULT_TEST_FAILED;
					}

				  else if ((getSize != 1) &&
					  (getSize != 2) &&
					  (getSize != 4) &&
					  (getSize != 8))
					{
					  if (AAFRESULT_BAD_SIZE != hr)
						throw AAFRESULT_TEST_FAILED;
					}

				  else if ((setSize > pvSize) ||
					  (pvSize > getSize))
					{
					  if (AAFRESULT_BAD_SIZE != hr)
						throw AAFRESULT_TEST_FAILED;
					}

				  else
					{
					  // if we're here, it should pass.
					  checkResult (hr);
					}
				}
	}
  catch (HRESULT & rResult)
	{
	  caughtHr = rResult;
	}

  if (ptds8)
    ptds8->Release();
  if (ptds16)
    ptds16->Release();
  if (ptds32)
    ptds32->Release();
  if (ptds64)
    ptds64->Release();
  if (ptdu8)
    ptdu8->Release();
  if (ptdu16)
    ptdu16->Release();
  if (ptdu32)
  	ptdu32->Release();
  if (ptdu64)
  	ptdu64->Release();
  if (pDict)
    pDict->Release();
  if (pHeader)
  	pHeader->Release();
  if (pFile)
  {
  	if(mode == kAAFUnitTestReadWrite)
	{
		hr = pFile->Save();
		if (! SUCCEEDED (hr))
		{  
	  	pFile->Release();
	  	return hr;
	  	}
	}
	hr = pFile->Close();
	pFile->Release();
  }

  
  return (caughtHr != AAFRESULT_SUCCESS) ? caughtHr : hr;
}


HRESULT CAAFTypeDefInt_test(testMode_t mode);
HRESULT CAAFTypeDefInt_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
    {
      hr =  TestTypeDefInt(mode);
	}
  catch (...)
    {
      cerr << "CAAFTypeDefInt_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
    }
  return hr;
}
