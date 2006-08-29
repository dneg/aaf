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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"
#include "ModuleTest.h"

//{060c2b340205110101001000-13-00-00-00-{12bd35d0-996e-11d4-9f7b-080036210804}}
static const aafMobID_t TEST_MobID = {
{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
0x13, 0x00, 0x00, 0x00, 
{0x12bd35d0, 0x996e, 0x11d4, {0x9f, 0x7b, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}};

// Convenient error handlers.
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
	IAAFFile 					*pFile = NULL;
	IAAFHeader 					*pHeader = NULL;
	IAAFDictionary				*pDictionary = NULL;
	IAAFMultipleDescriptor 		*pMDesc = NULL;
	IAAFSourceMob 				*pSourceMob = NULL;
	IAAFMob						*pMob = NULL;
	aafUInt32					i;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the file.
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));

		CAAFBuiltinDefs defs (pDictionary);

		//Make the first mob
		// Create a Mob
		checkResult(defs.cdSourceMob()->
			CreateInstance(IID_IAAFSourceMob, 
			(IUnknown **)&pSourceMob));

		
		// Initialize mob properties:
		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"EnumAAFFileDescriptorsTest"));
		

		IAAFEssenceDescriptor 		*eDesc = NULL;
		checkResult(defs.cdMultipleDescriptor()->
					CreateInstance(IID_IAAFMultipleDescriptor, 
								   (IUnknown **)&pMDesc));		
		checkResult(pMDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&eDesc));
		
		
		checkResult(pSourceMob->SetEssenceDescriptor (eDesc));


/* CountFileDescriptors()	******************************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (pMDesc->CountFileDescriptors(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		
		
		// Verify that there are no FileDescriptors
		aafUInt32	numFileDescriptors = 0;
		if (pMDesc->CountFileDescriptors(&numFileDescriptors) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		if (0 != numFileDescriptors)
			localhr = AAFRESULT_TEST_FAILED;
			
		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	CountFileDescriptors() ...		Passed"<< endl;
		else
		{
			cout<< "	CountFileDescriptors() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}


/* Append 3 File Descriptors ******************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (pMDesc->AppendFileDescriptor(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		

		// Append and Count a bunch of FileDescriptors
		for (i=1; i<=3; i++)
		{
			// Make a concrete subclass of FileDescriptor, and attach it to
			// the MultipleDescriptor
			IAAFFileDescriptor*			pFileDescriptor = NULL;
			checkResult(defs.cdAIFCDescriptor()->
						CreateInstance(IID_IAAFFileDescriptor, 
									   (IUnknown **)&pFileDescriptor));		
			IAAFAIFCDescriptor*			pAIFCDesc = NULL;
			checkResult(pFileDescriptor->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
			checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
			pAIFCDesc->Release();
			pAIFCDesc = NULL;
								   	
			if (pMDesc->AppendFileDescriptor(pFileDescriptor) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;

			// Verify the number of FileDescriptors
			aafUInt32 numFileDescriptors = 0;
			pMDesc->CountFileDescriptors(&numFileDescriptors);
			if (i != numFileDescriptors)
				localhr = AAFRESULT_TEST_FAILED;

			// Verify that FileDescriptor was appended
			IAAFFileDescriptor*			pFileDescriptor2 = NULL;
			pMDesc->GetFileDescriptorAt(i-1, &pFileDescriptor2);
			if (pFileDescriptor2 != pFileDescriptor)
				localhr = AAFRESULT_TEST_FAILED;

			pFileDescriptor->Release();
			pFileDescriptor = NULL;
			pFileDescriptor2->Release();
			pFileDescriptor2 = NULL;
		}

		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	AppendFileDescriptor() ...		Passed"<< endl;
		else
		{
			cout<< "	AppendFileDescriptor() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}


/*************************************************************/

	// Add the source mob into the tree
	checkResult(pHeader->AddMob(pMob));

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}


	if (pMob)
		pMob->Release();

	if (pSourceMob)
		pSourceMob->Release();
	
	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
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
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool						bFileOpen = false;

	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IEnumAAFMobs*				MobTopIter = NULL;
	IAAFMob*					aTopMob = NULL;

	IAAFEssenceDescriptor*		pEdesc = NULL;
	IAAFSourceMob*				pSourceTopMob = NULL;
	IAAFFileDescriptor*			pFileDescriptor = NULL;
	IAAFMultipleDescriptor*		pMDesc = NULL;
	aafNumSlots_t				numTopMobs;

	IEnumAAFFileDescriptors*	FDIter = NULL;
	IAAFFileDescriptor*			aFD = NULL;
	aafNumSlots_t				numFDs, iFD;
	HRESULT						localhr = S_OK;
	IEnumAAFFileDescriptors*	cloneFDIter = NULL;
	IAAFFileDescriptor**		FDArray = NULL;
	aafUInt32					numFetched = 0;


	try
	{	
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numTopMobs));
		checkExpression (1 == numTopMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->GetMobs (NULL, &MobTopIter));
		
		aafWChar		name[500];
		aafMobID_t		mobID;

		checkResult(MobTopIter->NextOne (&aTopMob));
		checkResult(aTopMob->GetName (name, sizeof(name)));
		checkResult(aTopMob->GetMobID (&mobID));

		checkResult(aTopMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceTopMob));
		checkResult(pSourceTopMob->GetEssenceDescriptor (&pEdesc));
		checkResult(pEdesc->QueryInterface (IID_IAAFMultipleDescriptor, (void **)&pMDesc));

		// Verify that there is now one FileDescriptor
		checkResult(pMDesc->CountFileDescriptors(&numFDs));
		checkExpression(3 == numFDs, AAFRESULT_TEST_FAILED);

		// This should read the one real FileDescriptor
		for ( iFD=0; iFD<numFDs; iFD++)
		{
			checkResult(pMDesc->GetFileDescriptorAt(iFD, &pFileDescriptor));

			pFileDescriptor->Release();
			pFileDescriptor = NULL;
		}			

		// cleanup for ppEnum
		checkResult(pMDesc->GetFileDescriptors(&FDIter));

