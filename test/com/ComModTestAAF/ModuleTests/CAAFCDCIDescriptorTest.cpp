//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=



#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"
#include "CAAFBuiltinDefs.h"


// Default testing values for CDCI
#define kCWTest		8
#define kHSTest		2
#define kVSTest		2
#define kCSTest		kAAFCoSiting
#define kBRLTest	16
#define kWRLTest	255
#define kCRTest		255
#define kPBTest		0
#define kAlphaSamplingWidthTestVal		8
#define kReversedByteOrderTestVal		kAAFTrue

// default test values for DID
#define kStoredHeightTestVal			248
#define kStoredWidthTestVal				720
#define kFrameLayoutTestVal				kAAFSeparateFields
#define kVideoLineMapSizeTestVal		2
#define kVideoLineMap1TestVal			10
#define kVideoLineMap2TestVal			11
#define kImageAspectRatioNumTestVal		4
#define kImageAspectRatioDenTestVal		3
#define kSampledHeightTestVal			247
#define kSampledWidthTestVal			719
#define kSampledXOffsetTestVal			5
#define kSampledYOffsetTestVal			6
#define kDisplayHeightTestVal			246
#define kDisplayWidthTestVal			718
#define kDisplayXOffsetTestVal			7
#define kDisplayYOffsetTestVal			8
#define kAlphaTransparencyTestVal		kAAFMaxValueTransparent
#define kImageAlignmentFactorTestVal	0

