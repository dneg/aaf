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
#include "CAAFDNxHDCodec.h"

#include <stdarg.h>		// for varargs
#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFCompressionDefs.h"
#include "AAFContainerDefs.h"
#include "AAFTypeDefUIDs.h"

#include "utf8.h"

#include "AAFSmartPointer.h"

// declare a handy typedef for smart pointer
#define TYPEDEF_SP( Object ) \
	typedef IAAFSmartPointer<IAAF##Object> IAAF##Object##SP

// smartpointer typedefs for common objects
TYPEDEF_SP( EssenceData );
TYPEDEF_SP( EssenceData2 );
TYPEDEF_SP( PlainEssenceData );
TYPEDEF_SP( KLVEssenceDataParameters );
TYPEDEF_SP( EssenceDataStream2 );
TYPEDEF_SP( Mob );
TYPEDEF_SP( SourceMob );
TYPEDEF_SP( MobSlot );


// {1B08D578-88CD-45f6-B0C6-967F5BAB28F0}
const CLSID CLSID_AAFDNxHDCodec = 
{ 0x1b08d578, 0x88cd, 0x45f6, { 0xb0, 0xc6, 0x96, 0x7f, 0x5b, 0xab, 0x28, 0xf0 } };

// This plugin currently only supports a single definition
const aafUInt32 kSupportedDefinitions = 1;

typedef struct {
	aafUID_t flavour;
	const aafCharacter *name;
} FlavourInfo;

static FlavourInfo kSupportedFlavours[13];
const aafUInt32 kNumSupportedFlavours = sizeof(kSupportedFlavours) / sizeof(FlavourInfo);

const wchar_t kDisplayName[] = L"AAF DNxHD Codec";
const wchar_t kDescription[] = L"Handles uncompressed CbYCrY and (compressed) DNxHD streams";

const aafProductVersion_t kAAFPluginVersion = {1, 0, 0, 0, kAAFVersionReleased};
const aafRational_t		kDefaultRate = { 30000, 1001 };
const aafUInt32			kDefaultNumCh = 1;
const aafInt32			kDefaultComponentWidth = 8;

//
// Plugin Descriptor information
//
// {EE7C48B5-FDF0-4773-9B16-2F7A7AA122A2}
const aafUID_t AVID_DNxHD_PLUGIN = 
{ 0xee7c48b5, 0xfdf0, 0x4773, { 0x9b, 0x16, 0x2f, 0x7a, 0x7a, 0xa1, 0x22, 0xa2 } };

static const wchar_t *kManufURL = L"http://www.avid.com";
static const wchar_t *kDownloadURL = L"ftp://ftp.avid.com/pub/";
static const wchar_t *kManufName = L"Avid Technology, Inc.";
static const wchar_t *kManufRev = L"Rev 0.1";

// Should be shared by all built-in plugins created by AVID. /* TRR */
const aafUID_t MANUF_AVID_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


const aafUID_t NULL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
const aafRational_t NULL_RATIONAL = {0, 0};
const aafRational_t DEFAULT_ASPECT_RATIO = {4, 3};

// some well-known EssenceElementKeys
const aafUID_t GC_EEK = { 0x0d010301, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01 } };

// the Avid legacy EEK for DNxHD
const aafUID_t Avid_EEK = { 0x0e040301, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01 } };


// Debugging log function which is optimised away for default builds
inline void plugin_trace(const char *fmt, ...)
{
#ifdef PLUGIN_TRACE
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

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

// Legacy property names
const aafCharacter kAAFPropName_DIDResolutionID[] = { 'R','e','s','o','l','u','t','i','o','n','I','D','\0' };
const aafCharacter kAAFPropName_DIDFrameSampleSize[] = { 'F','r','a','m','e','S','a','m','p','l','e','S','i','z','e','\0' };
const aafCharacter kAAFPropName_DIDFirstFrameOffset[]	= { 'F','i','r','s','t','F','r','a','m','e','O','f','f','s','e','t','\0' };
const aafCharacter kAAFPropName_DIDImageSize[]	= { 'I','m','a','g','e','S','i','z','e','\0' };

// Constructor

CAAFDNxHDCodec::CAAFDNxHDCodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{
	plugin_trace("CAAFDNxHDCodec::CAAFDNxHDCodec() constructor\n");

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
	_pixelFormat = Layout_BGRAI;
	_fieldDominance = kAAFNoDominant;
	_fieldStartOffset = 0;
	_fieldEndOffset = 0;
	_bitsPerPixelAvg = 0;

	_numberOfSamples = 0;
	_padBytesPerRow = 0;

	_HeaderFilled=0;
	_ComprID=0xFFFFFFFF; //definitely an invalid ID

	_UseLegacyAvidUIDs=FALSE;
	_nCodecThreads=2; //a reasonable default

	_phys = 1;			// a sensible default for all single essence objects
						// multi-essence needs to set up unique values for each slot

	_eek = NULL_UID;	// the codec will initialize this via SetEEK() when PutEssenceFormat() is first called
						// future: EssenceAccess may call SetEEK() directly

	_TotalBytesWritten=0;


#ifdef USE_DNxHD_CODEC

	// for access to the Avid DNxHD Codec SDK
	// please visit http://www.avid.com/DNxHD
	// or contact DNxHDSupport@avid.com

	_pAdvDNxCodec=NULL;
	_CodecInitialized=false;
	_DNxHD_buffer=NULL;

#endif

	// Ugly workaround for broken MSVC initializer support
	kSupportedFlavours[0].flavour = kAAFNilCodecFlavour;
	kSupportedFlavours[1].flavour = kAAFCodecFlavour_VC3_1235;
	kSupportedFlavours[2].flavour = kAAFCodecFlavour_VC3_1237;
	kSupportedFlavours[3].flavour = kAAFCodecFlavour_VC3_1238;
	kSupportedFlavours[4].flavour = kAAFCodecFlavour_VC3_1241;
	kSupportedFlavours[5].flavour = kAAFCodecFlavour_VC3_1242;
	kSupportedFlavours[6].flavour = kAAFCodecFlavour_VC3_1243;
	kSupportedFlavours[7].flavour = kAAFCodecFlavour_VC3_1250;
	kSupportedFlavours[8].flavour = kAAFCodecFlavour_VC3_1251;
	kSupportedFlavours[9].flavour = kAAFCodecFlavour_VC3_1252;
	kSupportedFlavours[10].flavour = kAAFCodecFlavour_VC3_1253;
	kSupportedFlavours[11].flavour = kAAFCodecFlavour_VC3_1244;
	kSupportedFlavours[12].flavour = kAAFCodecFlavour_VC3_1254;
	kSupportedFlavours[0].name = L"AAF DNxHD Codec (no flavour)";
	kSupportedFlavours[1].name = L"AAF DNxHD Codec ID 1235";
	kSupportedFlavours[2].name = L"AAF DNxHD Codec ID 1237";
	kSupportedFlavours[3].name = L"AAF DNxHD Codec ID 1238";
	kSupportedFlavours[4].name = L"AAF DNxHD Codec ID 1241";
	kSupportedFlavours[5].name = L"AAF DNxHD Codec ID 1242";
	kSupportedFlavours[6].name = L"AAF DNxHD Codec ID 1243";
	kSupportedFlavours[7].name = L"AAF DNxHD Codec ID 1250";
	kSupportedFlavours[8].name = L"AAF DNxHD Codec ID 1251";
	kSupportedFlavours[9].name = L"AAF DNxHD Codec ID 1252";
	kSupportedFlavours[10].name = L"AAF DNxHD Codec ID 1253";
	kSupportedFlavours[11].name = L"AAF DNxHD Codec ID 1244";
	kSupportedFlavours[12].name = L"AAF DNxHD Codec ID 1254";
}


// Desctructor

CAAFDNxHDCodec::~CAAFDNxHDCodec ()
{
	plugin_trace("CAAFDNxHDCodec::~CAAFDNxHDCodec()\n");
  // Do NOT release the _access interface since this object
  // contains the reference to this codec instance! We need
  // avoid the dreaded reference counting cycle of death!
	if(_stream != NULL)
	{
		_stream->Release();
		_stream = NULL;
	}
#ifdef USE_DNxHD_CODEC
	// variables for DNxHD coder interface
	if(_DNxHD_buffer)
		delete[] _DNxHD_buffer;

	if(_pAdvDNxCodec)
	{
		DeleteAvidOptimizedDNxHDCodec(_pAdvDNxCodec);
		_pAdvDNxCodec=NULL;
		FinalAvidOptimizedDNxHDCleanUp();
	}
	
#endif
}


// Only save a new reference.
void CAAFDNxHDCodec::SetEssenceStream(IAAFEssenceStream *stream)
{
	plugin_trace("CAAFDNxHDCodec::SetEssenceStream()\n");
	if (_stream != stream)
	{
		if (NULL != _stream)
			_stream->Release();
		_stream = stream;
		if (NULL != stream)
			stream->AddRef();
	}
}

void CAAFDNxHDCodec::SetCompressionEnabled(aafCompressEnable_t compEnable)
{
	_compressEnable = (compEnable == kAAFCompressionEnable) ? kAAFCompressionEnable : kAAFCompressionDisable;
}

void CAAFDNxHDCodec::SetNumberOfSamples(const aafLength_t& numberOfSamples)
{
	_numberOfSamples = numberOfSamples;
}




HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::CountDefinitions (aafUInt32 *pDefCount)
{
	plugin_trace("CAAFDNxHDCodec::CountDefinitions()\n");
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetIndexedDefinitionID (aafUInt32 /* index */, aafUID_t *uid)
{
	plugin_trace("CAAFDNxHDCodec::GetIndexedDefinitionID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = kAAFCodecDNxHD;		// UID of the DNxHD codec definition

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetPluginDescriptorID (aafUID_t *uid)
{
	plugin_trace("CAAFDNxHDCodec::GetPluginDescriptorID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AVID_DNxHD_PLUGIN;		// UID of the PluginDescriptor

	return AAFRESULT_SUCCESS;
}


// The method creates some descriptor properties to handle legacy DNxHD 
// variations .
HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::CreateLegacyPropDefs( 
    IAAFDictionary	*p_dict )
{
	plugin_trace("CAAFDNxHDCodec::CreateLegacyPropDefs()\n");
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
		if ( p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFrameSampleSize, &p_propdef) != AAFRESULT_SUCCESS)
		{
			checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
				kAAFPropID_DIDFrameSampleSize, kAAFPropName_DIDFrameSampleSize, 
				p_typedef_int32, &p_propdef ) );
		}
		p_propdef->Release();
		p_propdef = NULL;

		if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDFirstFrameOffset, &p_propdef) != AAFRESULT_SUCCESS)
		{
			checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
				kAAFPropID_DIDFirstFrameOffset, kAAFPropName_DIDFirstFrameOffset, 
				p_typedef_int32, &p_propdef ) );
		}
		p_propdef->Release();
		p_propdef = NULL;

		if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDImageSize, &p_propdef) != AAFRESULT_SUCCESS)
		{
			checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
				kAAFPropID_DIDImageSize, kAAFPropName_DIDImageSize, 
				p_typedef_int32, &p_propdef ) );
		}
		p_propdef->Release();
		p_propdef = NULL;

		if (p_did_classdef->LookupPropertyDef(kAAFPropID_DIDResolutionID, &p_propdef) != AAFRESULT_SUCCESS)
		{
			checkResult( p_did_classdef->RegisterOptionalPropertyDef( 
				kAAFPropID_DIDResolutionID, kAAFPropName_DIDResolutionID, 
				p_typedef_int32, &p_propdef ) );
		}
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
    CAAFDNxHDCodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	plugin_trace("CAAFDNxHDCodec::GetIndexedDefinitionObject()\n");
	HRESULT hr = S_OK;
	IAAFCodecDef	*codecDef = NULL;
	IAAFDataDef		*pDefPicture = NULL, *pDefLegacyPicture = NULL;
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

		// Creates descriptor properties to handle legacy DNxHD.
		checkResult( CreateLegacyPropDefs( dict ) );

		// Create the Codec Definition:
		checkResult(dict->LookupClassDef(AUID_AAFCodecDef, &pcd));
		checkResult(pcd->CreateInstance(IID_IAAFCodecDef, 
										(IUnknown **)&codecDef));
		pcd->Release();
		pcd = 0;
		
		// Support "Picture" type of data definition.
		checkResult(dict->LookupDataDef(kAAFDataDef_Picture, &pDefPicture));
		checkResult(codecDef->AddEssenceKind(pDefPicture));
		checkResult(dict->LookupDataDef(kAAFDataDef_LegacyPicture, &pDefLegacyPicture));
		checkResult(codecDef->AddEssenceKind(pDefLegacyPicture));
		
		// Initialize the standard Definition properties.
		checkResult(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		uid = kAAFCodecDNxHD;
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
	if (NULL != pDefLegacyPicture)
	  {
		pDefLegacyPicture->Release();
		pDefLegacyPicture = 0;
	  }
	if (NULL != pDefPicture)
	  {
		pDefPicture->Release();
		pDefPicture = 0;
	  }
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
    CAAFDNxHDCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
	plugin_trace("CAAFDNxHDCodec::CreateDescriptor()\n");
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

		checkResult(desc->Initialize(AVID_DNxHD_PLUGIN,
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
    CAAFDNxHDCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	plugin_trace("CAAFDNxHDCodec::SetEssenceAccess()\n");
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
    CAAFDNxHDCodec::CountFlavours(aafUInt32 *pCount)
{
	plugin_trace("CAAFDNxHDCodec::CountFlavours()\n");
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pCount = kNumSupportedFlavours;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetIndexedFlavourID (aafUInt32 index,
        aafUID_t *  pFlavour)
{
	plugin_trace("CAAFDNxHDCodec::GetIndexedFlavourID()\n");
	if (pFlavour == NULL)
		return AAFRESULT_NULL_PARAM;

	if (index >= kNumSupportedFlavours)
		return AAFRESULT_BADINDEX;

	*pFlavour = kSupportedFlavours[index].flavour;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	plugin_trace("CAAFDNxHDCodec::CountDataDefinitions()\n");
	if (NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetIndexedDataDefinition (aafUInt32  /* index */,
        aafUID_t * pFlavour)
{
	plugin_trace("CAAFDNxHDCodec::GetIndexedDataDefinition()\n");
	if (NULL == pFlavour)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}



HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetMaxCodecDisplayNameLength (
        aafUInt32  *bufSize)
{
	plugin_trace("CAAFDNxHDCodec::GetMaxCodecDisplayNameLength()\n");
	if (NULL == bufSize)
		return AAFRESULT_NULL_PARAM;

	// Loop over all flavours recording the largest name's buffer size
	aafUInt32 max = 0;
	for (size_t i = 0; i < sizeof(kSupportedFlavours); i++)
	{
		FlavourInfo &fi = kSupportedFlavours[i];
		// compute length of name in bytes including terminating NUL
		aafUInt32 len = (wcsu8slen(fi.name) + 1) *
									sizeof(aafCharacter);
		if (len > max)
			max = len;
	}
	*bufSize = max;

	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetCodecDisplayName (aafUID_constref flavour,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
	plugin_trace("CAAFDNxHDCodec::GetCodecDisplayName()\n");
	if (NULL == pName)
		return AAFRESULT_NULL_PARAM;
	if (0 >= bufSize)
		return AAFRESULT_INVALID_PARAM;

	for (size_t i = 0; i < sizeof(kSupportedFlavours); i++)
	{
		FlavourInfo &fi = kSupportedFlavours[i];
		if (flavour == fi.flavour)
		{
			// compute length of name in bytes including terminating NUL
			aafUInt32 len = (wcsu8slen(fi.name) + 1) *
										sizeof(aafCharacter);
			if(len > bufSize)
				len = bufSize;
			memcpy(pName, fi.name, len);
			return AAFRESULT_SUCCESS;
		}
	}

	// Indicate flavour not implemented
	return AAFRESULT_NOT_IMPLEMENTED;
}


// SetFlavour is used as an alternative to PutEssenceFormat.
// All internal settings and descriptor properties are set using
// a flavour instead of requiring many format specifiers to be used.
HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::SetFlavour (aafUID_constref flavour)
{
	plugin_trace("CAAFDNxHDCodec::SetFlavour()\n");
	if (flavour == kAAFNilCodecFlavour)
		return AAFRESULT_SUCCESS;

	// set _ComprID
	     if (flavour == kAAFCodecFlavour_VC3_1235)  _ComprID=1235;
	else if (flavour == kAAFCodecFlavour_VC3_1237)  _ComprID=1237;
	else if (flavour == kAAFCodecFlavour_VC3_1238)  _ComprID=1238;
	else if (flavour == kAAFCodecFlavour_VC3_1241)  _ComprID=1241;
	else if (flavour == kAAFCodecFlavour_VC3_1242)  _ComprID=1242;
	else if (flavour == kAAFCodecFlavour_VC3_1243)  _ComprID=1243;
	else if (flavour == kAAFCodecFlavour_VC3_1244)  _ComprID=1244;
	else if (flavour == kAAFCodecFlavour_VC3_1250)  _ComprID=1250;
	else if (flavour == kAAFCodecFlavour_VC3_1251)  _ComprID=1251;
	else if (flavour == kAAFCodecFlavour_VC3_1252)  _ComprID=1252;
	else if (flavour == kAAFCodecFlavour_VC3_1253)  _ComprID=1253;
	else if (flavour == kAAFCodecFlavour_VC3_1254)  _ComprID=1254;
	else
		return AAFRESULT_NOT_IMPLEMENTED;

	// set _compression
	memcpy( &_compression, &kAAFCompressionDef_VC3_1, sizeof(_compression) );
	aafUInt16 byte14n15;
	switch(_ComprID)
	{
	case 1235: byte14n15 = 0x7101; break;
	case 1237: byte14n15 = 0x7103; break;
	case 1238: byte14n15 = 0x7104; break;
	case 1241: byte14n15 = 0x7107; break;
	case 1242: byte14n15 = 0x7108; break;
	case 1243: byte14n15 = 0x7109; break;
	case 1244: byte14n15 = 0x710a; break;
	case 1250: byte14n15 = 0x7110; break;
	case 1251: byte14n15 = 0x7111; break;
	case 1252: byte14n15 = 0x7112; break;
	case 1253: byte14n15 = 0x7113; break;
	case 1254: byte14n15 = 0x7114; break;
	default:
		return HRESULT(AAFRESULT_NOT_IMPLEMENTED);
		break;
	}
	_compression.Data2=byte14n15;

	_fileBytesPerSample=GetBytesPerSample();
	_componentWidth=Is10Bit()?10:8;
	_frameLayout=IsInterlaced()?kAAFSeparateFields:kAAFFullFrame;
	_imageHeight = _storedHeight = GetLinesPerField();
	_imageWidth = _storedWidth = GetPixelsPerLine();
	_horizontalSubsampling = 2;

	SetCodecState();

	return AAFRESULT_SUCCESS;
}

// IsCompressionSupported is used by the toolkit to test whether this plugin
// is capable of opening and reading essence for the given compression ID.
HRESULT STDMETHODCALLTYPE
	CAAFDNxHDCodec::IsCompressionSupported(
		aafUID_constref compression,
		aafBool* pIsSupported)
{
	plugin_trace("CAAFDNxHDCodec::IsCompressionSupported()\n");

	if (NULL == pIsSupported)
		return AAFRESULT_NULL_PARAM;

	if (IsDNxHD(compression))
		*pIsSupported = kAAFTrue;
	else
		*pIsSupported = kAAFFalse;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::CountChannels (IAAFSourceMob *fileMob,
        aafUID_constref essenceKind,
        IAAFEssenceStream *stream,
        aafUInt16 *  pNumChannels)
{
	plugin_trace("CAAFDNxHDCodec::CountChannels()\n");
	if (NULL == fileMob || NULL == stream || NULL == pNumChannels)
		return AAFRESULT_NULL_PARAM;

	if(EqualAUID(&essenceKind, &kAAFDataDef_Picture)
		|| EqualAUID(&essenceKind, &kAAFDataDef_LegacyPicture))
	{
		*pNumChannels = 1;
	}
	else
		*pNumChannels = 0;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetSelectInfo (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
        aafSelectInfo_t *  pSelectInfo)
{
	plugin_trace("CAAFDNxHDCodec::GetSelectInfo()\n");
	HRESULT hr = S_OK;
	aafUInt32		storedWidth, storedHeight;
	aafInt16		padBits;
	aafUInt32		compWidth, bitsPerPixel = 0;
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
		if (AAFRESULT_SUCCESS == hr && IsDNxHD(compressionID) && 0 == padBits)
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
    CAAFDNxHDCodec::CountSamples (
        aafUID_constref essenceKind,
        aafLength_t *  pNumSamples)
{
	plugin_trace("CAAFDNxHDCodec::CountSamples()\n");
	if (NULL == pNumSamples)
		return AAFRESULT_NULL_PARAM;

	if(EqualAUID(&essenceKind, &kAAFDataDef_Picture)
		|| EqualAUID(&essenceKind, &kAAFDataDef_LegacyPicture))
	{
		*pNumSamples = _numberOfSamples;
	}
	else
		*pNumSamples = 0;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::ValidateEssence (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
		aafCheckVerbose_t  /* verbose */,
        aafCheckWarnings_t /* warning */,
         aafUInt32  /* bufSize */,
		wchar_t *  pName,
        aafUInt32  *bytesWritten)
{
	plugin_trace("CAAFDNxHDCodec::ValidateEssence()\n");
	if (NULL == fileMob || NULL == stream ||
		  NULL == pName   || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
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
CAAFDNxHDCodec::Create (IAAFSourceMob *p_srcmob,
  aafUID_constref flavour,
  aafUID_constref essenceKind,
  aafRational_constref sampleRate,
  IAAFEssenceStream * stream,
  aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFDNxHDCodec::Create()\n");

    HRESULT	    hr = S_OK;

    
    // Check input arguments.
    if( NULL == p_srcmob || NULL == stream )
	return AAFRESULT_NULL_PARAM;

    if( EqualAUID( &kAAFNilCodecFlavour, &flavour) != kAAFTrue )
	return AAFRESULT_NULL_PARAM;


    try
    {
	    // Check essence kind
		checkExpression( kAAFTrue == EqualAUID(&essenceKind, &kAAFDataDef_Picture)
			|| kAAFTrue == EqualAUID(&essenceKind, &kAAFDataDef_LegacyPicture),
			AAFRESULT_INVALID_DATADEF );

		// Sets whether or not to compress samples as they are written
		SetCompressionEnabled(compEnable);

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
    CAAFDNxHDCodec::Open (IAAFSourceMob *p_srcmob,
		aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFDNxHDCodec::Open()\n");
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

		// Sets whether or not to decompress the samples as they are read.
	    SetCompressionEnabled(compEnable);

		// Initialize codec parameters reading descriptor information.
		hr = ReadDescriptor( _descriptorHelper );
		checkExpression (hr == S_OK, hr);

		// if _fileBytesPerSample was not provided by (optional) Descriptor::FrameSampleSize
		// try to obtain it by decoding Compression and ContainerFormat
		if( !_fileBytesPerSample )
		{
			if( _ComprID==0 || _ComprID==0xFFFFFFFF )
				_ComprID = GetComprID( _compression, _containerFormat );

			_fileBytesPerSample = GetBytesPerSample();
		}

		if (_compressEnable == kAAFCompressionEnable && IsDNxHD(_compression))
		{
#ifdef USE_DNxHD_CODEC
#else
			// Can't decompress without DNxHD Codec
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}
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



HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::WriteSamples(
	aafUInt32	nSamples,
        aafUInt32	buflen,
        aafDataBuffer_t	buffer,
        aafUInt32	*pTotalSamplesWritten,
        aafUInt32	*pTotalBytesWritten)
{
	plugin_trace("CAAFDNxHDCodec::WriteSamples()\n");
    HRESULT	hr = S_OK;
    aafUInt32	n;
    aafUInt32	sampleBytesWritten;	// Number of sample bytes written.
    aafUInt32	sampleSize;		// Number of bytes per sample.
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



	sampleSize = _fileBytesPerSample;

	// Make sure the given buffer is really large enough for the complete
	// pixel data.
	//checkExpression( sampleSize*nSamples <= buflen, AAFRESULT_SMALLBUF );



	// Write samples one by one aligning them if necessary.
	for( n=0; n<nSamples; n++ )
	{
		if (_compressEnable == kAAFCompressionEnable && IsDNxHD(_compression))
		{
#ifdef USE_DNxHD_CODEC

			aafUInt8 * comprBuffer;
			comprBuffer=new aafUInt8[MAX_DNxHD_COMPRESSED_FRAME];

			DNX_compression_ID_t FormatNumber;
			switch(_ComprID)
			{
			case 1235: FormatNumber=DNX_220X_1080p_COMPRESSION_ID;  break;
			case 1237: FormatNumber=DNX_145_1080p_COMPRESSION_ID;  break;
			case 1238: FormatNumber=DNX_220_1080p_COMPRESSION_ID;  break;
			case 1241: FormatNumber=DNX_220X_1080i_COMPRESSION_ID;  break;
			case 1242: FormatNumber=DNX_145_1080i_COMPRESSION_ID;  break;
			case 1243: FormatNumber=DNX_220_1080i_COMPRESSION_ID;  break;
			case 1244: FormatNumber=DNX_145_1440_1080i_COMPRESSION_ID;  break;
			case 1250: FormatNumber=DNX_220X_720p_COMPRESSION_ID;  break;
			case 1251: FormatNumber=DNX_220_720p_COMPRESSION_ID; break;
			case 1252: FormatNumber=DNX_145_720p_COMPRESSION_ID; break;
			case 1253: FormatNumber=DNX_36_1080p_COMPRESSION_ID;  break;
			//case 1254: FormatNumber=DNX_50_1080i_COMPRESSION_ID;  break;
			default:
				FormatNumber=DNX_36_1080p_COMPRESSION_ID;  //make any guess
				break;
			}

			if(!_CodecInitialized)
			{
				_CodecInitialized=true;
				// variables for DNxHD coder interface
				_DNxHD_buffer=NULL;
				long ret=InitializeAvidOptimizedDNxHDCodec();
				if(ret==0)
					_pAdvDNxCodec=CreateAvidOptimizedDNxHDCodec();
				else
					return AAFRESULT_CODEC_INVALID;
				if(!_pAdvDNxCodec)
					throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
			}
			

				UncompInfo_t CompressInfo;
			
				switch( _pixelFormat )
				{
					// This example supports only P_UYVY, UYVY, BGRAI, BGRA
					// DNxHD Codec V1.0 supports also ARGB, ARGBI, YUYV, P_YUYV

				default:
				case Layout_P_UYVY:
 					CompressInfo.m_chromaSubSampling			= CSS_4220;
					CompressInfo.m_colorComponentOrder			= CCO_CbYCrY_NoA;
					CompressInfo.m_componentType				= CT_10Bit_2_8;
					CompressInfo.m_colorSpace					= CS_YCbCr_709;
					CompressInfo.m_bIsAlphaPresent				= kACFFalse;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				case Layout_UYVY:
 					CompressInfo.m_chromaSubSampling			= CSS_4220;
					CompressInfo.m_colorComponentOrder			= CCO_CbYCrY_NoA;
					CompressInfo.m_componentType				= CT_UCHAR;
					CompressInfo.m_colorSpace					= CS_YCbCr_709;
					CompressInfo.m_bIsAlphaPresent				= kACFFalse;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				case Layout_BGRAI:
 					CompressInfo.m_chromaSubSampling			= CSS_4444;
					CompressInfo.m_colorComponentOrder			= CCO_BGRA_Interleaved;
					CompressInfo.m_componentType				= CT_SHORT;
					CompressInfo.m_colorSpace					= CS_RGB;
					CompressInfo.m_bIsAlphaPresent				= kACFBool_Max;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				case Layout_BGRA:
 					CompressInfo.m_chromaSubSampling			= CSS_4444;
					CompressInfo.m_colorComponentOrder			= CCO_BGRA_Interleaved;
					CompressInfo.m_componentType				= CT_UCHAR;
					CompressInfo.m_colorSpace					= CS_RGB;
					CompressInfo.m_bIsAlphaPresent				= kACFBool_Max;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				}

				if( 1244 == _ComprID )
				{
					CompressInfo.m_signalStandard				= SS_274M_I;
					CompressInfo.m_bufferFieldOrder				= BFO_Merged_F1_First;
					CompressInfo.m_rasterGeometryType			= RGT_NativeCompressed;
				}
				else if( IsInterlaced() )
				{
					CompressInfo.m_signalStandard				= SS_274M_I;
					CompressInfo.m_bufferFieldOrder				= BFO_Merged_F1_First;
					CompressInfo.m_rasterGeometryType			= RGT_SignalStandard;
				}
				else if( Is720p() )
				{
					CompressInfo.m_signalStandard				= SS_296M_P;
					CompressInfo.m_bufferFieldOrder				= BFO_Progressive;
					CompressInfo.m_rasterGeometryType			= RGT_NativeCompressed;
				}
				else // Progressive 1920x1080
				{
					CompressInfo.m_signalStandard				= SS_274M_P;
					CompressInfo.m_bufferFieldOrder				= BFO_Progressive;
					CompressInfo.m_rasterGeometryType			= RGT_SignalStandard;
				}

				CompressInfo.m_resolution					= RS_FULL;
  				CompressInfo.m_componentRangeType			= CRT_VideoRange;
				CompressInfo.m_bufferLineOrientation		= BLO_Top_Bot;
				CompressInfo.m_uncompressedBufferAlignment	= 16;

			int ret=_pAdvDNxCodec->SetCompressionID( FormatNumber );
			ret=_pAdvDNxCodec->SetInputOutputDataTypes( &CompressInfo,  CO_ENCODE);
			ret=_pAdvDNxCodec->SetRowBytesForEncode(0);//let codec calculate RowBytes
			_pAdvDNxCodec->SetNumThreadsForEncode(_nCodecThreads);

			if(ret)
				throw HRESULT( AAFRESULT_INVALID_OP_CODEC );

			int szSrcBuff=MAX_DNxHD_UNCOMPRESSED_FRAME;
			int szOutBuff=MAX_DNxHD_COMPRESSED_FRAME;
			acfUInt32 szFrameSize;

			acfUInt32 result=_pAdvDNxCodec->EncodeFrame(buffer,comprBuffer,szSrcBuff,szOutBuff,&szFrameSize);
			if(result)
				throw HRESULT( AAFRESULT_INVALID_OP_CODEC );

			sampleBytesWritten = 0;
			checkResult( _stream->Write( szFrameSize, comprBuffer, &sampleBytesWritten));

			delete[] comprBuffer;

#else
			// Can't compress without DNxHD codec
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}
		else
		{
			if(_HeaderFilled<DNxHDHEADERSIZE)
			{
				if(_HeaderFilled==0)
				{
					_TotalBytesWritten=0;
					if(buflen>=DNxHDHEADERSIZE)
					{
						memcpy(_Header,buffer,DNxHDHEADERSIZE);
						_HeaderFilled=DNxHDHEADERSIZE;

					}
					if(_HeaderFilled==DNxHDHEADERSIZE)
						UpdateDescriptor( _descriptorHelper );
				}
			}
	    	sampleBytesWritten = 0;
	    	checkResult( _stream->Write( buflen, buffer, 
				&sampleBytesWritten));
		}
	    buffer += sampleBytesWritten;
	    *pTotalBytesWritten += sampleBytesWritten;
		_TotalBytesWritten+= sampleBytesWritten;  //keep own record of number of bytes written


	    }

	    SetNumberOfSamples( _TotalBytesWritten/_fileBytesPerSample);

	    // Update total number of samples written.
	    (*pTotalSamplesWritten)=(aafUInt32)_numberOfSamples;

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
HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::ReadSamples(
	aafUInt32	nSamples,
	aafUInt32	buflen,
	aafDataBuffer_t	buffer,
	aafUInt32	*pTotalSamplesRead,
	aafUInt32	*pTotalBytesRead )
{
	plugin_trace("CAAFDNxHDCodec::ReadSamples()\n");
    HRESULT	hr = S_OK;
    aafUInt32	n;
    aafUInt32	sampleBytesRead;	// Number of sample bytes read.
    aafUInt32	sampleSize;		// Number of bytes per sample.


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

	plugin_trace("CAAFDNxHDCodec::ReadSamples() sampleSize=%d nSamples=%d sampleSize*nSamples=%d buflen=%d\n",
				sampleSize, nSamples, sampleSize*nSamples, buflen);


	// Read samples one by one skipping alignment data
	for( n = 0; n < nSamples; n++ )
	{
	    sampleBytesRead = 0;

		if (_compressEnable == kAAFCompressionEnable && IsDNxHD(_compression))
		{
#ifdef USE_DNxHD_CODEC
			if(!_HeaderFilled)
			{ //this must be the first round so get the header and the vital information
				_HeaderFilled=true;
				checkResult( _stream->Read( DNxHDHEADERSIZE, _Header, &sampleBytesRead ));

				_ComprID=(_Header[0x28]<<24)+(_Header[0x29]<<16)+(_Header[0x2A]<<8)+_Header[0x2B];

				 _ALPF=(_Header[0x18]<<8)+_Header[0x19];
				 _SPL=(_Header[0x1A]<<8)+_Header[0x1B];

				int LinesPerField=_ALPF;
				if(IsInterlaced())
					LinesPerField=_ALPF/2;

				 _SBD=_Header[0x21]>>5;

				 _stream->Seek(0); //return to top to get whole frame

				 if(_DNxHD_buffer==NULL)
					 _DNxHD_buffer=new aafUInt8[GetBytesPerSample()];


			}

			// Make sure the given buffer is really large enough for the complete
			// pixel data.
			checkExpression( GetUncomprFrameSize() <= buflen, AAFRESULT_SMALLBUF );

			checkExpression( _DNxHD_buffer != NULL, AAFRESULT_INTERNAL_ERROR );

			//get encoded frame
			checkResult( _stream->Read( GetBytesPerSample(), _DNxHD_buffer, &sampleBytesRead ));
			if(sampleBytesRead==GetBytesPerSample())
			{//got a full frame

				try{

				if(!_CodecInitialized)
				{
					_CodecInitialized=true;
					// variables for DNxHD coder interface
					long ret=InitializeAvidOptimizedDNxHDCodec();
					if(ret==0)
						_pAdvDNxCodec=CreateAvidOptimizedDNxHDCodec();
					else
						return AAFRESULT_CODEC_INVALID;

					if(_pAdvDNxCodec)
						throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
				}


				UncompInfo_t CompressInfo;

				switch( _pixelFormat )
				{
					// This example supports only P_UYVY, UYVY, BGRAI, BGRA
					// DNxHD Codec V1.0 supports also ARGB, ARGBI, YUYV, P_YUYV

				default:
				case Layout_P_UYVY:
 					CompressInfo.m_chromaSubSampling			= CSS_4220;
					CompressInfo.m_colorComponentOrder			= CCO_CbYCrY_NoA;
					CompressInfo.m_componentType				= CT_10Bit_2_8;
					CompressInfo.m_colorSpace					= CS_YCbCr_709;
					CompressInfo.m_bIsAlphaPresent				= kACFFalse;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				case Layout_UYVY:
 					CompressInfo.m_chromaSubSampling			= CSS_4220;
					CompressInfo.m_colorComponentOrder			= CCO_CbYCrY_NoA;
					CompressInfo.m_componentType				= CT_UCHAR;
					CompressInfo.m_colorSpace					= CS_YCbCr_709;
					CompressInfo.m_bIsAlphaPresent				= kACFFalse;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				case Layout_BGRAI:
 					CompressInfo.m_chromaSubSampling			= CSS_4444;
					CompressInfo.m_colorComponentOrder			= CCO_BGRA_Interleaved;
					CompressInfo.m_componentType				= CT_SHORT;
					CompressInfo.m_colorSpace					= CS_RGB;
					CompressInfo.m_bIsAlphaPresent				= kACFBool_Max;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				case Layout_BGRA:
 					CompressInfo.m_chromaSubSampling			= CSS_4444;
					CompressInfo.m_colorComponentOrder			= CCO_BGRA_Interleaved;
					CompressInfo.m_componentType				= CT_UCHAR;
					CompressInfo.m_colorSpace					= CS_RGB;
					CompressInfo.m_bIsAlphaPresent				= kACFBool_Max;
					CompressInfo.m_isAlpha0Transparent			= kACFBool_Max;
					break;
				}

				if( 1244 == _ComprID )
				{
					CompressInfo.m_signalStandard				= SS_274M_I;
					CompressInfo.m_bufferFieldOrder				= BFO_Merged_F1_First;
					CompressInfo.m_rasterGeometryType			= RGT_NativeCompressed;
				}
				else if( IsInterlaced() )
				{
					CompressInfo.m_signalStandard				= SS_274M_I;
					CompressInfo.m_bufferFieldOrder				= BFO_Merged_F1_First;
					CompressInfo.m_rasterGeometryType			= RGT_SignalStandard;
				}
				else if( Is720p() )
				{
					CompressInfo.m_signalStandard				= SS_296M_P;
					CompressInfo.m_bufferFieldOrder				= BFO_Progressive;
					CompressInfo.m_rasterGeometryType			= RGT_NativeCompressed;
				}
				else // Progressive 1920x1080
				{
					CompressInfo.m_signalStandard				= SS_274M_P;
					CompressInfo.m_bufferFieldOrder				= BFO_Progressive;
					CompressInfo.m_rasterGeometryType			= RGT_SignalStandard;
				}

				CompressInfo.m_resolution					= RS_FULL;
  				CompressInfo.m_componentRangeType			= CRT_VideoRange;
				CompressInfo.m_bufferLineOrientation		= BLO_Top_Bot;
				CompressInfo.m_uncompressedBufferAlignment	= 16;

				int ret=_pAdvDNxCodec->SetCompressionID( (DNX_compression_ID_t) _ComprID);
				ret=_pAdvDNxCodec->SetInputOutputDataTypes( &CompressInfo, CO_DECODE);
				ret=_pAdvDNxCodec->SetRowBytesForDecode(0);//let codec calculate RowBytes
				_pAdvDNxCodec->SetNumThreadsForDecode(2);

				if(ret)
					throw HRESULT( AAFRESULT_INVALID_OP_CODEC );

				int szSrcBuff=GetBytesPerSample();

				acfUInt32 result=_pAdvDNxCodec->DecodeFrame(_DNxHD_buffer,buffer,szSrcBuff,buflen);
				if(result)
					throw HRESULT( AAFRESULT_INVALID_OP_CODEC );

				buffer += GetUncomprFrameSize();
				*pTotalBytesRead += GetUncomprFrameSize();
				}
				catch(...)
				{
					fprintf( stderr, "\nUnexpected exception decoding DNx essence\n\n");
				}

			}
			else
			{
				*pTotalBytesRead=0;

			}
		

#else
			// Can't decompress without DNxHD Codec
			throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
		}
		else
		{
			// Make sure the given buffer is really large enough for the complete
			// pixel data.
			checkExpression( sampleSize*nSamples <= buflen, AAFRESULT_SMALLBUF );

	    	checkResult( _stream->Read( sampleSize, buffer, &sampleBytesRead ));
	    	buffer += sampleBytesRead;
	    	*pTotalBytesRead += sampleBytesRead;
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
    CAAFDNxHDCodec::Seek (aafPosition_t  sampleFrame)
{
	plugin_trace("CAAFDNxHDCodec::Seek()\n");
    HRESULT	hr = S_OK;

    try
    {
	checkAssertion(NULL != _stream);
	checkExpression( sampleFrame <= _numberOfSamples, 
	    AAFRESULT_BADFRAMEOFFSET );

	if (IsDNxHD(_compression))
	{
		checkResult(_stream->Seek( (_fileBytesPerSample ) * sampleFrame) );
	}
	else
	{
		checkResult(_stream->Seek( (_fileBytesPerSample ) * sampleFrame) );
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


// Helper utility to make sure the current information in the codec is 
// synchronized with the given descriptor information . 
// Called in Open and Create methods.
HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::ReadDescriptor( 
    CAAFCDCIDescriptorHelper&	descriptorHelper )
{
	plugin_trace("CAAFDNxHDCodec::ReadDescriptor()\n");
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


	hr = descriptorHelper.GetFrameSampleSize( &_fileBytesPerSample );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_NO_MORE_OBJECTS == hr ||
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT ||
		    hr == AAFRESULT_NO_MORE_OBJECTS )
	    _fileBytesPerSample = 0;


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

	if (IsDNxHD(_compression))
	{
		checkExpression( _horizontalSubsampling == 1 || 
				 _horizontalSubsampling == 2 ||
				 _horizontalSubsampling == 4, 
				 AAFRESULT_BADPIXFORM );
	}
	else
	{
		checkExpression( _horizontalSubsampling == 1 || 
				 _horizontalSubsampling == 2, 
				 AAFRESULT_BADPIXFORM );
	}

	// VerticalSubsampling (optional)
	hr = descriptorHelper.GetVerticalSubsampling(
		    &_verticalSubsampling );
	checkExpression( AAFRESULT_PROP_NOT_PRESENT == hr || 
			 AAFRESULT_SUCCESS == hr, hr );
	if( hr == AAFRESULT_PROP_NOT_PRESENT )
	{
	    _verticalSubsampling = 1;
	}

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
	hr = S_OK; //reset hr here
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


// Prepares Codec internals for WriteSamples()
// Called by PutEssenceFormat() and SetFlavour()
void CAAFDNxHDCodec::SetCodecState(void)
{
	// compression should be disabled for non-DNxHD data
	if (_compressEnable == kAAFCompressionEnable)
		checkExpression (IsDNxHD(_compression),AAFRESULT_BADCOMPR);

	// Allow only supported combinations of subsampling and height
	if (IsDNxHD(_compression))
	{
		_verticalSubsampling = 1;	



		// Check sensible verticalSubsampling values
		checkExpression( _verticalSubsampling == 1 || _verticalSubsampling == 2, AAFRESULT_BADPIXFORM );


	}
	else
	{
		// for all other  DNxHD video
		checkExpression( _verticalSubsampling == 1, AAFRESULT_BADPIXFORM );
	}

	if (_compressEnable == kAAFCompressionEnable && IsDNxHD(_compression))
	{
#ifdef USE_DNxHD_CODEC

		//FIXME initialize any variables needede for  compression
#else
		// Can't compress without DNxHD codec
		throw HRESULT( AAFRESULT_INVALID_OP_CODEC );
#endif
	}

	UpdateDescriptor( _descriptorHelper );
	UpdateCalculatedData();
}


void CAAFDNxHDCodec::UpdateDescriptor (CAAFCDCIDescriptorHelper& descriptorHelper)
{
	plugin_trace("CAAFDNxHDCodec::UpdateDescriptor()\n");
	// Update FileDescriptor properties

	UpdateCalculatedData();

	checkResult(descriptorHelper.SetLength(_numberOfSamples));
	checkResult(descriptorHelper.SetSampleRate(_sampleRate));

	// Update DigitalImageDescriptor properties

	if(_UseLegacyAvidUIDs)
	{
		memcpy(&_compression,&kAAFCompressionDef_Avid_DNxHD_Legacy, sizeof(aafUID_t));
	}
	else
	{
		memcpy(&_compression,&kAAFCompressionDef_VC3_1, sizeof(aafUID_t));
		aafUInt16 byte14n15;
		switch(_ComprID)
		{
		case 1235: byte14n15 = 0x7101; break;
		case 1237: byte14n15 = 0x7103; break;
		case 1238: byte14n15 = 0x7104; break;
		case 1241: byte14n15 = 0x7107; break;
		case 1242: byte14n15 = 0x7108; break;
		case 1243: byte14n15 = 0x7109; break;
		case 1244: byte14n15 = 0x710a; break;
		case 1250: byte14n15 = 0x7110; break;
		case 1251: byte14n15 = 0x7111; break;
		case 1252: byte14n15 = 0x7112; break;
		case 1253: byte14n15 = 0x7113; break;
		case 1254: byte14n15 = 0x7114; break;
		default:
			throw HRESULT(AAFRESULT_NOT_IMPLEMENTED);
			break;
		}
		_compression.Data2=byte14n15;
	}

	if(_UseLegacyAvidUIDs)
		checkResult( descriptorHelper.SetResolutionID( _ComprID ) );
	
	checkResult(descriptorHelper.SetContainerFormat(_containerFormat));

	checkResult(descriptorHelper.SetCompression(_compression));
	checkResult(descriptorHelper.SetStoredView(_storedHeight, _storedWidth));
	checkResult(descriptorHelper.SetSampledView(_sampledHeight, _sampledWidth, _sampledXOffset, _sampledYOffset));
	checkResult(descriptorHelper.SetDisplayView(_displayHeight, _displayWidth, _displayXOffset, _displayYOffset));
	checkResult(descriptorHelper.SetFrameLayout(_frameLayout));
	checkResult(descriptorHelper.SetVideoLineMap(_videoLineMapSize, _videoLineMap));
	checkResult(descriptorHelper.SetImageAspectRatio(_imageAspectRatio));
	checkResult(descriptorHelper.SetAlphaTransparency(_alphaTransparency));
	checkResult(descriptorHelper.SetImageAlignmentFactor(_imageAlignmentFactor));
	checkResult(descriptorHelper.SetFieldDominance(_fieldDominance));
	checkResult(descriptorHelper.SetFieldStartOffset(_fieldStartOffset));
	checkResult(descriptorHelper.SetFieldEndOffset(_fieldEndOffset));
	if (_gamma != NULL_UID)
		checkResult(descriptorHelper.SetGamma(_gamma));

	// CDCIDescriptor methods:
	checkResult(descriptorHelper.SetComponentWidth(_componentWidth));
	checkResult(descriptorHelper.SetHorizontalSubsampling(_horizontalSubsampling));
	checkResult(descriptorHelper.SetVerticalSubsampling(_verticalSubsampling));
	checkResult(descriptorHelper.SetColorSiting(_colorSiting));
	checkResult(descriptorHelper.SetBlackReferenceLevel(_blackReferenceLevel));
	checkResult(descriptorHelper.SetWhiteReferenceLevel(_whiteReferenceLevel));
	checkResult(descriptorHelper.SetColorRange(_colorRange));
	checkResult(descriptorHelper.SetPaddingBits(_paddingBits));

	checkResult( descriptorHelper.SetFrameSampleSize(_fileBytesPerSample ) );

	//checkResult( descriptorHelper.SetResolutionID( 1235 ) );
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
void CAAFDNxHDCodec::UpdateCalculatedData( void )
{
	int ALPF;
	int SPL;

	plugin_trace("CAAFDNxHDCodec::UpdateCalculatedData()\n");

	if(_HeaderFilled==0) 
	{
		switch(_ComprID)
		{
		case 1235:  
		case 1237: 
		case 1238: 
			ALPF=1080;
			SPL=1920;
			break;
		case 1241:  
		case 1242:  
		case 1243:  
			ALPF=1080;
			SPL=1920;
			break;
		case 1250:  
		case 1251:  
		case 1252:  
			ALPF=720;
			SPL=1280;
			break;
		case 1253:  
		case 1254:  
			ALPF=1080;
			SPL=1920;
			break;
		case 1244:  
			ALPF=1080;
			SPL=1440;
			break;
		default:
			//not got any information yet
			return ;
			break;
		}
	}
	else //get info from raw essence
	{

		_ComprID=(_Header[0x28]<<24)+(_Header[0x29]<<16)+(_Header[0x2A]<<8)+_Header[0x2B];

		ALPF=(_Header[0x18]<<8)+_Header[0x19];
		SPL=(_Header[0x1A]<<8)+_Header[0x1B];
	}
		

	int LinesPerField=ALPF;
	if(IsInterlaced())
		LinesPerField=ALPF/2;

	int SBD=_Header[0x21]>>5;


	_storedHeight=LinesPerField;
	_storedWidth=SPL;
	_sampledHeight=LinesPerField;
	_sampledWidth=SPL;
	_displayHeight=LinesPerField;
	_displayWidth=SPL;
	_sampledXOffset=0;
	_sampledYOffset=0;

	_horizontalSubsampling=2;
	_verticalSubsampling=1;
	_colorSiting=0;
	_blackReferenceLevel=16;
	_whiteReferenceLevel=235;
	_colorRange=225;

	if(SBD==2)
		_componentWidth=10;
	else
		_componentWidth=8;

	_imageAlignmentFactor=0;
	_imageAspectRatio.numerator=16;
	_imageAspectRatio.denominator=9;

	int F1 = 0;
	int F2 = 0;
	switch(LinesPerField)
	{
	case 1080 : F1=42; break;
	case 540: F1=21;F2=584; break;
	case 720: F1=26; break;
	default: break;
	}


	_videoLineMap[0]=F1;
	_videoLineMap[1]=F2;
	
	// set the EssenceElementKey
	IAAFEssenceDataStream2SP pES2;
	checkResult( _stream->QueryInterface(IID_IAAFEssenceDataStream2, (void **)&pES2) );
    HRESULT hr = S_OK;
	if( !_UseLegacyAvidUIDs )
	{
		// preset to known GC key
		_eek = GC_EEK;

		hr = pES2->SetEssenceElementKey( _eek, 0x15, 1, 0x0D, 1, 1 );
	}
	else
	{
		// preset to known Avid legacy key
		_eek = Avid_EEK;

		hr = pES2->SetEssenceElementKey( _eek, 0x15, 1, 0x0D, 1, 1 );
	}

	// if this is a KLV (MXF) file, set the ContainerID 
	if( hr == AAFRESULT_SUCCESS )
	{
		if( !_UseLegacyAvidUIDs )
		{
			_containerFormat = kAAFContainerDef_MXFGC_Clipwrapped_VC3;
		}
		else
		{
			switch(_ComprID)
			{
			case 1235: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_220X_1080p; break;
			case 1237: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_145_1080p; break;
			case 1238: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_220_1080p; break;
			case 1241: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_220X_1080i; break;
			case 1242: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_145_1080i; break;
			case 1243: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_220_1080i; break;
			case 1244: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i; break;
			case 1250: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_220X_720p; break;
			case 1251: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_220_720p; break;
			case 1252: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_145_720p; break;
			case 1253: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_36_1080p; break;
			// case 1254: _containerFormat = kAAFContainerDef_MXFGC_Avid_DNX_50_1080i; break;
			default:   _containerFormat = kAAFContainerDef_MXFGC_Clipwrapped_VC3; break;
			}
		}
	}
	else if( hr == AAFRESULT_OPERATION_NOT_PERMITTED )
	{
		// must be a file kind where eek isn't important
		// do not override preset container def
	}

    // Compute the file bytes per sample.
    _fileBytesPerSample = GetBytesPerSample();
    _bitsPerPixelAvg = (aafUInt16)_componentWidth;

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
    CAAFDNxHDCodec::CompleteWrite (IAAFSourceMob *fileMob = NULL)
{
	plugin_trace("CAAFDNxHDCodec::CompleteWrite()\n");
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
    CAAFDNxHDCodec::CreateDescriptorFromStream (IAAFEssenceStream * pStream,
        IAAFSourceMob *fileMob)
{
	plugin_trace("CAAFDNxHDCodec::CreateDescriptorFromStream()\n");
	if (NULL == pStream || NULL == fileMob)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
	plugin_trace("CAAFDNxHDCodec::GetCurrentEssenceStream()\n");
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
	    aafInt8			expInt8;
	    aafRational_t	expRational;
	    aafBoolean_t	expBoolean;
	    aafRect_t		expRect;
		BufferLayout_t	expBufferLayout;
	    // BufferLayout_t supercedes aafColorSpace_t	expColorSpace;
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
    CAAFDNxHDCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	plugin_trace("CAAFDNxHDCodec::PutEssenceFormat()\n");
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

		if (EqualAUID(&kAAFNumThreads, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

			_nCodecThreads=param.operand.expUInt32;
		}
		else if (EqualAUID(&kAAFLegacyAUIDs, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expUInt32), AAFRESULT_INVALID_PARM_SIZE);

			if( param.operand.expUInt32!=0 )
				_UseLegacyAvidUIDs=true;
			else
				_UseLegacyAvidUIDs=false;
		}
		else if (EqualAUID(&kAAFCompression, &param.opcode))
		{	// Validate the in-memory size.
			aafUID_t		nullCompID = NULL_UID;
			checkExpression(param.size == sizeof(param.operand.expUID), AAFRESULT_INVALID_PARM_SIZE);

			// NOTE: this does not alter _ComprID
			// use SetCodecFlavor() to achieve that

			// Only DNxHD compressions supported
			checkExpression (IsDNxHD(param.operand.expUID), AAFRESULT_BADCOMPR);

			memcpy( &_compression, &(param.operand.expUID), 
			    sizeof(param.operand.expUID) );
		}
		else if (EqualAUID(&kAAFBufferLayout, &param.opcode))
		{	// Validate the in-memory size.
			checkExpression(param.size == sizeof(param.operand.expBufferLayout), AAFRESULT_INVALID_PARM_SIZE);

			// Currently we only support the following Buffer Layouts:
			checkExpression(  Layout_P_UYVY == param.operand.expBufferLayout
				           || Layout_UYVY == param.operand.expBufferLayout
				           || Layout_BGRAI == param.operand.expBufferLayout
				           || Layout_BGRA == param.operand.expBufferLayout
						   ,  AAFRESULT_BADPIXFORM);

			_pixelFormat = param.operand.expBufferLayout;
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
			    param.operand.expFrameLayout == kAAFSeparateFields ,
			    AAFRESULT_BADEXPORTLAYOUT );


			_frameLayout = param.operand.expFrameLayout;
		}

		// TODO: the remainder of these don't actually affect DNxHD (which has preset values)
		// the code should be removed as part of the V1.1.x codec updates

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

		    // 1, 2 or 4 are valid values
		    checkExpression( param.operand.expUInt32 == 1 ||
					param.operand.expUInt32 == 2 ||
					param.operand.expUInt32 == 4, AAFRESULT_BADPIXFORM );

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

	SetCodecState();

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


HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::GetEssenceFormat(
	IAAFEssenceFormat	*pTemplate,
	IAAFEssenceFormat	**pResult)
{
	plugin_trace("CAAFDNxHDCodec::GetEssenceFormat()\n");
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

		if( EqualAUID( &kAAFNumThreads, &param.opcode) )
		{
			// Write out the LegacyUID status.
			param.operand.expUInt32 = _nCodecThreads;
			checkResult( ADD_FORMAT_SPECIFIER( 
				p_fmt, kAAFNumThreads, param.operand.expUInt32 ) );
		}
		else if( EqualAUID( &kAAFLegacyAUIDs, &param.opcode) )
		{
			// Write out the LegacyUID status.
			param.operand.expUInt32 = _UseLegacyAvidUIDs;
			checkResult( ADD_FORMAT_SPECIFIER( 
				p_fmt, kAAFLegacyAUIDs, param.operand.expUInt32 ) );
		}
		else if( EqualAUID( &kAAFCompression, &param.opcode) )
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
		param.operand.expBufferLayout = _pixelFormat;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelFormat, param.operand.expBufferLayout ) );
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
		// DNxHD codec does not transfer separate lines.
		param.operand.expBoolean = kAAFFalse;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFWillTransferLines, param.operand.expBoolean ) );
	    }
	    else if( EqualAUID( &kAAFIsCompressed, &param.opcode ) )
	    {
		// DNxHD codec does not handle compressed data.
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
    CAAFDNxHDCodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
	plugin_trace("CAAFDNxHDCodec::GetDefaultEssenceFormat()\n");
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


		// Write out the LegacyUID status.
		param.operand.expUInt32 = FALSE;
		checkResult( ADD_FORMAT_SPECIFIER( 
			p_fmt, kAAFLegacyAUIDs, param.operand.expUInt32 ) );
		

		// Write out current compression ID
		memcpy( &(param.operand.expUID), &_compression, 
		    sizeof(param.operand.expUID) );
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFCompression, param.operand.expUID ) );

		// Write out the current pixel format.
		param.operand.expBufferLayout = _pixelFormat;
		checkResult( ADD_FORMAT_SPECIFIER( 
		    p_fmt, kAAFPixelFormat, param.operand.expBufferLayout ) );

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

		if (IsDNxHD(_compression))
			param.operand.expBoolean = kAAFTrue;
		else
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
    CAAFDNxHDCodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	plugin_trace("CAAFDNxHDCodec::GetEssenceDescriptorID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFCDCIDescriptor; // stored class UID of Compressed Digital Image Decriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFDNxHDCodec::GetEssenceDataID (aafUID_t *uid)
{
	plugin_trace("CAAFDNxHDCodec::GetEssenceDataID()\n");
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AUID_AAFEssenceData;			// stored class UID of the DNxHD data object

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::GetIndexedSampleSize (
    aafUID_constref	dataDefID,
    aafPosition_t	pos,
    aafLength_t		*pResult)
{
	plugin_trace("CAAFDNxHDCodec::GetIndexedSampleSize()\n");
	if(pos < 0 || pos > _numberOfSamples) // zero based sample index.
		return AAFRESULT_EOF;


	// All samples are the same... 
	return GetLargestSampleSize( dataDefID, pResult );
}



HRESULT STDMETHODCALLTYPE CAAFDNxHDCodec::GetLargestSampleSize(
    aafUID_constref	dataDefID,
    aafLength_t		*pResult )
{
	plugin_trace("CAAFDNxHDCodec::GetLargestSampleSize()\n");
	HRESULT hr = AAFRESULT_SUCCESS;


	if (NULL == pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = 0;

	try
	{
		if(EqualAUID(&dataDefID, &kAAFDataDef_Picture)
			|| EqualAUID(&dataDefID, &kAAFDataDef_LegacyPicture))
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
HRESULT CAAFDNxHDCodec::InternalQueryInterface
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
    // and the IAAFEssenceCodec2 interface 
    else if( aafIsEqualIID( riid, IID_IAAFEssenceCodec2 ) )
    { 
        *ppvObj = (IAAFEssenceCodec2 *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    // and the IAAFEssenceCodec3 interface 
    else if( aafIsEqualIID( riid, IID_IAAFEssenceCodec3 ) )
    { 
        *ppvObj = (IAAFEssenceCodec3 *)this; 
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
AAF_DEFINE_FACTORY(AAFDNxHDCodec)
