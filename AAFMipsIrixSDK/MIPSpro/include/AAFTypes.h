#ifndef __AAFTypes_h__
#define __AAFTypes_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAFCOMPlatformTypes_h__
#include "AAFCOMPlatformTypes.h"
#endif




/*
 * Basic types that don't depend on other types.
 */


#if 0
// MIDL only defintion for 64 bit integer

#if ((!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64)))

typedef signed char		aafInt8;
typedef signed short int	aafInt16;
typedef signed long int		aafInt32;

typedef unsigned char		aafUInt8;
typedef unsigned short int	aafUInt16;
typedef unsigned long int	aafUInt32;

typedef hyper			aafInt64;
typedef unsigned hyper		aafUInt64;
#define AAF_INT64_NATIVE	1

typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;

#else
#error Using an incompatible midl host compiler!
#endif

#else
#include "AAFPlatform.h"

#endif  // #if 0


// Add compatibility with 32/64 conversion routines.
#undef PORT_USE_NATIVE64
#undef PORTKEY_INT64_TYPE
#if AAF_INT64_NATIVE
#define PORT_USE_NATIVE64 1
#define PORTKEY_INT64_TYPE aafInt64
#else
#define PORT_USE_NATIVE64 0
#define PORTKEY_INT64_TYPE aafInt64
#endif


typedef unsigned char * aafMemPtr_t;

typedef aafInt32		AAFRESULT;

#ifndef NULL
#ifdef __cplusplus
#define NULL (0L)
#else
#define NULL ((void *)0L)
#endif
#endif

typedef aafInt32 aafBoolean_t;
typedef enum _aafBoolean_e
{
  kAAFFalse = 0,
  kAAFTrue = 1
} aafBoolean_e;

#ifndef MAX_NUM_RGBA_COMPS
#define MAX_NUM_RGBA_COMPS 8 /* limit on the size of component arrays */
#if 0
typedef aafInt32 aafMAX_NUM_RGBA_COMPS;
typedef enum _aafMAX_NUM_RGBA_COMPS_e
{
  MAX_NUM_RGBA_COMPS = 8				// Lower to 6 to match type object
} aafMAX_NUM_RGBA_COMPS_e;
#endif
#endif


#if 0
typedef [string] aafCharacter * aafString_t;
#endif
typedef aafCharacter * aafString_t;

/************************************************************
 *
 * AAF types that map to an integral type or pointer to
 *  integeral type.
 *
 * These each map to an existing integral type or pointer.  Note
 * that aafClassID_t is a structure used to hold storage for a
 * classID, but most functions take an aafClassIDPtr_t reference.
 *
 *************************************************************/

/*** Types for all components***/
typedef aafInt64   		aafLength_t;

/*** Types for mob slots***/
typedef aafInt64  		aafPosition_t;
typedef aafInt64		aafFrameOffset_t;
typedef aafInt64		aafFrameLength_t;
typedef aafUInt32   	aafSlotID_t;  

typedef aafUInt32  		aafNumSlots_t;
typedef struct _aafUID_t   aafClassID_t;
typedef aafUInt8    	*aafDataValue_t;

// Used by all api methods that need to access an array of bytes.
typedef aafUInt8    	*aafDataBuffer_t;


/*** Types for operation groups ***/
typedef struct _aafUID_t   aafArgIDType_t;
	
/*** Types for plugins ***/
typedef struct _aafUID_t	aafHardwarePlatform_t;
typedef struct _aafUID_t	aafEngine_t;
typedef struct _aafUID_t	aafPluginAPI_t;
typedef struct _aafUID_t	aafCodecID_t;

/* typedefs for image processing */
typedef aafUInt8 	    aafCompSizeArray_t[MAX_NUM_RGBA_COMPS];
typedef aafInt8 	    aafCompArray_t[MAX_NUM_RGBA_COMPS];
typedef aafInt32 		aafVideoLineMap_t[2];
typedef aafInt32   		aafJPEGTableID_t; /* for TIFF objects */

/************************************************************
 *
 * Enumerated AAFI types.
 *
 *************************************************************/

typedef aafInt32 aafFileRev_t;
typedef enum _aafFileRev_e
{
  kAAFRev1 = 0
} aafFileRev_e;

typedef aafInt32 aafMediaOpenMode_t;
typedef enum _aafMediaOpenMode_e
{
	kAAFMediaOpenReadOnly = 0, 
	kAAFMediaOpenAppend = 1
} aafMediaOpenMode_e;
	
typedef aafInt32 aafFieldDom_t;
typedef enum _aafFieldDom_e
{
	kAAFDominantField1 = 0, 
	kAAFDominantField2 = 1, 
	kAAFNoDominant = 2
} aafFieldDom_e;

