
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

#include <stdio.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFContainerDefs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xe86291dc, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static aafRational_t	checkSampleRate = { 2997, 100 };
static aafUID_t			checkContainer = ContainerFile;
static aafLength_t		checkLength = 42;
static aafUInt32			checkLinkedSlotID = 5;

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
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFContainerDef*  pContainer = NULL;
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *edesc = NULL;
	IAAFFileDescriptor *pFileDesc = NULL;
	IAAFFileDescriptor2 *pFileDesc2 = NULL;
	
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
		long			test;
		aafRational_t	audioRate = { 44100, 1 };
		
		// Create a Mob
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));
		
		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"FileDescriptorTest"));
		
		// Add some slots
		for(test = 0; test < 2; test++)
		{
			checkResult(pSourceMob->AddNilReference (test+1, 0, defs.ddkAAFSound(), audioRate));
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

		checkResult(edesc->QueryInterface(IID_IAAFFileDescriptor, (void **) &pFileDesc));
		checkResult(pFileDesc->SetSampleRate (checkSampleRate));
		checkResult(pDictionary->LookupContainerDef(checkContainer, &pContainer));
		checkResult(pFileDesc->SetContainerFormat (pContainer));
		pContainer->Release();
		pContainer = NULL;

		checkResult(pFileDesc->SetLength (checkLength));
		checkResult(pFileDesc->QueryInterface(IID_IAAFFileDescriptor2, (void **)&pFileDesc2));
		checkResult(pFileDesc2->SetLinkedSlotID (checkLinkedSlotID));
//		checkResult(pFileDesc->SetIsInContainer (kAAFTrue));
		
		checkResult(pSourceMob->SetEssenceDescriptor (edesc));
		
		checkResult(pHeader->AddMob(pMob));
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	if (pFileDesc)
		pFileDesc->Release();

	if (pFileDesc2)
		pFileDesc2->Release();

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
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFContainerDef*  pContainer = NULL;
	IAAFDefObject	*  pDef = NULL;
	IAAFEssenceDescriptor		*pEdesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob			*aMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot		*slot = NULL;
	IAAFFileDescriptor *pFileDesc = NULL;
	IAAFFileDescriptor2 *pFileDesc2 = NULL;
	aafNumSlots_t	numMobs, n, s;
	HRESULT						hr = S_OK;
	aafRational_t				testSampleRate;
	aafUID_t					testContainer;
	aafLength_t					testLength;
	aafUInt32					testLinkedSlotID;
//	aafBool						testBool;
	
	
	try
	{ 
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		
		checkResult(pHeader->GetMobs (NULL, &mobIter));
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500];
			aafNumSlots_t	numSlots;
			aafMobID_t		mobID;
			aafSlotID_t		trackID;
			
			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));
			
			checkResult(aMob->CountSlots (&numSlots));
			if (2 != numSlots)
				return AAFRESULT_TEST_FAILED;
			if(numSlots != 0)
			{
				checkResult(aMob->GetSlots(&slotIter));
				
				for(s = 0; s < numSlots; s++)
				{
					checkResult(slotIter->NextOne (&slot));
					checkResult(slot->GetSlotID(&trackID));
					
					slot->Release();
					slot = NULL;
				}
			}
			checkResult(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));
			checkResult(pSourceMob->GetEssenceDescriptor (&pEdesc));
			checkResult(pEdesc->QueryInterface(IID_IAAFFileDescriptor, (void **) &pFileDesc));
			checkResult(pFileDesc->GetSampleRate (&testSampleRate));
			checkExpression(testSampleRate.numerator == checkSampleRate.numerator, AAFRESULT_TEST_FAILED);
			checkExpression(testSampleRate.denominator == checkSampleRate.denominator, AAFRESULT_TEST_FAILED);
			checkResult(pFileDesc->GetContainerFormat (&pContainer));
			checkResult(pContainer->QueryInterface(IID_IAAFDefObject, (void **) &pDef));
			checkResult(pDef->GetAUID(&testContainer));
			pContainer->Release();
			pContainer = NULL;
			pDef->Release();
			pDef = NULL;
			checkExpression(memcmp(&testContainer, &checkContainer, sizeof(testContainer)) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pFileDesc->GetLength (&testLength));
			checkExpression(checkLength == testLength, AAFRESULT_TEST_FAILED);
			checkResult(pFileDesc->QueryInterface(IID_IAAFFileDescriptor2, (void **)&pFileDesc2));
			checkResult(pFileDesc2->GetLinkedSlotID (&testLinkedSlotID));
			checkExpression(checkLinkedSlotID == testLinkedSlotID, AAFRESULT_TEST_FAILED);
//			checkResult(pFileDesc->GetIsInContainer (&testBool));
//			checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
			
			pEdesc->Release();
			pEdesc = NULL;
			pFileDesc->Release();
			pFileDesc = NULL;
			pFileDesc2->Release();
			pFileDesc2 = NULL;
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
	
	
	// Cleanup and return
	if (slot)
		slot->Release();
	
	if (pFileDesc)
		pFileDesc->Release();
			
	if (pFileDesc2)
		pFileDesc2->Release();
			
	if (slotIter)
		slotIter->Release();
	
	if (pEdesc)
		pEdesc->Release();
	
	if (pSourceMob)
		pSourceMob->Release();
	
	if (aMob)
		aMob->Release();
	
	if(mobIter)
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

extern "C" HRESULT CAAFFileDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFFileDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
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
		cerr << "CAAFFileDescriptor_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	return hr;
}


