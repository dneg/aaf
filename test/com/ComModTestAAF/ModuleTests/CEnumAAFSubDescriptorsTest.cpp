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
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x53c55746, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

const aafUID_t TestSubDescriptorClassID = 
{ 0x023a1cae, 0xdc16, 0x4db9, { 0x95, 0xf9, 0x43, 0xd0, 0x56, 0xca, 0xd3, 0x78 } };

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


static void RegisterSubDescriptorTest( IAAFDictionary* pDict )
{

  // Create a new class definition for the TestDescriptiveFramework.

  IAAFSmartPointer<IAAFClassDef> pClassDef;
  checkResult( pDict->CreateMetaInstance( AUID_AAFClassDef, IID_IAAFClassDef,
					  (IUnknown**)&pClassDef ) );

  IAAFSmartPointer<IAAFClassDef> pBaseClassDef;
  checkResult( pDict->LookupClassDef( AUID_AAFSubDescriptor, &pBaseClassDef ) );

  checkResult( pClassDef->Initialize( TestSubDescriptorClassID,
				      pBaseClassDef,
				      L"TestSubDescriptor",
				      kAAFTrue ) );

  checkResult( pDict->RegisterClassDef( pClassDef ) );
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFSubDescriptor	*				pSubDescriptor = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	IAAFEssenceDescriptor2 *edesc2 = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUInt32					numSubDescriptors;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
//	aafUID_t					ddef = kAAFDataDef_Sound;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"EnumAAFSubDescriptors Test";
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
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"EssenceDescriptorTest"));
		
		// Create a concrete subclass of the descriptor:
		checkResult(defs.cdAIFCDescriptor()->
					CreateInstance(IID_IAAFEssenceDescriptor, 
								   (IUnknown **)&edesc));		

		IAAFAIFCDescriptor*			pAIFCDesc = NULL;
		checkResult(edesc->QueryInterface (IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc));
		checkResult(pAIFCDesc->SetSummary (5, (unsigned char*)"TEST"));
		pAIFCDesc->Release();
		pAIFCDesc = NULL;

 		checkResult(pSourceMob->SetEssenceDescriptor (edesc));

			// Verify that there are no subdescriptors
                checkResult(edesc->QueryInterface(IID_IAAFEssenceDescriptor2, (void **)&edesc2));
		checkResult(edesc2->CountSubDescriptors(&numSubDescriptors));
		checkExpression(0 == numSubDescriptors, AAFRESULT_TEST_FAILED);
		
		RegisterSubDescriptorTest( pDictionary );
  		
		// Make a subdescriptor, and attach it to the EssenceDescriptor
		checkResult( pDictionary->CreateInstance( TestSubDescriptorClassID,
							   IID_IAAFSubDescriptor,
							   (IUnknown**)&pSubDescriptor ) );		

		checkResult(edesc2->AppendSubDescriptor(pSubDescriptor));
		pSubDescriptor->Release();
		pSubDescriptor = NULL;

		// Verify that there is now one subdescriptor
		checkResult(edesc2->CountSubDescriptors(&numSubDescriptors));
		checkExpression(1 == numSubDescriptors, AAFRESULT_TEST_FAILED);

		// Make a second subdescriptor, and attach it to the EssenceDescriptor
		checkResult( pDictionary->CreateInstance( TestSubDescriptorClassID,
							   IID_IAAFSubDescriptor,
							   (IUnknown**)&pSubDescriptor ) );		
	
		checkResult(edesc2->AppendSubDescriptor(pSubDescriptor));
		pSubDescriptor->Release();
		pSubDescriptor = NULL;

		// Add the source mob into the tree
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}



	// Cleanup object references
	if (pSubDescriptor)
		pSubDescriptor->Release();

	if (edesc)
		edesc->Release();

	if (edesc2)
		edesc2->Release();

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
	IAAFEssenceDescriptor2		*pEdesc2 = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IEnumAAFSubDescriptors *			pEnum = NULL;
	IEnumAAFSubDescriptors *			pCloneEnum = NULL;
	IAAFSubDescriptor	*				pSubDescriptor = NULL;
	aafUInt32					numSubDescriptors;
	aafNumSlots_t	numMobs, n;
	HRESULT						hr = AAFRESULT_SUCCESS;
	bool bFileOpen = false;
	IAAFSubDescriptor				*	pArray[2] = { NULL, NULL };
	IAAFSubDescriptor				**	pArrayPoint = pArray;
	aafUInt32			resultCount;


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
			checkResult(pEdesc->QueryInterface(IID_IAAFEssenceDescriptor2, (void **)&pEdesc2));

			// Verify that there is now two subdescriptors
			checkResult(pEdesc2->CountSubDescriptors(&numSubDescriptors));
		  checkExpression(2 == numSubDescriptors, AAFRESULT_TEST_FAILED);
		
			checkResult(pEdesc2->GetSubDescriptors(&pEnum));

			/* Read and check the first element */
			checkResult(pEnum->NextOne(&pSubDescriptor));
			
			pSubDescriptor->Release();
			pSubDescriptor = NULL;

			/**/
			/* Read and check the second element */
			checkResult(pEnum->NextOne(&pSubDescriptor));
			pSubDescriptor->Release();
			pSubDescriptor = NULL;
			/*****/
			
			/* Reset, and check the first element again*/
			checkResult(pEnum->Reset());
			checkResult(pEnum->NextOne(&pSubDescriptor));
			pSubDescriptor->Release();
			pSubDescriptor = NULL;
			
			/* Reset, Skip, and check the second element again*/
			checkResult(pEnum->Reset());
			checkResult(pEnum->Skip(1));
			checkResult(pEnum->NextOne(&pSubDescriptor));
			pSubDescriptor->Release();
			pSubDescriptor = NULL;

			/* Reset, and read both elements */
			checkResult(pEnum->Reset());
			checkResult(pEnum->Next (2, (IAAFSubDescriptor **)&pArray, &resultCount));
			checkExpression (resultCount == 2, AAFRESULT_TEST_FAILED);
			pArrayPoint[0]->Release();
			pArrayPoint[0] = NULL;
			
			pArrayPoint[1]->Release();
			pArrayPoint[1] = NULL;
			
			/* Read one past to make sure that it fails */
			checkExpression(pEnum->NextOne(&pSubDescriptor) != AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED);
			/* Clone the enumerator, and read one element */
			checkResult(pEnum->Clone(&pCloneEnum));
			checkResult(pCloneEnum->Reset());
			checkResult(pCloneEnum->NextOne(&pSubDescriptor));
	
			pSubDescriptor->Release();
			pSubDescriptor = NULL;

			pEnum->Release();
			pEnum = NULL;

			pEdesc->Release();
			pEdesc = NULL;

			pEdesc2->Release();
			pEdesc2 = NULL;

			pSourceMob->Release();
			pSourceMob = NULL;

			aMob->Release();
			aMob = NULL;
			pCloneEnum->Release();
			pCloneEnum = NULL;

		}
	}
	catch (HRESULT& rResult)
	{
    hr = rResult;
	}

	// Cleanup object references
	if (pSubDescriptor)
		pSubDescriptor->Release();

	if (pEnum)
		pEnum->Release();

	if (pEdesc)
		pEdesc->Release();

	if (pEdesc2)
		pEdesc2->Release();

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
 
extern "C" HRESULT CEnumAAFSubDescriptors_test(testMode_t mode);
extern "C" HRESULT CEnumAAFSubDescriptors_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  aafWChar * pFileName = L"EnumAAFSubDescriptors.aaf";

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
	  cerr << "CEnumAAFSubDescriptors_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
