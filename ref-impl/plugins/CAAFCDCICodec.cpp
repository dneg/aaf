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
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"


// {C995E9A9-4156-11d4-A367-009027DFCA6A}
const CLSID CLSID_AAFCDCICodec = 
{ 0xc995e9a9, 0x4156, 0x11d4, { 0xa3, 0x67, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

// {EDB35391-6D30-11d3-A036-006094EB75CB}
const aafUID_t AAF_CMPR_AUNC422 = { 0xedb35391, 0x6d30, 0x11d3, { 0xa0, 0x36, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };

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

CAAFCDCICodec::CAAFCDCICodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{

	_nativeByteOrder = GetNativeByteOrder();
	_access = NULL;
	_stream = NULL;
	_openMode = kAAFMediaOpenReadOnly;
	_length = 0;
	_sampleRate = NULL_RATIONAL;
	_containerFormat = NULL_UID;
	_compression = NULL_UID;
	_storedHeight = 0;
	_storedWidth = 0;
	_sampledHeight = 0;
	_sampledWidth = 0;
	_sampledXOffset = 0;
	_sampledYOffset = 0;
	_displayHeight = 0;
	_displayWidth = 0;
	_displayXOffset = 0;
	_displayYOffset = 0;
	_frameLayout = kAAFFullFrame;
	_videoLineMapSize = 0;
	_videoLineMap[0] = _videoLineMap[1] = 0;
	_imageAspectRatio = NULL_RATIONAL;
	_alphaTransparency = kAAFMinValueTransparent;
	_gamma = NULL_UID;
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
	_pixelFormat = kAAFColorSpaceYUV;
	_fieldDominance = kAAFNoDominant;
	_fieldStartOffset = 0;
	_fieldEndOffset = 0;
	_bitsPerPixelAvg = kDefaultPixelWidth;
	_bitsPerSample = 0;
	_numberOfSamples = 0;
	_padBytesPerRow = 0;
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


// MediaComposer property extensions to AAFDigitalImageDescriptor.
const aafUID_t kAAFPropID_DIDResolutionID = { 0xce2aca4d, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameSampleSize = { 0xce2aca50, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafCharacter kAAFPropName_DIDResolutionID[] = { 'R','e','s','o','l','u','t','i','o','n','I','D','\0' };
const aafCharacter kAAFPropName_DIDFrameSampleSize[] = { 'F','r','a','m','e','S','a','m','p','l','e','S','i','z','e','\0' };


// The method creates some descriptor properties to handle legacy CDCI 
// variations.
HRESULT STDMETHODCALLTYPE CAAFCDCICodec::CreateLegacyPropDefs( 
    IAAFDictionary	*p_dict )
{
    HRESULT		hr = S_OK;
    IAAFClassDef	*p_did_classdef = NULL;
    IAAFTypeDef		*p_typedef_int32 = NULL;
    IAAFPropertyDef	*p_propdef = NULL;


    try
    {
	// Get DigitalImage descriptor class definition and
	// and integer type definition.
	checkResult( p_dict->LookupClassDef( kAAFClassID_DigitalImageDescriptor, 
	    &p_did_classdef ) );
	checkResult( p_dict->LookupTypeDef( kAAFTypeID_Int32, 
	    &p_typedef_int32 ) );


	// Register legacy property definitions
	checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
	    kAAFPropID_DIDResolutionID, kAAFPropName_DIDResolutionID, 
	    p_typedef_int32, &p_propdef ) );
	p_propdef->Release();
	p_propdef = NULL;

	checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
	    kAAFPropID_DIDFrameSampleSize, kAAFPropName_DIDFrameSampleSize, 
	    p_typedef_int32, &p_propdef ) );
	p_propdef->Release();
	p_propdef = NULL;

    }
    catch( HRESULT& rhr )
    {
	hr = rhr; // return thrown error code.
    }

    if( p_did_classdef )
	p_did_classdef->Release();
    if( p_typedef_int32 )
	p_typedef_int32->Release();
    if( p_propdef )
	p_propdef->Release();


    return hr;
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

		// Creates descriptor properties to handle legacy CDCI.
		checkResult( CreateLegacyPropDefs( dict ) );

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
		*pNumChannels = 1;
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
	aafUInt32		compWidth, bitsPerPixel;
	aafUInt32		subSampling;
	aafRational_t		sampleRate = NULL_RATIONAL;
	aafFrameLayout_t	frameLayout;
	aafUInt32		numFields = 0;
	aafUInt32		bitsPerSample; // uncompressed.
	aafUID_t		compressionID = NULL_UID;


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
		hr = descriptorHelper.GetCompression(&compressionID);
		checkExpression(AAFRESULT_PROP_NOT_PRESENT == hr || AAFRESULT_SUCCESS == hr, hr);
		if (AAFRESULT_SUCCESS == hr && EqualAUID(&compressionID, &kAAFCodecCDCI) && 0 == padBits)
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

		checkResult(descriptorHelper.GetFrameLayout(&frameLayout));

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



// Check input arguments.
// Use the given source mob to initialize codec's descriptor helper.
// Use the descriptor helper to read codec parameters from descriptor.
// Set appropriate open mode.
// Set parameters which require specific initialization upon essence creation.
// Save the given essence stream to use with codec.
// Call UpdateCalculatedData() to set calculated parameters.
// Finally, put modified codec parameters back to descriptor calling 
//  UpdateDescriptor().
//
HRESULT STDMETHODCALLTYPE
CAAFCDCICodec::Create (IAAFSourceMob *p_srcmob,
  aafUID_constref flavour,
  aafUID_constref essenceKind,
  aafRational_constref sampleRate,
  IAAFEssenceStream * stream,
  aafCompressEnable_t compEnable)
{
    HRESULT	    hr = S_OK;

    
    // Check input arguments.
    if( NULL == p_srcmob || NULL == stream )
	return AAFRESULT_NULL_PARAM;

    if( EqualAUID( &kAAFNilCodecFlavour, &flavour) != kAAFTrue )
	return AAFRESULT_NULL_PARAM;


    try
    {
	    // Check essence kind
	    checkExpression( kAAFTrue == EqualAUID(&essenceKind, &DDEF_Picture),
		AAFRESULT_INVALID_DATADEF );

	    // Initialize the descriptor helper:
	    checkResult(_descriptorHelper.Initialize(p_srcmob));

	    // Initialize codec parameters reading descriptor information.
	    ReadDescriptor( _descriptorHelper );

	    // We are allowed to write to the given stream.
	    _openMode = kAAFMediaOpenAppend;

	    // Save the given essence stream.
	    SetEssenceStream(stream);

	    // Reset some parameters.
	    _imageAspectRatio = DEFAULT_ASPECT_RATIO;
	    _sampleRate = sampleRate;
	    _length = 0;
	    _numberOfSamples = 0;
	    UpdateCalculatedData();

	    // Update descriptor.
	    UpdateDescriptor( _descriptorHelper );
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



// Check input arguments.
// Use the given source mob to initialize codec's descriptor helper.
// Use the descriptor helper to read codec parameters from descriptor.
// Set appropriate open mode.
// Save the given essence stream to use with codec.
//
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::Open (IAAFSourceMob *p_srcmob,
		aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t compEnable)
{
    HRESULT hr = S_OK;


    // Check input arguments.
    if( p_srcmob == NULL || stream == NULL )
	return AAFRESULT_NULL_PARAM;

    try
    {
	// Initialize the descriptor helper:
	checkResult( _descriptorHelper.Initialize( p_srcmob ) );

	// Save the mode to the given stream.
	_openMode = openMode;

	// Save the given essence stream.
	SetEssenceStream(stream);

	// Initialize codec parameters reading descriptor information.
	ReadDescriptor( _descriptorHelper );
    }
    catch (HRESULT& rhr)
    {
	    hr = rhr; // return thrown error code.
    }
    catch(...)
    {
	    // We CANNOT throw an exception out of a COM interface method!
	    // Return a reasonable exception code.
	    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }


    return hr;
}



HRESULT STDMETHODCALLTYPE CAAFCDCICodec::WriteSamples(
	aafUInt32	nSamples,
        aafUInt32	buflen,
        aafDataBuffer_t	buffer,
        aafUInt32	*pTotalSamplesWritten,
        aafUInt32	*pTotalBytesWritten)
{
    HRESULT	hr = S_OK;
    aafUInt32	n;
    aafUInt32	sampleBytesWritten;	// Number of sample bytes written.
    aafUInt32	sampleSize;		// Number of bytes per sample.
    aafUInt32	alignmentBytes = 0;	// Number of alignment bytes.
    aafUInt8	*pAligmentBuffer = NULL;// Padding bytes to be written.


    try
    {
	// Check input args
	checkExpression( buffer != NULL && pTotalSamplesWritten != NULL && 
		pTotalBytesWritten != NULL, AAFRESULT_NULL_PARAM );

	// Init output
	*pTotalBytesWritten = 0;
	*pTotalSamplesWritten = 0;

	// More input args check
	checkExpression( nSamples != 0, AAFRESULT_INVALID_PARAM );

	// Preconditions.
	checkAssertion( NULL != _stream );
	checkExpression( _componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE );
	checkExpression( _fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE );

	sampleSize = _fileBytesPerSample;

	// Make sure the given buffer is really large enough for the complete
	// pixel data.
	checkExpression( sampleSize*nSamples <= buflen, AAFRESULT_SMALLBUF );



	// Write samples one by one aligning them if necessary.
	for( n=0; n<nSamples; n++ )
	{
	    sampleBytesWritten = 0;
	    checkResult( _stream->Write( sampleSize, buffer, 
		&sampleBytesWritten));
	    buffer += sampleBytesWritten;
	    *pTotalBytesWritten += sampleBytesWritten;


	    // Add padding for _imageAlignmentFactor.
	    // Do not count these bytes when returning 
	    // total number of bytes written.
	    if( _imageAlignmentFactor > 0 )
	    {
		alignmentBytes = sampleBytesWritten % _imageAlignmentFactor;

		if( alignmentBytes > 0)
		{
		    pAligmentBuffer = new aafUInt8[ alignmentBytes ];
		    memset( static_cast<void*>(pAligmentBuffer), 0, 
			    alignmentBytes );
		    checkResult( _stream->Write( 
			    alignmentBytes, 
			    static_cast<aafDataBuffer_t>(pAligmentBuffer), 
			    &sampleBytesWritten ) );
		    delete[] pAligmentBuffer;
		    pAligmentBuffer = NULL;
		}
	    }

	    SetNumberOfSamples( _numberOfSamples + 1 );

	    // Update total number of samples written.
	    (*pTotalSamplesWritten)++;
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

    delete[] pAligmentBuffer;


    return hr;
}



/*
	nSamples		- number of samples to read.
	buflen			- size of the buffer to hold read data.
	buffer			- buffer to hold read data.
	pTotalSamplesRead	- number of read samples.
	pTotalBytesRead		- number of read bytes. This covers only 
				    number of bytes transfered to user buffer. 
				    Alignment bytes are not counted here.
*/
HRESULT STDMETHODCALLTYPE CAAFCDCICodec::ReadSamples(
	aafUInt32	nSamples,
	aafUInt32	buflen,
	aafDataBuffer_t	buffer,
	aafUInt32	*pTotalSamplesRead,
	aafUInt32	*pTotalBytesRead )
{
    HRESULT	hr = S_OK;
    aafUInt32	n;
    aafUInt32	sampleBytesRead;	// Number of sample bytes read.
    aafUInt32	sampleSize;		// Number of bytes per sample.
    aafUInt32	alignmentBytes;		// Number of unused alignment bytes.


    try
    {
	// Check input args
	checkExpression( buffer != NULL && pTotalSamplesRead != NULL && 
		pTotalBytesRead != NULL, AAFRESULT_NULL_PARAM );

	// Init output
	*pTotalSamplesRead = 0;
	*pTotalBytesRead = 0;

	// More input args check
	checkExpression( nSamples != 0, AAFRESULT_INVALID_PARAM );

	// Preconditions.
	checkAssertion( NULL != _stream );
	checkExpression( _componentWidth != 0, AAFRESULT_ZERO_PIXELSIZE );
	checkExpression( _fileBytesPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE );

	sampleSize = _fileBytesPerSample;

	// Make sure the given buffer is really large enough for the complete
	// pixel data.
	checkExpression( sampleSize*nSamples <= buflen, AAFRESULT_SMALLBUF );



	// Read samples one by one skipping alignment data
	for( n = 0; n < nSamples; n++ )
	{
	    sampleBytesRead = 0;
	    checkResult( _stream->Read( sampleSize, buffer, &sampleBytesRead ));
	    buffer += sampleBytesRead;
	    *pTotalBytesRead += sampleBytesRead;


	    // If sample is aligned skip unused alignment bytes.
	    // Do not count these bytes when returning total number of 
	    // bytes read.
	    if( _imageAlignmentFactor > 0 )
	    {
		alignmentBytes = sampleBytesRead % _imageAlignmentFactor;
		if( alignmentBytes > 0)
		    checkResult( _stream->Seek( alignmentBytes ) );
	    }

	    // Update total number of samples read.
	    (*pTotalSamplesRead)++;
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



/*
    Seek

    Seek to a particular sample frame on the media.

    Input:
	sampleFrame	- zero-based number of frame to seek to.

    Return:
    AAFRESULT_SUCCESS		- success.
    AAFRESULT_BADFRAMEOFFSET	- invalid frame number.
    AAFRESULT_BADSAMPLEOFFSET	- invalid offset in the essnce stream.
				    This error is returned by stream->Seek().
    AAFRESULT_NOT_INITIALIZED	- returned by stream->Seek().
    AAFRESULT_UNEXPECTED_EXCEPTION
*/
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::Seek (aafPosition_t  sampleFrame)
{
    HRESULT	hr = S_OK;
    aafUInt32	alignmentBytes = _imageAlignmentFactor > 0 ? 
		(_fileBytesPerSample % _imageAlignmentFactor) : 0;


    try
    {
	checkAssertion(NULL != _stream);
	checkExpression( sampleFrame <= _numberOfSamples, 
	    AAFRESULT_BADFRAMEOFFSET );
	checkResult(_stream->Seek( (_fileBytesPerSample + alignmentBytes) * 
	    sampleFrame) );
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



// Helper utility to make sure the current information in the codec is 
// synchronized with the given descriptor information . 
// Called in Open and Create methods.
HRESULT STDMETHODCALLTYPE CAAFCDCICodec::ReadDescriptor( 
    CAAFCDCIDescriptorHelper&	descriptorHelper )
{
    HRESULT		hr = S_OK;
    IAAFContainerDef	*pContainerDef = NULL;
    IAAFDefObject	*pDefObj = NULL;


    try
    {
	//
	// FileDescriptor methods:
	//

	// Length is optional property. If it's not present 
	// set it to 0.
	hr = descriptorHelper.GetLength( &_length );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _length = 0;

	_numberOfSamples = (aafUInt32)_length;

	// SampleRate is optional property. If it's not present 
	// set it to 0.
	hr = descriptorHelper.GetSampleRate( &_sampleRate );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _sampleRate = NULL_RATIONAL;

	// ContainerFormat
	// pContainerDef and pDefObj released below...
	checkResult(descriptorHelper.GetContainerFormat(&pContainerDef));
	checkResult(pContainerDef->QueryInterface(IID_IAAFDefObject, 
	    (void **)&pDefObj));
	checkResult(pDefObj->GetAUID(&_containerFormat));


	//
	// DigitalImageDescriptor methods
	//

	// Compression is optional property. If it's not present
	// set it to default value. 
	hr = descriptorHelper.GetCompression( &_compression );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _compression = NULL_UID;

	// Stored view
	checkResult( descriptorHelper.GetStoredView(
	    &_storedHeight, &_storedWidth ) );
	_imageHeight = _storedHeight;
	_imageWidth = _storedWidth;

	// Sampled view is optional property. If it's not present 
	// set sampled view to be the same as stored view.
	hr = descriptorHelper.GetSampledView(
	    &_sampledHeight, &_sampledWidth,
	    &_sampledXOffset, &_sampledYOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );

	if( AAFRESULT_PROP_NOT_PRESENT == hr )
	{
	    _sampledWidth = _storedWidth;
	    _sampledHeight = _storedHeight;
	    _sampledXOffset = 0;
	    _sampledYOffset = 0;
	}

	// Display view is optional property. If it's not present 
	// set display view to be the same as stored view.
	hr = descriptorHelper.GetDisplayView( 
	    &_displayHeight, &_displayWidth, 
	    &_displayXOffset, &_displayYOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
	{
	    _displayWidth = _storedWidth;
	    _displayHeight = _storedHeight;
	    _displayXOffset = 0;
	    _displayYOffset = 0;
	}

	// FrameLayout (required).
	checkResult(descriptorHelper.GetFrameLayout( &_frameLayout ) );

	// VideoLineMap (required).
	// Video line map contains either 1 or 2 values depending
	// on frame layout. 
	checkResult( descriptorHelper.GetVideoLineMapSize( 
	    &_videoLineMapSize ) );
	checkResult( descriptorHelper.GetVideoLineMap(
	    _videoLineMapSize, _videoLineMap ) );

	// ImageAspectRatio (required).
	checkResult(descriptorHelper.GetImageAspectRatio(
	    &_imageAspectRatio));

	// AlphaTransparency (optional).
	hr = descriptorHelper.GetAlphaTransparency( &_alphaTransparency );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _alphaTransparency = kAAFMinValueTransparent;

	// Gamma (optional).
	hr = descriptorHelper.GetGamma( &_gamma );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _gamma = NULL_UID;

	// ImageAlignmentFactor (optional).
	hr = descriptorHelper.GetImageAlignmentFactor( 
	    &_imageAlignmentFactor );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _imageAlignmentFactor = 0;

	// FieldDominance (optional)
	// Although FieldDominance is optional property I would expect it 
	// to be there for 2 fields layout. But if it's not there, let 
	// user decide what to do.
	hr = descriptorHelper.GetFieldDominance( &_fieldDominance );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _fieldDominance = kAAFNoDominant;

	// FieldStartOffset (optional)
	hr = descriptorHelper.GetFieldStartOffset( &_fieldStartOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _fieldStartOffset = 0;

	// Get FieldEndOffset (optional)
	hr = descriptorHelper.GetFieldEndOffset( &_fieldEndOffset );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _fieldEndOffset = 0;


 	//
	// CDCIDescriptor
	//

	// ComponentWidth (required)
	checkResult( descriptorHelper.GetComponentWidth(&_componentWidth));
	// Check for supported values.
	checkExpression( _componentWidth == 8 || 
			 _componentWidth == 10 ||
			 _componentWidth == 16, AAFRESULT_BADPIXFORM );

	// HorizontalSubsampling (required)
	checkResult(descriptorHelper.GetHorizontalSubsampling(
	    &_horizontalSubsampling ) );
	checkExpression( _horizontalSubsampling == 1 || 
			 _horizontalSubsampling == 2, 
			 AAFRESULT_BADPIXFORM );

	// VerticalSubsampling (optional)
	// Currently we don't handle VerticalSubsampling other 
	// than 1.
	hr = descriptorHelper.GetVerticalSubsampling(
		    &_verticalSubsampling );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	    _verticalSubsampling = 1;
	checkExpression( _verticalSubsampling == 1, AAFRESULT_BADPIXFORM );



	// ColorSiting (optional)
	// Do not check for supported values because ColorSiting does not
	// participate in codec's internal calculations. 
	// Let user handle it.
	hr = descriptorHelper.GetColorSiting(&_colorSiting);
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_colorSiting = kAAFCoSiting;

	// BlackReferenceLevel (optional)
	hr = descriptorHelper.GetBlackReferenceLevel( 
	    &_blackReferenceLevel );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_blackReferenceLevel = 0;

	// WhiteReferenceLevel (optional)
	hr = descriptorHelper.GetWhiteReferenceLevel( 
	    &_whiteReferenceLevel );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr);
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
	    _whiteReferenceLevel = (1U << _componentWidth) - 1;

	// ColorRange (optional)
	hr = descriptorHelper.GetColorRange(&_colorRange);
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_colorRange = (1U << _componentWidth) - 1;

	// PaddingBits (optional)
	hr = descriptorHelper.GetPaddingBits( &_paddingBits );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if (AAFRESULT_PROP_NOT_PRESENT == hr)
		_paddingBits = 0;


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
    if( pContainerDef != NULL )
	pContainerDef->Release();
    if( pDefObj != NULL )
	pDefObj->Release();


    return hr;
}


void CAAFCDCICodec::UpdateDescriptor (CAAFCDCIDescriptorHelper& descriptorHelper)
{
	// Update FileDescriptor properties
	checkResult(descriptorHelper.SetLength(_numberOfSamples));
	checkResult(descriptorHelper.SetSampleRate(_sampleRate));

	// Update DigitalImageDescriptor properties
	checkResult(descriptorHelper.SetCompression(_compression));
	checkResult(descriptorHelper.SetStoredView(_storedHeight, _storedWidth));
	checkResult(descriptorHelper.SetSampledView(_sampledHeight, _sampledWidth, _sampledXOffset, _sampledYOffset));
	checkResult(descriptorHelper.SetDisplayView(_displayHeight, _displayWidth, _displayXOffset, _displayYOffset));
	checkResult(descriptorHelper.SetFrameLayout(_frameLayout));
	checkResult(descriptorHelper.SetVideoLineMap(_videoLineMapSize, _videoLineMap));
	checkResult(descriptorHelper.SetImageAspectRatio(_imageAspectRatio));
	checkResult(descriptorHelper.SetAlphaTransparency(_alphaTransparency));
	checkResult(descriptorHelper.SetGamma(_gamma));
	checkResult(descriptorHelper.SetImageAlignmentFactor(_imageAlignmentFactor));
	checkResult(descriptorHelper.SetFieldDominance(_fieldDominance));
	checkResult(descriptorHelper.SetFieldStartOffset(_fieldStartOffset));
	checkResult(descriptorHelper.SetFieldEndOffset(_fieldEndOffset));

	// CDCIDescriptor methods:
	checkResult(descriptorHelper.SetComponentWidth(_componentWidth));
	checkResult(descriptorHelper.SetHorizontalSubsampling(_horizontalSubsampling));
	checkResult(descriptorHelper.SetColorSiting(_colorSiting));
	checkResult(descriptorHelper.SetBlackReferenceLevel(_blackReferenceLevel));
	checkResult(descriptorHelper.SetWhiteReferenceLevel(_whiteReferenceLevel));
	checkResult(descriptorHelper.SetColorRange(_colorRange));
	checkResult(descriptorHelper.SetPaddingBits(_paddingBits));

	// If working with legacy CDCI set some specific descriptor properties.
	if( memcmp(&_compression,&AAF_CMPR_AUNC422,sizeof(_compression))==0  &&
	    _fieldStartOffset == 0  &&  _fieldEndOffset == 4 )
	{
	    checkResult( descriptorHelper.SetResolutionID( 0x97 ) );
	    checkResult( descriptorHelper.SetFrameSampleSize( 
		_fileBytesPerSample ) );
	}

}


// Routine to keep calculated member data up-to-date.
void CAAFCDCICodec::UpdateCalculatedData( void )
{
    aafUInt32 numFields = 0;


    // Number of fields per frame
    if( _frameLayout == kAAFSeparateFields ||
	_frameLayout == kAAFMixedFields )
	numFields = 2;
    else
	numFields = 1;


    // Computer the file bytes per sample.
    _fileBytesPerSample = 0;
    _bitsPerPixelAvg = 0;
    _bitsPerSample = 0;


    //
    // Calculate number of bits per pixel. 
    // Be aware that for 4:2:2 format number of bits per pi
    // 4:4:4 = 1 sample for each of luma and two chroma channels
    if (_horizontalSubsampling == 1)
    {
	_bitsPerPixelAvg = (aafInt16)((_componentWidth * 3) + _paddingBits);

	// Number of bits per sample = num bits per field * num of fields.
	// Number of bits per field  = num bits per image pixels + 
	//	    num bits per client fill (field start/end offsets)..
	// Number of bits per image pixels = num bits per line * num of 
	//	    lines (height).
	// Number of bits per line = num of bits per pixel * width + num 
	//	    of padding bits per line.
	//
	// Note that number of padding bits per line is not number of padding
	//	    bits per pixel.
	_bitsPerSample = ( (_imageWidth * _bitsPerPixelAvg + 
				_padBytesPerRow * 8) * _imageHeight  + 
				(_fieldStartOffset + _fieldEndOffset) * 8 ) *
				numFields;
    }
    // 4:2:2 = two-pixels get 2 samples of luma, 1 of each chroma channel, 
    // avg == 2
    //
    // Note: Supported component sizes named by the spec are 8, 10 and 16 bits.
    //		With these values YCrYCb will always be on a byte boundary. 
    else if (_horizontalSubsampling == 2)
    {
	_bitsPerPixelAvg = (aafInt16)((_componentWidth * 2) + _paddingBits);

	// See comments for 4:4:4
	_bitsPerSample = ( (_imageWidth/2  * (_componentWidth*4 + _paddingBits)+
				_padBytesPerRow * 8) * _imageHeight  + 
				(_fieldStartOffset + _fieldEndOffset) * 8 ) *
				numFields;
    }

    checkAssertion( _bitsPerPixelAvg % 8  ==  0 );


    _fileBytesPerSample = (_bitsPerSample + 7) / 8;
}


/*
    CompleteWrite

    Keeps essence descriptor information up to date.

    Input:
    fileMob	- file source mob we want to keep in sync 
		    with the original one. This parameter is optional.

    Return:
    AAFRESULT_ASSERTION_VIOLATION
    AAFRESULT_UNEXPECTED_EXCEPTION
*/
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::CompleteWrite (IAAFSourceMob *fileMob = NULL)
{
    HRESULT hr = S_OK;


    try
    {
	// Make sure the descriptor information is up-to-date.
	UpdateDescriptor(_descriptorHelper); // throw HRESULT


	// Handle the source mob we want to keep synchronized.
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
	    aafUInt8		expData[kMaxEssenceFormatData];
	    aafUInt32		expUInt32;
	    aafInt32		expInt32;
	    aafUInt16		expUInt16;
	    aafInt16		expInt16;
	    aafUInt8		expUInt8;
	    aafInt8		expInt8;
	    aafRational_t	expRational;
	    aafBoolean_t	expBoolean;
	    aafRect_t		expRect;
	    aafColorSpace_t	expColorSpace;
	    aafCompArray_t	expCompArray;
	    aafCompSizeArray_t	expCompSizeArray;
	    aafFrameLayout_t	expFrameLayout;
	    aafVideoLineMap_t	expVideoLineMap;
	    aafColorSiting_t	expColorSiting;
	    aafFieldDom_t	expFieldDominance;
	    aafAlphaTransparency_t expAlphaTransparency;
	    aafUID_t		expUID;
	} operand;
} aafEssenceFormatData_t;



#define ADD_FORMAT_SPECIFIER( f, id, p ) \
		(f->AddFormatSpecifier( \
			id, \
			sizeof(p), \
			reinterpret_cast<aafDataBuffer_t>(&(p))))


//  kAAFPixelSize is calculated value and cannot be set. 
//	Set kAAFCDCIPadBits, kAAFCDCICompWidth and kAAFCDCIHorizSubsampling
//	to change kAAFPixelSize.
// 
HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
    HRESULT			hr = S_OK;
    aafInt32			numSpecifiers, i;
    aafEssenceFormatData_t	param;


    if (NULL == pFormat)
	    return AAFRESULT_NULL_PARAM;


    try
    {
	checkResult(pFormat->NumFormatSpecifiers (&numSpecifiers));



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
		else if (EqualAUID(&kAAFPixelFormat, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expColorSpace), AAFRESULT_INVALID_PARM_SIZE);

			// Currently we only support the following pixel transformations.
			checkExpression(kAAFColorSpaceRGB == param.operand.expColorSpace ||
					      kAAFColorSpaceYUV == param.operand.expColorSpace,
					      AAFRESULT_BADPIXFORM);

			_pixelFormat = param.operand.expColorSpace;
		}
		else if (EqualAUID(&kAAFFrameLayout, &param.opcode))
		{
			// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expFrameLayout), AAFRESULT_INVALID_PARM_SIZE);


			// @@@
			// What do we do if the codec is asked to change the frame layout
			// after samples have already been written to the stream? This
			// should be a error in essence access...
			checkAssertion( param.operand.expFrameLayout == _frameLayout || _numberOfSamples == 0 );


			// Validate the frame layout:
			checkExpression( 
			    param.operand.expFrameLayout == kAAFFullFrame ||
			    param.operand.expFrameLayout == kAAFOneField ||
			    param.operand.expFrameLayout == kAAFSeparateFields ||
			    param.operand.expFrameLayout == kAAFMixedFields,
			    AAFRESULT_BADEXPORTLAYOUT );


			_frameLayout = param.operand.expFrameLayout;
		}
		else if( EqualAUID( &kAAFFieldDominance, &param.opcode ) )
		{
			// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expFieldDominance), AAFRESULT_INVALID_PARM_SIZE);

			// 1 or 2 are valid values
			checkExpression( param.operand.expFieldDominance == 1 || 
					    param.operand.expFieldDominance == 2,
					    AAFRESULT_ILLEGAL_VALUE );

			_fieldDominance = param.operand.expFieldDominance;
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
		else if( EqualAUID( &kAAFAlphaTransparency, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expAlphaTransparency), AAFRESULT_INVALID_PARM_SIZE);

		    // kAAFMinValueTransparent or kAAFMaxValueTransparent are valid values
		    checkExpression( param.operand.expAlphaTransparency == kAAFMinValueTransparent || 
					param.operand.expAlphaTransparency == kAAFMaxValueTransparent,
					AAFRESULT_ILLEGAL_VALUE );

		    _alphaTransparency = param.operand.expAlphaTransparency;
		}
		else if( EqualAUID( &kAAFGamma, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUID), AAFRESULT_INVALID_PARM_SIZE);

		    _gamma = param.operand.expUID;
		}
		else if( EqualAUID( &kAAFImageAlignmentFactor, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _imageAlignmentFactor = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFVideoLineMap, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expVideoLineMap), AAFRESULT_INVALID_PARM_SIZE);
			_videoLineMap[0] = param.operand.expVideoLineMap[0];
			_videoLineMap[1] = param.operand.expVideoLineMap[1];
		}
		else if (EqualAUID(&kAAFCDCICompWidth, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

			// 8, 10 or 16 are valid values
			checkExpression( param.operand.expUInt32 == 8 || 
					 param.operand.expUInt32 == 10 ||
					 param.operand.expUInt32 == 16, AAFRESULT_ILLEGAL_VALUE );

			_componentWidth = param.operand.expUInt32;
		}
		else if( EqualAUID( &kAAFCDCIHorizSubsampling, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    // 1 or 2 are valid values
		    checkExpression( param.operand.expUInt32 == 1 || param.operand.expUInt32 == 2,
					AAFRESULT_BADPIXFORM );

		    _horizontalSubsampling = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFCDCIColorSiting, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expColorSiting), AAFRESULT_INVALID_PARM_SIZE);
			
			// 0, 1 or 2 are valid values
			checkExpression( param.operand.expColorSiting == 0 || 
					 param.operand.expColorSiting == 1 ||
					 param.operand.expColorSiting == 2, AAFRESULT_ILLEGAL_VALUE );

			// We currently only support 8 bit components...
			_colorSiting = param.operand.expColorSiting;
		}
		else if (EqualAUID(&kAAFCDCIBlackLevel, &param.opcode))
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _blackReferenceLevel = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFCDCIWhiteLevel, &param.opcode))
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _whiteReferenceLevel = param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFCDCIColorRange, &param.opcode))
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _colorRange = param.operand.expUInt32;
		}
		else if( EqualAUID( &kAAFCDCIPadBits, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expInt16), AAFRESULT_INVALID_PARM_SIZE);

		    _paddingBits = param.operand.expInt16;
		}
		else if (EqualAUID(&kAAFPadBytesPerRow, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expUInt16), AAFRESULT_INVALID_PARM_SIZE);

			_padBytesPerRow = param.operand.expUInt16;
		}
		else if( EqualAUID( &kAAFFieldStartOffset, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _fieldStartOffset = param.operand.expUInt32;
		}
		else if( EqualAUID( &kAAFFieldEndOffset, &param.opcode ) )
		{
		    // Validate the in-memory size.
		    checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

		    _fieldEndOffset = param.operand.expUInt32;
		}
		else if (EqualAUID( &kAAFSampleRate, &param.opcode ) )
		{
			// Validate the in-memory size.
			checkExpression( param.size == sizeof(param.operand.expRational), AAFRESULT_INVALID_PARM_SIZE);
			_sampleRate = param.operand.expRational;
		}
		else if (EqualAUID( &kAAFLegacyCDCI, &param.opcode ) )
		{
			memcpy( &_compression, &AAF_CMPR_AUNC422, 
			    sizeof(_compression) );
			_fieldStartOffset = 0;
			_fieldEndOffset = 4;
		}
		// Below are parameters which are accessible for client to read 
		// but not to modify. Some of them are constant for this codec
		// (like kAAFNumChannels), some are calculated values like 
		// kAAFMaxSampleBytes.
		// This implementation ignores them.
		else if( EqualAUID( &kAAFNumChannels, &param.opcode ) ||
			 EqualAUID( &kAAFPixelSize, &param.opcode ) ||
			 EqualAUID( &kAAFMaxSampleBytes, &param.opcode ) ||
			 EqualAUID( &kAAFWillTransferLines, &param.opcode ) ||
			 EqualAUID( &kAAFIsCompressed, &param.opcode ) )
		{
		    // Do nothing
		}
		else
		    throw HRESULT( AAFRESULT_ILLEGAL_FILEFMT );

		
	} // for (i = 0...)

	
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



HRESULT STDMETHODCALLTYPE CAAFCDCICodec::GetEssenceFormat(
	IAAFEssenceFormat	*pTemplate,
	IAAFEssenceFormat	**pResult)
{
    HRESULT			hr = S_OK;
    IAAFEssenceFormat		*p_fmt = NULL;
    aafInt32			numSpecifiers;
    aafInt32			i;
    aafEssenceFormatData_t	param;


    if( NULL == pTemplate || NULL == pResult )
	return AAFRESULT_NULL_PARAM;

    // Initialize out parameter in case of failure.
    *pResult = NULL;

    try
    {
	// Start with an "empty" file format specifier.
	checkResult( _access->GetEmptyFileFormat( &p_fmt ) );
	
	checkResult( pTemplate->NumFormatSpecifiers( &numSpecifiers ) );

	// Use the data in the given template to create and initialize
	// a corresponding data from the codec.
	for( i=0; i<numSpecifiers; i++ )
	{
	    memset( &param, 0, sizeof(param) );
	    checkResult( pTemplate->GetIndexedFormatSpecifier( 
		i, &param.opcode, sizeof(param.operand.expData), 
		param.operand.expData, &param.size ) );


	    if( EqualAUID( &kAAFCompression, &param.opcode) )
	    {
		// Write out current compression ID
		memcpy( &(param.operand.expUID), &_compression, 
		    sizeof(param.operand.expUID) );
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCompression, param.operand.expUID ) );
	    }
	    else if( EqualAUID( &kAAFPixelFormat, &param.opcode ) )
	    {
		// Write out the current pixel format.
		param.operand.expColorSpace = _pixelFormat;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelFormat, param.operand.expColorSpace ) );
	    }
	    else if( EqualAUID( &kAAFFrameLayout, &param.opcode ) )
	    {
		// Write out the current framelayout.
		param.operand.expFrameLayout = _frameLayout;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFrameLayout, param.operand.expFrameLayout ) );
	    }
	    else if( EqualAUID( &kAAFFieldDominance, &param.opcode ) )
	    {
		// Write out the current framelayout.
		param.operand.expFieldDominance = _fieldDominance;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldDominance, param.operand.expFieldDominance ));
	    }
	    else if( EqualAUID( &kAAFStoredRect, &param.opcode ) )
	    {
		// Write out the current image dimentions for 
		// the default stored rectangle.
		param.operand.expRect.xOffset = 0;
		param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _storedWidth;
		param.operand.expRect.ySize = _storedHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFStoredRect, param.operand.expRect ));
	    }
	    else if( EqualAUID( &kAAFSampledRect, &param.opcode ) )
	    {
		// Write out the current sampled rectangle.
		param.operand.expRect.xOffset = _sampledXOffset;
		param.operand.expRect.yOffset = _sampledYOffset;
		param.operand.expRect.xSize = _sampledWidth;
		param.operand.expRect.ySize = _sampledHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampledRect, param.operand.expRect ));
	    }
	    else if( EqualAUID( &kAAFDisplayRect, &param.opcode ) )
	    {
		// Write out the current displayed rectangle.
		param.operand.expRect.xOffset = _displayXOffset;
		param.operand.expRect.yOffset = _displayYOffset;
		param.operand.expRect.xSize = _displayWidth;
		param.operand.expRect.ySize = _displayHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFDisplayRect, param.operand.expRect ) );
	    }
	    else if( EqualAUID( &kAAFPixelSize, &param.opcode ) )
	    {
		/// Write out the current bitsPerPixel 
		// (make sure that this has been computed!
		checkAssertion( _bitsPerPixelAvg != 0 );
		param.operand.expInt16 = _bitsPerPixelAvg;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelSize, param.operand.expInt16 ) );
	    }
	    else if( EqualAUID( &kAAFAspectRatio, &param.opcode ) )
	    {
		// Write out the current aspect ratio.
		param.operand.expRational = _imageAspectRatio;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAspectRatio, param.operand.expRational ) );
	    }
	    else if( EqualAUID( &kAAFAlphaTransparency, &param.opcode ) )
	    {
		// Write out the current alpha transparency.
		param.operand.expAlphaTransparency = _alphaTransparency;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAlphaTransparency, 
		    param.operand.expAlphaTransparency ) );
	    }
	    else if( EqualAUID( &kAAFGamma, &param.opcode ) )
	    {
		// Write out the current gamma value.
		param.operand.expUID = _gamma;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFGamma, param.operand.expUID ) );
	    }
	    else if( EqualAUID( &kAAFImageAlignmentFactor, &param.opcode ) )
	    {
		// Write out the current alignment.
		param.operand.expUInt32 = _imageAlignmentFactor;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFImageAlignmentFactor, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFVideoLineMap, &param.opcode ) )
	    {
		// Write out both values from the video line map. 
		// The frameLayout will determine which entries are valid.
		param.operand.expVideoLineMap[0] = _videoLineMap[0];
		param.operand.expVideoLineMap[1] = _videoLineMap[1];
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFVideoLineMap, param.operand.expVideoLineMap ) );
	    }
	    else if( EqualAUID( &kAAFWillTransferLines, &param.opcode ) )
	    {
		// CDCI codec does not transfer separate lines.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFWillTransferLines, param.operand.expBoolean ) );
	    }
	    else if( EqualAUID( &kAAFIsCompressed, &param.opcode ) )
	    {
		// CDCI codec does not handle compressed data.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFIsCompressed, param.operand.expBoolean ) );
	    }
	    else if( EqualAUID( &kAAFCDCICompWidth, &param.opcode ) )
	    {
		param.operand.expUInt32 = _componentWidth;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCICompWidth, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFCDCIHorizSubsampling, &param.opcode ) )
	    {
		param.operand.expUInt32 = _horizontalSubsampling;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIHorizSubsampling, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFCDCIColorSiting, &param.opcode ) )
	    {
		// Write out the current color siting value.
		param.operand.expColorSiting = _colorSiting;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorSiting, param.operand.expColorSiting ) );
	    }
	    else if( EqualAUID( &kAAFCDCIBlackLevel, &param.opcode ) )
	    {
		// Write out the current black reference level.
		param.operand.expUInt32 = _blackReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIBlackLevel, param.operand.expUInt32));
	    }
	    else if( EqualAUID( &kAAFCDCIWhiteLevel, &param.opcode ) )
	    {
		// Write out the current white reference level.
		param.operand.expUInt32 = _whiteReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIWhiteLevel, param.operand.expUInt32));
	    }
	    else if( EqualAUID( &kAAFCDCIColorRange, &param.opcode ) )
	    {
		// Write out the current color range.
		param.operand.expUInt32 = _colorRange;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorRange, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFCDCIPadBits, &param.opcode ) )
	    {
		// Write out the current padding bits value.
		param.operand.expInt16 = _paddingBits;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIPadBits, param.operand.expInt16 ) );
	    }
	    else if( EqualAUID( &kAAFFieldStartOffset, &param.opcode ) )
	    {
		param.operand.expUInt32 = _fieldStartOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldStartOffset, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFFieldEndOffset, &param.opcode ) )
	    {
		param.operand.expUInt32 = _fieldEndOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldEndOffset, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFPadBytesPerRow, &param.opcode ) )
	    {
		// Write out the current pad bytes per row.
		param.operand.expUInt16 = _padBytesPerRow;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPadBytesPerRow, param.operand.expUInt16 ) );
	    }
	    else if( EqualAUID( &kAAFMaxSampleBytes, &param.opcode ) )
	    {
		// Write out the sample size.
		param.operand.expUInt32 = _fileBytesPerSample;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFMaxSampleBytes, param.operand.expUInt32 ) );
	    }
	    else if( EqualAUID( &kAAFSampleRate, &param.opcode ) )
	    {
		// Write out the current sample rate value.
		param.operand.expRational = _sampleRate;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampleRate, param.operand.expRational ) );
	    }
	    else if( EqualAUID( &kAAFNumChannels, &param.opcode ) )
	    {
		/// We only support a single channel...
		param.operand.expUInt32 = kDefaultNumCh;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFNumChannels, param.operand.expUInt32 ) );
	    }
	    else
	    {	
		// The given opcode was not handled!
		throw HRESULT(AAFRESULT_INVALID_OP_CODEC);
	    }


	}  // for(i)


	// If the format specifier has been completely initialized
	// then return this value to the caller (clear local variable
	// so that we don't perform an extra release).
	*pResult = p_fmt;
	p_fmt = NULL;
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
    if (NULL != p_fmt) // should delete the object on failure
	p_fmt->Release();



    return hr;

}

