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
// The Original Code of this file is Copyright 1998-2008, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#undef WIN32_LEAN_AND_MEAN


#include "AAF.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#if !defined( OS_WINDOWS )

#define WAVE_FORMAT_PCM 0x0001

//typedef struct tWAVEFORMATEX
//{
//    WORD        wFormatTag;         /* format type */
//    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
 //   DWORD       nSamplesPerSec;     /* sample rate */
//    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
//    WORD        nBlockAlign;        /* block size of data */
//    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
//    WORD        cbSize;             /* the count in bytes of the size of */
//				    /* extra information (after cbSize) */
//} WAVEFORMATEX, *PWAVEFORMATEX;


#endif  // !defined( OS_WINDOWS )


// Simple utilities to swap bytes.
static void SwapBytes(void *buffer, size_t count)
{
  unsigned char *pBuffer = (unsigned char *)buffer;
  unsigned char tmp;
  int front = 0;
  int back = count - 1;

  for (front = 0, back = count - 1; front < back; ++front, --back)
  {
    tmp = pBuffer[front];
    pBuffer[front] = pBuffer[back];
    pBuffer[back] = tmp;
  }
}

// The WAVE header is stored on disk in little endian byte order
// so provide some macros to swap byte order when necessary
#define LE_WRITE_LONG(bo, ptr, val) { memcpy(ptr, val, 4); if (bo == kAAFByteOrderBig) SwapBytes(ptr,4); ptr += 4; }
#define LE_WRITE_SHORT(bo, ptr, val) { memcpy(ptr, val, 2); if (bo == kAAFByteOrderBig) SwapBytes(ptr,2); ptr += 2; }
#define LE_READ_LONG(bo, ptr, val) { memcpy(val, ptr, 4); if (bo == kAAFByteOrderBig) SwapBytes(val,4); ptr += 4; }
#define LE_READ_SHORT(bo, ptr, val) { memcpy(val, ptr, 2); if (bo == kAAFByteOrderBig) SwapBytes(val,2); ptr += 2; }

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x09273e8e, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};


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
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFSourceMob*	pSourceMob = NULL;
	IAAFMob*	pMob = NULL;
  IAAFWAVEDescriptor*	pWAVEDesc = NULL;
  IAAFEssenceDescriptor*	pEssDesc = NULL;
	HRESULT			hr = AAFRESULT_SUCCESS;


  try
  {
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));

		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);

		// Create a source mob
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));
		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));

		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"WAVEDescriptorTest"));
		checkResult(defs.cdWAVEDescriptor()->
					CreateInstance(IID_IAAFWAVEDescriptor, 
								   (IUnknown **)&pWAVEDesc));		

		// Get Endianness so we can store the corrent binary sequence to disk
		IAAFEndian* pEndian = NULL;
		checkResult(pHeader->QueryInterface(IID_IAAFEndian, (void **)&pEndian));
		eAAFByteOrder_t byteorder;
		checkResult(pEndian->GetNativeByteOrder(&byteorder));
		pEndian->Release();
		pEndian = NULL;

		unsigned char writeBuf[18], *writePtr;
		aafInt16	shortVal;
		aafInt32	longVal;

		//typedef struct tWAVEFORMATEX
		//{
		//    WORD        wFormatTag;         /* format type */
		//    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
		//	  DWORD       nSamplesPerSec;     /* sample rate */
		//    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
		//    WORD        nBlockAlign;        /* block size of data */
		//    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
		//    WORD        cbSize;             /* the count in bytes of the size of */
		//				    /* extra information (after cbSize) */
		//} WAVEFORMATEX, *PWAVEFORMATEX;

		writePtr = writeBuf;
		shortVal = WAVE_FORMAT_PCM;
		LE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// wFormatTag
		shortVal = 1;
		LE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// nChannels
		longVal = 44100;
		LE_WRITE_LONG(byteorder, writePtr, &longVal);	// nSamplesPerSec
		longVal = 88200;
		LE_WRITE_LONG(byteorder, writePtr, &longVal);	// nAvgBytesPerSec
		shortVal = 2;
		LE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// nBlockAlign
		shortVal = 16;
		LE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// wBitsPerSample
		shortVal = 0;
		LE_WRITE_SHORT(byteorder, writePtr, &shortVal);	// cbSize

		checkResult(pWAVEDesc->SetSummary(sizeof(writeBuf), (aafDataValue_t)writeBuf));

    checkResult(pWAVEDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));

		// Add the MOB to the file
		checkResult(pHeader->AddMob(pMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	
  
  // Cleanup and return
  if (pEssDesc)
    pEssDesc->Release();

  if (pWAVEDesc)
    pWAVEDesc->Release();

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
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IEnumAAFMobs*	pMobIter = NULL;
	IAAFMob*	pMob = NULL;
	IAAFSourceMob*	pSourceMob = NULL;
	IAAFEssenceDescriptor*	pEssDesc = NULL;
	IAAFWAVEDescriptor*	pWAVEDesc = NULL;
	aafNumSlots_t	numMobs = 0;
	HRESULT			hr = AAFRESULT_SUCCESS;


  try
  {
	  // Open the AAF file
	  checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

	  // Get the AAF file header.
	  checkResult(pFile->GetHeader(&pHeader));

	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->GetMobs(NULL, &pMobIter));
		checkResult(pMobIter->NextOne(&pMob));
		checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
		
    // Back into testing mode
		checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
		
    // if there is an Essence Descriptor then it MUST be an (essence) WAVE Descriptor
		checkResult(pEssDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **) &pWAVEDesc));

		unsigned char readBuf[18], *readPtr;
		aafInt16	shortVal;
		aafInt32	longVal;
		  aafUInt32		size = 0;

		checkResult(pWAVEDesc->GetSummaryBufferSize(&size));


		checkResult(pWAVEDesc->GetSummary(sizeof(readBuf), (aafDataValue_t)readBuf));

		// Get Endianness so we can read the corrent binary sequence from disk
		IAAFEndian* pEndian = NULL;
		checkResult(pHeader->QueryInterface(IID_IAAFEndian, (void **)&pEndian));
		eAAFByteOrder_t byteorder;
		checkResult(pEndian->GetNativeByteOrder(&byteorder));
		pEndian->Release();
		pEndian = NULL;


		readPtr = readBuf;
		LE_READ_SHORT(byteorder, readPtr, &shortVal);	// wFormatTag
		checkExpression(shortVal == WAVE_FORMAT_PCM, AAFRESULT_TEST_FAILED);
		LE_READ_SHORT(byteorder, readPtr, &shortVal);	// nChannels
		checkExpression(shortVal == 1, AAFRESULT_TEST_FAILED);
		LE_READ_LONG(byteorder, readPtr, &longVal);	// nSamplesPerSec
		checkExpression(longVal == 44100, AAFRESULT_TEST_FAILED);
		LE_READ_LONG(byteorder, readPtr, &longVal);	// nAvgBytesPerSec
		checkExpression(longVal == 88200, AAFRESULT_TEST_FAILED);
		LE_READ_SHORT(byteorder, readPtr, &shortVal);	// nBlockAlign
		checkExpression(shortVal == 2, AAFRESULT_TEST_FAILED);
		LE_READ_SHORT(byteorder, readPtr, &shortVal);	// wBitsPerSample
		checkExpression(shortVal == 16, AAFRESULT_TEST_FAILED);
		LE_READ_SHORT(byteorder, readPtr, &shortVal);	// cbSize
		checkExpression(shortVal == 0, AAFRESULT_TEST_FAILED);
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }
	
  
  // Cleanup and return
  if (pEssDesc)
    pEssDesc->Release();

  if (pWAVEDesc)
    pWAVEDesc->Release();

  if (pMob)
    pMob->Release();

  if (pMobIter)
    pMobIter->Release();

  if (pSourceMob)
    pSourceMob->Release();

	if (pHeader)
    pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

extern "C" HRESULT CAAFWAVEDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFWAVEDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t	fileNameBufLen = 128;
	aafWChar 	pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFWAVEDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}








