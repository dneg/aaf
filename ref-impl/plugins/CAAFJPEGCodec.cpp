/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
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


#include "jpegesdata.h"


// {0DB382D1-3BAC-11d3-BFD6-00104BC9156D}
const CLSID CLSID_AAFJPEGCodec = 
{ 0xdb382d1, 0x3bac, 0x11d3, { 0xbf, 0xd6, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };

// The minimum number of elements added to the current sample index
// during a resize operation. (value same as in omcJPEG.c)
const aafUInt32 kDefaultSampleIndexIncrement = 1024;

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
const aafRational_t DEFAULT_ASPECT_RATIO = {4, 3};

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

inline void checkAssertion(bool test)
{
  if (!test)
    throw HRESULT(AAFRESULT_ASSERTION_VIOLATION);
}




// Constructor

CAAFJPEGCodec::CAAFJPEGCodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{

	_nativeByteOrder = GetNativeByteOrder();
	_access = NULL;
	_stream = NULL;
	_openMode = kMediaOpenReadOnly;
	_compressEnable = kSDKCompressionEnable;
	_length = 0;  // 64 bit int
	_isInAAFContainer = AAFFalse; // not in an aaf file
	_sampleRate = NULL_RATIONAL;
	_containerFormat = NULL_ID;
	_codecID = CodecJPEG;
	_storedHeight = 0;
	_storedWidth = 0;
	_sampledHeight = 0;  // SampledView
	_sampledWidth = 0;
	_sampledXOffset = 0;
	_sampledYOffset = 0;
	_displayHeight = 0;  // SampledView
	_displayWidth = 0;
	_displayXOffset = 0;
	_displayYOffset = 0;
	_frameLayout = kFullFrame;
	_videoLineMapSize = sizeof(_videoLineMap)/sizeof(aafInt32);

	// initialize the video line map.
	for (aafUInt32 i = 0; i < _videoLineMapSize; ++i)
		_videoLineMap[i] = 0;

	_imageAspectRatio = NULL_RATIONAL;
	_alphaTransparency = kMinValueTransparent;
	_gamma = NULL_RATIONAL;
	_imageAlignmentFactor = 0;
	_componentWidth = 0;
	_horizontalSubsampling = 1;
	_verticalSubsampling = 1;
	_colorSiting = kCoSiting;
	_blackReferenceLevel = 0;
	_whiteReferenceLevel = 0;
	_colorRange = 0;
	_paddingBits = 0;
	_imageHeight = 0;
	_imageWidth = 0;
	_fileBytesPerSample = 0;
	_descriptorFlushed = AAFFalse;
	_startingIndex = 0;
	_currentIndex = 0;
	_writeIndex = 0;
	_maxIndex = 0;
	_sampleIndex = NULL;
	_pixelFormat = kColorSpaceRGB;
	_fieldDominance = kNoDominant;
	_memBitsPerPixel = 0;
	_bitsPerPixelAvg = 0;
	_memBytesPerSample = 0;
	_bitsPerSample = 0;
	_numberOfSamples = 0;
	_padBytesPerRow = 0;
	_headerLoaded = false;
	
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


	if (NULL != _sampleIndex)
	{
		delete [] _sampleIndex;
		_sampleIndex = NULL;
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

void CAAFJPEGCodec::SetNumberOfSamples(const aafLength_t& numberOfSamples)
{
	_numberOfSamples = numberOfSamples;
}

void CAAFJPEGCodec::SetCurrentIndex(aafUInt32 currentIndex)
{
	_currentIndex = currentIndex;
}

void CAAFJPEGCodec::SetWriteIndex(aafUInt32 writeIndex)
{
	_writeIndex = writeIndex;
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
	{
		*pNumChannels = 1;
	}
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
	aafFrameLayout_t frameLayout;
	aafUInt32 numFields = 0;
	aafUInt32 bitsPerSample; // uncompressed.

	
	if (NULL == fileMob || NULL == stream || NULL == pSelectInfo)
		return AAFRESULT_NULL_PARAM;


	try
	{
		// Make sure that given file mob contains the expected descriptor.
		CAAFJPEGDescriptorHelper descriptorHelper;
		checkResult(descriptorHelper.Initialize(fileMob));
		
		// Padding bits must be zero for this codec 
		hr = descriptorHelper.GetPaddingBits(&padBits);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			padBits = 0;

		// Get the compression code id from the descriptor and make sure that we
		// can handle it in this codec.
		aafUID_t codecID = NULL_ID;
		hr = descriptorHelper.GetCompression(&codecID);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_SUCCESS == hr && EqualAUID(&codecID, &CodecJPEG) && 0 == padBits)
		{
			pSelectInfo->willHandleMDES = AAFTrue;
		}
		else
		{ // If the compression property was not present or the 
			// give compression is not supported or padBits was not zero
			// the set to false.
			pSelectInfo->willHandleMDES = AAFFalse;
		}


		// JPEG/JFIF is always in big-endian byte order...
		if (INTEL_ORDER == _nativeByteOrder)
			pSelectInfo->isNative = AAFFalse;
		else
			pSelectInfo->isNative = AAFTrue;

		// This is a software only codec.
		pSelectInfo->hwAssisted = AAFFalse;

		pSelectInfo->relativeLoss = 10;	/* !!! Need to read MDES header here */ // comment from omcJPEG.c

		// Compute the avgBitsPerSec:
		checkResult(descriptorHelper.GetStoredView(&storedHeight, &storedWidth));
		checkResult(descriptorHelper.GetComponentWidth(&compWidth));

		checkResult(_descriptorHelper.GetFrameLayout(&frameLayout));

		switch (frameLayout)
		{
			case kFullFrame:
			case kOneField:
				numFields = 1;
				break;
		
			case kSeparateFields:
			case kMixedFields:
				numFields = 2;
				break;
		
			default:
				break;
		} /* end switch */
		
		
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

		bitsPerSample = (bitsPerPixel * storedWidth * storedHeight) * numFields;

		checkResult(descriptorHelper.GetSampleRate(&sampleRate));
		checkExpression(0 < sampleRate.denominator && 0 < sampleRate.numerator,
			              AAFRESULT_BADRATE);


		pSelectInfo->avgBitsPerSec = (bitsPerSample * sampleRate.numerator) / sampleRate.denominator;
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

	if(EqualAUID(&essenceKind, &DDEF_Picture))
	{
		*pNumSamples = _numberOfSamples;
	}
	else
		*pNumSamples = 0;
	
	return AAFRESULT_SUCCESS;
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

	
	if (NULL == unk || NULL == stream || 0 == numParms   || NULL == createParms)
		return AAFRESULT_NULL_PARAM;
	else if (0 < numParms || NULL == createParms->mediaKind)
		return AAFRESULT_INVALID_PARAM;


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
		
		// Use omfi code as a guide...

		// Set the default aspect ratio: (Can't we always determine this from the image dimensions? /* TRR */)
		_imageAspectRatio = DEFAULT_ASPECT_RATIO;

		// The first sample is always at offset 0...of course.E
		// we have not written a sample yet so the next call to AddSampleIndexEntry will
		// allow use the compute the size of the first, 0th, sample.
		checkResult(AddSampleIndexEntry(0));
		// setupStream()

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
	aafUInt32 numFields = 0;

	
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
		// ????? use omcJPEG.c as a guide:


		//
		// FileDescriptor methods:
		//
		checkResult(_descriptorHelper.GetLength(&_length));
		checkAssertion(_length < 0xFFFFFFFF);
		_numberOfSamples = (aafUInt32)_length; // The length in the file descriptor (mdes in omfi) seems to be in samples.
		checkResult(_descriptorHelper.GetIsInContainer(&_isInAAFContainer));
		checkResult(_descriptorHelper.GetSampleRate(&_sampleRate));
		checkResult(_descriptorHelper.GetContainerFormat(&_containerFormat));

		//
		// DigitalImageDescriptor methods:
		//
		checkResult(_descriptorHelper.GetCompression(&_codecID));
		checkAssertion(AAFTrue == EqualAUID(&_codecID, &CodecJPEG));
		checkResult(_descriptorHelper.GetStoredView(&_storedHeight, &_storedWidth));
		_imageHeight = _storedHeight;
		_imageWidth = _storedWidth;
				
		hr = _descriptorHelper.GetSampledView(&_sampledHeight, &_sampledWidth, 
			                                    &_sampledXOffset, &_sampledYOffset);
		checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
		{
			_sampledWidth = _imageWidth;
			_sampledHeight = _imageHeight;
			_sampledXOffset = 0;
			_sampledYOffset = 0;
		}

		hr = _descriptorHelper.GetDisplayView(&_displayHeight, &_displayWidth, 
			                                    &_displayXOffset, &_displayYOffset);
		checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
		{
			_displayWidth = _imageWidth;
			_displayHeight = _imageHeight;
			_displayXOffset = 0;
			_displayYOffset = 0;
		}

		checkResult(_descriptorHelper.GetImageAspectRatio(&_imageAspectRatio));
		checkResult(_descriptorHelper.GetFrameLayout(&_frameLayout));

		switch (_frameLayout)
		{
			case kFullFrame:
			case kOneField:
				numFields = 1;
				break;
		
			case kSeparateFields:
			case kMixedFields:
				numFields = 2;
				break;
		
			default:
				break;
		} /* end switch */

		// Get VideoLineMap
		if (kFullFrame != _frameLayout)
		{
			hr = _descriptorHelper.GetVideoLineMapSize(&_videoLineMapSize);
			checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
			if (AAFRESULT_PROP_NOT_PRESENT == hr || 0 == _videoLineMapSize)
			{
				_videoLineMap[0] = _videoLineMap[1] = 0;
			}
			else
			{
				checkAssertion(2 > _videoLineMapSize);
				checkResult(_descriptorHelper.GetVideoLineMap(_videoLineMapSize, _videoLineMap));

				if (_frameLayout != kSeparateFields && _frameLayout != kMixedFields && 2 == _videoLineMapSize)
				{
					// There were two elements in the video line mab no corresponding froma layout type.
					// Should this be an error in the descriptor? 
					_videoLineMap[1] = 0;
				}
			}
		} // if (kFullFrame != _frameLayout)


		// Computer the file bytes per sample.
		_fileBytesPerSample = 0;
		checkResult(_descriptorHelper.GetComponentWidth(&_componentWidth));
		checkExpression(8 == _componentWidth, AAFRESULT_JPEGBASELINE); // only 8-bit supported

		// The is true for RGB or YUV. (no alpha support)
		_memBitsPerPixel = _componentWidth * 3;

		checkResult(_descriptorHelper.GetHorizontalSubsampling(&_horizontalSubsampling));
//		checkResult(_descriptorHelper.GetVerticalSubsampling(&_verticalSubsampling));
		checkResult(_descriptorHelper.GetPaddingBits(&_paddingBits));

		/* 4:4:4 = 1 sample for each of luma and two chroma channels */   
		if (_horizontalSubsampling == 1)
		{
			_bitsPerPixelAvg = (aafInt16)((_componentWidth * 3) + _paddingBits);
			_bitsPerSample = (aafInt32) _imageWidth * (aafInt32) _imageHeight *
	  								(_componentWidth * 3) * numFields;
	  }							
	  /* 4:2:2 = two-pixels get 2 samples of luma, 1 of each chroma channel, avg == 2 */							
	  else if (_horizontalSubsampling == 2)						
		{
			_bitsPerPixelAvg = (aafInt16)((_componentWidth * 2) + _paddingBits);
			_bitsPerSample = (aafInt32) _imageWidth * (aafInt32) _imageHeight *
	  								(_componentWidth * 2) * numFields;
	  }							
	  else
	  	throw HRESULT(AAFRESULT_ILLEGAL_FILEFMT);
	  								
		_fileBytesPerSample = (_bitsPerSample + 7) / 8;
		_memBytesPerSample = _fileBytesPerSample;


		hr = _descriptorHelper.GetColorSiting(&_colorSiting);
		checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_colorSiting = kCoSiting;

		hr = _descriptorHelper.GetBlackReferenceLevel(&_blackReferenceLevel);
		checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_blackReferenceLevel = 0;

		hr = _descriptorHelper.GetWhiteReferenceLevel(&_whiteReferenceLevel);
		checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_whiteReferenceLevel = (1U << _componentWidth) - 1;

		hr = _descriptorHelper.GetColorRange(&_colorRange);
		checkResult(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_colorRange = (1U << _componentWidth) - 2;

		// Get the compression factor?
		//
		//status = omfsReadInt32(main, media->mdes, pers->omAvJPEDResolutionID, &pdata->jpeg.JPEGTableID);
		//if (status != OM_ERR_NONE)
		//	pdata->jpeg.JPEGTableID = 0;
		
		// Attempt to create and load the sample index from the end of the essence stream.
		// For now just fail if the index cannot be found.
		aafLength_t samples;
		checkResult(ReadNumberOfSamples(_stream, samples));
		checkResult(AllocateSampleIndex(samples));
		SetNumberOfSamples(samples);
		checkResult(ReadSampleIndex());

		// Initialize the positioning indecies.
		_startingIndex = static_cast<aafUInt32>(samples - 1);

		// We can read from the beginning of the stream for both read only and append mode.
		SetCurrentIndex(0);

		// Set the index for the next value write (of open for append).
		SetWriteIndex(_startingIndex + 1);
		
		// Just in case...remap the "not present" error to success...
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			hr = AAFRESULT_SUCCESS;
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
	HRESULT hr = S_OK;
	aafUInt32 n;

	if (NULL == resultBlock)
		return AAFRESULT_NULL_PARAM;
	
	// Initialize the return values.
	for (n = 0; n < (aafUInt32)xferBlockCount; n++)
	{
		resultBlock[n].bytesXfered = 0;
		resultBlock[n].samplesXfered = 0;
	}
	
	if (NULL == xferBlock)
		return AAFRESULT_NULL_PARAM;
	// We only support writing a single block at a time.
	else if (1 != xferBlockCount)
		return AAFRESULT_ONESAMPLEWRITE;
	// this codec only handles a single channel
	else if (1 != xferBlock[0].subTrackNum)
		return AAFRESULT_CODEC_CHANNELS;
	else if (0 == xferBlock[0].numSamples)
		return AAFRESULT_INVALID_PARAM;


	try
	{
		checkExpression(_componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE);
		/* this codec only allows one-channel media */

		if (kSDKCompressionEnable == _compressEnable)
		{
			// If we are being asked compress the given buffer then
			// the we should have already calculated the size of a sample.
			checkExpression(_fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);
			
			// Setup the compression parameters.
			aafCompressionParams param;

			// Get the dimensions of the image data to compress.
			param.imageWidth = _imageWidth;
			param.imageHeight = _imageHeight;
//			param.imageHeight = (kSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;

			param.colorSpace = _pixelFormat;
			param.horizontalSubsampling = _horizontalSubsampling;
			param.verticalSubsampling = _verticalSubsampling;
			param.blackReferenceLevel = _blackReferenceLevel;
			param.whiteReferenceLevel = _whiteReferenceLevel;
			param.colorRange = _colorRange;

			// Default quality (until we have support for custom tables.)
			param.quality = 75; 

			// Compute the number of bytes in a single row of pixel data.
			param.rowBytes = (_imageWidth * 3) + _padBytesPerRow;

			// Calculate the size of the sample data to be compressed.
			param.bufferSize = param.rowBytes * param.imageHeight;

			// Make sure the given buffer is really large enough for the complete
			// uncompressed pixel data.
			checkExpression(param.bufferSize < xferBlock[0].buflen, AAFRESULT_SMALLBUF);
			
			// Adjust the parameters for separate fields...
			if (kSeparateFields == _frameLayout)
			{
				param.imageHeight /= 2;
				param.bufferSize = param.rowBytes * param.imageHeight;
			}
			

			// Make sure that we are already at the end of the stream.
			if (_currentIndex < _writeIndex)
				Seek(_writeIndex);

			for (n = 0; n < xferBlock[0].numSamples; n++)
			{
				param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
				checkResult(CompressImage(param));
				resultBlock[0].bytesXfered += param.bufferSize;

				if (kSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
					checkResult(CompressImage(param));
					resultBlock[0].bytesXfered += param.bufferSize;
				}
				
				// Update the return values.
				resultBlock[0].samplesXfered++;

				// Add a new entry to the index and update the sample count...
				AddNewCompressedSample();
			}
		}
		else
		{
			// Data is already compressed so we can just write the data 
			// using the "raw" interface. WriteRawData will update frame index if necessary.
			checkResult(WriteRawData(xferBlock[0].numSamples,
			                         xferBlock[0].buffer,
			                         xferBlock[0].buflen));
			
			// Update the return values...
			resultBlock[0].bytesXfered = xferBlock[0].buflen;
			resultBlock[0].samplesXfered += xferBlock[0].numSamples;
		}
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
    CAAFJPEGCodec::ReadBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	HRESULT hr = S_OK;
	aafUInt32 n;

	if (NULL == resultBlock)
		return AAFRESULT_NULL_PARAM;
	
	// Initialize the return values.
	for (n = 0; n < (aafUInt32)xferBlockCount; n++)
	{
		resultBlock[n].bytesXfered = 0;
		resultBlock[n].samplesXfered = 0;
	}
	
	if (NULL == xferBlock)
		return AAFRESULT_NULL_PARAM;
	// We only support writing a single block at a time.
	else if (1 != xferBlockCount)
		return AAFRESULT_ONESAMPLEWRITE;
	// this codec only handles a single channel
	else if (1 != xferBlock[0].subTrackNum)
		return AAFRESULT_CODEC_CHANNELS;


	try
	{
		if (kSDKCompressionEnable == _compressEnable)
		{
			// If we are being asked decompress the given buffer.
			
			// Setup the decompression parameters.
			aafCompressionParams param;

			// Get the dimensions of the image data to compress.
			param.imageWidth = _imageWidth;
			param.imageHeight = (kSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;

			param.colorSpace = _pixelFormat;
			param.horizontalSubsampling = _horizontalSubsampling;
			param.verticalSubsampling = _verticalSubsampling;
			param.blackReferenceLevel = _blackReferenceLevel;
			param.whiteReferenceLevel = _whiteReferenceLevel;
			param.colorRange = _colorRange;

			// Default quality (until we have support for custom tables.)
			param.quality = 75; // ignored for input.

			// Compute the number of bytes in a single row of pixel data.
			param.rowBytes = (xferBlock[0].buflen / xferBlock[0].numSamples) / _imageHeight;

			// Calculate the size of the image data to be compressed.
			param.bufferSize = param.rowBytes * param.imageHeight;

			for (n = 0; n < xferBlock[0].numSamples; n++)
			{
				param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
				checkResult(DecompressImage(param));
				resultBlock[0].bytesXfered += param.bufferSize;

				if (kSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
					checkResult(DecompressImage(param));
					resultBlock[0].bytesXfered += param.bufferSize;
				}

				// Update the current index.
				SetCurrentIndex(_currentIndex + 1);
			}
		}
		else
		{
			// Data is already compressed so we can just write the data 
			// using the "raw" interface. WriteRawData will update frame index if necessary.
			checkResult(ReadRawData(xferBlock[0].numSamples,
			                        xferBlock[0].buflen,
			                        xferBlock[0].buffer,
			                        &resultBlock[0].bytesXfered, 
			                        &resultBlock[0].samplesXfered));
		}

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
    CAAFJPEGCodec::Seek (aafPosition_t  sampleFrame)
{
	HRESULT hr = S_OK;

	try
	{
		checkAssertion(NULL != _stream);
		checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);
		// Validate the index.
		checkExpression(sampleFrame < _numberOfSamples, AAFRESULT_BADFRAMEOFFSET);
		checkExpression(sampleFrame == _numberOfSamples && kMediaOpenAppend == _openMode, AAFRESULT_BADFRAMEOFFSET);
		
		// Get the corresponding offset from the sample index.
		aafUInt32 index = static_cast<aafUInt32>(sampleFrame);

		checkResult(_stream->Seek(_sampleIndex[index]));

		// Update the current index
		SetCurrentIndex(index);
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
    CAAFJPEGCodec::CompleteWrite (IAAFSourceMob *fileMob)
{


	// Flush the jpeg data source

	// Write the sample index onto the end of the stream.

	return AAFRESULT_NOT_IMPLEMENTED;
}		


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteFractionalSample (aafDataBuffer_t  /*buffer*/,
        aafInt32  /*buflen*/)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}





HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ReadFractionalSample (aafUInt32  /*buflen*/,
        aafDataBuffer_t  /*buffer*/,
        aafUInt32 *  /*bytesRead*/)
{

	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteRawData (
			aafUInt32 nSamples, 
			aafDataBuffer_t  buffer,
      aafUInt32  buflen)
{
	HRESULT hr = S_OK;

	// Validate input parameters:
	if (0 == nSamples || NULL == buffer || 0 == buflen)
		return AAFRESULT_NULL_PARAM;
	// For now we only support writing a single sample since it would be too time consuming
	// to scan for SOI and EOI markers...
	else if (1 != nSamples)
		return AAFRESULT_ONESAMPLEWRITE;
	


	try
	{
		// Preconditions:
		checkAssertion(NULL != _stream);
		checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);
		
		// Make sure that we have been opened for append.		
		checkExpression(kMediaOpenAppend == _openMode, AAFRESULT_NOT_WRITEABLE);

		// Make sure that we do not attempt to write before the initial
		// starting index.
		checkExpression(_currentIndex >= _startingIndex, AAFRESULT_NOT_WRITEABLE);
		checkAssertion(_currentIndex <= _writeIndex);

		// Make sure that we are already at the end of the stream.
		if (_currentIndex < _writeIndex)
			Seek(_writeIndex);


		// We should only be able to write to the end of the stream. This should be 
		// enforced by the implementation of the IAAFEssenceStream. This is why we 
		// compute the offset to the start of the from the position after the write 
		// operation just in case the seek position had to be reset to the end by the
		// implementation of the IAAFEssenceStream.

		// Write the compressed sample data.
		checkResult(_stream->Write(buffer, buflen));

		// Add a new entry to the index and update the sample count...
		AddNewCompressedSample();
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
    CAAFJPEGCodec::ReadRawData (aafUInt32 nSamples,
		aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead,
        aafUInt32 *  samplesRead)
{
	HRESULT hr = S_OK;

	// Validate input parameters:
	// For now we only support writing a single sample since it would be too time consuming
	// to scan for SOI and EOI markers...
	if (0 == nSamples || NULL == buffer || 0 == buflen)
		return AAFRESULT_NULL_PARAM;
	else if (1 != nSamples)
		return AAFRESULT_ONESAMPLEWRITE;


	try
	{
		checkAssertion(NULL != _stream);
		checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);

		// Make sure we do not attempt to read beyond the end of the sample stream.
		checkExpression(_currentIndex < _writeIndex, AAFRESULT_EOF);
		
		// Make sure the given buffer is large enougth to hold the current sample
		aafUInt32 sampleSize = static_cast<aafUInt32>(_sampleIndex[_currentIndex + 1] - _sampleIndex[_currentIndex]);
		checkExpression(buflen < sampleSize, AAFRESULT_SMALLBUF);

		aafPosition_t offset;
		checkResult(_stream->GetPosition(&offset));
		checkResult(_stream->Read(buflen, buffer, bytesRead));

		// Update the current index.
		SetCurrentIndex(_currentIndex + 1);
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

// Private data structure for handling essence format specifier data.
const size_t kMaxEssenceFormatData = 64;

typedef struct _aafEssenceFormatData_t
{
	aafUID_t opcode;
	union
	{
		aafUInt8 expData[kMaxEssenceFormatData];
		aafInt32 expUInt32;
		aafInt32 expInt32;
		aafInt16 expUInt16;
		aafInt16 expInt16;
		aafInt16 expUInt8;
		aafInt16 expInt8;
		aafRational_t expRational;
		aafRect_t expRect;
		aafColorSpace_t expColorSpace;
		aafCompArray_t expCompArray;
		aafCompSizeArray_t expCompSizeArray;
		aafFrameLayout_t expFrameLayout;
		aafVideoLineMap_t expVideoLineMap;
		aafColorSiting_t expColorSiting;
	} operand;
} aafEssenceFormatData_t;


		
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	HRESULT hr = S_OK;
	aafInt32		numSpecifiers, i, bytesRead;
	aafEssenceFormatData_t param;
	aafUInt32 numFields = 0;
	aafFrameLayout_t frameLayout = _frameLayout;
	aafUInt32 horizontalSubsampling = _horizontalSubsampling;

	if (NULL == pFormat)
		return AAFRESULT_NULL_PARAM;


	try
	{
		checkResult(pFormat->NumFormatSpecifiers (&numSpecifiers));

		// Get the frame layout first so that we can figure out how many fields
		// there are in the sample. We 
		for (i = 0; i < numSpecifiers; ++i)
		{
			memset(&param, 0, sizeof(param));
			checkResult(pFormat->GetIndexedFormatSpecifier (i, 
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &bytesRead));

			if (EqualAUID(&kAAFFrameLayout, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expFrameLayout), AAFRESULT_INVALID_PARM_SIZE);
				// Validate the frame layout:
				switch (param.operand.expFrameLayout)
				{
					case kFullFrame:
					case kOneField:
						numFields = 1;
						break;
				
					case kSeparateFields:
					case kMixedFields:
						numFields = 2;
						break;
				
					default:
						// Invalid frame layout!
						checkExpression(numFields != 0, AAFRESULT_BADEXPORTLAYOUT);
						break;
				} /* end switch */	

				// Save the frame layout
				frameLayout = param.operand.expFrameLayout;

				// What do we do if the codec is asked to change the frame layout
				// after samples have already been written to the stream? This
				// should be a error in essence access...
				checkAssertion(frameLayout != _frameLayout && 0 < _numberOfSamples);
			}
			else if (EqualAUID(&kAAFCDCIHorizSubsampling, &param.opcode))
			{
				horizontalSubsampling = param.operand.expUInt32;
				switch (horizontalSubsampling)
				{
				case 1:
				case 2:
					break;
					
				default:
					throw HRESULT(AAFRESULT_BADPIXFORM);
				}
			}
		}


		for (i = 0; i < numSpecifiers; ++i)
		{
			memset(&param, 0, sizeof(param));
			checkResult(pFormat->GetIndexedFormatSpecifier (i, 
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &bytesRead));
			
			if (EqualAUID(&kAAFNumChannels, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expInt32), AAFRESULT_INVALID_PARM_SIZE);
				// We only support a single channel.
				checkExpression(1 == param.operand.expInt32, AAFRESULT_CODEC_CHANNELS);
			}
			else if (EqualAUID(&kAAFVideoLineMap, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expVideoLineMap), AAFRESULT_INVALID_PARM_SIZE);
				_videoLineMap[0] = param.operand.expVideoLineMap[0];
				_videoLineMap[1] = param.operand.expVideoLineMap[1];
			}
			else if (EqualAUID(&kAAFPixelSize, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expInt16), AAFRESULT_INVALID_PARM_SIZE);
				// This is a calculated value!
				//_bitsPerPixelAvg = param.operand.expInt16;
			}
			else if (EqualAUID(&kAAFStoredRect, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
				
				_storedWidth = param.operand.expRect.xSize;
				_storedHeight = param.operand.expRect.ySize;
				_imageWidth = _storedWidth;
				_imageHeight = _storedHeight;
			}
			else if (EqualAUID(&kAAFSampledRect, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
				
				_sampledXOffset = param.operand.expRect.xOffset;
				_sampledYOffset = param.operand.expRect.yOffset;
				_sampledHeight = param.operand.expRect.ySize;
				_sampledWidth = param.operand.expRect.xSize;
				_sampledHeight = param.operand.expRect.ySize;
			}
			else if (EqualAUID(&kAAFDisplayRect, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
				
				_displayXOffset = param.operand.expRect.xOffset;
				_displayYOffset = param.operand.expRect.yOffset;
				_displayHeight = param.operand.expRect.ySize;
				_displayWidth = param.operand.expRect.xSize;
				_displayHeight = param.operand.expRect.ySize;
			}
			else if (EqualAUID(&kAAFAspectRatio, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expRational), AAFRESULT_INVALID_PARM_SIZE);
				
				_imageAspectRatio = param.operand.expRational;
			}
			else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expInt16), AAFRESULT_INVALID_PARM_SIZE);
				
				// We currently only support 8 bit components...
				checkExpression(8 == param.operand.expInt16, AAFRESULT_JPEGBASELINE); // only 8-bit supported
				_componentWidth = param.operand.expInt16;
			}
			else if (EqualAUID(&kAAFCDCIColorSiting, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expColorSiting), AAFRESULT_INVALID_PARM_SIZE);
				
				// We currently only support 8 bit components...
				_colorSiting = param.operand.expColorSiting;
			}
			else if (EqualAUID(&kAAFCDCIBlackLevel, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);
				_blackReferenceLevel = param.operand.expUInt32;
			}
			else if (EqualAUID(&kAAFCDCIWhiteLevel, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);
				_whiteReferenceLevel = param.operand.expUInt32;
			}
			else if (EqualAUID(&kAAFCDCIColorRange, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);
				_colorRange = param.operand.expUInt32;
			}
			else if (EqualAUID(&kAAFPadBytesPerRow, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(bytesRead == sizeof(param.operand.expUInt16), AAFRESULT_INVALID_PARM_SIZE);
				_padBytesPerRow = param.operand.expUInt16;
			}
			
			
		} // for (i = 0...)


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
    CAAFJPEGCodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
{
	HRESULT hr = S_OK;
	IAAFEssenceFormat	*fmt = NULL;
	aafInt32 numSpecifiers, i, bytesRead;
	aafEssenceFormatData_t param;

	if (NULL == pTemplate || NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	// Initialize out parameter in case of failure.
	*pResult = NULL;

	try
	{
		// Start with an "empty" file format specifier.
		checkResult(_access->GetEmptyFileFormat(&fmt));
		
		checkResult(pTemplate->NumFormatSpecifiers (&numSpecifiers));

		// Use the data in the given template to create and initialize
		// a corresponding data from the codec.
		for (i = 0; i < numSpecifiers; ++i)
		{
			memset(&param, 0, sizeof(param));
			checkResult(pTemplate->GetIndexedFormatSpecifier (i, 
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &bytesRead));


			if (EqualAUID(&kAAFNumChannels, &param.opcode))
			{	// We only support a single channel...		
				param.operand.expInt32 = kDefaultNumCh;
				checkResult(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(param.operand), (aafDataBuffer_t)&param.operand));
			}
			else if (EqualAUID(&kAAFVideoLineMap, &param.opcode))
			{	// Write out both values from the video line map. The frameLayout will determine which entries are
				// valid.
				param.operand.expVideoLineMap[0] = _videoLineMap[0];
				param.operand.expVideoLineMap[1] = _videoLineMap[1];
				checkResult(fmt->AddFormatSpecifier (kAAFVideoLineMap, sizeof(param.operand.expVideoLineMap), (aafDataBuffer_t)&param.operand.expVideoLineMap));
			}
			else if (EqualAUID(&kAAFPixelSize, &param.opcode))
			{	// Write out the current bitsPerPixel (make sure that this has been computed!
				checkAssertion(0 != _bitsPerPixelAvg);
				param.operand.expInt16 = _bitsPerPixelAvg;
				checkResult(fmt->AddFormatSpecifier (kAAFPixelSize, sizeof(param.operand.expInt16), (aafDataBuffer_t)&param.operand.expInt16));
			}
			else if (EqualAUID(&kAAFStoredRect, &param.opcode))
			{	// Write out the current image dimentions for the default stored rectangle.
				param.operand.expRect.xOffset = param.operand.expRect.yOffset = 0;
				param.operand.expRect.xSize = _imageWidth;
				param.operand.expRect.ySize = _imageWidth;
				checkResult(fmt->AddFormatSpecifier (kAAFStoredRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
			}
			else if (EqualAUID(&kAAFSampledRect, &param.opcode))
			{	// Write out the current sampled rectangle.
				param.operand.expRect.xOffset = _sampledXOffset;
				param.operand.expRect.yOffset = _sampledYOffset;
				param.operand.expRect.ySize = _sampledHeight;
				param.operand.expRect.xSize = _sampledWidth;
				param.operand.expRect.ySize = _sampledHeight;
				checkResult(fmt->AddFormatSpecifier (kAAFSampledRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
			}
			else if (EqualAUID(&kAAFDisplayRect, &param.opcode))
			{	// Write out the current sampled rectangle.
				param.operand.expRect.xOffset = _displayXOffset;
				param.operand.expRect.yOffset = _displayYOffset;
				param.operand.expRect.ySize = _displayHeight;
				param.operand.expRect.xSize = _displayWidth;
				param.operand.expRect.ySize = _displayHeight;
				checkResult(fmt->AddFormatSpecifier (kAAFDisplayRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
			}
			else if (EqualAUID(&kAAFAspectRatio, &param.opcode))
			{	// Write out the current aspect ratio.				
				param.operand.expRational = _imageAspectRatio;
				checkResult(fmt->AddFormatSpecifier (kAAFAspectRatio, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRational));
			}
			else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
			{	// We currently only support 8 bit components...
				checkExpression(8 == _componentWidth, AAFRESULT_JPEGBASELINE); // only 8-bit supported
				param.operand.expInt16 = (aafInt16)_componentWidth;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCICompWidth, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expInt16));
			}
			else if (EqualAUID(&kAAFCDCIColorSiting, &param.opcode))
			{	// Write out the current color siting value.
				param.operand.expColorSiting = _colorSiting;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCIColorSiting, sizeof(param.operand.expColorSiting), (aafDataBuffer_t)&param.operand.expColorSiting));
			}
			else if (EqualAUID(&kAAFCDCIBlackLevel, &param.opcode))
			{	// Write out the current black reference level.
				param.operand.expUInt32 = _blackReferenceLevel;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCIBlackLevel, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}
			else if (EqualAUID(&kAAFCDCIWhiteLevel, &param.opcode))
			{	// Write out the current white reference level.
				param.operand.expUInt32 = _whiteReferenceLevel;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCIWhiteLevel, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}
			else if (EqualAUID(&kAAFCDCIColorRange, &param.opcode))
			{	// Write out the current color range.
				param.operand.expUInt32 = _colorRange;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCIColorRange, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}
			else if (EqualAUID(&kAAFPixelFormat, &param.opcode))
			{	// Write out the current pixel format.
				param.operand.expColorSpace = _pixelFormat;
				checkResult(fmt->AddFormatSpecifier (kAAFPixelFormat, sizeof(param.operand.expColorSpace), (aafDataBuffer_t)&param.operand.expColorSpace));
			}
			else if (EqualAUID(&kAAFRGBCompLayout, &param.opcode) && kColorSpaceRGB ==_pixelFormat)
			{	// Default to standard component order.
				param.operand.expCompArray[0] = 'R';
				param.operand.expCompArray[1] = 'G';
				param.operand.expCompArray[2] = 'B';
				param.operand.expCompArray[3] = 0;
				checkResult(fmt->AddFormatSpecifier (kAAFRGBCompLayout, sizeof(param.operand.expCompArray), (aafDataBuffer_t)&param.operand.expCompArray));
			}
			else if (EqualAUID(&kAAFRGBCompSizes, &param.opcode) && kColorSpaceRGB ==_pixelFormat)
			{	// Default to standard component order.
				param.operand.expCompSizeArray[0] = 8;
				param.operand.expCompSizeArray[1] = 8;
				param.operand.expCompSizeArray[2] = 8;
				param.operand.expCompSizeArray[3] = 0;
				checkResult(fmt->AddFormatSpecifier (kAAFRGBCompSizes, sizeof(param.operand.expCompSizeArray), (aafDataBuffer_t)&param.operand.expCompSizeArray));
			}
			else if (EqualAUID(&kAAFPadBytesPerRow, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expUInt16 = _padBytesPerRow;
				checkResult(fmt->AddFormatSpecifier (kAAFPadBytesPerRow, sizeof(param.operand.expUInt16), (aafDataBuffer_t)&param.operand.expUInt16));
			}
			else
			{	// The given opcode was not handled!
				throw HRESULT(AAFRESULT_INVALID_OP_CODEC);
			}
		}


		// If the format specifier has been completely initialized
		// then return this value to the caller (clear local variable
		// so that we don't perform an extra release).
		*pResult = fmt;
		fmt = NULL;
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
	if (NULL != fmt) // should delete the object on failure
		fmt->Release();

	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
	HRESULT hr = S_OK;
	IAAFEssenceFormat	*fmt = NULL;
	aafEssenceFormatData_t param;

	
	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	// Initialize out parameter in case of failure.
	*pResult = NULL;

	try
	{
		// Start with an "empty" file format specifier.
		checkResult(_access->GetEmptyFileFormat(&fmt));
		
		// This codec only handles a single channel of Picture data.
		param.operand.expInt32 = kDefaultNumCh;
		checkResult(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(param.operand), (aafDataBuffer_t)&param.operand));
		
		// Write the frameLayout as its enum type and size.
		param.operand.expFrameLayout = _frameLayout;
		checkResult(fmt->AddFormatSpecifier (kAAFFrameLayout, sizeof(param.operand.expFrameLayout), (aafDataBuffer_t)&param.operand.expFrameLayout));

		// Write out both values from the video line map. The frameLayout will determine which entries are
		// valid.
		param.operand.expVideoLineMap[0] = _videoLineMap[0];
		param.operand.expVideoLineMap[1] = _videoLineMap[1];
		checkResult(fmt->AddFormatSpecifier (kAAFVideoLineMap, sizeof(param.operand.expVideoLineMap), (aafDataBuffer_t)&param.operand.expVideoLineMap));

		// Write out the current bitsPerPixel (make sure that this has been computed!
		checkAssertion(0 != _bitsPerPixelAvg);
		param.operand.expInt16 = _bitsPerPixelAvg;
		checkResult(fmt->AddFormatSpecifier (kAAFPixelSize, sizeof(param.operand.expInt16), (aafDataBuffer_t)&param.operand.expInt16));

		// Write out the current image dimentions for the default stored rectangle.
		param.operand.expRect.xOffset = param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _imageWidth;
		param.operand.expRect.ySize = _imageWidth;
		checkResult(fmt->AddFormatSpecifier (kAAFStoredRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));

		
		// Write out the current component width.
		param.operand.expInt16 = (aafInt16)_componentWidth;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCICompWidth, sizeof(param.operand.expInt16), (aafDataBuffer_t)&param.operand.expInt16));


		// Set the sample format and color space and component order and size.
		// Initially use the same setup as omcJPEG.c...
		//
		if ((kSDKCompressionEnable == _compressEnable) && (kColorSpaceRGB == _pixelFormat))
		{
			param.operand.expColorSpace = kColorSpaceRGB;
			checkResult(fmt->AddFormatSpecifier (kAAFPixelFormat,  sizeof(param.operand.expColorSpace), (aafDataBuffer_t)&param.operand.expColorSpace));

			// Default to standard component order.
			param.operand.expCompArray[0] = 'R';
			param.operand.expCompArray[1] = 'G';
			param.operand.expCompArray[2] = 'B';
			param.operand.expCompArray[3] = 0;
			checkResult(fmt->AddFormatSpecifier (kAAFRGBCompLayout, sizeof(param.operand.expCompArray), (aafDataBuffer_t)param.operand.expCompArray));
				
			// We currently only support an 8 bit component width in software.
			param.operand.expCompSizeArray[0] = 8;
			param.operand.expCompSizeArray[1] = 8;
			param.operand.expCompSizeArray[2] = 8;
			param.operand.expCompSizeArray[3] = 0;
			checkResult(fmt->AddFormatSpecifier (kAAFRGBCompSizes, sizeof(param.operand.expCompSizeArray), (aafDataBuffer_t)param.operand.expCompSizeArray));
		}
		else
		{
			param.operand.expColorSpace = kColorSpaceYUV;
			checkResult(fmt->AddFormatSpecifier (kAAFPixelFormat, sizeof(param.operand.expColorSpace), (aafDataBuffer_t)&param.operand.expColorSpace)); 
		}


		// If the format specifier has been completely initialized
		// then return this value to the caller (clear local variable
		// so that we don't perform an extra release).
		*pResult = fmt;
		fmt = NULL;
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
	if (NULL != fmt) // should delete the object on failure
		fmt->Release();

	return hr;
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
	HRESULT hr = S_OK;


	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;
	else if(pos < 0 || pos > _numberOfSamples) // zero based sample index.
		return AAFRESULT_EOF;

	// Initialize the return value.
	*pResult = 0;

	try
	{
		if(EqualAUID(&dataDefID, &DDEF_Picture))
		{
			if (kSDKCompressionDisable == _compressEnable)
			{ // The samples are compressed to we can so get the size of the sample
				// from the corresponding element in the sampleIndex.
				checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);

				aafUInt32 index = static_cast<aafUInt32>(pos);
				checkAssertion(index >= 0 && index < _maxIndex);

				// Compute the length from the difference between the current
				// offset and the "next" sample offset.
				*pResult = _sampleIndex[index + 1] - _sampleIndex[index];
			}
			else
			{ // The samples will be uncompressed so return the previously
				// computed value for a single uncompressed sample.
				*pResult = _memBytesPerSample;
			}
		}
		else
		{	// We only support picture...
			return AAFRESULT_CODEC_CHANNELS;
		}
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
    CAAFJPEGCodec::GetLargestSampleSize (aafUID_t dataDefID, aafLength_t *pResult)
{
	HRESULT hr = S_OK;


	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	try
	{
		if(EqualAUID(&dataDefID, &DDEF_Picture))
		{ 
			if (kSDKCompressionDisable == _compressEnable)
			{ // If data is compressed, and will not be software decompressed, find
				// the largest frame by scanning the sample index.  This may take a
				// while.
				checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);
				checkAssertion(_numberOfSamples <= _maxIndex);

				aafUInt32 index;
				aafUInt32 samples = static_cast<aafUInt32>(_numberOfSamples);
				aafLength_t sampleSize = 0;

				if (0 < samples)
				{
					for (index = 0; index < samples; ++index)
					{
						// Compute the length from the difference between the current
						// offset and the "next" sample offset.
						sampleSize = _sampleIndex[index + 1] - _sampleIndex[index];
						if (sampleSize > *pResult)
							*pResult = sampleSize;
					}
					// The returned value to always be greater than zero!
					checkExpression(0 < *pResult, AAFRESULT_BADFRAMEOFFSET);
				}
				else
				{	// As a fallback, if there are no samples written yet then
					// return the uncompressed size.
					*pResult = _memBytesPerSample;
				}
			}
			else
			{ // The samples will be uncompressed so return the previously
				// computed value for a single uncompressed sample.
				*pResult = _memBytesPerSample;
			}
		}
		else
			return AAFRESULT_CODEC_CHANNELS ;
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
    CAAFJPEGCodec::AddSampleIndexEntry (aafPosition_t pos)
{
	HRESULT hr = S_OK;

	try
	{
		//checkAssertion(NULL != _stream);
		// Make sure that the sample index exists.
		checkResult(AllocateSampleIndex(_writeIndex + 1));

		// Save the position and bump the current index location.
		_sampleIndex[_writeIndex] = pos;
		SetWriteIndex(_writeIndex + 1);
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


/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
cplusplus_error_exit (j_common_ptr cinfo)
{
  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to catch block in main compress or decompress routine. */
  throw HRESULT(cinfo->err);
}


// Compress a single image data from the given buffer. Return the actual
// number of bytes written.
HRESULT CAAFJPEGCodec::CompressImage(const aafCompressionParams& param)
{
	HRESULT hr = S_OK;

	if (NULL == param.buffer)
		return AAFRESULT_NULL_PARAM;
	else if (0 >= param.imageWidth || 0 >= param.imageHeight || 0 >= param.rowBytes)
		return AAFRESULT_INVALID_PARAM;
	// We must write something.
	else if (param.bufferSize >= (param.imageHeight * param.rowBytes))
		return AAFRESULT_SMALLBUF;


  /* This struct contains the JPEG compression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   * It is possible to have several such structures, representing multiple
   * compression/decompression processes, in existence at once.  We refer
   * to any one struct (and its associated working data) as a "JPEG object".
   */
  struct jpeg_compress_struct cinfo;

  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * But here we just take the easy way out and use the standard error handler, which will
   * print a message on stderr and call throw an c++ exception if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;

  /* More stuff */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */


	try
	{
		// The stream must have already be created or opened.
		checkAssertion(NULL != _stream);

		// This version only supports compression of RGB data
		checkExpression(kColorSpaceRGB == param.colorSpace && 1 == param.horizontalSubsampling,
		               AAFRESULT_BADPIXFORM);

		// This should already have been calculated.
		checkAssertion(0 < param.rowBytes);


		/* Step 1: allocate and initialize JPEG compression object */

		/* We have to set up the error handler first, in case the initialization
		 * step fails.  (Unlikely, but it could happen if you are out of memory.)
		 * This routine fills in the contents of struct jerr, and returns jerr's
		 * address which we place into the link field in cinfo.
		 */
		cinfo.err = jpeg_std_error(&jerr);
		jerr.error_exit = cplusplus_error_exit;

		/* Now we can initialize the JPEG compression object. */
		jpeg_create_compress(&cinfo);
		

		/* Step 2: specify data destination (eg, an IAAFEssenceStream) */
		/* Note: steps 2 and 3 can be done in either order. */
		jpeg_essencestream_dest(&cinfo, _stream);
		

		/* Step 3: set parameters for compression */

		/* First we supply a description of the input image.
		 * Four fields of the cinfo struct must be filled in:
		 */
		cinfo.image_width = param.imageWidth; 	/* image width and height, in pixels */
		cinfo.image_height = param.imageHeight;
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

		/* Now use the library's routine to set default compression parameters.
		 * (You must set at least cinfo.in_color_space before calling this,
		 * since the defaults depend on the source color space.)
		 */
		jpeg_set_defaults(&cinfo);
	
		/* Now you can set any non-default parameters you wish to.
		 * Here we just illustrate the use of quality (quantization table) scaling:
		 */
		jpeg_set_quality(&cinfo, param.quality, TRUE /* limit to baseline-JPEG values */);


		/* Step 4: Start compressor */

		/* TRUE ensures that we will write a complete interchange-JPEG file.
		 * Pass TRUE unless you are very sure of what you're doing.
		 */
		jpeg_start_compress(&cinfo, TRUE);


		/* Step 5: while (scan lines remain to be written) */
		/*           jpeg_write_scanlines(...); */

		/* Here we use the library's state variable cinfo.next_scanline as the
		 * loop counter, so that we don't have to keep track ourselves.
		 * To keep things simple, we pass one scanline per call; you can pass
		 * more if you wish, though.
		 */

		while (cinfo.next_scanline < cinfo.image_height)
		{
			/* jpeg_write_scanlines expects an array of pointers to scanlines.
			 * Here the array is only one element long, but you could pass
			 * more than one scanline at a time if that's more convenient.
			 */
			row_pointer[0] = & param.buffer[cinfo.next_scanline * param.rowBytes];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}


		/* Step 6: Finish compression */

		jpeg_finish_compress(&cinfo);
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

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	/* And we're done! */
	
	
	return hr;
}


// Decompress a single image from the current position in the stream returning
// the image data in buffer and the actual number of bytes written. Note: bufLen must
// be large enough to hold all of the decompressed data
HRESULT CAAFJPEGCodec::DecompressImage(aafCompressionParams& param)
{
	HRESULT hr = S_OK;


	if (NULL == param.buffer)
		return AAFRESULT_NULL_PARAM;
	// We must be able to read something.
	else if (0 == param.bufferSize)
		return AAFRESULT_SMALLBUF;


  /* This struct contains the JPEG decompression parameters and pointers to
   * working space (which is allocated as needed by the JPEG library).
   */
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */

  /* This struct represents a JPEG error handler.  It is declared separately
   * because applications often want to supply a specialized error handler
   * But here we just take the easy way out and use the standard error handler, which will
   * print a message on stderr and call throw an c++ exception if compression fails.
   * Note that this struct must live as long as the main JPEG parameter
   * struct, to avoid dangling-pointer problems.
   */
  struct jpeg_error_mgr jerr;

  /* More stuff */
  JSAMPROW row_pointer[1];		/* Output row buffer */
	

	try
	{
		// The stream must have already be created or opened.
		checkAssertion(NULL != _stream);

		// This version only supports compression of RGB data
		checkExpression(kColorSpaceRGB == _pixelFormat && 1 == _horizontalSubsampling,
		               AAFRESULT_BADPIXFORM);

		
		/* Step 1: allocate and initialize JPEG decompression object */

		/* We set up the normal JPEG error routines, then override error_exit. */
		cinfo.err = jpeg_std_error(&jerr);
		jerr.error_exit = cplusplus_error_exit;

		/* Now we can initialize the JPEG decompression object. */
		jpeg_create_decompress(&cinfo);


		/* Step 2: specify data source (eg, an IAAFEssenceStream) */
		jpeg_essencestream_src(&cinfo, _stream);


		/* Step 3: read file parameters with jpeg_read_header() */

		(void) jpeg_read_header(&cinfo, TRUE);
		/* We can ignore the return value from jpeg_read_header since
		 *   (a) suspension is not possible with the essence stream data source, and
		 *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
		 * See libjpeg.doc for more info.
		 */


		/* Step 4: set parameters for decompression */

		/* In this example, we don't need to change any of the defaults set by
		 * jpeg_read_header(), so we do nothing here.
		 */


		/* Step 5: Start decompressor */

		(void) jpeg_start_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		 * with the essence stream data source.
		 */

 
		/* We may need to do some setup of our own at this point before reading
		 * the data.  After jpeg_start_decompress() we have the correct scaled
		 * output image dimensions available, as well as the output colormap
		 * if we asked for color quantization.
		 * In this example, we need to make sure the output work buffer is of the right size.
		 */
		checkExpression(((int)cinfo.output_height * param.rowBytes) == param.bufferSize,
		                AAFRESULT_SMALLBUF); // cinfo.output_width

		// Initialize the starting row pointer to the beginning of the buffer.
		row_pointer[0] = &param.buffer[0];


		/* Step 6: while (scan lines remain to be read) */
		/*           jpeg_read_scanlines(...); */

		/* Here we use the library's state variable cinfo.output_scanline as the
		 * loop counter, so that we don't have to keep track ourselves.
		 */
		while (cinfo.output_scanline < cinfo.output_height)
		{
			/* jpeg_read_scanlines expects an array of pointers to scanlines.
			 * Here the array is only one element long, but you could ask for
			 * more than one scanline at a time if that's more convenient.
			 */
			row_pointer[0] = & param.buffer[cinfo.output_scanline * param.rowBytes];
			(void) jpeg_read_scanlines(&cinfo, row_pointer, 1);
		}


		/* Step 7: Finish decompression */

		(void) jpeg_finish_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		 * with the stdio data source.
		 */
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

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);


  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.num_warnings is nonzero).
   */

  /* And we're done! */
	
	
	return hr;
}



// Utility to get the current offset in the stream and add an
// entry to the sample index based on the given compressedDataSize.
void CAAFJPEGCodec::AddNewCompressedSample()
{
	aafPosition_t offset;
	checkResult(_stream->GetPosition(&offset));

	checkResult(AddSampleIndexEntry(offset));

	// Update the number of samples.
	SetNumberOfSamples(_numberOfSamples + 1);

	// Update the current index.
	SetCurrentIndex(_currentIndex + 1);
}



// SampleIndex access methods : may be temporary. We need these methods
// to read/write the index because the SampleIndex property in EssenceData
// is not implemented. Also, these methods will allow all essence streams
// to be treated the sample, whether in an AAF file or not.

const char * kAAFJPEG_end = "AaFjPeGm";
const char * kAAFJPEG_start = "aAfJpEgM";

const int kAAFJPEG_MarkerSize = 8;
const int kAAFJPEG_MinTrailerSize = kAAFJPEG_MarkerSize * 2 + (sizeof(aafLength_t) * 2);

typedef struct _AAFJPEG_Trailer  // written packed.
{
	aafUInt8 startMarker[8]; // All 0xFF.
	aafLength_t numberOfSamples;
	// other stuff in the future???
	aafLength_t sizeOfTrailer; // should be kAAFJPEG_TrailerSize;
	aafUInt8 endMarker[8];
} AAFJPEG_Trailer;



HRESULT CAAFJPEGCodec::ReadNumberOfSamples(
	IAAFEssenceStream *stream,
	aafLength_t& numberOfSamples)
{
	HRESULT hr = S_OK;

	if(NULL == stream)
		return AAFRESULT_NULL_PARAM;

	
	try
	{
		// assume 64bit integer support
		aafLength_t streamSize;
		aafPosition_t offset;
		aafUInt32 bytesRead = 0;

		checkResult(stream->GetLength(&streamSize));
		// Make sure there is enough room to read the trailer.
		checkExpression(streamSize >= kAAFJPEG_MarkerSize, AAFRESULT_NOFRAMEINDEX);
		
		// Read and validate the trailer...
		AAFJPEG_Trailer trailer;
		memset(&trailer, 0, sizeof(trailer));

		// make sure the last 8 bytes match our expected end marker.
		offset = streamSize - 1 - kAAFJPEG_MarkerSize;
		checkResult(stream->Seek(offset));
		checkResult(stream->Read(kAAFJPEG_MarkerSize, trailer.endMarker, &bytesRead));
		checkAssertion(kAAFJPEG_MarkerSize == bytesRead);
		checkExpression(0 == memcmp(kAAFJPEG_end, trailer.endMarker, kAAFJPEG_MarkerSize),
			              AAFRESULT_NOFRAMEINDEX);
		
		// Read the 8 bytes size of the trailer. This is always in big-endian just like 
		// the JPEG stream...
		offset = streamSize - 1 - (kAAFJPEG_MarkerSize + sizeof(aafLength_t));
		checkResult(stream->Seek(offset));
		checkResult(stream->Read(sizeof(aafLength_t), (aafDataBuffer_t)&trailer.sizeOfTrailer, &bytesRead));
		checkAssertion(sizeof(aafLength_t) == bytesRead);
		// Swap the data if necessary...
		if (INTEL_ORDER == _nativeByteOrder)
			AAFByteSwap64((aafInt64 *)&trailer.sizeOfTrailer);

		// Make sure that there are enough bytes in the stream to read the rest of the trailer.
		checkExpression(streamSize >= trailer.sizeOfTrailer, AAFRESULT_NOFRAMEINDEX);
		offset = streamSize - 1 - trailer.sizeOfTrailer;
		checkResult(stream->Seek(offset));

		// Read and validate the startMaker...
		checkResult(stream->Read(kAAFJPEG_MarkerSize, trailer.startMarker, &bytesRead));
		checkAssertion(kAAFJPEG_MarkerSize == bytesRead);
		checkExpression(0 == memcmp(kAAFJPEG_start, trailer.startMarker, kAAFJPEG_MarkerSize),
			              AAFRESULT_NOFRAMEINDEX);

		// Read the numberOfSamples in the sample index.
		checkResult(stream->Read(sizeof(aafLength_t), (aafDataBuffer_t)&trailer.numberOfSamples, &bytesRead));
		checkAssertion(sizeof(aafLength_t) == bytesRead);
		// Swap the data if necessary...
		if (INTEL_ORDER == _nativeByteOrder)
			AAFByteSwap64((aafInt64 *)&trailer.numberOfSamples);

		numberOfSamples = trailer.numberOfSamples;
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

HRESULT CAAFJPEGCodec::AllocateSampleIndex(
	const aafLength_t& numberOfSamples)
{
	HRESULT hr = S_OK;
	aafPosition_t *pSampleIndex = NULL;


	try
	{
		// Allocate the entire index in a single contiguous memory block.
		// If the jpeg essence stream contained enough samples at 30fps for 24 hrs
		// the frame index will be approximately 20MB.
		aafUInt32 oldcount, newcount, index;

		// if we have too many samples for aafUInt32 were in trouble!
		newcount = static_cast<aafUInt32>(numberOfSamples);
		oldcount = static_cast<aafUInt32>(_numberOfSamples); // current sample count

		// Add an extra frame to the sample index so that the last element
		// can contain the position one byte beyond the end of last sample.
		// This offset should be equal to the position of the beginning
		// of the sample index. This means that we can always use 
		// pSampleIndex[index + 1] - pSampleIndex[index] to calculate
		// the sample size for the given (valid) index.
		++newcount;
		++oldcount;

		if (newcount > _maxIndex)
		{
			if (kMediaOpenReadOnly != _openMode)
			{
				// Add extra elements to minimize resizing overhead.
				newcount += kDefaultSampleIndexIncrement;
			}

			pSampleIndex = new aafPosition_t[newcount];
			checkExpression(NULL != pSampleIndex, E_OUTOFMEMORY);

#ifdef _DEBUG
			// Initialize the memory. This is only for debugging since
			// the memory will be completely initialized by ReadSampleIndex.
			for (index = 0; index < newcount; ++index)
				pSampleIndex[index] = 0;
#endif // #ifdef _DEBUG

			if (NULL != _sampleIndex)
			{
				// Copy the old index data into the new index.
				for (index = 0; index < oldcount; ++index)
					pSampleIndex[index] = _sampleIndex[index];

				// Cleanup the old index.
				delete [] _sampleIndex;
				_sampleIndex = NULL;
			}

			// Save the new sample index and update the maximum valid index value.
			_sampleIndex = pSampleIndex;
			_maxIndex = newcount - 1;
		}
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

HRESULT CAAFJPEGCodec::ReadSampleIndex()
{
	HRESULT hr = S_OK;

	
	try
	{
		// PRECONDITION
		checkAssertion(NULL != _stream && NULL != _sampleIndex);

		// Save the starting position.
		aafPosition_t startingPosition;
		checkResult(_stream->GetPosition(&startingPosition));

		// Read the entire index in a single operation to minimize i/o.
		// If the jpeg essence stream contained enough samples at 30fps for 24 hrs
		// the frame index will be approximately 20MB.
		aafUInt32 count, bufferLen, bytesRead;

		// if we have too many samples for aafUInt32 were in trouble!
		count = static_cast<aafUInt32>(_numberOfSamples); 
		bufferLen = count * sizeof(aafPosition_t);
		bytesRead = 0;
		checkResult(_stream->Read(bufferLen, (aafDataBuffer_t)_sampleIndex, &bytesRead));
		
		// Swap the index bytes if necessary.
		if (INTEL_ORDER == _nativeByteOrder)
		{
			aafUInt32 index;
			for (index = 0; index < count; ++index)
			{
				AAFByteSwap64((aafInt64 *)&_sampleIndex[index]);
			}
		}
		
		// Add the extra frame to the sample index so that the last element
		// can contain the position one byte beyond the end of last sample.
		// This offset should be equal to the position of the beginning
		// of the sample index. This means that we can always use 
		// pSampleIndex[index + 1] - pSampleIndex[index] to calculate
		// the sample size for the given (valid) index.
		_sampleIndex[count] = startingPosition;
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



HRESULT CAAFJPEGCodec::WriteSampleIndex()
{
	HRESULT hr = HRESULT_NOT_IMPLEMENTED;

	
	try
	{
		// PRECONDITION
		checkAssertion(NULL != _stream && NULL != _sampleIndex);

		// Save the starting position. This should be after the last byte of
		// jpeg essence.
		aafPosition_t startingPosition;
		checkResult(_stream->GetPosition(&startingPosition));

		
		// Write the entire index in a single operation to minimize i/o.
		// If the jpeg essence stream contained enough samples at 30fps for 24 hrs
		// the frame index will be approximately 20MB.
		aafUInt32 count, bufferLen, index;

		// if we have too many samples for aafUInt32 were in trouble!
		count = static_cast<aafUInt32>(_numberOfSamples); 

		// Swap the index bytes if necessary before writing...
		if (INTEL_ORDER == _nativeByteOrder)
		{
			for (index = 0; index < count; ++index)
			{
				AAFByteSwap64((aafInt64 *)&_sampleIndex[index]);
			}
		}
		
		bufferLen = count * sizeof(aafPosition_t);
		checkResult(_stream->Write((aafDataBuffer_t)_sampleIndex, bufferLen));

#ifdef _DEBUG		
		// Swap the index bytes if necessary. 
		// THIS SHOULD ONLY BE NECESSARY FOR DEBUGGING!!!
		if (INTEL_ORDER == _nativeByteOrder)
		{
			for (index = 0; index < count; ++index)
			{
				AAFByteSwap64((aafInt64 *)&_sampleIndex[index]);
			}
		}
#endif // #ifdef _DEBUG	


		// Write the trailer...
		AAFJPEG_Trailer trailer;
		memset(&trailer, 0, sizeof(trailer));

		memcpy(trailer.startMarker, kAAFJPEG_start, kAAFJPEG_MarkerSize);
		trailer.numberOfSamples = _numberOfSamples;
	
		// other data in the future...

		trailer.sizeOfTrailer = kAAFJPEG_MinTrailerSize; // no additional data...
		memcpy(trailer.endMarker, kAAFJPEG_end, kAAFJPEG_MarkerSize);

		if (INTEL_ORDER == _nativeByteOrder)
		{
			AAFByteSwap64((aafInt64 *)&trailer.sizeOfTrailer);
			AAFByteSwap64((aafInt64 *)&trailer.numberOfSamples);
		}
		
		// Write the trailer packed...
		checkResult(_stream->Write((aafDataBuffer_t)trailer.startMarker, kAAFJPEG_MarkerSize));
		checkResult(_stream->Write((aafDataBuffer_t)&trailer.numberOfSamples, sizeof(aafLength_t)));
		
		// and other data...

		checkResult(_stream->Write((aafDataBuffer_t)&trailer.sizeOfTrailer, sizeof(aafLength_t)));
		checkResult(_stream->Write((aafDataBuffer_t)trailer.endMarker, kAAFJPEG_MarkerSize));
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

    // We support the IAAFEssenceCodec interface 
    if (riid == IID_IAAFEssenceCodec) 
    { 
        *ppvObj = (IAAFEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
		// and the IAAFPlugin interface.
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