HRESULT STDMETHODCALLTYPE
    CAAFCDCICodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
	HRESULT			hr = S_OK;
	IAAFEssenceFormat	*p_fmt = NULL;
	aafEssenceFormatData_t	param;

	
	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;


	// Initialize out parameter in case of failure.
	*pResult = NULL;



	try
	{
		// Start with an "empty" file format specifier.
		checkResult( _access->GetEmptyFileFormat( &p_fmt ) );
		

		// Write out current compression ID
		memcpy( &(param.operand.expUID), &_compression, 
		    sizeof(param.operand.expUID) );
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCompression, param.operand.expUID ) );

		// Write out the current pixel format.
		param.operand.expColorSpace = _pixelFormat;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelFormat, param.operand.expColorSpace ) );

		// Write out the current framelayout.
		param.operand.expFrameLayout = _frameLayout;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFrameLayout, param.operand.expFrameLayout ) );

		// Write out the current framelayout.
		param.operand.expFieldDominance = _fieldDominance;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldDominance, param.operand.expFieldDominance ));

		// Write out the current image dimentions for 
		// the default stored rectangle.
		param.operand.expRect.xOffset = 0;
		param.operand.expRect.yOffset = 0;
		param.operand.expRect.xSize = _storedWidth;
		param.operand.expRect.ySize = _storedHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFStoredRect, param.operand.expRect ));

		// Write out the current sampled rectangle.
		param.operand.expRect.xOffset = _sampledXOffset;
		param.operand.expRect.yOffset = _sampledYOffset;
		param.operand.expRect.xSize = _sampledWidth;
		param.operand.expRect.ySize = _sampledHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampledRect, param.operand.expRect ));

		// Write out the current displayed rectangle.
		param.operand.expRect.xOffset = _displayXOffset;
		param.operand.expRect.yOffset = _displayYOffset;
		param.operand.expRect.xSize = _displayWidth;
		param.operand.expRect.ySize = _displayHeight;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFDisplayRect, param.operand.expRect ) );

		/// Write out the current bitsPerPixel 
		// (make sure that this has been computed!
		checkAssertion( _bitsPerPixelAvg != 0 );
		param.operand.expInt16 = _bitsPerPixelAvg;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelSize, param.operand.expInt16 ) );

		// Write out the current aspect ratio.
		param.operand.expRational = _imageAspectRatio;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAspectRatio, param.operand.expRational ) );

		// Write out the current alpha transparency.
		param.operand.expAlphaTransparency = _alphaTransparency;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFAlphaTransparency, 
		    param.operand.expAlphaTransparency ) );

		// Write out the current gamma value.
		param.operand.expUID = _gamma;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFGamma, param.operand.expUID ) );

		// Write out the current alignment.
		param.operand.expUInt32 = _imageAlignmentFactor;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFImageAlignmentFactor, param.operand.expUInt32 ) );

		// Write out both values from the video line map. 
		// The frameLayout will determine which entries are valid.
		param.operand.expVideoLineMap[0] = _videoLineMap[0];
		param.operand.expVideoLineMap[1] = _videoLineMap[1];
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFVideoLineMap, param.operand.expVideoLineMap ) );

		// CDCI codec does not transfer separate lines.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFWillTransferLines, param.operand.expBoolean ) );

		// CDCI codec does not handle compressed data.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFIsCompressed, param.operand.expBoolean ) );

		param.operand.expUInt32 = _componentWidth;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCICompWidth, param.operand.expUInt32 ) );

		param.operand.expUInt32 = _horizontalSubsampling;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIHorizSubsampling, param.operand.expUInt32 ) );

		// Write out the current color siting value.
		param.operand.expColorSiting = _colorSiting;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorSiting, param.operand.expColorSiting ) );

		// Write out the current black reference level.
		param.operand.expUInt32 = _blackReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIBlackLevel, param.operand.expUInt32));

		// Write out the current white reference level.
		param.operand.expUInt32 = _whiteReferenceLevel;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIWhiteLevel, param.operand.expUInt32));

		// Write out the current color range.
		param.operand.expUInt32 = _colorRange;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIColorRange, param.operand.expUInt32 ) );

		// Write out the current padding bits value.
		param.operand.expInt16 = _paddingBits;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCDCIPadBits, param.operand.expInt16 ) );

		param.operand.expUInt32 = _fieldStartOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldStartOffset, param.operand.expUInt32 ) );

		param.operand.expUInt32 = _fieldEndOffset;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFFieldEndOffset, param.operand.expUInt32 ) );

		// Write out the current pad bytes per row.
		param.operand.expUInt16 = _padBytesPerRow;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPadBytesPerRow, param.operand.expUInt16 ) );

		// Write out the sample size.
		param.operand.expUInt32 = _fileBytesPerSample;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFMaxSampleBytes, param.operand.expUInt32 ) );

		// Write out the current sample rate value.
		param.operand.expRational = _sampleRate;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFSampleRate, param.operand.expRational ) );

		/// We only support a single channel...
		param.operand.expUInt32 = kDefaultNumCh;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFNumChannels, param.operand.expUInt32 ) );

		// If the format specifier has been completely initialized
		// then return this value to the caller (clear local variable
		// so that we don't perform an extra release).
		*pResult = p_fmt;
		p_fmt = NULL;
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
	if (NULL != p_fmt) // should delete the object on failure
		p_fmt->Release();


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