// our test Mob id 
static const aafMobID_t	TEST_MobID = 
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x3b38e782, 0x03fd, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static HRESULT SetDigitalImageDescProps(IAAFCDCIDescriptor* pDesc)
{
	IAAFDigitalImageDescriptor* pDIDesc;
	aafRational_t	ratio;
	aafInt32		VideoLineMap[kVideoLineMapSizeTestVal] = {kVideoLineMap1TestVal,kVideoLineMap2TestVal};
	aafUID_t		compression;

	pDesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **) &pDIDesc);

	memset(&compression, 0, sizeof(aafUID_t));

	// TODO: add all DigitalImage properties
	// Required Properties
	pDIDesc->SetStoredView(kStoredHeightTestVal, kStoredWidthTestVal);
	pDIDesc->SetFrameLayout(kFrameLayoutTestVal);
	pDIDesc->SetVideoLineMap(kVideoLineMapSizeTestVal, VideoLineMap);
	
	ratio.numerator = kImageAspectRatioNumTestVal;
	ratio.denominator = kImageAspectRatioDenTestVal;
	pDIDesc->SetImageAspectRatio(ratio);

	// Optional Properties
	pDIDesc->SetCompression(compression);
	pDIDesc->SetSampledView(kSampledHeightTestVal, kSampledWidthTestVal, kSampledXOffsetTestVal, kSampledYOffsetTestVal);
	pDIDesc->SetDisplayView(kDisplayHeightTestVal, kDisplayWidthTestVal, kDisplayXOffsetTestVal, kDisplayYOffsetTestVal);
	pDIDesc->SetAlphaTransparency(kAlphaTransparencyTestVal);
	pDIDesc->SetImageAlignmentFactor(kImageAlignmentFactorTestVal);

	pDIDesc->Release();

	return AAFRESULT_SUCCESS;
}

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
	IAAFFile*		pFile = NULL;
	IAAFHeader*		pHeader = NULL;
	IAAFDictionary*	pDictionary = NULL;
	IAAFSourceMob*	pSourceMob = NULL;
	HRESULT			hr = AAFRESULT_SUCCESS;


  // Remove the previous test file if any.
  RemoveTestFile(pFileName);

	// Create the AAF file
	hr = CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile );
	if (FAILED(hr))
		return hr;

  // Get the AAF file header.
  hr = pFile->GetHeader(&pHeader);
  if (FAILED(hr))
  {
    pFile->Release();
    return hr;
  }

  // Get the AAF Dictionary so that we can create valid AAF objects.
  hr = pHeader->GetDictionary(&pDictionary);
	if (SUCCEEDED(hr))
  {
	  CAAFBuiltinDefs defs (pDictionary);
	  // Create a source mob
	  hr = defs.cdSourceMob()->
		CreateInstance(IID_IAAFSourceMob, 
					   (IUnknown **)&pSourceMob);
	  if (SUCCEEDED(hr))
	  {
		  IAAFMob*	pMob = NULL;

		  hr = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		  if (SUCCEEDED(hr))
		  {
			  IAAFCDCIDescriptor*	pCDCIDesc = NULL;
			  IAAFCDCIDescriptor2*	pCDCIDesc2 = NULL;

			  pMob->SetMobID(TEST_MobID);
			  pMob->SetName(L"CDCIDescriptorTest");
			  hr = defs.cdCDCIDescriptor()->
				CreateInstance(IID_IAAFCDCIDescriptor, 
							   (IUnknown **)&pCDCIDesc);		
			  if (SUCCEEDED(hr))
			  {
				  // Add all CDCI properties
				  hr = pCDCIDesc->SetComponentWidth(kCWTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetHorizontalSubsampling(kHSTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetVerticalSubsampling(kVSTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetColorSiting(kCSTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetBlackReferenceLevel(kBRLTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetWhiteReferenceLevel(kWRLTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetColorRange(kCRTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetPaddingBits(kPBTest);
				  if (SUCCEEDED(hr)) hr = SetDigitalImageDescProps(pCDCIDesc);

			    // Optional Properties accessed using IAAFCDCIDescriptor2
					if (SUCCEEDED(hr)) hr = pCDCIDesc->QueryInterface (IID_IAAFCDCIDescriptor2, (void **)&pCDCIDesc2);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc2->SetAlphaSamplingWidth(kAlphaSamplingWidthTestVal);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc2->SetReversedByteOrder(kReversedByteOrderTestVal);
				  pCDCIDesc2->Release();
				  pCDCIDesc2 = NULL;

				  if (SUCCEEDED(hr))
				  {
					  IAAFEssenceDescriptor*	pEssDesc = NULL;

					  hr = pCDCIDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc);
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
				  pCDCIDesc->Release();
				  pCDCIDesc = NULL;
			  }

			  // Add the MOB to the file
			  if (SUCCEEDED(hr))
				  hr = pHeader->AddMob(pMob);

			  pMob->Release();
			  pMob = NULL;
		  }
		  pSourceMob->Release();
		  pSourceMob = NULL;
	  }

    pDictionary->Release();
    pDictionary = NULL;
  }

	if (pHeader) pHeader->Release();

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
	aafNumSlots_t	numMobs = 0;
	HRESULT			hr = AAFRESULT_SUCCESS;

	// Open the AAF file
	hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);
	if (FAILED(hr))
		return hr;

	// Get the AAF file header.
	hr = pFile->GetHeader(&pHeader);
	if (FAILED(hr))
	{
		pFile->Release();
		return hr;
	}

	hr = pHeader->CountMobs(kAAFAllMob, &numMobs);
	if (1 != numMobs)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	hr = pHeader->GetMobs(NULL, &pMobIter);
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
					IAAFCDCIDescriptor*	pCDCIDesc = NULL;
					IAAFCDCIDescriptor2*	pCDCIDesc2 = NULL;

					// if there is an Essence Descriptor then it MUST be an (essence) CDCI Descriptor
					hr = pEssDesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **) &pCDCIDesc);
					if (SUCCEEDED(hr))
					{
						aafInt16			val16 = 0;
						aafInt32			val32 = 0;
						aafUInt32			uval32 = 0;
						aafColorSiting_t	csval = 0;
						aafBoolean_t	boolval = 0;

						// test for expected CDCI properties
						hr = pCDCIDesc->GetComponentWidth(&val32);
						if (SUCCEEDED(hr) && val32 != kCWTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetHorizontalSubsampling(&uval32);
						if (SUCCEEDED(hr) && uval32 != kHSTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetVerticalSubsampling(&uval32);
						if (SUCCEEDED(hr) && uval32 != kVSTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetColorSiting(&csval);
						if (SUCCEEDED(hr) && csval != kCSTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetBlackReferenceLevel(&uval32);
						if (SUCCEEDED(hr) && uval32 != kBRLTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetWhiteReferenceLevel(&uval32);
						if (SUCCEEDED(hr) && uval32 != kWRLTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetColorRange(&uval32);
						if (SUCCEEDED(hr) && uval32 != kCRTest) hr = AAFRESULT_TEST_FAILED;

						if (SUCCEEDED(hr)) hr = pCDCIDesc->GetPaddingBits(&val16);
						if (SUCCEEDED(hr) && val16 != kPBTest) hr = AAFRESULT_TEST_FAILED;

				    // Optional Properties accessed using IAAFCDCIDescriptor2
						if (SUCCEEDED(hr)) hr = pCDCIDesc->QueryInterface (IID_IAAFCDCIDescriptor2, (void **)&pCDCIDesc2);

					  if (SUCCEEDED(hr)) hr = pCDCIDesc2->GetAlphaSamplingWidth(&uval32);
						if (SUCCEEDED(hr) && uval32 != kAlphaSamplingWidthTestVal) hr = AAFRESULT_TEST_FAILED;

					  if (SUCCEEDED(hr)) hr = pCDCIDesc2->GetReversedByteOrder(&boolval);
						if (SUCCEEDED(hr) && boolval != kReversedByteOrderTestVal) hr = AAFRESULT_TEST_FAILED;

					  pCDCIDesc2->Release();
					  pCDCIDesc2 = NULL;

						pCDCIDesc->Release();
						pCDCIDesc = NULL;
					}
					else
					{
						hr = AAFRESULT_TEST_FAILED;
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

extern "C" HRESULT CAAFCDCIDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
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
		cerr << "CAAFCDCIDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}
