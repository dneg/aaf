/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFWAVECodec.h"

#include <assert.h>


#include "CAAFEssenceFormat.h"
#include "ImplAAFEssenceFormat.h"


// CLSID for AAFEssenceCodec 
//{8D7B04B1-95E1-11d2-8089-006008143E6F}
const CLSID CLSID_AAFWaveCodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

#if AS_PLUGIN
// CLSID for AAFPlugin 
// {3631F7A3-9121-11d2-8088-006008143e6f}
const CLSID CLSID_AAFPlugin = { 0x3631F7A3, 0x9121, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


// Default Interface for AAFPlugin 
// {3631F7A4-9121-11d2-8088-006008143e6f}
const IID IID_IAAFPlugin = { 0x3631F7A4, 0x9121, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

// CLSID for AAFEssenceCodec 
// {3631F7A1-9121-11d2-8088-006008143e6f}
const CLSID CLSID_AAFEssenceCodec = { 0x3631F7A1, 0x9121, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


// Default Interface for AAFEssenceCodec 
// {3631F7A2-9121-11d2-8088-006008143e6f}
const IID IID_IAAFEssenceCodec = { 0x3631F7A2, 0x9121, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
#else
extern const CLSID CLSID_AAFPlugin;
extern const IID IID_IAAFPlugin;
extern const CLSID CLSID_AAFEssenceCodec;
extern const IID IID_IAAFEssenceCodec;
#endif

#if 0 //!!!
CAAFPlugin::CAAFPlugin (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFUnknown(pControllingUnknown)
{
}


CAAFPlugin::~CAAFPlugin ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFPlugin::Start ()
{
  return HRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFPlugin::Finish ()
{
  return HRESULT_NOT_IMPLEMENTED;
}

//
// 
// 
HRESULT CAAFPlugin::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFPlugin) 
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
//!!!AAF_DEFINE_FACTORY(AAFPlugin)

#endif

CAAFWaveCodec::CAAFWaveCodec (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFDefaultCodec (pControllingUnknown, AAFFalse)
{
}


CAAFWaveCodec::~CAAFWaveCodec ()
{
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedVariantID (aafInt32  index,
        aafUID_t *  pVariant)
{
  return HRESULT_NOT_IMPLEMENTED;
}


	
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::SetCompression (aafCompressEnable_t  enable)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetIndexedDataDefinition (aafInt32  index,
        aafUID_t *  pVariant)
{
  return HRESULT_NOT_IMPLEMENTED;
}

//!!!Need a way to convert between CLSID and aafUID_t
const aafUID_t LOCAL_AAFWAVEDescriptor = { 0x4c2e1691, 0x8ae6, 0x11d2, { 0x81, 0x3c, 0x00, 0x60, 0x97, 0x31, 0x01, 0x72 } };
const aafUID_t LOCAL_AAFEssenceData = { 0x6a33f4e1, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };

HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetMetaInfo (aafCodecMetaInfo_t *  pInfo)
{
	pInfo->mdesClassID = LOCAL_AAFWAVEDescriptor;
    pInfo->dataClassID = LOCAL_AAFEssenceData;		//!!!Isn't this a subclas still?
//!!!	aafUID_t    			pInfo->plugin.pluginID;					/* OUT */
	pInfo->plugin.pluginInterfaceRevision = kPluginRev1;	/* IN/OUT */
	pInfo->plugin.pluginCodeRevision = AAFReferenceImplementationVersion;
	pInfo->plugin.minimumFileRevision = kAAFRev1;		/* OUT - (revs lower than this will be private */
	pInfo->plugin.validMaximumFileRevision = AAFFalse;	/* OUT - Is there a maximum rev? */
//	pInfo->plugin.maximumFileRevision;		/* OUT - (revs higher than this will be private */
	pInfo->plugin.minimumSDKRevision = AAFReferenceImplementationVersion;
    pInfo->numVariants = 0;
    pInfo->numDataDefinitions = 1;

	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetCodecDisplayName (aafUID_t  variant,
        wchar_t *  pName,
        aafInt32  bufSize)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetSelectInfo (aafUID_t  fileMobID,
        aafCodecSelectInfo_t *  pInfo)
{
  return HRESULT_NOT_IMPLEMENTED;
}

	

	
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetNumChannels (aafUID_t  fileMobID,
        aafUID_t  essenceKind,
        aafInt16 *  pNumChannels)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::SemanticCheck (aafUID_t  fileMobID,
        aafCheckVerbose_t  verbose,
        aafCheckWarnings_t *  warning,
        wchar_t *  pName,
        aafInt32  bufSize)
{
  return HRESULT_NOT_IMPLEMENTED;
}

		
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Create (aafUID_t  fileMobID,
        aafUID_t  variant,
        IAAFEssenceStream * stream)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Open (aafUID_t  fileMobID,
        aafMediaOpenMode_t  openMode,
        IAAFEssenceStream * stream)
{
  return HRESULT_NOT_IMPLEMENTED;
}

	


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::WriteBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::ReadBlocks (aafDeinterleave_t  inter,
        aafInt16  xferBlockCount,
        aafmMultiXfer_t *  xferBlock)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Seek (aafPosition_t  sampleFrame)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::Close ()
{
  return HRESULT_NOT_IMPLEMENTED;
}

		


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::WriteFractionalSample (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  return HRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::ReadFractionalSample (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  return HRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::WriteRawData (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  return HRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::ReadRawData (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  return HRESULT_NOT_IMPLEMENTED;
}



	
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::CreateDescriptorFromStream (IAAFEssenceStream * pStream,
        aafUID_t  pSourceMobID)
{
  return HRESULT_NOT_IMPLEMENTED;
}




HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetCurrentEssenceStream (IAAFEssenceStream ** ppStream)
{
  return HRESULT_NOT_IMPLEMENTED;
}

		
HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::PutEssenceFormat (IAAFEssenceFormat * pFormat)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFWaveCodec::GetEssenceFormat (IAAFEssenceFormat ** ppFormat)
{
  return HRESULT_NOT_IMPLEMENTED;
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

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
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

#if 0

/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

#include "masterhd.h"
#include <string.h>

#include "omPublic.h"
#include "omMedia.h"
#include "omcStd.h" 
#include "omcWAVE.h" 
#include "omPvt.h"

typedef struct
{
	void			*buf;
	omfInt32		buflen;
	omfInt32		samplesLeft;
	omfInt32		bytesXfered;
	omfmMultiXfer_t	*xfer;
} interleaveBuf_t;

#define MAX_FMT_OPS	32

typedef struct
{
	omfUInt16          fileBitsPerSample;
	omfRational_t   fileRate;
	omfInt64          formSizeOffset;
	omfInt64          dataSizeOffset;
	omfInt64          numSamplesOffset;
	omfAudioMemOp_t	fmtOps[MAX_FMT_OPS+1];
	omfAudioMemOp_t	fileFmt[8];
	interleaveBuf_t	*interleaveBuf;
}               userDataWAVE_t;

static omfErr_t readWAVEHeader(omfHdl_t mainFile, omfCodecStream_t *stream,
  			omfDDefObj_t	dataKind,
			userDataWAVE_t *pdata,		/* OUT */
 			omfInt64		*dataStart,
			omfInt16		*numChPtr,
			omfInt64		*sampleFrames,
			omfUInt16		*bytesPerFrame);
			
static omfErr_t GetWAVEData(omfCodecStream_t *stream, omfInt32 maxsize, void *data);
static omfErr_t codecIsValidMDESWAVE(omfCodecParms_t * parmblk);
static omfErr_t codecGetAudioInfoWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media);
static omfErr_t codecOpenWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecCreateWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecCloseWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecWriteBlocksWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecReadBlocksWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t CreateAudioDataEnd(omfMediaHdl_t media);
static omfErr_t omfmAudioSetFrameNumber(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecPutAudioInfoWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media);
static omfErr_t CreateWAVEheader(omfHdl_t main, 
			omfCodecStream_t	*stream,
 			userDataWAVE_t	*pdata,
			omfInt16		numCh,
			omfBool			isData);
static omfErr_t InitMDESProps(omfCodecParms_t * info,
			                      omfMediaHdl_t media,
			                      omfHdl_t main);
static omfErr_t setupStream(omfCodecStream_t *stream, omfDDefObj_t datakind, userDataWAVE_t * pdata);
static omfErr_t InitWAVECodec(omfCodecParms_t *parmblk);
static void SplitBuffers(void *original, omfInt32 srcSamples, omfInt16 sampleSize, omfInt16 numDest, interleaveBuf_t *destPtr);
static omfErr_t codecImportRawWAVE(omfCodecParms_t *parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecSemCheckWAVE(omfCodecParms_t *parmblk, omfMediaHdl_t media, omfHdl_t main);
static omfErr_t codecGetInfoWAVE(omfCodecParms_t * info,
			                      omfMediaHdl_t media,
			                      omfHdl_t main);

static omfErr_t codecPutInfoWAVE(omfCodecParms_t * info,
			                      omfMediaHdl_t media,
			                      omfHdl_t main);
static omfErr_t codecNumChannelsWAVE(omfCodecParms_t * info,
			                      omfMediaHdl_t media,
			                      omfHdl_t main);
static omfErr_t codecGetSelectInfoWAVE(omfCodecParms_t * info,
			                      omfMediaHdl_t media,
			                      omfHdl_t main);

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
omfErr_t        omfCodecWAVE(omfCodecFunctions_t func, omfCodecParms_t * parmblk)
{
	omfErr_t        	status = OM_ERR_NONE;
	omfCodecOptDispPtr_t	*dispPtr;

	switch (func)
	{
	case kCodecInit:
		InitWAVECodec(parmblk);
		break;

	case kCodecGetMetaInfo:
		strncpy((char *)parmblk->spc.metaInfo.name, "WAVE Codec", parmblk->spc.metaInfo.nameLen);
		parmblk->spc.metaInfo.info.mdesClassID = "WAVD";
		parmblk->spc.metaInfo.info.dataClassID = "WAVE";
		parmblk->spc.metaInfo.info.codecID = CODEC_WAVE_AUDIO;
		parmblk->spc.metaInfo.info.rev = CODEC_REV_3;
		parmblk->spc.metaInfo.info.dataKindNameList = SOUNDKIND;
		parmblk->spc.metaInfo.info.minFileRev = kOmfRev1x;
		parmblk->spc.metaInfo.info.maxFileRevIsValid = FALSE;		/* There is no maximum rev */
		return (OM_ERR_NONE);
			
	case kCodecLoadFuncPointers:
		dispPtr = parmblk->spc.loadFuncTbl.dispatchTbl;

		dispPtr[ kCodecOpen ] = codecOpenWAVE;
		dispPtr[ kCodecCreate ] = codecCreateWAVE;
		dispPtr[ kCodecGetInfo ] = codecGetInfoWAVE;
		dispPtr[ kCodecPutInfo ] = codecPutInfoWAVE;
		dispPtr[ kCodecReadSamples ] = codecReadBlocksWAVE;
		dispPtr[ kCodecWriteSamples ] = codecWriteBlocksWAVE;
		dispPtr[ kCodecClose ] = codecCloseWAVE;
		dispPtr[ kCodecSetFrame ] = omfmAudioSetFrameNumber;
		dispPtr[ kCodecGetNumChannels ] = codecNumChannelsWAVE;
		dispPtr[ kCodecGetSelectInfo ] = codecGetSelectInfoWAVE;
		dispPtr[ kCodecImportRaw ] = codecImportRawWAVE;
		dispPtr[ kCodecInitMDESProps ] = InitMDESProps;
/*		dispPtr[ kCodecReadLines ] = NULL;
		dispPtr[ kCodecWriteLines ] = NULL;
		dispPtr[ kCodecGetVarietyCount ] = NULL;
		dispPtr[ kCodecGetVarietyInfo ] = NULL;
		dispPtr[ kCodecSemanticCheck ] = NULL;
		dispPtr[ kCodecAddFrameIndexEntry ] = NULL;
*/
		return(OM_ERR_NONE);
	default:
		status = OM_ERR_INVALID_OP_CODEC;
		break;
	}

	return (status);
}

static omfErr_t codecGetInfoWAVE(omfCodecParms_t * parmblk,
			                      omfMediaHdl_t media,
			                      omfHdl_t main)
{
	omfErr_t			status = OM_ERR_NONE;
	omfType_t			dataType;
	omfMaxSampleSize_t	*samplePtr;
	userDataWAVE_t 		*pdata;

	switch (parmblk->spc.mediaInfo.infoType)
	{
	case kMediaIsContiguous:
		dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
		status = OMIsPropertyContiguous(main, media->dataObj,OMWAVEData,
										dataType, (omfBool *)parmblk->spc.mediaInfo.buf);
		break;
	case kAudioInfo:
		status = codecGetAudioInfoWAVE(parmblk, media);
		break;

	case kMaxSampleSize:
		samplePtr = (omfMaxSampleSize_t *) parmblk->spc.mediaInfo.buf;
		if(samplePtr->mediaKind == media->soundKind)
		{
			pdata = (userDataWAVE_t *)media->userData;
			samplePtr->largestSampleSize = (pdata->fileBitsPerSample + 7)/ 8;
		}
		else
			status = OM_ERR_CODEC_CHANNELS;
		break;

	default:
		status = OM_ERR_INVALID_OP_CODEC;
		break;
	}

	return(status);
}

static omfErr_t codecPutInfoWAVE(omfCodecParms_t * parmblk,
			                      omfMediaHdl_t media,
			                      omfHdl_t main)
{
	omfErr_t	status = OM_ERR_NONE;
	userDataWAVE_t 		*pdata;
	omfInt16			numFmtOps, n;
	omfAudioMemOp_t 	*aparms;

	switch (parmblk->spc.mediaInfo.infoType)
	{
	case kAudioInfo:
		status = codecPutAudioInfoWAVE(parmblk, media);
		break;
	case kSetAudioMemFormat:
		pdata = (userDataWAVE_t *) media->userData;
		numFmtOps = 0;
		while((pdata->fmtOps[numFmtOps].opcode != kOmfAFmtEnd) && (numFmtOps <= MAX_FMT_OPS))
			numFmtOps++;
		
		if(numFmtOps >= MAX_FMT_OPS)
			status = OM_ERR_TOO_MANY_FMT_OPS;
		else
		{
			aparms = ((omfAudioMemOp_t *)parmblk->spc.mediaInfo.buf);
			for(n = 0 ; ; aparms++, n++)
			{
				pdata->fmtOps[n] =  *((omfAudioMemOp_t *)aparms);
				if(aparms->opcode == kOmfAFmtEnd)
					break;
			}
			status = setupStream(media->stream, media->soundKind, pdata);
		}
		break;
	default:
		status = OM_ERR_INVALID_OP_CODEC;
		break;
	}

	return(status);
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
static omfErr_t InitMDESProps(omfCodecParms_t * info,
			                      omfMediaHdl_t media,
			                      omfHdl_t main)
{
	omfObject_t		obj = info->spc.initMDES.mdes;
	omfType_t		dataType = (info->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
	userDataWAVE_t	initialPData;
    omfCodecStream_t streamData;
    omfLength_t		zero;
#ifdef OMFI_ENABLE_STREAM_CACHE
	streamData.cachePtr = NULL;
#endif
	streamData.procData = NULL;
	omfsCvtInt32toInt64(0, &zero);
	
	XPROTECT(main)
	{
		CHECK(omcOpenStream(main, main, &streamData,
							  info->spc.initMDES.mdes, OMWAVDSummary, dataType));
		initialPData.fileBitsPerSample = 0;
		initialPData.fileRate.numerator = 0;
		initialPData.fileRate.denominator = 1;
		omfsCvtInt32toInt64(0, &initialPData.formSizeOffset);
		omfsCvtInt32toInt64(0, &initialPData.dataSizeOffset);
		omfsCvtInt32toInt64(0, &initialPData.numSamplesOffset);
		initialPData.fmtOps[0].opcode = kOmfAFmtEnd;
		initialPData.fileFmt[0].opcode = kOmfAFmtEnd;
		initialPData.interleaveBuf = NULL;
		CHECK(CreateWAVEheader(main, &streamData, &initialPData, 1, FALSE));
		CHECK(omcCloseStream(&streamData));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
}

static omfErr_t codecGetSelectInfoWAVE(omfCodecParms_t * parmblk,
			                      omfMediaHdl_t media,
			                      omfHdl_t main)
{
    omfHdl_t       		file;
	omfErr_t			status = OM_ERR_NONE;
    omfType_t           dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
    userDataWAVE_t      pdataBlock;
    omfDDefObj_t        dataKind;
    omfCodecStream_t    streamData;
	
#ifdef OMFI_ENABLE_STREAM_CACHE
	streamData.cachePtr = NULL;
#endif
	streamData.procData = NULL;

	file = parmblk->spc.selectInfo.file;
	XPROTECT(file)
	{
	    omfiDatakindLookup(file, SOUNDKIND, &dataKind, &status);
	    CHECK(status);
	    CHECK(omcOpenStream(file, file, &streamData, parmblk->spc.selectInfo.mdes,
	                                            OMWAVDSummary, dataType));
	    CHECK(setupStream(&streamData, dataKind, &pdataBlock));
	    CHECK(readWAVEHeader(file, &streamData, dataKind, &pdataBlock,
	                                            NULL, NULL, NULL, NULL));
		parmblk->spc.selectInfo.info.willHandleMDES = TRUE;
	#if PORT_BYTESEX_LITTLE_ENDIAN
		parmblk->spc.selectInfo.info.isNative = TRUE;
	#else
		parmblk->spc.selectInfo.info.isNative = FALSE;
	#endif
		parmblk->spc.selectInfo.info.hwAssisted = FALSE;
        parmblk->spc.selectInfo.info.relativeLoss = 0;
        parmblk->spc.selectInfo.info.avgBitsPerSec =
        									(pdataBlock.fileBitsPerSample *
        									pdataBlock.fileRate.numerator) /
        									pdataBlock.fileRate.denominator;
	    CHECK(omcCloseStream(&streamData));
    }
    XEXCEPT
    XEND

	return(OM_ERR_NONE);
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
static omfErr_t codecNumChannelsWAVE(omfCodecParms_t * parmblk,
			                      omfMediaHdl_t media,
			                      omfHdl_t main)
{
	omfHdl_t			file;
	omfCodecStream_t	streamData;
	omfType_t			dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
	omfInt64				sampleFrames;
	userDataWAVE_t		pdataBlock;
	omfDDefObj_t		dataKind;
	omfInt16				numCh;
	omfUInt16				bytesPerFrame;
	omfErr_t			status;
	
#ifdef OMFI_ENABLE_STREAM_CACHE
	streamData.cachePtr = NULL;
#endif
	streamData.procData = NULL;

	file = parmblk->spc.getChannels.file;
	XPROTECT(file)
	{
		omfiDatakindLookup(parmblk->spc.getChannels.file, SOUNDKIND, &dataKind, &status);
		CHECK(status);
		if (parmblk->spc.getChannels.mediaKind == dataKind)
			{
			CHECK(omcOpenStream(file, file, &streamData,
								parmblk->spc.getChannels.mdes,
								OMWAVDSummary, dataType));
			CHECK(setupStream(&streamData, dataKind, &pdataBlock));
			CHECK(readWAVEHeader(parmblk->spc.getChannels.file, &streamData, dataKind, &pdataBlock,
								NULL, &numCh, &sampleFrames, &bytesPerFrame));
			CHECK(omcCloseStream(&streamData));
			parmblk->spc.getChannels.numCh = numCh;
			}
		else
			parmblk->spc.getChannels.numCh = 0;
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
static omfErr_t InitWAVECodec(omfCodecParms_t *parmblk)
	{	
	parmblk->spc.init.persistRtn = NULL;
	return(OM_ERR_NONE);
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
static omfErr_t codecGetAudioInfoWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media)
{
	userDataWAVE_t *pdata;
	omfAudioMemOp_t *aparms;

	omfAssertMediaHdl(media);

	pdata = (userDataWAVE_t *) media->userData;
	aparms = *((omfAudioMemOp_t **)parmblk->spc.mediaInfo.buf);
	for( ; aparms->opcode != kOmfAFmtEnd; aparms++)
	  {
		switch(aparms->opcode)
		  {
		  case kOmfSampleSize:
			aparms->operand.sampleSize = pdata->fileBitsPerSample;
			break;
		  case kOmfSampleRate:
			aparms->operand.sampleRate = pdata->fileRate;
			break;
			
		  case kOmfSampleFormat:
			aparms->operand.format = kOmfSignedMagnitude;
			break;
	
		  case kOmfNumChannels:
			aparms->operand.numChannels = media->numChannels;
			break;
		  }
	  }

	return (OM_ERR_NONE);
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
static omfErr_t codecPutAudioInfoWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media)
{
	userDataWAVE_t *pdata;
	omfAudioMemOp_t *aparms;
	omfHdl_t        main;
	omfType_t		dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);

	omfAssertMediaHdl(media);
	main = media->mainFile;
	
	XPROTECT(main)
	{
		pdata = (userDataWAVE_t *) media->userData;
		aparms = *((omfAudioMemOp_t **)parmblk->spc.mediaInfo.buf);
		for( ; aparms->opcode != kOmfAFmtEnd; aparms++)
		{
			switch(aparms->opcode)
			{
			case kOmfSampleSize:
				pdata->fileBitsPerSample = aparms->operand.sampleSize;
				break;
				
			case kOmfSampleRate:
				pdata->fileRate = aparms->operand.sampleRate;
				break;
	
			case kOmfSampleFormat:	/* Fixed sample format */
				RAISE(OM_ERR_INVALID_OP_CODEC);
				break;
	
			case kOmfNumChannels:
				media->numChannels = aparms->operand.numChannels;
				break;
			}
		}
	
		CHECK(omcCloseStream(media->stream));	/* Close the existing data
							 * stream */
	
		if(main->fmt == kOmfiMedia)
		{
			CHECK(omcOpenStream(main, main, media->stream, media->mdes, OMWAVDSummary, dataType));
			CHECK(setupStream(media->stream, media->soundKind, pdata));
			CHECK(CreateWAVEheader(main,  media->stream, pdata, media->numChannels, FALSE));
			CHECK(omcCloseStream(media->stream));
		}
		
		CHECK(omcOpenStream(main, media->dataFile, media->stream, media->dataObj, OMWAVEData, dataType));
		CHECK(setupStream(media->stream, media->soundKind, pdata));
		CHECK(CreateWAVEheader(main,  media->stream, pdata, media->numChannels, TRUE));
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
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
static omfErr_t readWAVEHeader(omfHdl_t mainFile, omfCodecStream_t *stream,
  			omfDDefObj_t	dataKind,
			userDataWAVE_t *pdata,		/* OUT */
 			omfInt64		*dataStart,
			omfInt16		*numChPtr,
			omfInt64		*sampleFrames,
			omfUInt16		*bytesPerFrame)
{
	omfInt64          offset, chunkStart;
	omfInt16			pcm_format, numCh;
	omfUInt16			fileBitsPerSample, localBytesPerFrame;
	char            chunkID[4];
	omfUInt32          chunkSize;
	omfBool         fmtFound = FALSE, dataFound = FALSE;
 	omfInt32           sampleRate, junk32;
	
	if(dataStart == NULL)
		dataFound = TRUE;		/* Don't try to find this */
	if(bytesPerFrame == NULL)
		bytesPerFrame = &localBytesPerFrame;
		
	pdata->fmtOps[0].opcode = kOmfAFmtEnd;
	pdata->interleaveBuf = NULL;
	
	XPROTECT(mainFile)
	{
		omfsCvtInt32toInt64(0, &pdata->formSizeOffset);
		omfsCvtInt32toInt64(0, &pdata->dataSizeOffset);
		omfsCvtInt32toInt64(0, &pdata->numSamplesOffset);
	
		omfsCvtInt32toInt64(12L, &offset);
		CHECK(omcSeekStreamTo(stream, offset));
	
		while (omcReadStream(stream, 4L, chunkID, NULL) == OM_ERR_NONE)
		{
			CHECK(GetWAVEData(stream, 4L, (void *) &chunkSize));	
			CHECK(omcGetStreamPosition(stream, &chunkStart));
	
			if (strncmp(chunkID, "fmt ", (size_t) 4) == 0)
			{
				/* WAVE field: wFormatTag */
				CHECK(GetWAVEData(stream, 2L, &pcm_format));
				if (pcm_format != 1)
					RAISE(OM_ERR_BADWAVEDATA);
	
				/* WAVE field: wChannels */
				CHECK(GetWAVEData(stream, 2L, &numCh));
	
				/* WAVE field: wSamplesPerSec */
				CHECK(GetWAVEData(stream, 4L, &sampleRate));
				pdata->fileRate.numerator = sampleRate;
				pdata->fileRate.denominator = 1;
	
				/* Skip WAVE field: avgBytesPerSec (4 bytes) */
				CHECK(GetWAVEData(stream, 4L, &junk32));
	
				/* WAVE field wBlockAlign */
				CHECK(GetWAVEData(stream, 2L, bytesPerFrame));
	
				/* WAVE field Sample Width */
				CHECK(GetWAVEData(stream, 2L, &fileBitsPerSample));
	
				*bytesPerFrame = ((fileBitsPerSample + 7) / 8) * numCh;
				pdata->fileBitsPerSample = fileBitsPerSample;
	
				fmtFound = TRUE;
			} else if (strncmp(chunkID, "data", (size_t) 4) == 0)
			{
				if(dataStart != NULL)
				{					
					if(sampleFrames != NULL)
					{
						omfsCvtInt32toInt64(chunkSize / *bytesPerFrame, sampleFrames);
					}
					/* Positioned at beginning of audio data */
					CHECK(omcGetStreamPosition(stream, dataStart));
				}
	
				dataFound = TRUE;
			}
			offset = chunkStart;
			CHECK(omfsAddInt32toInt64(chunkSize, &offset));
	
			/* Re-setup the stream to use the new pdata->fileBitsPerSample */
			CHECK(setupStream(stream, dataKind, pdata));
			CHECK(omcSeekStreamTo(stream, offset));
	
			if (fmtFound && dataFound)	/* Do we have all information yet? */
				break;
		}
	}
	XEXCEPT
	{
		RERAISE(OM_ERR_BADWAVEDATA);
	}
	XEND
	
	if(numChPtr != NULL)
		*numChPtr = numCh;
	
	return(OM_ERR_NONE);
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
static omfErr_t codecOpenWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	omfUInt16          index, bytesPerFrame, fileBitsPerSample;
	omfInt16			numCh;
	userDataWAVE_t *pdata;
	omfInt64          numSamples;
	omfType_t		dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);

	omfAssertMediaHdl(media);

	/*
	 * Find the 'fmt' and 'data' chunks to extract data, skip other
	 * chunks
	 */
	XPROTECT(main)
	{
		media->userData = (userDataWAVE_t *) omOptMalloc(main, sizeof(userDataWAVE_t));
		if (media->userData == NULL)
			RAISE(OM_ERR_NOMEMORY);
	
		pdata = (userDataWAVE_t *) media->userData;
		
		/*
		 * Start at offset 12 in the WAVE data, skipping the ckID, ckSize and
		 * 'WAVE' identifier
		 */
		CHECK(omcOpenStream(main, media->dataFile, media->stream, media->dataObj,
							OMWAVEData, dataType));
		CHECK(setupStream(media->stream, media->soundKind, pdata));
	
		CHECK(readWAVEHeader(main, media->stream, media->soundKind, pdata,
 			&media->dataStart, &numCh, &numSamples, &bytesPerFrame));
	
		fileBitsPerSample = pdata->fileBitsPerSample;
		if (bytesPerFrame == 2 * (((fileBitsPerSample + 7) / 8) * numCh))
			bytesPerFrame /= 2;
	
		if (bytesPerFrame != (((fileBitsPerSample+7) / 8) * numCh))
			RAISE (OM_ERR_BADWAVEDATA);
	
		for (index = 0; index < numCh; index++)
		{
			media->channels[index].dataOffset = media->dataStart;
			media->channels[index].numSamples = numSamples;
		}
	
		CHECK(omcSeekStreamTo(media->stream, media->dataStart));
	}
	XEXCEPT
	{
		RERAISE(OM_ERR_BADWAVEDATA);
	}
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t codecCloseWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	userDataWAVE_t *pdata;

	omfAssertMediaHdl(media);
	pdata = (userDataWAVE_t *) media->userData;

	XPROTECT(main)
	{
		if((media->openType == kOmfiCreated) || (media->openType == kOmfiAppended))
			CHECK(CreateAudioDataEnd(media));
	
		CHECK(omcCloseStream(media->stream));
	
#ifdef DEBUG_CHECK_OUTPUT
		{
		char			debugBuf[28];
		omfUInt32		debugBytesRead;
		omfProperty_t	summary = (parmblk->fileRev == kOmfRev2x ? pers->omWAVDSummary2 : pers->omWAVDSummary);
		omfType_t		dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
		omfProperty_t	data = (parmblk->fileRev == kOmfRev2x ? pers->omWAVEData2 : pers->omWAVEData);

		CHECK(omcOpenStream(main, main, media->stream, media->mdes, OMWAVESummary, dataType));
		CHECK(setupStream(media->stream, media->soundKind, pdata));
		CHECK(omcReadStream(media->stream, sizeof(debugBuf), debugBuf, &debugBytesRead));
		CHECK(omcCloseStream(media->stream));
		
		CHECK(omcOpenStream(main, main, media->stream, media->dataObj, omWAVEData, dataType));
		CHECK(setupStream(media->stream, media->soundKind, pdata));
		CHECK(omcReadStream(media->stream, sizeof(debugBuf), debugBuf, &debugBytesRead));
		CHECK(omcCloseStream(media->stream));
		}
#endif

		if(pdata->interleaveBuf != NULL)
			omOptFree(main, pdata->interleaveBuf);
		omOptFree(main, media->userData);
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t omfmAudioSetFrameNumber(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	omfInt64          nBytes;
	omfInt64          temp, offset, one;
	userDataWAVE_t *pdata;
	omfInt32           bytesPerFrame;

	omfAssertMediaHdl(media);
	pdata = (userDataWAVE_t *) media->userData;

	XPROTECT(main)
	{
		omfsCvtInt32toInt64(1, &one);
		temp = media->channels[0].numSamples;
		CHECK(omfsAddInt32toInt64(1, &temp));
		if (omfsInt64Greater(parmblk->spc.setFrame.frameNumber, temp))
			RAISE(OM_ERR_BADSAMPLEOFFSET);
	
		nBytes = parmblk->spc.setFrame.frameNumber;
		
		/* Make the result zero-based (& check for bad frame numbers as well). */
		if(omfsInt64Less(nBytes, one))
			RAISE(OM_ERR_BADSAMPLEOFFSET);
		CHECK(omfsSubInt64fromInt64(one, &nBytes));
		bytesPerFrame = ((pdata->fileBitsPerSample + 7) / 8) * media->numChannels;
		CHECK(omfsMultInt32byInt64(bytesPerFrame, nBytes, &nBytes));
		offset = media->dataStart;
		CHECK(omfsAddInt64toInt64(nBytes, &offset));
	
		CHECK(omcSeekStreamTo(media->stream, offset));
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t GetWAVEData(omfCodecStream_t *stream, omfInt32 maxsize, void *data)
{
	XPROTECT(stream->mainFile)
	{
		CHECK(omcReadStream(stream, maxsize, data, NULL));
	
		if ((maxsize == sizeof(omfInt32)) && stream->swapBytes)
			omfsFixLong((omfInt32 *) data);
		else if ((maxsize == sizeof(omfInt16)) && stream->swapBytes)
			omfsFixShort((omfInt16 *) data);
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t PutWAVEData(omfCodecStream_t *stream, omfInt32 maxsize, void *data)
{
	omfInt32           dataL;
	omfInt16           dataS;

	XPROTECT(stream->mainFile)
	{
		if ((maxsize == sizeof(omfInt32)) && stream->swapBytes)
		{
			dataL = *((omfInt32 *) data);
			omfsFixLong(&dataL);
			data = &dataL;
		} else if ((maxsize == sizeof(omfInt16)) && stream->swapBytes)
		{
			dataS = *((omfInt16 *) data);
			omfsFixShort(&dataS);
			data = &dataS;
		}
		CHECK(omcWriteStream(stream, maxsize, data));
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t PutWAVEDataAt(omfCodecStream_t *stream, omfInt64 offset, omfInt32 maxsize, void *data)
{
	XPROTECT(stream->mainFile)
	{
		CHECK(omcSeekStreamTo(stream, offset));
		CHECK(PutWAVEData(stream, maxsize, data));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
static omfErr_t codecReadBlocksWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	omfUInt32       nbytes, fileBytes, memBytes;
	omfInt32        bytesPerSample, n, startBuflen, xferSamples, sub;
	omfUInt32		maxSamplesLeft;
	char			*start;
	userDataWAVE_t 	*pdata;
	omfmMultiXfer_t *xfer;
	omfInt16		memBitsPerSample, ch, xf;
	char			tmpBuf[256];
	
	omfAssertMediaHdl(media);

	XPROTECT(main)
	{
		pdata = (userDataWAVE_t *) media->userData;
		XASSERT(pdata->fileBitsPerSample != 0, OM_ERR_ZERO_SAMPLESIZE);

		memBitsPerSample = pdata->fileBitsPerSample;
		for(n = 0; pdata->fmtOps[n].opcode != kOmfAFmtEnd; n++)
			if(pdata->fmtOps[n].opcode == kOmfSampleSize)
				memBitsPerSample = pdata->fmtOps[n].operand.sampleSize;
		
		for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
		{
			parmblk->spc.mediaXfer.xfer[n].bytesXfered = 0;
		}
		
		if(parmblk->spc.mediaXfer.inter == leaveInterleaved)
		{
			bytesPerSample = ((pdata->fileBitsPerSample + 7) / 8);
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
				xfer->samplesXfered = 0;
		
				fileBytes = xfer->numSamples * bytesPerSample * media->numChannels;
				CHECK(omcComputeBufferSize(media->stream, fileBytes, omcComputeMemSize, &memBytes));
				if (memBytes > xfer->buflen)
					RAISE(OM_ERR_SMALLBUF);
		
				CHECK(omcReadSwabbedStream(media->stream, fileBytes, memBytes, xfer->buffer, &nbytes));
				xfer->bytesXfered = nbytes;
				xfer->samplesXfered = nbytes / (bytesPerSample * media->numChannels);
			}
		}
		else if(media->numChannels == 1)
		{
			bytesPerSample = ((pdata->fileBitsPerSample + 7) / 8);
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
				xfer->samplesXfered = 0;
		
				fileBytes = xfer->numSamples * bytesPerSample;
				CHECK(omcComputeBufferSize(media->stream, fileBytes, omcComputeMemSize,&memBytes));
				if (memBytes > xfer->buflen)
					RAISE(OM_ERR_SMALLBUF);
		
				CHECK(omcReadSwabbedStream(media->stream, fileBytes, memBytes, xfer->buffer, &nbytes));
				xfer->bytesXfered = nbytes;
				xfer->samplesXfered = nbytes / bytesPerSample;
			}
		}
		else
		{
			if(pdata->interleaveBuf == NULL)
				pdata->interleaveBuf = (interleaveBuf_t *)omOptMalloc(main, media->numChannels * sizeof(interleaveBuf_t));
			if(pdata->interleaveBuf == NULL)
				RAISE(OM_ERR_NOMEMORY);
			bytesPerSample = ((pdata->fileBitsPerSample + 7) / 8);
			for (n = 0; n < media->numChannels; n++)
			  {
				pdata->interleaveBuf[n].buf = NULL;
				pdata->interleaveBuf[n].xfer = NULL;
			  }
			
			maxSamplesLeft = 0;
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
				xfer->samplesXfered = 0;
				
				if((xfer->subTrackNum <= 0) || (xfer->subTrackNum > media->numChannels))
					RAISE(OM_ERR_CODEC_CHANNELS);

 				sub = xfer->subTrackNum-1;
 				if(pdata->interleaveBuf[sub].buf != NULL)
					RAISE(OM_ERR_XFER_DUPCH);

				pdata->interleaveBuf[sub].buf = xfer->buffer;
				pdata->interleaveBuf[sub].samplesLeft = xfer->numSamples;
				pdata->interleaveBuf[sub].buflen = xfer->buflen;
				pdata->interleaveBuf[sub].bytesXfered = 0;
				pdata->interleaveBuf[sub].xfer = xfer;
				if(xfer->numSamples > maxSamplesLeft)
					maxSamplesLeft = xfer->numSamples;
			}
					
			for (n = 0, start = NULL; (n < media->numChannels) && (start == NULL); n++)
			{
				if(pdata->interleaveBuf[n].buf != NULL)
				{
					start = (char *)pdata->interleaveBuf[n].buf;
					startBuflen = pdata->interleaveBuf[n].buflen;
				}
			}
			if(start == NULL)
				RAISE(OM_ERR_BADDATAADDRESS);
			
			while(maxSamplesLeft > 0)
			{
				if(maxSamplesLeft * bytesPerSample  * media->numChannels >= sizeof(tmpBuf))
					xferSamples = maxSamplesLeft/ media->numChannels;
				else
				{
					xferSamples = maxSamplesLeft;
					start = tmpBuf;
				}
				fileBytes = xferSamples * bytesPerSample * media->numChannels;
				CHECK(omcComputeBufferSize(media->stream, fileBytes, omcComputeMemSize, &memBytes));
				if ((omfInt32)memBytes > startBuflen)
					RAISE(OM_ERR_SMALLBUF);
		
				CHECK(omcReadSwabbedStream(media->stream, fileBytes, memBytes, start, &nbytes));
				
				SplitBuffers(start, xferSamples * media->numChannels, memBitsPerSample, 
							media->numChannels, pdata->interleaveBuf);
				start += memBytes / media->numChannels;
				maxSamplesLeft -= xferSamples;

				for (ch = 0; ch < media->numChannels; ch++)
				{
					xfer = pdata->interleaveBuf[ch].xfer;
 					if(xfer != NULL)
					{
						xfer->bytesXfered = pdata->interleaveBuf[ch].bytesXfered;
						xfer->samplesXfered = xfer->bytesXfered / bytesPerSample;
					}
				}
			}
		}
	}
	XEXCEPT
	{
		if((XCODE() == OM_ERR_EOF) || (XCODE() == OM_ERR_END_OF_DATA))
		{
			for (xf = 0; xf < parmblk->spc.mediaXfer.numXfers; xf++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + xf;
				xfer->bytesXfered = nbytes;
				xfer->samplesXfered = nbytes / (bytesPerSample * media->numChannels);
			}
		}
	}
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t codecWriteBlocksWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	omfUInt32      	fileBytes, memBytes, memBytesPerSample;
	omfInt32      		bytesPerSample, n, ch, xfers, samp;
	omfUInt32			maxSamplesLeft;
	userDataWAVE_t *pdata;
	omfmMultiXfer_t *xfer;
	interleaveBuf_t	*inter;
	char					*destPtr;
	char					sampleBuf[256];

	omfAssertMediaHdl(media);

	XPROTECT(main)
	{
		pdata = (userDataWAVE_t *) media->userData;
		XASSERT(pdata->fileBitsPerSample != 0, OM_ERR_ZERO_SAMPLESIZE);

		bytesPerSample = ((pdata->fileBitsPerSample + 7)/ 8);
		for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
		{
			parmblk->spc.mediaXfer.xfer[n].bytesXfered = 0;
			parmblk->spc.mediaXfer.xfer[n].samplesXfered = 0;
		}
		
		if(parmblk->spc.mediaXfer.inter == leaveInterleaved)
		{
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
		
				fileBytes = xfer->numSamples * bytesPerSample * media->numChannels;
				CHECK(omcComputeBufferSize(media->stream, fileBytes, omcComputeMemSize, &memBytes));
				if (memBytes > xfer->buflen)
					RAISE(OM_ERR_SMALLBUF);
				
				CHECK(omcWriteSwabbedStream(media->stream, fileBytes, memBytes, xfer->buffer));
		
				for(ch = 0; ch < media->numChannels; ch++)
				{
					CHECK(omfsAddInt32toInt64(xfer->numSamples, &media->channels[ch].numSamples));
				}
		
				xfer->bytesXfered = xfer->numSamples * bytesPerSample;
				xfer->samplesXfered += xfer->numSamples / bytesPerSample;
			}
		}
		else if(media->numChannels == 1)
		{
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
		
				fileBytes = xfer->numSamples * bytesPerSample;
				CHECK(omcComputeBufferSize(media->stream, fileBytes, omcComputeMemSize, &memBytes));
				if (memBytes > xfer->buflen)
					RAISE(OM_ERR_SMALLBUF);
				
				CHECK(omcWriteSwabbedStream(media->stream, fileBytes, memBytes, xfer->buffer));
		
				CHECK(omfsAddInt32toInt64(xfer->numSamples, &media->channels[0].numSamples));
		
				xfer->bytesXfered = xfer->numSamples * bytesPerSample;
				xfer->samplesXfered += xfer->numSamples / bytesPerSample;
			}
		}
		else
		{
			if(pdata->interleaveBuf == NULL)
				pdata->interleaveBuf = (interleaveBuf_t *)omOptMalloc(main, media->numChannels * sizeof(interleaveBuf_t));
			if(pdata->interleaveBuf == NULL)
				RAISE(OM_ERR_NOMEMORY);
			bytesPerSample = ((pdata->fileBitsPerSample+ 7) / 8);
			for (n = 0; n < media->numChannels; n++)
				pdata->interleaveBuf[n].buf = NULL;
			
			maxSamplesLeft = 0;
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
				xfer->bytesXfered = xfer->numSamples * bytesPerSample;
				xfer->samplesXfered = xfer->numSamples;
				
				XASSERT((xfer->subTrackNum >= 1) && xfer->subTrackNum <= media->numChannels, OM_ERR_CODEC_CHANNELS);
				inter = pdata->interleaveBuf + (xfer->subTrackNum-1);
				XASSERT(inter->buf == NULL, OM_ERR_XFER_DUPCH);
				inter->buf = xfer->buffer;
				inter->samplesLeft = xfer->numSamples;
				inter->buflen = xfer->buflen;
				inter->bytesXfered = 0;
				if(maxSamplesLeft == 0)
					maxSamplesLeft = xfer->numSamples;
				else if(xfer->numSamples != maxSamplesLeft)
					RAISE(OM_ERR_MULTI_WRITELEN);
			}
			
			CHECK(omcComputeBufferSize(media->stream, bytesPerSample, omcComputeMemSize, &memBytesPerSample));

			while(maxSamplesLeft > 0)
			{
				xfers = sizeof(sampleBuf) / (memBytesPerSample * parmblk->spc.mediaXfer.numXfers);
				if((omfUInt32) xfers > maxSamplesLeft)
					xfers = (omfInt32) maxSamplesLeft;
					
				destPtr = sampleBuf;
				for (samp = 0; samp < xfers; samp++)
				{
					for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
					{
						inter = pdata->interleaveBuf+n;
						memcpy(destPtr, inter->buf, memBytesPerSample);
						inter->buf = (char *)inter->buf + memBytesPerSample;
						destPtr += memBytesPerSample;
						inter->samplesLeft--;
						inter->bytesXfered += memBytesPerSample;
					}
					maxSamplesLeft--;
				}
				
				memBytes = xfers * memBytesPerSample * parmblk->spc.mediaXfer.numXfers;
				CHECK(omcComputeBufferSize(media->stream, memBytes, omfComputeFileSize, &fileBytes));
				CHECK(omcWriteSwabbedStream(media->stream, fileBytes, memBytes, sampleBuf));
			}
			
			for (n = 0; n < parmblk->spc.mediaXfer.numXfers; n++)
			{
				xfer = parmblk->spc.mediaXfer.xfer + n;
				CHECK(omfsAddInt32toInt64(xfer->numSamples, &media->channels[xfer->subTrackNum-1].numSamples));
			}
		}
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
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
static omfErr_t ComputeWriteChunkSize(
			omfHdl_t				main,
			omfCodecStream_t	*stream,
			omfInt64 sizeOff, 
			omfInt64 end)
{
	omfInt64          tmpOffset, savePos, result;
	omfUInt32          size;

	XPROTECT(main)
	{
		CHECK(omcGetStreamPosition(stream, &savePos));
	
		tmpOffset = sizeOff;
		CHECK(omfsAddInt32toInt64(4L, &tmpOffset));
		result = end;
		CHECK(omfsSubInt64fromInt64(tmpOffset, &result));
		CHECK(omfsTruncInt64toUInt32(result, &size));	/* OK WAVE */
		CHECK(PutWAVEDataAt(stream, sizeOff, 4L, &size));
	
		CHECK(omcSeekStreamTo(stream, savePos));
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t CreateAudioDataEnd(omfMediaHdl_t media)
{
	userDataWAVE_t *pdata;
	omfInt64          curOffset;
	omfHdl_t        main;

	omfAssertMediaHdl(media);
	main = media->mainFile;

	XPROTECT(main)
	{
		/* this routine will be called after sample data is written */
		/* Now set the patches for all the fields of the wave data */
		pdata = (userDataWAVE_t *) media->userData;
		CHECK(omcGetStreamPosition(media->stream, &curOffset));
	
		CHECK(ComputeWriteChunkSize(main, media->stream, pdata->formSizeOffset, curOffset));
		CHECK(ComputeWriteChunkSize(main, media->stream, pdata->dataSizeOffset, curOffset));
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t CreateWAVEheader(omfHdl_t main, 
			omfCodecStream_t	*stream,
 			userDataWAVE_t	*pdata,
			omfInt16		numCh,
			omfBool			isData)

{
	omfInt32           chunksize, avgBytesPerSec, samplesPerSec;
	omfInt32           zero = 0;
	omfInt16           bytesPerFrame;
	omfInt16           pcm_format = 1;
	omfInt64		savePos;
	
	XPROTECT(main)
	{	
		CHECK(omcSeekStream32(stream, 0));	
		CHECK(omcWriteStream(stream, 4L, (void *) "RIFF"));
	
		CHECK(omcGetStreamPosition(stream, &pdata->formSizeOffset));
		CHECK(PutWAVEData(stream, 4L, (void *) &zero));
	
		CHECK(omcWriteStream(stream, 8L, (void *) "WAVEfmt "));
	
		chunksize = 16;		/* WAVE format size is fixed */
		CHECK(PutWAVEData(stream, 4L, (void *) &chunksize));
	
		CHECK(PutWAVEData(stream, 2L, (void *) &pcm_format));
	
		CHECK(PutWAVEData(stream, 2L, (void *) &numCh));
	
		if (pdata->fileRate.denominator != 0)
			samplesPerSec = (long) FloatFromRational(pdata->fileRate);
		else
			samplesPerSec = 0;
		CHECK(PutWAVEData(stream, 4L, (void *) &samplesPerSec));
	
		bytesPerFrame = ((pdata->fileBitsPerSample + 7) / 8) * numCh;
		avgBytesPerSec = samplesPerSec * bytesPerFrame;
		CHECK(PutWAVEData(stream, 4L, (void *) &avgBytesPerSec));
	
		CHECK(PutWAVEData(stream, 2L, (void *) &bytesPerFrame));
		CHECK(PutWAVEData(stream, 2L, (void *) &pdata->fileBitsPerSample));
	
		if (isData)
		{
			CHECK(omcWriteStream(stream, 4L, (void *) "data"));
			CHECK(omcGetStreamPosition(stream, &pdata->dataSizeOffset));	/* overload AIFC's
												 * "SSND" field, it's
												 * equivalent */
			CHECK(PutWAVEData(stream, 4L, (void *) &zero));
		}
		else		/* No sample data, so patch FORM size here. */
		{
			CHECK(omcGetStreamPosition(stream, &savePos));
			CHECK(ComputeWriteChunkSize(main, stream, pdata->formSizeOffset, savePos));
		}
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
static omfErr_t codecCreateWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	userDataWAVE_t *pdata;
	omfType_t		dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
	omfUID_t		uid;
	
	omfAssertMediaHdl(media);

	XPROTECT(main)
	{
		media->userData = (userDataWAVE_t *) omOptMalloc(main, sizeof(userDataWAVE_t));
		if (media->userData == NULL)
		{
			RAISE(OM_ERR_NOMEMORY);
		}
	
		pdata = (userDataWAVE_t *) media->userData;
		pdata->fmtOps[0].opcode = kOmfAFmtEnd;
		pdata->fileBitsPerSample = 0;
		pdata->interleaveBuf = NULL;
	
		if(main->fmt == kOmfiMedia)
		{
			if((parmblk->fileRev == kOmfRev1x) || (parmblk->fileRev == kOmfRevIMA))
			{
				CHECK(omfsReadExactEditRate(main, media->mdes, OMMDFLSampleRate, &pdata->fileRate));
				CHECK(omfsReadUID(main, media->fileMob, OMMOBJMobID, &uid));
				CHECK(omfsWriteUID(main, media->dataObj, OMWAVEMobID, uid));
			}
			else
			{
				CHECK(omfsReadRational(main, media->mdes, OMMDFLSampleRate, &pdata->fileRate));
			}
		}
		else
		  pdata->fileRate = media->channels[0].sampleRate;

		omfsCvtInt32toInt64(0, &pdata->formSizeOffset);
		omfsCvtInt32toInt64(0, &pdata->dataSizeOffset);
		omfsCvtInt32toInt64(0, &pdata->numSamplesOffset);
	
		pdata = (userDataWAVE_t *) media->userData;
		if(main->fmt == kOmfiMedia)
		{
			CHECK(omcOpenStream(main, main, media->stream, media->mdes, OMWAVDSummary, dataType));
			CHECK(setupStream(media->stream, media->soundKind, pdata));
	
			CHECK(CreateWAVEheader(main,  media->stream, pdata, media->numChannels, FALSE));
			CHECK(omcCloseStream(media->stream));
#ifdef DEBUG_CHECK_OUTPUT
			{
			char			debugBuf[28];
			omfUInt32		debugBytesRead;
			omcWAVEPersistent_t *pers = (omcWAVEPersistent_t *)parmblk->pers;
			omfProperty_t	summary = (parmblk->fileRev == kOmfRev2x ? pers->omWAVDSummary2 : pers->omWAVDSummary);
	
			CHECK(omcOpenStream(main, main, media->stream, media->mdes, summary, dataType));
			CHECK(setupStream(media->stream, media->soundKind, pdata));
			CHECK(omcReadStream(media->stream, sizeof(debugBuf), debugBuf, &debugBytesRead));
			CHECK(omcCloseStream(media->stream));
			}
#endif
		}
		
		CHECK(omcOpenStream(main, media->dataFile, media->stream, media->dataObj, OMWAVEData, dataType));
		CHECK(setupStream(media->stream, media->soundKind, pdata));
	
		CHECK(CreateWAVEheader(main,  media->stream, pdata, media->numChannels, TRUE));
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
}

/************************
 * codecImportRawWAVE	(INTERNAL)
 *
 * 	Open a raw WAVE file and create an MDES for it on the locator
 *		Don't add the locator, as that is the applications responsibility
 *		(We don't know what type of locator to add).
 */
static omfErr_t codecImportRawWAVE(omfCodecParms_t * parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	omfHdl_t				file, rawFile;
	omfCodecStream_t	readStream, writeStream;
	omfType_t			dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
	userDataWAVE_t		pdataBlock;
	omfInt16			numCh;
	omfUInt16			bytesPerFrame;
	omfInt64			sampleFrames;
	omfObject_t			mdes;
	omfErr_t				status;
	omfDDefObj_t			dataKind;
	
#ifdef OMFI_ENABLE_STREAM_CACHE
	readStream.cachePtr = NULL;
	writeStream.cachePtr = NULL;
#endif
	readStream.procData = NULL;
	writeStream.procData = NULL;

	file = parmblk->spc.rawImportInfo.main;
	rawFile = parmblk->spc.rawImportInfo.rawFile;
	
	XPROTECT(file)
	{
		omfiDatakindLookup(parmblk->spc.getChannels.file, SOUNDKIND, &dataKind, &status);
		CHECK(status);

		CHECK(omcOpenStream(file, rawFile, &readStream, NULL,
							OMNoProperty, OMNoType));
		CHECK(setupStream(&readStream, dataKind, &pdataBlock)); 
		CHECK(readWAVEHeader(parmblk->spc.getChannels.file, &readStream,
							dataKind, &pdataBlock, NULL, &numCh, &sampleFrames,
							&bytesPerFrame));
		CHECK(omcCloseStream(&readStream));
		
		if(parmblk->fileRev == kOmfRev2x)
		{
			CHECK(omfsReadObjRef(file, parmblk->spc.rawImportInfo.fileMob,
										OMSMOBMediaDescription, &mdes));
		}
		else
		{
			CHECK(omfsReadObjRef(file, parmblk->spc.rawImportInfo.fileMob,
										OMMOBJPhysicalMedia, &mdes));
		}

		CHECK(omcOpenStream(file, file, &writeStream,  mdes, OMWAVDSummary, dataType));
		CHECK(setupStream(&writeStream, dataKind, &pdataBlock)); 
		CHECK(CreateWAVEheader(file, &writeStream, &pdataBlock, numCh, FALSE));
		CHECK(omcCloseStream(&writeStream));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
}

/************************
 * codecSemCheckWAVE	(INTERNAL)
 *
 *			Semantic check the WAVE data at a higher level.
 */
static omfErr_t codecSemCheckWAVE(
			omfCodecParms_t	*parmblk, omfMediaHdl_t media, omfHdl_t main)
{
	omfHdl_t			file;
	omfCodecStream_t	streamData;
	omfType_t			dataType = (parmblk->fileRev == kOmfRev2x ? OMDataValue : OMVarLenBytes);
	omfDDefObj_t		dataKind;
	omfInt16			mdesNumCh, dataNumCh;
	omfObject_t			mdes, dataObj;
	userDataWAVE_t		mdesPD, dataPD;
	omfErr_t			status;
	omfBool				getWarnings;
	
#ifdef OMFI_ENABLE_STREAM_CACHE
	streamData.cachePtr = NULL;
#endif
	streamData.procData = NULL;

	mdes = parmblk->spc.semCheck.mdes;
	dataObj = parmblk->spc.semCheck.dataObj;
	
	/* There is nothing to semantic check if just a media descriptor is poresent
	 */
	if(dataObj == NULL)
		return(OM_ERR_NONE);
		
	file = parmblk->spc.semCheck.file;
	parmblk->spc.semCheck.message = NULL;
	getWarnings = (parmblk->spc.semCheck.warn == kCheckPrintWarnings);
	XPROTECT(file)
	{
		omfiDatakindLookup(file, SOUNDKIND, &dataKind, &status);
		CHECK(status);
		/* First read the metadata out of the MDES Summary */
		CHECK(omcOpenStream(file, file, &streamData, mdes,
							OMWAVDSummary, dataType));
		CHECK(setupStream(&streamData, dataKind, &mdesPD));
		CHECK(readWAVEHeader(file, &streamData, dataKind, &mdesPD,
							NULL, &mdesNumCh, NULL, NULL));
		CHECK(omcCloseStream(&streamData));
		/* Next read the metadata out of the dataObject */
		CHECK(omcOpenStream(file, file, &streamData, dataObj,
							OMWAVEData, dataType));
		CHECK(setupStream(&streamData, dataKind, &dataPD));
		CHECK(readWAVEHeader(file, &streamData, dataKind, &dataPD,
							NULL, &dataNumCh, NULL, NULL));
		CHECK(omcCloseStream(&streamData));
		/* Finally, make sure that the data agrees */
		if(mdesNumCh != dataNumCh)
		{
			parmblk->spc.semCheck.message = "Number of channels";
			RAISE(OM_ERR_DATA_MDES_DISAGREE);
		}
		if(mdesPD.fileBitsPerSample != dataPD.fileBitsPerSample)
		{
			parmblk->spc.semCheck.message = "Bits Per Sample";
			RAISE(OM_ERR_DATA_MDES_DISAGREE);
		}
		if((mdesPD.fileRate.numerator != dataPD.fileRate.numerator) ||
		   (mdesPD.fileRate.denominator != dataPD.fileRate.denominator))
		{
			parmblk->spc.semCheck.message = "Frame Rate";
			RAISE(OM_ERR_DATA_MDES_DISAGREE);
		}
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
static omfErr_t setupStream(omfCodecStream_t *stream, omfDDefObj_t datakind, userDataWAVE_t * pdata)
{
	omfCStrmFmt_t   memFmt, fileFmt;
	omfUInt16	actualBitsPerSample;

	XPROTECT(stream->mainFile)
	{
		memFmt.mediaKind = datakind;
		memFmt.afmt = pdata->fmtOps;
		memFmt.vfmt = NULL;
		CHECK(omcSetMemoryFormat(stream, memFmt));
	
		/* Round sample size up to the next byte as per WAVE spec */
		actualBitsPerSample = pdata->fileBitsPerSample;
		if((actualBitsPerSample % 8) != 0)
			actualBitsPerSample +=  (8 - (actualBitsPerSample % 8));

		fileFmt.mediaKind = datakind;
		fileFmt.afmt = pdata->fileFmt;
		fileFmt.vfmt = NULL;
		pdata->fileFmt[0].opcode = kOmfSampleSize;
		pdata->fileFmt[0].operand.sampleSize = actualBitsPerSample;
		pdata->fileFmt[1].opcode = kOmfSampleRate;
		pdata->fileFmt[1].operand.sampleRate = pdata->fileRate;
		pdata->fileFmt[2].opcode = kOmfSampleFormat;
		pdata->fileFmt[2].operand.format = kOmfSignedMagnitude;
		pdata->fileFmt[3].opcode = kOmfAFmtEnd;
		CHECK(omcSetFileFormat(stream, fileFmt, (omfBool)(OMNativeByteOrder != INTEL_ORDER)));
		CHECK(omfmSetSwabProc(stream, stdCodecSwabProc, stdCodecSwabLenProc,
								stdCodecNeedsSwabProc));		
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
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
 * Given a source buffer & an array of destination buffers, split the samples up in
 * round-robin fashion.  If a destination buffer in the array is NULL, then src is
 * incremented by one sample, but no data transfer occurs.  The first destination
 * buffer is allowed to be the same as the source buffer, but the other destination
 * buffers must be distinct to avoid corruption of the data.
 *
 * The number of entries in dest[] must be equal to numDest, and the dest array WILL
 * BE MODIFIED during execution, so it will have to be reloaded.
 */
static void SplitBuffers(void *original, omfInt32 srcSamples, omfInt16 sampleSize, omfInt16 numDest, interleaveBuf_t *destPtr)
{
	omfInt16	*src16, *srcEnd16, *dest16;
	char	*src8, *srcEnd8, *dest8;
	omfInt16	n, x, sampleBytes;
	
	sampleBytes = (sampleSize + 7) / 8;
	if(sampleSize == 16)
	{
		src16 = (omfInt16 *)original;
		srcEnd16 = src16 + srcSamples;
		while(src16 < srcEnd16)
		{
			for(n = 0; (n < numDest) && (src16 < srcEnd16) ; n++)
			{
				dest16 = (omfInt16 *)destPtr[n].buf;
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

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
#endif
