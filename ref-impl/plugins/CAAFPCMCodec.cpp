//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=
#include "CAAFPCMCodec.h"

#include <assert.h>
#include <string.h>
#include <stdarg.h>		// for varargs

#include "AAFResult.h"

#include "AAF.h"

#include "aafErr.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFContainerDefs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"


const aafUID_t NULL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
const aafProductVersion_t AAFPluginImplementationVersion = {1, 0, 0, 1, kAAFVersionBeta};
const aafRational_t		defaultRate = { 44100, 1 };
const aafUInt32			defaultSampleWidth = 8;
const aafUInt32			defaultNumCh = 1;

const CLSID CLSID_AAFPCMCodec = { 0x98aa902d, 0x90ce, 0x402c, { 0xa3, 0x65, 0xf3, 0x75, 0x8c, 0x78, 0x99, 0x86 } };
const aafUID_t AAFSDK_PCM_PLUGIN = { 0xbd1e9f34, 0xf509, 0x4ab9, { 0xb5, 0x4e, 0xf8, 0xb7, 0xb8, 0x46, 0xdf, 0xa2 } };


static wchar_t *manufURL = L"http://www.aafassociation.org";
static wchar_t *downloadURL = L"http://aaf.sourceforge.net";
const aafUID_t MANUF_AAFSDK_PLUGINS = { 0x4ae7f1e0, 0x2a01, 0x4209, { 0xbd, 0xe8, 0xbc, 0x11, 0x93, 0xe2, 0xec, 0x12 } };

static wchar_t *manufName = L"BBC R&D";
static wchar_t *manufRev = L"Rev 0.0";



static void SplitBuffers(void *original, aafUInt32 srcSamples, aafUInt16 sampleSize, aafUInt16 numDest, pcm_interleaveBuf_t *destPtr);

inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw HRESULT(r);
}

