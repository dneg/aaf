/***********************************************************************
 *
 *              Copyright (c) 1997 Avid Technology, Inc.
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

#ifndef _OMF_CODEC_LINK_
#define _OMF_CODEC_LINK_ 1

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

typedef omfInt32   omfCodecRev_t;

#define CODEC_REV_1			(omfCodecRev_t)1
#define CODEC_REV_2			(omfCodecRev_t)2			/* Plugin Codecs, varieties */
#define CODEC_REV_3			(omfCodecRev_t)3			/* Separate func pointer allowed */
#define CODEC_CURRENT_REV	CODEC_REV_3

typedef enum
{
	deinterleave, leaveInterleaved
} omfDeinterleave_t;
	

/*************************************************************
 *
 * Used only to call back from a plug-in codec.
 *
 *************************************************************/
typedef struct
{
	omfInt16           numParms;
}               omfCallbackParms_t;

typedef omfErr_t(*omfCallbackPtr) (omfInt32 selector, 
								   omfCallbackParms_t * parmblk);

typedef struct 
{
	omfBool         	hwAssisted;		/* OUT */
	omfBool				isNative;		/* OUT */
	omfBool        		willHandleMDES;	/* OUT */
	omfInt16			relativeLoss;	/* OUT - 0 == lossless */
	omfUInt32			avgBitsPerSec;	/* OUT */
} omfCodecSelectInfo_t;
	
typedef struct 
{
	omfClassIDPtr_t     mdesClassID;	/* OUT */
	omfClassIDPtr_t     dataClassID;	/* OUT */
	omfCodecID_t    	codecID;		/* OUT */
	omfCodecRev_t		rev;			/* IN/OUT */
	omfString			dataKindNameList;	/* OUT - Comma-delimited list of kind names */
	omfFileRev_t		minFileRev;		/* OUT - (revs lower than this will be private */
	omfBool				maxFileRevIsValid; /* OUT - Is there a maximum rev? */
	omfFileRev_t		maxFileRev;		/* OUT - (revs higher than this will be private */
} omfCodecMetaInfo_t;
/************************************************************
 *
 * structures for the GetInfo and PutInfo calls
 *
 * Includes the enum which tells the codec which structure to use.
 * Used by the toolkit and codecs.
 *
 *************************************************************/

typedef struct
{
	omfDDefObj_t  mediaKind;
	omfInt32         largestSampleSize;
}               omfMaxSampleSize_t;

typedef struct
{
	omfJPEGcomponent_t JPEGcomp;
	omfUInt8          *QTables;
	omfUInt8          *ACTables;
	omfUInt8          *DCTables;
	omfInt16           QTableSize;
	omfInt16           ACTableSize;
	omfInt16           DCTableSize;
}               omfJPEGTables_t;

typedef struct
{
	omfInt16           blockLength;
}               omfAudioCompressParms_t;

typedef struct
{
	omfDDefObj_t  	mediaKind;
	omfPosition_t		frameNum;		/* IN  */
	omfLength_t		frameSize;		/* OUT  */
}		omfFrameSizeParms_t;
typedef enum
{
	kAudioInfo,
	kVideoInfo,
	kJPEGTables,
	kCompressionParms,
	kAudioCompressParms,
	kVideoMemFormat,
	kSetAudioMemFormat,
	kMaxSampleSize,
	kPersistentData,
	kMediaIsContiguous,
	kSampleSize
}               omfInfoType_t;

/*
 * API Typecodes (parsed by the codec)
 */
typedef enum
{
	kCodecInit,
	kCodecOpen,
	kCodecCreate,
	kCodecGetInfo,
	kCodecPutInfo,
	kCodecReadSamples,
	kCodecWriteSamples,
	kCodecReadLines,
	kCodecWriteLines,
	kCodecClose,
	kCodecSetFrame,
	kCodecGetNumChannels,
	kCodecInitMDESProps,
	kCodecGetMetaInfo,
	kCodecGetSelectInfo,
	kCodecImportRaw,
	kCodecGetVarietyCount,
	kCodecGetVarietyInfo,
	kCodecSemanticCheck,
	kCodecAddFrameIndexEntry,
	kCodecLoadFuncPointers,
#if OMFI_CODEC_DIAGNOSTICS
	kCodecRunDiagnostics,
#endif
	/* Add all new typecodes BEFORE here */
	kLASTFUNCCODE
} omfCodecFunctions_t;

