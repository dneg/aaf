/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
\******************************************/

#include "CAAFJPEGCodec.h"

#include <assert.h>
#include "AAFResult.h"

#include "AAF.h"

#include "aafUtils.h"
#include "aafCvt.h"
#include "aafDataDefs.h"
#include "aafDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"



// {0DB382D1-3BAC-11d3-BFD6-00104BC9156D}
const CLSID CLSID_AAFJPEGCodec = 
{ 0xdb382d1, 0x3bac, 0x11d3, { 0xbf, 0xd6, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };




// This plugin currently only supports a single definition
const aafInt32 kSupportedDefinitions = 1;

// This plugin currently only supports a single code flavour
const aafInt32 kSupportedCodeFlavours = 1;

const wchar_t kDisplayName[] = L"AAF JPEG Codec";
const wchar_t kDescription[] = L"Handles Standard JFIF/JPEG";

const aafProductVersion_t kAAFPluginVersion = {1, 0, 0, 1, kVersionBeta};
const aafRational_t		kDefaultRate = { 2997, 100 };
const aafInt32			kDefaultSampleWidth = 24;
const aafInt32			kDefaultNumCh = 1;

//
// Plugin Descriptor information
//
const aafUID_t AVID_JPEG_PLUGIN = 
{ 0x28ac17ce, 0x3bac, 0x11d3, { 0xbf, 0xd6, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };

static wchar_t *kManufURL = L"http://www.avid.com";
static wchar_t *kDownloadURL = L"ftp://ftp.avid.com/pub/";
static aafVersionType_t samplePluginVersion = { 0, 1 };

static wchar_t *kManufName = L"Avid Technology, Inc.";
static wchar_t *kManufRev = L"Rev 0.1";

// Should be shared by all built-in plugins created by AVID. /* TRR */
const aafUID_t MANUF_AVID_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


const aafUID_t NULL_ID = {0};
const aafRational_t NULL_RATIONAL = {0, 0};

// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw r;
}

inline void checkExpression(bool test, AAFRESULT r)
{
  if (!test)
    throw r;
}




CJPEGDescriptorHelper::CJPEGDescriptorHelper() :
	_filemob(NULL),
	_edes(NULL),
	_filedes(NULL),
	_dides(NULL),
	_cdcides(NULL)
{
}

CJPEGDescriptorHelper::~CJPEGDescriptorHelper()
{
	Clear();
}

void CJPEGDescriptorHelper::Clear(void)
{
	// Release all of our references...
	if (NULL != _cdcides)
	{
		_cdcides->Release();
		_cdcides = NULL;
	}
	if (NULL != _dides)
	{
		_dides->Release();
		_dides = NULL;
	}
	if (NULL != _filedes)
	{
		_filedes->Release();
		_filedes = NULL;
	}
	if (NULL != _edes)
	{
		_edes->Release();
		_edes = NULL;
	}
	if (NULL != _filemob)
	{
		_filemob->Release();
		_filemob = NULL;
	}

	if (NULL != _filemob_unk)
	{
		_filemob_unk->Release();
		_filemob_unk = NULL;
	}
}

HRESULT CJPEGDescriptorHelper::Initialize(IAAFSourceMob *filemob)
{
	HRESULT hr = S_OK;


	try
	{
		// Release all previous interface pointers if any exist...
		Clear();

		// Save the unknown interface pointer for equality testing.
		// NOTE: When testing objects for equality only IUnknown pointers can be
		// compared since they are on the only implemented interface pointers that
		// cannot be tearoff interfaces (implemented by another object).
		checkResult(filemob->QueryInterface(IID_IUnknown, (void **)&_filemob_unk));
		

		// Get the file mob interface, we need it in order to get the associated
		// essence descriptor.
		checkResult(_filemob_unk->QueryInterface(IID_IAAFSourceMob, (void **)&_filemob));
		checkResult(_filemob->GetEssenceDescriptor(&_edes));

		// Get the interfaces needed for the rest of the descriptor methods.
		checkResult(_edes->QueryInterface(IID_IAAFFileDescriptor, (void **)&_filedes));
		checkResult(_edes->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&_dides));
		checkResult(_edes->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&_dides));
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	return hr;
}


