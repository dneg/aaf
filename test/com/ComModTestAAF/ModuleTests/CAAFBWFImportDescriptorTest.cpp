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
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFPropertyDefs.h"

#include "CAAFBuiltinDefs.h"


static const aafUInt32 chunkID = 1;
static const aafUInt32 chunkID2 = 2;
static const aafUInt32 chunkLength= 25;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x2DFB8BAF, 0x0972, 0x11d4, {0xA3, 0x57, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6A}}};

static const 	aafMobID_t	TEST_referencedMobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x2DFB8BB0, 0x0972, 0x11d4, {0xA3, 0x57, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6A}}};


#define TEST_FileSecurityReport		256
#define TEST_FileSecurityWave		128
#define TEST_CodingHistory		L"CodingHistory test"
#define TEST_BasicData			L"BasicData test"
#define TEST_StartOfModulation	L"StartOfModulation test"
#define TEST_QualityEvent		L"QualityEvent test"
#define TEST_EndOfModulation	L"EndOfModulation test"
#define TEST_QualityParameter	L"QualityParameter test"
#define TEST_OperatorComment	L"OperatorComment test"
#define TEST_CueSheet			L"CueSheet test"


static const char RIFFChunksmiley[] =        /* 16x16 smiley face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "****************"
  "****************"
  " ** ******** ** "
  " *** ****** *** "
  "  ***  **  ***  "
  "   ****  ****   "
  "    ********    "
  "      ****     ";

static const char RIFFChunkfrowney[] =        /* 16x16 frowney face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "*******  *******"
  "*****  **  *****"
  " *** ****** *** "
  " ** ******** ** "
  "  ************  "
  "   **********   "
  "    ********    "
  "      ****     ";


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
	IAAFFile *			pFile = NULL;
	bool				bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMob*			pMob = NULL;
	IAAFRIFFChunk*		pRIFFChunk = NULL;
	IAAFRIFFChunk*		pRIFFChunk2 = NULL;
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFBWFImportDescriptor*			pBWFImportDesc = NULL;
	aafUInt32			testNum;
	HRESULT				hr = S_OK;



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
 		
		checkResult(defs.cdSourceMob()->
			  CreateInstance(IID_IAAFSourceMob, 
							 (IUnknown **)&pSourceMob));
		pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
	
		pMob->SetMobID(TEST_MobID);
		pMob->SetName(L"BWFImportDescriptorTest");
		
		//Create BWFImportDescriptor to hold the RIFFChunk
		checkResult( defs.cdBWFImportDescriptor()->
					  CreateInstance(IID_IAAFBWFImportDescriptor, (IUnknown **)&pBWFImportDesc));
					 
		// Create RIFFChunks and append them to BWFImportDescriptor

		checkResult(defs.cdRIFFChunk()->CreateInstance(IID_IAAFRIFFChunk, 
							 (IUnknown **)&pRIFFChunk));
		checkResult(defs.cdRIFFChunk()->CreateInstance(IID_IAAFRIFFChunk, 
							 (IUnknown **)&pRIFFChunk2));
							 
		checkResult(pBWFImportDesc->AppendUnknownBWFChunk(pRIFFChunk));
		checkResult(pBWFImportDesc->AppendUnknownBWFChunk(pRIFFChunk2));
		checkExpression(pBWFImportDesc->AppendUnknownBWFChunk(pRIFFChunk)==AAFRESULT_OBJECT_ALREADY_ATTACHED, AAFRESULT_TEST_FAILED);
		checkResult(pBWFImportDesc->SetFileSecurityReport(TEST_FileSecurityReport));
		checkResult(pBWFImportDesc->SetFileSecurityWave(TEST_FileSecurityWave));
		checkResult(pBWFImportDesc->SetCodingHistory(TEST_CodingHistory));
		checkResult(pBWFImportDesc->SetBasicData(TEST_BasicData));
		checkResult(pBWFImportDesc->SetStartOfModulation(TEST_StartOfModulation));
		checkResult(pBWFImportDesc->SetQualityEvent(TEST_QualityEvent));
		checkResult(pBWFImportDesc->SetEndOfModulation(TEST_EndOfModulation));
		checkResult(pBWFImportDesc->SetQualityParameter(TEST_QualityParameter));	
		checkResult(pBWFImportDesc->SetOperatorComment(TEST_OperatorComment));
		checkResult(pBWFImportDesc->SetCueSheet(TEST_CueSheet));
						 
		checkResult( pBWFImportDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));
		
		//Add the MOB to the file
		checkResult(pHeader->AddMob(pMob));
		
		testNum = 0;
		checkResult(pRIFFChunk->Initialize(chunkID));
		checkResult(pRIFFChunk->Write(sizeof(RIFFChunksmiley), (unsigned char *)RIFFChunksmiley, &testNum));
		checkExpression(testNum == sizeof(RIFFChunksmiley), AAFRESULT_TEST_FAILED);
		testNum = 0;
		checkResult(pRIFFChunk2->Initialize(chunkID2));
		checkResult(pRIFFChunk2->Write(sizeof(RIFFChunkfrowney), (unsigned char *)RIFFChunkfrowney, &testNum));
		checkExpression(testNum == sizeof(RIFFChunkfrowney), AAFRESULT_TEST_FAILED);
		pFile->Save();

		pEssDesc->Release();
		pEssDesc = NULL;		
		pBWFImportDesc->Release();
		pBWFImportDesc = NULL;
		
		pRIFFChunk->Release();
		pRIFFChunk = NULL;
		
		pRIFFChunk2->Release();
		pRIFFChunk2 = NULL;
	
		pMob->Release();
		pMob = NULL;
	
		pSourceMob->Release();
		pSourceMob = NULL;

		pDictionary->Release();
		pDictionary = NULL;

		pHeader->Release();
		pHeader = NULL;


		pFile->Close();	
		pFile->Release();
		pFile = NULL;
		
	return hr;

}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*				pDictionary = NULL;
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFBWFImportDescriptor*	pBWFImportDesc = NULL;
	IEnumAAFRIFFChunks*			pEnum = NULL;
	IAAFRIFFChunk*				pRIFFChunk = NULL;
	IAAFRIFFChunk*				pRIFFChunkTest = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	aafUInt32					numData, bytesRead, com, testNum;
	aafLength_t					testRIFFLen;
	aafNumSlots_t				numMobs;
	char						Value[sizeof(RIFFChunksmiley)];
	char						Value2[sizeof(RIFFChunkfrowney)];
	HRESULT						hr = AAFRESULT_SUCCESS;
	wchar_t						testString[256];

	checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

		checkResult( pFile->GetHeader(&pHeader));
		// Get the AAF Dictionary so that we can create a fake RIFFChunk to test RemoveUnknownBWFChunks.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		checkResult(defs.cdRIFFChunk()->CreateInstance(IID_IAAFRIFFChunk, 
							 (IUnknown **)&pRIFFChunkTest));
		
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		if (1 == numMobs )
		{
			checkResult(pHeader->GetMobs(NULL, &pMobIter));
			checkResult(pMobIter->NextOne(&pMob));
			checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
			
			// Back into testing mode
			checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
			checkResult( pEssDesc->QueryInterface( IID_IAAFBWFImportDescriptor, (void**)&pBWFImportDesc ));	 
					
			checkResult(pBWFImportDesc->GetFileSecurityReport(&testNum));
			checkExpression(testNum==TEST_FileSecurityReport, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetFileSecurityWave(&testNum));
			checkExpression(testNum==TEST_FileSecurityWave, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetCodingHistory(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_CodingHistory) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetBasicData(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_BasicData) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetStartOfModulation(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_StartOfModulation) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetQualityEvent(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_QualityEvent) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetEndOfModulation(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_EndOfModulation) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetQualityParameter(testString, sizeof(testString)));	
			checkExpression(wcscmp(testString, TEST_QualityParameter) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetOperatorComment(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_OperatorComment) == 0, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetCueSheet(testString, sizeof(testString)));
			checkExpression(wcscmp(testString, TEST_CueSheet) == 0, AAFRESULT_TEST_FAILED);
			
			checkResult(pBWFImportDesc->CountUnknownBWFChunks(&numData));
			checkExpression(2 == numData, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->GetUnknownBWFChunks(&pEnum));

			for(com = 0; com < numData; com++)
			{	
				checkResult(pEnum->NextOne(&pRIFFChunk));	
				pRIFFChunk->GetLength(&testRIFFLen);
				checkExpression(testRIFFLen!=chunkLength /*huh?*/, AAFRESULT_TEST_FAILED);
				checkResult(pRIFFChunk->GetLength(&testRIFFLen));
				pRIFFChunk->GetChunkID(&testNum);
				
				if (testNum==1){
					checkExpression(sizeof(RIFFChunksmiley) == testRIFFLen, AAFRESULT_TEST_FAILED);
					checkResult(pRIFFChunk->Read( sizeof(Value), (unsigned char *)Value, &bytesRead));
					checkExpression(memcmp(Value, RIFFChunksmiley, sizeof(RIFFChunksmiley)) == 0, AAFRESULT_TEST_FAILED);						
				}
				else if (testNum==2){
					checkExpression(sizeof(RIFFChunkfrowney) == testRIFFLen, AAFRESULT_TEST_FAILED);
					checkResult(pRIFFChunk->Read( sizeof(Value2), (unsigned char *)Value2, &bytesRead));
					checkExpression(memcmp(Value2, RIFFChunkfrowney, sizeof(RIFFChunkfrowney)) == 0, AAFRESULT_TEST_FAILED);
				}
				pRIFFChunk->Release();
				pRIFFChunk = NULL;
				
			}
			checkResult(pEnum->Reset());
		  	checkResult(pEnum->NextOne(&pRIFFChunk));
			checkExpression((pBWFImportDesc->RemoveUnknownBWFChunkAt(2))==AAFRESULT_BADINDEX, AAFRESULT_TEST_FAILED);
			checkResult(pBWFImportDesc->RemoveUnknownBWFChunkAt(0));
			checkResult(pBWFImportDesc->CountUnknownBWFChunks(&numData));
			checkExpression(1 == numData, AAFRESULT_TEST_FAILED);
		
			//pRIFFChunk->Release();
			pRIFFChunk = NULL;
			pRIFFChunkTest->Release();
			pRIFFChunkTest = NULL;
			pEnum->Release();
			pEnum = NULL;
			pEssDesc->Release();
			pEssDesc = NULL;
								
							
			pSourceMob->Release();
			pSourceMob = NULL;
				
			pMobIter->Release();
			pMobIter = NULL;
	
			
		}
		pMob->Release();
		pMob = NULL;
		
		pHeader->Release();
		pHeader = NULL;
		
		pFile->Close();
    	pFile->Release();
		pFile = NULL;

	return hr;
}

extern "C" HRESULT CAAFBWFImportDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFBWFImportDescriptor_test(
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
		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "BWFImportDescriptor_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}

    
    
    
    
    


