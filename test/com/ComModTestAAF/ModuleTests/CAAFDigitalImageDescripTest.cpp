// @doc INTERNAL
// @com This file implements the module test for CAAFDigitalImageDescriptor
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"


// default test values
#define kStoredHeightTestVal			248
#define kStoredWidthTestVal				720
#define kFrameLayoutTestVal				kSeparateFields
#define kVideoLineMapMaxElement			2
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
	ProductInfo.productName = L"AAFDigitalImageDescriptor Test";
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
	IAAFDictionary*	pDictionary = NULL;
	IAAFSourceMob*	pSourceMob = NULL;
	IAAFMob*	pMob = NULL;
	IAAFDigitalImageDescriptor*	pDIDesc = NULL;
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
    checkResult(pMob->SetName(L"DigitalImageDescriptorTest"));


    // Create a digitial image descriptor.
    checkResult(pDictionary->CreateInstance(&AUID_AAFDigitalImageDescriptor,
                IID_IAAFDigitalImageDescriptor, 
                (IUnknown **)&pDIDesc));		

    aafRational_t	ratio;
    aafInt32		VideoLineMap[kVideoLineMapMaxElement] = {kVideoLineMap1TestVal,kVideoLineMap2TestVal};
    aafUID_t		compression;
    memset(&compression, 0, sizeof(aafUID_t));

    // Add all DigitalImage properties
    // Reguired Properties
    checkResult(pDIDesc->SetStoredView(kStoredHeightTestVal, kStoredWidthTestVal));
    checkResult(pDIDesc->SetFrameLayout(kFrameLayoutTestVal));
    checkResult(pDIDesc->SetVideoLineMap(kVideoLineMapMaxElement, VideoLineMap));

    ratio.numerator = kImageAspectRatioNumTestVal;
    ratio.denominator = kImageAspectRatioDenTestVal;
    checkResult(pDIDesc->SetImageAspectRatio(ratio));

    // Optional Properties
    checkResult(pDIDesc->SetCompression(&compression));
    checkResult(pDIDesc->SetSampledView(kSampledHeightTestVal, kSampledWidthTestVal, kSampledXOffsetTestVal, kSampledYOffsetTestVal));
    checkResult(pDIDesc->SetDisplayView(kDisplayHeightTestVal, kDisplayWidthTestVal, kDisplayXOffsetTestVal, kDisplayYOffsetTestVal));
    checkResult(pDIDesc->SetAlphaTransparency(kAlphaTransparencyTestVal));
    checkResult(pDIDesc->SetImageAlignmentFactor(kImageAlignmentFactorTestVal));

    ratio.numerator = kGammaNumTestVal;
    ratio.denominator = kGammaDenTestVal;
    checkResult(pDIDesc->SetGamma(ratio));

    // Save the initialized descriptor with the source mob.
    checkResult(pDIDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
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

  if (pDIDesc)
    pDIDesc->Release();

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
	IAAFDigitalImageDescriptor*	pDIDesc = NULL;
	aafNumSlots_t	numMobs = 0;
	HRESULT			hr = AAFRESULT_SUCCESS;


  try
  {
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));

    // Make sure there is one a single mob in the file.
	  checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

    // Loop to the first mob.
	  checkResult(pHeader->EnumAAFAllMobs(NULL, &pMobIter));
	  checkResult(pMobIter->NextOne(&pMob));

	  checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));

    // Back into testing mode
	  checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));

    // if there is an Essence Descriptor then it MUST be an (essence) DigitalImage Descriptor
	  checkResult(pEssDesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **) &pDIDesc));

    // TODO: test for expected DigitalImage properties
	  aafUInt32				val1, val2;
	  aafInt32				val3, val4;
	  aafFrameLayout_t		framelayout;
	  aafAlphaTransparency_t	alphaTrans;
	  aafRational_t			ratio;
	  aafInt32				VideoLineMap[kVideoLineMapMaxElement];
	  aafUID_t				compression, compTestVal;

	  memset(&compTestVal, 0, sizeof(aafUID_t));

	  // Add all DigitalImage properties
	  // Reguired Properties
	  checkResult(pDIDesc->GetStoredView(&val1, &val2));
	  checkExpression(val1 == kStoredHeightTestVal && val2 == kStoredWidthTestVal,
						        AAFRESULT_TEST_FAILED);

	  checkResult(pDIDesc->GetFrameLayout(&framelayout));
	  checkExpression(framelayout == kFrameLayoutTestVal, AAFRESULT_TEST_FAILED);

	  checkResult(pDIDesc->GetVideoLineMapSize(&val1));
	  checkExpression(val1 == kVideoLineMapMaxElement, AAFRESULT_TEST_FAILED);

	  checkResult(pDIDesc->GetVideoLineMap(kVideoLineMapMaxElement, VideoLineMap));
	  checkExpression(VideoLineMap[0] == kVideoLineMap1TestVal && 
                    VideoLineMap[1] == kVideoLineMap2TestVal,
                    AAFRESULT_TEST_FAILED);
						  
	  checkResult(pDIDesc->GetImageAspectRatio(&ratio));
    checkExpression(ratio.numerator == kImageAspectRatioNumTestVal &&
                    ratio.denominator == kImageAspectRatioDenTestVal,
                    AAFRESULT_TEST_FAILED);

	  // Optional Properties
	  checkResult(pDIDesc->GetCompression(&compression));
	  checkExpression(memcmp(&compression, &compTestVal, sizeof(aafUID_t)) == 0,
                    AAFRESULT_TEST_FAILED);

	  checkResult(pDIDesc->GetSampledView(&val1, &val2, &val3, &val4));
	  checkExpression(val1 == kSampledHeightTestVal && val2 == kSampledWidthTestVal &&
                    val3 == kSampledXOffsetTestVal && val4 == kSampledYOffsetTestVal,
                    AAFRESULT_TEST_FAILED);

		checkResult(pDIDesc->GetDisplayView(&val1, &val2, &val3, &val4));
		checkExpression(val1 == kDisplayHeightTestVal && val2 == kDisplayWidthTestVal &&
                    val3 == kDisplayXOffsetTestVal && val4 == kDisplayYOffsetTestVal,
                    AAFRESULT_TEST_FAILED);

		checkResult(pDIDesc->GetAlphaTransparency(&alphaTrans));
		checkExpression(alphaTrans == kAlphaTransparencyTestVal,
                    AAFRESULT_TEST_FAILED);

		checkResult(pDIDesc->GetImageAlignmentFactor(&val3));
		checkExpression(val3 == kImageAlignmentFactorTestVal,
                    AAFRESULT_TEST_FAILED);

		checkResult(pDIDesc->GetGamma(&ratio));
		checkExpression(ratio.numerator == kGammaNumTestVal &&
			              ratio.denominator == kGammaDenTestVal,
                    AAFRESULT_TEST_FAILED);
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  // Cleanup and return
  if (pEssDesc)
    pEssDesc->Release();

  if (pDIDesc)
    pDIDesc->Release();

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

extern "C" HRESULT CAAFDigitalImageDescriptor_test()
{
	aafWChar*	pFileName = L"AAFDigitalImageDescTest.aaf";
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

	try
	{
		hr = CreateAAFFile(pFileName);
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFDigitalImageDescriptor_test...Caught general C++ exception!" << endl; 
	}

	return hr;
}