bool CJPEGDescriptorHelper::operator==(const CJPEGDescriptorHelper& rhs)
{
	// PRECONDITION: Objects must have been successfully initialized.
	assert((NULL != _filemob_unk) && (NULL != rhs._filemob_unk));
	return (_filemob_unk == rhs._filemob_unk);
}


bool CJPEGDescriptorHelper::operator!=(const CJPEGDescriptorHelper& rhs)
{
	// PRECONDITION: Objects must have been successfully initialized.
	assert((NULL != _filemob_unk) && (NULL != rhs._filemob_unk));
	return (_filemob_unk != rhs._filemob_unk);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetNumLocators (aafInt32 *  pCount)
{
	assert(NULL != _edes);
	return _edes->GetNumLocators(pCount);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::AppendLocator (IAAFLocator * pLocator)
{
	assert(NULL != _edes);
	return _edes->AppendLocator (pLocator);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::PrependLocator (IAAFLocator * pLocator)
{
	assert(NULL != _edes);
	return _edes->PrependLocator (pLocator);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::EnumAAFAllLocators (IEnumAAFLocators ** ppEnum)
{
	assert(NULL != _edes);
	return _edes->EnumAAFAllLocators (ppEnum);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetLength (aafLength_t  length)
{
	assert(NULL != _filedes);
	return _filedes->SetLength(length);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetLength (aafLength_t *  pLength)
{
	assert(NULL != _filedes);
	return _filedes->GetLength(pLength);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetIsInContainer (aafBool  isAAF)
{
	assert(NULL != _filedes);
	return _filedes->SetIsInContainer (isAAF);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetIsInContainer (aafBool *  pIsAAF)
{
	assert(NULL != _filedes);
	return _filedes->GetIsInContainer (pIsAAF);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetSampleRate (aafRational_t *  pRate)
{
	assert(NULL != _filedes);
	return _filedes->SetSampleRate (pRate);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetSampleRate (aafRational_t*  pRate)
{
	assert(NULL != _filedes);
	return _filedes->GetSampleRate (pRate);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetContainerFormat (aafUID_t *  pFormat)
{
	assert(NULL != _filedes);
	return _filedes->SetContainerFormat (pFormat);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetContainerFormat (aafUID_t *  pFormat)
{
	assert(NULL != _filedes);
	return _filedes->GetContainerFormat (pFormat);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetCompression (aafUID_t *  pCodecID)
{
  assert(NULL != _dides);
	return _dides->SetCompression (pCodecID);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetCompression (aafUID_t *  pCompression)
{
  assert(NULL != _dides);
	return _dides->GetCompression (pCompression);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetStoredView (aafUInt32  StoredHeight,
        aafUInt32  StoredWidth)
{
  assert(NULL != _dides);
	return _dides->SetStoredView (StoredHeight, StoredWidth);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetStoredView (aafUInt32 *  pStoredHeight,
        aafUInt32 *  pStoredWidth)
{
  assert(NULL != _dides);
	return _dides->GetStoredView (pStoredHeight, pStoredWidth);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetSampledView (aafUInt32  SampledHeight,
        aafUInt32  SampledWidth,
        aafInt32  SampledXOffset,
        aafInt32  SampledYOffset)
{
  assert(NULL != _dides);
	return _dides->SetSampledView (SampledHeight, SampledWidth, SampledXOffset, SampledYOffset);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetSampledView (aafUInt32 *  pSampledHeight,
        aafUInt32 *  pSampledWidth,
        aafInt32 *  pSampledXOffset,
        aafInt32 *  pSampledYOffset)
{
  assert(NULL != _dides);
	return _dides->GetSampledView (pSampledHeight, pSampledWidth, pSampledXOffset, pSampledYOffset);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetDisplayView (aafUInt32  DisplayHeight,
        aafUInt32  DisplayWidth,
        aafInt32  DisplayXOffset,
        aafInt32  DisplayYOffset)
{
  assert(NULL != _dides);
	return _dides->SetDisplayView (DisplayHeight, DisplayWidth, DisplayXOffset, DisplayYOffset);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetDisplayView (aafUInt32 *  pDisplayHeight,
        aafUInt32 *  pDisplayWidth,
        aafInt32 *  pDisplayXOffset,
        aafInt32 *  pDisplayYOffset)
{
  assert(NULL != _dides);
	return _dides->GetDisplayView (pDisplayHeight, pDisplayWidth, pDisplayXOffset, pDisplayYOffset);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetFrameLayout (aafFrameLayout_t  FrameLayout)
{
  assert(NULL != _dides);
	return _dides->SetFrameLayout (FrameLayout);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetFrameLayout (aafFrameLayout_t *  pFrameLayout)
{
  assert(NULL != _dides);
	return _dides->GetFrameLayout (pFrameLayout);
}


HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetVideoLineMap (aafUInt32  numberElements,
        aafInt32 *  pVideoLineMap)
{
  assert(NULL != _dides);
	return _dides->SetVideoLineMap (numberElements, pVideoLineMap);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetVideoLineMap (aafUInt32  numberElements,
        aafInt32 *  pVideoLineMap)
{
  assert(NULL != _dides);
	return _dides->GetVideoLineMap (numberElements, pVideoLineMap);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetVideoLineMapSize (aafUInt32 *  pNumberElements)
{
  assert(NULL != _dides);
	return _dides->GetVideoLineMapSize (pNumberElements);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetImageAspectRatio (aafRational_t  ImageAspectRatio)
{
  assert(NULL != _dides);
	return _dides->SetImageAspectRatio (ImageAspectRatio);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetImageAspectRatio (aafRational_t *  pImageAspectRatio)
{
  assert(NULL != _dides);
	return _dides->GetImageAspectRatio (pImageAspectRatio);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetAlphaTransparency (aafAlphaTransparency_t  AlphaTransparency)
{
  assert(NULL != _dides);
	return _dides->SetAlphaTransparency (AlphaTransparency);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetAlphaTransparency (aafAlphaTransparency_t *  pAlphaTransparency)
{
  assert(NULL != _dides);
	return _dides->GetAlphaTransparency (pAlphaTransparency);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetGamma (aafRational_t  Gamma)
{
  assert(NULL != _dides);
	return _dides->SetGamma (Gamma);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetGamma (aafRational_t *  pGamma)
{
  assert(NULL != _dides);
	return _dides->GetGamma (pGamma);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetImageAlignmentFactor (aafInt32  ImageAlignmentFactor)
{
  assert(NULL != _dides);
	return _dides->SetImageAlignmentFactor (ImageAlignmentFactor);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetImageAlignmentFactor (aafInt32 *  pImageAlignmentFactor)
{
  assert(NULL != _dides);
	return _dides->GetImageAlignmentFactor (pImageAlignmentFactor);
}





HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetComponentWidth (aafInt32  ComponentWidth)
{
	assert(NULL != _cdcides);
	return _cdcides->SetComponentWidth (ComponentWidth);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetComponentWidth (aafInt32 *  pComponentWidth)
{
	assert(NULL != _cdcides);
	return _cdcides->GetComponentWidth (pComponentWidth);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetHorizontalSubsampling (aafUInt32  HorizontalSubsampling)
{
	assert(NULL != _cdcides);
	return _cdcides->SetHorizontalSubsampling (HorizontalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetHorizontalSubsampling (aafUInt32 *  pHorizontalSubsampling)
{
	assert(NULL != _cdcides);
	return _cdcides->GetHorizontalSubsampling (pHorizontalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetColorSiting (aafColorSiting_t  ColorSiting)
{
	assert(NULL != _cdcides);
	return _cdcides->SetColorSiting (ColorSiting);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetColorSiting (aafColorSiting_t *  pColorSiting)
{
	assert(NULL != _cdcides);
	return _cdcides->GetColorSiting (pColorSiting);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetBlackReferenceLevel (aafUInt32  BlackReferenceLevel)
{
	assert(NULL != _cdcides);
	return _cdcides->SetBlackReferenceLevel (BlackReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetBlackReferenceLevel (aafUInt32 *  pBlackReferenceLevel)
{
	assert(NULL != _cdcides);
	return _cdcides->GetBlackReferenceLevel (pBlackReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetWhiteReferenceLevel (aafUInt32  WhiteReferenceLevel)
{
	assert(NULL != _cdcides);
	return _cdcides->SetWhiteReferenceLevel (WhiteReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetWhiteReferenceLevel (aafUInt32 *  pWhiteReferenceLevel)
{
	assert(NULL != _cdcides);
	return _cdcides->GetWhiteReferenceLevel (pWhiteReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetColorRange (aafUInt32  ColorRange)
{
	assert(NULL != _cdcides);
	return _cdcides->SetColorRange (ColorRange);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetColorRange (aafUInt32 *  pColorRange)
{
	assert(NULL != _cdcides);
	return _cdcides->GetColorRange (pColorRange);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::SetPaddingBits (aafInt16  PaddingBits)
{
	assert(NULL != _cdcides);
	return _cdcides->SetPaddingBits (PaddingBits);
}



HRESULT STDMETHODCALLTYPE
    CJPEGDescriptorHelper::GetPaddingBits (aafInt16 *  pPaddingBits)
{
	assert(NULL != _cdcides);
	return _cdcides->GetPaddingBits (pPaddingBits);
}

// Constructor

CAAFJPEGCodec::CAAFJPEGCodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown),
	_access(NULL),
	_stream(NULL),
	_openMode(kMediaOpenReadOnly),
	_compressEnable(kSDKCompressionEnable),
	_length(0),  // 64 bit int
	_isInAAFContainer(AAFFalse), // not in an aaf file
	_sampleRate(NULL_RATIONAL),
	_containerFormat(NULL_ID),
	_codecID(CodecJPEG),
	_storedHeight(0),
	_storedWidth(0),
	_sampledHeight(0),  // SampledView
	_sampledWidth(0),
	_sampledXOffset(0),
	_sampledYOffset(0),
	_displayHeight(0),  // SampledView
	_displayWidth(0),
	_displayXOffset(0),
	_displayYOffset(0),
	_frameLayout(kFullFrame),
	_videoLineMapSize(sizeof(_videoLineMap)/sizeof(aafInt32)),
	_imageAspectRatio(NULL_RATIONAL),
	_alphaTransparency(kMinValueTransparent),
	_gamma(NULL_RATIONAL),
	_imageAlignmentFactor(0),
	_componentWidth(0),
	_horizontalSubsampling(1),
	_verticalSubsampling(1),
	_colorSiting(kCoSiting),
	_blackReferenceLevel(0),
	_whiteReferenceLevel(0),
	_colorRange(0),
	_paddingBits(0),
	_fileBytesPerSample(0),
	_descriptorFlushed(AAFFalse),
	_currentIndex(0),
	_maxIndex(0),
	_frameIndex(NULL),
	_fieldDominance(kNoDominant),
	_memBitsPerPixel(0),
	_bitsPerPixelAvg(0),
	_memBytesPerSample(0),
	_bitsPerSample(0),
	_headerLoaded(false)
{
	// initialize the video line map.
	for (aafUInt32 i = 0; i < _videoLineMapSize; ++i)
		_videoLineMap[i] = 0;

}


// Desctructor

CAAFJPEGCodec::~CAAFJPEGCodec ()
{
	if(_access != NULL)
	{
		_access->Release();
		_access = NULL;
	}
	if(_stream != NULL)
	{
		_stream->Release();
		_stream = NULL;
	}

}


// Only save a new reference.
void CAAFJPEGCodec::SetEssenceStream(IAAFEssenceStream *stream)
{
	if (_stream != stream)
	{
		if (NULL != _stream)
			_stream->Release();
		_stream = stream;
		if (NULL != stream)
			stream->AddRef();
	}
}




HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::Start (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::Finish (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetNumDefinitions (aafInt32 *pDefCount)
{
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedDefinitionID (aafInt32 index, aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = CodecJPEG;		// UID of the JPEG codec definition

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetPluginDescriptorID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AVID_JPEG_PLUGIN;		// UID of the PluginDescriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedDefinitionObject (aafInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	HRESULT hr = S_OK;
	IAAFCodecDef	*codecDef = NULL;
	IAAFDefObject	*obj = NULL;
	aafUID_t		uid;
	
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;

	// Don't we need to validate the index as well? /* TRR??? */
	if (0 > kSupportedDefinitions || kSupportedDefinitions <= index)
		return AAFRESULT_BADINDEX;

	try
	{
		//!!!Later, add in dataDefs supported & filedescriptor class

		// Create the Codec Definition:
		checkResult(dict->CreateInstance(&AUID_AAFCodecDef,
							IID_IAAFCodecDef, 
							(IUnknown **)&codecDef));
		
		// Support "Picture" type of data definition.
		uid = DDEF_Picture;
		checkResult(codecDef->AppendEssenceKind (&uid));

		
		// Initialize the standard Definition properties.
		checkResult(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		uid = CodecJPEG;
		checkResult(obj->Init(&uid, 
		                      const_cast<wchar_t *>(kDisplayName),
		                      const_cast<wchar_t *>(kDescription)));

		// Cleanup
		codecDef->Release();
		codecDef = NULL;


		// Return a reference to our initialized definition object.
		*def = obj; // reference count already incremented, we do not need to call AddRef().
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup on error.
	if (NULL != codecDef)
		codecDef->Release();
	if (NULL != obj)
		obj->Release();


	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDescriptor **descPtr)
{
	HRESULT hr = S_OK;
	IAAFPluginDescriptor	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	aafUID_t				category = AUID_AAFDefObject, manufacturer = MANUF_AVID_PLUGINS;
	aafUID_t				plugID = AVID_JPEG_PLUGIN;
	
	if ((NULL == dict) || (NULL == descPtr))
		return AAFRESULT_NULL_PARAM;
	
	// Initialize return parameter.
	*descPtr = NULL;

	try
	{
		checkResult(dict->CreateInstance(&AUID_AAFPluginDescriptor,
			IID_IAAFPluginDescriptor, 
			(IUnknown **)&desc));

		checkResult(desc->Init(&plugID,
		                       const_cast<wchar_t *>(kDisplayName),
		                       const_cast<wchar_t *>(kDescription)));
		checkResult(desc->SetCategoryClass(&category));
		checkResult(desc->SetPluginVersionString(kManufRev));
		checkResult(desc->SetManufacturerID(&manufacturer));
		checkResult(desc->SetPluginManufacturerName(kManufName));
		checkResult(desc->SetIsSoftwareOnly(AAFTrue));
		checkResult(desc->SetIsAccelerated(AAFFalse));
		checkResult(desc->SetSupportsAuthentication(AAFFalse));

		// Create the network locator for the Manufacturer's web site: 
		checkResult(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kManufURL));
		checkResult(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		checkResult(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		
		// Create a Network locator to point to our default download site.
		checkResult(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kDownloadURL));
		checkResult(desc->AppendLocator(pLoc));
	
		pLoc->Release();
		pLoc = NULL;

		*descPtr = desc; // reference count already incremented, we do not need to call AddRef()
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup...
	if (NULL != desc)
		desc->Release();
	if (NULL != pLoc)
		pLoc->Release();
	if (NULL != pNetLoc)
		pNetLoc->Release();


	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	if(_access != NULL)
		_access->Release();

	_access = access;

	if(access != NULL)
		access->AddRef();

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetFlavourCount(aafInt32 *pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pCount = kSupportedCodeFlavours;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedFlavourID (aafInt32  index,
        aafUID_t *  pVariant)
{
	if(pVariant == NULL)
		return AAFRESULT_NULL_PARAM;

	*pVariant = NilCodecFlavour;

	return AAFRESULT_SUCCESS;
}


	
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::SetCompressionEnabled (aafBool  enable)
{
	// Translate our bool enum into the compression enum.
	_compressEnable = (enable == AAFTrue) ? kSDKCompressionEnable : kSDKCompressionDisable;
  return HRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetDataDefinitionCount (aafInt32 *pDefCount)
{
	if (NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedDataDefinition (aafInt32  index,
        aafUID_t *  pVariant)
{
	if (NULL == pVariant)
		return AAFRESULT_NULL_PARAM;

  return HRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetMaxCodecDisplayNameLength (
        aafInt32  *bufSize)
{
	if (NULL == bufSize)
		return AAFRESULT_NULL_PARAM;
	
	*bufSize = sizeof(kDisplayName)/sizeof(wchar_t);

	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetCodecDisplayName (aafUID_t  variant,
        wchar_t *  pName,
        aafInt32  bufSize)
{
	if (NULL == pName)
		return AAFRESULT_NULL_PARAM;
	if (0 >= bufSize)
		return AAFRESULT_INVALID_PARAM;

	aafInt32	len = sizeof(kDisplayName);
	if(len > bufSize)
		len = bufSize;
	memcpy(pName, kDisplayName, len);
	return AAFRESULT_SUCCESS;
}
	
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetNumChannels (IAAFSourceMob *fileMob,
        aafUID_t  essenceKind,
        IAAFEssenceStream *stream,
        aafInt16 *  pNumChannels)
{
	if (NULL == fileMob || NULL == stream || NULL == pNumChannels)
		return AAFRESULT_NULL_PARAM;

	if (EqualAUID(&DDEF_Picture, &essenceKind))
		*pNumChannels = 1;
	else
		*pNumChannels = 0;
	
	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetSelectInfo (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
        aafSelectInfo_t *  pSelectInfo)
{
	HRESULT hr = S_OK;
	aafUInt32		storedWidth, storedHeight;
	aafInt16		padBits;
	aafInt32		compWidth, bitsPerPixel;
	aafUInt32	subSampling;
	aafRational_t sampleRate = NULL_RATIONAL;

	
	if (NULL == fileMob || NULL == stream || NULL == pSelectInfo)
		return AAFRESULT_NULL_PARAM;


	try
	{
		// Make sure that given file mob contains the expected descriptor.
		CJPEGDescriptorHelper descriptorHelper;
		checkResult(descriptorHelper.Initialize(fileMob));

		// Get the compression code id from the descriptor and make sure that we
		// can handle it in this codec.
		aafUID_t codecID = NULL_ID;
		hr = descriptorHelper.GetCompression(&codecID);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_SUCCESS == hr && EqualAUID(&codecID, &CodecJPEG))
		{
			pSelectInfo->willHandleMDES = AAFTrue;
		}
		else
		{ // If the compression property was not present or the 
			// give compression is not supported the set to false.
			pSelectInfo->willHandleMDES = AAFFalse;
		}


#if PORT_BYTESEX_LITTLE_ENDIAN
		pSelectInfo->isNative = AAFFalse;
#else
		// JPEG/JFIF is always BIG_ENDIAN
		pSelectInfo->isNative = AAFTrue;
#endif

		// This is a software only codec.
		pSelectInfo->hwAssisted = AAFFalse;

		pSelectInfo->relativeLoss = 10;	/* !!! Need to read MDES header here */ // comment from omcJPEG.c

		// Compute the avgBitsPerSec:
		checkResult(descriptorHelper.GetStoredView(&storedHeight, &storedWidth));
		checkResult(descriptorHelper.GetComponentWidth(&compWidth));
		hr = descriptorHelper.GetPaddingBits(&padBits);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			padBits = 0;
		checkResult(descriptorHelper.GetHorizontalSubsampling(&subSampling));

		switch (subSampling)
		{
			/* 4:4:4 = 1 sample for each of luma and two chroma channels */   
			case 1:
				bitsPerPixel = (compWidth * 3) + padBits;
				break;
				
			/* 4:2:2 = two-pixels get 2 samples of luma, 1 of each chroma channel, avg == 2 */							
			case 2:
				bitsPerPixel = (compWidth * 2) + padBits;					
				break;
		}


//  The following line from the omcJPEG.c (line 397?) appears to be computing the
//  bits per sample not bits per second!
//	info->spc.selectInfo.info.avgBitsPerSec = storedWidth * storedHeight * bitsPerPixel;

		checkResult(descriptorHelper.GetSampleRate(&sampleRate));
		checkExpression(0 < sampleRate.denominator && 0 <= sampleRate.numerator,
			              AAFRESULT_BADRATE);

		pSelectInfo->avgBitsPerSec = (bitsPerPixel * sampleRate.numerator) / sampleRate.denominator;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}
	
	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetNumSamples (
        aafUID_t  essenceKind,
        aafLength_t *  pNumSamples)
{
	if (NULL == pNumSamples)
		return AAFRESULT_NULL_PARAM;

	
	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ValidateEssence (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
		aafCheckVerbose_t  verbose,
        aafCheckWarnings_t warning,
         aafInt32  bufSize,
		wchar_t *  pName,
        aafInt32  *bytesWritten)
{
	if (NULL == fileMob || NULL == stream ||
		  NULL == pName   || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;

	return HRESULT_NOT_IMPLEMENTED;
}

		
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::Create (IAAFSourceMob *unk,
        aafUID_t  variant,
        IAAFEssenceStream * stream,
        aafInt32 numParms,
        aafmMultiCreate_t *createParms)
{
	HRESULT hr = S_OK;
	IAAFSourceMob			*fileMob = NULL;

	
	if (NULL == unk || NULL == stream ||
		  0 == numParms   || NULL == createParms)
		return AAFRESULT_NULL_PARAM;



	try
	{
		// We are allowed to write to the given stream.
		_openMode = kMediaOpenAppend;

		// Save the given essence stream.
		SetEssenceStream(stream);

		_sampleRate = createParms[0].sampleRate;	// !!!Assumes all sample rates are the same
		checkExpression(AAFTrue == EqualAUID(createParms[0].mediaKind, &DDEF_Picture),
			              AAFRESULT_INVALID_DATADEF);


		// Initialize the descriptor helper:
		checkResult(_descriptorHelper.Initialize(unk));
		
		// What are the descriptor properties that need to be initialized at this point???
		// ?????


	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::Open (IAAFSourceMob *unk,
        aafSlotID_t	slotID,
		aafMediaOpenMode_t  openMode,
        IAAFEssenceStream * stream)
{
	HRESULT hr = S_OK;

	
	if (NULL == unk || NULL == stream)
		return AAFRESULT_NULL_PARAM;



	try
	{
		// Save the mode to the given stream.
		_openMode = openMode;

		// Save the given essence stream.
		SetEssenceStream(stream);

		// Initialize the descriptor helper:
		checkResult(_descriptorHelper.Initialize(unk));
		
		// What are the descriptor properties that need to be initialized at this point???
		// ?????


	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup
	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	
	return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ReadBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{

	return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::Seek (aafPosition_t  sampleFrame)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CompleteWrite (IAAFSourceMob *fileMob)
{

	return AAFRESULT_NOT_IMPLEMENTED;
}		


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteFractionalSample (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{

	return AAFRESULT_NOT_IMPLEMENTED;
}





HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ReadFractionalSample (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{

	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteRawData (aafUInt32 nSamples, aafDataBuffer_t  buffer,
        aafUInt32  buflen)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ReadRawData (aafUInt32 nSamples,
		aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead,
        aafUInt32 *  samplesRead)
{

	return AAFRESULT_NOT_IMPLEMENTED;
}



	
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CreateDescriptorFromStream (IAAFEssenceStream * pStream,
        IAAFSourceMob *fileMob)
{
	if (NULL == pStream || NULL == fileMob)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
	if (NULL == ppStream)
		return AAFRESULT_NULL_PARAM;
	if (NULL == _stream)
		return AAFRESULT_NULLOBJECT;
	
	*ppStream = _stream;
	_stream->AddRef();

	return AAFRESULT_SUCCESS;
}

		
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	if (NULL == pFormat)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
{
	if (NULL == pTemplate || NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = NULL;

	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = NULL;

	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFCDCIDescriptor; // stored class UID of Compressed Digital Image Decriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetEssenceDataID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFEssenceData;			// stored class UID of the JPEG data object

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedSampleSize (aafUID_t dataDefID, aafPosition_t pos, aafLength_t *pResult)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetLargestSampleSize (aafUID_t dataDefID, aafLength_t *pResult)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::AddSampleIndexEntry (aafPosition_t pos)
{
  return HRESULT_NOT_IMPLEMENTED;
}





//
// COM Infrastructure
// 

// What interfaces does this plugin support
// Override of CAAFUnknown method.
HRESULT CAAFJPEGCodec::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFEssenceCodec) 
    { 
        *ppvObj = (IAAFEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (riid == IID_IAAFPlugin) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// COM Factory function.
// Define the contrete object support implementation.
// 
HRESULT CAAFJPEGCodec::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFJPEGCodec *pAAFJPEGCodec = new CAAFJPEGCodec(pUnkOuter);
 	if (NULL == pAAFJPEGCodec)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceCodec *>(pAAFJPEGCodec);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }

