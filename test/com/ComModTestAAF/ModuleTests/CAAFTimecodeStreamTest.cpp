// @doc INTERNAL
// @com This file implements the module test for CAAFTimecodeStream
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


#ifndef __AAF_h__
#include "AAF.h"
#endif

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
//#include "aafCvt.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x65d164da, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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
	IAAFTimelineMobSlot			*pNewSlot = NULL;
	IAAFTimecodeStream			*pTimecodeStream = NULL;
	IAAFSegment					*pSeg = NULL;
	IAAFComponent*		pComponent = NULL;
	
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTimecodeStream Test";
	ProductInfo.productVersion = &v;
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
		CAAFBuiltinDefs defs (pDictionary);
		
		// Create a CompositionMob
		checkResult(defs.cdCompositionMob()->
					CreateInstance(IID_IAAFCompositionMob, 
								   (IUnknown **)&pCompMob));
		
		// Get a MOB interface
		checkResult(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		checkResult(pMob->SetMobID(TEST_MobID));
		
		checkResult(pCompMob->Initialize(L"COMPMOB01"));
		
		// Create a concrete subclass fo TimecodeStream
		checkResult(defs.cdTimecodeStream12M()->
					CreateInstance(IID_IAAFTimecodeStream, 
								   (IUnknown **)&pTimecodeStream));		
		 checkResult(pTimecodeStream->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		 checkResult(pComponent->SetDataDef(defs.ddPicture()));
		pComponent->Release();
		pComponent = NULL;
				
		checkResult(pTimecodeStream->QueryInterface (IID_IAAFSegment, (void **)&pSeg));
		aafRational_t editRate = { 0, 1};
		checkResult(pMob->AppendNewTimelineSlot (editRate,
												 pSeg,
												 0,
												 L"TimecodeStream",
												 0,
												 &pNewSlot));
		checkResult(pHeader->AddMob(pMob));
		
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
	
	if (pComponent)
		pComponent->Release();

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
	
	aafNumSlots_t				numMobs;
	HRESULT						hr = S_OK;
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the number of mobs in the file (should be one)
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		checkResult(pHeader->GetMobs( NULL, &pMobIter));
		while (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
		{
			checkResult(pMob->GetSlots (&pEnum));
			
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

extern "C" HRESULT CAAFTimecodeStream_test(testMode_t mode);
extern "C" HRESULT CAAFTimecodeStream_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFTimecodeStreamTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTimecodeStream::test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

	// Cleanup our object if it exists.
	return hr;
}


#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
