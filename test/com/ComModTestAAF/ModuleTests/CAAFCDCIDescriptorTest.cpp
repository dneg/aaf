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



/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/




#include "AAF.h"
#include "AAFResult.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFDefUIDs.h"

// Default testing values for CDCI
#define kCWTest		8
#define kHSTest		2
#define kCSTest		kCoSiting
#define kBRLTest	16
#define kWRLTest	255
#define kCRTest		255
#define kPBTest		0

// default test values for DID
#define kStoredHeightTestVal			248
#define kStoredWidthTestVal				720
#define kFrameLayoutTestVal				kSeparateFields
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
#define kAlphaTransparencyTestVal		kMaxValueTransparent
#define kImageAlignmentFactorTestVal	0
#define kGammaNumTestVal				7
#define kGammaDenTestVal				8

static HRESULT SetDigitalImageDescProps(IAAFCDCIDescriptor* pDesc)
{
	IAAFDigitalImageDescriptor* pDIDesc;
	aafRational_t	ratio;
	aafInt32		VideoLineMap[kVideoLineMapSizeTestVal] = {kVideoLineMap1TestVal,kVideoLineMap2TestVal};
	aafUID_t		compression;

	pDesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **) &pDIDesc);

	memset(&compression, 0, sizeof(aafUID_t));

	// TODO: add all DigitalImage properties
	// Reguired Properties
	pDIDesc->SetStoredView(kStoredHeightTestVal, kStoredWidthTestVal);
	pDIDesc->SetFrameLayout(kFrameLayoutTestVal);
	pDIDesc->SetVideoLineMap(kVideoLineMapSizeTestVal, VideoLineMap);
	
	ratio.numerator = kImageAspectRatioNumTestVal;
	ratio.denominator = kImageAspectRatioDenTestVal;
	pDIDesc->SetImageAspectRatio(ratio);

	// Optional Properties
	pDIDesc->SetCompression(&compression);
	pDIDesc->SetSampledView(kSampledHeightTestVal, kSampledWidthTestVal, kSampledXOffsetTestVal, kSampledYOffsetTestVal);
	pDIDesc->SetDisplayView(kDisplayHeightTestVal, kDisplayWidthTestVal, kDisplayXOffsetTestVal, kDisplayYOffsetTestVal);
	pDIDesc->SetAlphaTransparency(kAlphaTransparencyTestVal);
	pDIDesc->SetImageAlignmentFactor(kImageAlignmentFactorTestVal);

	ratio.numerator = kGammaNumTestVal;
	ratio.denominator = kGammaDenTestVal;
	pDIDesc->SetGamma(ratio);

	pDIDesc->Release();

	return AAFRESULT_SUCCESS;
}

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

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFCDCIDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

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
	IAAFDictionary*	pDictionary = NULL;
	IAAFSourceMob*	pSourceMob = NULL;
	aafUID_t		newUID;
	HRESULT			hr = AAFRESULT_SUCCESS;


  // Remove the previous test file if any.
  RemoveTestFile(pFileName);

	// Create the AAF file
	hr = OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader);
	if (FAILED(hr))
		return hr;

  // Get the AAF Dictionary so that we can create valid AAF objects.
  hr = pHeader->GetDictionary(&pDictionary);
	if (SUCCEEDED(hr))
  {
	  // Create a source mob
	  hr = pDictionary->CreateInstance(&AUID_AAFSourceMob,
						  IID_IAAFSourceMob, 
						  (IUnknown **)&pSourceMob);
	  if (SUCCEEDED(hr))
	  {
		  IAAFMob*	pMob = NULL;

		  hr = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		  if (SUCCEEDED(hr))
		  {
			  IAAFCDCIDescriptor*	pCDCIDesc = NULL;

			  CoCreateGuid((GUID *)&newUID);
			  pMob->SetMobID(&newUID);
			  pMob->SetName(L"CDCIDescriptorTest");
			  hr = pDictionary->CreateInstance(&AUID_AAFCDCIDescriptor,
									  IID_IAAFCDCIDescriptor, 
									  (IUnknown **)&pCDCIDesc);		
			  if (SUCCEEDED(hr))
			  {
				  // Add all CDCI properties
				  hr = pCDCIDesc->SetComponentWidth(kCWTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetHorizontalSubsampling(kHSTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetColorSiting(kCSTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetBlackReferenceLevel(kBRLTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetWhiteReferenceLevel(kWRLTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetColorRange(kCRTest);
				  if (SUCCEEDED(hr)) hr = pCDCIDesc->SetPaddingBits(kPBTest);
				  if (SUCCEEDED(hr)) hr = SetDigitalImageDescProps(pCDCIDesc);

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
				  hr = pHeader->AppendMob(pMob);

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
					IAAFCDCIDescriptor*	pCDCIDesc = NULL;

					// if there is an Essence Descriptor then it MUST be an (essence) CDCI Descriptor
					hr = pEssDesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **) &pCDCIDesc);
					if (SUCCEEDED(hr))
					{
						aafInt16			val16;
						aafInt32			val;
						aafUInt32			uval;
						aafColorSiting_t	csval;

						// test for expected CDCI properties
						hr = pCDCIDesc->GetComponentWidth(&val);
						if (SUCCEEDED(hr) && val == kCWTest)
							hr = pCDCIDesc->GetHorizontalSubsampling(&uval);

						if (SUCCEEDED(hr) && uval == kHSTest)
							hr = pCDCIDesc->GetColorSiting(&csval);

						if (SUCCEEDED(hr) && csval == kCSTest)
							hr = pCDCIDesc->GetBlackReferenceLevel(&uval);

						if (SUCCEEDED(hr) && uval == kBRLTest)
							hr = pCDCIDesc->GetWhiteReferenceLevel(&uval);

						if (SUCCEEDED(hr) && uval == kWRLTest)
							hr = pCDCIDesc->GetColorRange(&uval);

						if (SUCCEEDED(hr) && uval == kCRTest)
							hr = pCDCIDesc->GetPaddingBits(&val16);

						if (FAILED(hr) || val16 != kPBTest)
							hr = AAFRESULT_TEST_FAILED;

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

extern "C" HRESULT CAAFCDCIDescriptor_test()
{
	aafWChar*	pFileName = L"AAFCDCIDescriptorTest.aaf";
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFCDCIDescriptor_test...Caught general C++ exception!" << endl; 
	}

	return hr;
}
