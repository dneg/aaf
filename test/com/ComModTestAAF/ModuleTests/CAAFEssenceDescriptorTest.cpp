// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceDescriptor
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

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x9cb4ad10, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

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
	// IAAFSession 				*pSession = NULL;
	IAAFFile 					*pFile = NULL;
	IAAFHeader 					*pHeader = NULL;
	IAAFDictionary				*pDictionary = NULL;
	IAAFLocator					*pLocator = NULL;
	IAAFLocator					*pLocator2 = NULL;
	IAAFSourceMob 				*pSourceMob = NULL;
	IAAFMob						*pMob = NULL;
	IAAFEssenceDescriptor 		*edesc = NULL;
	IEnumAAFLocators			*pEnumLocators = NULL;
	aafRational_t	audioRate = { 44100, 1 };
	aafProductIdentification_t	ProductInfo;
	aafMobID_t					newMobID;
	aafUInt32					numLocators, numLocators2;
	aafUInt32					i;
	HRESULT						hr = AAFRESULT_SUCCESS,
								localhr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
//	aafUID_t					ddef = DDEF_Sound;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFEssenceDescriptor Test";
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
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(newMobID));
		checkResult(pMob->SetName(L"EssenceDescriptorTest"));
		
		// Create the descriptor:
		checkResult(defs.cdEssenceDescriptor()->
					CreateInstance(IID_IAAFEssenceDescriptor, 
								   (IUnknown **)&edesc));		
 		checkResult(pSourceMob->SetEssenceDescriptor (edesc));