typedef aafInt32 aafFieldTop_t;
typedef enum _aafFieldTop_e
{
	kAAFTopField1 = 0, 
	kAAFTopField2 = 1, 
	kAAFTopFieldNone = 2
} aafFieldTop_e;

typedef aafInt32 aafAlphaTransparency_t;
typedef enum _aafAlphaTransparency_e
{
	kAAFMinValueTransparent = 0,
	kAAFMaxValueTransparent = 1
} aafAlphaTransparency_e;

typedef aafInt32 aafFieldNumber_t;
typedef enum _aafFieldNumber_e
{
	kAAFUnspecifiedField = 0,
	kAAFFieldOne = 1,
	kAAFFieldTwo = 2
} aafFieldNumber_e;

typedef aafInt32 aafPixelFormat_t;
typedef enum _aafPixelFormat_e
{
	kAAFPixNone = 0, 
	kAAFPixRGBA = 1, 
	kAAFPixYUV = 2
} aafPixelFormat_e;


typedef aafInt32 aafFileExistence_t;
typedef enum _aafFileExistence_e
{
  kAAFFileExistence_new = 1,
  kAAFFileExistence_existing = 2
} aafFileExistence_e;


typedef aafInt32 aafFileAccess_t;
typedef enum _aafFileAccess_e
{
  kAAFFileAccess_none  = 0,
  kAAFFileAccess_read  = 1 << 0,
  kAAFFileAccess_write = 1 << 1,
  kAAFFileAccess_modify = kAAFFileAccess_read | kAAFFileAccess_write
} aafFileAccess_e;


typedef aafInt32 aafJPEGcomponent_t;
#if 0
typedef enum _aafJPEGcomponent_e
{
	kAAFJcLuminance = 0, 
	kAAFJcChrominance = 1, 
	kAAFJcLuminanceFP16 = 2, 
	kAAFJcChrominanceFP16 = 3
} aafJPEGcomponent_e;
#endif

typedef enum _aafJPEGcomponent_e
{
kAAFJcLuminance = 0, 
kAAFJcChrominance = 1
#if ! STANDARD_JPEG_Q
, kAAFJcLuminanceFP16 = 2, 
kAAFJcChrominanceFP16 = 3
#endif
} aafJPEGcomponent_e;

typedef aafInt32 aafCompressEnable_t;
typedef enum _aafCompressEnable_e
{
	kAAFCompressionEnable = 0, /*--cf ? Replaced SDK with AAF */
	kAAFCompressionDisable = 1 /*--cf ? Replaced SDK with AAF */       
} aafCompressEnable_e;

typedef aafInt32 aafFileFormat_t;
typedef enum _aafFileFormat_e
{
	kAAFEssence = 0, /*--cf changed iMedia to Essense as per Tom's request */
	kAAFForeignMedia = 1
} aafFileFormat_e;

typedef aafInt32 aafDropType_t;
typedef enum _aafDropType_e
{
	kAAFTcNonDrop = 0, 
	kAAFTcDrop = 1
} aafDropType_e;

typedef aafInt32 aafFrameLayout_t;
typedef enum _aafFrameLayout_e
{
	kAAFFullFrame = 0, 
	kAAFSeparateFields = 1, 
	kAAFOneField = 2, 
	kAAFMixedFields = 3
} aafFrameLayout_e;
	
typedef aafInt32 aafColorSiting_t;
typedef enum _aafColorSiting_e
{
	kAAFCoSiting = 0, 
	kAAFAveraging = 1, 
	kAAFThreeTap = 2
} aafColorSiting_e;
	
typedef aafInt32 aafPropertyOpt_t;
typedef enum _aafPropertyOpt_e
{
	kAAFPropRequired = 0,
	kAAFPropOptional = 1
} aafPropertyOpt_e;

typedef aafInt32 AAFDataDefinitionMatch_t;
typedef enum _AAFDataDefinitionMatch_e
{
	kAAFExactMatch = 0,
	kAAFConvertTo = 1,
	kAAFConvertFrom = 2
} AAFDataDefinitionMatch_e;
	  
typedef aafInt32 aafAppendOption_t;
typedef enum _aafAppendOption_e
{
	kAAFAppend = 0,
	kAAFForceOverwrite = 1
} aafAppendOption_e;

typedef aafInt32 aafReferenceType_t;
typedef enum _aafReferenceType_e
{
	kAAFRefLimitMinimum = 0,
	kAAFRefLimitMaximum = 1,
	kAAFRefMinimum = 2,
	kAAFRefMaximum = 3,
	kAAFRefEnumvalue = 4
} aafReferenceType_e;

