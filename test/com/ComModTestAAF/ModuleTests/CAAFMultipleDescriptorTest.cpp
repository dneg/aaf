//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
#include <assert.h>
using namespace std;

#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

//{060c2b340205110101001000-13-00-00-00-{12bd35d0-996e-11d4-9f7b-080036210804}}
static const aafMobID_t TEST_MobID = {
{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 
0x13, 0x00, 0x00, 0x00, 
{0x12bd35d0, 0x996e, 0x11d4, {0x9f, 0x7b, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}}};

static
aafProductIdentification_t productInfo;

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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMultipleDescriptor*	pMDesc = NULL;
	IAAFSourceMob*		pSourceMob = NULL;
	IAAFMob*		pMob = NULL;
	aafUInt32		i;
	HRESULT			hr = AAFRESULT_SUCCESS;
	bool			bFileOpen = false;

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);

		// Create the file
		checkResult(AAFFileOpenNewModify(pFileName, 0, &productInfo, &pFile));
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
		checkResult(pMob->SetName(L"EssenceDescriptorTest"));
		

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


/* AppendFileDescriptor()	******************************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (pMDesc->AppendFileDescriptor(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		

		// Append and Count a bunch of FileDescriptors
		for (i=1; i<=10; i++)
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

		// Make sure we can't add it again

//		if (edesc->AppendFileDescriptor(pFileDescriptor) != AAFRESULT_OBJECT_ALREADY_ATTACHED)

//			localhr = AAFRESULT_TEST_FAILED;


		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	AppendFileDescriptor() ...		Passed"<< endl;
		else
		{
			cout<< "	AppendFileDescriptor() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}


/* PrependFileDescriptor()	******************************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (pMDesc->PrependFileDescriptor(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		

		for (; i<=20; i++)
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
								   	
			if (pMDesc->PrependFileDescriptor(pFileDescriptor) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;

			// Verify the number of FileDescriptors
			aafUInt32 numFileDescriptors = 0;
			pMDesc->CountFileDescriptors(&numFileDescriptors);
			if (i != numFileDescriptors)
				localhr = AAFRESULT_TEST_FAILED;
				
			// Verify that FileDescriptor was prepended
			IAAFFileDescriptor*			pFileDescriptor2 = NULL;
			pMDesc->GetFileDescriptorAt(0, &pFileDescriptor2);
			if (pFileDescriptor2 != pFileDescriptor)
				localhr = AAFRESULT_TEST_FAILED;

			pFileDescriptor->Release();
			pFileDescriptor = NULL;
			pFileDescriptor2->Release();
			pFileDescriptor2 = NULL;
		}

		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	PrependFileDescriptor() ...	Passed"<< endl;
		else
		{
			cout<< "	PrependFileDescriptor() ...	FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}


/* InsertFileDescriptorAt()	**************************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;

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

		// Verify that we can't remove an index value that is out of range
		aafUInt32 numFileDescriptors = 0;
		pMDesc->CountFileDescriptors(&numFileDescriptors);
		if (pMDesc->InsertFileDescriptorAt(numFileDescriptors+1, pFileDescriptor) != AAFRESULT_BADINDEX)
			localhr = AAFRESULT_TEST_FAILED;

		// Verify behavior when NULL is passed in
		if (pMDesc->InsertFileDescriptorAt(1, NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

		// Insert it
		if (pMDesc->InsertFileDescriptorAt(0, pFileDescriptor) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		// Check it
		IAAFFileDescriptor*			pFileDescriptor2 = NULL;
		pMDesc->GetFileDescriptorAt(0, &pFileDescriptor2);
		if (pFileDescriptor2 != pFileDescriptor)
			localhr = AAFRESULT_TEST_FAILED;
		// Count it	
		aafUInt32 numFileDescriptors2 = 0;
		pMDesc->CountFileDescriptors(&numFileDescriptors2);
		if (numFileDescriptors2 != numFileDescriptors+1)
			localhr = AAFRESULT_TEST_FAILED;

		pFileDescriptor->Release();
		pFileDescriptor = NULL;
		pFileDescriptor2->Release();
		pFileDescriptor2 = NULL;


		pMDesc->CountFileDescriptors(&numFileDescriptors);
			// Make a concrete subclass of FileDescriptor, and attach it to
			// the MultipleDescriptor
			checkResult(defs.cdAIFCDescriptor()->
						CreateInstance(IID_IAAFFileDescriptor, 
									   (IUnknown **)&pFileDescriptor));	
			/*IAAFAIFCDescriptor*	*/		pAIFCDesc = NULL;
			checkResult(pFileDescriptor->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
			checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
			pAIFCDesc->Release();
			pAIFCDesc = NULL;

		// Insert it
		if (pMDesc->InsertFileDescriptorAt(numFileDescriptors/2, pFileDescriptor) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		// Check it
		pMDesc->GetFileDescriptorAt(numFileDescriptors/2, &pFileDescriptor2);
		if (pFileDescriptor2 != pFileDescriptor)
			localhr = AAFRESULT_TEST_FAILED;
		// Count it	
		pMDesc->CountFileDescriptors(&numFileDescriptors2);
		if (numFileDescriptors2 != numFileDescriptors+1)
			localhr = AAFRESULT_TEST_FAILED;

		pFileDescriptor->Release();
		pFileDescriptor = NULL;
		pFileDescriptor2->Release();
		pFileDescriptor2 = NULL;


		pMDesc->CountFileDescriptors(&numFileDescriptors);
			// Make a concrete subclass of FileDescriptor, and attach it to
			// the MultipleDescriptor
			checkResult(defs.cdAIFCDescriptor()->
						CreateInstance(IID_IAAFFileDescriptor, 
									   (IUnknown **)&pFileDescriptor));		
			/*IAAFAIFCDescriptor**/			pAIFCDesc = NULL;
			checkResult(pFileDescriptor->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
			checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
			pAIFCDesc->Release();
			pAIFCDesc = NULL;

		// Insert it.  note: its 0 based so the end is numFileDescriptors - 1
		if (pMDesc->InsertFileDescriptorAt(numFileDescriptors-1, pFileDescriptor) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		// Check it
		pMDesc->GetFileDescriptorAt(numFileDescriptors-1, &pFileDescriptor2);
		if (pFileDescriptor2 != pFileDescriptor)
			localhr = AAFRESULT_TEST_FAILED;
		// Count it	
		pMDesc->CountFileDescriptors(&numFileDescriptors2);
		if (numFileDescriptors2 != numFileDescriptors+1)
			localhr = AAFRESULT_TEST_FAILED;
			
		// Make sure we can't add it again
		if (pMDesc->InsertFileDescriptorAt(numFileDescriptors+1, pFileDescriptor) != AAFRESULT_OBJECT_ALREADY_ATTACHED)
			localhr = AAFRESULT_TEST_FAILED;

		pFileDescriptor->Release();
		pFileDescriptor = NULL;
		pFileDescriptor2->Release();
		pFileDescriptor2 = NULL;

		
		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	InsertFileDescriptorAt() ...	Passed"<< endl;
		else
		{
			cout<< "	InsertFileDescriptorAt() ...	FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}


/* GetFileDescriptorAt()	******************************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;
		aafUInt32 numFileDescriptors = 0;
		pMDesc->CountFileDescriptors(&numFileDescriptors);
		// Verify that we can't remove an index value that is out of range
		// note: FileDescriptors index is 0 based so the index numFileDescriptors is out of range
		IAAFFileDescriptor*			pFileDescriptor = NULL;
		if (pMDesc->GetFileDescriptorAt(numFileDescriptors, &pFileDescriptor) != AAFRESULT_BADINDEX)
			localhr = AAFRESULT_TEST_FAILED;

		if (pFileDescriptor)
		{
			pFileDescriptor->Release();
			pFileDescriptor = NULL;
		}

		// Verify behavior when NULL is passed in
		if (pMDesc->GetFileDescriptorAt(1, NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;
			
		for (i=0; i<numFileDescriptors; i++)
		{
			IAAFFileDescriptor*		pFileDescriptor = NULL;
			if (pMDesc->GetFileDescriptorAt(i, &pFileDescriptor) != AAFRESULT_SUCCESS)
			{
				localhr = AAFRESULT_TEST_FAILED;
			}
			else
			{
				pFileDescriptor->Release();
				pFileDescriptor = NULL;
			}
		}
			
		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	GetFileDescriptorAt() ...		Passed"<< endl;
		else
		{
			cout<< "	GetFileDescriptorAt() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}


#if 1		

/* RemoveFileDescriptorAt()	******************************************/
	{
		HRESULT localhr = AAFRESULT_SUCCESS;

		// Verify that we can't remove an index value that is out of range
		// note: FileDescriptors index is 0 based so the index numFileDescriptors is out of range
		aafUInt32 numFileDescriptors = 0;
		pMDesc->CountFileDescriptors(&numFileDescriptors);

		if (pMDesc->RemoveFileDescriptorAt (numFileDescriptors) != AAFRESULT_BADINDEX)
			localhr = AAFRESULT_TEST_FAILED;
				
		// Remove FileDescriptor at beginning (index=0), but first remember the
		// next descriptor (index=1).
		IAAFFileDescriptor*			pFileDescriptor = NULL;
		pMDesc->GetFileDescriptorAt(1, &pFileDescriptor);
		if (pMDesc->RemoveFileDescriptorAt (0) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		// Verify the count
		aafUInt32 numFileDescriptors2 = 0;
		pMDesc->CountFileDescriptors(&numFileDescriptors2);
		if (numFileDescriptors2 != (numFileDescriptors - 1))
			localhr = AAFRESULT_TEST_FAILED;

		IAAFFileDescriptor*			pFileDescriptor2 = NULL;
		pMDesc->GetFileDescriptorAt(0, &pFileDescriptor2);
		// Verify that the FileDescriptors shifted properly
		if (pFileDescriptor != pFileDescriptor2)
			localhr = AAFRESULT_TEST_FAILED;
		
		pFileDescriptor->Release();
		pFileDescriptor = NULL;
		pFileDescriptor2->Release();
		pFileDescriptor2 = NULL;


		// Remove FileDescriptor at middle (index=numFileDescriptors/2), but first remember the
		// next descriptor (index=numFileDescriptors/2+1).
		pMDesc->CountFileDescriptors(&numFileDescriptors);
		pMDesc->GetFileDescriptorAt((numFileDescriptors/2 +1), &pFileDescriptor);
		if (pMDesc->RemoveFileDescriptorAt (numFileDescriptors/2) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		
		pMDesc->CountFileDescriptors(&numFileDescriptors2);
		if (numFileDescriptors2 != (numFileDescriptors - 1))
			localhr = AAFRESULT_TEST_FAILED;
		
		pMDesc->GetFileDescriptorAt(numFileDescriptors/2, &pFileDescriptor2);
		// Verify that the FileDescriptors shifted properly
		if (pFileDescriptor != pFileDescriptor2)
			localhr = AAFRESULT_TEST_FAILED;

		pFileDescriptor->Release();
		pFileDescriptor = NULL;
		pFileDescriptor2->Release();
		pFileDescriptor2 = NULL;


		// Remove FileDescriptor at end, but Release it first
		pMDesc->CountFileDescriptors(&numFileDescriptors);
		pMDesc->GetFileDescriptorAt(numFileDescriptors-1, &pFileDescriptor);
		pFileDescriptor->Release();

		// Remove FileDescriptor at end (index=numFileDescriptors-1), but first remember the
		// previous descriptor (index=numFileDescriptors-1-1).
		pMDesc->CountFileDescriptors(&numFileDescriptors);
		pMDesc->GetFileDescriptorAt(numFileDescriptors-2, &pFileDescriptor);
		if (pMDesc->RemoveFileDescriptorAt (numFileDescriptors-1) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		
		pMDesc->CountFileDescriptors(&numFileDescriptors2);
		if (numFileDescriptors2 != (numFileDescriptors - 1))
			localhr = AAFRESULT_TEST_FAILED;

		pMDesc->GetFileDescriptorAt(numFileDescriptors2-1, &pFileDescriptor2);
		// Verify that the FileDescriptors shifted properly
		if (pFileDescriptor != pFileDescriptor2)
			localhr = AAFRESULT_TEST_FAILED;

		pFileDescriptor->Release();
		pFileDescriptor = NULL;
		pFileDescriptor2->Release();
		pFileDescriptor2 = NULL;


		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	RemoveFileDescriptorAt() ...	Passed"<< endl;
		else
		{
			cout<< "	RemoveFileDescriptorAt() ...	FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
	}
#endif

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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFMob			*aMob = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFFileDescriptor	*				pFileDescriptor = NULL;
	IAAFMultipleDescriptor	*	pMDesc = NULL;
	aafUInt32					numFileDescriptors;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;


	try
	{	
    // Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;

    // We can't really do anthing in AAF without the header.
  	checkResult(pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->GetMobs (NULL, &mobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafMobID_t		mobID;

			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));

			checkResult(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));
			checkResult(pEdesc->QueryInterface (IID_IAAFMultipleDescriptor, (void **)&pMDesc));

			// Verify that there is now one FileDescriptor
			checkResult(pMDesc->CountFileDescriptors(&numFileDescriptors));
		 	checkExpression(20 == numFileDescriptors, AAFRESULT_TEST_FAILED);

			// This should read the one real FileDescriptor
			for ( n=0; n<numFileDescriptors; n++)
			{
				checkResult(pMDesc->GetFileDescriptorAt(n, &pFileDescriptor));

				pFileDescriptor->Release();
				pFileDescriptor = NULL;
			}			

			pEdesc->Release();
			pEdesc = NULL;

			pSourceMob->Release();
			pSourceMob = NULL;

			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}

	// Cleanup object references
	if (pFileDescriptor)
		pFileDescriptor->Release();

	if (pEdesc)
		pEdesc->Release();

	if (pSourceMob)
		pSourceMob->Release();
	
	if (aMob)
		aMob->Release();

	if (mobIter)
		mobIter->Release();

	if (pHeader)
		pHeader->Release();
			
	if (pFile)
	{	// Close file, clean-up and return
		if (bFileOpen)
			pFile->Close();
 		pFile->Release();
	}

	/*
	hr = pSession->EndSession();
 	if (AAFRESULT_SUCCESS != hr)
		return hr;
	*/

	return hr;
}

// Genereates a file name based on the passed in
// base name and file encoding specified by file_kind.
// The resulting file name is stored in the buffer
// addressed by the passed in p_generated_name_buffer
// pointer. The number of characters that can fit in
// the buffer (including NULL-terminator) is specified
// by generated_name_buffer_length.
static bool GenerateTestFileName(
    const aafCharacter* p_base_name,
    const size_t generated_name_buffer_length,
    aafCharacter* p_generated_name_buffer )
{
    assert( p_base_name != 0 );
    assert( wcslen( p_base_name ) != 0 );
    assert( generated_name_buffer_length > 1 );
    assert( p_generated_name_buffer != 0 );


    //
    // 1. Base name
    //
    const size_t  base_name_length = wcslen( p_base_name );


    //
    //  4. Extension
    //
    const aafCharacter  extension[] = L".aaf";
    const size_t extension_length = wcslen( extension );


    //
    // Putting it all together
    //
    const size_t  file_name_length = base_name_length +
                                     extension_length;

    bool succeeded = true;

    if( file_name_length < generated_name_buffer_length )
    {
        wcscpy( p_generated_name_buffer, p_base_name );
        wcscat( p_generated_name_buffer, extension );

        succeeded = true;
    }
    else
    {
        succeeded = false;
    }

    return succeeded;
}


// Required function prototype.
extern "C" HRESULT CAAFMultipleDescriptor_test(
    testMode_t mode
);

//
// The public entry for this module test,
//
HRESULT CAAFMultipleDescriptor_test(
    testMode_t mode
)
{
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 2;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	productInfo.companyName = L"AAF Developers Desk";
	productInfo.productName = L"AAFMultipleDescriptor Test";
	productInfo.productVersion = &v;
	productInfo.productVersionString = NULL;
	productInfo.productID = UnitTestProductID;
	productInfo.platform = NULL;

	HRESULT hr = AAFRESULT_SUCCESS;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productInfo.productName, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFMultipleDescriptor_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
