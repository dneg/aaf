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
  	IAAFComponent*		pCompArray[kNumComponents] = {0};
	aafUInt32			numFetched, i;
	HRESULT				hr = S_OK,
						localhr = S_OK;
	aafUInt32			numCpnts;

	pSequence->CountComponents(&numCpnts);
	if (numCpnts != kNumComponents)
		return AAFRESULT_TEST_FAILED;

	hr = pSequence->GetComponents(&pCompIter);
	if (FAILED(hr))
		return AAFRESULT_TEST_FAILED;


/* Test the Reset method *******************************/
	if (pCompIter->Reset() == AAFRESULT_SUCCESS)
		cout<< "	Reset() ...		Passed" << endl;	
	else	{
		cout<< "	Reset() ...		Failed!!!" << endl;	
		hr = AAFRESULT_TEST_FAILED;
	}

/* Test the NextOne method ******************************/

	// Call NextOne once for each mob for a total of numCpnts times	
	for (i=0; i<numCpnts; i++)	{
		if (pCompIter->NextOne(&pComp) == AAFRESULT_SUCCESS)	{
			pComp->Release();
			pComp = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
	}			
	
	// Make sure we are at the end
	if (pCompIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
	
	pCompIter->Reset();
	// this should return AAFRESULT_NULL_PARAM
	if (pCompIter->NextOne(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

	if (SUCCEEDED(localhr))
		cout<< "	NextOne() ...	Passed" << endl;	
	else	{
		cout<< "	NextOne() ...	Failed!!!" << endl;	
		hr = AAFRESULT_TEST_FAILED;
	}
		  
/* Test the Skip method ******************************/

	localhr = S_OK;
	pCompIter->Reset();

	// Try Skipping 0 objects			
	if (pCompIter->Skip(0) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

	// skip over each Component one at a time.
	for (i=0; i<numCpnts; i++)
		if (pCompIter->Skip(1) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

	// Make sure we are at the end.
	if (pCompIter->Skip(1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

	pCompIter->Reset();
	// Skip over multiple Components at a time.		
	for (i=2; i<=numCpnts; i++)	{
		if (pCompIter->Skip(i) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		pCompIter->Reset();
	}

	// Make sure we can't skip past the end.
	if (pCompIter->Skip(i+1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

	if (SUCCEEDED(localhr))
		cout<< "	Skip() ...		Passed" << endl;
	else	{
		cout<< "	Skip() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}
		  
/* Next()  ******************************************/
	localhr = S_OK;
	pCompIter->Reset();
	numFetched = 1;

	// Make sure a count of 0 succeeds
	if (pCompIter->Next(0, &pComp, &numFetched) != AAFRESULT_SUCCESS)
		localhr = AAFRESULT_TEST_FAILED;
	
	if (0 != numFetched)
		localhr = AAFRESULT_TEST_FAILED;


	// Iterate thru the Components using Next doing 1 at a time
	pCompIter->Reset();
	for ( i=0; i<numCpnts ;i++)	{
		if (pCompIter->Next(1, &pComp, &numFetched) == AAFRESULT_SUCCESS)	{
			pComp->Release();
	 	    pComp = NULL;

			if (1 != numFetched)
				localhr = AAFRESULT_TEST_FAILED;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
	}

	// Make sure we are at the end
	if (pCompIter->Next(1, &pComp, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
		localhr = AAFRESULT_TEST_FAILED;
					
	// Test the Next method filling out an array of Components
	numFetched = 0;
	pCompIter->Reset();
	for ( i=2; i<=numCpnts ;i++)	{
		if (pCompIter->Next(i, pCompArray, &numFetched) == AAFRESULT_SUCCESS)	{
			if (i != numFetched)
				localhr = AAFRESULT_TEST_FAILED;

			for (i = 0; i < numFetched; i++)
				if (pCompArray[i] != NULL)	// should have been set
				{
					pCompArray[i]->Release();
					pCompArray[i] = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
			
		pCompIter->Reset();
	}

	// Make sure we can't get more Mobs than numCpnts	
	if (pCompIter->Next(i+1, pCompArray, &numFetched) != AAFRESULT_SUCCESS)
		localhr = AAFRESULT_TEST_FAILED;

	if (numCpnts != numFetched)
		localhr = AAFRESULT_TEST_FAILED;
		
	for (i = 0; i < numCpnts; i++)
		if (pCompArray[i] != NULL)	
		{
			pCompArray[i]->Release();
			pCompArray[i] = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
		
	
	pCompIter->Reset();
	pCompIter->Skip(2);

	// Make sure we can't go past the end to fill the array
	if (pCompIter->Next(numCpnts, pCompArray, &numFetched) != AAFRESULT_SUCCESS)
		localhr = AAFRESULT_TEST_FAILED;

	if ((numCpnts-2) != numFetched)
		localhr = AAFRESULT_TEST_FAILED;
		
	for (i = 0; i < numCpnts-2; i++)
		if (pCompArray[i] != NULL)
		{
			pCompArray[i]->Release();
			pCompArray[i] = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;

	pCompIter->Reset();
	// Make sure it returns AAFRESULT_NULL_PARAM
	if (pCompIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
		localhr = AAFRESULT_TEST_FAILED;

	// Make sure it returns AAFRESULT_NULL_PARAM	
	if (pCompIter->Next(1, pCompArray, NULL) != AAFRESULT_NULL_PARAM)
		localhr = AAFRESULT_TEST_FAILED;

	if (SUCCEEDED(localhr))
		cout<< "	Next() ...		Passed" << endl;
	else	{
		cout<< "	Next() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}


/* Clone() ************************************/

	// Test the Clone method with with enumerator at begining
	localhr = S_OK;
	pCompIter->Reset();
	if (pCompIter->Clone(&pCompCloneIter) == AAFRESULT_SUCCESS)	{
		for (i=0; i < numCpnts; i++)	
			if (pCompCloneIter->NextOne(&pComp) == AAFRESULT_SUCCESS)	{
				pComp->Release();
    			pComp = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;		

		if (pCompCloneIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		pCompCloneIter->Reset();
		if (pCompCloneIter->Next(numCpnts, pCompArray, &numFetched) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		if (numCpnts != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numCpnts; i++)
			if (pCompArray[i] != NULL)	{
				pCompArray[i]->Release();
				pCompArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;

		pCompCloneIter->Reset();

		if (pCompCloneIter->Next(numCpnts+1, pCompArray, &numFetched) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		if (numCpnts != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numCpnts; i++)
			if (pCompArray[i] != NULL)	{
				pCompArray[i]->Release();
				pCompArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;

		pCompCloneIter->Reset();
		pCompCloneIter->Skip(1);

		if (pCompCloneIter->Next(numCpnts, pCompArray, &numFetched) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		if ((numCpnts-1) != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numCpnts-1; i++)
			if (pCompArray[i] != NULL)	{
				pCompArray[i]->Release();
				pCompArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
	
		pCompCloneIter->Release();
	 	pCompCloneIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;
	
	// Test the Clone method with with enumerator at end.
	// Indirectly tests the Skip and Reset methods.
	pCompIter->Reset();
	pCompIter->Skip(numCpnts-1);
	if (pCompIter->Clone(&pCompCloneIter) == AAFRESULT_SUCCESS) {
		if (pCompCloneIter->NextOne(&pComp) == AAFRESULT_SUCCESS)	{
			pComp->Release();
		    pComp = NULL;
		}
		if (pCompCloneIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		pCompCloneIter->Release();
  		pCompCloneIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;

	// Test the Clone method with with enumerator in the middle.
	// Indirectly tests the Skip and Reset methods.
	pCompIter->Reset();
	pCompIter->Skip(numCpnts-2);
	if (pCompIter->Clone(&pCompCloneIter) == AAFRESULT_SUCCESS)	{
		pCompCloneIter->Skip(1);
		if (pCompCloneIter->NextOne(&pComp) == AAFRESULT_SUCCESS)	{
			pComp->Release();
		    pComp = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
		
		if (pCompCloneIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		pCompCloneIter->Release();
  		pCompCloneIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;


	pCompIter->Reset();
	if (pCompIter->Clone(&pCompCloneIter) == AAFRESULT_SUCCESS)	{
		if (pCompCloneIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;
	
		if (pCompCloneIter->Next(1, pCompArray, NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

		pCompCloneIter->Release();
 		pCompCloneIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;

	
	if (SUCCEEDED(localhr))
		cout<< "	Clone() ...		Passed" << endl;
	else	{
		cout<< "	Clone() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}

	if (pComp)
		pComp->Release();

	if (pCompIter)
		pCompIter->Release();

	if (pCompCloneIter)
		pCompCloneIter->Release();

	for (numCpnts=0; numCpnts < kNumComponents; ++numCpnts)
		if (pCompArray[numCpnts])
			pCompArray[numCpnts]->Release();

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
	HRESULT hr = AAFRESULT_SUCCESS;
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

  return hr;
}












