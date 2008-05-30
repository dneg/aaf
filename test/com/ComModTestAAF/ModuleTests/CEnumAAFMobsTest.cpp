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
// The Original Code of this file is Copyright 1998-2008, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFExtEnum.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_File_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x6ee8dbba, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const 	aafMobID_t	TEST_Master_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x74fdbb4c, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static const 	aafMobID_t	TEST_Composition_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x7ad23b60, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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



static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool 						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
  	IAAFDictionary*  			pDictionary = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob						*pMob = NULL;
	IAAFEssenceDescriptor 		*edesc = NULL;
	HRESULT						hr = S_OK;


  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


    // Create the file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
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

	  checkResult(pMob->SetMobID(TEST_File_MobID));
	  checkResult(pMob->SetName(L"File Mob"));

	  // Check the Mob2 usage code implementations.
	  // Need IAAFMob2 for to do that.
	  {
	    IAAFSmartPointer<IAAFMob2> pMobInterface2;
	    checkResult( pMob->QueryInterface( IID_IAAFMob2, reinterpret_cast<void**>(&pMobInterface2) ) );
	    checkResult( pMobInterface2->SetUsageCode( kAAFUsage_Template ) );
	  }

	  // Create a concrete subclass of FileDescriptor
 	  checkResult(defs.cdAIFCDescriptor()->
				  CreateInstance(IID_IAAFEssenceDescriptor, 
								 (IUnknown **)&edesc));		


		IAAFAIFCDescriptor*			pAIFCDesc = NULL;
		checkResult(edesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
		pAIFCDesc->Release();
		pAIFCDesc = NULL;

    checkResult(pSourceMob->SetEssenceDescriptor (edesc));

	  checkResult(pHeader->AddMob(pMob));

    // Reusing local variable so we need to release the inteface.
    pMob->Release();
    pMob = NULL;

	  // Create a MasterMob
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(pMob->SetMobID(TEST_Master_MobID));
	  checkResult(pMob->SetName(L"Master Mob"));

	  // Check the Mob2 usage code implementations.
	  // Need IAAFMob2 for to do that.
	  {
	    IAAFSmartPointer<IAAFMob2> pMobInterface2;
	    checkResult( pMob->QueryInterface( IID_IAAFMob2, reinterpret_cast<void**>(&pMobInterface2) ) );
	    checkResult( pMobInterface2->SetUsageCode( kAAFUsage_Template ) );
	  }

	  checkResult(pHeader->AddMob(pMob));

    // Reusing local variable so we need to release the inteface.
    pMob->Release();
    pMob = NULL;

	  // Create a CompositionMob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(pMob->SetMobID(TEST_Composition_MobID));
  	  checkResult(pMob->SetName(L"Composition Mob"));

	  // Check the Mob2 usage code implementations.
	  // Need IAAFMob2 for to do that.
	  IAAFSmartPointer<IAAFMob2> pMobInterface2;
	  checkResult( pMob->QueryInterface( IID_IAAFMob2, reinterpret_cast<void**>(&pMobInterface2) ) );
	  checkResult( pMobInterface2->SetUsageCode( kAAFUsage_TopLevel ) );

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



static void CheckMobsByUsageCode( IAAFHeader* pHeader,
				  aafSearchCrit_t& usageCrit,
				  int expectedCount,
				  bool checkIID,
				  const IID& expectedIID )
{
  IAAFSmartPointer<IEnumAAFMobs> pEnumByUsage;
  checkResult( pHeader->GetMobs( &usageCrit, &pEnumByUsage ) );

  // Should get one back, and it should be a composition mob.
  IAAFSmartPointer<IAAFMob> pNextMob;
  HRESULT nextHr;
  int count;
  for( count = 0, nextHr = pEnumByUsage->NextOne( &pNextMob );
       AAFRESULT_SUCCESS == nextHr;
       count++, nextHr = pEnumByUsage->NextOne( &pNextMob ) ) {
    if ( checkIID ) {
      // Quering for one interface but passing a pointer to another
      // interface is not a right thing to do but since it's a pointer
      // to IUnknown, the parent of them all, and all we do is call
      // Release() on it, it's OK.
      IAAFSmartPointer<IUnknown> pUnused;
      checkResult( pNextMob->QueryInterface( expectedIID, (void**)&pUnused ) );
    }
  }
    
  checkExpression( AAFRESULT_NO_MORE_OBJECTS == nextHr,
		   AAFRESULT_TEST_FAILED );
  
  checkExpression( expectedCount == count, AAFRESULT_TEST_FAILED );
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool 						bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	aafNumSlots_t				numMobs, i;
	HRESULT						hr = S_OK;
	HRESULT						localhr = S_OK;
	IEnumAAFMobs 				*mobIter = NULL;
	IEnumAAFMobs 				*cloneMobIter = NULL;
	IAAFMob						*aMob = NULL;
	IAAFMob						**mobArray = NULL;
	aafSearchCrit_t				criteria;
	aafUInt32					numFetched = 0;


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
	if (mobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_OBJECTS)
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
	
	// skip over each Mob one at a time.
	for (i=0; i<numMobs; i++)
		if (mobIter->Skip(1) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

	// Make sure we are at the end.
	if (mobIter->Skip(1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
	
	mobIter->Reset();
	// Skip over multiple Mobs at a time.		
	for (i=2; i<=numMobs; i++)	{
		if (mobIter->Skip(i) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		mobIter->Reset();
	}

	// Make sure we are at the end.
	if (mobIter->Skip(numMobs+1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
			
	if (SUCCEEDED(localhr))
		cout<< "	Skip() ...		Passed" << endl;
	else	{
		cout<< "	Skip() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}

/* Next()  ******************************************/

	mobArray = new IAAFMob *[numMobs];

	localhr = S_OK;
	numFetched = 1;

	// Iterate thru the Mobs using Next doing 1 at a time
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
	if (mobIter->Next(1, &aMob, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
		localhr = AAFRESULT_TEST_FAILED;
	if(numFetched != 0)
		localhr = AAFRESULT_TEST_FAILED;
		
	// Test the Next method filling out an array of Mobs
	numFetched = 0;
	mobIter->Reset();
	for ( i=2; i<=numMobs ;i++)	{
		if (mobIter->Next(i, mobArray, &numFetched) == AAFRESULT_SUCCESS)	{
			if (i != numFetched)
				localhr = AAFRESULT_TEST_FAILED;

			for (i = 0; i < numFetched; i++)
				if (mobArray[i] != NULL)	// should have been set
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

	// Make sure we can't get more Mobs than numMobs	
	if (mobIter->Next(i+1, mobArray, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
		localhr = AAFRESULT_TEST_FAILED;

	if (numMobs != numFetched)
		localhr = AAFRESULT_TEST_FAILED;
		
	for (i = 0; i < numMobs; i++)
		if (mobArray[i] != NULL)	
		{
			mobArray[i]->Release();
			mobArray[i] = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
		
	
	mobIter->Reset();
	mobIter->Skip(2);

	// Make sure we can't go past the end to fill the array
	if (mobIter->Next(numMobs, mobArray, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
		localhr = AAFRESULT_TEST_FAILED;

	if ((numMobs-2) != numFetched)
		localhr = AAFRESULT_TEST_FAILED;
		
	for (i = 0; i < numMobs-2; i++)
		if (mobArray[i] != NULL)
		{
			mobArray[i]->Release();
			mobArray[i] = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;
			
	mobIter->Reset();
	// Make sure it returns AAFRESULT_NULL_PARAM
	if (mobIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
		localhr = AAFRESULT_TEST_FAILED;

	// Make sure it returns E_INVALIDARG	
	if (mobIter->Next(1, mobArray, &numFetched) != AAFRESULT_SUCCESS)
		localhr = AAFRESULT_TEST_FAILED;
	else
	{
		for (i = 0; i < numFetched; i++)
		if (mobArray[i] != NULL)
		{
			mobArray[i]->Release();
			mobArray[i] = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;		
	}

	if (SUCCEEDED(localhr))
		cout<< "	Next() ...		Passed" << endl;
	else	{
		cout<< "	Next() ...		Failed!!!" << endl;
		hr = AAFRESULT_TEST_FAILED;
	}


/* Clone() ************************************/

	// Test the Clone method with with enumerator at begining
	localhr = S_OK;
	mobIter->Reset();
	if (mobIter->Clone(&cloneMobIter) == AAFRESULT_SUCCESS)	{
		for (i=0; i < numMobs; i++)	{
			if (cloneMobIter->NextOne(&aMob) == AAFRESULT_SUCCESS)	{
				aMob->Release();
    			aMob = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;		
		}

		if (cloneMobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Reset();
		if (cloneMobIter->Next(numMobs, mobArray, &numFetched) 
			!= AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		if (numMobs != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numMobs; i++) {
			if (mobArray[i] != NULL)	{
				mobArray[i]->Release();
				mobArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
		}

		cloneMobIter->Reset();

		if (cloneMobIter->Next(numMobs+1, mobArray, &numFetched) 
			!= AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		if (numMobs != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numMobs; i++) {
			if (mobArray[i] != NULL)	{
				mobArray[i]->Release();
				mobArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
		}

		cloneMobIter->Reset();
		cloneMobIter->Skip(1);

		if (cloneMobIter->Next(numMobs, mobArray, &numFetched) 
			!= AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		if ((numMobs-1) != numFetched)
			localhr = AAFRESULT_TEST_FAILED;
		
		for (i = 0; i < numMobs-1; i++) {
			if (mobArray[i] != NULL)	{
				mobArray[i]->Release();
				mobArray[i] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
		}
	
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
		if (cloneMobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_OBJECTS)
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
		
		if (cloneMobIter->NextOne(&aMob) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		cloneMobIter->Release();
  		cloneMobIter = NULL;
	}
	else
		localhr = AAFRESULT_TEST_FAILED;


	mobIter->Reset();
	if (mobIter->Clone(&cloneMobIter) == AAFRESULT_SUCCESS)	{
		if (cloneMobIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;
	
		if (cloneMobIter->Next(1, mobArray, &numFetched) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		else
		{
			for (i = 0; i < numFetched; i++) {
				if (mobArray[i] != NULL)
				{
					mobArray[i]->Release();
					mobArray[i] = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;		
			}
		}

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


	  // Test usageCode search criteria.
	  {
	    // Should find one TopLevel mob, and two Template mobs.

	    // There should be one mob with usage code TopLevel, and it should be
	    // a composition mob.
	    aafSearchCrit_t usageCrit;
	    usageCrit.searchTag = kAAFByUsageCode;
	    usageCrit.tags.usageCode = kAAFUsage_TopLevel; 
	    CheckMobsByUsageCode( pHeader, usageCrit, 1, true, IID_IAAFCompositionMob );

	    // There should be one composition mob with usage code TopLevel.
	    usageCrit.searchTag = kAAFByCompositionMobUsageCode;
	    usageCrit.tags.usageCode = kAAFUsage_TopLevel; 
	    CheckMobsByUsageCode( pHeader, usageCrit, 1, true, IID_IAAFCompositionMob );

	    // There should be two mobs with usage code Template.  They are of difference
	    // types, hence, don't check the type.
	    usageCrit.searchTag = kAAFByUsageCode;
	    usageCrit.tags.usageCode = kAAFUsage_Template; 
	    CheckMobsByUsageCode( pHeader, usageCrit, 2, false, IID_IUnknown );

	    // One of the Template mobs should be a SourceMob, the other should be MasterMob.
	    usageCrit.searchTag = kAAFBySourceMobUsageCode;
	    usageCrit.tags.usageCode = kAAFUsage_Template; 
	    CheckMobsByUsageCode( pHeader, usageCrit, 1, true, IID_IAAFSourceMob );

	    usageCrit.searchTag = kAAFByMasterMobUsageCode;
	    usageCrit.tags.usageCode = kAAFUsage_Template; 
	    CheckMobsByUsageCode( pHeader, usageCrit, 1, true, IID_IAAFMasterMob ); 
	  }

  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	

	// Cleanup and return
	
	delete [] mobArray;

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
 
extern "C" HRESULT CEnumAAFMobs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFMobs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_TEST_PARTIAL_SUCCESS; //AAFRESULT_SUCCESS;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

  try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if(AAFRESULT_SUCCESS == hr)
			hr = ReadAAFFile( pFileName );
		
		if(hr == AAFRESULT_SUCCESS)
			hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
	}
  catch (...)
	{
	  cerr << "CEnumAAFMobs_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}