typedef aafInt32 aafSwapNeeded_t;
typedef enum _aafSwapNeeded_e
{
	kAAFSwapIfNeeded = 0,
	kAAFNeverSwap = 1
} aafSwapNeeded_e;

typedef aafInt32 aafTimecodeSourceType_t;
typedef enum _aafTimecodeSourceType_e
{
	kAAFTimecodeLTC = 0,
	kAAFTimecodeVITC = 1
} aafTimecodeSourceType_e;

/************************************************************
 *
 * Mob specific data types
 *
 *************************************************************/
typedef aafInt32 aafMobKind_t;
typedef enum _aafMobKind_e
{
	kAAFCompMob = 0, 
	kAAFMasterMob = 1, 
	kAAFFileMob = 2, 
	kAAFTapeMob = 3, 
	kAAFFilmMob = 4,
	kAAFPrimaryMob = 5, 
	kAAFAllMob = 6
} aafMobKind_e;

typedef aafInt32 aafOperationChoice_t;
typedef enum _aafOperationChoice_e
{
  kAAFFindNull = 0,
  kAAFFindIncoming = 1,
  kAAFFindOutgoing = 2,
  kAAFFindRender = 3,
  kAAFFindOperationSrc1 = 4,
  kAAFFindOperationSrc2 = 5,
  kAAFFindOperationSrc3 = 6,
  kAAFFindOperationSrc4 = 7, 
  kAAFFindOperationSrc5 = 8,
  kAAFFindOperationSrc6 = 9,
  kAAFFindOperationSrc7 = 10,
  kAAFFindOperationSrc8 = 11
} aafOperationChoice_e;

typedef aafInt32 aafDepend_t;
typedef enum _aafDepend_e
{
	kAAFNoFollowDepend = 0, 
	kAAFFollowDepend = 1
} aafDepend_e;

typedef aafInt32 aafIncMedia_t;
typedef enum _aafIncMedia_e
{
	kAAFNoIncludeMedia = 0,
	kAAFIncludeMedia = 1
} aafIncMedia_e;

/* aafIndexType_t: describes the types of HEAD indices */
typedef aafInt32 aafIndexType_t;
typedef enum _aafIndexType_e
{
	kAAFCompMobIndex = 0,
	kAAFSourceMobIndex = 1,
	kAAFMediaIndex = 2,
	kAAFPrimaryMobIndex = 3
} aafIndexType_e;

/* aafFadeType_t: describes values for SCLP fadein and fadeout types  */
typedef aafInt32 aafFadeType_t;
typedef enum _aafFadeType_e
{
	kAAFFadeNone = 0,
	kAAFFadeLinearAmp = 1,
	kAAFFadeLinearPower = 2
} aafFadeType_e;

typedef aafInt32 aafAudioSampleType_t;
typedef enum _aafAudioSampleType_e
{
	kAAFOffsetBinary = 0,
	kAAFSignedMagnitude = 1
} aafAudioSampleType_e;
	
typedef aafInt32 aafTapeCaseType_t;
typedef enum _aafTapeCaseType_e
{
	kAAFTapeCaseNull = 0,
	kAAFThreeFourthInchVideoTape = 1,
    	kAAFVHSVideoTape = 2,
	kAAF8mmVideoTape = 3,
	kAAFBetacamVideoTape = 4,
	kAAFCompactCassette = 5,
	kAAFDATCartridge = 6,
	kAAFNagraAudioTape = 7
} aafTapeCaseType_e;

typedef aafInt32 aafVideoSignalType_t;
typedef enum _aafVideoSignalType_e
{
	kAAFVideoSignalNull = 0,
	kAAFNTSCSignal = 1,
	kAAFPALSignal = 2,
	kAAFSECAMSignal = 3
} aafVideoSignalType_e;

typedef aafInt32 aafTapeFormatType_t;
typedef enum _aafTapeFormatType_e
{
	kAAFTapeFormatNull = 0,
	kAAFBetacamFormat = 1,
	kAAFBetacamSPFormat = 2,
	kAAFVHSFormat = 3,
	kAAFSVHSFormat = 4,
	kAAF8mmFormat = 5,
	kAAFHi8Format = 6
} aafTapeFormatType_e;

typedef aafInt32 aafAttributeKind_t;
typedef enum _aafAttributeKind_e
{
	kAAFNullAttribute = 0,
	kAAFIntegerAttribute = 1,
	kAAFStringAttribute = 2,
	kAAFObjectAttribute = 3
} aafAttributeKind_e;
	
typedef aafInt32 aafPulldownKind_t;
typedef enum _aafPulldownKind_e
{
	kAAFTwoThreePD = 0,
	kAAFPALPD = 1,
	kAAFOneToOneNTSC = 2,
	kAAFOneToOnePAL = 3
} aafPulldownKind_e;

