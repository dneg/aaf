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

const aafUInt32 SizeOfFixedChannelStatusData = 24;
const aafUInt32 SizeOfFixedUserData = 24;




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

static HRESULT 
TestArrayBasedStuff(aafUInt32 channelCount, IAAFAES3PCMDescriptor2 * pAES3PCMDesc2)
{
	for(aafUInt32 count = 0; count < channelCount; ++count) {
		// test ChannelStatusModeArray
		aafChannelStatusModeType_t currChanStatus = kAAFChannelStatusMode_None;
		checkResult(pAES3PCMDesc2->GetChannelStatusModeAt(count,
			&currChanStatus));
		checkExpression(currChanStatus == kAAFChannelStatusMode_Essence,
			AAFRESULT_TEST_FAILED);

		// test FixedChannelStatusDataArray
		aafUInt8 fixedChannelStatusData[SizeOfFixedChannelStatusData];
		checkResult(pAES3PCMDesc2->GetFixedChannelStatusDataAt(count,
			SizeOfFixedChannelStatusData,
			fixedChannelStatusData));
		for(aafUInt32 index = 0; index < SizeOfFixedChannelStatusData; ++index) {
			checkExpression(fixedChannelStatusData[index] == (count + index + 100),
				AAFRESULT_TEST_FAILED);
		}

		// test dataModeArray
		aafUserDataModeType_t currDataMode = kAAFUserDataMode_NotDefined;
		checkResult(pAES3PCMDesc2->GetUserDataModeAt(count,
			&currDataMode));
		checkExpression(currDataMode == kAAFUserDataMode_IEC,
			AAFRESULT_TEST_FAILED);

		// test fixedUserDataArray
		aafUInt8 fixedUserData[SizeOfFixedUserData];
		checkResult(pAES3PCMDesc2->GetFixedUserDataAt(count, 
			SizeOfFixedChannelStatusData,
			fixedUserData));
		for(aafUInt8 index = 0; index < SizeOfFixedUserData; ++index) {
			checkExpression(fixedUserData[index] == (count + index + 100),
				AAFRESULT_TEST_FAILED);
		}
	}
	return(S_OK);
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
			checkResult(pSourceMob->AddNilReference (test+1, 0, defs.
													 ddkAAFSound(), 
													 audioRate));
		}
		
		// Create a an instance of AES3PCMDescriptor
		checkResult(defs.cdAES3PCMDescriptor()->
					CreateInstance(IID_IAAFAES3PCMDescriptor, 
								   (IUnknown **)&pAES3PCMDesc));		

		checkResult(pAES3PCMDesc->QueryInterface(IID_IAAFEssenceDescriptor, 
												 (void **) &pEssenceDesc));
        checkResult(pSourceMob->SetEssenceDescriptor(pEssenceDesc));
		
		checkResult(pHeader->AddMob(pMob));

		// Create a an instance of AES3PCMDescriptor2
		IAAFAES3PCMDescriptor2 *pAES3PCMDesc2 = NULL;
		checkResult(defs.cdAES3PCMDescriptor()->
					CreateInstance(IID_IAAFAES3PCMDescriptor2, 
								   (IUnknown **)&pAES3PCMDesc2));		

		pEssenceDesc->Release();

		checkResult(pAES3PCMDesc2->QueryInterface(IID_IAAFEssenceDescriptor, 
				    (void **) &pEssenceDesc));
        checkResult(pSourceMob->SetEssenceDescriptor(pEssenceDesc));

		IAAFSoundDescriptor *pSoundDesc = NULL;
		pAES3PCMDesc2->QueryInterface(IID_IAAFSoundDescriptor,
									  (void **) &pSoundDesc);

		checkResult(pFile->Save());

		// test initialization
		AAFRESULT res = pAES3PCMDesc2->SetEmphasis(kAAFEmphasis_Unknown);
		checkExpression(res == AAFRESULT_NOT_INITIALIZED,
						AAFRESULT_TEST_FAILED);
		checkResult(pAES3PCMDesc2->Initialize());

		// test emphasis
		aafEmphasisType_t emphasisType = kAAFEmphasis_Unknown;
		res = pAES3PCMDesc2->GetEmphasis(&emphasisType);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);

		checkResult(pAES3PCMDesc2->SetEmphasis(kAAFEmphasis_Reserved0));
		checkResult(pAES3PCMDesc2->GetEmphasis(&emphasisType));
		checkExpression((emphasisType == kAAFEmphasis_Reserved0), 
						AAFRESULT_TEST_FAILED);
		res = pAES3PCMDesc2->SetEmphasis(8);
		checkExpression(res == AAFRESULT_INVALID_ENUM_VALUE,
						AAFRESULT_TEST_FAILED);
		checkResult(pAES3PCMDesc2->SetEmphasis(kAAFEmphasis_ITU));
		checkResult(pAES3PCMDesc2->GetEmphasis(&emphasisType));
		checkExpression((emphasisType == kAAFEmphasis_ITU), 
						AAFRESULT_TEST_FAILED);

		// test blockStartOffset
		aafUInt16 blockStartOffset = 0;
		res = pAES3PCMDesc2->GetBlockStartOffset(&blockStartOffset);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);
		checkResult(pAES3PCMDesc2->SetBlockStartOffset(100));
		checkResult(pAES3PCMDesc2->GetBlockStartOffset(&blockStartOffset));
		checkExpression((blockStartOffset == 100), 
						AAFRESULT_TEST_FAILED);
		checkResult(pAES3PCMDesc2->SetBlockStartOffset(65535));
		checkResult(pAES3PCMDesc2->GetBlockStartOffset(&blockStartOffset));
		checkExpression((blockStartOffset == 65535), 
						AAFRESULT_TEST_FAILED);

		// test auxBitsMode
        aafAuxBitsModeType_t auxBitsMode = kAAFAuxBitsMode_NotDefined;
		res = pAES3PCMDesc2->GetAuxBitsMode(&auxBitsMode);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);
		res = pAES3PCMDesc2->SetAuxBitsMode(8);
		checkExpression(res == AAFRESULT_INVALID_ENUM_VALUE,
						AAFRESULT_TEST_FAILED);
		checkResult(pAES3PCMDesc2->SetAuxBitsMode(kAAFAuxBitsMode_Reserved3));
		checkResult(pAES3PCMDesc2->GetAuxBitsMode(&auxBitsMode));
		checkExpression(auxBitsMode == kAAFAuxBitsMode_Reserved3,
						AAFRESULT_TEST_FAILED);


		// now test array based stuff

		aafChannelStatusModeType_t ChannelStatusMode = 
									kAAFChannelStatusMode_None;
		res = pAES3PCMDesc2->GetChannelStatusModeAt(1, &ChannelStatusMode);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);

		aafUInt8 fixedChannelStatusData[SizeOfFixedChannelStatusData];
		res = 
			pAES3PCMDesc2->GetFixedChannelStatusDataAt(1, 
			SizeOfFixedChannelStatusData,
			fixedChannelStatusData);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);

		aafUserDataModeType_t UserDataMode = kAAFUserDataMode_NotDefined;
		res = pAES3PCMDesc2->GetUserDataModeAt(1, &UserDataMode);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);

		aafUInt8 fixedUserData[SizeOfFixedUserData];
		res = pAES3PCMDesc2->GetFixedUserDataAt(1, SizeOfFixedChannelStatusData,
			fixedUserData);
		checkExpression(res == AAFRESULT_PROP_NOT_PRESENT,
						AAFRESULT_TEST_FAILED);

		aafUInt32 channelCount = 4;
		checkResult(pSoundDesc->SetChannelCount(channelCount));
		channelCount = 0;
		checkResult(pSoundDesc->GetChannelCount(&channelCount));
		checkExpression(channelCount == 4, AAFRESULT_TEST_FAILED);

		for(aafUInt8 count = 0; count < channelCount; ++count) {
			// test ChannelStatusModeArray
			res = pAES3PCMDesc2->SetChannelStatusModeAt(count, 6);
			checkExpression(res == AAFRESULT_INVALID_ENUM_VALUE, 
				AAFRESULT_TEST_FAILED);
			checkResult(pAES3PCMDesc2->SetChannelStatusModeAt(count,
				kAAFChannelStatusMode_Essence));

			// test FixedChannelStatusDataArray
			for(aafUInt8 index = 0; 
				index < SizeOfFixedChannelStatusData; 
				++index) 
			{
				fixedChannelStatusData[index] = count + index + 100;
			}
			checkResult(pAES3PCMDesc2->SetFixedChannelStatusDataAt(count,
						SizeOfFixedChannelStatusData,
						fixedChannelStatusData));

			// test dataModeArray
			res = pAES3PCMDesc2->SetUserDataModeAt(count, 16);
			checkExpression(res == AAFRESULT_INVALID_ENUM_VALUE, 
				AAFRESULT_TEST_FAILED);
			checkResult(pAES3PCMDesc2->SetUserDataModeAt(count,
				kAAFUserDataMode_IEC));

			// test fixedUserDataArray
			for(aafUInt8 index = 0; index < SizeOfFixedUserData; ++index) {
				fixedUserData[index] = count + index + 100;
			}
			checkResult(pAES3PCMDesc2->SetFixedUserDataAt(count,
							SizeOfFixedChannelStatusData,
							fixedUserData));
		}
		//TestArrayBasedStuff(channelCount, pAES3PCMDesc2);



		//checkResult(pFile->Save());
		pAES3PCMDesc2->Release();
		pAES3PCMDesc2 = NULL;
		pSoundDesc->Release();
		pSoundDesc = NULL;
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
		pFile->Close();
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
			checkResult(pMob->QueryInterface (IID_IAAFSourceMob, 
											  (void **)&pSourceMob));

			// Get the sound descriptor
			checkResult(pSourceMob->GetEssenceDescriptor(&pEssenceDesc));
			checkResult(pEssenceDesc->QueryInterface(IID_IAAFAES3PCMDescriptor,
				(void **)&pAES3PCMDesc));
			IAAFAES3PCMDescriptor2 * pAES3PCMDesc2 = NULL;
			checkResult(pEssenceDesc->QueryInterface(IID_IAAFAES3PCMDescriptor2,
				(void **)&pAES3PCMDesc2));


			//// test emphasis
			//aafEmphasisType_t emphasisType = kAAFEmphasis_Unknown;
			//checkResult(pAES3PCMDesc2->GetEmphasis(&emphasisType));
			//checkExpression((emphasisType == kAAFEmphasis_ITU), 
			//				AAFRESULT_TEST_FAILED);

			//// test blockStartOffset
			//aafUInt16 blockStartOffset = 0;
			//checkResult(pAES3PCMDesc2->GetBlockStartOffset(&blockStartOffset));
			//checkExpression((blockStartOffset == 65535), 
			//				AAFRESULT_TEST_FAILED);

			//// test auxBitsMode
			//aafAuxBitsModeType_t auxBitsMode = kAAFAuxBitsMode_NotDefined;
			//checkResult(pAES3PCMDesc2->GetAuxBitsMode(&auxBitsMode));
			//checkExpression(auxBitsMode == kAAFAuxBitsMode_Reserved3,
			//				AAFRESULT_TEST_FAILED);

			//// now test array based stuff
			//IAAFSoundDescriptor *pSoundDesc = NULL;
			//pAES3PCMDesc2->QueryInterface(IID_IAAFSoundDescriptor,
			//							  (void **) &pSoundDesc);
			//aafUInt32 channelCount = 0;
			//checkResult(pSoundDesc->GetChannelCount(&channelCount));
			//checkExpression(channelCount == 4, AAFRESULT_TEST_FAILED);
			//hr = TestArrayBasedStuff(channelCount, pAES3PCMDesc2);
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


