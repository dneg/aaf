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

#ifndef _OMF_TYPES_INCLUDED_
#define _OMF_TYPES_INCLUDED_ 1

#include "masterhd.h"

#ifdef XPDEBUG
#include "xpdebug.h"
#endif

#include "omDefs.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

/************************************************************
 *
 * Portability wrappers around integral types.
 *
 * Each types is guaranteed to by AT LEAST as many bits as its
 * name would suggest.
 *
 * omfBool is used instead of Boolean, in order to reduce conflicts
 * with the different implementations of Boolean.
 *
 *************************************************************/

/* startdoc */
/************************************************************
 *
 * Integer and Boolean types
 *
 *************************************************************/

typedef unsigned char omfUInt8;
typedef short   omfInt16;
typedef unsigned short omfUInt16;
typedef long    omfInt32;
typedef unsigned long omfUInt32;
typedef char    omfBool;

/* enddoc */

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef MAX_NUM_RGBA_COMPS
#define MAX_NUM_RGBA_COMPS 8 /* limit on the size of component arrays */
#endif

/* startdoc */
#if PORT_USE_NATIVE64
typedef PORTKEY_INT64_TYPE omfInt64;
#else
typedef struct		/* Treated as an iteger, defined as array of 4 shorts if needed */
{
	omfUInt16	words[4];
}               omfInt64;
#endif
/* enddoc */

/* startdoc */
/************************************************************
 *
 * OMFI types that map to an integral type or pointer to
 *  integeral type.
 *
 * These each map to an existing integral type or pointer.  Note
 * that omfClassID_t is a structure used to hold storage for a
 * classID, but most functions take an omfClassIDPtr_t reference.
 *
 *************************************************************/

typedef char   *omfString;

/*** Types for all components***/
typedef omfInt64   omfLength_t;

/*** Types for tracks and mob slots***/
typedef omfInt64  omfPosition_t;
typedef omfUInt32	omfFrameOffset_t;
typedef omfUInt32	omfFrameLength_t;
typedef omfUInt32   omfTrackID_t;  
typedef omfInt32   omfNumTracks_t;

/* 1.5 compatibility types */

typedef omfInt16   omfCharSetType_t;
typedef char    omfObjectTag_t[4];
typedef char    *omfTagPtr_t;
typedef omfInt16   omfPhysicalMobType_t;
typedef omfInt32   omfUsageCode_t;
typedef omfInt16	omfTrackType_t;		/* 1.5 compatability only */

typedef char   *omfCodecID_t;
typedef omfInt32   omfNumSlots_t;
typedef char    omfClassID_t[4];
typedef char    *omfClassIDPtr_t;
typedef void    *omfDataValue_t;
typedef char    omfUniqueName_t[OMUNIQUENAME_SIZE];
typedef char    *omfUniqueNamePtr_t;


/*** Types for effects ***/
typedef omfInt32   omfArgIDType_t;

/* Typedefs for object references */
#if 1
typedef struct CMObject_ *omfObject_t;     /* "refNum" for objects               */
#else
typedef void   *omfObject_t;
#endif
typedef omfObject_t omfMobObj_t;
typedef omfObject_t omfMSlotObj_t;
typedef omfObject_t omfCpntObj_t;
typedef omfObject_t omfEffObj_t;
typedef omfObject_t omfESlotObj_t;
typedef omfObject_t omfEDefObj_t;
typedef omfObject_t omfCntlPtObj_t;
typedef omfObject_t omfSegObj_t;
typedef omfObject_t omfDDefObj_t;
typedef omfObject_t omfCDictObj_t;
typedef omfObject_t omfPropObj_t;
typedef omfObject_t omfTDescObj_t;
	
/* typedefs for image processing */
typedef unsigned char omfCompSizeArray_t[MAX_NUM_RGBA_COMPS];
typedef char omfCompArray_t[MAX_NUM_RGBA_COMPS];
typedef omfInt32 omfVideoLineMap_t[2];
typedef omfInt32   omfJPEGTableID_t; /* for TIFF objects */

/************************************************************
 *
 * Public definitions of the opaque handle types.  The file omPvt.h or
 * omCodec.h define the actual structure which is referenced.
 *
 *************************************************************/