typedef aafInt32	aafPhaseFrame_t;

typedef aafInt32 aafPulldownDir_t;
typedef enum _aafPulldownDir_e
{
	kAAFTapeToFilmSpeed = 0,
	kAAFFilmToTapeSpeed = 1
}	aafPulldownDir_e;

typedef struct _aafMobComment_t
{
	aafString_t category;
	aafString_t comment;
} aafMobComment_t;

typedef aafInt32 aafRGBAComponentKind_t;
typedef enum _aafRGBAComponentKind_e
{
	kAAFCompNone = 0x30,
	kAAFCompAlpha = 0x41,
	kAAFCompBlue = 0x42,
	kAAFCompFill = 0x46,
	kAAFCompGreen = 0x47,
	kAAFCompPalette = 0x50,
	kAAFCompRed = 0x52
} aafRGBAComponentKind_e;

typedef struct _aafRGBAComponent_t
{
	aafRGBAComponentKind_t Code;
	aafUInt8               Size;
} aafRGBAComponent_t;



/************************************************************
 *
 * Operation Group Types
 *
 *************************************************************/	  
typedef aafInt32 aafInterpKind_t;
typedef enum _aafInterpKind_e
{
	kAAFConstInterp = 1, 
	kAAFLinearInterp = 2
} aafInterpKind_e;

typedef aafInt32 aafEditHint_t;
typedef enum _aafEditHint_e
{
    	kAAFNoEditHint = 0, 
	kAAFProportional = 1, 
	kAAFRelativeLeft = 2, 
	kAAFRelativeRight = 3, 
	kAAFRelativeFixed = 4
} aafEditHint_e;

typedef aafInt32 aafProductReleaseType_t;
typedef enum _aafProductReleaseType_e
{
	kAAFVersionUnknown = 0, 
	kAAFVersionReleased = 1, 
	kAAFVersionDebug = 2,
	kAAFVersionPatched = 3, 
	kAAFVersionBeta = 4, 
	kAAFVersionPrivateBuild = 5
} aafProductReleaseType_e;

/************************************************************
 *
 * Structured AAFI types.
 *
 *************************************************************/
typedef struct _aafVersionType_t
{
	aafInt8            major;
	aafInt8            minor;
} aafVersionType_t;

typedef struct _aafProductVersion_t
{
	aafUInt16				major;
	aafUInt16				minor;
	aafUInt16				tertiary;
	aafUInt16				patchLevel;
	aafProductReleaseType_t	type;
} aafProductVersion_t;

typedef struct _aafUID_t
{
//	aafUInt32           value[4]; // binary compatibility with GUID/CLSID and IID structures.
    aafUInt32 Data1;
    aafUInt16 Data2;
    aafUInt16 Data3;
    aafUInt8  Data4[8];
} aafUID_t;

typedef struct  _aafMobID_t
{
    aafUInt8			SMPTELabel[12];		// 12-bytes of label prefix
	aafUInt8			length;
    aafUInt8			instanceHigh;
    aafUInt8			instanceMid;
    aafUInt8			instanceLow;
    struct _aafUID_t	material;
}	aafMobID_t;



typedef struct _aafProductIdentification_t
{
	aafCharacter *             companyName;
	aafCharacter *             productName;
	aafCharacter *             productVersionString;
	aafUID_t                   productID;
    aafCharacter *             platform;        // optional
    aafProductVersion_t *      productVersion;  // optional
} aafProductIdentification_t;

extern const aafProductVersion_t AAFReferenceImplementationVersion;



typedef struct _aafDateStruct_t
{
  aafInt16 year;   /* range -32,767 to +32767 */
  aafUInt8 month;  /* range: 1-12, inclusive */
  aafUInt8 day;    /* range: 1-31, inclusive */
} aafDateStruct_t;

typedef struct _aafTimeStruct_t
{
  aafUInt8 hour;     /* range 0-23 inclusive */
  aafUInt8 minute;   /* range 0-59 inclusive */
  aafUInt8 second;   /* range 0-59 inclusive */
  aafUInt8 fraction; /* range 0..99 inclusive; accuracy: .01 sec */
} aafTimeStruct_t;

typedef struct _aafTimeStamp_t
{
  aafDateStruct_t date;
  aafTimeStruct_t time;
} aafTimeStamp_t;



typedef struct _aafRational_t
{
	aafInt32           numerator;
	aafInt32           denominator;
} aafRational_t;

typedef struct _aafRect_t
{
	aafInt32           xOffset;
	aafInt32           yOffset;
	aafInt32           xSize;
	aafInt32           ySize;
} aafRect_t;

