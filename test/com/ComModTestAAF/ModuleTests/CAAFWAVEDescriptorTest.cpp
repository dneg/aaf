// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#undef WIN32_LEAN_AND_MEAN


#include "CAAFWAVEDescriptor.h"
#include "CAAFWAVEDescriptor.h"
#ifndef __CAAFWAVEDescriptor_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#if defined(_MAC) || defined(macintosh)

#define WAVE_FORMAT_PCM 0x0001

typedef struct tWAVEFORMATEX
{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
				    /* extra information (after cbSize) */
} WAVEFORMATEX, *PWAVEFORMATEX;


#endif



#if defined(_WIN32) || defined(WIN32)
  // Wave data does not have to be swapped on Windows platforms.
  #define SWAPSUMMARY(summary)
#else
  // Assume all other platforms are big-endian.
  // this will change when we adapt the sdk to
  // other platforms...

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

  static void SwapSummary(WAVEFORMATEX&	summary)
  {
    SwapBytes(&summary.wFormatTag, sizeof(summary.wFormatTag));
    SwapBytes(&summary.nChannels, sizeof(summary.nChannels));
    SwapBytes(&summary.nSamplesPerSec, sizeof(summary.nSamplesPerSec));
    SwapBytes(&summary.nAvgBytesPerSec, sizeof(summary.nAvgBytesPerSec));
    SwapBytes(&summary.nBlockAlign, sizeof(summary.nBlockAlign));
    SwapBytes(&summary.wBitsPerSample, sizeof(summary.wBitsPerSample));
    SwapBytes(&summary.cbSize, sizeof(summary.cbSize));

    // Ignore extra information for now trr: 1999-02-19
  }

  #define SWAPSUMMARY(summary) SwapSummary(summary);
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

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFWAVEDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;


	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
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
	aafUID_t		newUID;
	HRESULT			hr = AAFRESULT_SUCCESS;


  try
  {
    // Remove the previous test file if any.
    RemoveTestFile(pFileName);


	  // Create the AAF file
	  checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader));

    // Get the AAF Dictionary so that we can create valid AAF objects.
    checkResult(pHeader->GetDictionary(&pDictionary));
 		
	  // Create a source mob
		checkResult(pDictionary->CreateInstance(&AUID_AAFSourceMob,
							IID_IAAFSourceMob, 
							(IUnknown **)&pSourceMob));
		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));

		checkResult(CoCreateGuid((GUID *)&newUID));
		checkResult(pMob->SetMobID(&newUID));
		checkResult(pMob->SetName(L"WAVEDescriptorTest"));
		checkResult(pDictionary->CreateInstance(&AUID_AAFWAVEDescriptor,
									  IID_IAAFWAVEDescriptor, 
									  (IUnknown **)&pWAVEDesc));		

		WAVEFORMATEX			summary;

		summary.cbSize = sizeof(WAVEFORMATEX);
		summary.wFormatTag = WAVE_FORMAT_PCM;
		summary.wBitsPerSample = 16;
		summary.nAvgBytesPerSec = 88200;
		summary.nChannels = 1;
		summary.nBlockAlign = 2;
		summary.nSamplesPerSec = 44100;

		// NOTE: The elements in the summary structure need to be byte swapped
		//       on Big Endian system (i.e. the MAC).
    SWAPSUMMARY(summary)
		checkResult(pWAVEDesc->SetSummary(sizeof(WAVEFORMATEX), (aafDataValue_t)&summary));

    checkResult(pWAVEDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));

		// Add the MOB to the file
		checkResult(pHeader->AppendMob(pMob));
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
	  checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));

	  checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->EnumAAFAllMobs(NULL, &pMobIter));
		checkResult(pMobIter->NextOne(&pMob));
		checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
		
    // Back into testing mode
		checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
		
    // if there is an Essence Descriptor then it MUST be an (essence) WAVE Descriptor
		checkResult(pEssDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **) &pWAVEDesc));

    WAVEFORMATEX	summary;
    aafUInt32		size = 0;

		checkResult(pWAVEDesc->GetSummaryBufferSize(&size));
		checkExpression(size == sizeof(WAVEFORMATEX), AAFRESULT_TEST_FAILED);


    checkResult(pWAVEDesc->GetSummary(size, (aafDataValue_t)&summary));

    // NOTE: The elements in the summary structure need to be byte swapped
		//       on Big Endian system (i.e. the MAC).
    SWAPSUMMARY(summary)

		checkExpression(summary.cbSize == sizeof(WAVEFORMATEX)	&&
									summary.wFormatTag == WAVE_FORMAT_PCM	&&
									summary.wBitsPerSample == 16			&&
									summary.nAvgBytesPerSec == 88200		&&
									summary.nChannels == 1					&&
									summary.nBlockAlign == 2				&&
									summary.nSamplesPerSec == 44100,
                  AAFRESULT_TEST_FAILED);
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

	if (pHeader)
    pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

HRESULT CAAFWAVEDescriptor::test()
{
	aafWChar*	pFileName = L"AAFWAVEDescriptorTest.aaf";
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFWAVEDescriptor::test...Caught general C++ exception!" << endl; 
	}

	return hr;
}