typedef struct omfiSession *omfSessionHdl_t;
typedef struct omfiFilePrivate omfFilePvt_t;
typedef struct omfiFile *omfHdl_t;
typedef struct omfiIterate *omfIterHdl_t;
typedef struct omfiSimpleTrack *omfTrackHdl_t;
typedef struct omfCodecStreamFuncs omfCodecStreamFuncs_t;
typedef struct omfiMedia *omfMediaHdl_t;

typedef struct omfiFileChecker *omfFileCheckHdl_t;
typedef struct omfiMobFind *omfMobFindHdl_t;

/*************************************************************
 *
 * Utility typedefs
 *
 *************************************************************/
typedef enum
{
	kOmGetFunction, kOmSetFunction
} omfAccessorType_t;

typedef enum
{
	kClsRequired, kClsRegistered, kClsPrivate
} omfClassType_t;

extern const omfUInt16 OMNativeByteOrder;

typedef enum
{
	kOmfTstRev1x, kOmfTstRev2x, kOmfTstRevEither
}               omfValidRev_t;
	
/************************************************************
 *
 * Enumerated OMFI types.
 *
 *************************************************************/
typedef enum
{
	kMediaOpenReadOnly, kMediaOpenAppend
} omfMediaOpenMode_t;
	
typedef enum
{
	kDominantField1 = 0, kDominantField2, kNoDominant
}               omfFieldDom_t;

typedef enum
{
	kTopField1 = 0, kTopField2, kTopFieldNone
}               omfFieldTop_t;

typedef enum
{
	kOmfVFmtEnd = 0,
		 
	kOmfPixelFormat, 		/* operand.expPixelFormat */
	kOmfFrameLayout, 		/* operand.expFrameLayout */
	kOmfFieldDominance,		/* operand.expFieldDom */
	kOmfStoredRect, 		/* operand.expRect */
	kOmfDisplayRect,  		/* operand.expRect */
	kOmfSampledRect,  		/* operand.expRect */
	kOmfPixelSize, 			/* operand.expInt16 */
	kOmfAspectRatio,		/* operand.expRational */
	kOmfAlphaTransparency,  /* operand.expInt32 */
	kOmfGamma,				/* operand.expRational */
	kOmfImageAlignmentFactor, /* operand.expInt32 */
	kOmfVideoLineMap,		/* operand.expLineMap */
	kOmfWillTransferLines,	/* operand.expBoolean */
	kOmfIsCompressed,		/* operand.expBoolean */
	kOmfLineLength,			/* operand.expInt32 */
	
	/* the following group is exclusive to RGBA codec */
	kOmfRGBCompLayout,		/* operand.expCompArray */
	kOmfRGBCompSizes,		/* operand.expCompSizeArray */
	kOmfRGBPalette,			/* operand.expPointer */
	kOmfRGBPaletteLayout,	/* operand.expCompArray */
	kOmfRGBPaletteSizes,	/* operand.expCompSizeArray */	
		
	 /* the following group is exclusive to CDCI codec */
	kOmfCDCICompWidth,	    /* operand.expInt32 */
	kOmfCDCIHorizSubsampling,/* operand.expUInt32 */
	kOmfCDCIColorSiting,    /* operand.expColorSiting */
	kOmfCDCIBlackLevel,     /* operand.expUInt32 */
	kOmfCDCIWhiteLevel,     /* operand.expUInt32 */
	kOmfCDCIColorRange,     /* operand.expUInt32 */
	kOmfCDCIPadBits,	    /* operand.expInt16 */
		
	kOmfVideoOpcodeReserved1,
	kOmfVideoOpcodeReserved2,
	kOmfVideoOpcodeReserved3,
	kOmfVideoOpcodeReserved4,
	kOmfVideoOpcodeReserved5,
	kOmfVideoOpcodeReserved6,
	kOmfVideoOpcodeReserved7,
	kOmfVideoOpcodeReserved8,
		
		
	kOmfLastVideoOpcode
}               omfVideoFmtOpcode_t;

typedef enum
{
	kOmfPixNone = 0, kOmfPixRGBA, kOmfPixYUV
} 				omfPixelFormat_t;