HRESULT STDMETHODCALLTYPE CAAFCDCICodec::GetIndexedSampleSize (
    aafUID_constref	dataDefID,
    aafPosition_t	pos,
    aafLength_t		*pResult)
{
	if(pos < 0 || pos > _numberOfSamples) // zero based sample index.
		return AAFRESULT_EOF;


	// All samlples are the same... 
	return GetLargestSampleSize( dataDefID, pResult );
}



HRESULT STDMETHODCALLTYPE CAAFCDCICodec::GetLargestSampleSize(
    aafUID_constref	dataDefID,
    aafLength_t		*pResult )
{
	HRESULT hr = AAFRESULT_SUCCESS;


	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = 0;

	try
	{
		if(EqualAUID(&dataDefID, &DDEF_Picture))
		{ 
			// The samples will be uncompressed so return the 
			// previously computed value for a single 
			// uncompressed sample.
			*pResult = _fileBytesPerSample;
		}
		else
			hr = AAFRESULT_CODEC_CHANNELS;
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


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
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IAAFEssenceCodec interface 
    if( aafIsEqualIID( riid, IID_IAAFEssenceCodec ) )
    { 
        *ppvObj = (IAAFEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    // and the IAAFPlugin interface.
    else if( aafIsEqualIID( riid, IID_IAAFPlugin ) )
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
