// @doc INTERNAL
// @com This file implements the module test for CAAFWAVEDescriptor
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


#undef WIN32_LEAN_AND_MEAN


#include "AAF.h"

#include <iostream.h>
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


#if defined(_WIN32)
#define WRITE_LONG(ptr, val) { memcpy(ptr, val, 4); ptr += 4; }
#define WRITE_SHORT(ptr, val) { memcpy(ptr, val, 2); ptr += 2; }
#define WRITE_CHARS(ptr, val, len) { memcpy(ptr, val, len); ptr += len; }
#define READ_LONG(ptr, val) { memcpy(val, ptr, 4); ptr += 4; }
#define READ_SHORT(ptr, val) { memcpy(val, ptr, 2); ptr += 2; }
#define READ_CHARS(ptr, val, len) { memcpy(val, ptr, len); ptr += len; }
#else
#define WRITE_LONG(ptr, val) { memcpy(ptr, val, 4); SwapBytes(ptr,4); ptr += 4; }
#define WRITE_SHORT(ptr, val) { memcpy(ptr, val, 2); SwapBytes(ptr,2); ptr += 2; }
#define WRITE_CHARS(ptr, val, len) { memcpy(ptr, val, len); ptr += len; }
#define READ_LONG(ptr, val) { memcpy(val, ptr, 4); SwapBytes(val,4); ptr += 4; }
#define READ_SHORT(ptr, val) { memcpy(val, ptr, 2); SwapBytes(val,2); ptr += 2; }
#define READ_CHARS(ptr, val, len) { memcpy(val, ptr, len); ptr += len; }
#endif


static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x09273e8e, 0x0406, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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
	ProductInfo.productName = L"AAFWAVEDescriptor Test";
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
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));

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
		WRITE_SHORT(writePtr, &shortVal);	// wFormatTag
		shortVal = 1;
		WRITE_SHORT(writePtr, &shortVal);	// nChannels
		longVal = 44100;
		WRITE_LONG(writePtr, &longVal);	// nSamplesPerSec
		longVal = 88200;
		WRITE_LONG(writePtr, &longVal);	// nAvgBytesPerSec
		shortVal = 2;
		WRITE_SHORT(writePtr, &shortVal);	// nBlockAlign
		shortVal = 16;
		WRITE_SHORT(writePtr, &shortVal);	// wBitsPerSample
		shortVal = 0;
		WRITE_SHORT(writePtr, &shortVal);	// cbSize

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
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));

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


		readPtr = readBuf;
		READ_SHORT(readPtr, &shortVal);	// wFormatTag
		checkExpression(shortVal == WAVE_FORMAT_PCM, AAFRESULT_TEST_FAILED);
		READ_SHORT(readPtr, &shortVal);	// nChannels
		checkExpression(shortVal == 1, AAFRESULT_TEST_FAILED);
		READ_LONG(readPtr, &longVal);	// nSamplesPerSec
		checkExpression(longVal == 44100, AAFRESULT_TEST_FAILED);
		READ_LONG(readPtr, &longVal);	// nAvgBytesPerSec
		checkExpression(longVal == 88200, AAFRESULT_TEST_FAILED);
		READ_SHORT(readPtr, &shortVal);	// nBlockAlign
		checkExpression(shortVal == 2, AAFRESULT_TEST_FAILED);
		READ_SHORT(readPtr, &shortVal);	// wBitsPerSample
		checkExpression(shortVal == 16, AAFRESULT_TEST_FAILED);
		READ_SHORT(readPtr, &shortVal);	// cbSize
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

extern "C" HRESULT CAAFWAVEDescriptor_test(testMode_t mode);
extern "C" HRESULT CAAFWAVEDescriptor_test(testMode_t mode)
{
	aafWChar*	pFileName = L"AAFWAVEDescriptorTest.aaf";
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

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
		cerr << "CAAFWAVEDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}