typedef enum
{
	kOmfAFmtEnd = 0, kOmfSampleSize, kOmfSampleRate, kOmfSampleFormat, 
	kOmfNumChannels
}               omfAudioFmtOpcode_t;

typedef enum
{
	kJcLuminance = 0, kJcChrominance
#if ! STANDARD_JPEG_Q
	, kJcLuminanceFP16, kJcChrominanceFP16
#endif
}               omfJPEGcomponent_t;

typedef enum
{
	kToolkitCompressionEnable = 0, kToolkitCompressionDisable
}               omfCompressEnable_t;

typedef enum
{
	kOmfiMedia = 0, kForeignMedia
}               omfFileFormat_t;

typedef enum
{
	kTcNonDrop = 0, kTcDrop
}               omfDropType_t;

typedef enum
{
	kNoLayout = 0, kFullFrame, kSeparateFields, kOneField, kMixedFields
}               omfFrameLayout_t;
	
typedef enum
{
	kCoSiting = 0, 
	kAveraging = 1, 
	kThreeTap = 2
}               omfColorSiting_t;
	
typedef enum
{
	kPropRequired, kPropOptional
} 				omfPropertyOpt_t;

typedef enum
{
	kExactMatch, kConvertTo, kConvertFrom
} omfDatakindMatch_t;
	  
typedef enum
{
	  	kOmfAppendIfAbsent, kOmfForceOverwrite
} omfAppendOption_t;

/************************************************************
 *
 * Mob specific data types
 *
 *************************************************************/
typedef enum
{
	kCompMob = 0, kMasterMob, kFileMob, kTapeMob, kFilmMob,
	kPrimaryMob, kAllMob
}               omfMobKind_t;

typedef enum
{
  kFindNull, kFindIncoming, kFindOutgoing, kFindRender,
  kFindEffectSrc1, kFindEffectSrc2, kFindEffectSrc3, kFindEffectSrc4, 
  kFindEffectSrc5, kFindEffectSrc6, kFindEffectSrc7, kFindEffectSrc8
} omfEffectChoice_t;

typedef enum
{
	kNoFollowDepend = 0, kFollowDepend
}               omfDepend_t;

typedef enum
{
	kNoIncludeMedia = 0, kIncludeMedia
}               omfIncMedia_t;

/* omfIndexType_t: describes the types of HEAD indices */
typedef enum
{
	kCompMobIndex = 0, kSourceMobIndex, kMediaIndex, kPrimaryMobIndex
}               omfIndexType_t;

/* omfFadeType_t: describes values for SCLP fadein and fadeout types  */
typedef enum
{
	kFadeNone = 0, kFadeLinearAmp, kFadeLinearPower
}               omfFadeType_t;

typedef enum
{
	kOmfOffsetBinary, kOmfSignedMagnitude
} omfAudioSampleType_t;
	
typedef enum
{
	kTapeCaseNull = 0,
	kThreeFourthInchVideoTape,
    kVHSVideoTape,
	k8mmVideoTape,
	kBetacamVideoTape,
	kCompactCassette,
	kDATCartridge,
	kNagraAudioTape
} omfTapeCaseType_t;

typedef enum
{
	kVideoSignalNull = 0,
	kNTSCSignal,
	kPALSignal,
	kSECAMSignal
} omfVideoSignalType_t;

typedef enum
{
	kTapeFormatNull = 0,
	kBetacamFormat,
	kBetacamSPFormat,
	kVHSFormat,
	kSVHSFormat,
	k8mmFormat,
	kHi8Format
} omfTapeFormatType_t;

typedef enum
{
	kOMFNullAttribute = 0,
	kOMFIntegerAttribute,
	kOMFStringAttribute,
	kOMFObjectAttribute
} omfAttributeKind_t;
	
typedef enum
{
	kOMFTwoThreePD,
	kOMFPALPD,
	kOMFOneToOneNTSC,
	kOMFOneToOnePAL
} omfPulldownKind_t;

typedef omfInt16	omfPhaseFrame_t;

typedef enum
{
	kOMFTapeToFilmSpeed,
	kOMFFilmToTapeSpeed
}	omfPulldownDir_t;

