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
#include "CAAFAIFCCodec.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "aafErr.h"
#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"

#define STD_HDRSIZE_NODATA		256			// Make sure that the buffer is big enough

const aafProductVersion_t AAFPluginImplementationVersion = {1, 0, 0, 1, kAAFVersionBeta};
const aafRational_t		defaultRate = { 44100, 1 };
const aafUInt32			defaultSampleWidth = 8;
const aafUInt32			defaultNumCh = 1;

// {67F9AF33-3700-11d4-A361-009027DFCA6A}
const CLSID CLSID_AAFAIFCCodec = { 0x67f9af33, 0x3700, 0x11d4, { 0xa3, 0x61, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

// {67F9AF34-3700-11d4-A361-009027DFCA6A}
const aafUID_t JEFFS_AIFC_PLUGIN = { 0x67f9af34, 0x3700, 0x11d4, { 0xa3, 0x61, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

static double          ConvertFromIeeeExtended(char *bytes);
static void            ConvertToIeeeExtended(double num, char *bytes);

static void SplitBuffers(void *original, aafUInt32 srcSamples, aafUInt16 sampleSize, aafUInt16 numDest, interleaveBufAIFF_t *destPtr);

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CountDefinitions (aafUInt32 *pDefCount)
{
	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetIndexedDefinitionID (aafUInt32 index, aafUID_t *uid)
{
	if(uid == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;
	
	*uid = kAAFCODEC_AIFC;		// UID of the AIFC codec definition
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetPluginDescriptorID (aafUID_t *uid)
{
	*uid = JEFFS_AIFC_PLUGIN;		// UID of the PluginDescriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	*uid = AUID_AAFAIFCDescriptor;		// stored class UID of the AIFC Decriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetEssenceDataID (aafUID_t *uid)
{
	*uid = AUID_AAFEssenceData;			// stored class UID of the AIFC data object
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
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
		uid = kAAFCODEC_AIFC;
		CHECK(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(codecDef->Initialize(uid, L"AIFC Codec", L"Handles RIFF AIFC data."));
		CAAFBuiltinDefs defs (dict);
		CHECK(codecDef->AddEssenceKind (defs.ddSound()));
		CHECK(dict->LookupClassDef(AUID_AAFAIFCDescriptor, &fileClass));
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

static wchar_t *manufURL = L"http://www.avid.com";
static wchar_t *downloadURL = L"ftp://ftp.avid.com/pub/";
const aafUID_t MANUF_JEFFS_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
static aafVersionType_t samplePluginVersion = { 0, 1 };

static wchar_t *manufName = L"Avid Technology, Inc.";
static wchar_t *manufRev = L"Rev 0.1";

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
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
		CHECK(desc->Initialize(JEFFS_AIFC_PLUGIN, L"Example AIFC Codec", L"Handles AIFC data."));
		
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
		
		CHECK(desc->SetManufacturerID(MANUF_JEFFS_PLUGINS));
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


CAAFAIFCCodec::CAAFAIFCCodec (IUnknown * pControllingUnknown)
: CAAFUnknown (pControllingUnknown)
{
	_headerLoaded = kAAFFalse;
	_nativeByteOrder = GetNativeByteOrder();
	_sampleRate = defaultRate;
	_bitsPerSample = defaultSampleWidth;
	_numCh = defaultNumCh;
	_bytesPerFrame = (defaultSampleWidth+7)/8;;
	_sampleFrames = 0;
	_interleaveBuf = NULL;
	_dataStartOffset = 0;
	_dataSizeOffset = 0;
	_readOnly = kAAFFalse;
	_stream = NULL;
	_access = NULL;
	_sampleDataHeaderWritten = kAAFFalse;
	_initialSeekPerformed = kAAFFalse;
	_mdes = NULL;
	_interleaveBuf = NULL;
}


CAAFAIFCCodec::~CAAFAIFCCodec ()
{
	// Do NOT release the _access interface since this object
	// contains the reference to this codec instance! We need
	// avoid the dreaded reference counting cycle of death!
	if(_stream != NULL)
		_stream->Release();
	if(_mdes != NULL)
		_mdes->Release();
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	if(access == NULL)
		return AAFRESULT_NULL_PARAM;
	
	_access = access;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CountFlavours(aafUInt32 *pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetIndexedFlavourID (aafUInt32  index,
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
CAAFAIFCCodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	//!!!Add error checking
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetIndexedDataDefinition (aafUInt32  /*index*/,
										 aafUID_t * pDataDefID)
{
	if (! pDataDefID)
		return AAFRESULT_NOT_IMPLEMENTED;
	
	return HRESULT_NOT_IMPLEMENTED;
}

//!!!Need a way to convert between CLSID and aafUID_t
//!!!Change this to stored object IDs
const aafUID_t LOCAL_AAFAIFCDescriptor = { 0xe684d773, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const aafUID_t LOCAL_AAFEssenceData = { 0x6a33f4e1, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const wchar_t	name[] = L"AIFC Codec";

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetMaxCodecDisplayNameLength (
											 aafUInt32  *bufSize)
{
	if(bufSize == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*bufSize = sizeof(name)/sizeof(wchar_t);
	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetCodecDisplayName (aafUID_constref /*flavour*/,	// No flavors
									aafCharacter *  pName,
									aafUInt32  bufSize)
{
	aafUInt32	len = sizeof(name);
	if(len > bufSize)
		len = bufSize;
	memcpy(pName, name, len);
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CountChannels (IAAFSourceMob * /*fileMob*/,
							  aafUID_constref essenceKind,
							  IAAFEssenceStream *stream,
							  aafUInt16 *  pNumChannels)
{
	XPROTECT()
	{
		if(EqualAUID(&essenceKind, &DDEF_Sound))
		{
			if(!_headerLoaded)
			{
				if(_stream == NULL)
				{
					_stream = stream;
					_stream->AddRef();
				}
				CHECK(loadAIFCHeader());
			}
			*pNumChannels = _numCh;
		}
		else
			*pNumChannels = 0;
    }
    XEXCEPT
		XEND;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetSelectInfo (IAAFSourceMob * /*fileMob*/,
							  IAAFEssenceStream *stream,
							  aafSelectInfo_t *  pSelectInfo)
{
	XPROTECT()
	{
		if(!_headerLoaded)
		{
			if(_stream == NULL)
			{
				_stream = stream;
				_stream->AddRef();
			}
			CHECK(loadAIFCHeader());
		}
		pSelectInfo->willHandleMDES = kAAFTrue;
#if PORT_BYTESEX_BIG_ENDIAN
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
    }
    XEXCEPT
		XEND;
	
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CountSamples (
							 aafUID_constref essenceKind,
							 aafLength_t *  pNumSamples)
{
	if(EqualAUID(&essenceKind, &DDEF_Sound))
	{
		*pNumSamples = _sampleFrames;
	}
	else
		*pNumSamples = 0;
	
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::ValidateEssence (IAAFSourceMob * /*fileMob*/,
								IAAFEssenceStream * /*stream*/,
								aafCheckVerbose_t   /*verbose*/,
								aafCheckWarnings_t  /*warning*/,
								aafUInt32   /*bufSize*/,
								wchar_t *   /*pName*/,
								aafUInt32  * /*bytesWritten*/)
{
	return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::Create (IAAFSourceMob *unk,
					   aafUID_constref flavour,
					   aafUID_constref essenceKind,
					   aafRational_constref sampleRate,
					   IAAFEssenceStream * stream,
					   aafCompressEnable_t compEnable)
{
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
CAAFAIFCCodec::Open (IAAFSourceMob *unk,
					 aafMediaOpenMode_t  openMode,
					 IAAFEssenceStream * stream,
					 aafCompressEnable_t compEnable)
{
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
CAAFAIFCCodec::WriteSamples (aafUInt32  nSamples,
							 aafUInt32  buflen,
							 aafDataBuffer_t  buffer,
							 aafUInt32 *samplesWritten,
							 aafUInt32 *bytesWritten)
{
	HRESULT hr = S_OK;
	
	if (NULL == buffer || NULL == samplesWritten || NULL == bytesWritten)
		return AAFRESULT_NULL_PARAM;
	if (0 == nSamples)
		return AAFRESULT_INVALID_PARAM;
	
	// If there multiple channels in the AIFC file and the data is interleaved
	// then write all of the interleaved samples together.
	aafmMultiXfer_t xferBlock;
	aafmMultiResult_t resultBlock;
	aafUID_t ddef = DDEF_Sound;
	
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
	
	
	return hr;
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::WriteBlocks (aafDeinterleave_t  inter,
							aafUInt16  xferBlockCount,
							aafmMultiXfer_t *  xferBlock,
							aafmMultiResult_t *  resultBlock)
{
	aafUInt32      	fileBytes;
	aafUInt32      	n, xfers, samp;
	aafUInt32		maxSamplesLeft;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result;
	interleaveBufAIFF_t	*interPtr;
	aafUInt8		*destPtr;
	aafUInt32		zero = 0;
	aafUInt8		sampleBuf[256], *ptr;
	aafUInt32		actualSize, bytesWritten;
	unsigned char	header[STD_HDRSIZE_NODATA];
	
	
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
		
		if(!_sampleDataHeaderWritten)
		{
			CHECK(CreateAIFCheader(header, STD_HDRSIZE_NODATA, _numCh, &actualSize));
			
			_stream->Seek(0);
			_sampleDataHeaderWritten = kAAFTrue;
			// The next four lines won't work for raw writes
			//****** Make the start of the SSND chunk here
			ptr = header + actualSize;
			*ptr++ = 'S';		// Make sure that this is NOT byte-swapped
			*ptr++ = 'S';
			*ptr++ = 'N';
			*ptr++ = 'D';
		
			/* fill in SSND size later */
			_dataSizeOffset = actualSize + 4;
			CHECK(fillSwappedAIFCData(&ptr, 4L, (void *) &zero));// Patch SSND length in later
		
			/* ssndoffset */
			CHECK(fillSwappedAIFCData(&ptr, 4L, (void *) &zero));
	
			/* blocksize */
			CHECK(fillSwappedAIFCData(&ptr, 4L, (void *) &zero));
			CHECK(_stream->Write(ptr-header, header, &bytesWritten));
		}
		
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
				
				resultBlock->bytesXfered = xfer->numSamples * _bytesPerFrame;
				resultBlock->samplesXfered += xfer->numSamples;
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
				
				
				result->bytesXfered = xfer->numSamples * _bytesPerFrame;
				result->samplesXfered += xfer->numSamples;
			}
		}
		else
		{
			aafUInt32	bytesPerSample;
			
			if(_interleaveBuf == NULL)
				_interleaveBuf = new interleaveBufAIFF_t[_numCh];
			if(_interleaveBuf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			bytesPerSample = ((_bitsPerSample+ 7) / 8);
			for (n = 0; n < _numCh; n++)
				_interleaveBuf[n].buf = NULL;
			
			maxSamplesLeft = 0;
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
				resultBlock->bytesXfered = xfer->numSamples * bytesPerSample;
				resultBlock->samplesXfered = xfer->numSamples;
				
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
CAAFAIFCCodec::ReadSamples (aafUInt32  nSamples,
							aafUInt32  buflen,
							aafDataBuffer_t  buffer,
							aafUInt32 *  samplesRead,
							aafUInt32 *  bytesRead)
{
	HRESULT hr = S_OK;
	
	// If there multiple channels in the AIFC file and the data is interleaved
	// then read all of the interleaved samples together.
	aafmMultiXfer_t xferBlock;
	aafmMultiResult_t resultBlock;
	aafUID_t ddef = DDEF_Sound;
	
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
CAAFAIFCCodec::ReadBlocks (aafDeinterleave_t  inter,
						   aafUInt16  xferBlockCount,
						   aafmMultiXfer_t *  xferBlock,
						   aafmMultiResult_t *  resultBlock)
{
	aafUInt32       nbytes, fileBytes;
	aafUInt32        n, startBuflen, xferSamples, sub;
	aafUInt32		maxSamplesLeft;
	aafUInt8		*start;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result;
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
		if(!_initialSeekPerformed)
		{
			CHECK(_stream->Seek(_dataStartOffset));	// Not compatible with raw read
			_initialSeekPerformed = kAAFTrue;
		}
		
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
			aafUInt32	bytesPerSample;
			
			if(_interleaveBuf == NULL)
				_interleaveBuf = new interleaveBufAIFF_t[_numCh];
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
CAAFAIFCCodec::Seek (aafPosition_t  sampleFrame)
{
	aafInt64          nBytes;
	aafInt64          temp, offset, one;
	aafUInt32           bytesPerFrame;
	
	XPROTECT()
	{
		CvtInt32toInt64(1, &one);
		temp = _sampleFrames;
		CHECK(AddInt32toInt64(1, &temp));
		if (Int64Greater(sampleFrame, temp))
			RAISE(AAFRESULT_BADSAMPLEOFFSET);
		
		nBytes = sampleFrame;
		
		/* Make the result zero-based (& check for bad frame numbers as well). */
		if(Int64Less(nBytes, one))
			RAISE(AAFRESULT_BADSAMPLEOFFSET);
		CHECK(SubInt64fromInt64(one, &nBytes));
		bytesPerFrame = ((_bitsPerSample + 7) / 8) * _numCh;
		CHECK(MultInt32byInt64(bytesPerFrame, nBytes, &nBytes));
		offset = _dataStartOffset;
		CHECK(AddInt64toInt64(nBytes, &offset));
		
		CHECK(_stream->Seek(offset));
	}
	XEXCEPT
		XEND
		
		return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CompleteWrite (IAAFSourceMob *fileMob)
{
	aafInt64		sampleLen;
	aafUInt32		AIFCDataLen;
	IAAFEssenceDescriptor	*essenceDesc = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	IAAFAIFCDescriptor		*AIFCDesc = NULL;
	aafDataValue_t			buf = NULL;
	
	XPROTECT()
	{
		if(!_readOnly && _sampleDataHeaderWritten)
			CHECK(CreateAudioDataEnd());	// Don't do this for raw calls?
		//		_stream = NULL;
		

		CHECK(_stream->Seek(_dataSizeOffset));
		CHECK(GetAIFCData(sizeof(AIFCDataLen), &AIFCDataLen));	// Read the AIFC data length
		sampleLen = AIFCDataLen / _bytesPerFrame;
		CHECK(_mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
		CHECK(fileDesc->SetLength(sampleLen));
		fileDesc->Release();
		fileDesc = NULL;
		

		if(_interleaveBuf != NULL)
			delete _interleaveBuf;
		
		if(fileMob != NULL)
		{
			aafUInt32		bufsiz;
			
			CHECK(fileMob->GetEssenceDescriptor(&essenceDesc));
			fileMob->Release();
			fileMob = NULL;
			CHECK(essenceDesc->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
			CHECK(fileDesc->SetLength(sampleLen));
			fileDesc->Release();
			fileDesc = NULL;
			CHECK(essenceDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&AIFCDesc));
			essenceDesc->Release();
			essenceDesc = NULL;
			CHECK(_mdes->GetSummaryBufferSize (&bufsiz));
			buf = new aafUInt8[bufsiz];
			if(buf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			CHECK(_mdes->GetSummary (bufsiz, buf));
			CHECK(AIFCDesc->SetSummary (bufsiz, buf));
			AIFCDesc->Release();
			AIFCDesc = NULL;
			delete [] buf;
			buf = NULL;
		}
	}
	XEXCEPT
	{
		if(essenceDesc != NULL)
			essenceDesc->Release();
		if(fileDesc != NULL)
			fileDesc->Release();
		if(AIFCDesc != NULL)
			AIFCDesc->Release();
		
		if(buf != NULL)
			delete [] buf;
	}
	XEND;
	
	return HRESULT_SUCCESS;
}		



HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::WriteRawData (aafUInt32 nSamples, aafDataBuffer_t  buffer,
							 aafUInt32  buflen)
{
	aafUInt32 bytesWritten;
  	if(buflen < (nSamples * _bytesPerFrame))
  		return AAFRESULT_SMALLBUF;
	return _stream->Write (nSamples * _bytesPerFrame, buffer, &bytesWritten);
}




HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::ReadRawData (aafUInt32 nSamples,
							aafUInt32  buflen,
							aafDataBuffer_t  buffer,
							aafUInt32 *  bytesRead,
							aafUInt32 *  samplesRead)
{
	XPROTECT()
	{
		XASSERT(buflen > nSamples * _bytesPerFrame, AAFRESULT_SMALLBUF);
		if(!_initialSeekPerformed)
		{
			CHECK(_stream->Seek(0L));
			_initialSeekPerformed = kAAFTrue;
		}
		
		CHECK(_stream->Read (nSamples * _bytesPerFrame, buffer, bytesRead));
		*samplesRead = (*bytesRead)/_bytesPerFrame ;
	}
	XEXCEPT
		XEND
		
		return HRESULT_SUCCESS;
}




HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::CreateDescriptorFromStream (IAAFEssenceStream *  /*pStream*/,
										   IAAFSourceMob * /*fileMob*/)
{
	return(AAFRESULT_NOT_IMPLEMENTED);
}




HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
	*ppStream = _stream;
	_stream->AddRef();
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32, actualSize;
	aafRational_t	valueRat;
	aafUID_t		opcode;
	aafUInt8		buf[256];
	aafUInt8		header[STD_HDRSIZE_NODATA];
	
	XPROTECT()
	{
		CHECK(pFormat->NumFormatSpecifiers (&numSpecifiers));
		
		for(n = 0; n < numSpecifiers; n++)
		{
			CHECK(pFormat->GetIndexedFormatSpecifier (n, &opcode, sizeof(buf), buf, &bytesRead));
			
			if(EqualAUID(&kAAFAudioSampleBits, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
			}
			else if(EqualAUID(&kAAFSampleRate, &opcode))
			{
				XASSERT(bytesRead == sizeof(aafRational_t), AAFRESULT_INVALID_PARM_SIZE);
			}
			else if(EqualAUID(&kAAFSampleFormat, &opcode))
			{
				RAISE(AAFRESULT_INVALID_OP_CODEC);
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
			}
			else
				RAISE(AAFRESULT_ILLEGAL_FILEFMT);
		}
		
		for(n = 0; n < numSpecifiers; n++)
		{
			CHECK(pFormat->GetIndexedFormatSpecifier (n, &opcode, sizeof(buf), buf, &bytesRead));
			
			if(EqualAUID(&kAAFAudioSampleBits, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
				memcpy(&valueUInt32, buf, bytesRead);
				_bitsPerSample = (aafUInt16)valueUInt32;
				_bytesPerFrame = ((_bitsPerSample + 7) / 8) * _numCh;
			}
			else if(EqualAUID(&kAAFSampleRate, &opcode))
			{
				XASSERT(bytesRead == sizeof(aafRational_t), AAFRESULT_INVALID_PARM_SIZE);
				memcpy(&valueRat, buf, bytesRead);
				_sampleRate = valueRat;
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				XASSERT(bytesRead == sizeof(valueUInt32), AAFRESULT_INVALID_PARM_SIZE);
				memcpy(&valueUInt32, buf, bytesRead);
				_numCh = (aafUInt16)valueUInt32;
				_bytesPerFrame = ((_bitsPerSample + 7) / 8) * _numCh;
			}
		}
		
		CHECK(CreateAIFCheader(header, STD_HDRSIZE_NODATA, _numCh, &actualSize));
		CHECK(_mdes->SetSummary (actualSize, header));
		
		// This will output the header on the next non-raw write
		_sampleDataHeaderWritten = kAAFFalse;
	}
	XEXCEPT
		XEND
		
		return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
{
	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32, actualSize;
	aafRational_t	valueRat;
	aafUID_t		opcode;
	aafUInt8		buf[256];
	aafUInt8		header[STD_HDRSIZE_NODATA];
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
			else if(EqualAUID(&kAAFSampleFormat, &opcode))
			{
				valueUInt32 = kAAFSignedMagnitude;
				memcpy(buf, &valueUInt32, sizeof(valueUInt32));
				CHECK(fmt->AddFormatSpecifier (kAAFSampleFormat, sizeof(valueUInt32), buf));
			}
			else if(EqualAUID(&kAAFNumChannels, &opcode))
			{
				valueUInt32 = _numCh;
				memcpy(buf, &valueUInt32, sizeof(valueUInt32));
				CHECK(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(valueUInt32), buf));
			}
			else if(EqualAUID(&kAAFMaxSampleBytes, &opcode))
			{
				valueUInt32 = (_bitsPerSample + 7)/ 8;
				memcpy(buf, &valueUInt32, sizeof(valueUInt32));
				CHECK(fmt->AddFormatSpecifier (kAAFMaxSampleBytes, sizeof(valueUInt32), buf));
			}
			else
				RAISE(AAFRESULT_INVALID_OP_CODEC);
		}
		
		CHECK(CreateAIFCheader(header, STD_HDRSIZE_NODATA, _numCh, &actualSize));
		CHECK(_mdes->SetSummary (actualSize, header));
		
		// This will output the header on the next non-raw write
		_sampleDataHeaderWritten = kAAFFalse;
	}
	XEXCEPT
		XEND
		
		return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
{
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
		//		CHECK(fmt->AddFormatSpecifier (kAAFSampleFormat, 0, NULL));
		parm = defaultNumCh;
		CHECK(fmt->AddFormatSpecifier (kAAFNumChannels, sizeof(parm), (aafDataBuffer_t)&parm));
		//		CHECK(fmt->AddFormatSpecifier (kAAFMaxSampleBytes, 0, NULL));
	}
	XEXCEPT
		XEND
		
		return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetIndexedSampleSize
(aafUID_constref dataDefID,
 aafPosition_t pos,
 aafLength_t *pResult)
{
	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(pos < 0 || pos >=_sampleFrames)
		return(AAFRESULT_EOF);
	
	if(EqualAUID(&dataDefID, &DDEF_Sound))
		*pResult = _bytesPerFrame;
	else
		return(AAFRESULT_CODEC_CHANNELS);
	
	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::GetLargestSampleSize (aafUID_constref dataDefID,
									 aafLength_t *pResult)
{
	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(EqualAUID(&dataDefID, &DDEF_Sound))
		*pResult = _bytesPerFrame;
	else
		return(AAFRESULT_CODEC_CHANNELS);
	return (AAFRESULT_SUCCESS);
}



HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::MultiCreate (IAAFSourceMob *unk,
							aafUID_constref /*flavour*/,
							IAAFEssenceStream * stream,
							aafCompressEnable_t compEnable,
							aafUInt32 numParms,
							aafmMultiCreate_t *createParms)
{
	IAAFSourceMob			*fileMob = NULL;
	IAAFEssenceDescriptor	*mdes = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	unsigned char			header[STD_HDRSIZE_NODATA];
	aafUInt32				actualSize;

	if(_stream == NULL)
	{
		_stream = stream;
		_stream->AddRef();
	}
	_readOnly = kAAFFalse;
	XPROTECT()
	{
		_sampleRate = createParms[0].sampleRate;	// !!!Assumes all sample rates are the same
		
		// whether or not we will be compressing the samples as they are read.
		CHECK(SetCompressionEnabled(compEnable));
		
		CHECK(unk->QueryInterface(IID_IAAFSourceMob, (void **)&fileMob));
		CHECK(fileMob->GetEssenceDescriptor(&mdes));
		fileMob->Release();
		fileMob = NULL;
		CHECK(mdes->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&_mdes));
		CHECK(CreateAIFCheader(header, STD_HDRSIZE_NODATA, (aafUInt16)numParms, &actualSize));
		CHECK(_mdes->SetSummary (actualSize, header));
		CHECK(mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
		mdes->Release();
		fileDesc->Release();
		
		//!!!		aafsCvtInt32toInt64(0, &pdata->formSizeOffset);
		//		aafsCvtInt32toInt64(0, &pdata->numSamplesOffset);
		
	}
	XEXCEPT
	{
		if(fileMob != NULL)
			fileMob->Release();
		if(mdes != NULL)
			mdes->Release();
		if(fileDesc != NULL)
			fileDesc->Release();
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
CAAFAIFCCodec::MultiOpen (IAAFSourceMob *unk,
						  aafMediaOpenMode_t  openMode,
						  IAAFEssenceStream * stream,
						  aafCompressEnable_t compEnable)
{
	IAAFSourceMob	*fileMob = NULL;
	IAAFEssenceDescriptor *edes = NULL;
	
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
		CHECK(fileMob->GetEssenceDescriptor(&edes));
		CHECK(edes->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&_mdes));
		
		if(!_headerLoaded)
		{
			CHECK(loadAIFCHeader());
		}
		fileMob->Release();
		fileMob = NULL;
		edes->Release();
		edes = NULL;
	}
	XEXCEPT
	{
		if(fileMob != NULL)
			fileMob->Release();
		if(edes != NULL)
			edes->Release();
	}
	XEND;
	return AAFRESULT_SUCCESS;
}


AAFRESULT CAAFAIFCCodec::SetCompressionEnabled (aafCompressEnable_t /*compEnable*/)
{
	return HRESULT_SUCCESS;
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
AAFRESULT CAAFAIFCCodec::fillSwappedAIFCData(aafUInt8 **destBufHdl, aafUInt32 maxsize, void *data)
{
	aafInt32        dataL;
	aafInt16        dataS;
	aafUInt8		*destBufPtr = *destBufHdl;
	
	if ((maxsize == sizeof(aafInt32)) && (MOTOROLA_ORDER != _nativeByteOrder))
	{
		dataL = *((aafInt32 *) data);
		AAFByteSwap32(&dataL);
		data = &dataL;
	} else if ((maxsize == sizeof(aafInt16)) && (MOTOROLA_ORDER != _nativeByteOrder))
	{
		dataS = *((aafInt16 *) data);
		AAFByteSwap16(&dataS);
		data = &dataS;
	}
	memcpy(destBufPtr, data, maxsize);
	*destBufHdl = destBufPtr + maxsize;
	
	return (AAFRESULT_SUCCESS);
}

AAFRESULT CAAFAIFCCodec::scanSwappedAIFCData(aafUInt8 **srcBufHdl, aafUInt32 maxsize, void *data)
{
	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;
	
	if ((maxsize == sizeof(aafInt32)) && (MOTOROLA_ORDER != _nativeByteOrder))
		AAFByteSwap32((aafInt32 *) data);
	else if ((maxsize == sizeof(aafInt16)) && (MOTOROLA_ORDER != _nativeByteOrder))
		AAFByteSwap16((aafInt16 *) data);
	
	return (AAFRESULT_SUCCESS);
}

AAFRESULT CAAFAIFCCodec::GetAIFCData(aafUInt32 len, void *buf)
{
	aafUInt32	bytesRead;
	
	XPROTECT()
	{
		CHECK(_stream->Read(len, (aafUInt8 *)buf, &bytesRead));
		if(bytesRead != len)
			RAISE(AAFRESULT_END_OF_DATA);
		if ((len == sizeof(aafInt32)) && (MOTOROLA_ORDER != _nativeByteOrder))
			AAFByteSwap32((aafInt32 *) buf);
		else if ((len == sizeof(aafInt16)) && (MOTOROLA_ORDER != _nativeByteOrder))
			AAFByteSwap16((aafInt16 *) buf);
	}
	XEXCEPT
		XEND;
	
	return(AAFRESULT_SUCCESS);
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
/*
* Creates AIFC information in the given object. If the object is a
* descriptor (WAVD), don't create the sound data chunk.
* 
*/

AAFRESULT CAAFAIFCCodec::CreateAIFCheader(aafUInt8		*buffer,
										  aafUInt32	bufsize,
										  aafUInt16	numCh,
										  aafUInt32	*actualSize)
										  
{
	aafInt32           zero = 0, len;
	aafInt16           pcm_format = 1;
	aafUInt8			*ptr = buffer, *COMMLenBuf, *FORMLenBuf, *patch;
	char            	tempstr[12];
	double      		tempdbl;
	aafInt16      		bps;
	char         		ss;
	
	XPROTECT()
	{	
		_numCh = numCh;
		if(bufsize < STD_HDRSIZE_NODATA)
			RAISE(AAFRESULT_SMALLBUF);
		
		*ptr++ = 'F';		// Make sure that this is NOT byte-swapped
		*ptr++ = 'O';
		*ptr++ = 'R';
		*ptr++ = 'M';
		
		FORMLenBuf = ptr;
		ptr += 4;			// Patch form length in later
		
		*ptr++ = 'A';
		*ptr++ = 'I';
		*ptr++ = 'F';
		*ptr++ = 'C';
		*ptr++ = 'C';
		*ptr++ = 'O';
		*ptr++ = 'M';
		*ptr++ = 'M';
		
		COMMLenBuf = ptr;
		CHECK(fillSwappedAIFCData(&ptr, 4L, (void *) &zero));		// Chunk Size
		CHECK(fillSwappedAIFCData(&ptr, 2L, (void *) &numCh));
		_numSamplesOffset = ptr - buffer;						// Save for later
		CHECK(fillSwappedAIFCData(&ptr, 4L, (void *) &zero));		// NumSamples
		bps = _bitsPerSample;
		CHECK(fillSwappedAIFCData(&ptr, 2L, (void *) &bps));
		tempdbl = FloatFromRational(_sampleRate);
		ConvertToIeeeExtended(tempdbl, tempstr);
		CHECK(fillSwappedAIFCData(&ptr, 10L, (void *) tempstr));
		*ptr++ = 'N';
		*ptr++ = 'O';
		*ptr++ = 'N';
		*ptr++ = 'E';
		ss = 14;
		CHECK(fillSwappedAIFCData(&ptr, 1L, (void *) &ss));
		CHECK(fillSwappedAIFCData(&ptr, 14L, (void *) "Not Compressed"));
		CHECK(fillSwappedAIFCData(&ptr, 1L, (void *) &zero));
	
		/* COMM chunk done, so patch up size field */
		len = ptr-(COMMLenBuf+4);
		patch = COMMLenBuf;
		CHECK(fillSwappedAIFCData(&patch, 4L, (void *) &len));
		// Patch FORM size.  If data is written is will be re-patched by CompleteWrite()
		len = ptr-(FORMLenBuf+4);
		patch = FORMLenBuf;
		CHECK(fillSwappedAIFCData(&patch, 4L, (void *) &len));

		*actualSize = ptr - buffer;
	}
	XEXCEPT
	XEND
		
	return (AAFRESULT_SUCCESS);
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
#define DEBUG_READ 1
AAFRESULT CAAFAIFCCodec::loadAIFCHeader(void)
{
	aafInt32			offset, chunkStart, formSize, ssndOffset, ssndBlocksize;
	aafInt64			chunkStart64;
	aafUInt8            chunkID[4];
	aafUInt32			chunkSize, bytesRead;
	aafBoolean_t				fmtFound = kAAFFalse, dataFound = kAAFFalse;
	aafInt64			savePos;
	char				ieeerate[10];
#if DEBUG_READ
	aafUInt8			debugBuf[64];
#endif
	
	if(_headerLoaded)
		return AAFRESULT_SUCCESS;
	
	XPROTECT()
	{
		CHECK(_stream->GetPosition (&savePos));
		
#if DEBUG_READ
		CHECK(_stream->Seek(0L));
		_stream->Read(64L, debugBuf, &bytesRead);
#endif
		CHECK(_stream->Seek(0L));
		CHECK(_stream->Read(4L, chunkID, &bytesRead));
		if (memcmp(&chunkID, "FORM", (size_t) 4) != 0)
			RAISE(AAFRESULT_BADAIFCDATA);
		CHECK(GetAIFCData(4L, (void *) &formSize));	
		CHECK(_stream->Read(4L, chunkID, &bytesRead));
		if (memcmp(&chunkID, "AIFC", (size_t) 4) != 0)
			RAISE(AAFRESULT_BADAIFCDATA);
		CHECK(_stream->GetPosition(&chunkStart64));
		CHECK(TruncInt64toInt32(chunkStart64, &offset));	// OK - 32-bit format
		
		while ((offset < formSize) && _stream->Read(4L, chunkID, &bytesRead) == AAFRESULT_SUCCESS)
		{
			CHECK(GetAIFCData(4L, (void *) &chunkSize));	
			CHECK(_stream->GetPosition(&chunkStart64));
			
			if (memcmp(&chunkID, "COMM", (size_t) 4) == 0)
			{
				/* AIFC field: numChannels */
				CHECK(GetAIFCData(2L, &_numCh));

				/* AIFC field: numSampleFrames */
				CHECK(GetAIFCData(4L, &_sampleFrames));

				/* AIFC field sampleSize */
				CHECK(GetAIFCData(2L, &_bitsPerSample));

				/* AIFC field: sampleRate */	// Need extended float code!!!
				CHECK(GetAIFCData(sizeof(ieeerate), &ieeerate));
				_sampleRate = RationalFromFloat(
										ConvertFromIeeeExtended(ieeerate));
				
				fmtFound = kAAFTrue;
			} else if (memcmp(&chunkID, "SSND", (size_t) 4) == 0)
			{
				/* Positioned at beginning of audio data */
				CHECK(_stream->GetPosition(&_dataStartOffset));
				CHECK(GetAIFCData(4L, (void *) &ssndOffset));
				CHECK(GetAIFCData(4L, (void *) &ssndBlocksize));
				/* Skip over ssndoffset and blocksize */
				_dataStartOffset += (4+4+ssndOffset);
				
				dataFound = kAAFTrue;
			}
			CHECK(TruncInt64toInt32(chunkStart64, &chunkStart));	// OK - 32-bit format
			offset = chunkStart + chunkSize;
			
			if(offset > formSize)
				break;
			if (fmtFound && dataFound)	/* Do we have all information yet? */
				break;
			CHECK(_stream->Seek(offset));
		}
		_headerLoaded = kAAFTrue;
		CHECK(_stream->Seek (savePos));
	}
	XEXCEPT
	{
		RERAISE(AAFRESULT_BADAIFCDATA);
	}
	XEND
		
		
		return(AAFRESULT_SUCCESS);
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
AAFRESULT CAAFAIFCCodec::ComputeWriteChunkSize(
											   aafInt64 sizeOff, 
											   aafInt64 end)
{
	aafInt64          tmpOffset, savePos, result;
	aafUInt32          size, bytesWritten;
	aafUInt8			*tmp;
	// BobT: buf[] only *really* needs to be pointer-sized, but we're
	// adding 1 here to keep BoundsChecker happy
	aafUInt8			buf[sizeof(void*) + 1];
#if DEBUG_READ
	aafUInt8			debugBuf[64];
	aafUInt32			bytesRead;
#endif
	
	XPROTECT()
	{
		CHECK(_stream->GetPosition(&savePos));
		
		tmpOffset = sizeOff;
		CHECK(AddInt32toInt64(4L, &tmpOffset));
		result = end;
		CHECK(SubInt64fromInt64(tmpOffset, &result));
		CHECK(TruncInt64toUInt32(result, &size));	/* OK AIFC */
#if DEBUG_READ
		CHECK(_stream->Seek(0L));
		_stream->Read(64L, debugBuf, &bytesRead);
#endif
		CHECK(_stream->Seek(sizeOff));
		tmp = buf;
		CHECK(fillSwappedAIFCData(&tmp, 4L, &size));
		CHECK(_stream->Write(4L, buf, &bytesWritten));
		
		CHECK(_stream->Seek(savePos));
	}
	XEXCEPT
		XEND
		
		return (AAFRESULT_SUCCESS);
}

AAFRESULT CAAFAIFCCodec::CreateAudioDataEnd(void)
{
	aafInt64    curOffset;
	aafUInt8	buf[sizeof(void*) + 1];
	aafUInt32	size, bytesWritten;
	aafUInt8	*tmp;
	
	XPROTECT()
	{
		/* this routine will be called after sample data is written */
		/* Now set the patches for all the fields of the AIFC data */
		CHECK(_stream->GetPosition(&curOffset));
		
		CHECK(ComputeWriteChunkSize(4, curOffset));
		CHECK(ComputeWriteChunkSize(_dataSizeOffset, curOffset));
		// +12 = 4 byte chunk length 8 bytes of SSND chunk header data (offset and blockSize)
		size = (aafUInt32)(curOffset - (_dataSizeOffset+12))/_bytesPerFrame;	/* OK AIFC */
		tmp = buf;
		CHECK(fillSwappedAIFCData(&tmp, 4L, &size));
		CHECK(_stream->Seek(_numSamplesOffset));
		CHECK(_stream->Write(4L, buf, &bytesWritten));
		CHECK(_stream->Seek(curOffset));
	}
	XEXCEPT
		XEND
		
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
static void SplitBuffers(void *original, aafUInt32 srcSamples, aafUInt16 sampleSize, aafUInt16 numDest, interleaveBufAIFF_t *destPtr)
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
HRESULT CAAFAIFCCodec::InternalQueryInterface
(
 REFIID riid,
 void **ppvObj)
{
    HRESULT hr = S_OK;
	
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

/**************************** Start of IEEE convert code *************************/

/* Moved math.h down here to make NEXT's compiler happy */
#include <math.h>

/*
 * C O N V E R T   T O   I E E E   E X T E N D E D
 * 
 * C O N V E R T   F R O M   I E E E   E X T E N D E D
 */

/*
 * Copyright (C) 1988-1991 Apple Computer, Inc. All rights reserved.
 * 
 * Warranty Information Even though Apple has reviewed this software, Apple
 * makes no warranty or representation, either express or implied, with
 * respect to this software, its quality, accuracy, merchantability, or
 * fitness for a particular purpose.  As a result, this software is provided
 * "as is," and you, its user, are assuming the entire risk as to its quality
 * and accuracy.
 * 
 * This code may be used and freely distributed as long as it includes this
 * copyright notice and the above warranty information.
 * 
 * Machine-independent I/O routines for IEEE floating-point numbers.
 * 
 * NaN's and infinities are converted to HUGE_VAL or HUGE, which happens to be
 * infinity on IEEE machines.  Unfortunately, it is impossible to preserve
 * NaN's in a machine-independent way. Infinities are, however, preserved on
 * IEEE machines.
 * 
 * These routines have been tested on the following machines: Apple Macintosh,
 * MPW 3.1 C compiler Apple Macintosh, THINK C compiler Silicon Graphics
 * IRIS, MIPS compiler Cray X/MP and Y/MP Digital Equipment VAX
 * 
 * 
 * Implemented by Malcolm Slaney and Ken Turkowski.
 * 
 * Malcolm Slaney contributions during 1988-1990 include big- and little- endian
 * file I/O, conversion to and from Motorola's extended 80-bit floating-point
 * format, and conversions to and from IEEE single- precision floating-point
 * format.
 * 
 * In 1991, Ken Turkowski implemented the conversions to and from IEEE
 * double-precision format, added more precision to the extended conversions,
 * and accommodated conversions involving +/- infinity, NaN's, and
 * denormalized numbers.
 */

/* #ifndef  PORT_MEM_DOS16     */

#ifndef HUGE_VAL
# define HUGE_VAL HUGE
#endif				/* HUGE_VAL */

# define FloatToUnsigned(f)  \
    ((aafUInt32)((((aafInt32)(f - 2147483648.0)) + 2147483647L) + 1))

# define UnsignedToFloat(u)    \
  (((double)((aafInt32)(u - 2147483647L - 1))) + 2147483648.0)

/* #endif   PORT_MEM_DOS16 */
/****************************************************************
 * Extended precision IEEE floating-point conversion routine.
 ****************************************************************/

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
static void ConvertToIeeeExtended(double num, char *bytes)
{

#if 1
	int             sign;
	int             expon;
	double          fMant, fsMant;
	aafUInt32          hiMant, loMant;

	if (num < 0)
	{
		sign = 0x8000;
		num *= -1;
	} else
	{
		sign = 0;
	}

	if (num == 0)
	{
		expon = 0;
		hiMant = 0;
		loMant = 0;
	} else
	{
		fMant = frexp(num, &expon);
		if ((expon > 16384) || !(fMant < 1))
		{		/* Infinity or NaN */
			expon = sign | 0x7FFF;
			hiMant = 0;
			loMant = 0;	/* infinity */
		} else
		{		/* Finite */
			expon += 16382;
			if (expon < 0)
			{	/* denormalized */
				fMant = ldexp(fMant, expon);
				expon = 0;
			}
			expon |= sign;
			fMant = ldexp(fMant, 32);
			fsMant = floor(fMant);
			hiMant = (aafUInt32)FloatToUnsigned(fsMant);
			fMant = ldexp(fMant - fsMant, 32);
			fsMant = floor(fMant);
			loMant = (aafUInt32)FloatToUnsigned(fsMant);
		}
	}

	bytes[0] = expon >> 8;
	bytes[1] = expon;
	bytes[2] = (char)(hiMant >> 24L);
	bytes[3] = (char) (hiMant >> 16L);
	bytes[4] = (char) (hiMant >> 8L);
	bytes[5] = (char) hiMant;
	bytes[6] = (char) (loMant >> 24L);
	bytes[7] = (char) (loMant >> 16L);
	bytes[8] = (char)( loMant >> 8L);
	bytes[9] = (char) loMant;
#else				/* Little-endian: long double in PC is IEEE
				 * 80 bit */

	long double     x;
	unsigned char  *p = (unsigned char *) &x;
	int             i;

	x = (long double) num;
	// flop the bytes
	for (i = 0; i < 10; i++)
		bytes[i] = *(p + 9 - i);

#endif				/* ifndef  PORT_MEM_DOS16 */
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
static double ConvertFromIeeeExtended(char *bytes)
{
	double          f;
	aafInt32           expon;
	aafUInt32          hiMant, loMant;

	expon = ((bytes[0] & 0x7F) << 8) | (bytes[1] & 0xFF);
	hiMant = ((aafUInt32) (bytes[2] & 0xFF) << 24)
		| ((aafUInt32) (bytes[3] & 0xFF) << 16)
		| ((aafUInt32) (bytes[4] & 0xFF) << 8)
		| ((aafUInt32) (bytes[5] & 0xFF));
	loMant = ((aafUInt32) (bytes[6] & 0xFF) << 24)
		| ((aafUInt32) (bytes[7] & 0xFF) << 16)
		| ((aafUInt32) (bytes[8] & 0xFF) << 8)
		| ((aafUInt32) (bytes[9] & 0xFF));

	if (expon == 0 && hiMant == 0 && loMant == 0)
	{
		f = 0;
	} else
	{
		if (expon == 0x7FFF)
		{		/* Infinity or NaN */
			f = HUGE_VAL;
		} else
		{
			expon -= 16383;
			f = ldexp(UnsignedToFloat(hiMant), (int) (expon -= 31));
			f += ldexp(UnsignedToFloat(loMant), (int) (expon -= 32));
		}
	}

	if (bytes[0] & 0x80)
		return -f;
	else
		return f;
}
/**************************** End of IEEE convert code *************************/

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFAIFCCodec)