// Debugging log function which is optimised away for default builds
static void plugin_trace(const char *fmt, ...)
{
#ifdef PLUGIN_TRACE
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

static aafUInt32 readUInt32_LE(aafUInt8 *p)
{
	return	( (p[3] << 24)
			| (p[2] << 16)
			| (p[1] << 8)
			| (p[0] << 0) );
}

static void storeUInt16_LE(aafUInt8 *p, aafUInt16 value)
{
	p[0] = (value & 0x00ff) >> 0;
	p[1] = (value & 0xff00) >> 8;
}

static void storeUInt32_LE(aafUInt8 *p, aafUInt32 value)
{
	p[0] = (aafUInt8)((value & 0x000000ff) >> 0);
	p[1] = (aafUInt8)((value & 0x0000ff00) >> 8);
	p[2] = (aafUInt8)((value & 0x00ff0000) >> 16);
	p[3] = (aafUInt8)((value & 0xff000000) >> 24);
}

HRESULT CAAFPCMCodec::write_updated_BWF_size(void)
{
	HRESULT res;
	aafInt64 currentPos;
	aafUInt32 bytesWritten;
	aafUInt32 datasize, riffsize;
	aafUInt8 buf[4];

	// TODO: To support ReadModify we would need to search for
	// the fmt and data chunks in order to update them.

	// FIXME: Seek to end-of-file or use some other way of recording total size
	res = _stream->GetPosition(&currentPos);

	// A BWF file is limited to 4GB (max UInt32)
	datasize = (aafUInt32)((currentPos - 646) & 0xffffffff);
	riffsize = (aafUInt32)((currentPos - 8) & 0xffffffff);

	storeUInt32_LE(buf, riffsize);
	res |= _stream->Seek(4);
	res |= _stream->Write(sizeof(buf), buf, &bytesWritten);

	storeUInt32_LE(buf, datasize);
	res |= _stream->Seek(12 + 24 + 602 + 4);
	res |= _stream->Write(sizeof(buf), buf, &bytesWritten);

	return res;
}

HRESULT CAAFPCMCodec::write_BWF_header(void)
{
	aafInt64 currentPos;
	aafUInt8 riff_fmt[12 + 24] = "RIFF....WAVEfmt ";
	aafUInt8 bext[602] = "bext";
	aafUInt8 data_hdr[8] = "data";

	aafUInt32 avgBPS = (_sampleRate.numerator / _sampleRate.denominator) * _bitsPerSample / 8;
	aafUInt32 rate = (_sampleRate.numerator / _sampleRate.denominator);

	// fmt chunk
	storeUInt16_LE(&riff_fmt[12+4], 16);				// size is always 16
	storeUInt16_LE(&riff_fmt[12+8], 0x0001);			// wFormatTag WAVE_FORMAT_PCM
	storeUInt16_LE(&riff_fmt[12+10], _numCh);			// nchannels
	storeUInt32_LE(&riff_fmt[12+12], rate);				// nSamplesPerSec
	storeUInt32_LE(&riff_fmt[12+16], avgBPS);			// nAvgBytesPerSec
	storeUInt16_LE(&riff_fmt[12+20], _bytesPerFrame);	// nBlockAlign
	storeUInt16_LE(&riff_fmt[12+22], _bitsPerSample);	// nBitsPerSample

	// bext chunk
	storeUInt32_LE(&bext[4], 602 - 8);					// bext chunk size

	// Originator Date, Time in the form YYYY-MM-DD, HH-MM-SS
	aafTimeStamp_t ts;
	AAFGetDateTime(&ts);
	sprintf((char *)&bext[256+32+32], "%04d-%02d-%02d%02d-%02d-%02d",
				ts.date.year, ts.date.month, ts.date.day,
				ts.time.hour, ts.time.minute, ts.time.second);

	storeUInt32_LE(&bext[256+32+32+10+8], (aafUInt32)(_origin & 0xffffffff));	// TimeRef low
	storeUInt32_LE(&bext[256+32+32+10+12], (aafUInt32)(_origin >> 32));			// TimeRef high

	memcpy(&bext[256+32+32+10+8+10], &_mobID, sizeof(_mobID));		// MobID

	aafUInt32 bytesWritten;
	_stream->Write(sizeof(riff_fmt), riff_fmt, &bytesWritten);
	_stream->Write(sizeof(bext), bext, &bytesWritten);
	_stream->Write(sizeof(data_hdr), data_hdr, &bytesWritten);

	_stream->GetPosition(&currentPos);
	_dataChunkOffset = (aafUInt32)currentPos;

	return AAFRESULT_SUCCESS;
}

HRESULT CAAFPCMCodec::find_data_chunk_offset(void)
{
	aafInt64 currentPos;
	aafUInt32 size, bytesRead;
	aafUInt8 buf[12];

	// Check for RIFF....WAVE file
	_stream->Read(12, buf, &bytesRead);
	if (memcmp(buf, "RIFF", 4) != 0 || memcmp(buf+8, "WAVE", 4))
	{
		return AAFRESULT_BADMEDIATYPE;
	}

	// Search for <data> chunk
	while (true)
	{
		_stream->Read(8, buf, &bytesRead);
		if (bytesRead != 8)
			return AAFRESULT_BADMEDIATYPE;		// Couldn't find data chunk

		_stream->GetPosition(&currentPos);

		size = readUInt32_LE(buf + 4);
		if (size <= 8)
			return AAFRESULT_BADMEDIATYPE;

		if (memcmp(buf, "data", 4) == 0)
		{
			// save offset
			_dataChunkOffset = (aafUInt32)currentPos;
			break;
		}
		else
		{
			// skip over chunk we are not interested in
			_stream->Seek(currentPos + size);
		}
	}

	return AAFRESULT_SUCCESS;
}

//
// Public Codec API
//

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CountDefinitions (aafUInt32 *pDefCount)
{
	plugin_trace("CAAFPCMCodec::CountDefinitions()\n");

	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetIndexedDefinitionID (aafUInt32 index, aafUID_t *uid)
{
	plugin_trace("CAAFPCMCodec::GetIndexedDefinitionID()\n");

	if(uid == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;
	*uid = kAAFCodecPCM;		// UID of the PCM codec definition
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetPluginDescriptorID (aafUID_t *uid)
{
	plugin_trace("CAAFPCMCodec::GetPluginDescriptorID()\n");

	*uid = AAFSDK_PCM_PLUGIN;		// UID of the PluginDescriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	plugin_trace("CAAFPCMCodec::GetEssenceDescriptorID()\n");

	*uid = AUID_AAFPCMDescriptor;		// stored class UID of the PCM Decriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetEssenceDataID (aafUID_t *uid)
{
	plugin_trace("CAAFPCMCodec::GetEssenceDataID()\n");

	*uid = AUID_AAFEssenceData;			// stored class UID of the PCM data object
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	plugin_trace("CAAFPCMCodec::GetIndexedDefinitionObject()\n");

	IAAFClassDef	*fileClass = NULL;
	IAAFCodecDef	*codecDef = NULL;
	IAAFDefObject	*obj = NULL;
	IAAFClassDef    *pcd = 0;
	aafUID_t		uid;

	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;

	XPROTECT()
	{
	  	CHECK(dict->LookupClassDef(AUID_AAFCodecDef, &pcd));
		CHECK(pcd->CreateInstance(IID_IAAFCodecDef, 
								  (IUnknown **)&codecDef));
		pcd->Release ();
		pcd = 0;
		uid = kAAFCodecPCM;
		CHECK(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(codecDef->Initialize(uid, L"PCM Codec", L"Handles RIFF PCM data."));
		CAAFBuiltinDefs defs (dict);
		CHECK(codecDef->AddEssenceKind (defs.ddkAAFSound()));
		CHECK(codecDef->AddEssenceKind (defs.ddSound()));
	  	CHECK(dict->LookupClassDef(AUID_AAFPCMDescriptor, &fileClass));
		CHECK(codecDef->SetFileDescriptorClass (fileClass));
		fileClass->Release ();
		fileClass = 0;
		*def = obj;
		codecDef->Release();
		codecDef = NULL;
	}
	XEXCEPT
	{
		if(codecDef != NULL)
		  {
			codecDef->Release();
			codecDef = 0;
		  }
		if(obj != NULL)
		  {
			obj->Release();
			obj = 0;
		  }
		if (pcd)
		  {
			pcd->Release ();
			pcd = 0;
		  }
		if (fileClass)
		  {
			fileClass->Release ();
			fileClass = 0;
		  }
	}
	XEND

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
	plugin_trace("CAAFPCMCodec::CreateDescriptor()\n");

	IAAFPluginDef	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	IAAFClassDef            *pcd = 0;

	XPROTECT()
	{
	    CHECK(dict->LookupClassDef(AUID_AAFPluginDef, &pcd));
		CHECK(pcd->CreateInstance(IID_IAAFPluginDef, 
								  (IUnknown **)&desc));
		pcd->Release ();
		pcd = 0;
		*descPtr = desc;
		desc->AddRef();
		CHECK(desc->Initialize(AAFSDK_PCM_PLUGIN, L"Example PCM Codec", L"Handles PCM data."));

		CHECK(desc->SetCategoryClass(AUID_AAFCodecDef));
		CHECK(desc->SetPluginVersionString(manufRev));
		CHECK(dict->LookupClassDef(AUID_AAFNetworkLocator, &pcd));
		CHECK(pcd->CreateInstance(IID_IAAFLocator, 
								  (IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (manufURL));
		CHECK(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		CHECK(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		CHECK(desc->SetManufacturerID(MANUF_AAFSDK_PLUGINS));
		CHECK(desc->SetPluginManufacturerName(manufName));
		CHECK(desc->SetIsSoftwareOnly(kAAFTrue));
		CHECK(desc->SetIsAccelerated(kAAFFalse));
		CHECK(desc->SetSupportsAuthentication(kAAFFalse));

		/**/
		CHECK(pcd->CreateInstance(IID_IAAFLocator, 
								  (IUnknown **)&pLoc));
		pcd->Release ();
		pcd = 0;
		CHECK(pLoc->SetPath (downloadURL));
		CHECK(desc->AppendLocator(pLoc));
		desc->Release();	// We have addRefed for the return value
		desc = NULL;
		pLoc->Release();
		pLoc = NULL;
	}
	XEXCEPT
	{
		if(desc != NULL)
		  {
			desc->Release();
			desc = 0;
		  }
		if(pLoc != NULL)
		  {
			pLoc->Release();
			pLoc = 0;
		  }
		if(pNetLoc != NULL)
		  {
			pNetLoc->Release();
			pNetLoc = 0;
		  }
		if (pcd)
		  {
			pcd->Release ();
			pcd = 0;
		  }
	}
	XEND

	return AAFRESULT_SUCCESS;
}


CAAFPCMCodec::CAAFPCMCodec (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{
	plugin_trace("CAAFPCMCodec::CAAFPCMCodec()\n");

	_stream = NULL;
	_pcmdes = NULL;
	_snddes = NULL;
	_access = NULL;
	_filedes = NULL;
	_containerFormat = NULL_UID;
    memset(&_mobID, 0, sizeof(_mobID));
	_origin = 0;
	_nativeByteOrder = GetNativeByteOrder();
	_sampleRate = defaultRate;
	_bitsPerSample = defaultSampleWidth;
	_numCh = defaultNumCh;
	_bytesPerFrame = (defaultSampleWidth+7)/8;
	_sampleFrames = 0;
	_lengthSamplesWritten = 0;
	_dataChunkOffset = 0;
	_interleaveBuf = NULL;
	_readOnly = kAAFFalse;
}


CAAFPCMCodec::~CAAFPCMCodec ()
{
	plugin_trace("CAAFPCMCodec::~CAAFPCMCodec()\n");

  // Do NOT release the _access interface since this object
  // contains the reference to this codec instance! We need
  // avoid the dreaded reference counting cycle of death!
	if(_stream != NULL)
		_stream->Release();
	if(_pcmdes != NULL)
		_pcmdes->Release();
	if(_snddes != NULL)
		_snddes->Release();
	if(_filedes != NULL)
		_filedes->Release();
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	plugin_trace("CAAFPCMCodec::SetEssenceAccess()\n");

	if(access == NULL)
		return AAFRESULT_NULL_PARAM;

	_access = access;

  return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CountFlavours(aafUInt32 *pCount)
{
	plugin_trace("CAAFPCMCodec::CountFlavours()\n");

	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetIndexedFlavourID (aafUInt32  index,
        aafUID_t *  pFlavour)
{
	plugin_trace("CAAFPCMCodec::GetIndexedFlavourID()\n");

	if(pFlavour == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;
	*pFlavour = kAAFNilCodecFlavour;
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	plugin_trace("CAAFPCMCodec::CountDataDefinitions()\n");

	//!!!Add error checking
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetIndexedDataDefinition (aafUInt32  /*index*/,
        aafUID_t * pDataDefID)
{
	plugin_trace("CAAFPCMCodec::GetIndexedDataDefinition()\n");

  if (! pDataDefID)
	return AAFRESULT_NOT_IMPLEMENTED;

  return AAFRESULT_NOT_IMPLEMENTED;
}

//!!!Need a way to convert between CLSID and aafUID_t
//!!!Change this to stored object IDs
const aafUID_t LOCAL_AAFPCMDescriptor = { 0x4c2e1691, 0x8ae6, 0x11d2, { 0x81, 0x3c, 0x00, 0x60, 0x97, 0x31, 0x01, 0x72 } };
const aafUID_t LOCAL_AAFEssenceData = { 0x6a33f4e1, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const wchar_t	name[] = L"PCM Codec";

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetMaxCodecDisplayNameLength (
        aafUInt32  *bufSize)
{
	plugin_trace("CAAFPCMCodec::GetMaxCodecDisplayNameLength()\n");

	if(bufSize == NULL)
		return AAFRESULT_NULL_PARAM;

	*bufSize = sizeof(name)/sizeof(wchar_t);
	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetCodecDisplayName (aafUID_constref /*flavour*/,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
	plugin_trace("CAAFPCMCodec::GetCodecDisplayName()\n");

	aafUInt32	len = sizeof(name);
	if(len > bufSize)
		len = bufSize;
	memcpy(pName, name, len);
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CountChannels (IAAFSourceMob * /*fileMob*/,
        aafUID_constref essenceKind,
        IAAFEssenceStream *stream,
        aafUInt16 *  pNumChannels)
{
	plugin_trace("CAAFPCMCodec::CountChannels()\n");

	if(EqualAUID(&essenceKind, &kAAFDataDef_Sound)
		|| EqualAUID(&essenceKind, &kAAFDataDef_LegacySound))
	{
		if(_stream == NULL)
		{
			_stream = stream;
			_stream->AddRef();
		}
		*pNumChannels = _numCh;
	}
	else
		*pNumChannels = 0;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetSelectInfo (IAAFSourceMob * /*fileMob*/,
        IAAFEssenceStream *stream,
        aafSelectInfo_t *  pSelectInfo)
{
	plugin_trace("CAAFPCMCodec::GetSelectInfo()\n");

	if(_stream == NULL)
	{
		_stream = stream;
		_stream->AddRef();
	}
	pSelectInfo->willHandleMDES = kAAFTrue;
#if defined(PORT_BYTESEX_LITTLE_ENDIAN)
	pSelectInfo->isNative = kAAFTrue;
#else
	pSelectInfo->isNative = kAAFFalse;
#endif
	pSelectInfo->hwAssisted = kAAFFalse;
	pSelectInfo->relativeLoss = 0;
	pSelectInfo->avgBitsPerSec =
		(_bitsPerSample *
		_sampleRate.numerator) /
		_sampleRate.denominator;

	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CountSamples (
        aafUID_constref essenceKind,
        aafLength_t *  pNumSamples)
{
	plugin_trace("CAAFPCMCodec::CountSamples()\n");

	if(EqualAUID(&essenceKind, &kAAFDataDef_Sound)
		|| EqualAUID(&essenceKind, &kAAFDataDef_LegacySound))
	{
		*pNumSamples = _sampleFrames;
	}
	else
		*pNumSamples = 0;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::ValidateEssence (IAAFSourceMob * /*fileMob*/,
        IAAFEssenceStream * /*stream*/,
		aafCheckVerbose_t   /*verbose*/,
        aafCheckWarnings_t  /*warning*/,
         aafUInt32   /*bufSize*/,
		wchar_t *   /*pName*/,
        aafUInt32  * /*bytesWritten*/)
{
	plugin_trace("CAAFPCMCodec::ValidateEssence()\n");

	return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
CAAFPCMCodec::Create (IAAFSourceMob *unk,
  aafUID_constref flavour,
  aafUID_constref essenceKind,
  aafRational_constref sampleRate,
  IAAFEssenceStream * stream,
  aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFPCMCodec::Create()\n");

	aafmMultiCreate_t createParms;


  XPROTECT()
	{
    memset(&createParms, 0, sizeof(createParms));
    createParms.mediaKind = &essenceKind;
    createParms.subTrackNum = 1;
    createParms.slotID = 1;
    createParms.sampleRate = sampleRate;
    CHECK(MultiCreate(unk, flavour, stream, compEnable, 1, &createParms));
	}
	XEXCEPT
	{
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::Open (IAAFSourceMob *unk,
		aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFPCMCodec::Open()\n");

	XPROTECT()
	{
    CHECK(MultiOpen(unk, openMode,stream, compEnable));
	}
	XEXCEPT
	{
	}
	XEND;
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::WriteSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *samplesWritten,
        aafUInt32 *bytesWritten)
{
	plugin_trace("CAAFPCMCodec::WriteSamples()\n");

	HRESULT hr = S_OK;

	if (NULL == buffer || NULL == samplesWritten || NULL == bytesWritten)
	  return AAFRESULT_NULL_PARAM;
	if (0 == nSamples)
	  return AAFRESULT_INVALID_PARAM;

	// The BWF file format limits filesize to 4GB.
	// The AAF and File containers do not have this limit.
	if (_containerFormat == kAAFContainerDef_RIFFWAVE)
	{
		aafPosition_t offset;
		checkResult(_stream->GetPosition(&offset));
		if ( offset + buflen > (aafPosition_t)4*1024*1024*1024-1 ) {
		  return AAFRESULT_EOF;
		}
	}

	// If there multiple channels in the PCM file and the data is interleaved
	// then write all of the interleaved samples together.
	aafmMultiXfer_t xferBlock;
	aafmMultiResult_t resultBlock;
	aafUID_t ddef = kAAFDataDef_Sound;

	resultBlock.bytesXfered = 0;
	resultBlock.samplesXfered = 0;
	xferBlock.mediaKind = &ddef; // temp var necessary because the mediaKind is non-const.
	xferBlock.subTrackNum = 1; // This should be a good default.
	xferBlock.numSamples = nSamples;
	xferBlock.buflen = buflen;
	xferBlock.buffer = buffer;

	hr = WriteBlocks(kAAFleaveInterleaved, 1, &xferBlock, &resultBlock);

	*samplesWritten = resultBlock.samplesXfered;
	*bytesWritten = resultBlock.bytesXfered;

	_lengthSamplesWritten += resultBlock.samplesXfered;
	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::WriteBlocks (aafDeinterleave_t  inter,
        aafUInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	plugin_trace("CAAFPCMCodec::WriteBlocks()\n");

	aafUInt32		fileBytes;
	aafUInt32		n, xfers, samp;
	aafUInt32		maxSamplesLeft;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result;
	pcm_interleaveBuf_t	*interPtr;
	aafUInt8		*destPtr;
	aafUInt8		sampleBuf[256];
	aafUInt32		bytesWritten;


	// Perform basic validation of input parameters.
	if (NULL == xferBlock || NULL == resultBlock)
	  return AAFRESULT_NULL_PARAM;

	for (n = 0; n < xferBlockCount; n++)
	{
	  if (NULL == xferBlock[n].buffer)
	    return AAFRESULT_NULL_PARAM;
	  else if (0 == xferBlock[n].buflen)
	    return AAFRESULT_INVALID_PARAM;
	}


	XPROTECT()
	{
		XASSERT(_bitsPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);

		for (n = 0; n < xferBlockCount; n++)
		{
			resultBlock[n].bytesXfered = 0;
			resultBlock[n].samplesXfered = 0;
		}

		if(inter == kAAFleaveInterleaved)
		{
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;

				fileBytes = xfer->numSamples * _bytesPerFrame;
				if (fileBytes > xfer->buflen)
					RAISE(AAFRESULT_SMALLBUF);

				CHECK(_stream->Write(fileBytes, xfer->buffer, &bytesWritten));

				resultBlock->bytesXfered = bytesWritten;
				resultBlock->samplesXfered += (bytesWritten / _bytesPerFrame);
			}
		}
		else if(_numCh == 1)
		{
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
				result = resultBlock + n;

				fileBytes = xfer->numSamples * _bytesPerFrame;
				if (fileBytes > xfer->buflen)
					RAISE(AAFRESULT_SMALLBUF);

				CHECK(_stream->Write(fileBytes, xfer->buffer, &bytesWritten));


				resultBlock->bytesXfered = bytesWritten;
				resultBlock->samplesXfered += (bytesWritten / _bytesPerFrame);
			}
		}
		else
		{
			aafUInt32	bytesPerSample;

			if(_interleaveBuf == NULL)
				_interleaveBuf = new pcm_interleaveBuf_t[_numCh];
			if(_interleaveBuf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			bytesPerSample = ((_bitsPerSample+ 7) / 8);
			for (n = 0; n < _numCh; n++)
				_interleaveBuf[n].buf = NULL;

			maxSamplesLeft = 0;
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
				resultBlock[n].bytesXfered = xfer->numSamples * bytesPerSample;
				resultBlock[n].samplesXfered = xfer->numSamples;

				XASSERT((xfer->subTrackNum >= 1) && xfer->subTrackNum <= _numCh, AAFRESULT_CODEC_CHANNELS);
				interPtr = _interleaveBuf + (xfer->subTrackNum-1);
				XASSERT(interPtr->buf == NULL, AAFRESULT_XFER_DUPCH);
				interPtr->buf = xfer->buffer;
				interPtr->samplesLeft = xfer->numSamples;
				interPtr->buflen = xfer->buflen;
				interPtr->bytesXfered = 0;
				if(maxSamplesLeft == 0)
					maxSamplesLeft = xfer->numSamples;
				else if(xfer->numSamples != maxSamplesLeft)
					RAISE(AAFRESULT_MULTI_WRITELEN);
			}

			while(maxSamplesLeft > 0)
			{
				xfers = sizeof(sampleBuf) / (bytesPerSample * xferBlockCount);
				if((aafUInt32) xfers > maxSamplesLeft)
					xfers = (aafUInt32) maxSamplesLeft;

				destPtr = sampleBuf;
				for (samp = 0; samp < xfers; samp++)
				{
					for (n = 0; n < xferBlockCount; n++)
					{
						interPtr = _interleaveBuf+n;
						memcpy(destPtr, interPtr->buf, bytesPerSample);
						interPtr->buf = (char *)interPtr->buf + bytesPerSample;
						destPtr += bytesPerSample;
						interPtr->samplesLeft--;
						interPtr->bytesXfered += bytesPerSample;
					}
					maxSamplesLeft--;
				}

				fileBytes = xfers * bytesPerSample * xferBlockCount;
				CHECK(_stream->Write(fileBytes, sampleBuf, &bytesWritten));
			}
		}
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}



HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::ReadSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  samplesRead,
        aafUInt32 *  bytesRead)
{
	plugin_trace("CAAFPCMCodec::ReadSamples()\n");

	HRESULT hr = S_OK;

	// If there multiple channels in the PCM file and the data is interleaved
	// then read all of the interleaved samples together.
	aafmMultiXfer_t xferBlock;
	aafmMultiResult_t resultBlock;
	aafUID_t ddef = kAAFDataDef_Sound;

	resultBlock.bytesXfered = 0;
	resultBlock.samplesXfered = 0;
	xferBlock.mediaKind = &ddef; // temp var necessary because the mediaKind is non-const.
	xferBlock.subTrackNum = 1; // This should be a good default.
	xferBlock.numSamples = nSamples;
	xferBlock.buflen = buflen;
	xferBlock.buffer = buffer;

	hr = ReadBlocks(kAAFleaveInterleaved, 1, &xferBlock, &resultBlock);

	*samplesRead = resultBlock.samplesXfered;
	*bytesRead = resultBlock.bytesXfered;

	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::ReadBlocks (aafDeinterleave_t  inter,
        aafUInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	plugin_trace("CAAFPCMCodec::ReadBlocks()\n");

	aafUInt32		nbytes, fileBytes;
	aafUInt32		n, xferSamples, sub;
	aafUInt32		maxSamplesLeft;
	aafUInt8		*start;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result = NULL;
	aafUInt16		ch, xf;
	aafUInt8		tmpBuf[256];


  // Perform basic validation of input parameters.
  if (NULL == xferBlock || NULL == resultBlock)
    return AAFRESULT_NULL_PARAM;

  for (n = 0; n < xferBlockCount; n++)
  {
    if (NULL == xferBlock[n].buffer)
      return AAFRESULT_NULL_PARAM;
    else if (0 == xferBlock[n].buflen)
      return AAFRESULT_INVALID_PARAM;
  }



	XPROTECT()
	{
		XASSERT(_bitsPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);

		for (n = 0; n < xferBlockCount; n++)
		{
			resultBlock[n].bytesXfered = 0;
			resultBlock[n].samplesXfered = 0;
		}

		if(inter == kAAFleaveInterleaved)
		{
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
				result = resultBlock + n;
				result->samplesXfered = 0;

				fileBytes = xfer->numSamples * _bytesPerFrame;
				if (fileBytes > xfer->buflen)
					RAISE(AAFRESULT_SMALLBUF);

				CHECK(_stream->Read(fileBytes, xfer->buffer, &nbytes));
				result->bytesXfered = nbytes;
				result->samplesXfered = nbytes / _bytesPerFrame;
			}
		}
		else if(_numCh == 1)
		{
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
				result = resultBlock + n;
				result->samplesXfered = 0;

				fileBytes = xfer->numSamples * _bytesPerFrame;
				if (fileBytes > xfer->buflen)
					RAISE(AAFRESULT_SMALLBUF);

				CHECK(_stream->Read(fileBytes, xfer->buffer, &nbytes));
				result->bytesXfered = nbytes;
				result->samplesXfered = nbytes / _bytesPerFrame;
			}
		}
		else
		{
			aafUInt32	bytesPerSample, startBuflen = 0;

			if(_interleaveBuf == NULL)
				_interleaveBuf = new pcm_interleaveBuf_t[_numCh];
			if(_interleaveBuf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			bytesPerSample = ((_bitsPerSample + 7) / 8);
			for (n = 0; n < _numCh; n++)
			{
				_interleaveBuf[n].buf = NULL;
				_interleaveBuf[n].xfer = NULL;
			}

			maxSamplesLeft = 0;
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
				result = resultBlock + n;
				result->samplesXfered = 0;

				if((xfer->subTrackNum <= 0) || (xfer->subTrackNum > _numCh))
					RAISE(AAFRESULT_CODEC_CHANNELS);

 				sub = xfer->subTrackNum-1;
 				if(_interleaveBuf[sub].buf != NULL)
					RAISE(AAFRESULT_XFER_DUPCH);

				_interleaveBuf[sub].buf = xfer->buffer;
				_interleaveBuf[sub].samplesLeft = xfer->numSamples;
				_interleaveBuf[sub].buflen = xfer->buflen;
				_interleaveBuf[sub].bytesXfered = 0;
				_interleaveBuf[sub].xfer = xfer;
				if(xfer->numSamples > maxSamplesLeft)
					maxSamplesLeft = xfer->numSamples;
			}

			for (n = 0, start = NULL; (n < _numCh) && (start == NULL); n++)
			{
				if(_interleaveBuf[n].buf != NULL)
				{
					start = (aafUInt8 *)_interleaveBuf[n].buf;
					startBuflen = _interleaveBuf[n].buflen;
				}
			}
			if(start == NULL)
				RAISE(AAFRESULT_BADDATAADDRESS);

			while(maxSamplesLeft > 0)
			{
				if(maxSamplesLeft * bytesPerSample  * _numCh >= sizeof(tmpBuf))
					xferSamples = maxSamplesLeft/ _numCh;
				else
				{
					xferSamples = maxSamplesLeft;
					start = tmpBuf;
				}
				fileBytes = xferSamples * bytesPerSample * _numCh;
				if ((aafUInt32)fileBytes > startBuflen)
					RAISE(AAFRESULT_SMALLBUF);

				CHECK(_stream->Read(fileBytes, start, &nbytes));

				SplitBuffers(start, xferSamples * _numCh, _bitsPerSample, 
							_numCh, _interleaveBuf);
				start += fileBytes / _numCh;
				maxSamplesLeft -= xferSamples;

				for (ch = 0; ch < _numCh; ch++)
				{
					xfer = _interleaveBuf[ch].xfer;
 					if(xfer != NULL)
					{
						result->bytesXfered = _interleaveBuf[ch].bytesXfered;
						result->samplesXfered = result->bytesXfered / bytesPerSample;
					}
				}
			}
		}
	}
	XEXCEPT
	{
		if((XCODE() == AAFRESULT_EOF) || (XCODE() == AAFRESULT_END_OF_DATA))
		{
			for (xf = 0; xf < xferBlockCount; xf++)
			{
				result = resultBlock + xf;
				result->bytesXfered = nbytes;
				result->samplesXfered = nbytes / _bytesPerFrame;	//!!!
			}
		}
	}
	XEND

	return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::Seek (aafPosition_t  sampleFrame)
{
	plugin_trace("CAAFPCMCodec::Seek()\n");

	aafInt64	byteOffsetForSample, offset;
	aafUInt32	bytesPerFrame;

	XPROTECT()
	{
		bytesPerFrame = ((_bitsPerSample + 7) / 8) * _numCh;
		byteOffsetForSample = sampleFrame * bytesPerFrame;

		// BWF files are limited to 4GB
		if (byteOffsetForSample > 0xffffffff)
			RAISE(AAFRESULT_BADSAMPLEOFFSET);

		offset = byteOffsetForSample + _dataChunkOffset;
		CHECK(_stream->Seek(offset));
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CompleteWrite (IAAFSourceMob *fileMob)
{
	plugin_trace("CAAFPCMCodec::CompleteWrite()\n");

	IAAFEssenceDescriptor	*essenceDesc = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	aafDataValue_t			buf = NULL;

	XPROTECT()
	{
		CHECK(_pcmdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
		CHECK(fileDesc->SetLength(_lengthSamplesWritten));
		fileDesc->Release();
		fileDesc = NULL;

		if(_interleaveBuf != NULL)
			delete _interleaveBuf;

		if(fileMob != NULL)
		{
			CHECK(fileMob->GetEssenceDescriptor(&essenceDesc));
			fileMob->Release();
			fileMob = NULL;
			CHECK(essenceDesc->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
			CHECK(fileDesc->SetLength(_lengthSamplesWritten));
			fileDesc->Release();
			fileDesc = NULL;
			essenceDesc->Release();
			essenceDesc = NULL;
		}

		// Update SoundDescriptor properties
		//
		CHECK(_snddes->SetQuantizationBits(_bitsPerSample));	// aafUInt32
		CHECK(_snddes->SetAudioSamplingRate(_sampleRate));		// aafRational_t
		CHECK(_snddes->SetChannelCount(_numCh));				// aafUInt32

		// Update PCMDescriptor properties
		//
		// The number of bytes used to store one sample of all channels.
		CHECK(_pcmdes->SetBlockAlign(
							(_bitsPerSample+7) / 8) * _numCh);	// aafUInt16 blockAlign

		// Average bytes per second of the essence stream.
		// (add 0.5 to round to nearest integer rather than truncate)
		aafUInt32 avBPS = (unsigned)
			(_bitsPerSample / 8.0 *
					_sampleRate.numerator / _sampleRate.denominator + 0.5);
		CHECK(_pcmdes->SetAverageBPS(avBPS));			// aafUInt32 BPS

		if (_containerFormat == kAAFContainerDef_RIFFWAVE)
		{
			CHECK(write_updated_BWF_size());
		}

		delete [] buf;
		buf = NULL;
	}
	XEXCEPT
	{
		if(essenceDesc != NULL)
			essenceDesc->Release();
		if(fileDesc != NULL)
			fileDesc->Release();

		if(buf != NULL)
			delete [] buf;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}		



HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::WriteRawData (aafUInt32 nSamples, aafDataBuffer_t  buffer,
        aafUInt32  buflen)
{
	plugin_trace("CAAFPCMCodec::WriteRawData()\n");

  aafUInt32 bytesWritten;

  	if(buflen < (nSamples * _bytesPerFrame))
  		return AAFRESULT_SMALLBUF;
	return _stream->Write (nSamples * _bytesPerFrame, buffer, &bytesWritten);
}




HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::ReadRawData (aafUInt32 nSamples,
		aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead,
        aafUInt32 *  samplesRead)
{
	plugin_trace("CAAFPCMCodec::ReadRawData()\n");

	XPROTECT()
	{
		XASSERT(buflen > nSamples * _bytesPerFrame, AAFRESULT_SMALLBUF);

		CHECK(_stream->Read (nSamples * _bytesPerFrame, buffer, bytesRead));
		*samplesRead = (*bytesRead)/_bytesPerFrame ;
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}




HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::CreateDescriptorFromStream (IAAFEssenceStream *  /*pStream*/,
        IAAFSourceMob * /*fileMob*/)
{
	plugin_trace("CAAFPCMCodec::CreateDescriptorFromStream()\n");

	return(AAFRESULT_NOT_IMPLEMENTED);
}




HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
	plugin_trace("CAAFPCMCodec::GetCurrentEssenceStream()\n");

	*ppStream = _stream;
	_stream->AddRef();
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	plugin_trace("CAAFPCMCodec::PutEssenceFormat()\n");

	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32;
	aafUID_t		opcode;
	aafUInt8		buf[256];

	XPROTECT()
	{
		CHECK(pFormat->NumFormatSpecifiers (&numSpecifiers));

		// First check sizes of supported format specifiers
		for(n = 0; n < numSpecifiers; n++)
		{
			CHECK(pFormat->GetIndexedFormatSpecifier (n, &opcode, sizeof(buf), buf, &bytesRead));

			if(EqualAUID(&kAAFAudioSampleBits, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
			}
			else
				RAISE(AAFRESULT_ILLEGAL_FILEFMT);
		}

		// Implement the actions for format specifiers
		for(n = 0; n < numSpecifiers; n++)
		{
			CHECK(pFormat->GetIndexedFormatSpecifier (n, &opcode, sizeof(buf), buf, &bytesRead));

			if(EqualAUID(&kAAFAudioSampleBits, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
				memcpy(&valueUInt32, buf, bytesRead);
				XASSERT(valueUInt32 > 0, AAFRESULT_ZERO_SAMPLESIZE);
				_bitsPerSample = (aafUInt16)valueUInt32;
                _bytesPerFrame = (_bitsPerSample + 7) / 8;
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
				memcpy(&valueUInt32, buf, bytesRead);
				XASSERT(valueUInt32 > 0, AAFRESULT_CODEC_CHANNELS);
				_numCh = (aafUInt16)valueUInt32;
			}
		}

		if (_containerFormat == kAAFContainerDef_RIFFWAVE)
		{
			// Write the BWF header and <bext> metadata chunk to the stream
			// in preparation for writing the PCM samples to the <data> chunk.
			CHECK(CAAFPCMCodec::write_BWF_header());
		}
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
{
	plugin_trace("CAAFPCMCodec::GetEssenceFormat()\n");

	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32;
	aafRational_t	valueRat;
	aafUID_t		opcode;
	aafUInt8		buf[256];
	IAAFEssenceFormat *fmt;

	XPROTECT()
	{
		CHECK(_access->GetEmptyFileFormat(&fmt));
		*pResult = fmt;

		CHECK(pTemplate->NumFormatSpecifiers (&numSpecifiers));

		for(n = 0; n < numSpecifiers; n++)
		{
			CHECK(pTemplate->GetIndexedFormatSpecifier (n, &opcode, sizeof(buf), buf, &bytesRead));

			if(EqualAUID(&kAAFAudioSampleBits, &opcode))
			{
				valueUInt32 = _bitsPerSample;
				memcpy(buf, &valueUInt32, sizeof(valueUInt32));
				CHECK(fmt->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(valueUInt32), buf));
			}
			else if(EqualAUID(&kAAFSampleRate, &opcode))
			{
				valueRat = _sampleRate;
				memcpy(buf, &valueRat, sizeof(valueRat));
				CHECK(fmt->AddFormatSpecifier (kAAFSampleRate, sizeof(valueRat), buf));
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				valueUInt32 = _numCh;
				memcpy(buf, &valueUInt32, sizeof(valueUInt32));
				CHECK(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(valueUInt32), buf));
			}
			else if(EqualAUID(&kAAFMaxSampleBytes, &opcode))
			{
				valueUInt32 = (_bitsPerSample + 7) / 8 * _numCh;
				memcpy(buf, &valueUInt32, sizeof(valueUInt32));
				CHECK(fmt->AddFormatSpecifier (kAAFMaxSampleBytes, sizeof(valueUInt32), buf));
			}
			else
				RAISE(AAFRESULT_INVALID_OP_CODEC);
		}
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
	plugin_trace("CAAFPCMCodec::GetDefaultEssenceFormat()\n");

	IAAFEssenceFormat	*fmt;
	aafInt32			parm;
	aafRational_t		ratParm;

	XPROTECT()
	{
		CHECK(_access->GetEmptyFileFormat(&fmt));
		*pResult = fmt;

		parm = defaultSampleWidth;
		CHECK(fmt->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(parm), (aafDataBuffer_t)&parm));
		ratParm = defaultRate;
		CHECK(fmt->AddFormatSpecifier (kAAFSampleRate, sizeof(ratParm), (aafDataBuffer_t)&ratParm));
		parm = defaultNumCh;
		CHECK(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(parm), (aafDataBuffer_t)&parm));
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetIndexedSampleSize
 (aafUID_constref dataDefID,
  aafPosition_t pos,
  aafLength_t *pResult)
{
	plugin_trace("CAAFPCMCodec::GetIndexedSampleSize()\n");

	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(pos < 0 || pos >=_sampleFrames)
		return(AAFRESULT_EOF);

	if(EqualAUID(&dataDefID, &kAAFDataDef_Sound)
		|| EqualAUID(&dataDefID, &kAAFDataDef_LegacySound))
		*pResult = _bytesPerFrame;
	else
		return(AAFRESULT_CODEC_CHANNELS);

	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::GetLargestSampleSize (aafUID_constref dataDefID,
										 aafLength_t *pResult)
{
	plugin_trace("CAAFPCMCodec::GetLargestSampleSize()\n");

	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(EqualAUID(&dataDefID, &kAAFDataDef_Sound)
		|| EqualAUID(&dataDefID, &kAAFDataDef_LegacySound))
		*pResult = _bytesPerFrame;
	else
		return(AAFRESULT_CODEC_CHANNELS);
	return (AAFRESULT_SUCCESS);
}



HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::MultiCreate (IAAFSourceMob *unk,
        aafUID_constref /*flavour*/,
        IAAFEssenceStream * stream,
        aafCompressEnable_t compEnable,
        aafUInt32 numParms,
        aafmMultiCreate_t *createParms)
{
	plugin_trace("CAAFPCMCodec::MultiCreate()\n");

	IAAFSourceMob			*fileMob = NULL;
	IAAFMob					*pMob = NULL;
	IAAFEssenceDescriptor	*mdes = NULL;
	IAAFContainerDef		*containerDef = NULL;
	IAAFDefObject			*defObj = NULL;

	if(_stream == NULL)
	{
		_stream = stream;
		_stream->AddRef();
	}
	_readOnly = kAAFFalse;
	XPROTECT()
	{
		_sampleRate = createParms[0].sampleRate;

	    // whether or not we will be compressing the samples as they are read.
		CHECK(SetCompressionEnabled(compEnable));

		CHECK(unk->QueryInterface(IID_IAAFSourceMob, (void **)&fileMob));
		CHECK(fileMob->GetEssenceDescriptor(&mdes));
		fileMob->Release();
		fileMob = NULL;

		CHECK(unk->QueryInterface(IID_IAAFMob, (void **)&pMob));
		CHECK(pMob->GetMobID(&_mobID));

		// Get first slot then read the Origin of this timeline slot
		IAAFMobSlot	*slot = NULL;
		CHECK(pMob->GetSlotAt(0, &slot));
		IAAFTimelineMobSlot *timelineSlot = NULL;
		CHECK(slot->QueryInterface(IID_IAAFTimelineMobSlot, (void **) &timelineSlot));
		CHECK(timelineSlot->GetOrigin(&_origin));

		slot->Release();
		timelineSlot->Release();
		pMob->Release();
		pMob = NULL;

		CHECK(mdes->QueryInterface(IID_IAAFSoundDescriptor, (void **)&_snddes));
		CHECK(mdes->QueryInterface(IID_IAAFPCMDescriptor, (void **)&_pcmdes));
		CHECK(mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&_filedes));

		CHECK(_filedes->GetContainerFormat(&containerDef));
		CHECK(containerDef->QueryInterface(IID_IAAFDefObject, (void **)&defObj));
		CHECK(defObj->GetAUID(&_containerFormat));

		containerDef->Release();
		defObj->Release();

		_pcmdes->Initialize();

		mdes->Release();
	}
	XEXCEPT
	{
		if(fileMob != NULL)
			fileMob->Release();
		if(pMob != NULL)
			pMob->Release();
		if(mdes != NULL)
			mdes->Release();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFPCMCodec::MultiOpen (IAAFSourceMob *unk,
		aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t compEnable)
{
	plugin_trace("CAAFPCMCodec::MultiOpen()\n");

	IAAFSourceMob			*fileMob = NULL;
	IAAFEssenceDescriptor	*mdes = NULL;
	IAAFContainerDef		*containerDef = NULL;
	IAAFDefObject			*defObj = NULL;

	if(_stream == NULL)
	{
		_stream = stream;
		_stream->AddRef();
	}
	_readOnly = openMode == kAAFMediaOpenReadOnly ? kAAFTrue : kAAFFalse;


	XPROTECT()
	{
		// whether or not we will be compressing the samples as they are read.
		CHECK(SetCompressionEnabled(compEnable));
		CHECK(unk->QueryInterface(IID_IAAFSourceMob, (void **)&fileMob));
		CHECK(fileMob->GetEssenceDescriptor(&mdes));
		CHECK(mdes->QueryInterface(IID_IAAFSoundDescriptor, (void **)&_snddes));
		CHECK(mdes->QueryInterface(IID_IAAFPCMDescriptor, (void **)&_pcmdes));
		CHECK(mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&_filedes));

		CHECK(_filedes->GetContainerFormat(&containerDef));
		CHECK(containerDef->QueryInterface(IID_IAAFDefObject, (void **)&defObj));
		CHECK(defObj->GetAUID(&_containerFormat));

		if (_containerFormat == kAAFContainerDef_RIFFWAVE)
		{
			CHECK(find_data_chunk_offset());
		}

		containerDef->Release();
		defObj->Release();

		fileMob->Release();
		fileMob = NULL;
		mdes->Release();
		mdes = NULL;
	}
	XEXCEPT
	{
		if(fileMob != NULL)
			fileMob->Release();
		if(mdes != NULL)
			mdes->Release();
	}
	XEND;
	return AAFRESULT_SUCCESS;
}


AAFRESULT CAAFPCMCodec::SetCompressionEnabled (aafCompressEnable_t /*compEnable*/)
{
	plugin_trace("CAAFPCMCodec::SetCompressionEnabled()\n");

  return AAFRESULT_SUCCESS;
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
AAFRESULT CAAFPCMCodec::fillSwappedPCMData(aafUInt8 **destBufHdl, aafUInt32 maxsize, void *data)
{
	plugin_trace("CAAFPCMCodec::fillSwappedPCMData()\n");

	aafInt32        dataL;
	aafInt16        dataS;
	aafUInt8		*destBufPtr = *destBufHdl;

	if ((maxsize == sizeof(aafInt32)) && (INTEL_ORDER != _nativeByteOrder))
	{
		dataL = *((aafInt32 *) data);
		AAFByteSwap32(&dataL);
		data = &dataL;
	} else if ((maxsize == sizeof(aafInt16)) && (INTEL_ORDER != _nativeByteOrder))
	{
		dataS = *((aafInt16 *) data);
		AAFByteSwap16(&dataS);
		data = &dataS;
	}
	memcpy(destBufPtr, data, maxsize);
	*destBufHdl = destBufPtr + maxsize;

	return (AAFRESULT_SUCCESS);
}

AAFRESULT CAAFPCMCodec::scanSwappedPCMData(aafUInt8 **srcBufHdl, aafUInt32 maxsize, void *data)
{
	plugin_trace("CAAFPCMCodec::scanSwappedPCMData()\n");

	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;

	if ((maxsize == sizeof(aafInt32)) && (INTEL_ORDER != _nativeByteOrder))
			AAFByteSwap32((aafInt32 *) data);
	else if ((maxsize == sizeof(aafInt16)) && (INTEL_ORDER != _nativeByteOrder))
			AAFByteSwap16((aafInt16 *) data);

	return (AAFRESULT_SUCCESS);
}


/*
 * Given a source buffer & an array of destination buffers, split the samples up in
 * round-robin fashion.  If a destination buffer in the array is NULL, then src is
 * incremented by one sample, but no data transfer occurs.  The first destination
 * buffer is allowed to be the same as the source buffer, but the other destination
 * buffers must be distinct to avoid corruption of the data.
 *
 * The number of entries in dest[] must be equal to numDest, and the dest array WILL
 * BE MODIFIED during execution, so it will have to be reloaded.
 */
static void SplitBuffers(void *original, aafUInt32 srcSamples, aafUInt16 sampleSize, aafUInt16 numDest, pcm_interleaveBuf_t *destPtr)
{
	aafInt16	*src16, *srcEnd16, *dest16;
	char	*src8, *srcEnd8, *dest8;
	aafInt16	n, x, sampleBytes;

	sampleBytes = (sampleSize + 7) / 8;
	if(sampleSize == 16)
	{
		src16 = (aafInt16 *)original;
		srcEnd16 = src16 + srcSamples;
		while(src16 < srcEnd16)
		{
			for(n = 0; (n < numDest) && (src16 < srcEnd16) ; n++)
			{
				dest16 = (aafInt16 *)destPtr[n].buf;
				if(dest16 != NULL)
				{
					destPtr[n].bytesXfered += sampleBytes;
					*dest16++ = *src16++;
				}
				else
					src16++;
				destPtr[n].buf = (char *)dest16;
			}
		}
	}
	else
	{
		src8 = (char *)original;
		srcEnd8 = src8 + srcSamples;
		while(src8 < srcEnd8)
		{
			for(n = 0; (n < numDest) && (src8 < srcEnd8) ; n++, dest8++)
			{
				dest8 = (char *)destPtr[n].buf;
				if(dest8 != NULL)
				{
					destPtr[n].bytesXfered += sampleBytes;
					{	for(x = 0; x < sampleBytes; x++)
							*dest8++ = *src8++;
					}
				}
				else
					src8++;
				destPtr[n].buf = (char *)dest8;
			}
		}
	}
}

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFPCMCodec::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
	plugin_trace("CAAFPCMCodec::InternalQueryInterface()\n");

    if (NULL == ppvObj)
        return E_INVALIDARG;

    if (EQUAL_UID(riid,IID_IAAFMultiEssenceCodec)) 
    { 
        *ppvObj = (IAAFMultiEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (EQUAL_UID(riid,IID_IAAFEssenceCodec)) 
    { 
        *ppvObj = (IAAFEssenceCodec *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
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
AAF_DEFINE_FACTORY(AAFPCMCodec)