/************************************************************
 *
 * Effect Types
 *
 *************************************************************/	  
typedef enum
{
	kConstInterp = 1, kLinearInterp
}               omfInterpKind_t;

typedef enum
{
    kNoEditHint = 0, kProportional, kRelativeLeft, kRelativeRight, 
	kRelativeFixed
} omfEditHint_t;

/************************************************************
 *
 * Data types for Utility Routines
 *
 *************************************************************/	  
typedef enum
{
	kRoundFloor = 0, kRoundCeiling
}               omfRounding_t;
	
typedef enum
{
	kCheckVerbose, kCheckQuiet
}		omfCheckVerbose_t;
	
typedef enum
{
	kCheckPrintWarnings, kCheckNoWarnings
}		omfCheckWarnings_t;

typedef enum
{
	kVersionUnknown = 0, kVersionReleased, kVersionDebug,
	kVersionPatched, kVersionBeta, kVersionPrivateBuild
} omfProductReleaseType_t;

/************************************************************
 *
 * Structured OMFI types.
 *
 *************************************************************/
typedef struct
{
	char            major;
	char            minor;
}               omfVersionType_t;

typedef struct
{
	omfUInt16				major;
	omfUInt16				minor;
	omfUInt16				tertiary;
	omfUInt16				patchLevel;
	omfProductReleaseType_t	type;
} omfProductVersion_t;

typedef struct
{
	char				*companyName;
	char				*productName;
	char				*productVersionString;
	omfInt32			productID;
	char				*platform;
	omfProductVersion_t	productVersion;
} omfProductIdentification_t;

extern const omfProductVersion_t omfiToolkitVersion;

typedef struct
{
	omfUInt32          TimeVal;
	char            IsGMT;
}               omfTimeStamp_t;

typedef struct
{
	omfInt32           prefix;
	omfUInt32          major;
	omfUInt32          minor;
}               omfUID_t;

typedef struct
{
	omfUID_t        ID;
	omfObject_t     Mob;
}               omfObjIndexElement_t;

typedef struct
{
	omfInt32           numerator;
	omfInt32           denominator;
}               omfRational_t;

typedef struct
{
	omfInt32           xOffset;
	omfInt32           yOffset;
	omfInt32           xSize;
	omfInt32           ySize;
}               omfRect_t;

/* #define subTrackNum physicalOutChan */

typedef struct
{
	omfDDefObj_t		mediaKind;		/* IN -- */
	omfInt16			subTrackNum;	/* IN - The physical output channel */
	omfUInt32           numSamples;	/* IN -- */
	omfUInt32          	buflen;	/* IN -- */
	void           		*buffer;	/* IN/OUT -- */
	omfUInt32          	bytesXfered;	/* OUT -- */
	omfUInt32			samplesXfered;	/* OUT */
}               omfmMultiXfer_t;

typedef struct
{
	omfDDefObj_t 		mediaKind;		/* IN -- */
	omfInt16			subTrackNum;	/* IN - The physical output channel */
	omfInt32           	trackID;		/* IN -- */
	omfRational_t		sampleRate;
}               omfmMultiCreate_t;

typedef struct
{
	omfVideoFmtOpcode_t opcode;
	union
	{
		omfPixelFormat_t	expPixelFormat;
		omfFrameLayout_t	expFrameLayout;
		omfFieldDom_t		expFieldDom;
		omfRect_t			expRect;
		omfInt32			expInt32;
		omfInt16			expInt16;
		omfUInt32			expUInt32;
		omfRational_t   	expRational;
		void			   *expPointer;
		omfCompArray_t		expCompArray;
		omfCompSizeArray_t	expCompSizeArray;
		omfColorSiting_t	expColorSiting;
		omfVideoLineMap_t	expLineMap;
		omfBool				expBoolean;
	} operand;
}               omfVideoMemOp_t;

typedef struct
{
	omfAudioFmtOpcode_t opcode;
	union
	{
	    omfAudioSampleType_t	format;	/* OffsetBinary or SignedMagnitude */
		omfInt16           		sampleSize;
		omfRational_t   		sampleRate;
		omfInt16				numChannels;
		omfInt32				expLong;
		omfRational_t   		expRational;
	} operand;
}               omfAudioMemOp_t;

