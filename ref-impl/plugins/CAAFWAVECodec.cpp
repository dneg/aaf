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
#include "CAAFWAVECodec.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

//#include "CAAFEssenceFormat.h"
//#include "ImplAAFEssenceFormat.h"
#include "aafErr.h"
#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

#include "CAAFBuiltinDefs.h"


#define HEADER_BUFSIZE			2000			// Big enough to hold ANY single-entry coding history

const aafProductVersion_t AAFPluginImplementationVersion = {1, 0, 0, 1, kAAFVersionBeta};
const aafRational_t		defaultRate = { 44100, 1 };
const aafUInt32			defaultSampleWidth = 8;
const aafUInt32			defaultNumCh = 1;

// CLSID for AAFEssenceCodec 
//{8D7B04B1-95E1-11d2-8089-006008143E6F}
const CLSID CLSID_AAFWaveCodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

const aafUID_t JEFFS_WAVE_PLUGIN = { 0x431D5CA1, 0xEDE2, 0x11d2, { 0x80, 0x9F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static void SplitBuffers(void *original, aafUInt32 srcSamples, aafUInt16 sampleSize, aafUInt16 numDest, interleaveBuf_t *destPtr);

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::CountDefinitions (aafUInt32 *pDefCount)
{
	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedDefinitionID (aafUInt32 index, aafUID_t *uid)
{
	if(uid == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index > 0)
		return AAFRESULT_BADINDEX;
	*uid = kAAFCodecWAVE;		// UID of the WAVE codec definition
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetPluginDescriptorID (aafUID_t *uid)
{
	*uid = JEFFS_WAVE_PLUGIN;		// UID of the PluginDescriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetEssenceDescriptorID (aafUID_t *uid)
{
	*uid = AUID_AAFWAVEDescriptor;		// stored class UID of the WAVE Decriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetEssenceDataID (aafUID_t *uid)
{
	*uid = AUID_AAFEssenceData;			// stored class UID of the WAVE data object
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedDefinitionObject (aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
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
		uid = kAAFCodecWAVE;
		CHECK(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(codecDef->Initialize(uid, L"WAVE Codec", L"Handles RIFF WAVE data."));
		CAAFBuiltinDefs defs (dict);
		CHECK(codecDef->AddEssenceKind (defs.ddSound()));
	  	CHECK(dict->LookupClassDef(AUID_AAFWAVEDescriptor, &fileClass));
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
    CAAFWaveCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
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
		CHECK(desc->Initialize(JEFFS_WAVE_PLUGIN, L"Example WAVE Codec", L"Handles RIFF WAVE data."));

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


CAAFWaveCodec::CAAFWaveCodec (IUnknown * pControllingUnknown)
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


CAAFWaveCodec::~CAAFWaveCodec ()
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
    CAAFWaveCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	if(access == NULL)
		return AAFRESULT_NULL_PARAM;

	_access = access;

  return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::CountFlavours(aafUInt32 *pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedFlavourID (aafUInt32  index,
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
    CAAFWaveCodec::CountDataDefinitions (aafUInt32 *pDefCount)
{
	//!!!Add error checking
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedDataDefinition (aafUInt32  /*index*/,
        aafUID_t * pDataDefID)
{
  if (! pDataDefID)
	return AAFRESULT_NOT_IMPLEMENTED;

  return HRESULT_NOT_IMPLEMENTED;
}

//!!!Need a way to convert between CLSID and aafUID_t
//!!!Change this to stored object IDs
const aafUID_t LOCAL_AAFWAVEDescriptor = { 0x4c2e1691, 0x8ae6, 0x11d2, { 0x81, 0x3c, 0x00, 0x60, 0x97, 0x31, 0x01, 0x72 } };
const aafUID_t LOCAL_AAFEssenceData = { 0x6a33f4e1, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const wchar_t	name[] = L"WAVE Codec";

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetMaxCodecDisplayNameLength (
        aafUInt32  *bufSize)
{
	if(bufSize == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*bufSize = sizeof(name)/sizeof(wchar_t);
	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetCodecDisplayName (aafUID_constref /*flavour*/,
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
    CAAFWaveCodec::CountChannels (IAAFSourceMob * /*fileMob*/,
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
				CHECK(loadWAVEHeader());
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
    CAAFWaveCodec::GetSelectInfo (IAAFSourceMob * /*fileMob*/,
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
			CHECK(loadWAVEHeader());
		}
		pSelectInfo->willHandleMDES = kAAFTrue;
#if PORT_BYTESEX_LITTLE_ENDIAN
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
    CAAFWaveCodec::CountSamples (
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
    CAAFWaveCodec::ValidateEssence (IAAFSourceMob * /*fileMob*/,
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
CAAFWaveCodec::Create (IAAFSourceMob *unk,
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
    CAAFWaveCodec::Open (IAAFSourceMob *unk,
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
    CAAFWaveCodec::WriteSamples (aafUInt32  nSamples,
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

  // If there multiple channels in the WAVE file and the data is interleaved
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
    CAAFWaveCodec::WriteBlocks (aafDeinterleave_t  inter,
        aafUInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	aafUInt32      	fileBytes;
	aafUInt32      	n, xfers, samp;
	aafUInt32		maxSamplesLeft, headerSize;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result;
	interleaveBuf_t	*interPtr;
	aafUInt8		*destPtr;
	aafUInt32		zero = 0;
	aafUInt8		sampleBuf[256];
  aafUInt32 bytesWritten;
	unsigned char	header[HEADER_BUFSIZE];


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
			CHECK(CreateWAVEheader(header, HEADER_BUFSIZE, _numCh, &headerSize));

			_stream->Seek(0);
			_sampleDataHeaderWritten = kAAFTrue;
			// The next four lines won't work for raw writes
			_stream->Write(headerSize, header, &bytesWritten);
			_stream->Write(4, (aafUInt8 *)"data", &bytesWritten);
			_stream->GetPosition(&_dataSizeOffset);
			_stream->Write(4, (aafUInt8 *)&zero, &bytesWritten);		
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
				_interleaveBuf = new interleaveBuf_t[_numCh];
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
    CAAFWaveCodec::ReadSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  samplesRead,
        aafUInt32 *  bytesRead)
{
  HRESULT hr = S_OK;

  // If there multiple channels in the WAVE file and the data is interleaved
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
    CAAFWaveCodec::ReadBlocks (aafDeinterleave_t  inter,
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
				_interleaveBuf = new interleaveBuf_t[_numCh];
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
    CAAFWaveCodec::Seek (aafPosition_t  sampleFrame)
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
    CAAFWaveCodec::CompleteWrite (IAAFSourceMob *fileMob)
{
	aafInt64				sampleLen;
	aafUInt32				WAVEDataLen;
	IAAFEssenceDescriptor	*essenceDesc = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	IAAFWAVEDescriptor		*waveDesc = NULL;
	aafDataValue_t			buf = NULL;

	XPROTECT()
	{
		if(!_readOnly && _sampleDataHeaderWritten)
			CHECK(CreateAudioDataEnd());	// Don't do this for raw calls?

		CHECK(_stream->Seek(_dataSizeOffset));
		CHECK(GetWAVEData(sizeof(WAVEDataLen), &WAVEDataLen));	// Read the AIFC data length
		sampleLen = WAVEDataLen / _bytesPerFrame;
		CHECK(_mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
		CHECK(fileDesc->SetLength(sampleLen));
		fileDesc->Release();
		fileDesc = NULL;

//		_stream = NULL;
		
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
			CHECK(essenceDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&waveDesc));
			essenceDesc->Release();
			essenceDesc = NULL;
			CHECK(_mdes->GetSummaryBufferSize (&bufsiz));
			buf = new aafUInt8[bufsiz];
			if(buf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			CHECK(_mdes->GetSummary (bufsiz, buf));
			CHECK(waveDesc->SetSummary (bufsiz, buf));
			waveDesc->Release();
			waveDesc = NULL;
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
		if(waveDesc != NULL)
			waveDesc->Release();

		if(buf != NULL)
			delete [] buf;
	}
	XEND;

	return HRESULT_SUCCESS;
}		



HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::WriteRawData (aafUInt32 nSamples, aafDataBuffer_t  buffer,
        aafUInt32  buflen)
{
  aafUInt32 bytesWritten;
  
  	if(buflen < (nSamples * _bytesPerFrame))
  		return AAFRESULT_SMALLBUF;
	return _stream->Write (nSamples * _bytesPerFrame, buffer, &bytesWritten);
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::ReadRawData (aafUInt32 nSamples,
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
    CAAFWaveCodec::CreateDescriptorFromStream (IAAFEssenceStream *  /*pStream*/,
        IAAFSourceMob * /*fileMob*/)
{
	return(AAFRESULT_NOT_IMPLEMENTED);
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
	*ppStream = _stream;
	_stream->AddRef();
	return AAFRESULT_SUCCESS;
}

		
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32, headerSize;
	aafRational_t	valueRat;
	aafUID_t		opcode;
	aafUInt8		buf[256];
	aafUInt8		header[HEADER_BUFSIZE];

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
		
		CHECK(CreateWAVEheader(header, HEADER_BUFSIZE, _numCh, &headerSize));
		CHECK(_mdes->SetSummary (headerSize, header));
		
		// This will output the header on the next non-raw write
		_sampleDataHeaderWritten = kAAFFalse;
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
{
	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32, headerSize;
	aafRational_t	valueRat;
	aafUID_t		opcode;
	aafUInt8		buf[256];
	aafUInt8		header[HEADER_BUFSIZE];
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
		
		CHECK(CreateWAVEheader(header, HEADER_BUFSIZE, _numCh, &headerSize));
		CHECK(_mdes->SetSummary (headerSize, header));
		
		// This will output the header on the next non-raw write
		_sampleDataHeaderWritten = kAAFFalse;
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetDefaultEssenceFormat(IAAFEssenceFormat **pResult)
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
    CAAFWaveCodec::GetIndexedSampleSize
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
    CAAFWaveCodec::GetLargestSampleSize (aafUID_constref dataDefID,
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
    CAAFWaveCodec::MultiCreate (IAAFSourceMob *unk,
        aafUID_constref /*flavour*/,
        IAAFEssenceStream * stream,
        aafCompressEnable_t compEnable,
        aafUInt32 numParms,
        aafmMultiCreate_t *createParms)
{
	IAAFSourceMob			*fileMob = NULL;
	IAAFEssenceDescriptor	*mdes = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	unsigned char			header[HEADER_BUFSIZE];
	aafUInt32				headerSize;

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
		CHECK(mdes->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&_mdes));
		CHECK(CreateWAVEheader(header, HEADER_BUFSIZE, (aafUInt16)numParms, &headerSize));
		CHECK(_mdes->SetSummary (headerSize, header));
		CHECK(mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
		mdes->Release();
		fileDesc->Release();

//!!!		omfsCvtInt32toInt64(0, &pdata->formSizeOffset);
//		omfsCvtInt32toInt64(0, &pdata->numSamplesOffset);

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
    CAAFWaveCodec::MultiOpen (IAAFSourceMob *unk,
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
		CHECK(edes->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&_mdes));

		if(!_headerLoaded)
		{
			CHECK(loadWAVEHeader());
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


AAFRESULT CAAFWaveCodec::SetCompressionEnabled (aafCompressEnable_t /*compEnable*/)
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
AAFRESULT CAAFWaveCodec::fillSwappedWAVEData(aafUInt8 **destBufHdl, aafUInt32 maxsize, void *data)
{
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

AAFRESULT CAAFWaveCodec::scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafUInt32 maxsize, void *data)
{
	memcpy(data, *srcBufHdl, maxsize);
	(*srcBufHdl) += maxsize;
	
	if ((maxsize == sizeof(aafInt32)) && (INTEL_ORDER != _nativeByteOrder))
			AAFByteSwap32((aafInt32 *) data);
	else if ((maxsize == sizeof(aafInt16)) && (INTEL_ORDER != _nativeByteOrder))
			AAFByteSwap16((aafInt16 *) data);

	return (AAFRESULT_SUCCESS);
}

AAFRESULT CAAFWaveCodec::GetWAVEData(aafUInt32 len, void *buf)
{
	aafUInt32	bytesRead;
	
	XPROTECT()
	{
		CHECK(_stream->Read(len, (aafUInt8 *)buf, &bytesRead));
		if(bytesRead != len)
			RAISE(AAFRESULT_END_OF_DATA);
		if ((len == sizeof(aafInt32)) && (INTEL_ORDER != _nativeByteOrder))
			AAFByteSwap32((aafInt32 *) buf);
		else if ((len == sizeof(aafInt16)) && (INTEL_ORDER != _nativeByteOrder))
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
 * Creates WAVE information in the given object. If the object is a
 * descriptor (WAVD), don't create the sound data chunk.
 * 
 */

AAFRESULT CAAFWaveCodec::CreateWAVEheader(aafUInt8		*buffer,
								aafUInt32	bufsize,
								aafUInt16	numCh,
								aafUInt32	*bytesWritten)

{
	aafInt32           chunksize, avgBytesPerSec, samplesPerSec;
	aafUInt32			headerSize;
	aafInt32           zero = 0, len;
	aafInt16           bytesPerFrame;
	aafInt16           pcm_format = 1;
	aafUInt8			*ptr = buffer, *nextChunk;
#if INCLUDE_BEXT
	aafUInt32			bextSize, headerSize, n;
	char				bextDescription[256+1];
	char				bextOriginator[32+1];
	char				bextOriginatorReference[32+1];
	char				bextOriginationDate[10+1];
	char				bextOriginationTime[8+1];
	aafInt64			bextTimeReference;
	aafUInt16			bextVersion;
	char				bextReserved[254];
	char				bextCodingHistory[512];
	aafUInt16			bextLenCodingHistory;
	aafTimeStamp_t		dateTime;
#endif

	XPROTECT()
	{	
#if INCLUDE_BEXT
		if(_sampleRate.denominator != 0)
		{
			sprintf(bextCodingHistory, "PCM: %s, %d bits, %g hz\r\n", (_numCh == 2 ? "stereo" : "mono"),
				_bitsPerSample, (double)_sampleRate.numerator / (double)_sampleRate.denominator);
		}
		else
		{
			sprintf(bextCodingHistory, "PCM: %s, %d bits", (numCh == 2 ? "stereo" : "mono"), _bitsPerSample);
		}
		bextLenCodingHistory = strlen(bextCodingHistory)+1;
		bextSize = 256+32+32+10+8+8+2+sizeof(bextReserved)+bextLenCodingHistory;
		headerSize = 36 + bextSize;
#else
		headerSize = 36;
#endif

		_numCh = numCh;
		if(bufsize < headerSize)
			RAISE(AAFRESULT_SMALLBUF);

		*ptr++ = 'R';		// Make sure that this is NOT byte-swapped
		*ptr++ = 'I';
		*ptr++ = 'F';
		*ptr++ = 'F';
	
		ptr += 4;			// Patch form length in later

		*ptr++ = 'W';
		*ptr++ = 'A';
		*ptr++ = 'V';
		*ptr++ = 'E';
		*ptr++ = 'f';
		*ptr++ = 'm';
		*ptr++ = 't';
		*ptr++ = ' ';

		chunksize = 16;		/* WAVE format size is fixed */
		CHECK(fillSwappedWAVEData(&ptr, 4L, (void *) &chunksize));
		CHECK(fillSwappedWAVEData(&ptr, 2L, (void *) &pcm_format));
		CHECK(fillSwappedWAVEData(&ptr, 2L, (void *) &numCh));
	
		if (_sampleRate.denominator != 0)
			samplesPerSec = (long) FloatFromRational(_sampleRate);
		else
			samplesPerSec = 0;
		CHECK(fillSwappedWAVEData(&ptr, 4L, (void *) &samplesPerSec));
	
		bytesPerFrame = ((_bitsPerSample + 7) / 8) * numCh;
		avgBytesPerSec = samplesPerSec * bytesPerFrame;
		CHECK(fillSwappedWAVEData(&ptr, 4L, (void *) &avgBytesPerSec));
	
		CHECK(fillSwappedWAVEData(&ptr, 2L, (void *) &bytesPerFrame));
		CHECK(fillSwappedWAVEData(&ptr, 2L, (void *) &_bitsPerSample));
		// !!!Postcondition to make sure that offset within

		/* patch FORM size here. */
		nextChunk = ptr;
		len = ptr - buffer;
		ptr = buffer+ 4;
		CHECK(fillSwappedWAVEData(&ptr, 4L, &len));	// Patch in

#if INCLUDE_BEXT
		// Set up for the broadcast chunk
		memset(bextDescription, 0, sizeof(bextDescription));
		sprintf(bextDescription, "<tbd>");
		/***/
		memset(bextOriginator, 0, sizeof(bextOriginator));
		sprintf(bextOriginator, "<tbd>");
		/***/
		memset(bextOriginatorReference, 0, sizeof(bextOriginatorReference));
		sprintf(bextOriginatorReference, "<tbd>");
		/***/
		memset(bextOriginationDate, 0, sizeof(bextOriginationDate));
		memset(bextOriginationTime, 0, sizeof(bextOriginationTime));
		AAFGetDateTime(&dateTime);
		sprintf(bextOriginationDate, "%04d-%02d-%02d", dateTime.date.year,
										dateTime.date.month, dateTime.date.day);
		sprintf(bextOriginationTime, "%02d:%02d:%02d", dateTime.time.hour,
										dateTime.time.minute, dateTime.time.second);
		/***/
		bextTimeReference = 0;
		/***/
		bextVersion = 0;
		/***/
		memset(bextReserved, 0, sizeof(bextReserved));
		/***/
		
		// Now write the broadcast chunk
		ptr = nextChunk;
		*ptr++ = 'b';
		*ptr++ = 'e';
		*ptr++ = 'x';
		*ptr++ = 't';
		len =  bextSize;
		CHECK(fillSwappedWAVEData(&ptr, 4L, &len));
		memcpy(ptr, bextDescription, 256);
		ptr += 256;
		memcpy(ptr, bextOriginator, 32);
		ptr += 32;
		memcpy(ptr, bextOriginatorReference, 32);
		ptr += 32;
		memcpy(ptr, bextOriginationDate, 10);
		ptr += 10;
		memcpy(ptr, bextOriginationTime, 8);
		ptr += 8;
		CHECK(fillSwappedWAVEData(&ptr, sizeof(bextTimeReference), &bextTimeReference));
		CHECK(fillSwappedWAVEData(&ptr, sizeof(bextVersion), &bextVersion));
		memcpy(ptr, bextReserved, sizeof(bextReserved));
		ptr += sizeof(bextReserved);
		memcpy(ptr, bextCodingHistory, bextLenCodingHistory);
		ptr += bextLenCodingHistory;
#else
		ptr = nextChunk;
#endif
		
		*bytesWritten = ptr - buffer;
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
AAFRESULT CAAFWaveCodec::loadWAVEHeader(void)
{
	aafInt32			offset, chunkStart, formSize;
	aafInt64			chunkStart64;
	aafInt16			pcm_format;
	aafUInt8            chunkID[4];
	aafUInt32			chunkSize, bytesRead;
	aafBoolean_t				fmtFound = kAAFFalse, dataFound = kAAFFalse;
 	aafInt32			junk32, rate;
	aafInt64			savePos;
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
		if (memcmp(&chunkID, "RIFF", (size_t) 4) != 0)
			RAISE(AAFRESULT_BADWAVEDATA);
		CHECK(GetWAVEData(4L, (void *) &formSize));	
		CHECK(_stream->Read(4L, chunkID, &bytesRead));
		if (memcmp(&chunkID, "WAVE", (size_t) 4) != 0)
			RAISE(AAFRESULT_BADWAVEDATA);
		CHECK(_stream->GetPosition(&chunkStart64));
		CHECK(TruncInt64toInt32(chunkStart64, &offset));	// OK - 32-bit format
	
		while ((offset < formSize) && _stream->Read(4L, chunkID, &bytesRead) == AAFRESULT_SUCCESS)
		{
			CHECK(GetWAVEData(4L, (void *) &chunkSize));	
			CHECK(_stream->GetPosition(&chunkStart64));
	
			if (memcmp(&chunkID, "fmt ", (size_t) 4) == 0)
			{
				/* WAVE field: wFormatTag */
				CHECK(GetWAVEData(2L, &pcm_format));
				if (pcm_format != 1)
					RAISE(AAFRESULT_BADWAVEDATA);
	
				/* WAVE field: wChannels */
				CHECK(GetWAVEData(2L, &_numCh));
	
				/* WAVE field: wSamplesPerSec */
				CHECK(GetWAVEData(4L, &rate));
				_sampleRate.numerator = rate;
				_sampleRate.denominator = 1;
	
				/* Skip WAVE field: avgBytesPerSec (4 bytes) */
				CHECK(GetWAVEData(4L, &junk32));
	
				/* WAVE field wBlockAlign */
				CHECK(GetWAVEData(2L, &_bytesPerFrame));
	
				/* WAVE field Sample Width */
				CHECK(GetWAVEData(2L, &_bitsPerSample));
	
				_bytesPerFrame = ((_bitsPerSample + 7) / 8) * _numCh;
	
				fmtFound = kAAFTrue;
			} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
			{
				CvtInt32toInt64(chunkSize / _bytesPerFrame, &_sampleFrames);
				/* Positioned at beginning of audio data */
				CHECK(_stream->GetPosition(&_dataStartOffset));
	
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
		RERAISE(AAFRESULT_BADWAVEDATA);
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
AAFRESULT CAAFWaveCodec::ComputeWriteChunkSize(
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
		CHECK(TruncInt64toUInt32(result, &size));	/* OK WAVE */
#if DEBUG_READ
		CHECK(_stream->Seek(0L));
		_stream->Read(64L, debugBuf, &bytesRead);
#endif
		CHECK(_stream->Seek(sizeOff));
		tmp = buf;
		CHECK(fillSwappedWAVEData(&tmp, 4L, &size));
		CHECK(_stream->Write(4L, buf, &bytesWritten));
	
		CHECK(_stream->Seek(savePos));
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}

AAFRESULT CAAFWaveCodec::CreateAudioDataEnd(void)
{
	aafInt64          curOffset;

	XPROTECT()
	{
		/* this routine will be called after sample data is written */
		/* Now set the patches for all the fields of the wave data */
		CHECK(_stream->GetPosition(&curOffset));
	
		CHECK(ComputeWriteChunkSize(4, curOffset));
		CHECK(ComputeWriteChunkSize(_dataSizeOffset, curOffset));
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
static void SplitBuffers(void *original, aafUInt32 srcSamples, aafUInt16 sampleSize, aafUInt16 numDest, interleaveBuf_t *destPtr)
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
HRESULT CAAFWaveCodec::InternalQueryInterface
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

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFWaveCodec)