typedef struct _aafmMultiXfer_t
{
    aafUID_t *	        mediaKind;		/* IN -- */
	aafInt16			subTrackNum;	/* IN - The physical output channel */
	aafUInt32           numSamples;		/* IN -- */
	aafUInt32          	buflen;			/* IN -- */
	aafDataBuffer_t		buffer;			/* IN/OUT -- */
} aafmMultiXfer_t;

typedef struct _aafmMultiResult_t
{
	aafUInt32          	bytesXfered;	/* OUT -- */
	aafUInt32			samplesXfered;	/* OUT */
} aafmMultiResult_t;

typedef struct _aafmMultiCreate_t
{
	const aafUID_t *	mediaKind;		/* IN -- */
	aafInt16			subTrackNum;	/* IN - The physical output channel */
	aafInt32           	slotID;		/* IN -- */
	aafRational_t		sampleRate;
} aafmMultiCreate_t;

/* Typedefs specific to edgecode and timecode */
typedef aafInt32 aafEdgeType_t;
typedef enum _aafEdgeType_e
{
	kAAFEtNull = 0, 
	kAAFEtKeycode = 1, 
	kAAFEtEdgenum4 = 2, 
	kAAFEtEdgenum5 = 3,
	kAAFEtHeaderSize = 8
} aafEdgeType_e;

typedef aafInt32 aafFilmType_t;
typedef enum _aafFilmType_e
{
	kAAFFtNull = 0, 
	kAAFFt35MM = 1, 
	kAAFFt16MM = 2, 
	kAAFFt8MM = 3, 
	kAAFFt65MM = 4
} aafFilmType_e;

typedef aafInt32 aafDirectionCode_t;
typedef enum _aafDirectionCode_e
{
    	kAAFDirCodeRight = 0, 
	kAAFDirCodeUpperRight = 1, 
	kAAFDirCodeUp = 2, 
	kAAFDirCodeUpperLeft = 3, 
	kAAFDirCodeLeft = 4, 
	kAAFDirCodeLowerLeft = 5, 
	kAAFDirCodeDown = 6, 
	kAAFDirCodeLowerRight = 7
} aafDirectionCode_e;

typedef aafInt32 aafColorSpace_t;
typedef enum _aafColorSpace_e
{
     	kAAFColorSpaceRGB = 0, 
	kAAFColorSpaceYUV = 1, 
	kAAFColorSpaceYIQ = 2, 
	kAAFColorSpaceHSI = 3, 
	kAAFColorSpaceHSV = 4, 
	kAAFColorSpaceYCrCb = 5, 
	kAAFColorSpaceYDrDb = 6, 
	kAAFColorSpaceCMYK = 7
} aafColorSpace_e;

typedef struct _aafTimecode_t
{
	aafFrameOffset_t   	startFrame;
	aafDropType_t   drop;
	aafUInt16		fps;	/* frames per second */
} aafTimecode_t;

typedef aafUInt8 aafEdgecodeHeader_t[kAAFEtHeaderSize];

typedef struct _aafEdgecode_t
{
	aafFrameOffset_t   startFrame;
	aafFilmType_t   filmKind;
	aafEdgeType_t codeFormat;
	aafEdgecodeHeader_t header;
} aafEdgecode_t;

/*** Types for clips ***/
typedef struct _aafSourceRef_t
{
	aafMobID_t        sourceID;
	aafSlotID_t    sourceSlotID;
	aafPosition_t   startTime;
} aafSourceRef_t;


typedef struct _aafDefaultFade_t
{
	aafLength_t		fadeLength;
	aafFadeType_t	fadeType;
	aafRational_t	fadeEditUnit;
	aafBoolean_t	valid;		/* Are the above fields valid? */
} aafDefaultFade_t;	

/************************************************************
 *
 * Multiple Media Representations Criteria types
 *
 *************************************************************/
//typedef aafInt16(*aafCriteriaProc_t)(IAAFFile  * file,
//                                     IAAFObject  * mdes, 
//                                     aafCodecID_t codecID);

typedef aafInt32 aafCriteriaType_t;
typedef enum _aafCriteriaType_e
{
	kAAFAnyRepresentation = 0,
	kAAFFastestRepresentation = 1,
	kAAFBestFidelityRepresentation = 2,
	kAAFSmallestRepresentation = 3 //, Jeff may add this one back.
//	kAAFUseRepresentationProc = 4
} aafCriteriaType_e;

typedef struct _aafMediaCriteria_t
{
	aafCriteriaType_t		type;
//	aafCriteriaProc_t		proc;	
} aafMediaCriteria_t;

/************************************************************
 *
 * Data Types for Search Criteria and Iterators 
 *
 *************************************************************/

