// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
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
#include <string.h>
#include <stdlib.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"


#define FORM_HDR_SIZE		12
#define COMM_CHUNK_SIZE		46
#define SSND_CHUNK_SIZE		16
#define SUMMARY_SIZE		FORM_HDR_SIZE+COMM_CHUNK_SIZE+SSND_CHUNK_SIZE

//typedef struct tAIFCSUMMARY
//{
//	FormAIFCChunk	formChunk;		// size 12
//	CommonChunk		commChunk;
//	SoundDataChunk	ssndChunk;
//} AIFCSummary;

// our test Mob id 
static const aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x1f64f50a, 0x03fd, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

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
#define WRITE_LONG(ptr, val) { memcpy(ptr, val, 4); SwapBytes(ptr,4); ptr += 4; }
#define WRITE_SHORT(ptr, val) { memcpy(ptr, val, 4); SwapBytes(ptr,2); ptr += 2; }
#define WRITE_CHARS(ptr, val, len) { memcpy(ptr, val, len); ptr += len; }
#else
#define WRITE_LONG(ptr, val) { memcpy(ptr, val, 4); ptr += 4; }
#define WRITE_SHORT(ptr, val) { memcpy(ptr, val, 4); ptr += 2; }
#define WRITE_CHARS(ptr, val, len) { memcpy(ptr, val, len); ptr += len; }
#endif

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
	ProductInfo.productName = L"AAFAIFCDescriptor Test";
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
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IAAFDictionary*			pDictionary = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFMob*				pMob = NULL;
	IAAFAIFCDescriptor*		pAIFCDesc = NULL;
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
 		
		// Create a source mob
		CAAFBuiltinDefs defs (pDictionary);
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));

		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));

		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"AIFCDescriptorTest"));
		checkResult(defs.cdAIFCDescriptor()->
					CreateInstance(IID_IAAFAIFCDescriptor, 
								   (IUnknown **)&pAIFCDesc));		

//		AIFCSummary summary;
		unsigned char	writeBuf[SUMMARY_SIZE], *writePtr;
		aafUInt32		longVal, lZero = 0, n;
		aafUInt16		shortVal, sZero = 0;
		static char *compressionName = "Not compressed";
		// Form Header
		// typedef struct tAIFCFORMATCHUNK
		// {
		//		char	ckID[4];		// "FORM"
		// 		long	ckDataSize;
		// 		char	formType[4];	// "AIFC"
		// } FormAIFCChunk;
		writePtr = writeBuf;
		WRITE_CHARS(writePtr, "FORM", 4);
		WRITE_CHARS(writePtr, "AIFC", 4);
		longVal = sizeof(writeBuf) - 8;
		WRITE_LONG(writePtr, &longVal);

		// COMM Chunk
		// typedef struct tCommonChunk
		// {
		// 		char			ckID[4];		// "COMM"
		// 		long			ckDataSize;
		// 		short			numChannels;
		// 		unsigned long	numSampleFrames;
		// 		short			sampleSize;
		// 		unsigned char	sampleRate[10];	// This should be IEEE floating point number
		// 		char			compressionType[4];
		// 		unsigned char	compNameLength;
		// 		char			compressionName[15];
		// } CommonChunk;
		WRITE_CHARS(writePtr, "COMM", 4);	// ckID
		longVal = 46;	// sizeof(CommonChunk) with no padding
		WRITE_LONG(writePtr, &longVal);		// ckDataSize
		shortVal = 1;
		WRITE_SHORT(writePtr, &shortVal);	// numChannels
		WRITE_LONG(writePtr, &lZero);		// numSampleFrames
		shortVal = 16;
		WRITE_SHORT(writePtr, &shortVal);	// sampleSize
		*writePtr++ = 0x40;					// SampleRate
		*writePtr++ = 0x0E;
		*writePtr++ = (char)0xac;
		*writePtr++ = 0x44;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		*writePtr++ = 0x0;
		WRITE_CHARS(writePtr, "NONE", 4);	// CompressionType
		*writePtr++ = strlen(compressionName);
		WRITE_CHARS(writePtr, compressionName, strlen(compressionName)); // compressionName
		for(n = 15 - strlen(compressionName); n >= 1; n--)
			*writePtr++ = 0;	// Chunks must not be an odd length

		// Sound Chunk
		// typedef struct tSoundDataChunk
		// {
		// 	char			ckID[4];		// "SSND"
		// 	long			ckDataSize;
		// 	unsigned long	offset;
		// 	unsigned long	blockSize;
		// 	char			soundData;
		// } SoundDataChunk;
		WRITE_CHARS(writePtr, "SSND", 4);		// ckID
		WRITE_LONG(writePtr, &lZero);			// ckDataSize
		WRITE_LONG(writePtr, &lZero);			// offset
		WRITE_LONG(writePtr, &lZero);			// blockSize
		// Check that writePtr-writeBuf == sizeof(writeBuf);

		// NOTE: The elements in the summary structure need to be byte swapped
		// on Big Endian system (i.e. the MAC).
//		SWAPSUMMARY(summary)
		checkResult(pAIFCDesc->SetSummary(SUMMARY_SIZE, (aafDataValue_t)writeBuf));

		checkResult(pAIFCDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
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

	if (pAIFCDesc)
		pAIFCDesc->Release();

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
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFEssenceDescriptor*	pEssDesc = NULL;
	IAAFAIFCDescriptor*		pAIFCDesc = NULL;
	aafNumSlots_t			numMobs = 0;
	HRESULT					hr = AAFRESULT_SUCCESS;


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
		
		// if there is an Essence Descriptor then it MUST be an (essence) AIFC Descriptor
		checkResult(pEssDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **) &pAIFCDesc));

		unsigned char summary[SUMMARY_SIZE];
	    aafUInt32		size = 0;

		checkResult(pAIFCDesc->GetSummaryBufferSize(&size));
		checkExpression(size == sizeof(summary), AAFRESULT_TEST_FAILED);


		checkResult(pAIFCDesc->GetSummary(size, (aafDataValue_t)&summary));
	
		checkExpression((strncmp((char*)summary+FORM_HDR_SIZE, "COMM", 4) == 0) &&
						(strncmp((char*)summary, "FORM", 4) == 0) &&
						(strncmp((char*)summary+FORM_HDR_SIZE+COMM_CHUNK_SIZE, "SSND", 4) == 0),
		                AAFRESULT_TEST_FAILED);
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
  
	// Cleanup and return
	if (pAIFCDesc)
		pAIFCDesc->Release();

  if (pEssDesc)
		pEssDesc->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (pMob)
		pMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

extern "C" HRESULT CAAFAIFCDescriptor_test(testMode_t mode);
extern "C" HRESULT CAAFAIFCDescriptor_test(testMode_t mode)
{
	aafWChar*	pFileName = L"AAFAIFCDescriptorTest.aaf";
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
		cerr << "CAAFAIFCDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}








