/***********************************************************************
 *
 *              Copyright (c) 2000 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
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
#include "CAAFCDCICodec.h"

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

#include "CAAFBuiltinDefs.h"


// {C995E9A9-4156-11d4-A367-009027DFCA6A}
static const CLSID CLSID_AAFCDCICodec = 
{ 0xc995e9a9, 0x4156, 0x11d4, { 0xa3, 0x67, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

// This plugin currently only supports a single definition
const aafUInt32 kSupportedDefinitions = 1;

// This plugin currently only supports a single code flavour
const aafUInt32 kSupportedCodeFlavours = 1;

const wchar_t kDisplayName[] = L"AAF CDCI Codec";
const wchar_t kDescription[] = L"Handles YUV and YCbCr";

const aafProductVersion_t kAAFPluginVersion = {1, 0, 0, 1, kAAFVersionBeta};
const aafRational_t		kDefaultRate = { 30000, 1001 };
const aafInt32			kDefaultPixelWidth = 24;
const aafUInt32			kDefaultNumCh = 1;
const aafInt32			kDefaultComponentWidth = 8;

//
// Plugin Descriptor information
//
// {C995E9AA-4156-11d4-A367-009027DFCA6A}
const aafUID_t AVID_CDCI_PLUGIN = 
{ 0xc995e9aa, 0x4156, 0x11d4, { 0xa3, 0x67, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

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
const aafUInt32		numComponents = 3;

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

CAAFCDCICodec::CAAFCDCICodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{

	_nativeByteOrder = GetNativeByteOrder();
	_access = NULL;
	_stream = NULL;
	_openMode = kAAFMediaOpenReadOnly;
	_compressEnable = kAAFCompressionEnable;
	_length = 0;  // 64 bit int
	_sampleRate = NULL_RATIONAL;
	_containerFormat = NULL_ID;
	_codecID = kAAFCodecCDCI;
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
	_pixelFormat = kAAFColorSpaceRGB;
	_fieldDominance = kAAFNoDominant;
	_memBitsPerPixel = kDefaultPixelWidth;
	_bitsPerPixelAvg = kDefaultPixelWidth;
	_memBytesPerSample = 0;
	_bitsPerSample = 0;
	_numberOfSamples = 0;
	_padBytesPerRow = 0;
	_clientFillStart = 0;
	_clientFillEnd = 0;
}


// Desctructor

CAAFCDCICodec::~CAAFCDCICodec ()
{
  // Do NOT release the _access interface since this object
  // contains the reference to this codec instance! We need
  // avoid the dreaded reference counting cycle of death!
	if(_stream != NULL)
	{
		_stream->Release();
		_stream = NULL;
	}
}


// Only save a new reference.
void CAAFCDCICodec::SetEssenceStream(IAAFEssenceStream *stream)
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

void CAAFCDCICodec::SetCompressionEnabled (aafCompressEnable_t compEnable)
{
	// make sure parameter has a valid value.
	_compressEnable = (compEnable == kAAFCompressionEnable) ? kAAFCompressionEnable : kAAFCompressionDisable;
}

void CAAFCDCICodec::SetNumberOfSamples(const aafLength_t& numberOfSamples)
{
	_numberOfSamples = numberOfSamples;
}




HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountDefinitions (aafUInt32 *pDefCount)
{
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedDefinitionID (aafUInt32 index, aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = kAAFCodecCDCI;		// UID of the CDCI codec definition

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetPluginDescriptorID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AVID_CDCI_PLUGIN;		// UID of the PluginDescriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	HRESULT hr = S_OK;
	IAAFCodecDef	*codecDef = NULL;
	IAAFClassDef	*fileClass = NULL;
	IAAFDefObject	*obj = NULL;
	IAAFClassDef    *pcd = 0;
	aafUID_t		uid;
	
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;

	if (0 > kSupportedDefinitions || kSupportedDefinitions <= index)
		return AAFRESULT_BADINDEX;

	try
	{
		//!!!Later, add in dataDefs supported & filedescriptor class

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
		uid = kAAFCodecCDCI;
		checkResult(codecDef->Initialize(uid, 
		                      const_cast<wchar_t *>(kDisplayName),
		                      const_cast<wchar_t *>(kDescription)));

	  	checkResult(dict->LookupClassDef(AUID_AAFCDCIDescriptor, &fileClass));
		checkResult(codecDef->SetFileDescriptorClass (fileClass));
		fileClass->Release ();
		fileClass = 0;

		// Cleanup
		codecDef->Release();
		codecDef = NULL;


		// Return a reference to our initialized definition object.
		*def = obj; // reference count already incremented, we do not need to call AddRef().
		obj = NULL;
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
    CAAFCDCICodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
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

		checkResult(desc->Initialize(AVID_CDCI_PLUGIN,
		                       const_cast<wchar_t *>(kDisplayName),
		                       const_cast<wchar_t *>(kDescription)));
		checkResult(desc->SetCategoryClass(AUID_AAFDefObject));
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
    CAAFCDCICodec::SetEssenceAccess(IAAFEssenceAccess *access)
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
    CAAFCDCICodec::CountFlavours(aafUInt32 *pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pCount = kSupportedCodeFlavours;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedFlavourID (aafUInt32  index,
        aafUID_t *  pFlavour)
{
	if(pFlavour == NULL)
		return AAFRESULT_NULL_PARAM;

	*pFlavour = kAAFNilCodecFlavour;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	if (NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedDataDefinition (aafUInt32  index,
        aafUID_t * pFlavour)
{
	if (NULL == pFlavour)
		return AAFRESULT_NULL_PARAM;

  return HRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetMaxCodecDisplayNameLength (
        aafUInt32  *bufSize)
{
	if (NULL == bufSize)
		return AAFRESULT_NULL_PARAM;
	
	*bufSize = sizeof(kDisplayName)/sizeof(wchar_t);

	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetCodecDisplayName (aafUID_constref flavour,
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
    CAAFCDCICodec::CountChannels (IAAFSourceMob *fileMob,
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
    CAAFCDCICodec::GetSelectInfo (IAAFSourceMob *fileMob,
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
		CAAFCDCIDescriptorHelper descriptorHelper;
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
		if (AAFRESULT_SUCCESS == hr && EqualAUID(&codecID, &kAAFCodecCDCI) && 0 == padBits)
		{
			pSelectInfo->willHandleMDES = kAAFTrue;
		}
		else
		{ // If the compression property was not present or the 
			// give compression is not supported or padBits was not zero
			// the set to false.
			pSelectInfo->willHandleMDES = kAAFFalse;
		}


		pSelectInfo->isNative = kAAFTrue;

		// This is a software only codec.
		pSelectInfo->hwAssisted = kAAFFalse;

		pSelectInfo->relativeLoss = 0;

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


//  The following line from the omcCDCI.c (line 397?) appears to be computing the
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
    CAAFCDCICodec::CountSamples (
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
    CAAFCDCICodec::ValidateEssence (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
		aafCheckVerbose_t  verbose,
        aafCheckWarnings_t warning,
         aafUInt32  bufSize,
		wchar_t *  pName,
        aafUInt32  *bytesWritten)
{
	if (NULL == fileMob || NULL == stream ||
		  NULL == pName   || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;

	return HRESULT_NOT_IMPLEMENTED;
}

		
HRESULT STDMETHODCALLTYPE
CAAFCDCICodec::Create (IAAFSourceMob *unk,
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
    CAAFCDCICodec::Open (IAAFSourceMob *unk,
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
		// ????? use omcCDCI.c as a guide:


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
		checkResult(_descriptorHelper.GetCompression(&_codecID));
//		checkAssertion(kAAFTrue == EqualAUID(&_codecID, &CodecCDCI));
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
				checkAssertion(2 > _videoLineMapSize);
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

		// Get the compression factor?
		//
		//status = omfsReadInt32(main, media->mdes, pers->omAvJPEDResolutionID, &pdata->CDCI.CDCITableID);
		//if (status != OM_ERR_NONE)
		//	pdata->CDCI.CDCITableID = 0;
		
		UpdateCalculatedData();

		// Attempt to create and load the sample index from the end of the essence stream.
		// For now just fail if the index cannot be found.
		aafLength_t samples;

    // Make sure that we have created the sample index.
    checkResult(ReadNumberOfSamples(_stream, samples));
		SetNumberOfSamples(samples);

		// Reset the position to the start of the data so that the next read will 
		// be from the correct position.
		checkResult(Seek(0));
		
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
    CAAFCDCICodec::WriteSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *samplesWritten,
        aafUInt32 *bytesWritten)
{
	HRESULT hr = S_OK;
	aafUInt32 n, rowBytes, bufferSize;
	aafUInt32 bytesXfered = 0, samplesXfered = 0; // TBD: Need to be return arguments.
	aafUInt32 localWritten;

	if (NULL == buffer || NULL == samplesWritten || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;
	// We only support writing a single block at a time.
	else if (0 == nSamples)
		return AAFRESULT_INVALID_PARAM;
	// this codec only handles a single channel
	else if (1 != nSamples)
		return AAFRESULT_CODEC_CHANNELS;



	*bytesWritten = 0;
	try
	{
		// Preconditions:
		checkAssertion(NULL != _stream);

    checkExpression(_componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE);
		/* this codec only allows one-channel media */

			// If we are being asked compress the given buffer then
			// the we should have already calculated the size of a sample.
			checkExpression(_fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);

			// Compute the number of bytes in a single row of pixel data.
			if (1 == _horizontalSubsampling)
			{
				rowBytes = (_imageWidth * numComponents) + _padBytesPerRow;
			}
			else if (2 == _horizontalSubsampling)
			{	// Add an extra byte if with is odd. NOTE: This will never
				// happen with full 601 frame.
				rowBytes = (_imageWidth * (numComponents - 1)) + (_imageWidth % 2) + _padBytesPerRow;
			}
				

			// Calculate the size of the sample data to be compressed.
			bufferSize = rowBytes * _imageHeight;

			// Make sure the given buffer is really large enough for the complete
			// uncompressed pixel data.
			checkExpression(bufferSize <= buflen, AAFRESULT_SMALLBUF);
			
			// Adjust the parameters for separate fields...
			if (kAAFSeparateFields == _frameLayout)
			{
				bufferSize = rowBytes * (_imageHeight/2);
			}
			


			for (n = 0; n < nSamples; n++)
			{
				/* Step 2: specify data destination (eg, an IAAFEssenceStream) */
				/* Note: steps 2 and 3 can be done in either order. */
//				CDCI_essencestream_dest(&cinfo, _stream);

				
				buffer = &buffer[bytesXfered];
				checkResult(_stream->Write(bufferSize, buffer, &localWritten));
				bytesXfered += localWritten;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					buffer = &buffer[bytesXfered];
					checkResult(_stream->Write(bufferSize, buffer, &localWritten));
					bytesXfered += localWritten;
				}

				// Add padding for _imageAlignmentFactor
				if (0 < _imageAlignmentFactor)
				{
					aafUInt32 alignmentBytes = bytesXfered % _imageAlignmentFactor;

					// TODO: Allocate and use a "aligmentBuffer" so that all of the padding
					// be written in a single write operation.
					aafUInt32 i;
					aafUInt8 ch = 0;
					for (i = 0; i < alignmentBytes; ++i)
						checkResult(_stream->Write(1, &ch, &localWritten));
				}

				// Update the return values.
				samplesXfered++;
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


	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::WriteBlocks (aafDeinterleave_t  inter,
        aafUInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	HRESULT		hr = S_OK;
	aafUInt32	n, bufferSize, rowBytes, bytesWritten;
	aafUInt8	*buffer;

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
	else if (0 == xferBlock[0].numSamples)
		return AAFRESULT_INVALID_PARAM;
	// this codec only handles a single channel
	else if (1 != xferBlock[0].numSamples /*xferBlock[0].subTrackNum*/)
		return AAFRESULT_CODEC_CHANNELS;



	try
	{
		// Preconditions:
		checkAssertion(NULL != _stream);

    checkExpression(_componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE);
		/* this codec only allows one-channel media */

		if (kAAFCompressionEnable == _compressEnable)
		{
			// If we are being asked compress the given buffer then
			// the we should have already calculated the size of a sample.
			checkExpression(_fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);

			// Compute the number of bytes in a single row of pixel data.
			if (1 == _horizontalSubsampling)
			{
				rowBytes = (_imageWidth * numComponents) + _padBytesPerRow;
			}
			else if (2 == _horizontalSubsampling)
			{	// Add an extra byte if with is odd. NOTE: This will never
				// happen with full 601 frame.
				rowBytes = (_imageWidth * (numComponents - 1)) + (_imageWidth % 2) + _padBytesPerRow;
			}
				

			// Calculate the size of the sample data to be compressed.
			bufferSize = rowBytes * _imageHeight;

			// Make sure the given buffer is really large enough for the complete
			// uncompressed pixel data.
			checkExpression(bufferSize <= xferBlock[0].buflen, AAFRESULT_SMALLBUF);
			
			// Adjust the parameters for separate fields...
			if (kAAFSeparateFields == _frameLayout)
			{
				bufferSize = rowBytes * (_imageHeight/2);
			}
			


			for (n = 0; n < xferBlock[0].numSamples; n++)
			{
				/* Step 2: specify data destination (eg, an IAAFEssenceStream) */
				/* Note: steps 2 and 3 can be done in either order. */
//				CDCI_essencestream_dest(&cinfo, _stream);

				
				buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
				checkResult(_stream->Write(bufferSize, buffer, &bytesWritten));
				resultBlock[0].bytesXfered += bytesWritten;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
					checkResult(_stream->Write(bufferSize, buffer, &bytesWritten));
					resultBlock[0].bytesXfered += bytesWritten;
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


	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::ReadSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  samplesRead,
        aafUInt32 *  bytesRead)
{
	HRESULT hr = S_OK;
	aafUInt32 n, rowBytes, bufferSize, imageHeight;
	aafUInt32 sampleSize = 0;


	if (NULL == buffer || NULL == samplesRead || NULL == bytesRead)
		return AAFRESULT_NULL_PARAM;
	// We only support writing a single block at a time.
	// this codec only handles a single channel
	else if (1 != nSamples)
		return AAFRESULT_CODEC_CHANNELS;
//	else if (_currentIndex >= _writeIndex)
//		return (AAFRESULT_EOF);

  // Initialize the return parameters.
  *samplesRead = 0;
  *bytesRead = 0;


	try
	{
		
		if (kAAFCompressionEnable == _compressEnable)
		{ // If we are being asked decompress the given buffer.
			// Get the dimensions of the image data to compress.
			imageHeight = (kAAFSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;

			// Compute the number of bytes in a single row of pixel data.
			rowBytes = (buflen / nSamples) / _imageHeight;

			// Calculate the size of the image data to be compressed.
			bufferSize = rowBytes * imageHeight;

			for (n = 0; n < nSamples; n++)
			{
				/* Step 2: specify data source (eg, an IAAFEssenceStream) */
				// Setup the data source to read an entire sample frame into memory.
//				CDCI_essencestream_src(&cinfo, _stream, _fileBytesPerSample);


				buffer = &buffer[*bytesRead];
//				checkResult(DecompressImage(param, cinfo));
				*bytesRead += bufferSize;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					buffer = &buffer[*bytesRead];
//					checkResult(DecompressImage(param, cinfo));
					*bytesRead += bufferSize;
				}

				*samplesRead++;
				// Update the current index.
//				SetCurrentIndex(_currentIndex + 1);
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


	// Cleanup
	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::ReadBlocks (aafDeinterleave_t  inter,
        aafUInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	HRESULT hr = S_OK;
	aafUInt32 n, bufferSize, rowBytes, imageHeight;
	aafUInt32 sampleSize = 0;
	aafUInt8	*buffer;


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
	else if (1 != xferBlock[0].numSamples)
		return AAFRESULT_CODEC_CHANNELS;
//	else if (_currentIndex >= _writeIndex)
//		return (AAFRESULT_EOF);


	try
	{

			/* Step 1: allocate and initialize CDCI decompression object */

			/* We set up the normal CDCI error routines, then override error_exit. */
//			cinfo.err = CDCI_std_error(&jerr);
//			jerr.error_exit = cplusplus_error_exit;

			/* Now we can initialize the CDCI decompression object. */
//			CDCI_create_decompress(&cinfo);


			// Get the dimensions of the image data to compress.
			imageHeight = (kAAFSeparateFields == _frameLayout) ? (_imageHeight / 2) : _imageHeight;

			// Compute the number of bytes in a single row of pixel data.
			rowBytes = (xferBlock[0].buflen / xferBlock[0].numSamples) / _imageHeight;

			// Calculate the size of the image data to be compressed.
			bufferSize = rowBytes * imageHeight;

			for (n = 0; n < xferBlock[0].numSamples; n++)
			{
				/* Step 2: specify data source (eg, an IAAFEssenceStream) */
				// Setup the data source to read an entire sample frame into memory.
//				sampleSize = GetSampleSizeFromIndex(_currentIndex);
//				CDCI_essencestream_src(&cinfo, _stream, sampleSize);


				buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
//				checkResult(DecompressImage(param, cinfo));
				resultBlock[0].bytesXfered += bufferSize;

				if (kAAFSeparateFields == _frameLayout)
				{
					// Compress the second field right after the first field.
					buffer = &xferBlock[0].buffer[resultBlock[0].bytesXfered];
//					checkResult(DecompressImage(param, cinfo));
					resultBlock[0].bytesXfered += bufferSize;
				}

				++resultBlock[0].samplesXfered;
				// Update the current index.
//				SetCurrentIndex(_currentIndex + 1);
				//checkResult(Seek(_currentIndex + 1));
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
    CAAFCDCICodec::Seek (aafPosition_t  sampleFrame)
{
	HRESULT hr = S_OK;

	try
	{
		checkAssertion(NULL != _stream);
		checkExpression(sampleFrame <= _numberOfSamples, AAFRESULT_BADFRAMEOFFSET);
		checkResult(_stream->Seek((_fileBytesPerSample + _clientFillStart + _clientFillEnd) * sampleFrame));
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


void CAAFCDCICodec::UpdateDescriptor (CAAFCDCIDescriptorHelper& descriptorHelper)
{
	checkResult(descriptorHelper.SetLength(_numberOfSamples));
	//	checkResult(descriptorHelper.SetIsInContainer(_isInAAFContainer));
	checkResult(descriptorHelper.SetSampleRate(_sampleRate));
	//	checkResult(descriptorHelper.SetContainerFormat(_containerFormat));
	checkResult(descriptorHelper.SetCompression(_codecID));
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
}


// Routine to keep calculated member data up-to-date.
void CAAFCDCICodec::UpdateCalculatedData(void)
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
    CAAFCDCICodec::CompleteWrite (IAAFSourceMob *fileMob)
{
	HRESULT hr = S_OK;

	try
	{
		// Flush the CDCI data source (if necessary)

		// Write the sample index onto the end of the stream.
//		checkResult(WriteSampleIndex());

		// Make sure the descriptor information is up-to-date.
		UpdateDescriptor(_descriptorHelper); // throw HRESULT

		if (NULL != fileMob)
		{	// Initialize the descriptor helper:
			CAAFCDCIDescriptorHelper descriptorHelper;
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
    CAAFCDCICodec::WriteRawData (
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
		
		// Make sure that we have been opened for append.		
		checkExpression(kAAFMediaOpenAppend == _openMode, AAFRESULT_NOT_WRITEABLE);



		// We should only be able to write to the end of the stream. This should be 
		// enforced by the implementation of the IAAFEssenceStream. This is why we 
		// compute the offset to the start from the position after the write 
		// operation just in case the seek position had to be reset to the end by the
		// implementation of the IAAFEssenceStream.

		// Write the compressed sample data.
		checkResult(_stream->Write(buflen, buffer, &bytesWritten));
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
    CAAFCDCICodec::ReadRawData (aafUInt32 nSamples,
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

	*bytesRead = 0;
	*samplesRead = 0;

	try
	{
		checkAssertion(NULL != _stream);
		checkExpression(buflen >= _fileBytesPerSample, AAFRESULT_SMALLBUF);

		aafPosition_t offset;
		checkResult(_stream->GetPosition(&offset));
		checkResult(_stream->Read(_fileBytesPerSample, buffer, bytesRead));

		*samplesRead = 1; // we currently only support one sample reads...
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
    CAAFCDCICodec::CreateDescriptorFromStream (IAAFEssenceStream * pStream,
        IAAFSourceMob *fileMob)
{
	if (NULL == pStream || NULL == fileMob)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
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
    CAAFCDCICodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
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
				&param.opcode, sizeof(param.operand.expData), param.operand.expData, &param.size));
			
			if (EqualAUID(&kAAFNumChannels, &param.opcode))
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
				checkExpression(param.size == sizeof(param.operand.expInt16), AAFRESULT_INVALID_PARM_SIZE);
				
				// We currently only support 8 bit components...
				_componentWidth = param.operand.expInt16;
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
			else if (EqualAUID(&kAAFCDCIClientFillStart, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expUInt16), AAFRESULT_INVALID_PARM_SIZE);
				_clientFillStart = param.operand.expUInt16;
			}
			else if (EqualAUID(&kAAFCDCIClientFillEnd, &param.opcode))
			{	// Validate the in-memory size.
				checkExpression(param.size == sizeof(param.operand.expUInt16), AAFRESULT_INVALID_PARM_SIZE);
				_clientFillEnd = param.operand.expUInt16;
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
    CAAFCDCICodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
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
				checkResult(fmt->AddFormatSpecifier (kAAFAspectRatio, sizeof(param.operand.expRect), (aafDataBuffer_t)&param.operand.expRational));
			}
			else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
			{	// We currently only support 8 bit components...
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
    CAAFCDCICodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
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
		// Initially use the same setup as omcCDCI.c...
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
    CAAFCDCICodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFCDCIDescriptor; // stored class UID of Compressed Digital Image Decriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetEssenceDataID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFEssenceData;			// stored class UID of the CDCI data object

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetIndexedSampleSize (aafUID_constref dataDefID,
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
			// The samples will be uncompressed so return the previously
			// computed value for a single uncompressed sample.
			*pResult = _fileBytesPerSample;
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
    CAAFCDCICodec::GetLargestSampleSize (aafUID_constref dataDefID,
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
			// The samples will be uncompressed so return the previously
			// computed value for a single uncompressed sample.
			*pResult = _fileBytesPerSample;
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








HRESULT CAAFCDCICodec::ReadNumberOfSamples(
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
		aafUInt32 bytesRead = 0;

		checkResult(stream->GetLength(&streamSize));
		// Make sure there is enough room to read the trailer.
//		checkExpression(streamSize >= kAAFCDCI_MarkerSize, AAFRESULT_NOFRAMEINDEX);
		
#if 0
		// Read and validate the trailer...
		AAFCDCI_Trailer trailer;
		memset(&trailer, 0, sizeof(trailer));

		// make sure the last 8 bytes match our expected end marker.
		offset = streamSize - kAAFCDCI_MarkerSize;
		checkResult(stream->Seek(offset));
		checkResult(stream->Read(kAAFCDCI_MarkerSize, trailer.endMarker, &bytesRead));
		checkAssertion(kAAFCDCI_MarkerSize == bytesRead);
		checkExpression(0 == memcmp(kAAFCDCI_end, trailer.endMarker, kAAFCDCI_MarkerSize),
			              AAFRESULT_NOFRAMEINDEX);
		
		// Read the 8 bytes size of the trailer. This is always in big-endian just like 
		// the CDCI stream...
		offset = streamSize - (kAAFCDCI_MarkerSize + sizeof(aafLength_t));
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
		checkResult(stream->Read(kAAFCDCI_MarkerSize, trailer.startMarker, &bytesRead));
		checkAssertion(kAAFCDCI_MarkerSize == bytesRead);
		checkExpression(0 == memcmp(kAAFCDCI_start, trailer.startMarker, kAAFCDCI_MarkerSize),
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
#endif
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
HRESULT CAAFCDCICodec::InternalQueryInterface
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
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFCDCICodec)
