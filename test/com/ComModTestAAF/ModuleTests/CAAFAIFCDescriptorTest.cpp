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


#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"


typedef struct tChunk
{
	char	ckID[4];
	long	ckDataSize;
	char	ckData;
} Chunk;

typedef struct tAIFCFORMATCHUNK
{
    char	ckID[4];		// "FORM"
	long	ckDataSize;
	char	formType[4];	// "AIFC"
} FormAIFCChunk;

typedef struct tCommonChunk
{
	char			ckID[4];		// "COMM"
	long			ckDataSize;
	short			numChannels;
	unsigned long	numSampleFrames;
	short			sampleSize;
	unsigned char	sampleRate[10];	// This should be IEEE floating point number
	char			compressionType[4];
	unsigned char	compNameLength;
	char			compressionName[15];
} CommonChunk;

typedef struct tSoundDataChunk
{
	char			ckID[4];		// "SSND"
	long			ckDataSize;
	unsigned long	offset;
	unsigned long	blockSize;
	char			soundData;
} SoundDataChunk;

typedef struct tAIFCSUMMARY
{
	FormAIFCChunk	formChunk;
	CommonChunk		commChunk;
	SoundDataChunk	ssndChunk;
} AIFCSummary;

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

  static void SwapSummary(AIFCSummary&	summary)
  {
    SwapBytes(&summary.commChunk, sizeof(summary.commChunk ));
    SwapBytes(&summary.formChunk, sizeof(summary.formChunk ));
    SwapBytes(&summary.ssndChunk, sizeof(summary.ssndChunk ));

	// tlk I am not completely convinced that this will work
	// on a MAC. but anyhow the whole AIFC data is bogus and
	// all we try to do here is unit test the AIFCDescriptor
	// Get/Setsummary modules !!! 1999, 4, 21
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
	ProductInfo.productName = L"AAFAIFCDescriptor Test";
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
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IAAFDictionary*			pDictionary = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFMob*				pMob = NULL;
	IAAFAIFCDescriptor*		pAIFCDesc = NULL;
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
		checkResult(pMob->SetName(L"AIFCDescriptorTest"));
		checkResult(pDictionary->CreateInstance(&AUID_AAFAIFCDescriptor,
												IID_IAAFAIFCDescriptor, 
												(IUnknown **)&pAIFCDesc));		

		AIFCSummary summary;
		
		strcpy(summary.formChunk.ckID , "FORM");
		strcpy(summary.formChunk.formType , "AIFC");
		summary.formChunk.ckDataSize = sizeof(AIFCSummary) - 8;
		strcpy(summary.commChunk.ckID, "COMM");
		summary.commChunk.ckDataSize = sizeof(CommonChunk);
		summary.commChunk.numChannels = 1;
		summary.commChunk.numSampleFrames = 0;
		summary.commChunk.sampleSize = 16;
		summary.commChunk.sampleRate[0] = 0x40;
		summary.commChunk.sampleRate[1] = 0x0E;
		summary.commChunk.sampleRate[2] = 0xac;
		summary.commChunk.sampleRate[3] = 0x44;
		summary.commChunk.sampleRate[4] = 0x0;
		summary.commChunk.sampleRate[5] = 0x0;
		summary.commChunk.sampleRate[6] = 0x0;
		summary.commChunk.sampleRate[7] = 0x0;
		summary.commChunk.sampleRate[8] = 0x0;
		summary.commChunk.sampleRate[9] = 0x0;
		strcpy(summary.commChunk.compressionType, "NONE");
		strcpy(summary.commChunk.compressionName, "Not compressed");
		summary.commChunk.compNameLength = strlen(summary.commChunk.compressionName );

		strcpy(summary.ssndChunk.ckID, "SSND");
		summary.ssndChunk.ckDataSize = 0;
		summary.ssndChunk.offset = 0;
		summary.ssndChunk.soundData = 0;

		// NOTE: The elements in the summary structure need to be byte swapped
		// on Big Endian system (i.e. the MAC).
		SWAPSUMMARY(summary)
		checkResult(pAIFCDesc->SetSummary(sizeof(AIFCSummary), (aafDataValue_t)&summary));

		checkResult(pAIFCDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
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
		checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));

		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->EnumAAFAllMobs(NULL, &pMobIter));
		checkResult(pMobIter->NextOne(&pMob));
		checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
		
		// Back into testing mode
		checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
		
		// if there is an Essence Descriptor then it MUST be an (essence) AIFC Descriptor
		checkResult(pEssDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **) &pAIFCDesc));

		AIFCSummary summary;
	    aafUInt32		size = 0;

		checkResult(pAIFCDesc->GetSummaryBufferSize(&size));
		checkExpression(size == sizeof(AIFCSummary), AAFRESULT_TEST_FAILED);


		checkResult(pAIFCDesc->GetSummary(size, (aafDataValue_t)&summary));
	
		// NOTE: The elements in the summary structure need to be byte swapped
		//       on Big Endian system (i.e. the MAC).
		SWAPSUMMARY(summary)

		checkExpression((strncmp(summary.commChunk.ckID, "COMM", 4) == 0) &&
						(strncmp(summary.formChunk.ckID, "FORM", 4) == 0) &&
						(strncmp(summary.ssndChunk.ckID, "SSND", 4) == 0),
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

extern "C" HRESULT CAAFAIFCDescriptor_test()
{
	aafWChar*	pFileName = L"AAFAIFCDescriptorTest.aaf";
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFAIFCDescriptor_test...Caught general C++ exception!" << endl; 
	}

	return hr;
}








