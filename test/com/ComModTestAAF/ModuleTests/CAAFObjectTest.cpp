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
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFFile>           IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>         IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFIdentification> IAAFIdentificationSP;

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


static HRESULT ObjectWriteTest ()
{
  HRESULT hr = AAFRESULT_TEST_FAILED;

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
	  aafProductVersion_t v;
	  v.major = 1;
	  v.minor = 0;
	  v.tertiary = 0;
	  v.patchLevel = 0;
	  v.type = kAAFVersionUnknown;
	  ProductInfo.companyName = L"AAF Developers Desk";
	  ProductInfo.productName = L"AAFObject Test";
	  ProductInfo.productVersion = &v;
	  ProductInfo.productVersionString = NULL;
	  ProductInfo.productID = UnitTestProductID;
	  ProductInfo.platform = NULL;

	  RemoveTestFile (testFileName);
	  checkResult (AAFFileOpenNewModify(testFileName,
										0,
										&ProductInfo,
										&pFile));
	  assert (pFile);
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);
	  checkResult (pHeader->GetDictionary (&pDict));
	  assert (pDict);
	  CAAFBuiltinDefs defs (pDict);

	  checkResult (defs.cdCompositionMob()->
				   CreateInstance (IID_IAAFCompositionMob,
								   (IUnknown **) &pCMob));
	  assert (pCMob);
	  checkResult (pCMob->QueryInterface (IID_IAAFMob,
										  (void **) &pMob));
	  assert (pMob);

	  checkResult (pCMob->Initialize (L"TestMob"));
	  checkResult (pHeader->AddMob (pMob));

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

	  //
	  // Check Generation functionality.
	  //

	  // Test IsGenerationTracked(), DisableGenerationTracking()
	  aafBoolean_t b;
	  checkResult (pObj->IsGenerationTracked (&b));
	  // Generation is not tracked by default.
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // check for redundant disables
	  checkResult (pObj->DisableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // Change the object (using the CompMob interface) so that
	  // subsequent Save() will know it's a dirty object.
	  aafRational_t eu = { 1, 1 };
	  checkResult (pCMob->SetDefaultFade (0, kAAFFadeNone, eu));

	  // Perform one save and verify that generation was not tracked.
	  checkResult (pFile->Save());

	  AAFRESULT testhr;
	  IAAFIdentificationSP pIdent;
	  aafUID_t identAuid;
	  testhr = pObj->GetGeneration(&pIdent);
	  checkExpression (AAFRESULT_INVALID_PARAM == testhr,
					   AAFRESULT_TEST_FAILED);
	  testhr = pObj->GetGenerationAUID(&identAuid);
	  checkExpression (AAFRESULT_INVALID_PARAM == testhr,
					   AAFRESULT_TEST_FAILED);
	  // make sure gen still isn't tracked
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // Test EnableGenerationTracking()
	  checkResult (pObj->EnableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFTrue, AAFRESULT_TEST_FAILED);

	  checkResult (pObj->DisableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // Check for redundant enables
	  checkResult (pObj->EnableGenerationTracking());
	  checkResult (pObj->EnableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFTrue, AAFRESULT_TEST_FAILED);

	  // Now that generation tracking is enabled, try a couple of
	  // save()s to see if the latest ident is saved.
	  //
	  // First, add a new identification.  We'll keep track of the
	  // identifications we're putting in by using a bogus productID
	  // AUID, and put small integers in the aafUID_t.Data1 field.
	  //
	  // Here's where we define two version AUIDs for our use.
	  aafUID_t versionUid1 = { 0 };
	  versionUid1.Data1 = 1;
	  aafUID_t versionUid2 = { 0 };
	  versionUid2.Data1 = 2;

	  // Now make a new identification with the first version AUID,
	  // and append it to the header.
	  IAAFIdentificationSP pNewIdent;
	  checkResult (defs.cdIdentification()->
				   CreateInstance (IID_IAAFIdentification,
								   (IUnknown **) &pNewIdent));
	  assert (pNewIdent);
	  checkResult (pNewIdent->Initialize (L"Avid",
										  L"Test File",
										  L"First Test Version",
										  versionUid1));
	  checkResult (pHeader->AppendIdentification (pNewIdent));

	  // With Version1 identification in the header, do a save and see
	  // if that generation made it to our file.
	  //
	  // First, change object to make sure Save() will save it
	  checkResult (pCMob->SetDefaultFade (1, kAAFFadeNone, eu));

	  checkResult (pFile->Save());
	  IAAFIdentificationSP pReadIdent1;
	  checkResult (pObj->GetGeneration(&pReadIdent1));
	  aafUID_t readAuid1 = { 0 };
	  checkResult (pReadIdent1->GetProductID(&readAuid1));
	  checkExpression (1 == readAuid1.Data1, AAFRESULT_TEST_FAILED);

	  // Now make a new identification with the second version AUID,
	  // and append it to the header.
	  checkResult (defs.cdIdentification()->
				   CreateInstance (IID_IAAFIdentification,
								   (IUnknown **) &pNewIdent));
	  assert (pNewIdent);
	  aafUID_t junkAuid;
	  // make sure this isn't allowed before initialization
	  HRESULT temphr = pNewIdent->GetProductID(&junkAuid);
	  checkExpression (AAFRESULT_NOT_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  checkResult (pNewIdent->Initialize (L"Avid",
										  L"Test File",
										  L"Second Test Version",
										  versionUid2));

	  // this should now work after initialization
	  checkResult (pNewIdent->GetProductID(&junkAuid));

	  checkResult (pHeader->AppendIdentification (pNewIdent));

	  // With Version2 identification in the header, do a save and see
	  // if that generation made it to our file.
	  //
	  // First, change object to make sure Save() will save it
	  checkResult (pCMob->SetDefaultFade (2, kAAFFadeNone, eu));
	  checkResult (pFile->Save());
	  IAAFIdentificationSP pReadIdent2;
	  checkResult (pObj->GetGeneration(&pReadIdent2));
	  aafUID_t readAuid2 = { 0 };
	  checkResult (pReadIdent2->GetProductID(&readAuid2));
	  checkExpression (2 == readAuid2.Data1, AAFRESULT_TEST_FAILED);

	  hr = AAFRESULT_SUCCESS;
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
	  AAFRESULT temphr;
	  temphr = pFile->Save();
	  if (! SUCCEEDED (temphr)) return temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
	  pFile->Release();
	}
  
  return hr;
}


static HRESULT ObjectReadTest ()
{
  HRESULT hr = AAFRESULT_TEST_FAILED;
	IAAFFile * pFile = NULL;

  try
	{
	  checkResult (AAFFileOpenExistingRead(testFileName,
										   0,
										   &pFile));
	  assert (pFile);
	  IAAFHeaderSP pHeader;
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);

	  IAAFIdentificationSP pReadIdent;
	  checkResult (pHeader->GetLastIdentification(&pReadIdent));

	  // ident should be initialized, so this should work 
	  aafUID_t junkAuid;
	  checkResult (pReadIdent->GetProductID(&junkAuid));

	  hr = AAFRESULT_SUCCESS;
	}

  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pFile)
  {
	  AAFRESULT temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
    pFile->Release();
    pFile = NULL;
  }

  return hr;
}


extern "C" HRESULT CAAFObject_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  hr = ObjectWriteTest ();
	  if (FAILED(hr))
		{
		  cerr << "CAAFObject_test...FAILED!" << endl;
		  return hr;
		}

	  hr = ObjectReadTest ();
	  if (FAILED(hr))
		{
		  cerr << "CAAFObject_test...FAILED!" << endl;
		  return hr;
		}

		
		if(hr == AAFRESULT_SUCCESS)
			hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
	}
  catch (...)
	{
	  cerr << "CAAFObject_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
