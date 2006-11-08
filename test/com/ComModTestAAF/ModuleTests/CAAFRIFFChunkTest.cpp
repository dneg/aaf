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


static const aafUInt32 chunkID = 3;
static const aafUInt32 chunkLength= 25;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x2DFB8BAF, 0x0972, 0x11d4, {0xA3, 0x57, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6A}}};

// {2DFB8BB2-0972-11d4-A357-009027DFCA6A}
static const aafUID_t TEST_RIFFChunk = 
{ 0x2dfb8bb2, 0x972, 0x11d4, { 0xa3, 0x57, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

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
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFBWFImportDescriptor*			pBWFImportDesc = NULL;
	aafUInt32			byteswritten;
	aafUInt32			bBytesWrote = 0;
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
		pMob->SetName(L"RIFFChunkTest");
		
		//Create BWFImportDescriptor to hold the RIFFChunk
		checkResult( defs.cdBWFImportDescriptor()->
					  CreateInstance(IID_IAAFBWFImportDescriptor, (IUnknown **)&pBWFImportDesc));
					 
		// Create RIFFChunk and add it to BWFImportDescriptor

		checkResult(defs.cdRIFFChunk()->CreateInstance(IID_IAAFRIFFChunk, 
							 (IUnknown **)&pRIFFChunk));
		checkResult(pBWFImportDesc->AppendUnknownBWFChunk(pRIFFChunk));
								 
		checkResult( pBWFImportDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));
		
		//Add the MOB to the file
		checkResult(pHeader->AddMob(pMob));

		bBytesWrote = 0;
		checkResult(pRIFFChunk->Initialize(chunkID));
		checkResult(pRIFFChunk->Write(sizeof(RIFFChunksmiley), (unsigned char *)RIFFChunksmiley, &bBytesWrote));
		byteswritten = bBytesWrote;
		bBytesWrote = 0;
		checkResult(pRIFFChunk->Write(sizeof(RIFFChunkfrowney), (unsigned char *)RIFFChunkfrowney, &bBytesWrote));
		byteswritten += bBytesWrote;
		checkExpression(byteswritten == sizeof(RIFFChunksmiley) + sizeof(RIFFChunkfrowney), AAFRESULT_TEST_FAILED);
		
		pFile->Save();

		pEssDesc->Release();
		pEssDesc = NULL;		
		pBWFImportDesc->Release();
		pBWFImportDesc = NULL;
		
		pRIFFChunk->Release();
		pRIFFChunk = NULL;
	
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
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFBWFImportDescriptor*	pBWFImportDesc = NULL;
	IEnumAAFRIFFChunks*		pEnum = NULL;
	IAAFRIFFChunk*				pRIFFChunk = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	aafUInt32		numData, bytesRead, com, testID;
	aafLength_t 				testLength, testRIFFLen;
	aafPosition_t				dataPos;
	aafNumSlots_t				numMobs;
	char			Value[sizeof(RIFFChunksmiley)];
	char			Value2[sizeof(RIFFChunkfrowney)];
	HRESULT						hr = AAFRESULT_SUCCESS;

	checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

		checkResult( pFile->GetHeader(&pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		if (1 == numMobs )
		{
			checkResult(pHeader->GetMobs(NULL, &pMobIter));
			checkResult(pMobIter->NextOne(&pMob));
			checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
			
			// Back into testing mode
			checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
			checkResult( pEssDesc->QueryInterface( IID_IAAFBWFImportDescriptor, (void**)&pBWFImportDesc ));	 
			checkResult(pBWFImportDesc->CountUnknownBWFChunks(&numData));
			
			checkExpression(1 == numData, AAFRESULT_TEST_FAILED);
			
			checkResult(pBWFImportDesc->GetUnknownBWFChunks(&pEnum));
			
			for(com = 0; com < numData; com++)
			{	
				checkResult(pEnum->NextOne(&pRIFFChunk));
				
				checkResult(pRIFFChunk->GetLength(&testRIFFLen));
				checkExpression((sizeof(RIFFChunksmiley)+sizeof(RIFFChunkfrowney)) == testRIFFLen, AAFRESULT_TEST_FAILED);
				checkResult(pRIFFChunk->Read( sizeof(Value), (unsigned char *)Value, &bytesRead));
				checkExpression(memcmp(Value, RIFFChunksmiley, sizeof(RIFFChunksmiley)) == 0, AAFRESULT_TEST_FAILED);
				checkResult(pRIFFChunk->Read( sizeof(Value2), (unsigned char *)Value2, &bytesRead));
				checkExpression(memcmp(Value2, RIFFChunkfrowney, sizeof(RIFFChunkfrowney)) == 0, AAFRESULT_TEST_FAILED);				
				
				checkResult(pRIFFChunk->GetPosition(&dataPos));
				checkExpression(dataPos==(sizeof(RIFFChunksmiley)+sizeof(RIFFChunkfrowney)), AAFRESULT_TEST_FAILED);
				checkResult(pRIFFChunk->SetPosition(0));
				checkResult(pRIFFChunk->Read( sizeof(Value), (unsigned char *)Value, &bytesRead));
				checkExpression(memcmp(Value, RIFFChunksmiley, sizeof(RIFFChunksmiley)) == 0, AAFRESULT_TEST_FAILED);
				
				pRIFFChunk->GetChunkID(&testID);	
				checkExpression(testID==chunkID, AAFRESULT_TEST_FAILED);
				pRIFFChunk->GetLength(&testLength);	
				checkExpression(testLength == sizeof(RIFFChunksmiley) + sizeof(RIFFChunkfrowney), AAFRESULT_TEST_FAILED);
				pRIFFChunk->Release();
				pRIFFChunk = NULL;
			}
			
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

extern "C" HRESULT CAAFRIFFChunk_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFRIFFChunk_test(
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
		cerr << "CAAFRIFFChunk_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}

    
    
    
    
    


