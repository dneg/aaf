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






#include "CAAFWAVEDescriptor.h"
#include "CAAFWAVEDescriptor.h"
#ifndef __CAAFWAVEDescriptor_h__
#error - improperly defined include guard
#endif

#include <iostream.h>
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

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"Make AVR Example";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)ppFile);
	if (AAFRESULT_SUCCESS != hr)
		return hr;
    hr = (*ppFile)->Initialize();
	if (AAFRESULT_SUCCESS != hr)
		return hr;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = (*ppFile)->OpenExistingRead(pFileName, 0);
		break;

	case kMediaOpenAppend:
		hr = (*ppFile)->OpenNewModify(pFileName, 0, &ProductInfo);
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
	IAAFSourceMob*	pSourceMob = NULL;
	aafUID_t		newUID;
	HRESULT			hr = AAFRESULT_SUCCESS;

	// Create the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

	// Create a source mob
	hr = CoCreateInstance(CLSID_AAFSourceMob,
						NULL, 
						CLSCTX_INPROC_SERVER, 
						IID_IAAFSourceMob, 
						(void **)&pSourceMob);
	if (SUCCEEDED(hr))
	{
		IAAFMob*	pMob = NULL;

		hr = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		if (SUCCEEDED(hr))
		{
			IAAFWAVEDescriptor*	pWAVEDesc = NULL;

			CoCreateGuid((GUID *)&newUID);
			pMob->SetMobID(&newUID);
			pMob->SetName(L"WAVEDescriptorTest");
			hr = CoCreateInstance(CLSID_AAFWAVEDescriptor,
									NULL, 
									CLSCTX_INPROC_SERVER, 
									IID_IAAFWAVEDescriptor, 
									(void **)&pWAVEDesc);		
			if (SUCCEEDED(hr))
			{
				IAAFEssenceDescriptor*	pEssDesc = NULL;
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

				hr = pWAVEDesc->SetSummary(sizeof(WAVEFORMATEX), (aafDataValue_t)&summary);
				if (SUCCEEDED(hr))
				{
					hr = pWAVEDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc);
					if (SUCCEEDED(hr))
					{
						hr = pSourceMob->SetEssenceDescriptor(pEssDesc);
						if (SUCCEEDED(hr))
						{
						}
						pEssDesc->Release();
						pEssDesc = NULL;
					}
				}
				pWAVEDesc->Release();
				pWAVEDesc = NULL;
			}

			// Add the MOB to the file
			if (SUCCEEDED(hr))
				hr = pHeader->AppendMob(pMob);

			pMob->Release();
			pMob = NULL;
		}
		pSourceMob->Release();
		pSourceMob = NULL;
	}

	if (pHeader) pHeader->Release();

	if (pFile)
	{
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
	aafNumSlots_t	numMobs = 0;
	HRESULT			hr = AAFRESULT_SUCCESS;

	// Open the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

	hr = pHeader->GetNumMobs(kAllMob, &numMobs);
	if (1 != numMobs)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	hr = pHeader->EnumAAFAllMobs(NULL, &pMobIter);
	if (SUCCEEDED(hr))
	{
		IAAFMob*	pMob = NULL;

		hr = pMobIter->NextOne(&pMob);
		if (SUCCEEDED(hr))
		{
			IAAFSourceMob*	pSourceMob = NULL;

			hr = pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob);
			if (SUCCEEDED(hr))
			{					 
				IAAFEssenceDescriptor*	pEssDesc = NULL;

				// Back into testing mode
				hr = pSourceMob->GetEssenceDescriptor(&pEssDesc);
				if (SUCCEEDED(hr))
				{
					IAAFWAVEDescriptor*	pWAVEDesc = NULL;

					// if there is an Essence Descriptor then it MUST be an (essence) WAVE Descriptor
					hr = pEssDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **) &pWAVEDesc);
					if (SUCCEEDED(hr))
					{
						WAVEFORMATEX	summary;
						aafUInt32		size = 0;

						pWAVEDesc->GetSummaryBufferSize(&size);
						if (size == sizeof(WAVEFORMATEX))
						{
							hr = pWAVEDesc->GetSummary(size, (aafDataValue_t)&summary);
							if (SUCCEEDED(hr))
							{
								// NOTE: The elements in the summary structure need to be byte swapped
								//       on Big Endian system (i.e. the MAC).

								if (summary.cbSize != sizeof(WAVEFORMATEX)	||
									summary.wFormatTag != WAVE_FORMAT_PCM	||
									summary.wBitsPerSample != 16			||
									summary.nAvgBytesPerSec != 88200		||
									summary.nChannels != 1					||
									summary.nBlockAlign != 2				||
									summary.nSamplesPerSec != 44100)
								{
									hr = AAFRESULT_TEST_FAILED;
								}
							}
						}
						else
						{
							hr = AAFRESULT_TEST_FAILED;
						}

						pWAVEDesc->Release();
						pWAVEDesc = NULL;
					}
					pEssDesc->Release();
					pEssDesc = NULL;
				}
				else
				{
					hr = AAFRESULT_TEST_FAILED;
				}
				pSourceMob->Release();
				pSourceMob = NULL;
			}
			pMob->Release();
			pMob = NULL;
		}
		pMobIter->Release();
		pMobIter = NULL;
	}

Cleanup:

	if (pHeader) pHeader->Release();

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