typedef aafInt32 aafSearchTag_t;
typedef enum _aafSearchTag_e
{
	kAAFNoSearch = 0, 
	kAAFByMobID = 1, 
	kAAFByMobKind = 2, 
	kAAFByName = 3, 
	kAAFByClass = 4, 
	kAAFByDataDef = 5,
	kAAFByMediaCrit = 6
} aafSearchTag_e;


#if 0
typedef struct _aafSearchCrit_t
{

    aafSearchTag_t     searchTag;
    [switch_is(searchTag)] union
    {
        [case(kAAFByMobID)]
			aafMobID_t mobID;		// shouldn't this be a pointer?
        [case(kAAFByMobKind)]
			aafMobKind_t mobKind;
        [case(kAAFByName)]
			aafString_t name;
        [case(kAAFByClass)]
			aafClassID_t objClass;	// shouldn't this be a pointer?
        [case(kAAFByDataDef)]
			aafUID_t datadef;	// shouldn't this be a pointer?
        [case(kAAFByMediaCrit)]
			aafCriteriaType_t mediaCrit;
        [default] ;
    } ;

} aafSearchCrit_t;
#endif
//
typedef struct _aafSearchCrit_t
{
    aafSearchTag_t     searchTag;
    union
    {
        aafMobID_t        mobID;		// shouldn't this be a pointer?
        aafMobKind_t    mobKind;
        aafString_t       name;
        aafClassID_t      objClass;	// shouldn't this be a pointer?
        aafUID_t		 datadef;	// shouldn't this be a pointer?
        aafCriteriaType_t mediaCrit;
    } tags;
} aafSearchCrit_t;



typedef aafInt32 aafDefinitionKind_t;
typedef enum _aafDefinitionKind_e
{
	kAAFClassDefinition	    = 1,
	kAAFPropertyDefinition  = 2,
	kAAFTypeDefinition      = 4,
	kAAFDataDefinition      = 8,
	kAAFOperationDefinition    = 16,
	kAAFAllDefinitions      = -1
} aafDefinitionKind_e;

typedef aafInt32 aafDefinitionCritType_t;
typedef enum _aafDefinitionCritType_e
{
	kAAFNoDefSearch = 0, 
	kAAFByDefKind = 1, 
	kAAFByDefName = 2, 
	kAAFByDefClass  = 3
} aafDefinitionCritType_e;

typedef struct _aafDefinitionCrit_t
{
	aafDefinitionCritType_t type;
	union 
	{
		aafDefinitionKind_t	defKind;
		aafString_t         name;
		aafClassID_t        objClass;	// shouldn't this be a pointer?
	} tags;
} aafDefinitionCrit_t;

typedef aafInt32 aafIdentificationCritType_t;
typedef enum _aafIdentificationCritType_e
{
	kAAFNoIdSearch = 0, 
	kAAFByIdProductID = 1, 
	kAAFByIdGeneration = 2, 
	kAAFByIdRefImplVersion = 3
} aafIdentificationCritType_e;

typedef struct _aafIdentificationCrit_t
{
	aafIdentificationCritType_t type;
	union 
	{
		aafUID_t		    productID;
		aafUID_t            generation;
		aafProductVersion_t referenceImplementationVersion;
	} tags;
} aafIdentificationCrit_t;


/************************************************************ 
 *
 * Data Types for Searching the Class Dictionary 
 *
 *************************************************************/ 

typedef aafInt32 aafSearchDictionaryTag_t;
typedef enum _aafSearchDictionaryTag_e
{
	kAAFAllClasses = 0,       // selects all classes in the given class dictionary.
	kAAFByClassID = 1,	       // selects the class with the given class id.
	kAAFByParentClassID = 2,  // selects all classes with the given parent class id. 
	kAAFByPropertyID = 3,     // selects all classes that contain the given property id.
	kAAFByTypeID = 4,         // selects all classes that contain properties with the given type id.
	kAAFByClassName = 5       // selects all classes with the given name
} aafSearchDictionaryTag_e;


//********
//
// Enum indicating data "Endian-ness"
// 
typedef aafInt32 eAAFByteOrder_t;
typedef enum _eAAFByteOrder_e
{
  //
  // Little Endian (e.g. Intel byte order)
  //
  kAAFByteOrderLittle	= 0,

  //
  // Big Endian (e.g. Mac or Sparc byte order)
  //
  kAAFByteOrderBig	= 1

} eAAFByteOrder_e;