/* CountLocators()	******************************************/
		localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (edesc->CountLocators(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		
		
		// Verify that there are no locators
		if (edesc->CountLocators(&numLocators) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		if (0 != numLocators)
			localhr = AAFRESULT_TEST_FAILED;
			
		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	CountLocators() ...		Passed"<< endl;
		else
		{
			cout<< "	CountLocators() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
		
/* AppendLocator()	******************************************/
		localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (edesc->AppendLocator(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		

		// Append and Count a bunch of Locators
		for (i=1; i<=10; i++)
		{
			// Make a locator, and attach it to the EssenceDescriptor
			checkResult(defs.cdLocator()->
					CreateInstance(IID_IAAFLocator, 
								   (IUnknown **)&pLocator));	
								   	
			if (edesc->AppendLocator(pLocator) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;

			// Verify the number of locators
			numLocators = 0;
			edesc->CountLocators(&numLocators);
			if (i != numLocators)
				localhr = AAFRESULT_TEST_FAILED;

			// Verify that locator was appended
			edesc->GetLocatorAt(i-1, &pLocator2);
			if (pLocator2 != pLocator)
				localhr = AAFRESULT_TEST_FAILED;
		}

		// Make sure we can't add it again

//		if (edesc->AppendLocator(pLocator) != AAFRESULT_OBJECT_ALREADY_ATTACHED)

//			localhr = AAFRESULT_TEST_FAILED;


		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	AppendLocator() ...		Passed"<< endl;
		else
		{
			cout<< "	AppendLocator() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
			

/* PrependLocator()	******************************************/
		localhr = AAFRESULT_SUCCESS;
		// Verify AAFRESULT_NULL_PARAM is returned
		if (edesc->PrependLocator(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;		

		for (i; i<=20; i++)
		{
			// Make a locator, and attach it to the EssenceDescriptor
			checkResult(defs.cdLocator()->
					CreateInstance(IID_IAAFLocator, 
								   (IUnknown **)&pLocator));	
								   	
			if (edesc->PrependLocator(pLocator) != AAFRESULT_SUCCESS)
				localhr = AAFRESULT_TEST_FAILED;

			// Verify the number of locators
			numLocators = 0;
			edesc->CountLocators(&numLocators);
			if (i != numLocators)
				localhr = AAFRESULT_TEST_FAILED;
				
			// Verify that locator was prepended
			edesc->GetLocatorAt(0, &pLocator2);
			if (pLocator2 != pLocator)
				localhr = AAFRESULT_TEST_FAILED;
		}

		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	PrependLocator() ...	Passed"<< endl;
		else
		{
			cout<< "	PrependLocator() ...	FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}


/* InsertLocatorAt()	**************************************/
		localhr = AAFRESULT_SUCCESS;

		// Make a locator to attach
		checkResult(defs.cdLocator()->
						CreateInstance(IID_IAAFLocator, 
				   			(IUnknown **)&pLocator));			

		// Verify that we can't remove an index value that is out of range
		if (edesc->InsertLocatorAt(numLocators+1, pLocator) != AAFRESULT_BADINDEX)
			localhr = AAFRESULT_TEST_FAILED;

		// Verify behavior when NULL is passed in
		if (edesc->InsertLocatorAt(1, NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;

		edesc->CountLocators(&numLocators);

		// Insert it
		if (edesc->InsertLocatorAt(0, pLocator) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		// Check it
		edesc->GetLocatorAt(0, &pLocator2);
		if (pLocator2 != pLocator)
			localhr = AAFRESULT_TEST_FAILED;
		// Count it	
		edesc->CountLocators(&numLocators2);
		if (numLocators2 != numLocators+1)
			localhr = AAFRESULT_TEST_FAILED;
			
		edesc->CountLocators(&numLocators);
		// Make a locator to attach in the middle
		checkResult(defs.cdLocator()->
						CreateInstance(IID_IAAFLocator, 
				   			(IUnknown **)&pLocator));			
		// Insert it
		if (edesc->InsertLocatorAt(numLocators/2, pLocator) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		// Check it
		edesc->GetLocatorAt(numLocators/2, &pLocator2);
		if (pLocator2 != pLocator)
			localhr = AAFRESULT_TEST_FAILED;
		// Count it	
		edesc->CountLocators(&numLocators2);
		if (numLocators2 != numLocators+1)
			localhr = AAFRESULT_TEST_FAILED;

		edesc->CountLocators(&numLocators);
		// Make a locator to attach to the end
		checkResult(defs.cdLocator()->
						CreateInstance(IID_IAAFLocator, 
				   			(IUnknown **)&pLocator));			
		// Insert it.  note: its 0 based so the end is numLocators - 1
		if (edesc->InsertLocatorAt(numLocators-1, pLocator) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		// Check it
		edesc->GetLocatorAt(numLocators-1, &pLocator2);
		if (pLocator2 != pLocator)
			localhr = AAFRESULT_TEST_FAILED;
		// Count it	
		edesc->CountLocators(&numLocators2);
		if (numLocators2 != numLocators+1)
			localhr = AAFRESULT_TEST_FAILED;
			
		// Make sure we can't add it again
		if (edesc->InsertLocatorAt(numLocators+1, pLocator) != AAFRESULT_OBJECT_ALREADY_ATTACHED)
			localhr = AAFRESULT_TEST_FAILED;

		
		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	InsertLocatorAt() ...	Passed"<< endl;
		else
		{
			cout<< "	InsertLocatorAt() ...	FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}
			
			
/* GetLocatorAt()	******************************************/
		localhr = AAFRESULT_SUCCESS;
		edesc->CountLocators(&numLocators);
		// Verify that we can't remove an index value that is out of range
		// note: Locators index is 0 based so the index numLocators is out of range
		if (edesc->GetLocatorAt(numLocators, &pLocator) != AAFRESULT_BADINDEX)
			localhr = AAFRESULT_TEST_FAILED;

		// Verify behavior when NULL is passed in
		if (edesc->GetLocatorAt(1, NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;
			
		for (i=0; i<numLocators; i++)
		{
			pLocator = NULL;
			if (edesc->GetLocatorAt(i, &pLocator) != AAFRESULT_SUCCESS)
				if (pLocator == NULL)
					localhr = AAFRESULT_TEST_FAILED;
				else
					pLocator->Release();
		}
			
		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	GetLocatorAt() ...		Passed"<< endl;
		else
		{
			cout<< "	GetLocatorAt() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}


/* GetLocators()	******************************************/
		if (edesc->GetLocators(NULL) != AAFRESULT_NULL_PARAM)
			localhr = AAFRESULT_TEST_FAILED;	

		pEnumLocators = NULL;
		if (edesc->GetLocators(&pEnumLocators) == AAFRESULT_SUCCESS)
		{
			if (pEnumLocators != NULL)
			{	
				// Try a simple test to confirm
				if (pEnumLocators->NextOne(&pLocator) != AAFRESULT_SUCCESS)					
					localhr = AAFRESULT_TEST_FAILED;
				edesc->GetLocatorAt(0, &pLocator2);

				if (pLocator != pLocator2)
					localhr = AAFRESULT_TEST_FAILED;
				else
					pLocator2->Release();
					
				pEnumLocators->Release();
			}
			else
				localhr = AAFRESULT_TEST_FAILED;			
		}
		else	
			localhr = AAFRESULT_TEST_FAILED;

		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	GetLocators() ...		Passed"<< endl;
		else
		{
			cout<< "	GetLocators() ...		FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}


		

/* RemoveLocatorAt()	******************************************/
		localhr = AAFRESULT_SUCCESS;

		// Verify that we can't remove an index value that is out of range
		// note: Locators index is 0 based so the index numLocators is out of range

		if (edesc->RemoveLocatorAt (numLocators) != AAFRESULT_BADINDEX)
			localhr = AAFRESULT_TEST_FAILED;
				
		// Remove locator at beginning, but Release it first
		edesc->CountLocators(&numLocators);

		edesc->GetLocatorAt(0, &pLocator);
		pLocator->Release();

		edesc->GetLocatorAt(1, &pLocator);
		if (edesc->RemoveLocatorAt (0) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;

		// Verify the count
		edesc->CountLocators(&numLocators2);
		if (numLocators2 != (numLocators - 1))
			localhr = AAFRESULT_TEST_FAILED;

		edesc->GetLocatorAt(0, &pLocator2);
		// Verify that the locators shifted properly
		if (pLocator != pLocator2)
			localhr = AAFRESULT_TEST_FAILED;
		
		// Remove locator in middle, but Release it first
		edesc->CountLocators(&numLocators);
		edesc->GetLocatorAt((numLocators/2), &pLocator);
		pLocator->Release();

		edesc->GetLocatorAt((numLocators/2 +1), &pLocator);
		if (edesc->RemoveLocatorAt (numLocators/2) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		
		edesc->CountLocators(&numLocators2);
		if (numLocators2 != (numLocators - 1))
			localhr = AAFRESULT_TEST_FAILED;
		
		edesc->GetLocatorAt(numLocators/2, &pLocator2);
		// Verify that the locators shifted properly
		if (pLocator != pLocator2)
			localhr = AAFRESULT_TEST_FAILED;

		// Remove locator at end, but Release it first
		edesc->CountLocators(&numLocators);
		edesc->GetLocatorAt(numLocators-1, &pLocator);
		pLocator->Release();

		edesc->GetLocatorAt(numLocators-2, &pLocator);
		if (edesc->RemoveLocatorAt (numLocators-1) != AAFRESULT_SUCCESS)
			localhr = AAFRESULT_TEST_FAILED;
		
		edesc->CountLocators(&numLocators2);
		if (numLocators2 != (numLocators - 1))
			localhr = AAFRESULT_TEST_FAILED;

		edesc->GetLocatorAt(numLocators2-1, &pLocator2);
		// Verify that the locators shifted properly
		if (pLocator != pLocator2)
			localhr = AAFRESULT_TEST_FAILED;

		if (localhr == AAFRESULT_SUCCESS)
			cout<< "	RemoveLocatorAt() ...	Passed"<< endl;
		else
		{
			cout<< "	RemoveLocatorAt() ...	FAILED"<< endl;
			hr = AAFRESULT_TEST_FAILED;
		}

/*************************************************************/

	// Add the source mob into the tree
	checkResult(pHeader->AddMob(pMob));

	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}


	// Cleanup object references
	edesc->CountLocators(&numLocators);

	for (i=0; i<numLocators; ++i)
	{
		edesc->GetLocatorAt(i, &pLocator);
		pLocator->Release();
	}

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
	IEnumAAFLocators *			pEnum = NULL;
	IAAFLocator	*				pLocator = NULL;
	aafUInt32					numLocators;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"AAFEssenceDescriptor Test. NOT!";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;

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

			// Verify that there is now one locator
			checkResult(pEdesc->CountLocators(&numLocators));
		 	checkExpression(20 == numLocators, AAFRESULT_TEST_FAILED);
		
			checkResult(pEdesc->GetLocators(&pEnum));

			// This should read the one real locator
			for ( n=0; n<numLocators; n++)
			{
				checkResult(pEnum->NextOne(&pLocator));

				pLocator->Release();
				pLocator = NULL;
			}

     		// We had better not succeed or get an unknown failure.
     		checkExpression(AAFRESULT_NO_MORE_OBJECTS == pEnum->NextOne(&pLocator),
                      AAFRESULT_TEST_FAILED);
			

			pEnum->Release();
			pEnum = NULL;

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
	if (pLocator)
		pLocator->Release();

	if (pEnum)
		pEnum->Release();

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
 
extern "C" HRESULT CAAFEssenceDescriptor_test()
{
  HRESULT hr = AAFRESULT_SUCCESS;
  aafWChar * pFileName = L"AAFEssenceDescriptorTest.aaf";

  try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
  catch (...)
	{
	  cerr << "CAAFEssenceDescriptor_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}

