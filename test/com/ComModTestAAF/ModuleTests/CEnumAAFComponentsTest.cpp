// @doc INTERNAL
// @com This file implements the module test for CEnumAAFComponents
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
#include "AAFDataDefs.h"
#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#define kNumComponents	5


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


static HRESULT CreateAAFSequence(IAAFDictionary *pDictionary,
                                 IAAFSequence** ppSequence)
{
	IAAFSequence*	pSequence = NULL;
	HRESULT			hr = S_OK;
	aafUInt32		i;

	CAAFBuiltinDefs defs (pDictionary);

 	hr = defs.cdSequence()->
	  CreateInstance(IID_IAAFSequence, 
					 (IUnknown **)&pSequence);		
 	if (SUCCEEDED(hr))
	{
		pSequence->Initialize(defs.ddSound());

		//
		//	Add some segments.  Need to test failure conditions
		//	(i.e. starting/ending w/ transition, two trans back
		//	to bacl).
		//
		for(i = 0; i < kNumComponents; i++)
		{
	    IAAFComponent*	pComponent = NULL;
			aafLength_t		len = 10;

			hr = defs.cdFiller()->
			  CreateInstance(IID_IAAFComponent, 
							 (IUnknown **)&pComponent);
 			if (FAILED(hr))
				break;

			pComponent->SetDataDef(defs.ddSound());
			pComponent->SetLength(len);
			hr = pSequence->AppendComponent(pComponent);

			pComponent->Release();
      pComponent = NULL;

			if (FAILED(hr))
				break;
		}
	}

	if (SUCCEEDED(hr))
	{
		*ppSequence = pSequence;
	}
	else
	{
		pSequence->Release();
		*ppSequence = NULL;
	}

	return hr;
}

static HRESULT TestEnumerator(IAAFSequence*	pSequence)
{
	IEnumAAFComponents*	pCompIter = NULL;
	IEnumAAFComponents*	pCompCloneIter = NULL;
	IAAFComponent*		pComp = NULL;
#if 0
  IAAFComponent*		pCompArray[kNumComponents] = {0};
	aafUInt32			numFetched, i;
#endif
	HRESULT				hr = S_OK;
	aafUInt32			numCpnts;

	pSequence->CountComponents(&numCpnts);
	if (numCpnts != kNumComponents)
		return AAFRESULT_TEST_FAILED;

	hr = pSequence->GetComponents(&pCompIter);
	if (FAILED(hr))
		return AAFRESULT_TEST_FAILED;

	// Test the NextOne method
	// Indirectly tests the Reset method.
	numCpnts = 0;
	while (pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
	{
		numCpnts++;
		pComp->Release();
    pComp = NULL;
	}

	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Try to Skip past the end of the sequence.  This
	// test should FAIL.
	pCompIter->Reset();
	hr = pCompIter->Skip(kNumComponents);
	if (SUCCEEDED(hr))
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

#if 0
	// Test the Next method with count == 1
	// Indirectly tests the Reset method.
	pCompIter->Reset();
	numCpnts = 0;
	while (pCompIter->Next(1, &pComp, NULL) == AAFRESULT_SUCCESS)
	{
		numCpnts++;
		pComp->Release();
    pComp = NULL;
	}

	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Test the Next method with count == kNumComponents
	// Indirectly tests the Reset method.
	pCompIter->Reset();
	numCpnts = 0;
	numFetched = 0;
	while (pCompIter->Next(kNumComponents, (IAAFComponent**)&pCompArray, &numFetched) == AAFRESULT_SUCCESS)
	{
		numCpnts += numFetched;

		for (i = 0; i < numFetched; i++)
			(pCompArray[i])->Release();
	}

	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}
#endif

	// Test the Clone method with with enumerator at begining
	// Indirectly tests the Reset method.
	pCompIter->Reset();
	hr = pCompIter->Clone(&pCompCloneIter);
	if (FAILED(hr))
		goto Cleanup;

	numCpnts = 0;
	while (pCompCloneIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
	{
		numCpnts++;
		pComp->Release();
    pComp = NULL;
	}
	pCompCloneIter->Release();
  pCompCloneIter = NULL;
	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Test the Clone method with with enumerator at end.
	// Indirectly tests the Skip and Reset methods.
	pCompIter->Reset();
	pCompIter->Skip(kNumComponents-1);
	hr = pCompIter->Clone(&pCompCloneIter);
	if (FAILED(hr))
		goto Cleanup;

	numCpnts = 0;
	while (pCompCloneIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
	{
		numCpnts++;
		pComp->Release();
    pComp = NULL;
	}
	pCompCloneIter->Release();
  pCompCloneIter = NULL;
	if (numCpnts != 1)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Test the Clone method with with enumerator in the middle.
	// Indirectly tests the Skip and Reset methods.
	pCompIter->Reset();
	pCompIter->Skip(kNumComponents-2);
	hr = pCompIter->Clone(&pCompCloneIter);
	if (FAILED(hr))
		goto Cleanup;

	pCompCloneIter->Skip(1);
	numCpnts = 0;
	while (pCompCloneIter->NextOne(&pComp) == AAFRESULT_SUCCESS)
	{
		numCpnts++;
		pComp->Release();
    pComp = NULL;
	}
	pCompCloneIter->Release();
  pCompCloneIter = NULL;
	if (numCpnts != 1)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

Cleanup:
	pCompIter->Release();

	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFSequence			*pSequence = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;


	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFComponents Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	try 
    {
	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);


	  // Create the file.
	  checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
	  bFileOpen = true;
  
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the number of mobs to force creation of the content storage.
	  // This is temporary as the content storage should be created by
	  // the call to OpenNewModify above.
	  aafNumSlots_t n;
	  checkResult(pHeader->CountMobs(kAAFAllMob, &n));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
 
	  // Create a sequence withou attaching it to the file.
	  checkResult(CreateAAFSequence(pDictionary, &pSequence));
    
	  // Test the enumeration methods.
	  checkResult(TestEnumerator(pSequence));
	}
	catch (HRESULT& rResult)
	{
	  hr = rResult;
	}


	// Cleanup and return
	if (pSequence)
	  pSequence->Release();

	if (pDictionary)
	  pDictionary->Release();

	if (pHeader)
	  pHeader->Release();
			
	if (pFile)
	  {	// Close file
		if (bFileOpen)
		  {
			pFile->Save();
			pFile->Close();
		  }
 		pFile->Release();
	  }

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *				pHeader = NULL;
  aafProductIdentification_t	ProductInfo;
  HRESULT						hr = S_OK;


  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"EnumAAFComponents Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;
	  
  try
  {
    // Open the file
    checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


	// Cleanup and return
  if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	return hr;
}

extern "C" HRESULT CEnumAAFComponents_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
 	aafWChar * pFileName = L"EnumAAFComponentsTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CEnumAAFComponents_test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}


  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
  return hr;
}