#if 0
typedef struct _aafSearchDictionaryCrit_t
{

    aafSearchDictionaryTag_t     searchTag;
    [switch_is(searchTag)] union
    {
        [case(kAAFByClassID)]
            aafUID_t    * classID;
        [case(kAAFByParentClassID)]
            aafUID_t    * parentClassID;
        [case(kAAFByPropertyID)]
            aafUID_t	* propertyID;
        [case(kAAFByTypeID)]
            aafUID_t	* typeID;
        [case(kAAFByClassName)]
            aafString_t   name;
        [default] ;
    } ;

} aafSearchDictionaryCrit_t;


//********
//
// Enum indicating general category of stored (property) data
//
//
#endif
//
typedef struct _aafSearchDictionaryCrit_t
{
	aafSearchDictionaryTag_t     searchTag;
	union 
	{
		aafUID_t    * classID;
		aafUID_t    * parentClassID;
		aafUID_t	* propertyID;
		aafUID_t	* typeID;
		aafString_t   name;
	} tags;
} aafSearchDictionaryCrit_t;



typedef aafInt32 eAAFTypeCategory_t;
typedef enum _eAAFTypeCategory_e
{
  kAAFTypeCatUnknown       = 0,  // can only occur in damaged files
  kAAFTypeCatInt           = 1,  // any integral type
  kAAFTypeCatCharacter     = 2,  // any character type
  kAAFTypeCatStrongObjRef  = 3,  // strong object reference
  kAAFTypeCatWeakObjRef    = 4,  // weak object reference
  kAAFTypeCatRename        = 5,  // renamed type
  kAAFTypeCatEnum          = 6,  // enumerated type
  kAAFTypeCatFixedArray    = 7,  // fixed-size array
  kAAFTypeCatVariableArray = 8,  // variably-sized array
  kAAFTypeCatSet           = 9,  // set of strong object references or
                                 // set of weak object references
  kAAFTypeCatRecord        = 10, // a structured type
  kAAFTypeCatStream        = 11, // potentially huge amount of data
  kAAFTypeCatString        = 12, // null-terminated variably-sized
                                 // array of characters
  kAAFTypeCatExtEnum       = 13, // extendible enumerated type
  kAAFTypeCatIndirect      = 14, // type must be determined at runtime
  kAAFTypeCatOpaque        = 15, // type can be determined at runtime
  kAAFTypeCatEncrypted     = 16  // type can be determined at runtime 
                                 // but bits are encrypted
} eAAFTypeCategory_e;


// typedefs to make things more consistent
typedef aafBoolean_t            aafBool;
typedef aafColorSiting_t        aafColorSitingType_t;
typedef aafEditHint_t           aafEditHintType_t;
typedef aafFrameLayout_t        aafLayoutType_t;
typedef aafTimecodeSourceType_t aafTCSource_t;
typedef aafPulldownDir_t        aafPulldownDirectionType_t;
typedef aafPulldownKind_t       aafPulldownKindType_t;


/**************************/
#ifndef STDMETHODCALLTYPE
  #ifndef _MSC_VER
    #ifdef __SC__
      #define STDMETHODCALLTYPE _cdecl
    #else
      #define STDMETHODCALLTYPE
    #endif
  #else
    #define STDMETHODCALLTYPE __stdcall
  #endif
#endif



#if 0
  #if defined(__midl)
    /* MIDL 2.0 definitions */
	typedef aafArgIDType_t *             aafArgIDType_constref;
	typedef aafEdgecodeHeader_t *        aafEdgecodeHeader_constref;
	typedef aafEdgecode_t *              aafEdgecode_constref;
	typedef aafEngine_t *                aafEngine_constref;
	typedef aafHardwarePlatform_t *      aafHardwarePlatform_constref;
	typedef aafMediaCriteria_t *         aafMediaCriteria_constref;
	typedef aafPluginAPI_t *             aafPluginAPI_constref;
	typedef aafPosition_t *              aafPosition_constref;
	typedef aafProductIdentification_t * aafProductIdentification_constref;
	typedef aafProductVersion_t *        aafProductVersion_constref;
	typedef aafRect_t *                  aafRect_constref;
	typedef aafSourceRef_t *             aafSourceRef_constref;
	typedef aafTimecode_t *              aafTimecode_constref;
    typedef aafLength_t *                aafLength_constref;
    typedef aafRational_t *              aafRational_constref;
    typedef aafTimeStamp_t *             aafTimeStamp_constref;
    typedef aafUID_t *                   aafUID_constref;
    typedef aafMobID_t *                 aafMobID_constref;
    typedef aafVersionType_t *           aafVersionType_constref;
  #else
    /* MIDL 1.1 definitions */
	typedef aafArgIDType_t               aafArgIDType_constref;
	typedef aafEdgecodeHeader_t          aafEdgecodeHeader_constref;
	typedef aafEdgecode_t                aafEdgecode_constref;
	typedef aafEngine_t                  aafEngine_constref;
	typedef aafHardwarePlatform_t        aafHardwarePlatform_constref;
	typedef aafMediaCriteria_t           aafMediaCriteria_constref;
	typedef aafPluginAPI_t               aafPluginAPI_constref;
	typedef aafPosition_t                aafPosition_constref;
	typedef aafProductIdentification_t   aafProductIdentification_constref;
	typedef aafProductVersion_t          aafProductVersion_constref;
	typedef aafRect_t                    aafRect_constref;
	typedef aafSourceRef_t               aafSourceRef_constref;
	typedef aafTimecode_t                aafTimecode_constref;
    typedef aafLength_t                  aafLength_constref;
    typedef aafRational_t                aafRational_constref;
    typedef aafTimeStamp_t               aafTimeStamp_constref;
    typedef aafUID_t                     aafUID_constref;
    typedef aafMobID_t                   aafMobID_constref;
    typedef aafVersionType_t             aafVersionType_constref;
  #endif

  typedef unsigned char *                aafMemConstPtr_t;
  typedef aafCharacter *                 aafCharacter_constptr;
  typedef aafMediaCriteria_t *           aafMediaCriteria_constptr;
  typedef aafOperationChoice_t *         aafOperationChoice_constptr;
  typedef aafProductVersion_t *          aafProductVersion_constptr;
  typedef aafProductIdentification_t *   aafProductIdentification_constptr;
  typedef aafSearchCrit_t *              aafSearchCrit_constptr;
  typedef aafUID_t *                     aafUID_constptr;
  typedef aafMobID_t *                   aafMobID_constptr;

