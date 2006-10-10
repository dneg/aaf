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
	IAAFSourceMob	*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IAAFEssenceDescriptor *pEssenceDesc = NULL;
	IAAFAES3PCMDescriptor *pAES3PCMDesc = NULL;
	
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

		
		long			test;
		aafRational_t	audioRate = { 44100, 1 };
		
		// Create a Mob
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));
		
		checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AES3PCMDescriptorTest"));
		
		// Add some slots
		for(test = 0; test < 2; test++)
		{
			checkResult(pSourceMob->AddNilReference (test+1, 0, defs.ddkAAFSound(), audioRate));
		}
		
		// Create a an instance of AES3PCMDescriptor
		checkResult(defs.cdAES3PCMDescriptor()->
					CreateInstance(IID_IAAFAES3PCMDescriptor, 
								   (IUnknown **)&pAES3PCMDesc));		

		checkResult(pAES3PCMDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **) &pEssenceDesc));
        checkResult(pSourceMob->SetEssenceDescriptor(pEssenceDesc));
		
		pAES3PCMDesc->Release();
		pAES3PCMDesc = NULL;
		
		checkResult(pHeader->AddMob(pMob));
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	if (pEssenceDesc)
		pEssenceDesc->Release();
	
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

static HRESULT ReadAAFFile(
    aafWChar * pFileName )
{
    // IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs *mobIter = NULL;
	IAAFEssenceDescriptor		*pEssenceDesc = NULL;
	IAAFSourceMob				*pSourceMob = NULL;
	IAAFMob			*pMob = NULL;
	IEnumAAFMobSlots	*slotIter = NULL;
	IAAFMobSlot		*slot = NULL;
	IAAFAES3PCMDescriptor 	*	pAES3PCMDesc = NULL;
	aafNumSlots_t	numMobs, n, s;
	HRESULT						hr = S_OK;


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
			
			checkResult(mobIter->NextOne (&pMob));
			checkResult(pMob->GetName (name, sizeof(name)));
			checkResult(pMob->GetMobID (&mobID));
			
			checkResult(pMob->CountSlots (&numSlots));
			if (2 != numSlots)
				return AAFRESULT_TEST_FAILED;
			if(numSlots != 0)
			{
				checkResult(pMob->GetSlots(&slotIter));
				
				for(s = 0; s < numSlots; s++)
				{
					checkResult(slotIter->NextOne (&slot));
					checkResult(slot->GetSlotID(&trackID));
					
					slot->Release();
					slot = NULL;
				}
			}
        // Get the source mob
        checkResult(pMob->QueryInterface (IID_IAAFSourceMob, (void **)&pSourceMob));

        // Get the sound descriptor
        checkResult(pSourceMob->GetEssenceDescriptor(&pEssenceDesc));
        checkResult(pEssenceDesc->QueryInterface(IID_IAAFAES3PCMDescriptor,
                                                 (void **)&pAES3PCMDesc));


		}
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }


    if (pAES3PCMDesc) 
    {
        pAES3PCMDesc->Release();
    }
    if (pEssenceDesc) 
    {
        pEssenceDesc->Release();
    }
    if (pSourceMob) 
    {
        pSourceMob->Release();
    }
    if (pMob) 
    {
        pMob->Release();
    }
    if (pHeader) 
    {
        pHeader->Release();
    }
    if (pFile) 
    {
        pFile->Close();
        pFile->Release();
    }


    return hr;
}


extern "C" HRESULT CAAFAES3PCMDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFAES3PCMDescriptor_test(
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
		cerr << "CAAFAES3PCMDescriptor_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	return hr;
}


