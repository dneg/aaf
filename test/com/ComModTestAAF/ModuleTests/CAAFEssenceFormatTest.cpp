// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceFormat
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFCodecDefs.h"
#include "AAFContainerDefs.h"
#include "AAFDefUIDs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"

#define	MobName			L"MasterMOBTest"
//#define	NumMobSlots		3

//static aafWChar* Manufacturer = L"Sony";
//static aafWChar* Model = L"MyModel";
//static aafTapeCaseType_t FormFactor = kAAFVHSVideoTape;
//static aafVideoSignalType_t VideoSignalType = kAAFPALSignal;
//static aafTapeFormatType_t TapeFormat = kAAFVHSFormat;
//static aafLength_t TapeLength = 3200 ;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xafc51ab6, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

//#define TAPE_MOB_OFFSET	10
//#define TAPE_MOB_LENGTH	60
//#define TAPE_MOB_NAME	L"A Tape Mob"

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

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFEssenceFormat Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	*ppFile = NULL;
	
	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;
		
	case kAAFMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;
		
	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}
	
	if (FAILED(hr))
	{
		if (*ppFile)
		{
			(*ppFile)->Release();
			*ppFile = NULL;
		}
		return hr;
	}
	
	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*			pFile = NULL;
	bool				bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*		pDictionary = NULL;
	IAAFMob*			pMob = NULL;
	IAAFMasterMob*		pMasterMob = NULL;
	IAAFEssenceAccess	*pAccess = NULL;
	IAAFEssenceFormat	*pFormat = NULL;
	HRESULT				hr = S_OK;
	aafRational_t		rate = { 44100, 1 };
	aafInt32			numSpec, writeValue, readValue, bytesRead;
	aafRational_t		writeRat, readRat;
	aafUID_t			readCode;

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, /*&pSession,*/ &pFile, &pHeader));
		bFileOpen = true;
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
				
		// Create a Master Mob
		checkResult(defs.cdMasterMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		// Set the IAAFMob properties
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(MobName));
		
		checkResult(pMob->QueryInterface(IID_IAAFMasterMob, (void **) &pMasterMob));
		// Add the master mob to the file BEFORE creating the essence
		checkResult(pHeader->AddMob(pMob));
		checkResult(pMasterMob->CreateEssence (1,
											   defs.ddSound(),
											   kAAFCodecWAVE,
											   rate,
											   rate,
											   kAAFCompressionDisable,
											   NULL,
											   ContainerAAF,
											   &pAccess));
		
		// Fianlly! Get an essence format to test
		checkResult(pAccess->GetEmptyFileFormat(&pFormat));
		checkResult(pFormat->NumFormatSpecifiers(&numSpec));
		checkExpression(0 == numSpec, AAFRESULT_TEST_FAILED);
		
		/***/
		writeValue = 16;
		checkResult(pFormat->AddFormatSpecifier(kAAFAudioSampleBits,
												sizeof(writeValue), (aafDataBuffer_t)&writeValue));
		writeValue = 1;
		checkResult(pFormat->AddFormatSpecifier(kAAFNumChannels,
												sizeof(writeValue), (aafDataBuffer_t)&writeValue));
		writeRat.numerator = 44100;
		writeRat.denominator = 1;
		checkResult(pFormat->AddFormatSpecifier(kAAFSampleRate,
												sizeof(writeRat), (aafDataBuffer_t)&writeRat));
		/***/
		checkResult(pFormat->NumFormatSpecifiers(&numSpec));
		checkExpression(3 == numSpec, AAFRESULT_TEST_FAILED);

		/***/
		checkResult(pFormat->GetFormatSpecifier (kAAFAudioSampleBits,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead));
		checkExpression(sizeof(readValue) == bytesRead, AAFRESULT_TEST_FAILED);
		checkExpression(16 == readValue, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pFormat->GetFormatSpecifier (kAAFNumChannels,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead));
		checkExpression(sizeof(readValue) == bytesRead, AAFRESULT_TEST_FAILED);
		checkExpression(1 == readValue, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pFormat->GetFormatSpecifier (kAAFSampleRate,
  												sizeof(readRat), (aafDataBuffer_t)&readRat,
												&bytesRead));
		checkExpression(sizeof(readRat) == bytesRead, AAFRESULT_TEST_FAILED);
		checkExpression(44100 == readRat.numerator, AAFRESULT_TEST_FAILED);
		checkExpression(1 == readRat.denominator, AAFRESULT_TEST_FAILED);
		/***/

		//******* Test indexed calls
		/***/
		checkResult(pFormat->GetIndexedFormatSpecifier (0, &readCode,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead));
		checkExpression(memcmp(&readCode,&kAAFAudioSampleBits, sizeof(readCode)) == 0, AAFRESULT_TEST_FAILED);
		checkExpression(sizeof(readValue) == bytesRead, AAFRESULT_TEST_FAILED);
		checkExpression(16 == readValue, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pFormat->GetIndexedFormatSpecifier (1, &readCode,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead));
		checkExpression(memcmp(&readCode,&kAAFNumChannels, sizeof(readCode)) == 0, AAFRESULT_TEST_FAILED);
		checkExpression(sizeof(readValue) == bytesRead, AAFRESULT_TEST_FAILED);
		checkExpression(1 == readValue, AAFRESULT_TEST_FAILED);
		/***/
		checkResult(pFormat->GetIndexedFormatSpecifier (2, &readCode,
  												sizeof(readRat), (aafDataBuffer_t)&readRat,
												&bytesRead));
		checkExpression(memcmp(&readCode,&kAAFSampleRate, sizeof(readCode)) == 0, AAFRESULT_TEST_FAILED);
		checkExpression(sizeof(readRat) == bytesRead, AAFRESULT_TEST_FAILED);
		checkExpression(44100 == readRat.numerator, AAFRESULT_TEST_FAILED);
		checkExpression(1 == readRat.denominator, AAFRESULT_TEST_FAILED);
		/***/
		//******* Test indexed calls out of range
		checkExpression(pFormat->GetIndexedFormatSpecifier (-1, &readCode,
  												sizeof(readRat), (aafDataBuffer_t)&readRat,
												&bytesRead) == AAFRESULT_FORMAT_BOUNDS, AAFRESULT_TEST_FAILED);
		checkExpression(pFormat->GetIndexedFormatSpecifier (3, &readCode,
  												sizeof(readRat), (aafDataBuffer_t)&readRat,
												&bytesRead) == AAFRESULT_FORMAT_BOUNDS, AAFRESULT_TEST_FAILED);
		//******* Test indexed calls small buffer size
		checkExpression(pFormat->GetIndexedFormatSpecifier (2, &readCode,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead) == AAFRESULT_SMALLBUF, AAFRESULT_TEST_FAILED);
		//******* Test ID calls small buffer size
		checkExpression(pFormat->GetFormatSpecifier (kAAFSampleRate,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead) == AAFRESULT_SMALLBUF, AAFRESULT_TEST_FAILED);
		//******* Test ID calls missing ID
		checkExpression(pFormat->GetFormatSpecifier (kAAFSampleFormat,
  												sizeof(readValue), (aafDataBuffer_t)&readValue,
												&bytesRead) == AAFRESULT_FORMAT_NOT_FOUND, AAFRESULT_TEST_FAILED);

		pAccess->CompleteWrite();
		pAccess->Release();
		pAccess = NULL;

	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return	
	if (pAccess)
		pAccess->Release();
	
	if (pMasterMob)
		pMasterMob->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFormat)
		pFormat->Release();
	
	if (pFile)
	{  // Close file
		if (bFileOpen)
		{
			pFile->Save();
			pFile->Close();
		}
		pFile->Release();
	}
	
	return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
	IAAFFile*		pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader*		pHeader = NULL;
	HRESULT			hr = S_OK;
	
	
	
	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		bFileOpen = true;
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup and return
	if (pHeader)
		pHeader->Release();
	
	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	
	return hr;
}


extern "C" HRESULT CAAFEssenceFormat_test(testMode_t mode);
extern "C" HRESULT CAAFEssenceFormat_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFEssenceFormatTest.aaf";
	
	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
			
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFEssenceFormat_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IAAFEssenceFormat tests have not been implemented:" << endl; 
//		cout << "     GetIndexedFormatSpecifier" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}
	
	return hr;
}
