/* Typedefs specific to edgecode and timecode */
typedef enum
{
	kEtNull = 0, kEtKeycode, kEtEdgenum4, kEtEdgenum5
}               omfEdgeType_t;

typedef enum
{
	kFtNull = 0, kFt35MM = 1, kFt16MM, kFt8MM, kFt65MM
}               omfFilmType_t;

typedef enum
   {
    kDirCodeRight = 0, kDirCodeUpperRight, kDirCodeUp, kDirCodeUpperLeft, 
	kDirCodeLeft, kDirCodeLowerLeft, kDirCodeDown, kDirCodeLowerRight
} omfDirectionCode_t;

typedef enum
   {
     kColorSpaceRGB = 0, kColorSpaceYUV, kColorSpaceYIQ, kColorSpaceHSI, 
	 kColorSpaceHSV, kColorSpaceYCrCb, kColorSpaceYDrDb, kColorSpaceCMYK
} omfColorSpace_t;

typedef struct
{
	omfFrameOffset_t   	startFrame;
	omfDropType_t   drop;
	omfUInt16		fps;	/* frames per second */
}               omfTimecode_t;

typedef struct
{
	omfFrameOffset_t   startFrame;
	omfFilmType_t   filmKind;
	omfEdgeType_t codeFormat;
	char			header[8];
}               omfEdgecode_t;

/*** Types for clips ***/
typedef struct
{
	omfUID_t        sourceID;
	omfTrackID_t    sourceTrackID;
	omfPosition_t   startTime;
}               omfSourceRef_t;

typedef struct
{
  omfMobObj_t mob;
  omfTrackID_t mobTrackID;
  omfPosition_t position;
  omfRational_t editrate;
  omfLength_t minLength;
  omfObject_t		filmTapePdwn;
  omfInt32			filmTapePhase;
  omfObject_t		tapeFilmPdwn;
  omfInt32			tapeFilmPhase;
  omfObject_t		effeObject;
} omfFindSourceInfo_t;

typedef struct
{
	omfLength_t		fadeLength;
	omfFadeType_t	fadeType;
	omfRational_t	fadeEditUnit;
	omfBool			valid;				/* Are the above fields valid? */
} omfDefaultFade_t;	

/************************************************************
 *
 * Multiple Media Representations Criteria types
 *
 *************************************************************/
typedef omfInt16(*omfCriteriaProc_t)(omfHdl_t file, omfObject_t mdes, 
								  omfCodecID_t codecID);

typedef enum
{
	kOmfAnyRepresentation = 0,
	kOmfFastestRepresentation,
	kOmfBestFidelityRepresentation,
	kOmfSmallestRepresentation,
	kOmfUseRepresentationProc
}					omfCriteriaType_t;

typedef struct
{
	omfCriteriaType_t		type;
	omfCriteriaProc_t		proc;	
}               omfMediaCriteria_t;

/************************************************************
 *
 * Data Types for Search Criteria and Iterators 
 *
 *************************************************************/

typedef enum
{
	kNoSearch = 0, kByMobID, kByMobKind, kByName, kByClass, kByDatakind,
	kByMediaCrit
}               omfSearchTag_t;

typedef struct
{
	omfSearchTag_t     searchTag;
	union 
	{
		omfUID_t        mobID;
		omfMobKind_t    mobKind;
		omfString       name;
		omfClassID_t      objClass;
		omfUniqueName_t datakind;
		omfCriteriaType_t mediaCrit;
	} tags;
}               omfSearchCrit_t;

/************************************************************
 *
 * Callback types
 *
 *************************************************************/
typedef     omfHdl_t (*omfLocatorFailureCB) (omfHdl_t file, 
											 omfObject_t mdes);

typedef		void (*omfProgressProc_t) (omfHdl_t file, omfInt32 curVal, 
									   omfInt32 endVal);
 
/************************************************************
 *
 * codec API types
 *
 *************************************************************/
 typedef enum {
	kOMFRegisterLinked,
	kOMFRegisterPlugin
} omfCodecType_t;



#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_MEDIA_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
