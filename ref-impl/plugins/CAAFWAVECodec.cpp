/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
\******************************************/

#include "CAAFWAVECodec.h"

#include <assert.h>
#include "AAFResult.h"

#include "AAF.h"

//#include "CAAFEssenceFormat.h"
//#include "ImplAAFEssenceFormat.h"
#include "aafErr.h"
#include "aafUtils.h"
#include "aafCvt.h"
#include "aafDataDefs.h"
#include "aafDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

#define STD_HDRSIZE_DATA		42
#define STD_HDRSIZE_NODATA		36

const aafProductVersion_t AAFPluginImplementationVersion = {1, 0, 0, 1, kVersionBeta};
const aafRational_t		defaultRate = { 44100, 1 };
const aafInt32			defaultSampleWidth = 8;
const aafInt32			defaultNumCh = 1;

// CLSID for AAFEssenceCodec 
//{8D7B04B1-95E1-11d2-8089-006008143E6F}
const CLSID CLSID_AAFWaveCodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

const aafUID_t JEFFS_WAVE_PLUGIN = { 0x431D5CA1, 0xEDE2, 0x11d2, { 0x80, 0x9F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

static void SplitBuffers(void *original, aafInt32 srcSamples, aafInt16 sampleSize, aafInt16 numDest, interleaveBuf_t *destPtr);

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Start (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Finish (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetNumDefinitions (aafInt32 *pDefCount)
{
	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedDefinitionID (aafInt32 index, aafUID_t *uid)
{
	if(uid == NULL)
		return AAFRESULT_NULL_PARAM;

	*uid = CodecWave;		// UID of the WAVE codec definition
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
    CAAFWaveCodec::GetIndexedDefinitionObject (aafInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	IAAFCodecDef	*codecDef = NULL;
	IAAFDefObject	*obj = NULL;
	aafUID_t		uid;
	
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		//!!!Later, add in dataDefs supported & filedescriptor class
		CHECK(dict->CreateInstance(&AUID_AAFCodecDef,
							IID_IAAFCodecDef, 
							(IUnknown **)&codecDef));
		uid = CodecWave;
		CHECK(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(obj->Init(&uid, L"WAVE Codec", L"Handles RIFF WAVE data."));
		uid = DDEF_Sound;
		CHECK(codecDef->AppendEssenceKind (&uid));
		*def = obj;
		codecDef->Release();
		codecDef = NULL;
	}
	XEXCEPT
	{
		if(codecDef != NULL)
			codecDef->Release();
		if(obj != NULL)
			obj->Release();
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
    CAAFWaveCodec::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDescriptor **descPtr)
{
	IAAFPluginDescriptor	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	aafUID_t				category = AUID_AAFDefObject, manufacturer = MANUF_JEFFS_PLUGINS;
	aafUID_t				plugID = JEFFS_WAVE_PLUGIN;
	
	XPROTECT()
	{
		CHECK(dict->CreateInstance(&AUID_AAFPluginDescriptor,
			IID_IAAFPluginDescriptor, 
			(IUnknown **)&desc));
		*descPtr = desc;
		desc->AddRef();
		CHECK(desc->Init(&plugID, L"Example WAVE Codec", L"Handles RIFF WAVE data."));

		CHECK(desc->SetCategoryClass(&category));
		CHECK(desc->SetPluginVersionString(manufRev));
		CHECK(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (manufURL));
		CHECK(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		CHECK(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		CHECK(desc->SetManufacturerID(&manufacturer));
		CHECK(desc->SetPluginManufacturerName(manufName));
		CHECK(desc->SetIsSoftwareOnly(AAFTrue));
		CHECK(desc->SetIsAccelerated(AAFFalse));
		CHECK(desc->SetSupportsAuthentication(AAFFalse));
		
		/**/
		CHECK(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
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
			desc->Release();
		if(pLoc != NULL)
			pLoc->Release();
		if(pNetLoc != NULL)
			pNetLoc->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}


CAAFWaveCodec::CAAFWaveCodec (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFUnknown (pControllingUnknown)
{
	_headerLoaded = AAFFalse;
	_nativeByteOrder = GetNativeByteOrder();
	_sampleRate = defaultRate;
	_bitsPerSample = defaultSampleWidth;
	_numCh = defaultNumCh;
	_bytesPerFrame = (defaultSampleWidth+7)/8;;
	_sampleFrames = 0;
	_interleaveBuf = NULL;
	_dataStartOffset = 0;
	_dataSizeOffset = 0;
	_readOnly = AAFFalse;
	_stream = NULL;
	_access = NULL;
	_sampleDataHeaderWritten = AAFFalse;
	_initialSeekPerformed = AAFFalse;
	_mdes = NULL;
	_interleaveBuf = NULL;
}


CAAFWaveCodec::~CAAFWaveCodec ()
{
	if(_access != NULL)
		_access->Release();
	if(_stream != NULL)
		_stream->Release();
	if(_mdes != NULL)
		_mdes->Release();
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::SetEssenceAccess(IAAFEssenceAccess *access)
{
	if(_access != NULL)
		_access->Release();
	if(access != NULL)
	{
		_access = access;
		_access->AddRef();
	}

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetFlavourCount(aafInt32 *pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedFlavourID (aafInt32  index,
        aafUID_t *  pVariant)
{
	if(pVariant == NULL)
		return AAFRESULT_NULL_PARAM;
	*pVariant = NilCodecFlavour;
	return AAFRESULT_SUCCESS;
}


	
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::SetCompressionEnabled (aafBool  enable)
{
  return HRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetDataDefinitionCount (aafInt32 *pDefCount)
{
	//!!!Add error checking
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedDataDefinition (aafInt32  index,
        aafUID_t *  pVariant)
{
  return HRESULT_NOT_IMPLEMENTED;
}

//!!!Need a way to convert between CLSID and aafUID_t
//!!!Change this to stored object IDs
const aafUID_t LOCAL_AAFWAVEDescriptor = { 0x4c2e1691, 0x8ae6, 0x11d2, { 0x81, 0x3c, 0x00, 0x60, 0x97, 0x31, 0x01, 0x72 } };
const aafUID_t LOCAL_AAFEssenceData = { 0x6a33f4e1, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const wchar_t	name[] = L"WAVE Codec";

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetMaxCodecDisplayNameLength (
        aafInt32  *bufSize)
{
	if(bufSize == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*bufSize = wcslen(name);
	return AAFRESULT_SUCCESS;
}	

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetCodecDisplayName (aafUID_t  variant,
        wchar_t *  pName,
        aafInt32  bufSize)
{
	aafInt32	len = sizeof(name);
	if(len > bufSize)
		len = bufSize;
	memcpy(pName, name, len);
	return AAFRESULT_SUCCESS;
}
	
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetNumChannels (IAAFSourceMob *fileMob,
        aafUID_t  essenceKind,
        IAAFEssenceStream *stream,
        aafInt16 *  pNumChannels)
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
    CAAFWaveCodec::GetNumSamples (
        aafUID_t  essenceKind,
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
    CAAFWaveCodec::ValidateEssence (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
		aafCheckVerbose_t  verbose,
        aafCheckWarnings_t warning,
         aafInt32  bufSize,
		wchar_t *  pName,
        aafInt32  *bytesWritten)
{
	return HRESULT_NOT_IMPLEMENTED;
}

		
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Create (IAAFSourceMob *unk,
        aafUID_t  variant,
        IAAFEssenceStream * stream,
        aafInt32 numParms,
        aafmMultiCreate_t *createParms)
{
	IAAFSourceMob			*fileMob = NULL;
	IAAFEssenceDescriptor	*mdes = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	AAFRESULT				aafError;
	unsigned char			header[STD_HDRSIZE_NODATA];

	if(_stream == NULL)
	{
		_stream = stream;
		_stream->AddRef();
	}
	_readOnly = AAFFalse;
	XPROTECT()
	{
		_sampleRate = createParms[0].sampleRate;	// !!!Assumes all sample rates are the same

		aafError = (unk->QueryInterface(IID_IAAFSourceMob, (void **)&fileMob));
		CHECK(fileMob->GetEssenceDescriptor(&mdes));
		fileMob->Release();
		fileMob = NULL;
		CHECK(mdes->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&_mdes));
		CHECK(CreateWAVEheader(header, STD_HDRSIZE_NODATA, (aafInt16)numParms));
		CHECK(_mdes->SetSummary (STD_HDRSIZE_NODATA, header));
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
    CAAFWaveCodec::Open (IAAFSourceMob *unk,
        aafSlotID_t	slotID,
		aafMediaOpenMode_t  openMode,
        IAAFEssenceStream * stream)
{
	AAFRESULT		aafError;
	IAAFSourceMob	*fileMob = NULL;
	IAAFEssenceDescriptor *edes = NULL;

	if(_stream == NULL)
	{
		_stream = stream;
		_stream->AddRef();
	}
	_readOnly = openMode == kMediaOpenReadOnly ? AAFTrue : AAFFalse;
	XPROTECT()
	{
		aafError = (unk->QueryInterface(IID_IAAFSourceMob, (void **)&fileMob));
		CHECK(fileMob->GetEssenceDescriptor(&edes));
		aafError = (edes->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&_mdes));

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


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::WriteBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	aafUInt32      	fileBytes;
	aafInt32      	n, xfers, samp;
	aafUInt32		maxSamplesLeft;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result;
	interleaveBuf_t	*interPtr;
	aafUInt8		*destPtr;
	aafUInt32		zero = 0;
	aafUInt8		sampleBuf[256];
	unsigned char	header[STD_HDRSIZE_NODATA];

	XPROTECT()
	{
		XASSERT(_bitsPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);

		if(!_sampleDataHeaderWritten)
		{
			CHECK(CreateWAVEheader(header, STD_HDRSIZE_NODATA, _numCh));

			_stream->Seek(0);
			_sampleDataHeaderWritten = AAFTrue;
			// The next four lines won't work for raw writes
			_stream->Write(header, STD_HDRSIZE_NODATA);
			_stream->Write((aafUInt8 *)"data", 4);
			_stream->GetPosition(&_dataSizeOffset);
			_stream->Write((aafUInt8 *)&zero, 4);		
		}

		for (n = 0; n < xferBlockCount; n++)
		{
			resultBlock[n].bytesXfered = 0;
			resultBlock[n].samplesXfered = 0;
		}
		
		if(inter == leaveInterleaved)
		{
			for (n = 0; n < xferBlockCount; n++)
			{
				xfer = xferBlock + n;
		
				fileBytes = xfer->numSamples * _bytesPerFrame;
				if (fileBytes > xfer->buflen)
					RAISE(AAFRESULT_SMALLBUF);
				
				CHECK(_stream->Write(xfer->buffer, fileBytes));
				
				resultBlock->bytesXfered = xfer->numSamples * _bytesPerFrame;
				resultBlock->samplesXfered += xfer->numSamples / _bytesPerFrame;
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
				
				CHECK(_stream->Write(xfer->buffer, fileBytes));
		
		
				result->bytesXfered = xfer->numSamples * _bytesPerFrame;
				result->samplesXfered += xfer->numSamples / _bytesPerFrame;
			}
		}
		else
		{
			aafInt32	bytesPerSample;
			
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
					xfers = (aafInt32) maxSamplesLeft;
					
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
				CHECK(_stream->Write(sampleBuf, fileBytes));
			}
		}
	}
	XEXCEPT
	XEND
	
	return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::ReadBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock,
        aafmMultiResult_t *  resultBlock)
{
	aafUInt32       nbytes, fileBytes;
	aafInt32        n, startBuflen, xferSamples, sub;
	aafUInt32		maxSamplesLeft;
	aafUInt8		*start;
	aafmMultiXfer_t *xfer;
	aafmMultiResult_t *result;
	aafInt16		ch, xf;
	aafUInt8		tmpBuf[256];
	

	XPROTECT()
	{
		if(!_initialSeekPerformed)
		{
			CHECK(_stream->Seek(_dataStartOffset));	// Not compatible with raw read
			_initialSeekPerformed = AAFTrue;
		}

		XASSERT(_bitsPerSample != 0, AAFRESULT_ZERO_SAMPLESIZE);
		
		for (n = 0; n < xferBlockCount; n++)
		{
			resultBlock[n].bytesXfered = 0;
		}
		
		if(inter == leaveInterleaved)
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
			aafInt32	bytesPerSample;

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
				if ((aafInt32)fileBytes > startBuflen)
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
	aafInt32           bytesPerFrame;

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
	aafInt64	byteLen, sampleLen;
	IAAFEssenceDescriptor	*essenceDesc = NULL;
	IAAFFileDescriptor		*fileDesc = NULL;
	IAAFWAVEDescriptor		*waveDesc = NULL;
	aafDataValue_t			buf = NULL;

	XPROTECT()
	{
		CHECK(_stream->GetLength (&byteLen));
		sampleLen = byteLen / _bytesPerFrame;
		CHECK(_mdes->QueryInterface(IID_IAAFFileDescriptor, (void **)&fileDesc));
		CHECK(fileDesc->SetLength(sampleLen));
		fileDesc->Release();
		fileDesc = NULL;

		if(!_readOnly && _sampleDataHeaderWritten)
			CHECK(CreateAudioDataEnd());	// Don't do this for raw calls?
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
    CAAFWaveCodec::WriteFractionalSample (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
	unsigned char	header[STD_HDRSIZE_NODATA];
	aafUInt32		zero = 0;

	XPROTECT()
	{
		if(!_sampleDataHeaderWritten)
		{
			CHECK(CreateWAVEheader(header, STD_HDRSIZE_NODATA, _numCh));

			_stream->Seek(0);
			_sampleDataHeaderWritten = AAFTrue;
			// The next four lines won't work for raw writes
			_stream->Write(header, STD_HDRSIZE_NODATA);
			_stream->Write((aafUInt8 *)"data", 4);
			_stream->GetPosition(&_dataSizeOffset);
			_stream->Write((aafUInt8 *)&zero, 4);		
		}
		CHECK(_stream->Write (buffer, buflen));
	}
	XEXCEPT
	XEND

	return HRESULT_SUCCESS;
}





HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::ReadFractionalSample (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
	XPROTECT()
	{
		if(!_initialSeekPerformed)
		{
			CHECK(_stream->Seek(_dataStartOffset));	// Not compatible with raw read
			_initialSeekPerformed = AAFTrue;
		}
		CHECK(_stream->Read (buflen, buffer, bytesRead));
	}
	XEXCEPT
	XEND

	return HRESULT_SUCCESS;
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::WriteRawData (aafUInt32 nSamples, aafDataBuffer_t  buffer,
        aafUInt32  buflen)
{
	return _stream->Write (buffer, nSamples * _bytesPerFrame);
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
			_initialSeekPerformed = AAFTrue;
		}

		CHECK(_stream->Read (nSamples * _bytesPerFrame, buffer, bytesRead));
		*samplesRead = (*bytesRead)/_bytesPerFrame ;
	}
	XEXCEPT
	XEND

	return HRESULT_SUCCESS;
}



	
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::CreateDescriptorFromStream (IAAFEssenceStream * pStream,
        IAAFSourceMob *fileMob)
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
	aafUInt32		valueUInt32;
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
		
		CHECK(CreateWAVEheader(header, STD_HDRSIZE_NODATA, _numCh));
		CHECK(_mdes->SetSummary (STD_HDRSIZE_NODATA, header));
		
		// This will output the header on the next non-raw write
		_sampleDataHeaderWritten = AAFFalse;
	}
	XEXCEPT
	XEND

	return (AAFRESULT_SUCCESS);
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetEssenceFormat (IAAFEssenceFormat *pTemplate, IAAFEssenceFormat **pResult)
{
	aafInt32		numSpecifiers, n, bytesRead;
	aafUInt32		valueUInt32;
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
		
		CHECK(CreateWAVEheader(header, STD_HDRSIZE_NODATA, _numCh));
		CHECK(_mdes->SetSummary (STD_HDRSIZE_NODATA, header));
		
		// This will output the header on the next non-raw write
		_sampleDataHeaderWritten = AAFFalse;
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
    CAAFWaveCodec::GetIndexedSampleSize (aafUID_t dataDefID, aafPosition_t pos, aafLength_t *pResult)
{
	aafUID_t	uid = DDEF_Sound;
	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(pos < 0 || pos >=_sampleFrames)
		return(AAFRESULT_EOF);
	if(EqualAUID(&dataDefID, &uid))
		*pResult = _bytesPerFrame;
	else
		return(AAFRESULT_CODEC_CHANNELS);

	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetLargestSampleSize (aafUID_t dataDefID, aafLength_t *pResult)
{
	aafUID_t	uid = DDEF_Sound;
	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(EqualAUID(&dataDefID, &uid))
		*pResult = _bytesPerFrame;
	else
		return(AAFRESULT_CODEC_CHANNELS);
	return (AAFRESULT_SUCCESS);
}

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::AddSampleIndexEntry (aafPosition_t pos)
{
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
AAFRESULT CAAFWaveCodec::fillSwappedWAVEData(aafUInt8 **destBufHdl, aafInt32 maxsize, void *data)
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

AAFRESULT CAAFWaveCodec::scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
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
								aafInt32	bufsize,
								aafInt16	numCh)

{
	aafInt32           chunksize, avgBytesPerSec, samplesPerSec;
	aafInt32           zero = 0, len;
	aafInt16           bytesPerFrame;
	aafInt16           pcm_format = 1;
	aafUInt8			*ptr = buffer;

	XPROTECT()
	{	
		_numCh = numCh;
		if(bufsize < STD_HDRSIZE_NODATA)
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
		len = ptr - buffer;
		ptr = buffer+ 4;
		CHECK(fillSwappedWAVEData(&ptr, 4L, &len));	// Patch in
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
	aafBool				fmtFound = AAFFalse, dataFound = AAFFalse;
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
	
				fmtFound = AAFTrue;
			} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
			{
				CvtInt32toInt64(chunkSize / _bytesPerFrame, &_sampleFrames);
				/* Positioned at beginning of audio data */
				CHECK(_stream->GetPosition(&_dataStartOffset));
	
				dataFound = AAFTrue;
			}
			CHECK(TruncInt64toInt32(chunkStart64, &chunkStart));	// OK - 32-bit format
			offset = chunkStart + chunkSize;
	
			if(offset > formSize)
				break;
			if (fmtFound && dataFound)	/* Do we have all information yet? */
				break;
			CHECK(_stream->Seek(offset));
		}
		_headerLoaded = AAFTrue;
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
	aafUInt32          size;
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
		CHECK(_stream->Write(buf, 4L));
	
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
static void SplitBuffers(void *original, aafInt32 srcSamples, aafInt16 sampleSize, aafInt16 numDest, interleaveBuf_t *destPtr)
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
HRESULT CAAFWaveCodec::InternalQueryInterface
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
    else if (riid == IID_IAAFEssenceSampleStream) 
    { 
        *ppvObj = (IAAFEssenceSampleStream *)this; 
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
// Define the contrete object support implementation.
// 
HRESULT CAAFWaveCodec::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFWaveCodec *pAAFWaveCodec = new CAAFWaveCodec(pUnkOuter);
 	if (NULL == pAAFWaveCodec)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceCodec *>(pAAFWaveCodec);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }

