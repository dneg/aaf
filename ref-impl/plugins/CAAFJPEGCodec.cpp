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
#include "CAAFJPEGCodec.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"


// {0DB382D1-3BAC-11d3-BFD6-00104BC9156D}
const CLSID CLSID_AAFJPEGCodec = 
{ 0xdb382d1, 0x3bac, 0x11d3, { 0xbf, 0xd6, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };

const aafUID_t AAF_CMPR_FULL_JPEG = { 0xedb35383, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };

const aafUID_t kAAFPropID_CDCIOffsetToFrameIndexes = { 0x9d15fca3, 0x54c5, 0x11d3, { 0xa0, 0x29, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameIndexByteOrder = { 0xb57e925d, 0x170d, 0x11d4, { 0xa0, 0x8f, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDResolutionID = { 0xce2aca4d, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFirstFrameOffset = { 0xce2aca4e, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameSampleSize = { 0xce2aca50, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };

// The minimum number of elements added to the current sample index
// during a resize operation. (value same as in omcJPEG.c)
const aafUInt32 kDefaultSampleIndexIncrement = 1024;

// This plugin currently only supports a single definition
const aafUInt32 kSupportedDefinitions = 1;

// This plugin currently only supports a single code flavour
const aafUInt32 kSupportedCodeFlavours = 1;

const wchar_t kDisplayName[] = L"AAF JPEG Codec";
const wchar_t kDescription[] = L"Handles Standard JFIF/JPEG";

const aafProductVersion_t kAAFPluginVersion = {1, 0, 0, 1, kAAFVersionBeta};
const aafRational_t		kDefaultRate = { 30000, 1001 };
const aafInt32			kDefaultPixelWidth = 24;
const aafUInt32			kDefaultNumCh = 1;
const aafInt32			kDefaultComponentWidth = 8;

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


const aafUID_t NULL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
const aafRational_t NULL_RATIONAL = {0, 0};
const aafRational_t DEFAULT_ASPECT_RATIO = {4, 3};

// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw HRESULT(r);
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
	_openMode = kAAFMediaOpenReadOnly;
	_compressEnable = kAAFCompressionEnable;
	_length = 0;  // 64 bit int
	_sampleRate = NULL_RATIONAL;
	_containerFormat = NULL_UID;
	_compression = NULL_UID;
	_codecID = kAAFCodecJPEG;
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
	_frameLayout = kAAFFullFrame;
	_videoLineMapSize = sizeof(_videoLineMap)/sizeof(aafInt32);

	// initialize the video line map.
	for (aafUInt32 i = 0; i < _videoLineMapSize; ++i)
		_videoLineMap[i] = 0;

	_imageAspectRatio = NULL_RATIONAL;
	_alphaTransparency = kAAFMinValueTransparent;
	_gamma = NULL_RATIONAL;
	_imageAlignmentFactor = 0;
	_componentWidth = kDefaultComponentWidth;
	_horizontalSubsampling = 1;
	_verticalSubsampling = 1;
	_colorSiting = kAAFCoSiting;
	_blackReferenceLevel = 0;
	_whiteReferenceLevel = 255;
	_colorRange = 0;
	_paddingBits = 0;
	_imageHeight = 0;
	_imageWidth = 0;
	_fileBytesPerSample = 0;
	_descriptorFlushed = kAAFFalse;
	_startingIndex = 0;
	_currentIndex = 0;
	_writeIndex = 0;
	_maxIndex = 0;
	_sampleIndex = NULL;
	_pixelFormat = kAAFColorSpaceRGB;
	_fieldDominance = kAAFNoDominant;
	_memBitsPerPixel = kDefaultPixelWidth;
	_bitsPerPixelAvg = kDefaultPixelWidth;
	_memBytesPerSample = 0;
	_bitsPerSample = 0;
	_numberOfSamples = 0;
	_padBytesPerRow = 0;
	
	_compression_IJG_Quality = 75; // good default quality.

	_rawSampleImage = NULL; /* a 3-D sample array: top index is color */
	_rawSampleArray = NULL;
	_rawSampleImageBuffer = NULL; // ultimate buffer for _sampleImage.
	_rawSampleImageBufferSize = 0;
	_resolutionID = 0;
}


// Desctructor

CAAFJPEGCodec::~CAAFJPEGCodec ()
{
  // Do NOT release the _access interface since this object
  // contains the reference to this codec instance! We need
  // avoid the dreaded reference counting cycle of death!
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

	// Cleanup the raw sample image buffer is one has been allocated.
	if (NULL != _rawSampleImageBuffer)
	{
		delete [] _rawSampleImageBuffer;
		_rawSampleImageBuffer = NULL;
		_rawSampleImageBufferSize = 0;
	}

	if (NULL != _rawSampleArray)
	{
		delete [] _rawSampleArray;
		_rawSampleArray = NULL;
	}

	if (NULL != _rawSampleImage)
	{
		delete [] _rawSampleImage;
		_rawSampleImage = NULL;
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

void CAAFJPEGCodec::SetCompressionEnabled (aafCompressEnable_t compEnable)
{
	// make sure parameter has a valid value.
	_compressEnable = (compEnable == kAAFCompressionEnable) ? kAAFCompressionEnable : kAAFCompressionDisable;
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


aafUInt32 CAAFJPEGCodec::GetSampleSizeFromIndex(const aafPosition_t& pos) // throw HRESULT
{
	// The samples are compressed to we can so get the size of the sample
	// from the corresponding element in the sampleIndex.
	checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);

	aafUInt32 index = static_cast<aafUInt32>(pos);
	checkAssertion(index >= 0 && index < _maxIndex);

	// Compute the length from the difference between the current
	// offset and the "next" sample offset.
	return (static_cast<aafUInt32>( _sampleIndex[index + 1] - _sampleIndex[index]));
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CountDefinitions (aafUInt32 *pDefCount)
{
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedDefinitionID (aafUInt32 index, aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;

	*uid = kAAFCodecJPEG;		// UID of the JPEG codec definition

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

void CAAFJPEGCodec::CreateLegacyPropDefs(IAAFDictionary *p_dict)
{
	IAAFPropertyDef			*pPropertyDef = NULL;
	IAAFTypeDef				*pTypeDef = NULL;
	IAAFClassDef            *pcd = 0;
	IAAFClassDef            *p_did_classdef = 0;

	checkResult(p_dict->LookupClassDef(kAAFClassID_CDCIDescriptor, &pcd));
	checkResult(p_dict->LookupClassDef(kAAFClassID_DigitalImageDescriptor, &p_did_classdef));
	if(pcd->LookupPropertyDef(kAAFPropID_CDCIOffsetToFrameIndexes, &pPropertyDef) != AAFRESULT_SUCCESS)
	{
		checkResult(p_dict->LookupTypeDef(kAAFTypeID_Int32, &pTypeDef));
		checkResult(pcd->RegisterOptionalPropertyDef (kAAFPropID_CDCIOffsetToFrameIndexes,
			L"OffsetToFrameIndexes",
			pTypeDef, &pPropertyDef));
		pTypeDef->Release();
		pTypeDef = NULL;
		pPropertyDef->Release();
		pPropertyDef = NULL;
	}
	if(p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFrameIndexByteOrder, &pPropertyDef) != AAFRESULT_SUCCESS)
	{
		checkResult(p_dict->LookupTypeDef(kAAFTypeID_Int16, &pTypeDef));
		checkResult(p_did_classdef->RegisterOptionalPropertyDef (kAAFPropID_DIDFrameIndexByteOrder,
			L"FrameIndexByteOrder",
			pTypeDef, &pPropertyDef));
		pTypeDef->Release();
		pTypeDef = NULL;
		pPropertyDef->Release();
		pPropertyDef = NULL;
	}		
	if(p_did_classdef->LookupPropertyDef(kAAFPropID_DIDResolutionID, &pPropertyDef) != AAFRESULT_SUCCESS)
	{
		checkResult(p_dict->LookupTypeDef(kAAFTypeID_Int32, &pTypeDef));
		checkResult(p_did_classdef->RegisterOptionalPropertyDef (kAAFPropID_DIDResolutionID,
			L"ResolutionID",
			pTypeDef, &pPropertyDef));
		pTypeDef->Release();
		pTypeDef = NULL;
		pPropertyDef->Release();
		pPropertyDef = NULL;
	}		
	if(p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFirstFrameOffset, &pPropertyDef) != AAFRESULT_SUCCESS)
	{
		checkResult(p_dict->LookupTypeDef(kAAFTypeID_Int32, &pTypeDef));
		checkResult(p_did_classdef->RegisterOptionalPropertyDef (kAAFPropID_DIDFirstFrameOffset,
			L"FirstFrameOffset",
			pTypeDef, &pPropertyDef));
		pTypeDef->Release();
		pTypeDef = NULL;
		pPropertyDef->Release();
		pPropertyDef = NULL;
	}		
	if(p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFrameSampleSize, &pPropertyDef) != AAFRESULT_SUCCESS)
	{
		checkResult(p_dict->LookupTypeDef(kAAFTypeID_Int32, &pTypeDef));
		checkResult(p_did_classdef->RegisterOptionalPropertyDef (kAAFPropID_DIDFrameSampleSize,
			L"FrameSampleSize",
			pTypeDef, &pPropertyDef));
		pTypeDef->Release();
		pTypeDef = NULL;
		pPropertyDef->Release();
		pPropertyDef = NULL;
	}		
	if (pcd)
	{
		pcd->Release ();
		pcd = 0;
	}
	if (p_did_classdef)
	{
		p_did_classdef->Release ();
		p_did_classdef = 0;
	}
	if (pPropertyDef)
	{
		pPropertyDef->Release ();
		pPropertyDef = 0;
	}
	if (pTypeDef)
	{
		pTypeDef->Release ();
		pTypeDef = 0;
	}
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	HRESULT hr = S_OK;
	IAAFCodecDef	*codecDef = NULL;
	IAAFClassDef	*fileClass = NULL;
	IAAFDefObject	*obj = NULL;
	IAAFClassDef    *pcd = 0;
	aafUID_t		uid;
	
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;

	// Don't we need to validate the index as well? /* TRR??? */
	if (0 > kSupportedDefinitions || kSupportedDefinitions <= index)
		return AAFRESULT_BADINDEX;

	try
	{
		//!!!Later, add in dataDefs supported & filedescriptor class
		CreateLegacyPropDefs(dict);

		// Create the Codec Definition:
	    checkResult(dict->LookupClassDef(AUID_AAFCodecDef, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFCodecDef, 
										(IUnknown **)&codecDef));
		pcd->Release();
		pcd = 0;
		
		// Support "Picture" type of data definition.
		CAAFBuiltinDefs defs (dict);
		checkResult(codecDef->AddEssenceKind (defs.ddPicture()));

		
		// Initialize the standard Definition properties.
		checkResult(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		uid = kAAFCodecJPEG;
		checkResult(codecDef->Initialize(uid, 
		                      const_cast<wchar_t *>(kDisplayName),
		                      const_cast<wchar_t *>(kDescription)));

	  	checkResult(dict->LookupClassDef(AUID_AAFWAVEDescriptor, &fileClass));
		checkResult(codecDef->SetFileDescriptorClass (fileClass));
		fileClass->Release ();
		fileClass = 0;

		// Cleanup
		codecDef->Release();
		codecDef = NULL;


		// Return a reference to our initialized definition object.
		*def = obj; // reference count already incremented, we do not need to call AddRef().
		obj = NULL;
		{
			
			
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

	// Cleanup on error.
	if (NULL != codecDef)
	  {
		codecDef->Release();
		codecDef = 0;
	  }
	if (NULL != obj)
	  {
		obj->Release();
		obj = 0;
	  }
	if (pcd)
	  {
		pcd->Release();
		pcd = 0;
	  }
	if (fileClass)
	  {
		fileClass->Release ();
		fileClass = 0;
	  }

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
	HRESULT hr = S_OK;
	IAAFPluginDef	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	IAAFClassDef            *pcd = 0;
	
	if ((NULL == dict) || (NULL == descPtr))
		return AAFRESULT_NULL_PARAM;
	
	// Initialize return parameter.
	*descPtr = NULL;

	try
	{
	    checkResult(dict->LookupClassDef(AUID_AAFPluginDef, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFPluginDef, 
										(IUnknown **)&desc));
		pcd->Release ();
		pcd = 0;

		checkResult(desc->Initialize(AVID_JPEG_PLUGIN,
		                       const_cast<wchar_t *>(kDisplayName),
		                       const_cast<wchar_t *>(kDescription)));
		checkResult(desc->SetCategoryClass(AUID_AAFCodecDef));
		checkResult(desc->SetPluginVersionString(kManufRev));
		checkResult(desc->SetManufacturerID(MANUF_AVID_PLUGINS));
		checkResult(desc->SetPluginManufacturerName(kManufName));
		checkResult(desc->SetIsSoftwareOnly(kAAFTrue));
		checkResult(desc->SetIsAccelerated(kAAFFalse));
		checkResult(desc->SetSupportsAuthentication(kAAFFalse));

		// Create the network locator for the Manufacturer's web site: 
		checkResult(dict->LookupClassDef(AUID_AAFNetworkLocator, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFLocator, 
										(IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kManufURL));
		checkResult(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		checkResult(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		
		// Create a Network locator to point to our default download site.
		checkResult(pcd->CreateInstance(IID_IAAFLocator, 
										(IUnknown **)&pLoc));
		pcd->Release ();
		pcd = 0;
		checkResult(pLoc->SetPath (kDownloadURL));
		checkResult(desc->AppendLocator(pLoc));
	
		pLoc->Release();
		pLoc = NULL;

		*descPtr = desc; // reference count already incremented, we do not need to call AddRef()
		desc = NULL;
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
	  {
		desc->Release();
		desc = 0;
	  }
	if (NULL != pLoc)
	  {
		pLoc->Release();
		pLoc = 0;
	  }
	if (NULL != pNetLoc)
	  {
		pNetLoc->Release();
		pNetLoc = 0;
	  }
	if (pcd)
	  {
		pcd->Release ();
		pcd = 0;
	  }
	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	if(access == NULL)
		return AAFRESULT_NULL_PARAM;

	_access = access;

  // NOTE: We do NOT reference count this interface or ANY interface
  // obtained from this interface with QueryInterface. The access
  // interface is actually the container for this codec and controls
  // the lifetime of this instance. If we were to acquire another
  // reference to this container then this object and its container will NOT
  // be deleted from memory.

  return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CountFlavours(aafUInt32 *pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pCount = kSupportedCodeFlavours;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedFlavourID (aafUInt32  index,
        aafUID_t *  pFlavour)
{
	if(pFlavour == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;

	*pFlavour = kAAFNilCodecFlavour;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	if (NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetIndexedDataDefinition (aafUInt32  /*index*/,
        aafUID_t * pFlavour)
{
	if (NULL == pFlavour)
		return AAFRESULT_NULL_PARAM;

  return HRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetMaxCodecDisplayNameLength (
        aafUInt32  *bufSize)
{
	if (NULL == bufSize)
		return AAFRESULT_NULL_PARAM;
	
	*bufSize = sizeof(kDisplayName)/sizeof(wchar_t);

	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::GetCodecDisplayName (aafUID_constref /*flavour*/,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
	if (NULL == pName)
		return AAFRESULT_NULL_PARAM;
	if (0 >= bufSize)
		return AAFRESULT_INVALID_PARAM;

	aafUInt32	len = sizeof(kDisplayName);
	if(len > bufSize)
		len = bufSize;
	memcpy(pName, kDisplayName, len);
	return AAFRESULT_SUCCESS;
}
	
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CountChannels (IAAFSourceMob *fileMob,
        aafUID_constref essenceKind,
        IAAFEssenceStream *stream,
        aafUInt16 *  pNumChannels)
{
	if (NULL == fileMob || NULL == stream || NULL == pNumChannels)
		return AAFRESULT_NULL_PARAM;

	if (EqualAUID(&DDEF_Picture, &essenceKind))
	{
		*pNumChannels = 1;
	}
	else
		*pNumChannels = 0;
	
	return AAFRESULT_SUCCESS;
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
		aafUID_t codecID = NULL_UID;
		hr = descriptorHelper.GetCompression(&codecID);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_SUCCESS == hr && EqualAUID(&codecID, &kAAFCodecJPEG) && 0 == padBits)
		{
			pSelectInfo->willHandleMDES = kAAFTrue;
		}
		else
		{ // If the compression property was not present or the 
			// give compression is not supported or padBits was not zero
			// the set to false.
			pSelectInfo->willHandleMDES = kAAFFalse;
		}


		// JPEG/JFIF is always in big-endian byte order...
		if (INTEL_ORDER == _nativeByteOrder)
			pSelectInfo->isNative = kAAFFalse;
		else
			pSelectInfo->isNative = kAAFTrue;

		// This is a software only codec.
		pSelectInfo->hwAssisted = kAAFFalse;

		pSelectInfo->relativeLoss = 10;	/* !!! Need to read MDES header here */ // comment from omcJPEG.c

		// Compute the avgBitsPerSec:
		checkResult(descriptorHelper.GetStoredView(&storedHeight, &storedWidth));
		checkResult(descriptorHelper.GetComponentWidth(&compWidth));

		checkResult(_descriptorHelper.GetFrameLayout(&frameLayout));

		switch (frameLayout)
		{
			case kAAFFullFrame:
			case kAAFOneField:
				numFields = 1;
				break;
		
			case kAAFSeparateFields:
			case kAAFMixedFields:
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
    CAAFJPEGCodec::CountSamples (
        aafUID_constref essenceKind,
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
		aafCheckVerbose_t   /*verbose*/,
        aafCheckWarnings_t  /*warning*/,
         aafUInt32   /*bufSize*/,
		wchar_t *  pName,
        aafUInt32  *bytesWritten)
{
	if (NULL == fileMob || NULL == stream ||
		  NULL == pName   || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;

	return HRESULT_NOT_IMPLEMENTED;
}

		
HRESULT STDMETHODCALLTYPE
CAAFJPEGCodec::Create (IAAFSourceMob *unk,
  aafUID_constref flavour,
  aafUID_constref essenceKind,
  aafRational_constref sampleRate,
  IAAFEssenceStream * stream,
  aafCompressEnable_t compEnable)
{
	HRESULT hr = S_OK;
	IAAFSourceMob			*fileMob = NULL;

	
	if (NULL == unk || NULL == stream )
		return AAFRESULT_NULL_PARAM;
  else if (kAAFTrue != EqualAUID(&kAAFNilCodecFlavour, &flavour))
    return AAFRESULT_NULL_PARAM;


	try
	{
		// Initialize the descriptor helper:
		checkResult(_descriptorHelper.Initialize(unk));

    checkExpression(kAAFTrue == EqualAUID(&essenceKind, &DDEF_Picture),
			              AAFRESULT_INVALID_DATADEF);
    
		_sampleRate = sampleRate;	// There is only one type of sample supported.
    
    // We are allowed to write to the given stream.
		_openMode = kAAFMediaOpenAppend;

		// Save the given essence stream.
		SetEssenceStream(stream);

    // whether or not we will be compressing the samples as they are written.
    SetCompressionEnabled(compEnable);
		
		// What are the descriptor properties that need to be initialized at this point???
		// ?????
		
		// Use omfi code as a guide...

		// Set the default aspect ratio: (Can't we always determine this from the image dimensions? /* TRR */)
		_imageAspectRatio = DEFAULT_ASPECT_RATIO;

    // Make sure that we have created the sample index.

		// The first sample is always at offset 0...of course.E
		// we have not written a sample yet so the next call to AddSampleIndexEntry will
		// allow use the compute the size of the first, 0th, sample.
		const aafUInt32			kStartPadding = 1024;
		unsigned char *padBuf;
		aafUInt32				bytesWritten, n;
		padBuf = new unsigned char [kStartPadding];
		padBuf[0] = 0xff;
		padBuf[1] = 0xd8;
		for(n = 2; n < kStartPadding-1; n++)
			padBuf[n] = 0xff;
		padBuf[kStartPadding-1] = 0xD9;
		checkResult(_stream->Seek(0));
		checkResult(_stream->Write(kStartPadding, padBuf, &bytesWritten));
		delete padBuf;
		checkResult(AddSampleIndexEntry(kStartPadding));
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
		aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t compEnable)
{
	HRESULT hr = S_OK;
	IAAFContainerDef	*containerDef;
	IAAFDefObject		*defObj;
	
	if (NULL == unk || NULL == stream)
		return AAFRESULT_NULL_PARAM;

	try
	{
		// Initialize the descriptor helper:
		checkResult(_descriptorHelper.Initialize(unk));

    // Save the mode to the given stream.
		_openMode = openMode;

		// Save the given essence stream.
		SetEssenceStream(stream);

    // whether or not we will be decompressing the samples as they are read.
    SetCompressionEnabled(compEnable);
		
		// What are the descriptor properties that need to be initialized at this point???
		// ????? use omcJPEG.c as a guide:


		//
		// FileDescriptor methods:
		//
		checkResult(_descriptorHelper.GetLength(&_length));
		checkAssertion(_length < 0xFFFFFFFF);
		_numberOfSamples = (aafUInt32)_length; // The length in the file descriptor (mdes in omfi) seems to be in samples.
		checkResult(_descriptorHelper.GetSampleRate(&_sampleRate));
		checkResult(_descriptorHelper.GetContainerFormat(&containerDef));
		checkResult(containerDef->QueryInterface(IID_IAAFDefObject, (void **)&defObj));
		checkResult(defObj->GetAUID(&_containerFormat));
		containerDef->Release();
		defObj->Release();

		//
		// DigitalImageDescriptor methods:
		//

		// Compression is optional property. If it's not present
		// set it to default value. 
		hr = _descriptorHelper.GetCompression( &_compression );
		checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
				 AAFRESULT_SUCCESS == hr, hr );
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		    _compression = NULL_UID;

//		checkAssertion(kAAFTrue == EqualAUID(&_codecID, &CodecJPEG));
		checkResult(_descriptorHelper.GetStoredView(&_storedHeight, &_storedWidth));
		_imageHeight = _storedHeight;
		_imageWidth = _storedWidth;
				
		hr = _descriptorHelper.GetSampledView(&_sampledHeight, &_sampledWidth, 
			                                    &_sampledXOffset, &_sampledYOffset);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
		{
			_sampledWidth = _imageWidth;
			_sampledHeight = _imageHeight;
			_sampledXOffset = 0;
			_sampledYOffset = 0;
		}

		hr = _descriptorHelper.GetDisplayView(&_displayHeight, &_displayWidth, 
			                                    &_displayXOffset, &_displayYOffset);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
		{
			_displayWidth = _imageWidth;
			_displayHeight = _imageHeight;
			_displayXOffset = 0;
			_displayYOffset = 0;
		}

		checkResult(_descriptorHelper.GetImageAspectRatio(&_imageAspectRatio));
		checkResult(_descriptorHelper.GetFrameLayout(&_frameLayout));


		// Get VideoLineMap
		if (kAAFFullFrame != _frameLayout)
		{
			hr = _descriptorHelper.GetVideoLineMapSize(&_videoLineMapSize);
			checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
			if (AAFRESULT_PROP_NOT_PRESENT == hr || 0 == _videoLineMapSize)
			{
				_videoLineMap[0] = _videoLineMap[1] = 0;
			}
			else
			{
				checkAssertion(2 >= _videoLineMapSize);
				checkResult(_descriptorHelper.GetVideoLineMap(_videoLineMapSize, _videoLineMap));

				if (_frameLayout != kAAFSeparateFields && _frameLayout != kAAFMixedFields && 2 == _videoLineMapSize)
				{
					// There were two elements in the video line mab no corresponding froma layout type.
					// Should this be an error in the descriptor? 
					_videoLineMap[1] = 0;
				}
			}
		} // if (kAAFFullFrame != _frameLayout)


		checkResult(_descriptorHelper.GetComponentWidth(&_componentWidth));
		checkExpression(8 == _componentWidth, AAFRESULT_JPEGBASELINE); // only 8-bit supported


		checkResult(_descriptorHelper.GetHorizontalSubsampling(&_horizontalSubsampling));
//		checkResult(_descriptorHelper.GetVerticalSubsampling(&_verticalSubsampling));
		checkResult(_descriptorHelper.GetPaddingBits(&_paddingBits));


		hr = _descriptorHelper.GetColorSiting(&_colorSiting);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_colorSiting = kAAFCoSiting;

		hr = _descriptorHelper.GetBlackReferenceLevel(&_blackReferenceLevel);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_blackReferenceLevel = 0;

		hr = _descriptorHelper.GetWhiteReferenceLevel(&_whiteReferenceLevel);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_whiteReferenceLevel = (1U << _componentWidth) - 1;

		hr = _descriptorHelper.GetColorRange(&_colorRange);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_PROP_NOT_PRESENT == hr)
			_colorRange = (1U << _componentWidth) - 2;

		hr = _descriptorHelper.GetResolutionID( &_resolutionID );
		checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
				 AAFRESULT_NO_MORE_OBJECTS == hr ||
				 AAFRESULT_SUCCESS == hr, hr );
		if( hr == AAFRESULT_PROP_NOT_PRESENT ||
		    hr == AAFRESULT_NO_MORE_OBJECTS )
		    _resolutionID = 0;

		// Get the compression factor?
		//
		//status = omfsReadInt32(main, media->mdes, pers->omAvJPEDResolutionID, &pdata->jpeg.JPEGTableID);
		//if (status != OM_ERR_NONE)
		//	pdata->jpeg.JPEGTableID = 0;
		
		UpdateCalculatedData();

		// Attempt to create and load the sample index from the end of the essence stream.
		// For now just fail if the index cannot be found.
		aafLength_t samples = 0;

		// Make sure that we have created the sample index.
		if(ReadNumberOfSamples(_stream, samples) == AAFRESULT_SUCCESS)
		{
			checkResult(AllocateSampleIndex(samples));
			SetNumberOfSamples(samples);
			checkResult(ReadSampleIndex());
		}
		else
		{
			aafUInt32	offset/*, bytesRead*/;
			aafUInt16	/*localSamples, */fileByteOrder;
			aafLength_t	streamLength, frameIndexBytes;

			checkResult(_descriptorHelper.GetOffsetFrameIndexes(&offset));
			checkResult(_descriptorHelper.GetFrameIndexByteOrder(&fileByteOrder));
			checkResult(_stream->Seek(offset/*-sizeof(localSamples)*/));
//			checkResult(_stream->Read (sizeof(localSamples), (unsigned char*)&localSamples, &bytesRead));
//			if (fileByteOrder != _nativeByteOrder)
//			{
//				AAFByteSwap32((aafInt32 *)&localSamples);
//			}
//			localSamples--;			// Legacy systems put out len including one extra frame
			checkResult(_stream->GetLength (&streamLength));
			frameIndexBytes = streamLength - offset;
			samples = frameIndexBytes / sizeof(aafUInt32);
			// This number will be too large, because legacy systems pad with zeros until
			// sector boundries, set the number of samples AGAIN when we do the actual import
			// and find a zero frame offset at a position other than zero in the frame table.
			checkResult(AllocateSampleIndex(samples));
			SetNumberOfSamples(samples);
			checkResult(ReadSampleIndex32(fileByteOrder));
		}

		// Reset the position to the start of the data so that the next read will 
		// be from the correct position.
		checkResult(Seek(0));

		// Initialize the positioning indecies.
		_startingIndex = static_cast<aafUInt32>(samples - 1);

		// We can read from the beginning of the stream for both read only and append mode.
		SetCurrentIndex(0);

		// Set the index for the next value write (of open for append).
		SetWriteIndex(_startingIndex + 1);
		
		// Just in case...remap the "not present" error to success...
		if (AAFRESULT_PROP_NOT_PRESENT == hr || 
		    AAFRESULT_NO_MORE_OBJECTS == hr)
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

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *samplesWritten,
        aafUInt32 *bytesWritten)
{
	HRESULT hr = S_OK;
	aafUInt32 n;
  aafUInt32 bytesXfered = 0, samplesXfered = 0; // TBD: Need to be return arguments.

	if (NULL == buffer || NULL == samplesWritten || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;
	// We only support writing a single block at a time.
	else if (0 == nSamples)
		return AAFRESULT_INVALID_PARAM;
	// this codec only handles a single channel
	else if (1 != nSamples)
		return AAFRESULT_ONESAMPLEWRITE;



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



	try
	{
		// Preconditions:
		checkAssertion(NULL != _stream);
		checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);

    checkExpression(_componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE);
		/* this codec only allows one-channel media */

		if (kAAFCompressionEnable == _compressEnable)
		{
			// If we are being asked compress the given buffer then
			// the we should have already calculated the size of a sample.
			checkExpression(_fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);


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


			// Setup the compression parameters.
			aafCompressionParams param;

			// Get the dimensions of the image data to compress.
			param.imageWidth = _imageWidth;
			param.imageHeight = _imageHeight;
			param.components = 3;
//			param.imageHeight = (kAAFSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;

			param.colorSpace = _pixelFormat;
			param.horizontalSubsampling = _horizontalSubsampling;
			param.verticalSubsampling = _verticalSubsampling;
			param.blackReferenceLevel = _blackReferenceLevel;
			param.whiteReferenceLevel = _whiteReferenceLevel;
			param.colorRange = _colorRange;

			// Default quality (until we have support for custom tables.)
			param.quality = _compression_IJG_Quality; 

			// Compute the number of bytes in a single row of pixel data.
			if (1 == _horizontalSubsampling)
			{
				param.rowBytes = (_imageWidth * param.components) + _padBytesPerRow;
			}
			else if (2 == _horizontalSubsampling)
			{	// Add an extra byte if with is odd. NOTE: This will never
				// happen with full 601 frame.
				param.rowBytes = (_imageWidth * (param.components - 1)) + (_imageWidth % 2) + _padBytesPerRow;
			}
				

			// Calculate the size of the sample data to be compressed.
			param.bufferSize = param.rowBytes * param.imageHeight;

			// Make sure the given buffer is really large enough for the complete
			// uncompressed pixel data.
			checkExpression(param.bufferSize <= buflen, AAFRESULT_SMALLBUF);
			
			// Adjust the parameters for separate fields...
			if (kAAFSeparateFields == _frameLayout)
			{
				param.imageHeight /= 2;
				param.bufferSize = param.rowBytes * param.imageHeight;
			}
			


			for (n = 0; n < nSamples; n++)
			{
				/* Step 2: specify data destination (eg, an IAAFEssenceStream) */
				/* Note: steps 2 and 3 can be done in either order. */
				jpeg_essencestream_dest(&cinfo, _stream);

				
				param.buffer = &buffer[bytesXfered];
				checkResult(CompressImage(param, cinfo));
				bytesXfered += param.bufferSize;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					param.buffer = &buffer[bytesXfered];
					checkResult(CompressImage(param, cinfo));
					bytesXfered += param.bufferSize;
				}

				// Add padding for _imageAlignmentFactor
				if (0 < _imageAlignmentFactor)
				{
					aafUInt32 alignmentBytes = bytesXfered % _imageAlignmentFactor;

					// TODO: Allocate and use a "aligmentBuffer" so that all of the padding
					// be written in a single write operation.
					aafUInt32 i;
					aafUInt8 ch = 0;
          aafUInt32 bytesWritten;
					for (i = 0; i < alignmentBytes; ++i)
						checkResult(_stream->Write(1, &ch, &bytesWritten));
				}

				// Update the return values.
				samplesXfered++;

				// Add a new entry to the index and update the sample count...
				AddNewCompressedSample();
			}
		}
		else
		{
			// Data is already compressed so we can just write the data 
			// using the "raw" interface. WriteRawData will update frame index if necessary.
			checkResult(WriteRawData(nSamples, buffer, buflen));
			
			// Update the return values...
			bytesXfered = buflen;
			samplesXfered += nSamples;
		}
		
		*samplesWritten = samplesXfered;
		*bytesWritten = bytesXfered;
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
	if (kAAFCompressionEnable == _compressEnable)
	{
		/* Step 7: release JPEG compression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_compress(&cinfo);
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteBlocks (aafDeinterleave_t  /*inter*/,
        aafUInt16  xferBlockCount,
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
		return AAFRESULT_CODEC_CHANNELS;
	else if (0 == xferBlock[0].numSamples)
		return AAFRESULT_INVALID_PARAM;
	// this codec only handles a single channel
	else if (1 != xferBlock[0].numSamples /*xferBlock[0].subTrackNum*/)
		return AAFRESULT_ONESAMPLEWRITE;



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



	try
	{
		// Preconditions:
		checkAssertion(NULL != _stream);
		checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);

    checkExpression(_componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE);
		/* this codec only allows one-channel media */

		if (kAAFCompressionEnable == _compressEnable)
		{
			// If we are being asked compress the given buffer then
			// the we should have already calculated the size of a sample.
			checkExpression(_fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);


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


			// Setup the compression parameters.
			aafCompressionParams param;

			// Get the dimensions of the image data to compress.
			param.imageWidth = _imageWidth;
			param.imageHeight = _imageHeight;
			param.components = 3;
//			param.imageHeight = (kAAFSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;

			param.colorSpace = _pixelFormat;
			param.horizontalSubsampling = _horizontalSubsampling;
			param.verticalSubsampling = _verticalSubsampling;
			param.blackReferenceLevel = _blackReferenceLevel;
			param.whiteReferenceLevel = _whiteReferenceLevel;
			param.colorRange = _colorRange;

			// Default quality (until we have support for custom tables.)
			param.quality = _compression_IJG_Quality; 

			// Compute the number of bytes in a single row of pixel data.
			if (1 == _horizontalSubsampling)
			{
				param.rowBytes = (_imageWidth * param.components) + _padBytesPerRow;
			}
			else if (2 == _horizontalSubsampling)
			{	// Add an extra byte if with is odd. NOTE: This will never
				// happen with full 601 frame.
				param.rowBytes = (_imageWidth * (param.components - 1)) + (_imageWidth % 2) + _padBytesPerRow;
			}
				

			// Calculate the size of the sample data to be compressed.
			param.bufferSize = param.rowBytes * param.imageHeight;

			// Make sure the given buffer is really large enough for the complete
			// uncompressed pixel data.
			checkExpression(param.bufferSize <= xferBlock[0].buflen, AAFRESULT_SMALLBUF);
			
			// Adjust the parameters for separate fields...
			if (kAAFSeparateFields == _frameLayout)
			{
				param.imageHeight /= 2;
				param.bufferSize = param.rowBytes * param.imageHeight;
			}
			


			for (n = 0; n < xferBlock[0].numSamples; n++)
			{
				/* Step 2: specify data destination (eg, an IAAFEssenceStream) */
				/* Note: steps 2 and 3 can be done in either order. */
				jpeg_essencestream_dest(&cinfo, _stream);

				
				param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
				checkResult(CompressImage(param, cinfo));
				resultBlock[0].bytesXfered += param.bufferSize;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
					checkResult(CompressImage(param, cinfo));
					resultBlock[0].bytesXfered += param.bufferSize;
				}

				// Add padding for _imageAlignmentFactor
				if (0 < _imageAlignmentFactor)
				{
					aafUInt32 alignmentBytes = resultBlock[0].bytesXfered % _imageAlignmentFactor;

					// TODO: Allocate and use a "aligmentBuffer" so that all of the padding
					// for the can be written in a single write operation.
					aafUInt32 i;
					aafUInt8 ch = 0;
					aafUInt32 bytesWritten;
					for (i = 0; i < alignmentBytes; ++i)
						checkResult(_stream->Write(1, &ch, &bytesWritten));
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
	if (kAAFCompressionEnable == _compressEnable)
	{
		/* Step 7: release JPEG compression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_compress(&cinfo);
	}

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ReadSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  samplesRead,
        aafUInt32 *  bytesRead)
{
	HRESULT hr = S_OK;
	aafUInt32 n;
	aafUInt32 sampleSize = 0;


	if (NULL == buffer || NULL == samplesRead || NULL == bytesRead)
		return AAFRESULT_NULL_PARAM;
	// We only support writing a single block at a time.
	// this codec only handles a single channel
	else if (1 != nSamples)
		return AAFRESULT_ONESAMPLEWRITE;
	else if (_currentIndex >= _writeIndex)
		return (AAFRESULT_EOF);

  // Initialize the return parameters.
  *samplesRead = 0;
  *bytesRead = 0;


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


	try
	{
		
		if (kAAFCompressionEnable == _compressEnable)
		{ // If we are being asked decompress the given buffer.

			/* Step 1: allocate and initialize JPEG decompression object */

			/* We set up the normal JPEG error routines, then override error_exit. */
			cinfo.err = jpeg_std_error(&jerr);
			jerr.error_exit = cplusplus_error_exit;

			/* Now we can initialize the JPEG decompression object. */
			jpeg_create_decompress(&cinfo);


			// Setup the decompression parameters.
			aafCompressionParams param;
			memset(&param, 0, sizeof(param));


			// Get the dimensions of the image data to compress.
			param.imageWidth = _imageWidth;
			param.imageHeight = /*JeffB: (kAAFSeparateFields == _frameLayout) ? (_imageHeight / 2) : */_imageHeight;
			param.components = 3;

			param.colorSpace = _pixelFormat;
			param.horizontalSubsampling = _horizontalSubsampling;
			param.verticalSubsampling = _verticalSubsampling;
			param.blackReferenceLevel = _blackReferenceLevel;
			param.whiteReferenceLevel = _whiteReferenceLevel;
			param.colorRange = _colorRange;

			// Default quality (until we have support for custom tables.)
			param.quality = _compression_IJG_Quality; // ignored for input.

			// Compute the number of bytes in a single row of pixel data.
			param.rowBytes = (buflen / nSamples) / _imageHeight;

			// Calculate the size of the image data to be compressed.
			param.bufferSize = param.rowBytes * param.imageHeight;

			for (n = 0; n < nSamples; n++)
			{
				/* Step 2: specify data source (eg, an IAAFEssenceStream) */
				// Setup the data source to read an entire sample frame into memory.
				sampleSize = GetSampleSizeFromIndex(_currentIndex);
				jpeg_essencestream_src(&cinfo, _stream, sampleSize);


				param.buffer = &buffer[*bytesRead];
				checkResult(DecompressImage(param, cinfo));
				*bytesRead += param.bufferSize;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					param.buffer = &buffer[*bytesRead];
					checkResult(DecompressImage(param, cinfo));
					*bytesRead += param.bufferSize;
				}

				(*samplesRead)++;
				// Update the current index.
				SetCurrentIndex(_currentIndex + 1);
				//checkResult(Seek(_currentIndex + 1));
			}
		}
		else
		{
			// Data is already compressed so we can just write the data 
			// using the "raw" interface. WriteRawData will update frame index if necessary.
			checkResult(ReadRawData(nSamples, buflen, buffer, bytesRead, samplesRead));
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


	if (kAAFCompressionEnable == _compressEnable)
	{
		/* Step 8: Release JPEG decompression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_decompress(&cinfo);
	}


	// Cleanup
	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::ReadBlocks (aafDeinterleave_t  /*inter*/,
        aafUInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	HRESULT hr = S_OK;
	aafUInt32 n;
	aafUInt32 sampleSize = 0;


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
		return AAFRESULT_CODEC_CHANNELS;
	// this codec only handles a single channel
	else if (1 != xferBlock[0].numSamples)
		return AAFRESULT_ONESAMPLEWRITE;
	else if (_currentIndex >= _writeIndex)
		return (AAFRESULT_EOF);


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


	try
	{
		
		if (kAAFCompressionEnable == _compressEnable)
		{ // If we are being asked decompress the given buffer.

			/* Step 1: allocate and initialize JPEG decompression object */

			/* We set up the normal JPEG error routines, then override error_exit. */
			cinfo.err = jpeg_std_error(&jerr);
			jerr.error_exit = cplusplus_error_exit;

			/* Now we can initialize the JPEG decompression object. */
			jpeg_create_decompress(&cinfo);


			// Setup the decompression parameters.
			aafCompressionParams param;
			memset(&param, 0, sizeof(param));


			// Get the dimensions of the image data to compress.
			param.imageWidth = _imageWidth;
			param.imageHeight = (kAAFSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;
			param.components = 3;

			param.colorSpace = _pixelFormat;
			param.horizontalSubsampling = _horizontalSubsampling;
			param.verticalSubsampling = _verticalSubsampling;
			param.blackReferenceLevel = _blackReferenceLevel;
			param.whiteReferenceLevel = _whiteReferenceLevel;
			param.colorRange = _colorRange;

			// Default quality (until we have support for custom tables.)
			param.quality = _compression_IJG_Quality; // ignored for input.

			// Compute the number of bytes in a single row of pixel data.
			param.rowBytes = (xferBlock[0].buflen / xferBlock[0].numSamples) / _imageHeight;

			// Calculate the size of the image data to be compressed.
			param.bufferSize = param.rowBytes * param.imageHeight;

			for (n = 0; n < xferBlock[0].numSamples; n++)
			{
				/* Step 2: specify data source (eg, an IAAFEssenceStream) */
				// Setup the data source to read an entire sample frame into memory.
				sampleSize = GetSampleSizeFromIndex(_currentIndex);
				jpeg_essencestream_src(&cinfo, _stream, sampleSize);


				param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
				checkResult(DecompressImage(param, cinfo));
				resultBlock[0].bytesXfered += param.bufferSize;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					param.buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
					checkResult(DecompressImage(param, cinfo));
					resultBlock[0].bytesXfered += param.bufferSize;
				}

				++resultBlock[0].samplesXfered;
				// Update the current index.
				SetCurrentIndex(_currentIndex + 1);
				//checkResult(Seek(_currentIndex + 1));
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


	if (kAAFCompressionEnable == _compressEnable)
	{
		/* Step 8: Release JPEG decompression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_decompress(&cinfo);
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
		checkExpression(sampleFrame <= _numberOfSamples, AAFRESULT_BADFRAMEOFFSET);
//		if (kAAFMediaOpenAppend == _openMode)
//			checkExpression(sampleFrame == _numberOfSamples, AAFRESULT_BADFRAMEOFFSET);
		
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


void CAAFJPEGCodec::UpdateDescriptor (CAAFJPEGDescriptorHelper& descriptorHelper)
{
	checkResult(descriptorHelper.SetLength(_numberOfSamples));
	//	checkResult(descriptorHelper.SetIsInContainer(_isInAAFContainer));
	checkResult(descriptorHelper.SetSampleRate(_sampleRate));
	//	checkResult(descriptorHelper.SetContainerFormat(_containerFormat));
	checkResult(descriptorHelper.SetCompression(_compression));
	checkResult(descriptorHelper.SetStoredView(_storedHeight, _storedWidth));
	checkResult(descriptorHelper.SetSampledView(_sampledHeight, _sampledWidth, _sampledXOffset, _sampledYOffset));
	checkResult(descriptorHelper.SetDisplayView(_displayHeight, _displayWidth, _displayXOffset, _displayYOffset));
	checkResult(descriptorHelper.SetFrameLayout(_frameLayout));
	checkResult(descriptorHelper.SetVideoLineMap(_videoLineMapSize, _videoLineMap));
	checkResult(descriptorHelper.SetImageAspectRatio(_imageAspectRatio));
	checkResult(descriptorHelper.SetAlphaTransparency(_alphaTransparency));
//	checkResult(descriptorHelper.SetGamma(_gamma));
	checkResult(descriptorHelper.SetImageAlignmentFactor(_imageAlignmentFactor));
	checkResult(descriptorHelper.SetComponentWidth(_componentWidth));
	checkResult(descriptorHelper.SetHorizontalSubsampling(_horizontalSubsampling));
	checkResult(descriptorHelper.SetColorSiting(_colorSiting));
	checkResult(descriptorHelper.SetBlackReferenceLevel(_blackReferenceLevel));
	checkResult(descriptorHelper.SetWhiteReferenceLevel(_whiteReferenceLevel));
	checkResult(descriptorHelper.SetColorRange(_colorRange));
	checkResult(descriptorHelper.SetPaddingBits(_paddingBits));

	if( _resolutionID != 0 )
	{
	    checkResult( descriptorHelper.SetResolutionID( _resolutionID ) );
	    checkResult( descriptorHelper.SetFrameSampleSize( 0 ) );
	    checkResult( descriptorHelper.SetFirstFrameOffset( 0 ) );
	}
}


// Routine to keep calculated member data up-to-date.
void CAAFJPEGCodec::UpdateCalculatedData(void)
{
	aafUInt32 numFields = 0;

		// We currently only support 601 4-4-4 and 4-2-2.
//	checkExpression((1 == _verticalSubsampling && 1 == _horizontalSubsampling) ||
//								  (1 == _verticalSubsampling && 2 == _horizontalSubsampling && 
//									 kAAFColorSpaceYUV == _pixelFormat),
//				          AAFRESULT_BADPIXFORM); // AAFRESULT_BADLAYOUT


	switch (_frameLayout)
	{
		case kAAFFullFrame:
		case kAAFOneField:
			numFields = 1;
			break;
	
		case kAAFSeparateFields:
		case kAAFMixedFields:
			numFields = 2;
			break;
	
		default:
			break;
	} /* end switch */

	// The is true for RGB or YUV. (no alpha support)
	_memBitsPerPixel = _componentWidth * 3;


	// Computer the file bytes per sample.
	_fileBytesPerSample = 0;
	_bitsPerPixelAvg = 0;
	_bitsPerSample = 0;

	if (kAAFColorSpaceRGB == _pixelFormat)
	{
		// If the output color space is RGB then decompressed sample size
		// ignores any subsampling in the compressed image since the 
		// decompressor will upsample the pixels for us.
		_bitsPerPixelAvg = (aafInt16)((_componentWidth * 3) + _paddingBits);
		_bitsPerSample = ((_bitsPerPixelAvg * _imageWidth) + _padBytesPerRow) * _imageHeight;
	}							
	else
	{
		/* 4:4:4 = 1 sample for each of luma and two chroma channels */   
		if (_horizontalSubsampling == 1)
		{
			_bitsPerPixelAvg = (aafInt16)((_componentWidth * 3) + _paddingBits);
			_bitsPerSample = (aafInt32) _imageWidth * (aafInt32) _imageHeight *
	  								(_componentWidth * 3) * numFields; // trr: is this reall correct? Why do we care about numFields since we are using the full _imageHeight?
		}							
		/* 4:2:2 = two-pixels get 2 samples of luma, 1 of each chroma channel, avg == 2 */							
		else if (_horizontalSubsampling == 2)						
		{
			_bitsPerPixelAvg = (aafInt16)((_componentWidth * 2) + _paddingBits);
			_bitsPerSample = (aafInt32) _imageWidth * (aafInt32) _imageHeight *
	  								(_componentWidth * 2) * numFields;
		}
	}

	_fileBytesPerSample = (_bitsPerSample + 7) / 8;
	_memBytesPerSample = _fileBytesPerSample;
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::CompleteWrite (IAAFSourceMob *fileMob)
{
	HRESULT hr = S_OK;

	try
	{
		// Flush the jpeg data source (if necessary)

		// Write the sample index onto the end of the stream.
//		checkResult(WriteSampleIndex());
		checkResult(WriteSampleIndex32());		// Legacy system compatability

		// Make sure the descriptor information is up-to-date.
		UpdateDescriptor(_descriptorHelper); // throw HRESULT

		if (NULL != fileMob)
		{	// Initialize the descriptor helper:
			CAAFJPEGDescriptorHelper descriptorHelper;
			checkResult(descriptorHelper.Initialize(fileMob));

			// Make sure the descriptor information is up-to-date.
			UpdateDescriptor(descriptorHelper); // throw HRESULT
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




HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::WriteRawData (
			aafUInt32 nSamples, 
			aafDataBuffer_t  buffer,
      aafUInt32  buflen)
{
	HRESULT hr = S_OK;
  aafUInt32 bytesWritten;

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
		checkExpression(kAAFMediaOpenAppend == _openMode, AAFRESULT_NOT_WRITEABLE);

		// Make sure that we do not attempt to write before the initial
		// starting index.
		checkExpression(_currentIndex >= _startingIndex, AAFRESULT_NOT_WRITEABLE);
		checkAssertion(_currentIndex <= _writeIndex);


		// We should only be able to write to the end of the stream. This should be 
		// enforced by the implementation of the IAAFEssenceStream. This is why we 
		// compute the offset to the start from the position after the write 
		// operation just in case the seek position had to be reset to the end by the
		// implementation of the IAAFEssenceStream.

		// Write the compressed sample data.
		checkResult(_stream->Write(buflen, buffer, &bytesWritten));

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
	if (0 == nSamples || NULL == buffer || 0 == buflen || 
		  NULL == bytesRead || NULL == samplesRead)
		return AAFRESULT_NULL_PARAM;
	else if (1 != nSamples)
		return AAFRESULT_ONESAMPLEWRITE;
	else if (_currentIndex >= _writeIndex)
		return (AAFRESULT_EOF);


	*bytesRead = 0;
	*samplesRead = 0;

	try
	{
		checkAssertion(NULL != _stream);
		checkExpression(NULL != _sampleIndex, AAFRESULT_NOFRAMEINDEX);

		// Make sure we do not attempt to read beyond the end of the sample stream.
		checkExpression(_currentIndex < _writeIndex, AAFRESULT_EOF);
		
		// Make sure the given buffer is large enougth to hold the current sample
		aafUInt32 sampleSize = static_cast<aafUInt32>(_sampleIndex[_currentIndex + 1] - _sampleIndex[_currentIndex]);
		checkExpression(buflen >= sampleSize, AAFRESULT_SMALLBUF);

		aafPosition_t offset;
		checkResult(_stream->GetPosition(&offset));
		checkResult(_stream->Read(sampleSize, buffer, bytesRead));

		*samplesRead = 1; // we currently only support one sample reads...

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
	aafInt32 size;
	union
	{
		aafUInt8 expData[kMaxEssenceFormatData];
		aafUInt32 expUInt32;
		aafInt32 expInt32;
		aafUInt16 expUInt16;
		aafInt16 expInt16;
		aafUInt8 expUInt8;
		aafInt8 expInt8;
		aafRational_t expRational;
		aafRect_t expRect;
		aafColorSpace_t expColorSpace;
		aafCompArray_t expCompArray;
		aafCompSizeArray_t expCompSizeArray;
		aafFrameLayout_t expFrameLayout;
		aafVideoLineMap_t expVideoLineMap;
		aafColorSiting_t expColorSiting;
		aafAlphaTransparency_t expAlphaTransparency;
		aafFieldNumber_t	   expFieldDominance;
		aafUID_t expUID;
	} operand;
} aafEssenceFormatData_t;


static void GetFormatParam(
  const aafEssenceFormatData_t& param, 
  aafUInt32& num) // throw HRESULT
{
	if (param.size == 1)
		num = param.operand.expUInt8;
	else if (param.size == 2)
		num = param.operand.expUInt16;
	else if (param.size == 4)
		num = param.operand.expUInt32;
	else
		throw HRESULT(AAFRESULT_INVALID_PARM_SIZE);
}


static void GetFormatParam(
  const aafEssenceFormatData_t& param, 
  aafInt32& num) // throw HRESULT
{
	if (param.size == 1)
		num = param.operand.expInt8;
	else if (param.size == 2)
		num = param.operand.expInt16;
	else if (param.size == 4)
		num = param.operand.expInt32;
	else
		throw HRESULT(AAFRESULT_INVALID_PARM_SIZE);
}


		
HRESULT STDMETHODCALLTYPE
    CAAFJPEGCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	HRESULT hr = S_OK;
	aafInt32		numSpecifiers, i;
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
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &param.size));

			if (EqualAUID(&kAAFFrameLayout, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expFrameLayout), AAFRESULT_INVALID_PARM_SIZE);
				// Validate the frame layout:
				switch (param.operand.expFrameLayout)
				{
					case kAAFFullFrame:
					case kAAFOneField:
						numFields = 1;
						break;
				
					case kAAFSeparateFields:
					case kAAFMixedFields:
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
				if( _numberOfSamples > 0 )
				    checkAssertion( frameLayout == _frameLayout );

				_frameLayout = frameLayout;
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
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &param.size));

			if (EqualAUID(&kAAFCompression, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expUID), AAFRESULT_INVALID_PARM_SIZE);

				memcpy( &_compression, &(param.operand.expUID), 
				    sizeof(param.operand.expUID) );
			}
			else if (EqualAUID(&kAAFNumChannels, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expInt32), AAFRESULT_INVALID_PARM_SIZE);
				// We only support a single channel.
				checkExpression(1 == param.operand.expInt32, AAFRESULT_CODEC_CHANNELS);
			}
			else if (EqualAUID(&kAAFVideoLineMap, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expVideoLineMap), AAFRESULT_INVALID_PARM_SIZE);
				_videoLineMap[0] = param.operand.expVideoLineMap[0];
				_videoLineMap[1] = param.operand.expVideoLineMap[1];
			}
//			else if (EqualAUID(&kAAFPixelSize, &param.opcode))
//			{	// Validate the in-memory size.
//				checkExpression(param.size == sizeof(param.operand.expInt16), AAFRESULT_INVALID_PARM_SIZE);
//				// This is a calculated value!
//				//_bitsPerPixelAvg = param.operand.expInt16;
//			}
			else if (EqualAUID(&kAAFStoredRect, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
				
				_storedWidth = param.operand.expRect.xSize;
				_storedHeight = param.operand.expRect.ySize;
				_imageWidth = _storedWidth;
				_imageHeight = _storedHeight;
			}
			else if (EqualAUID(&kAAFSampledRect, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
				
				_sampledXOffset = param.operand.expRect.xOffset;
				_sampledYOffset = param.operand.expRect.yOffset;
				_sampledHeight = param.operand.expRect.ySize;
				_sampledWidth = param.operand.expRect.xSize;
				_sampledHeight = param.operand.expRect.ySize;
			}
			else if (EqualAUID(&kAAFDisplayRect, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expRect), AAFRESULT_INVALID_PARM_SIZE);
				
				_displayXOffset = param.operand.expRect.xOffset;
				_displayYOffset = param.operand.expRect.yOffset;
				_displayHeight = param.operand.expRect.ySize;
				_displayWidth = param.operand.expRect.xSize;
				_displayHeight = param.operand.expRect.ySize;
			}
			else if (EqualAUID(&kAAFAspectRatio, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expRational), AAFRESULT_INVALID_PARM_SIZE);
				
				_imageAspectRatio = param.operand.expRational;
			}
			else if (EqualAUID(&kAAFPixelFormat, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expColorSpace), AAFRESULT_INVALID_PARM_SIZE);

				// Currently we only support the following pixel transformations.
				checkExpression(kAAFColorSpaceRGB == param.operand.expColorSpace ||
					              kAAFColorSpaceYUV == param.operand.expColorSpace,
					              AAFRESULT_BADPIXFORM);

				_pixelFormat = param.operand.expColorSpace;
			}
			else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expInt32), AAFRESULT_INVALID_PARM_SIZE);
				
				// We currently only support 8 bit components...
				checkExpression(kDefaultComponentWidth == param.operand.expInt32, AAFRESULT_JPEGBASELINE); // only 8-bit supported
				_componentWidth = param.operand.expInt32;
			}
			else if (EqualAUID(&kAAFCDCIColorSiting, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expColorSiting), AAFRESULT_INVALID_PARM_SIZE);
				
				// We currently only support 8 bit components...
				_colorSiting = param.operand.expColorSiting;
			}
			else if (EqualAUID(&kAAFCDCIBlackLevel, &param.opcode))
			{	// Validate the in-memory size.
				::GetFormatParam(param, _blackReferenceLevel);
			}
			else if (EqualAUID(&kAAFCDCIWhiteLevel, &param.opcode))
			{	// Validate the in-memory size.
				::GetFormatParam(param, _whiteReferenceLevel);
			}
			else if (EqualAUID(&kAAFCDCIColorRange, &param.opcode))
			{	// Validate the in-memory size.
				::GetFormatParam(param, _colorRange);
			}
			else if (EqualAUID(&kAAFPadBytesPerRow, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expUInt16), AAFRESULT_INVALID_PARM_SIZE);
				_padBytesPerRow = param.operand.expUInt16;
			}
			else if (EqualAUID(&kAAFCompressionQuality, &param.opcode))
			{	// Validate the in-memory size.
				aafUInt32 compressionQuality = 0;
				::GetFormatParam(param, compressionQuality);

				if (25 > compressionQuality || 100 < compressionQuality)
					throw HRESULT(AAFRESULT_JPEGBASELINE);

				_compression_IJG_Quality = compressionQuality;
			}
			else if (EqualAUID(&kAAFResolutionID, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expInt32), AAFRESULT_INVALID_PARM_SIZE);

				aafInt32 resID = param.operand.expInt32;
				switch( resID )
				{
				    case 75:
				    case 76:
				    case 77:
				    case 78:
				    case 79:
				    case 80:
				    case 82:
				    case 95:
				    case 96:
				    case 97:
				    case 98:
				    case 102:
				    case 103:
				    case 104:
				    case 110:
				    case 111:
				    case 112:
				    case 113:
					_resolutionID = resID;
					memcpy( &_compression, 
						&AAF_CMPR_FULL_JPEG, 
						sizeof(_compression) );
					break;
				    default:
					throw HRESULT(AAFRESULT_INVALID_PARAM);

				}
			}

			
			
		} // for (i = 0...)

		
		if (horizontalSubsampling != _horizontalSubsampling)
			_horizontalSubsampling = horizontalSubsampling;


		UpdateCalculatedData();

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
	aafInt32 numSpecifiers, i;
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
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &param.size));

			if( EqualAUID( &kAAFCompression, &param.opcode) )
			{
			    // Write out current compression ID
			    memcpy( &(param.operand.expUID), &_compression, 
				sizeof(param.operand.expUID) );
				checkResult(fmt->AddFormatSpecifier (kAAFCompression, sizeof(param.operand.expUID), (aafDataBuffer_t)&param.operand.expUID));
			}
			else if (EqualAUID(&kAAFNumChannels, &param.opcode))
			{	// We only support a single channel...		
				param.operand.expInt32 = kDefaultNumCh;
				checkResult(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));
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
				param.operand.expRect.ySize = _imageHeight;
				checkResult(fmt->AddFormatSpecifier (kAAFStoredRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
			}
			else if (EqualAUID(&kAAFSampledRect, &param.opcode))
			{	// Write out the current sampled rectangle.
				param.operand.expRect.xOffset = _sampledXOffset;
				param.operand.expRect.yOffset = _sampledYOffset;
				param.operand.expRect.xSize = _sampledWidth;
				param.operand.expRect.ySize = _sampledHeight;
				checkResult(fmt->AddFormatSpecifier (kAAFSampledRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
			}
			else if (EqualAUID(&kAAFDisplayRect, &param.opcode))
			{	// Write out the current sampled rectangle.
				param.operand.expRect.xOffset = _displayXOffset;
				param.operand.expRect.yOffset = _displayYOffset;
				param.operand.expRect.xSize = _displayWidth;
				param.operand.expRect.ySize = _displayHeight;
				checkResult(fmt->AddFormatSpecifier (kAAFDisplayRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
			}
			else if (EqualAUID(&kAAFAspectRatio, &param.opcode))
			{	// Write out the current aspect ratio.				
				param.operand.expRational = _imageAspectRatio;
				checkResult(fmt->AddFormatSpecifier (kAAFAspectRatio, sizeof(param.operand.expRational), (aafDataBuffer_t)&param.operand.expRational));
			}
			else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
			{	// We currently only support 8 bit components...
				checkExpression(kDefaultComponentWidth == _componentWidth, AAFRESULT_JPEGBASELINE); // only 8-bit supported
				param.operand.expInt32 = (aafInt32)_componentWidth;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCICompWidth, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));
			}
			else if (EqualAUID(&kAAFSampleRate, &param.opcode))
			{	// Write out the current color siting value.
				param.operand.expRational = _sampleRate;
				checkResult(fmt->AddFormatSpecifier (kAAFSampleRate, sizeof(param.operand.expRational), (aafDataBuffer_t)&param.operand.expRational));
			}
			else if (EqualAUID(&kAAFMaxSampleBytes, &param.opcode))
			{	// Write out the current color siting value.
				aafLength_t	maxLen;

				GetLargestSampleSize (DDEF_Picture, &maxLen);
				param.operand.expInt32 = (aafUInt32)maxLen;
				checkResult(fmt->AddFormatSpecifier (kAAFMaxSampleBytes, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));
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
			else if (EqualAUID(&kAAFImageAlignmentFactor, &param.opcode))
			{	// Write out the current color range.
				param.operand.expUInt32 = _imageAlignmentFactor;
				checkResult(fmt->AddFormatSpecifier (kAAFImageAlignmentFactor, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}
			else if (EqualAUID(&kAAFPixelFormat, &param.opcode))
			{	// Write out the current pixel format.
				param.operand.expColorSpace = _pixelFormat;
				checkResult(fmt->AddFormatSpecifier (kAAFPixelFormat, sizeof(param.operand.expColorSpace), (aafDataBuffer_t)&param.operand.expColorSpace));
			}
			else if (EqualAUID(&kAAFFieldDominance, &param.opcode))
			{	// Write out the current pixel format.
				param.operand.expFieldDominance = _pixelFormat;
				checkResult(fmt->AddFormatSpecifier (kAAFFieldDominance, sizeof(param.operand.expFieldDominance), (aafDataBuffer_t)&param.operand.expFieldDominance));
			}
			else if (EqualAUID(&kAAFRGBCompLayout, &param.opcode) && kAAFColorSpaceRGB ==_pixelFormat)
			{	// Default to standard component order.
				param.operand.expCompArray[0] = 'R';
				param.operand.expCompArray[1] = 'G';
				param.operand.expCompArray[2] = 'B';
				param.operand.expCompArray[3] = 0;
				checkResult(fmt->AddFormatSpecifier (kAAFRGBCompLayout, sizeof(param.operand.expCompArray), (aafDataBuffer_t)&param.operand.expCompArray));
			}
			else if (EqualAUID(&kAAFRGBCompSizes, &param.opcode) && kAAFColorSpaceRGB ==_pixelFormat)
			{	// Default to standard component order.
				param.operand.expCompSizeArray[0] = 8;
				param.operand.expCompSizeArray[1] = 8;
				param.operand.expCompSizeArray[2] = 8;
				param.operand.expCompSizeArray[3] = 0;
				checkResult(fmt->AddFormatSpecifier (kAAFRGBCompSizes, sizeof(param.operand.expCompSizeArray), (aafDataBuffer_t)&param.operand.expCompSizeArray));
			}
			else if (EqualAUID(&kAAFFrameLayout, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expFrameLayout = _frameLayout;
				checkResult(fmt->AddFormatSpecifier (kAAFFrameLayout, sizeof(param.operand.expFrameLayout), (aafDataBuffer_t)&param.operand.expFrameLayout));
			}
			else if (EqualAUID(&kAAFPadBytesPerRow, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expUInt16 = _padBytesPerRow;
				checkResult(fmt->AddFormatSpecifier (kAAFPadBytesPerRow, sizeof(param.operand.expUInt16), (aafDataBuffer_t)&param.operand.expUInt16));
			}
			else if (EqualAUID(&kAAFCDCIHorizSubsampling, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expUInt32 = _horizontalSubsampling;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCIHorizSubsampling, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}
			else if (EqualAUID(&kAAFFieldStartOffset, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expUInt32 = 0;
				checkResult(fmt->AddFormatSpecifier (kAAFFieldStartOffset, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}
			else if (EqualAUID(&kAAFFieldEndOffset, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expUInt32 = 0;
				checkResult(fmt->AddFormatSpecifier (kAAFFieldEndOffset, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));
			}

			else if (EqualAUID(&kAAFCDCIPadBits, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expUInt16 = _paddingBits;
				checkResult(fmt->AddFormatSpecifier (kAAFCDCIPadBits, sizeof(param.operand.expUInt16), (aafDataBuffer_t)&param.operand.expUInt16));
			}
			else if (EqualAUID(&kAAFAlphaTransparency, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expAlphaTransparency = _alphaTransparency;
				checkResult(fmt->AddFormatSpecifier (kAAFAlphaTransparency, sizeof(param.operand.expAlphaTransparency), (aafDataBuffer_t)&param.operand.expAlphaTransparency));
			}
			else if (EqualAUID(&kAAFGamma, &param.opcode))
			{	// Write out the current pad bytes per row.
				param.operand.expRational = _gamma;
				checkResult(fmt->AddFormatSpecifier (kAAFGamma, sizeof(param.operand.expRational), (aafDataBuffer_t)&param.operand.expRational));
			}
			else if (EqualAUID(&kAAFResolutionID, &param.opcode))
			{
				param.operand.expInt32 = _resolutionID;
				checkResult(fmt->AddFormatSpecifier (kAAFResolutionID, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));
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
		
		// Write out current compression ID
		memcpy( &(param.operand.expUID), &_compression, 
		    sizeof(param.operand.expUID) );
		checkResult(fmt->AddFormatSpecifier (kAAFCompression, sizeof(param.operand.expUID), (aafDataBuffer_t)&param.operand.expUID));

		// This codec only handles a single channel of Picture data.
		param.operand.expInt32 = kDefaultNumCh;
		checkResult(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));
		
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

		// Write out the current image dimentions for the default sampled rectangle.
		param.operand.expRect.xOffset = param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _imageWidth;
		param.operand.expRect.ySize = _imageWidth;
		checkResult(fmt->AddFormatSpecifier (kAAFSampledRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
		
		// Write out the current image dimentions for the default display rectangle.
		param.operand.expRect.xOffset = param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _imageWidth;
		param.operand.expRect.ySize = _imageWidth;
		checkResult(fmt->AddFormatSpecifier (kAAFDisplayRect, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRect));
		
		// Write out the current component width.
		param.operand.expInt32 = (aafInt32)_componentWidth;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCICompWidth, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));

		param.operand.expRational = _sampleRate;
		checkResult(fmt->AddFormatSpecifier (kAAFSampleRate, sizeof(param.operand.expRational), (aafDataBuffer_t)&param.operand.expRational));

		param.operand.expInt32 = 0;
		checkResult(fmt->AddFormatSpecifier (kAAFMaxSampleBytes, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));

		param.operand.expColorSiting = _colorSiting;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCIColorSiting, sizeof(param.operand.expColorSiting), (aafDataBuffer_t)&param.operand.expColorSiting));

		param.operand.expUInt32 = _blackReferenceLevel;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCIBlackLevel, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));

		param.operand.expUInt32 = _whiteReferenceLevel;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCIWhiteLevel, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));

		param.operand.expUInt32 = _colorRange;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCIColorRange, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));

		param.operand.expUInt32 = _horizontalSubsampling;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCIHorizSubsampling, sizeof(param.operand.expUInt16), (aafDataBuffer_t)&param.operand.expUInt16));

		param.operand.expRational = _gamma;
		checkResult(fmt->AddFormatSpecifier (kAAFGamma, sizeof(param.operand.expRational), (aafDataBuffer_t)&param.operand.expRational));

		param.operand.expUInt16 = _paddingBits;
		checkResult(fmt->AddFormatSpecifier (kAAFCDCIPadBits, sizeof(param.operand.expUInt16), (aafDataBuffer_t)&param.operand.expUInt16));

		param.operand.expRational = _imageAspectRatio;
		checkResult(fmt->AddFormatSpecifier (kAAFAspectRatio, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRational));

		param.operand.expAlphaTransparency = _alphaTransparency;
		checkResult(fmt->AddFormatSpecifier (kAAFAlphaTransparency, sizeof(param.operand.expAlphaTransparency), (aafDataBuffer_t)&param.operand.expAlphaTransparency));

		param.operand.expUInt32 = 0;
		checkResult(fmt->AddFormatSpecifier (kAAFFieldStartOffset, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));

		param.operand.expUInt32 = 0;
		checkResult(fmt->AddFormatSpecifier (kAAFFieldEndOffset, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));

		param.operand.expFieldDominance = _pixelFormat;
		checkResult(fmt->AddFormatSpecifier (kAAFFieldDominance, sizeof(param.operand.expFieldDominance), (aafDataBuffer_t)&param.operand.expFieldDominance));

		param.operand.expUInt32 = _imageAlignmentFactor;
		checkResult(fmt->AddFormatSpecifier (kAAFImageAlignmentFactor, sizeof(param.operand.expUInt32), (aafDataBuffer_t)&param.operand.expUInt32));

		param.operand.expInt32 = _resolutionID;
		checkResult(fmt->AddFormatSpecifier (kAAFResolutionID, sizeof(param.operand.expInt32), (aafDataBuffer_t)&param.operand.expInt32));

		// Set the sample format and color space and component order and size.
		// Initially use the same setup as omcJPEG.c...
		//
		if ((kAAFCompressionEnable == _compressEnable) && (kAAFColorSpaceRGB == _pixelFormat))
		{
			param.operand.expColorSpace = kAAFColorSpaceRGB;
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
			param.operand.expColorSpace = kAAFColorSpaceYUV;
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
    CAAFJPEGCodec::GetIndexedSampleSize (aafUID_constref dataDefID,
										 aafPosition_t pos,
										 aafLength_t *pResult)
{
	HRESULT hr = S_OK;


	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;
	if(pos < 0 || pos > _numberOfSamples) // zero based sample index.
		return AAFRESULT_EOF;

	// Initialize the return value.
	*pResult = 0;

	try
	{
		if(EqualAUID(&dataDefID, &DDEF_Picture))
		{
			if (kAAFCompressionDisable == _compressEnable)
			{ // The samples are compressed to we can so get the size of the sample
				// from the corresponding element in the sampleIndex.
				*pResult = GetSampleSizeFromIndex(pos);
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
    CAAFJPEGCodec::GetLargestSampleSize (aafUID_constref dataDefID,
										 aafLength_t *pResult)
{
	HRESULT hr = S_OK;

	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = 0;

	try
	{
		if(EqualAUID(&dataDefID, &DDEF_Picture))
		{ 
			if (kAAFCompressionDisable == _compressEnable)
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

HRESULT CAAFJPEGCodec::AddSampleIndexEntry (aafPosition_t pos)
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


JSAMPIMAGE CAAFJPEGCodec::GetRawSampleImage(const aafCompressionParams& param) // throw HRESULT
{
	// We currently only support 601 and 4-2-2.
	checkExpression(1 == param.verticalSubsampling && 2 == param.horizontalSubsampling &&
									kAAFColorSpaceYUV == param.colorSpace,
				          AAFRESULT_BADPIXFORM); // AAFRESULT_BADLAYOUT

	if (NULL == _rawSampleImage)
	{
		aafUInt32 comp_size[3], mcu_sample_width[3], mcu_sample_height[3], sub_width;
		aafUInt32 comp_offset[3];
		aafUInt32 ci, row, total_rows, offset;
		JSAMPARRAY sampleArray;

		// Allocate the top-level color component array to hold the 2-D array for each
		// component.
		_rawSampleImage = new JSAMPARRAY[3];
		checkExpression(NULL != _rawSampleImage, AAFRESULT_NOMEMORY); // throw HRESULT!
		for (ci = 0; ci < 3; ++ci)
			_rawSampleImage[ci] = NULL;

		// Compute the size of the Luminance component data array.
		comp_offset[0] = 0;
		mcu_sample_width[0]  = param.imageWidth + (param.imageWidth % DCTSIZE);
		mcu_sample_height[0] = DCTSIZE * 2;
		comp_size[0] = (mcu_sample_width[0] * mcu_sample_height[0]) * sizeof(JSAMPLE);

		// Compute the size of the Cb component data array.
		comp_offset[1] = comp_size[0];
		sub_width = param.imageWidth / 2;
		mcu_sample_width[1]  = sub_width + (sub_width % DCTSIZE);
		mcu_sample_height[1] = mcu_sample_height[0];
		comp_size[1] = (mcu_sample_width[1] * mcu_sample_height[1]) * sizeof(JSAMPLE);

		// Compute the size of the Cr component data array.
		comp_offset[2] = comp_size[0] + comp_size[1];
		mcu_sample_width[2]  = mcu_sample_width[1];
		mcu_sample_height[2] = mcu_sample_height[1];
		comp_size[2] = comp_size[1];

		
		// Allocate the buffer for all of the raw sample row pointers.
		total_rows = mcu_sample_height[0] + mcu_sample_height[1] + mcu_sample_height[2];
		_rawSampleArray = new JSAMPROW[total_rows];
		checkExpression(NULL != _rawSampleArray, AAFRESULT_NOMEMORY); // throw HRESULT!

		// Clear out the array of pointers. We may only do this in the debug version...
		for (row = 0; row < total_rows; ++row)
			_rawSampleArray[row] = NULL;

		// Assign the sample array pointer for each component from the raw sample array.
		_rawSampleImage[0] = &_rawSampleArray[0];
		_rawSampleImage[1] = &_rawSampleArray[mcu_sample_height[0]];
		_rawSampleImage[2] = &_rawSampleArray[mcu_sample_height[0] + mcu_sample_height[1]];
		

		// Compute the total number of JSAMPLE's needed to hold the raw data.
		_rawSampleImageBufferSize = comp_size[0] + comp_size[1] + comp_size[2];
		
		// Attempt to allocate the buffer.
		_rawSampleImageBuffer = new JSAMPLE[_rawSampleImageBufferSize];
		checkExpression(NULL != _rawSampleImageBuffer, AAFRESULT_NOMEMORY); // throw HRESULT!


		// Now we need to initialize the elements of the JSAMPIMAGE with pointers
		// into the allocated raw sample image buffer.
		for (ci = 0; ci < 3; ++ci)
		{
			// Assign the array of row pointers. These will be assigned from offsets
			// into the _rawSampleImageBuffer so they will NOT have to be deleted.

			sampleArray = _rawSampleImage[ci];		
			offset = comp_offset[ci];
			
			for (row = 0; row < mcu_sample_height[ci]; ++row)
			{
				sampleArray[row] = _rawSampleImageBuffer + offset;

				// compute offset to the next row.
				offset += mcu_sample_width[ci];
			}
		}
	}

	// Return the current buffer.
	return _rawSampleImage;
}

// Mapping from 601-1 4-2-2):
const int Y_POS = 1;
const int Cb_POS = 0;
const int Cr_POS = 2;

const int Y_OFFSET = 2;
const int Cb_OFFSET = 4;
const int Cr_OFFSET = 4;

static void SetupFor422(aafUInt32 imageWidth, aafUInt32 comp_pos[], aafUInt32 comp_offset[], aafUInt32 comp_width[], aafUInt32 comp_height[])
{
	comp_pos[0] = Y_POS;
	comp_pos[1] = Cb_POS;
	comp_pos[2] = Cr_POS;

	comp_offset[0] = Y_OFFSET;
	comp_offset[1] = Cb_OFFSET;
	comp_offset[2] = Cr_OFFSET;


	comp_width[0] = imageWidth;
	comp_width[2] = comp_width[1] = (imageWidth / 2);

	comp_height[0] = DCTSIZE * 2;
	comp_height[2] = comp_height[1] = comp_height[0];
}





void CAAFJPEGCodec::DumpSampleImage(
	const aafCompressionParams& param, 
	JSAMPIMAGE rawSampleImage)
{
	// We currently only support 601 and 4-2-2.
	checkExpression(1 == param.verticalSubsampling && 2 == param.horizontalSubsampling &&
									kAAFColorSpaceYUV == param.colorSpace,
				          AAFRESULT_BADPIXFORM); // AAFRESULT_BADLAYOUT
#ifdef _AAF_DUMP
	
	printf("static const aafUInt8 %s[][][] =\n{\n", "SampleImage");


	aafUInt32 ci, row, col, comp_pos[3], comp_offset[3], comp_width[3], comp_height[3];
	int x;

	SetupFor422(param.imageWidth, comp_pos, comp_offset, comp_width, comp_height);



	for (ci = 0; ci < 3; ++ci)
	{
		printf("  { // Color component table %d\n    ", ci);

		for (row = 0; row < comp_height[ci]; ++row)
		{
			for (col = 0; col < comp_width[ci]; ++col)
			{
				x = (int)rawSampleImage[ci][row][col];
				printf("0x%02X", x);
				if (!((col == (comp_width[ci] - 1)) && (row == (comp_height[ci] - 1))))
					printf(",");
			}

			printf("\n    ");
		}

		if (ci < 2)
			printf("\n  },\n");
		else
			printf("\n  }\n");

		printf("\n");
	}


	printf("\n};\n");
#endif // #ifdef _AAF_DUMP
}



aafUInt32 CAAFJPEGCodec::CopyDataToSampleImage(
	const aafCompressionParams& param, 
	aafUInt32 startingOffset,
	JSAMPIMAGE rawSampleImage) // throw HRESULT
{
	// We currently only support 601 and 4-2-2.
	checkExpression(1 == param.verticalSubsampling && 2 == param.horizontalSubsampling &&
									kAAFColorSpaceYUV == param.colorSpace,
				          AAFRESULT_BADPIXFORM); // AAFRESULT_BADLAYOUT
	
	// Move the CbYCrY' elements from the interleaved compression buffer
 	// out into the separate color component images within the given raw sample
	// image. This sample image is suitable for input to the jpeg_write_raw_data
	// function of IJG 6(b).

	// The following code was modelled on the IJG null_convert in jccolor.c:
	aafUInt32 ci, row, offset, col, comp_pos[3], comp_offset[3], comp_width[3], comp_height[3];
	JSAMPARRAY output_array = NULL;
	JSAMPROW output_row = NULL;

	SetupFor422(param.imageWidth, comp_pos, comp_offset, comp_width, comp_height);

try
{

	for (ci = 0; ci < 3; ++ci)
	{
		offset = startingOffset + comp_pos[ci];
		output_array = rawSampleImage[ci];

		for (row = 0; row < comp_height[ci]; ++row)
		{
			output_row = output_array[row];
			
			for (col = 0; col < comp_width[ci]; ++col, offset += comp_offset[ci])
			{
				output_row[col] = (JSAMPLE)param.buffer[offset];
			}
		}
	}
}
catch (...)
{
	offset = offset;
}

	DumpSampleImage(param, rawSampleImage);

	return (startingOffset + _rawSampleImageBufferSize);
}

aafUInt32 CAAFJPEGCodec::CopyDataFromSampleImage(
	JSAMPIMAGE rawSampleImage, 
	aafUInt32 startingOffset,
	aafCompressionParams& param) // throw HRESULT
{
	// We currently only support 601 and 4-2-2.
	checkExpression(1 == param.verticalSubsampling && 2 == param.horizontalSubsampling &&
									kAAFColorSpaceYUV == param.colorSpace,
				          AAFRESULT_BADPIXFORM); // AAFRESULT_BADLAYOUT

	DumpSampleImage(param, rawSampleImage);

	// Move the CbYCrY' elements from the interleaved compression buffer
 	// out into the separate color component images within the given raw sample
	// image. This sample image is suitable for input to the jpeg_write_raw_data
	// function of IJG 6(b).

	// The following code was modelled on the IJG null_convert in jccolor.c:
	aafUInt32 ci, row, offset, col, comp_pos[3], comp_offset[3], comp_width[3], comp_height[3];
	JSAMPARRAY input_array = NULL;
	JSAMPROW input_row = NULL;

	SetupFor422(param.imageWidth, comp_pos, comp_offset, comp_width, comp_height);

	for (ci = 0; ci < 3; ++ci)
	{
		offset = startingOffset + comp_pos[ci];
		input_array = rawSampleImage[ci];

		for (row = 0; row < comp_height[ci]; ++row)
		{
			input_row = input_array[row];
			
			for (col = 0; col < comp_width[ci]; ++col)
			{
				param.buffer[offset] = (aafUInt8)input_row[col];
				offset += comp_offset[ci];
			}
		}
	}

	return (startingOffset + _rawSampleImageBufferSize);
}



// Compress a single image data from the given buffer. Return the actual
// number of bytes written.
HRESULT CAAFJPEGCodec::CompressImage(
	const aafCompressionParams& param, 
	struct jpeg_compress_struct& cinfo)
{
	HRESULT hr = S_OK;

	if (NULL == param.buffer)
		return AAFRESULT_NULL_PARAM;
	else if (0 >= param.imageWidth || 0 >= param.imageHeight || 0 >= param.rowBytes)
		return AAFRESULT_INVALID_PARAM;
	// We must write something.
	else if (param.bufferSize < (param.imageHeight * param.rowBytes))
		return AAFRESULT_SMALLBUF;

	/* More stuff */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */


	try
	{
		// The stream must have already be created or opened.
		checkAssertion(NULL != _stream);

		// This version only supports compression of RGB data
//		checkExpression(1 == param.horizontalSubsampling, AAFRESULT_BADPIXFORM);

		// This should already have been calculated.
		checkAssertion(0 < param.rowBytes);

		/* Step 3: set parameters for compression */

		/* First we supply a description of the input image.
		 * Four fields of the cinfo struct must be filled in:
		 */
		cinfo.image_width = param.imageWidth; 	/* image width and height, in pixels */
		cinfo.image_height = param.imageHeight;
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	

		/* colorspace of input image */
		switch (param.colorSpace)
		{
			case kAAFColorSpaceYUV:
			case kAAFColorSpaceYCrCb:
				cinfo.in_color_space = JCS_YCbCr;
				break;

			default:
				cinfo.in_color_space = JCS_RGB;
		}



		/* Now use the library's routine to set default compression parameters.
		 * (You must set at least cinfo.in_color_space before calling this,
		 * since the defaults depend on the source color space.)
		 */
		jpeg_set_defaults(&cinfo);


		/* Now you can set any non-default parameters you wish to.
		 * Here we just illustrate the use of quality (quantization table) scaling:
		 */
		jpeg_set_quality(&cinfo, param.quality, TRUE /* limit to baseline-JPEG values */);



		if (JCS_YCbCr == cinfo.in_color_space && 
			  (1 != param.horizontalSubsampling || 1 != param.verticalSubsampling) )
		{
			// Notify the library that we are supplying raw YCbCr data.
			cinfo.raw_data_in = TRUE; // this was set to FALSE by jpeg_set_defaults.

			// Ensure that the sampling factors are correct.
			cinfo.comp_info[0].h_samp_factor = 2;
			cinfo.comp_info[0].v_samp_factor = 2;
			cinfo.comp_info[1].h_samp_factor = 1;
			cinfo.comp_info[1].v_samp_factor = 2;
			cinfo.comp_info[2].h_samp_factor = 1;
			cinfo.comp_info[2].v_samp_factor = 2;


			// Get the raw image buffer (allocate one if necessary).
			JSAMPIMAGE rawImage = GetRawSampleImage(param); // may be out of memory.

		  /* Step 4: Start compressor */

			/* TRUE ensures that we will write a complete interchange-JPEG file.
			 * Pass TRUE unless you are very sure of what you're doing.
			 */
			jpeg_start_compress(&cinfo, TRUE);


			/* Step 5: Write the image data into the compressor */

			// Compress the raw subsampled data one MCU row at a time.
			aafUInt32 total_rows_with_MCU = param.imageHeight + (param.imageHeight % DCTSIZE);
			aafUInt32 total_cols_with_MCU = param.imageWidth + (param.imageWidth % DCTSIZE);
			aafUInt32 num_MCU_rows = 0;

			/* Make sure that at least one iMCU row has been passed. */
			aafUInt32 lines_per_iMCU_row = cinfo.max_v_samp_factor * DCTSIZE;

			// 2 MCU rows of Y and 1 MCU row for each chromonance component.
			aafUInt32 offset_to_next_iMCU_row = 0;

			while (num_MCU_rows < total_rows_with_MCU)
			{
				// Now map the input interleaved image data into the rawImage buffer.
				// NOTE: The interleaved format is 601 4-2-2 (SMPTE 259).
				offset_to_next_iMCU_row = CopyDataToSampleImage(param, offset_to_next_iMCU_row, rawImage);

				num_MCU_rows += jpeg_write_raw_data(&cinfo, rawImage, lines_per_iMCU_row);
			}
		} 
		else
		{
			/* Step 4: Start compressor */

			/* TRUE ensures that we will write a complete interchange-JPEG file.
			 * Pass TRUE unless you are very sure of what you're doing.
			 */
			jpeg_start_compress(&cinfo, TRUE);


			/* Step 5: Write the image data into the compressor */

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

	/* And we're done! */
	
	
	return hr;
}


// Decompress a single image from the current position in the stream returning
// the image data in buffer and the actual number of bytes written. Note: bufLen must
// be large enough to hold all of the decompressed data
HRESULT CAAFJPEGCodec::DecompressImage(
	aafCompressionParams& param, 
	struct jpeg_decompress_struct& cinfo)
{
	HRESULT hr = S_OK;


	if (NULL == param.buffer)
		return AAFRESULT_NULL_PARAM;
	// We must be able to read something.
	else if (0 == param.bufferSize)
		return AAFRESULT_SMALLBUF;



  /* More stuff */
  JSAMPROW row_pointer[1];		/* Output row buffer */
	

	try
	{
		// The stream must have already be created or opened.
		checkAssertion(NULL != _stream);

		// This version only supports compression of RGB data
//		checkExpression(1 == _horizontalSubsampling, AAFRESULT_BADPIXFORM);

		
		/* Step 3: read file parameters with jpeg_read_header() */

		(void) jpeg_read_header(&cinfo, TRUE);
		/* We can ignore the return value from jpeg_read_header since
		 *   (a) suspension is not possible with the essence stream data source, and
		 *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
		 * See libjpeg.doc for more info.
		 */


		/* Step 4: set parameters for decompression */

		// Set the output color space.
		switch (param.colorSpace)
		{
			case kAAFColorSpaceYUV:
			case kAAFColorSpaceYCrCb:
				cinfo.out_color_space = JCS_YCbCr;
				break;

			default:
				cinfo.out_color_space = JCS_RGB;
		}


		/* In this example, we don't need to change any of the defaults set by
		 * jpeg_read_header(), so we do nothing here.
		 */



		if (JCS_YCbCr == cinfo.out_color_space && 
			  (1 != param.horizontalSubsampling || 1 != param.verticalSubsampling) )
		{
			// Notify the library that we are requresting raw YCbCr data.
			cinfo.raw_data_out = TRUE; // this was set to FALSE by jpeg_set_defaults.

			// Ensure that the sampling factors are correct.
//			cinfo.comp_info[1].h_samp_factor = param.horizontalSubsampling;
//			cinfo.comp_info[1].v_samp_factor = param.verticalSubsampling;
//			cinfo.comp_info[2].h_samp_factor = param.horizontalSubsampling;
//			cinfo.comp_info[2].v_samp_factor = param.verticalSubsampling;


			/* Step 5: Start decompressor */

 			(void) jpeg_start_decompress(&cinfo);
			/* We can ignore the return value since suspension is not possible
			 * with the essence stream data source.
			 */


			// Get the raw image buffer (allocate one if necessary).
			JSAMPIMAGE rawImage = GetRawSampleImage(param); // may be out of memory.

			// Compress the raw subsampled data one MCU row at a time.
			aafUInt32 total_rows_with_MCU = param.imageHeight + (param.imageHeight % DCTSIZE);
			aafUInt32 total_cols_with_MCU = param.imageWidth + (param.imageWidth % DCTSIZE);
			aafUInt32 num_MCU_rows = 0;

			/* Make sure that at least one iMCU row has been passed. */
			aafUInt32 lines_per_iMCU_row = cinfo.max_v_samp_factor * DCTSIZE;

			// 2 MCU rows of Y and 1 MCU row for each chromonance component.
			aafUInt32 offset_to_next_iMCU_row = 0;

			
			while (num_MCU_rows < total_rows_with_MCU)
			{
				num_MCU_rows += jpeg_read_raw_data(&cinfo, rawImage, lines_per_iMCU_row);

				// Now map the output interleaved image data from the rawImage buffer.
				// NOTE: The interleaved format is 601 4-2-2 (SMPTE 259).
				offset_to_next_iMCU_row = CopyDataFromSampleImage(rawImage, offset_to_next_iMCU_row, param);
			}
			

		} 
		else
		{
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
		offset = streamSize - kAAFJPEG_MarkerSize;
		checkResult(stream->Seek(offset));
		checkResult(stream->Read(kAAFJPEG_MarkerSize, trailer.endMarker, &bytesRead));
		checkAssertion(kAAFJPEG_MarkerSize == bytesRead);
		checkExpression(0 == memcmp(kAAFJPEG_end, trailer.endMarker, kAAFJPEG_MarkerSize),
			              AAFRESULT_NOFRAMEINDEX);
		
		// Read the 8 bytes size of the trailer. This is always in big-endian just like 
		// the JPEG stream...
		offset = streamSize - (kAAFJPEG_MarkerSize + sizeof(aafLength_t));
		checkResult(stream->Seek(offset));
		checkResult(stream->Read(sizeof(aafLength_t), (aafDataBuffer_t)&trailer.sizeOfTrailer, &bytesRead));
		checkAssertion(sizeof(aafLength_t) == bytesRead);
		// Swap the data if necessary...
		if (INTEL_ORDER == _nativeByteOrder)
			AAFByteSwap64((aafInt64 *)&trailer.sizeOfTrailer);

		// Make sure that there are enough bytes in the stream to read the rest of the trailer.
		checkExpression(streamSize >= trailer.sizeOfTrailer, AAFRESULT_NOFRAMEINDEX);
		offset = streamSize - trailer.sizeOfTrailer;
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

		// Set the position to the end of the essence data so that we 
		// are ready to read the sample index.
		offset = streamSize - trailer.sizeOfTrailer;
		offset -= numberOfSamples * sizeof(aafPosition_t);
		checkResult(stream->Seek(offset));
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
			if (kAAFMediaOpenReadOnly != _openMode)
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


HRESULT CAAFJPEGCodec::ReadSampleIndex32(aafUInt16 fileByteOrder)
{
	HRESULT hr = S_OK;
	aafUInt32		*tempBuf, index, numActualSamples;
	
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
		numActualSamples = count;		// Assume until proven otherwise below.
		bufferLen = count * sizeof(aafUInt32);
		bytesRead = 0;
		tempBuf = new aafUInt32[count];
		checkResult(_stream->Read(bufferLen, (aafDataBuffer_t)tempBuf, &bytesRead));
		
		
		// Swap the index bytes if necessary.
		if (fileByteOrder != _nativeByteOrder)
		{
			aafUInt32 index;

			for (index = 0; index < count; ++index)
			{
				AAFByteSwap32((aafInt32 *)&tempBuf[index]);
			}
		}

		// An offset of zero in a position other than 0 in the table means
		// that we have hit the logical end of a table which was padded to
		// sector boundries.
		// Transfer the first sample outside of the loop to save one check inside.
		_sampleIndex[0] = tempBuf[0];
		for (index = 1; index < count; ++index)
		{
			if(tempBuf[index] == 0)
			{
				numActualSamples = index - 1;		// -1 because there is a sample for one off the end
				SetNumberOfSamples(numActualSamples);
				break;
			}
			_sampleIndex[index] = tempBuf[index];
		}

		// Add the extra frame to the sample index so that the last element
		// can contain the position one byte beyond the end of last sample.
		// This offset should be equal to the position of the beginning
		// of the sample index. This means that we can always use 
		// pSampleIndex[index + 1] - pSampleIndex[index] to calculate
		// the sample size for the given (valid) index.
//		_sampleIndex[numActualSamples] = startingPosition;
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
	HRESULT hr = S_OK;
  aafUInt32 bytesWritten;

	
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
		checkResult(_stream->Write(bufferLen, (aafDataBuffer_t)_sampleIndex, &bytesWritten));

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
		checkResult(_stream->Write(kAAFJPEG_MarkerSize, (aafDataBuffer_t)trailer.startMarker, &bytesWritten));
		checkResult(_stream->Write(sizeof(aafLength_t),(aafDataBuffer_t)&trailer.numberOfSamples, &bytesWritten));
		
		// and other data...

		checkResult(_stream->Write(sizeof(aafLength_t), (aafDataBuffer_t)&trailer.sizeOfTrailer, &bytesWritten));
		checkResult(_stream->Write(kAAFJPEG_MarkerSize, (aafDataBuffer_t)trailer.endMarker, &bytesWritten));
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

HRESULT CAAFJPEGCodec::WriteSampleIndex32()
{
	HRESULT hr = S_OK;
	aafUInt32 bytesWritten;
	aafUInt16	count16;
	aafUInt32	*tempBuf, startPos32;
	
	try
	{
		// PRECONDITION
		checkAssertion(NULL != _stream && NULL != _sampleIndex);

		// Save the starting position. This should be after the last byte of
		// jpeg essence.
		aafPosition_t startingPosition;
		checkResult(_stream->GetPosition(&startingPosition));
		startPos32 = (aafUInt32)startingPosition;
		// Assert that (aafInt64)startPos32 == startingPosition

		// Write the entire index in a single operation to minimize i/o.
		// If the jpeg essence stream contained enough samples at 30fps for 24 hrs
		// the frame index will be approximately 20MB.
		aafUInt32 bufferLen, index;

		// if we have too many samples for aafUInt32 were in trouble!
		count16 = static_cast<aafUInt16>(_numberOfSamples); 
		// Assert that (aafUInt32)count16 == _numberOfSamples

		checkResult(_stream->Write(sizeof(count16), (aafDataBuffer_t)&count16, &bytesWritten));

		tempBuf = new aafUInt32[count16+2];

		for (index = 0; index < count16; ++index)
		{
			tempBuf[index] = (aafUInt32)_sampleIndex[index];
			// Assert here that tempBuf[index] == _sampleIndex[index]
		}
		tempBuf[count16] = (aafUInt32)startingPosition;
		// Assert here that (aafInt64)tempBuf[count16] == startingPosition
		tempBuf[count16+1] = 0;
		
		bufferLen = (count16+2) * sizeof(aafUInt32);		// Ad zero byte to end
		checkResult(_stream->Write(bufferLen, (aafDataBuffer_t)tempBuf, &bytesWritten));

		delete tempBuf;

		checkResult(_descriptorHelper.PutFrameIndexProperties(startPos32+2, _nativeByteOrder));
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
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFJPEGCodec::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IAAFEssenceCodec interface 
    if (EQUAL_UID(riid,IID_IAAFEssenceCodec)) 
    { 
        *ppvObj = (IAAFEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
		// and the IAAFPlugin interface.
    else if (EQUAL_UID(riid,IID_IAAFPlugin)) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFJPEGCodec)
