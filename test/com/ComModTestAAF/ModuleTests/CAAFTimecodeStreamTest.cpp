// @doc INTERNAL
// @com This file implements the module test for CAAFTimecodeStream
/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


#ifndef __AAF_h__
#include "AAF.h"
#endif

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"



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

static char				testPattern[] = "ATestBuffer Pattern";
static aafRational_t	testSpeed = { 2997, 100 };

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob						*pMob = NULL;
	IAAFMobSlot					*pNewSlot = NULL;
	IAAFTimecodeStream				*pTimecodeStream = NULL;
	IAAFSegment					*pSeg = NULL;
	
	aafUID_t					newMobID;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	aafLength_t					zero;
	
	CvtInt32toLength(0, zero);
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTimecodeStream Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		
		// Create the file
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		
		// Create a CompositionMob
		checkResult(pDictionary->CreateInstance(&AUID_AAFCompositionMob,
			IID_IAAFCompositionMob, 
			(IUnknown **)&pCompMob));
		
		// Get a MOB interface
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(CoCreateGuid((GUID *)&newMobID));
		checkResult(pMob->SetMobID(&newMobID));
		
		checkResult(pCompMob->Initialize(L"COMPMOB01"));
		
		checkResult(pDictionary->CreateInstance(&AUID_AAFTimecodeStream,
			IID_IAAFTimecodeStream, 
			(IUnknown **)&pTimecodeStream));		
				
		checkResult(pTimecodeStream->QueryInterface (IID_IAAFSegment, (void **)&pSeg));
		checkResult(pMob->AppendNewSlot (pSeg, 0, L"TimecodeStream", &pNewSlot));
		checkResult(pHeader->AppendMob(pMob));
		
		/* Can we do this bottom up?? !!! */
		checkResult(pTimecodeStream->SetSampleRate(testSpeed));
		checkResult(pTimecodeStream->SetSourceType(kAAFTimecodeLTC));
		checkResult(pTimecodeStream->SetSource(sizeof(testPattern), (aafUInt8 *)testPattern));
		
		// We can't test SetPositionTimecode() and SetUserDataAtPosition(), as these
		// require an implementation for packing and unpacking the bits, which this
		// abstract superclass does not have.
		
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (pNewSlot)
		pNewSlot->Release();
	
	if (pSeg)
		pSeg->Release();
	
	if (pTimecodeStream)
		pTimecodeStream->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pCompMob)
		pCompMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile) 
	{
		if (bFileOpen)
		{
			pFile->Save();
			pFile->Close();
		}
		pFile->Release();
	}
	
	return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
    // IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *				pHeader = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	IEnumAAFMobSlots*			pEnum = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	IAAFSegment*				pSeg = NULL;
	IAAFTimecodeStream*			pTimecodeStream = NULL;
	char						testBuf[256];
	aafTimecodeSourceType_t		checkType;
	aafUInt32					sourceLen, bytesRead;
	aafRational_t				checkSpeed;
	
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	HRESULT						hr = S_OK;
	
	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"Make AVR Example. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;
	
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		checkResult(pHeader->EnumAAFAllMobs( NULL, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->EnumAAFAllMobSlots (&pEnum));
			
			while (AAFRESULT_SUCCESS == pEnum->NextOne (&pMobSlot))
			{
				checkResult(pMobSlot->GetSegment (&pSeg));
				// Get a TimecodeStream interface 
				checkResult(pSeg->QueryInterface (IID_IAAFTimecodeStream, (void **)&pTimecodeStream));
				
				checkResult(pTimecodeStream->GetSampleRate(&checkSpeed));
				checkExpression(testSpeed.numerator == testSpeed.numerator, AAFRESULT_TEST_FAILED);
				checkExpression(testSpeed.denominator == testSpeed.denominator, AAFRESULT_TEST_FAILED);
				
				checkResult(pTimecodeStream->GetSourceType(&checkType));
				checkExpression(checkType == kAAFTimecodeLTC, AAFRESULT_TEST_FAILED);
				checkResult(pTimecodeStream->GetSourceBufLen(&sourceLen));
				checkExpression(sourceLen == sizeof(testPattern), AAFRESULT_TEST_FAILED);
				
				checkResult(pTimecodeStream->GetSource(sourceLen, (aafUInt8 *)testBuf, &bytesRead));
				checkExpression(strcmp(testBuf, testPattern) == 0, AAFRESULT_TEST_FAILED);
				
				// We can't test GetSampleSize(), GetPositionTimecode(), GetUserDataLength(),
				// We can't test GetUserDataAtPosition(), SegmentOffsetToTC(), and
				// SegmentTCToOffset(), as these require an implementation for managing a specific
				// bit pattern (pack and unpack bits, return size of bits, etc...), which this
				// abstract superclass does not have.
				
				
				
				
				pTimecodeStream->Release();
				pTimecodeStream = NULL;
				
				pSeg->Release();
				pSeg = NULL;
			}
			
			pEnum->Release();
			pEnum = NULL;
			
			pMob->Release();
			pMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (pTimecodeStream)
		pTimecodeStream->Release();
	
	if (pSeg)
		pSeg->Release();
	
	if (pMobSlot)
		pMobSlot->Release();
	
	if (pEnum)
		pEnum->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pMobIter)
		pMobIter->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile) 
	{
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
	}
	
	return hr;
}

extern "C" HRESULT CAAFTimecodeStream_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"TimecodeStreamTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTimecodeStream::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.
	return hr;
}


#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