/* Test the Reset method *******************************/
		
		if (FDIter->Reset() == AAFRESULT_SUCCESS)
			cout<< "	Reset() ...		Passed" << endl;	
		else	{
			cout<< "	Reset() ...		Failed!!!" << endl;	
			hr = AAFRESULT_TEST_FAILED;
		}

/* Test the NextOne method ******************************/
	
		// Call NextOne once for each FileDescriptor for a total of numFDs times	
		for (iFD=0; iFD<numFDs; iFD++)	{
			if (FDIter->NextOne(&aFD) == AAFRESULT_SUCCESS)	{
				aFD->Release();
				aFD = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
		}

		// Make sure we are at the end
		if (FDIter->NextOne(&aFD) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
	
		FDIter->Reset();
		// this should return AAFRESULT_NULL_PARAM
		if (FDIter->NextOne(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

		if (SUCCEEDED(localhr))
			cout<< "	NextOne() ...	Passed" << endl;	
		else	{
			cout<< "	NextOne() ...	Failed!!!" << endl;	
			hr = AAFRESULT_TEST_FAILED;
		}

/* Test the Skip method ******************************/

		localhr = S_OK;
		FDIter->Reset();
	
		// skip over each FileDescriptor one at a time.
		for (iFD=0; iFD<numFDs; iFD++)
			if (FDIter->Skip(1) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;

		// Make sure we are at the end.
		if (FDIter->Skip(1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
	
		FDIter->Reset();
		// Skip over multiple FileDescriptors at a time.		
		for (iFD=2; iFD<=numFDs; iFD++)	{
			if (FDIter->Skip(iFD) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;

			FDIter->Reset();
		}

		// Make sure we are at the end.
		if (FDIter->Skip(numFDs+1) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
			
		if (SUCCEEDED(localhr))
			cout<< "	Skip() ...		Passed" << endl;
		else	{
			cout<< "	Skip() ...		Failed!!!" << endl;
			hr = AAFRESULT_TEST_FAILED;
		}

/* Next()  ******************************************/

		FDArray = new IAAFFileDescriptor *[numFDs];

		localhr = S_OK;
		numFetched = 1;

		// Iterate thru the FileDescriptors using Next doing 1 at a time
		FDIter->Reset();
		for ( iFD=0; iFD<numFDs ;iFD++)	{
			if (FDIter->Next(1, &aFD, &numFetched) == AAFRESULT_SUCCESS)	{
				aFD->Release();
				aFD = NULL;

				if (1 != numFetched)
					localhr = AAFRESULT_TEST_FAILED;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;
		}
			
		// Make sure we are at the end
		if (FDIter->Next(1, &aFD, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;
		if(numFetched != 0)
			localhr = AAFRESULT_TEST_FAILED;
		
		// Test the Next method filling out an array of FileDescriptors
		numFetched = 0;
		FDIter->Reset();
		for ( iFD=2; iFD<=numFDs ;iFD++)	{
			if (FDIter->Next(iFD, FDArray, &numFetched) == AAFRESULT_SUCCESS)	{
				if (iFD != numFetched)
					localhr = AAFRESULT_TEST_FAILED;

				for (iFD = 0; iFD < numFetched; iFD++)
					if (FDArray[iFD] != NULL)	// should have been set
					{
						FDArray[iFD]->Release();
						FDArray[iFD] = NULL;
					}
					else
						localhr = AAFRESULT_TEST_FAILED;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;

			FDIter->Reset();
		}

		// Make sure we can't get more FileDescriptors than numFDs	
		if (FDIter->Next(iFD+1, FDArray, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		if (numFDs != numFetched)
			localhr = AAFRESULT_TEST_FAILED;

		for (iFD = 0; iFD < numFDs; iFD++)
			if (FDArray[iFD] != NULL)	
			{
				FDArray[iFD]->Release();
				FDArray[iFD] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;


		FDIter->Reset();
		FDIter->Skip(2);

		// Make sure we can't go past the end to fill the array
		if (FDIter->Next(numFDs, FDArray, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
			localhr = AAFRESULT_TEST_FAILED;

		if ((numFDs-2) != numFetched)
			localhr = AAFRESULT_TEST_FAILED;

		for (iFD = 0; iFD < numFDs-2; iFD++)
			if (FDArray[iFD] != NULL)
			{
				FDArray[iFD]->Release();
				FDArray[iFD] = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;

		FDIter->Reset();
		// Make sure it returns AAFRESULT_NULL_PARAM
		if (FDIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

		// Make sure it returns E_INVALIDARG	
		if (FDIter->Next(1, FDArray, &numFetched) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		else
		{
			for (iFD = 0; iFD < numFetched; iFD++)
				if (FDArray[iFD] != NULL)
				{
					FDArray[iFD]->Release();
					FDArray[iFD] = NULL;
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
		FDIter->Reset();
		if (FDIter->Clone(&cloneFDIter) == AAFRESULT_SUCCESS)	{
			for (iFD=0; iFD < numFDs; iFD++)	{
				if (cloneFDIter->NextOne(&aFD) == AAFRESULT_SUCCESS)	{
					aFD->Release();
					aFD = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;		
			}

			if (cloneFDIter->NextOne(&aFD) != AAFRESULT_NO_MORE_OBJECTS)
				localhr = AAFRESULT_TEST_FAILED;

			cloneFDIter->Reset();
			if (cloneFDIter->Next(numFDs, FDArray, &numFetched) 
				!= AAFRESULT_SUCCESS)
					localhr = AAFRESULT_TEST_FAILED;

			if (numFDs != numFetched)
				localhr = AAFRESULT_TEST_FAILED;

			for (iFD = 0; iFD < numFDs; iFD++) {
				if (FDArray[iFD] != NULL)	{
					FDArray[iFD]->Release();
					FDArray[iFD] = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;
			}

			cloneFDIter->Reset();

			if (cloneFDIter->Next(numFDs+1, FDArray, &numFetched) 
				!= AAFRESULT_NO_MORE_OBJECTS)
					localhr = AAFRESULT_TEST_FAILED;

			if (numFDs != numFetched)
				localhr = AAFRESULT_TEST_FAILED;

			for (iFD = 0; iFD < numFDs; iFD++) {
				if (FDArray[iFD] != NULL)	{
					FDArray[iFD]->Release();
					FDArray[iFD] = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;
			}

			cloneFDIter->Reset();
			cloneFDIter->Skip(1);

			if (cloneFDIter->Next(numFDs, FDArray, &numFetched) 
				!= AAFRESULT_NO_MORE_OBJECTS)
					localhr = AAFRESULT_TEST_FAILED;

			if ((numFDs-1) != numFetched)
				localhr = AAFRESULT_TEST_FAILED;

			for (iFD = 0; iFD < numFDs-1; iFD++) {
				if (FDArray[iFD] != NULL)	{
					FDArray[iFD]->Release();
					FDArray[iFD] = NULL;
				}
				else
					localhr = AAFRESULT_TEST_FAILED;
			}

			cloneFDIter->Release();
			cloneFDIter = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;

		// Test the Clone method with with enumerator at end.
		// Indirectly tests the Skip and Reset methods.
		FDIter->Reset();
		FDIter->Skip(numFDs-1);
		if (FDIter->Clone(&cloneFDIter) == AAFRESULT_SUCCESS) {
			if (cloneFDIter->NextOne(&aFD) == AAFRESULT_SUCCESS)	{
				aFD->Release();
				aFD = NULL;
			}
			if (cloneFDIter->NextOne(&aFD) != AAFRESULT_NO_MORE_OBJECTS)
				localhr = AAFRESULT_TEST_FAILED;

			cloneFDIter->Release();
			cloneFDIter = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;

		// Test the Clone method with with enumerator in the middle.
		// Indirectly tests the Skip and Reset methods.
		FDIter->Reset();
		FDIter->Skip(numFDs-2);
		if (FDIter->Clone(&cloneFDIter) == AAFRESULT_SUCCESS)	{
			cloneFDIter->Skip(1);
			if (cloneFDIter->NextOne(&aFD) == AAFRESULT_SUCCESS)	{
				aFD->Release();
				aFD = NULL;
			}
			else
				localhr = AAFRESULT_TEST_FAILED;

			if (cloneFDIter->NextOne(&aFD) != AAFRESULT_NO_MORE_OBJECTS)
				localhr = AAFRESULT_TEST_FAILED;

			cloneFDIter->Release();
			cloneFDIter = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;


		FDIter->Reset();
		if (FDIter->Clone(&cloneFDIter) == AAFRESULT_SUCCESS)	{
			if (cloneFDIter->Next(1, NULL, &numFetched) != AAFRESULT_NULL_PARAM)
				localhr = AAFRESULT_TEST_FAILED;

			if (cloneFDIter->Next(1, FDArray, &numFetched) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;
			else
			{
				for (iFD = 0; iFD < numFetched; iFD++) {
					if (FDArray[iFD] != NULL)
					{
						FDArray[iFD]->Release();
						FDArray[iFD] = NULL;
					}
					else
						localhr = AAFRESULT_TEST_FAILED;		
				}
			}

			cloneFDIter->Release();
			cloneFDIter = NULL;
		}
		else
			localhr = AAFRESULT_TEST_FAILED;


		if (SUCCEEDED(localhr))
			cout<< "	Clone() ...		Passed" << endl;
		else	{
			cout<< "	Clone() ...		Failed!!!" << endl;
			hr = AAFRESULT_TEST_FAILED;
		}

		checkResult(pMDesc->CountFileDescriptors(&numFDs));
		checkExpression (3 == numFDs, AAFRESULT_TEST_FAILED);

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}

	// Cleanup object references
	if (FDArray)
		delete [] FDArray;

	if (FDIter)
		FDIter->Release();

	if (pFileDescriptor)
		pFileDescriptor->Release();

	if (pEdesc)
		pEdesc->Release();

	if (pSourceTopMob)
		pSourceTopMob->Release();
	
	if (aTopMob)
		aTopMob->Release();

	if (MobTopIter)
		MobTopIter->Release();

	if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	return hr;
}

// Required function prototype.
extern "C" HRESULT CEnumAAFFileDescriptors_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

//
// The public entry for this module test,
//
HRESULT CEnumAAFFileDescriptors_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CEnumAAFFileDescriptors_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