#endif // 0

#if defined(__cplusplus)
#define aafArgIDType_constref        const aafArgIDType_t &
#define aafEdgecodeHeader_constref   const aafEdgecodeHeader_t &
#define aafEdgecode_constref         const aafEdgecode_t &
#define aafEngine_constref           const aafEngine_t &
#define aafHardwarePlatform_constref const aafHardwarePlatform_t &
#define aafLength_constref           const aafLength_t &
#define aafMediaCriteria_constref    const aafMediaCriteria_t &
#define aafPluginAPI_constref        const aafPluginAPI_t &
#define aafPosition_constref         const aafPosition_t &
#define aafProductIdentification_constref const aafProductIdentification_t &
#define aafProductVersion_constref   const aafProductVersion_t &
#define aafRational_constref         const aafRational_t &
#define aafRect_constref             const aafRect_t &
#define aafSourceRef_constref        const aafSourceRef_t &
#define aafTimeStamp_constref        const aafTimeStamp_t &
#define aafTimecode_constref         const aafTimecode_t &
#define aafUID_constref              const aafUID_t &
#define aafMobID_constref            const aafMobID_t &
#define aafVersionType_constref      const aafVersionType_t &
#else // !__cplusplus
#define aafArgIDType_constref         const aafArgIDType_t * const
#define aafEdgecodeHeader_constref    const aafEdgecodeHeader_t * const
#define aafEdgecode_constref          const aafEdgecode_t * const
#define aafEngine_constref            const aafEngine_t * const
#define aafHardwarePlatform_constref  const aafHardwarePlatform_t * const
#define aafLength_constref            const aafLength_t * const
#define aafMediaCriteria_constref     const aafMediaCriteria_t * const
#define aafPluginAPI_constref         const aafPluginAPI_t * const
#define aafPosition_constref          const aafPosition_t * const
#define aafProductIdentification_constref const aafProductIdentification_t * const
#define aafProductVersion_constref    const aafProductVersion_t * const
#define aafRational_constref          const aafRational_t * const
#define aafRect_constref              const aafRect_t * const
#define aafSourceRef_constref         const aafSourceRef_t * const
#define aafTimeStamp_constref         const aafTimeStamp_t * const
#define aafTimecode_constref          const aafTimecode_t * const
#define aafUID_constref               const aafUID_t * const
#define aafMobID_constref             const aafMobID_t * const
#define aafVersionType_constref       const aafVersionType_t * const
#endif // !__cplusplus

#define aafMemConstPtr_t              const unsigned char *
#define aafCharacter_constptr         const aafCharacter *
#define aafMediaCriteria_constptr     const aafMediaCriteria_t *
#define aafOperationChoice_constptr   const aafOperationChoice_t *
#define aafProductVersion_constptr    const aafProductVersion_t *
#define aafProductIdentification_constptr const aafProductIdentification_t *
#define aafSearchCrit_constptr        const aafSearchCrit_t *
#define aafUID_constptr               const aafUID_t *
#define aafMobID_constptr             const aafMobID_t *


#endif // #ifndef __AAFTypes_h__