#define NUM_FUNCTION_CODES		kLASTFUNCCODE

typedef struct omfCodecParms omfCodecParms_t;

typedef omfErr_t(*omfCodecDispPtr_t) (omfCodecFunctions_t func, 
									  omfCodecParms_t * parmblk);

typedef omfErr_t(*omfCodecOptDispPtr_t) (omfCodecParms_t 	*parmblk,
										omfMediaHdl_t 			media,
										omfHdl_t				main);
									  
/************************************************************
 *
 * The codec parameter block and opcode values
 * Used by the toolkit and codecs.
 *
 *************************************************************/
struct omfCodecParms
{
	void				*pers;
	omfCallbackPtr  	dispatchPtr;
#ifdef OMF_CODEC_COMPAT
	omfMediaHdl_t   	media;
#endif
	omfFileRev_t		fileRev;
	union
	{
		struct
		{
			omfSessionHdl_t		session;		/* IN */
			void           		*persistRtn;	/* OUT (alloced by codec) */
		} init;
		struct
		{
			omfCodecOptDispPtr_t	*dispatchTbl;		/* IN */
			omfInt32           		tableSize;			/* IN */
		} loadFuncTbl;
		struct
		{
			omfHdl_t			file;
			omfObject_t			mdes;		/* IN */
			omfCodecSelectInfo_t info;			/* OUT */
		} selectInfo;
		struct
		{
			omfHdl_t			main;
			omfObject_t			fileMob;		/* IN */
			omfHdl_t			rawFile;			/* IN */
		} rawImportInfo;
		struct
		{
			char				*variant;		/* IN */
			char           		*name;			/* IN/OUT */
			omfInt32           	nameLen;			/* IN */
			omfCodecMetaInfo_t	info;			/* OUT */
		} metaInfo;
		struct
		{
			char			    	*mediaKindString;		/* IN */
			omfInt32		count;			/* OUT */
		} varietyCount;
		struct
		{
			char			    	*mediaKindString;	/* IN */
			omfInt32		index;			/* IN */
			char           		*varietyName;		/* OUT */
		} varietyInfo;
		struct
		{
			omfHdl_t			file;
			omfObject_t     	mdes;			/* IN */
			omfDDefObj_t	    mediaKind;			/* IN */
			omfInt16           	numCh;			/* OUT */
		} getChannels;
		struct
		{
			omfHdl_t			file;
			omfObject_t     	mdes;			/* IN */
			char		     	*variant;		/* IN */
		} initMDES;
		struct
		{
			void           		*buf;			/* IN/OUT */
			omfInt32           	bufLen;			/* IN */
			omfInfoType_t   	infoType;		/* IN */
			omfDDefObj_t	    mediaKind;		/* IN */
		} mediaInfo;
		struct
		{
			omfmMultiXfer_t 	*xfer;			/* IN */
			omfInt16           	numXfers;		/* IN */
			omfDeinterleave_t	inter;
		} mediaXfer;
		struct
		{
			void           		*buf;			/* IN/OUT */
			omfInt32           	bufLen;			/* IN */
			omfInt32           	numLines;		/* IN */
			omfUInt32          	bytesXfered;	/* OUT */
		} mediaLinesXfer;
		struct
		{
			omfPosition_t		frameNumber;	/* IN */
		} setFrame;
		struct
		{
			omfPosition_t		frameOffset;	/* IN */
		} addIndex;
		struct
		{
			omfObject_t     	mdes;		/* IN */
			omfHdl_t			file;			/* IN */
			omfObject_t		dataObj;		/* IN */
			char				*message;	/* OUT */
			omfCheckVerbose_t verbose;
			omfCheckWarnings_t warn;
		} semCheck;
#if OMFI_CODEC_DIAGNOSTICS
		struct
		{
			omfObject_t     	mdes;		/* IN */
			omfObject_t		dataObj;		/* IN */
			FILE				*logFile;		/* IN */
		} diag;
#endif
	} spc;
};

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_CODEC_LINK_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

