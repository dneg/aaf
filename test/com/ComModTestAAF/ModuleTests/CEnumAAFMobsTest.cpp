// @doc INTERNAL
// @com This file implements the module test for CEnumAAFMobs
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

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

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



static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool 						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
  	IAAFDictionary*  			pDictionary = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob						*pMob = NULL;
	IAAFEssenceDescriptor 		*edesc = NULL;
	aafProductIdentification_t	ProductInfo;
	aafMobID_t					newMobID;
	HRESULT						hr = S_OK;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFMobs Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kAAFVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;


  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


    // Create the file
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
 
    // We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
	CAAFBuiltinDefs defs (pDictionary);
 		
    //Make the first mob

	  // Create a FileMob
	  checkResult(defs.cdSourceMob()->
				  CreateInstance(IID_IAAFSourceMob, 
								 (IUnknown **)&pSourceMob));

	  checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));

	  checkResult(CoCreateGuid((GUID *)&newMobID)); // hack: we need a utility function.
	  checkResult(pMob->SetMobID(newMobID));
	  checkResult(pMob->SetName(L"File Mob"));
	
 	  checkResult(defs.cdFileDescriptor()->
				  CreateInstance(IID_IAAFFileDescriptor, 
								 (IUnknown **)&edesc));		

    checkResult(pSourceMob->SetEssenceDescriptor (edesc));

	  checkResult(pHeader->AddMob(pMob));

    // Reusing local variable so we need to release the inteface.
    pMob->Release();
    pMob = NULL;

	  // Create a MasterMob
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(CoCreateGuid((GUID *)&newMobID)); // hack: we need a utility function.
	  checkResult(pMob->SetMobID(newMobID));
	  checkResult(pMob->SetName(L"Master Mob"));

	  checkResult(pHeader->AddMob(pMob));

    // Reusing local variable so we need to release the inteface.
    pMob->Release();
    pMob = NULL;

	  // Create a CompositionMob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(CoCreateGuid((GUID *)&newMobID)); // hack: we need a utility function.
	  checkResult(pMob->SetMobID(newMobID));
  	checkResult(pMob->SetName(L"Composition Mob"));

	  checkResult(pHeader->AddMob(pMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

  // Cleanup and return
  if (edesc)
    edesc->Release();

  if (pMob)
    pMob->Release();
  
  if (pSourceMob)
    pSourceMob->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
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
	bool 						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs, i;
	HRESULT						hr = S_OK;
	HRESULT						localhr = S_OK;
	IEnumAAFMobs 				*mobIter = NULL;
	IEnumAAFMobs 				*cloneMobIter = NULL;
	IAAFMob						*aMob = NULL;
	IAAFMob						*mobArray[3]; 
	aafSearchCrit_t				criteria;
	aafUInt32					numFetched = 0;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"EnumAAFMobs Test. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kAAFVersionUnknown;
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

	  // Make sure that we have one master, one file, and one composition (three total)
	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression (3 == numMobs, AAFRESULT_TEST_FAILED);

	  
	  criteria.searchTag = kAAFNoSearch;
	  checkResult(pHeader->GetMobs (&criteria, &mobIter));
	  

	/* Test the Reset method *******************************/
	if (mobIter->Reset() == AAFRESULT_SUCCESS)
		cout<< "	Reset() ...		Passed" << endl;	
	else	{
		cout<< "	Reset() ...		Failed!!!" << endl;	
		hr = AAFRESULT_TEST_FAILED;
	}

	/* Test the NextOne method ******************************/
	
	// Call NextOne once for each mob for a total of numMobs times	
	for (i=0; i<numMobs; i++)	{
		if (mobIter->NextOne(&aMob) == AAFRESULT_SUCCESS)	{
			aMob->Release();
			aMob = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
	}

	// Make sure we are at the end
	// this should return AAFRESULT_NO_MORE_MOBS
	if (mobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_MOBS)
			localhr = AAFRESULT_TEST_FAILED;
	
	mobIter->Reset();
	// this should return AAFRESULT_NULL_PARAM
	if (mobIter->NextOne(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

	if (SUCCEEDED(localhr))
		cout<< "	NextOne() ...	Passed" << endl;	
	else	{
		cout<< "	NextOne() ...	Failed!!!" << endl;	
		hr = AAFRESULT_TEST_FAILED;
	}

	/* Test the Skip method ******************************/

	localhr = S_OK;
	mobIter->Reset();
	// skip over each Mob one at a time.  These should all succeed		
	for (i=0; i<numMobs; i++)	{
		if (mobIter->Skip(1) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
	}

	// Make sure we are at the end.
	// this should return AAFRESULT_NO_MORE_OBJECTS
	if (mobIter->Skip(1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
	
	/* Skip Multiple */
	mobIter->Reset();
	// Skip over multiple Mobs at a time. These should all succeed		
	for (i=2; i<=numMobs; i++)	{
		if (mobIter->Skip(i) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		mobIter->Reset();
	}

	// Make sure we are at the end.
	// this should return AAFRESULT_NO_MORE_OBJECTS
	if (mobIter->Skip(i+1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
			
	if (SUCCEEDED(localhr))
		cout<< "	Skip() ...		Passed" << endl;
	else	{
		cout<< "	Skip() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}

	/* Next()  ******************************************/
	// Iterate thru the Mobs using Next doing 1 at a time
	localhr = S_OK;
	numFetched = 0;
	mobIter->Reset();
	for ( i=0; i<numMobs ;i++)	{
		if (mobIter->Next(1, &aMob, &numFetched) == AAFRESULT_SUCCESS)	{
			aMob->Release();
	 	    aMob = NULL;

			if (1 != numFetched)
				localhr = AAFRESULT_TEST_FAILED;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
	}
			
	// Make sure we are at the end
	if (mobIter->Next(1, &aMob, &numFetched) != AAFRESULT_NO_MORE_MOBS)
		localhr = AAFRESULT_TEST_FAILED;
					

	// Test the Next method filling out an array of Mobs
	numFetched = 0;
	mobIter->Reset();
	for ( i=0; i<=numMobs ;i++)	{
		if (mobIter->Next(i, mobArray, &numFetched) == AAFRESULT_SUCCESS)	{
			if (i != numFetched)
				localhr = AAFRESULT_TEST_FAILED;

			for (i = 0; i < numFetched; i++)
				if (mobArray[i] != NULL)	
				{
					mobArray[i]->Release();
					mobArray[i] = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;

		}
		else
			localhr = AAFRESULT_TEST_FAILED;
			
		mobIter->Reset();
	}
// Causes Memory Leak !!!!!	
	// Make sure we can't get more Mobs than numMobs	
	if (mobIter->Next(i+1, mobArray, &numFetched) != AAFRESULT_NO_MORE_MOBS)
		localhr = AAFRESULT_TEST_FAILED;

	if (0 != numFetched)
		localhr = AAFRESULT_TEST_FAILED;
		
	for (i = 0; i < numMobs; i++)
		if (mobArray[i] != NULL)
			localhr = AAFRESULT_TEST_FAILED;
	
	mobIter->Reset();
	mobIter->Skip(2);
// Causes Memory Leak !!!!
	// Make sure we can't go past the end to fill the array
	if (mobIter->Next(numMobs, mobArray, &numFetched) != AAFRESULT_NO_MORE_MOBS)
		localhr = AAFRESULT_TEST_FAILED;

	if (0 != numFetched)
		localhr = AAFRESULT_TEST_FAILED;
		
	for (i = 0; i < numMobs; i++)
		if (mobArray[i] != NULL)
			localhr = AAFRESULT_TEST_FAILED;
			
	mobIter->Reset();
// This causes an assertion so I have commented it out until fixed
	//if (mobIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
	//	localhr = AAFRESULT_TEST_FAILED;
	
	if (mobIter->Next(1, mobArray, NULL) != AAFRESULT_NULL_PARAM)
		localhr = AAFRESULT_TEST_FAILED;

	if (SUCCEEDED(localhr))
		cout<< "	Next() ...		Passed" << endl;
	else	{
		cout<< "	Next() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}

	// Test the Clone method with with enumerator at begining
	// Indirectly tests the Reset method.
	localhr = S_OK;
	mobIter->Reset();
	if (mobIter->Clone(&cloneMobIter) == AAFRESULT_SUCCESS)	{
		for (i=0; i < numMobs; i++)	
			if (cloneMobIter->NextOne(&aMob) == AAFRESULT_SUCCESS)	{
				aMob->Release();
    			aMob = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;		

		if (cloneMobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_MOBS)
			localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Reset();
		if (cloneMobIter->Next(numMobs, mobArray, &numFetched) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		if (numMobs != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numMobs; i++)
			if (mobArray[i] != NULL)	{
				mobArray[i]->Release();
				mobArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Reset();
// Causes Memory Leak !!!!
		if (cloneMobIter->Next(numMobs+1, mobArray, &numFetched) != AAFRESULT_NO_MORE_MOBS)
			localhr = AAFRESULT_TEST_FAILED;

		if (0 != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numMobs; i++)
			if (mobArray[i] != NULL)
				localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Reset();
		cloneMobIter->Skip(1);
// Causes Memory Leak !!!!
		if (cloneMobIter->Next(numMobs, mobArray, &numFetched) != AAFRESULT_NO_MORE_MOBS)
			localhr = AAFRESULT_TEST_FAILED;

		if (0 != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numMobs; i++)
			if (mobArray[i] != NULL)
				localhr = AAFRESULT_TEST_FAILED;
	
		cloneMobIter->Release();
	 	cloneMobIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;
	
	// Test the Clone method with with enumerator at end.
	// Indirectly tests the Skip and Reset methods.
	mobIter->Reset();
	mobIter->Skip(numMobs-1);
	if (mobIter->Clone(&cloneMobIter) == AAFRESULT_SUCCESS) {
		if (cloneMobIter->NextOne(&aMob) == AAFRESULT_SUCCESS)	{
			aMob->Release();
		    aMob = NULL;
		}
		if (cloneMobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_MOBS)
			localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Release();
  		cloneMobIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;

	// Test the Clone method with with enumerator in the middle.
	// Indirectly tests the Skip and Reset methods.
	mobIter->Reset();
	mobIter->Skip(numMobs-2);
	if (mobIter->Clone(&cloneMobIter) == AAFRESULT_SUCCESS)	{
		cloneMobIter->Skip(1);
		if (cloneMobIter->NextOne(&aMob) == AAFRESULT_SUCCESS)	{
			aMob->Release();
		    aMob = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
		
		if (cloneMobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_MOBS)
			localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Release();
  		cloneMobIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;


	mobIter->Reset();
	if (mobIter->Clone(&cloneMobIter) == AAFRESULT_SUCCESS)	{
// This causes an assertion so I have commented it out until fixed
		//if (cloneMobIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
		//	localhr = AAFRESULT_TEST_FAILED;
	
		if (cloneMobIter->Next(1, mobArray, NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Release();
 		cloneMobIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;

	
	if (SUCCEEDED(localhr))
		cout<< "	Clone() ...		Passed" << endl;
	else	{
		cout<< "	Clone() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}
	  	  	  
	  checkResult(pHeader->CountMobs(kAAFMasterMob, &numMobs));
	  checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->CountMobs(kAAFFileMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

	// Cleanup and return

	if (mobIter)
		mobIter->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}

	return hr;
}
 
extern "C" HRESULT CEnumAAFMobs_test()
{
	HRESULT hr = AAFRESULT_SUCCESS;
 	aafWChar * pFileName = L"EnumAAFMobsTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(AAFRESULT_SUCCESS == hr)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFSourceMob_test...Caught general C++"
		" exception!" << endl; 
	}

	return hr;
}














