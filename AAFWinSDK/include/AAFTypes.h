/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at TIMESTAMP REMOVED
 */
/* Compiler settings for ../ref-impl/include/com-api/AAFTypes.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __AAFTypes_h__
#define __AAFTypes_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

/* header files for imported files */
#include "wtypes.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_AAFTypes_0000 */
/* [local] */ 

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


extern RPC_IF_HANDLE __MIDL_itf_AAFTypes_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AAFTypes_0000_v0_0_s_ifspec;

#ifndef __IAAFTypes_INTERFACE_DEFINED__
#define __IAAFTypes_INTERFACE_DEFINED__

/* interface IAAFTypes */
/* [auto_handle][unique][uuid] */ 

/*
 * Basic types that don't depend on other types.
 */
#if 0
// MIDL only defintion for 64 bit integer
typedef signed char aafInt8;

typedef short aafInt16;

typedef long aafInt32;

typedef unsigned char aafUInt8;

typedef unsigned short aafUInt16;

typedef unsigned long aafUInt32;

typedef hyper aafInt64;

typedef MIDL_uhyper aafUInt64;

typedef wchar_t aafWChar;

typedef wchar_t aafCharacter;

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
typedef unsigned char __RPC_FAR *aafMemPtr_t;

typedef aafInt32 AAFRESULT;

#ifndef NULL
#ifdef __cplusplus
#define NULL (0L)
#else
#define NULL ((void *)0L)
#endif
#endif
typedef aafInt32 aafBoolean_t;

typedef 
enum _aafBoolean_e
    {	kAAFFalse	= 0,
	kAAFTrue	= 1
    }	aafBoolean_e;

#ifndef MAX_NUM_RGBA_COMPS
#define MAX_NUM_RGBA_COMPS 8 /* limit on the size of component arrays */
#if 0
typedef aafInt32 aafMAX_NUM_RGBA_COMPS;

typedef 
enum _aafMAX_NUM_RGBA_COMPS_e
    {	MAX_NUM_RGBA_COMPS	= 8
    }	aafMAX_NUM_RGBA_COMPS_e;

#endif
#endif
#if 0
typedef /* [string] */ aafCharacter __RPC_FAR *aafString_t;

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
typedef aafInt64 aafLength_t;

/*** Types for mob slots***/
typedef aafInt64 aafPosition_t;

typedef aafInt64 aafFrameOffset_t;

typedef aafInt64 aafFrameLength_t;

typedef aafUInt32 aafSlotID_t;

typedef aafUInt32 aafNumSlots_t;

typedef struct _aafUID_t aafClassID_t;

typedef aafUInt8 __RPC_FAR *aafDataValue_t;

typedef aafUInt8 __RPC_FAR *aafDataBuffer_t;

/*** Types for operation groups ***/
typedef struct _aafUID_t aafArgIDType_t;

/*** Types for plugins ***/
typedef struct _aafUID_t aafHardwarePlatform_t;

typedef struct _aafUID_t aafEngine_t;

typedef struct _aafUID_t aafPluginAPI_t;

typedef struct _aafUID_t aafCodecID_t;

/* typedefs for image processing */
typedef aafUInt8 __RPC_FAR aafCompSizeArray_t[ 8 ];

typedef aafInt8 __RPC_FAR aafCompArray_t[ 8 ];

typedef aafInt32 __RPC_FAR aafVideoLineMap_t[ 2 ];

typedef aafInt32 aafJPEGTableID_t;

/************************************************************
 *
 * Enumerated AAFI types.
 *
 *************************************************************/
typedef aafInt32 aafFileRev_t;

typedef 
enum _aafFileRev_e
    {	kAAFRev1	= 0
    }	aafFileRev_e;

typedef aafInt32 aafMediaOpenMode_t;

typedef 
enum _aafMediaOpenMode_e
    {	kAAFMediaOpenReadOnly	= 0,
	kAAFMediaOpenAppend	= 1
    }	aafMediaOpenMode_e;

typedef aafInt32 aafFieldDom_t;

typedef 
enum _aafFieldDom_e
    {	kAAFDominantField1	= 0,
	kAAFDominantField2	= 1,
	kAAFNoDominant	= 2
    }	aafFieldDom_e;

typedef aafInt32 aafFieldTop_t;

typedef 
enum _aafFieldTop_e
    {	kAAFTopField1	= 0,
	kAAFTopField2	= 1,
	kAAFTopFieldNone	= 2
    }	aafFieldTop_e;

typedef aafInt32 aafAlphaTransparency_t;

typedef 
enum _aafAlphaTransparency_e
    {	kAAFMinValueTransparent	= 0,
	kAAFMaxValueTransparent	= 1
    }	aafAlphaTransparency_e;

typedef aafInt32 aafFieldNumber_t;

typedef 
enum _aafFieldNumber_e
    {	kAAFUnspecifiedField	= 0,
	kAAFFieldOne	= 1,
	kAAFFieldTwo	= 2
    }	aafFieldNumber_e;

typedef aafInt32 aafPixelFormat_t;

typedef 
enum _aafPixelFormat_e
    {	kAAFPixNone	= 0,
	kAAFPixRGBA	= 1,
	kAAFPixYUV	= 2
    }	aafPixelFormat_e;

typedef aafInt32 aafFileExistence_t;

typedef 
enum _aafFileExistence_e
    {	kAAFFileExistence_new	= 1,
	kAAFFileExistence_existing	= 2
    }	aafFileExistence_e;

typedef aafInt32 aafFileAccess_t;

typedef 
enum _aafFileAccess_e
    {	kAAFFileAccess_none	= 0,
	kAAFFileAccess_read	= 1 << 0,
	kAAFFileAccess_write	= 1 << 1,
	kAAFFileAccess_modify	= kAAFFileAccess_read | kAAFFileAccess_write
    }	aafFileAccess_e;

typedef aafInt32 aafJPEGcomponent_t;

#if 0
typedef 
enum _aafJPEGcomponent_e
    {	kAAFJcLuminance	= 0,
	kAAFJcChrominance	= 1,
	kAAFJcLuminanceFP16	= 2,
	kAAFJcChrominanceFP16	= 3
    }	aafJPEGcomponent_e;

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

typedef 
enum _aafCompressEnable_e
    {	kAAFCompressionEnable	= 0,
	kAAFCompressionDisable	= 1
    }	aafCompressEnable_e;

typedef aafInt32 aafFileFormat_t;

typedef 
enum _aafFileFormat_e
    {	kAAFEssence	= 0,
	kAAFForeignMedia	= 1
    }	aafFileFormat_e;

typedef aafInt32 aafDropType_t;

typedef 
enum _aafDropType_e
    {	kAAFTcNonDrop	= 0,
	kAAFTcDrop	= 1
    }	aafDropType_e;

typedef aafInt32 aafFrameLayout_t;

typedef 
enum _aafFrameLayout_e
    {	kAAFFullFrame	= 0,
	kAAFSeparateFields	= 1,
	kAAFOneField	= 2,
	kAAFMixedFields	= 3
    }	aafFrameLayout_e;

typedef aafInt32 aafColorSiting_t;

typedef 
enum _aafColorSiting_e
    {	kAAFCoSiting	= 0,
	kAAFAveraging	= 1,
	kAAFThreeTap	= 2
    }	aafColorSiting_e;

typedef aafInt32 aafPropertyOpt_t;

typedef 
enum _aafPropertyOpt_e
    {	kAAFPropRequired	= 0,
	kAAFPropOptional	= 1
    }	aafPropertyOpt_e;

typedef aafInt32 AAFDataDefinitionMatch_t;

typedef 
enum _AAFDataDefinitionMatch_e
    {	kAAFExactMatch	= 0,
	kAAFConvertTo	= 1,
	kAAFConvertFrom	= 2
    }	AAFDataDefinitionMatch_e;

typedef aafInt32 aafAppendOption_t;

typedef 
enum _aafAppendOption_e
    {	kAAFAppend	= 0,
	kAAFForceOverwrite	= 1
    }	aafAppendOption_e;

typedef aafInt32 aafReferenceType_t;

typedef 
enum _aafReferenceType_e
    {	kAAFRefLimitMinimum	= 0,
	kAAFRefLimitMaximum	= 1,
	kAAFRefMinimum	= 2,
	kAAFRefMaximum	= 3,
	kAAFRefEnumvalue	= 4
    }	aafReferenceType_e;

typedef aafInt32 aafSwapNeeded_t;

typedef 
enum _aafSwapNeeded_e
    {	kAAFSwapIfNeeded	= 0,
	kAAFNeverSwap	= 1
    }	aafSwapNeeded_e;

typedef aafInt32 aafTimecodeSourceType_t;

typedef 
enum _aafTimecodeSourceType_e
    {	kAAFTimecodeLTC	= 0,
	kAAFTimecodeVITC	= 1
    }	aafTimecodeSourceType_e;

/************************************************************
 *
 * Mob specific data types
 *
 *************************************************************/
typedef aafInt32 aafMobKind_t;

typedef 
enum _aafMobKind_e
    {	kAAFCompMob	= 0,
	kAAFMasterMob	= 1,
	kAAFFileMob	= 2,
	kAAFTapeMob	= 3,
	kAAFFilmMob	= 4,
	kAAFPrimaryMob	= 5,
	kAAFAllMob	= 6
    }	aafMobKind_e;

typedef aafInt32 aafOperationChoice_t;

typedef 
enum _aafOperationChoice_e
    {	kAAFFindNull	= 0,
	kAAFFindIncoming	= 1,
	kAAFFindOutgoing	= 2,
	kAAFFindRender	= 3,
	kAAFFindOperationSrc1	= 4,
	kAAFFindOperationSrc2	= 5,
	kAAFFindOperationSrc3	= 6,
	kAAFFindOperationSrc4	= 7,
	kAAFFindOperationSrc5	= 8,
	kAAFFindOperationSrc6	= 9,
	kAAFFindOperationSrc7	= 10,
	kAAFFindOperationSrc8	= 11
    }	aafOperationChoice_e;

typedef aafInt32 aafDepend_t;

typedef 
enum _aafDepend_e
    {	kAAFNoFollowDepend	= 0,
	kAAFFollowDepend	= 1
    }	aafDepend_e;

typedef aafInt32 aafIncMedia_t;

typedef 
enum _aafIncMedia_e
    {	kAAFNoIncludeMedia	= 0,
	kAAFIncludeMedia	= 1
    }	aafIncMedia_e;

/* aafIndexType_t: describes the types of HEAD indices */
typedef aafInt32 aafIndexType_t;

typedef 
enum _aafIndexType_e
    {	kAAFCompMobIndex	= 0,
	kAAFSourceMobIndex	= 1,
	kAAFMediaIndex	= 2,
	kAAFPrimaryMobIndex	= 3
    }	aafIndexType_e;

/* aafFadeType_t: describes values for SCLP fadein and fadeout types  */
typedef aafInt32 aafFadeType_t;

typedef 
enum _aafFadeType_e
    {	kAAFFadeNone	= 0,
	kAAFFadeLinearAmp	= 1,
	kAAFFadeLinearPower	= 2
    }	aafFadeType_e;

typedef aafInt32 aafAudioSampleType_t;

typedef 
enum _aafAudioSampleType_e
    {	kAAFOffsetBinary	= 0,
	kAAFSignedMagnitude	= 1
    }	aafAudioSampleType_e;

typedef aafInt32 aafTapeCaseType_t;

typedef 
enum _aafTapeCaseType_e
    {	kAAFTapeCaseNull	= 0,
	kAAFThreeFourthInchVideoTape	= 1,
	kAAFVHSVideoTape	= 2,
	kAAF8mmVideoTape	= 3,
	kAAFBetacamVideoTape	= 4,
	kAAFCompactCassette	= 5,
	kAAFDATCartridge	= 6,
	kAAFNagraAudioTape	= 7
    }	aafTapeCaseType_e;

typedef aafInt32 aafVideoSignalType_t;

typedef 
enum _aafVideoSignalType_e
    {	kAAFVideoSignalNull	= 0,
	kAAFNTSCSignal	= 1,
	kAAFPALSignal	= 2,
	kAAFSECAMSignal	= 3
    }	aafVideoSignalType_e;

typedef aafInt32 aafTapeFormatType_t;

typedef 
enum _aafTapeFormatType_e
    {	kAAFTapeFormatNull	= 0,
	kAAFBetacamFormat	= 1,
	kAAFBetacamSPFormat	= 2,
	kAAFVHSFormat	= 3,
	kAAFSVHSFormat	= 4,
	kAAF8mmFormat	= 5,
	kAAFHi8Format	= 6
    }	aafTapeFormatType_e;

typedef aafInt32 aafAttributeKind_t;

typedef 
enum _aafAttributeKind_e
    {	kAAFNullAttribute	= 0,
	kAAFIntegerAttribute	= 1,
	kAAFStringAttribute	= 2,
	kAAFObjectAttribute	= 3
    }	aafAttributeKind_e;

typedef aafInt32 aafPulldownKind_t;

typedef 
enum _aafPulldownKind_e
    {	kAAFTwoThreePD	= 0,
	kAAFPALPD	= 1,
	kAAFOneToOneNTSC	= 2,
	kAAFOneToOnePAL	= 3
    }	aafPulldownKind_e;

typedef aafInt32 aafPhaseFrame_t;

typedef aafInt32 aafPulldownDir_t;

typedef 
enum _aafPulldownDir_e
    {	kAAFTapeToFilmSpeed	= 0,
	kAAFFilmToTapeSpeed	= 1
    }	aafPulldownDir_e;

typedef struct  _aafMobComment_t
    {
    aafString_t category;
    aafString_t comment;
    }	aafMobComment_t;

typedef aafInt32 aafRGBAComponentKind_t;

typedef 
enum _aafRGBAComponentKind_e
    {	kAAFCompNone	= 0x30,
	kAAFCompAlpha	= 0x41,
	kAAFCompBlue	= 0x42,
	kAAFCompFill	= 0x46,
	kAAFCompGreen	= 0x47,
	kAAFCompPalette	= 0x50,
	kAAFCompRed	= 0x52
    }	aafRGBAComponentKind_e;

typedef struct  _aafRGBAComponent_t
    {
    aafRGBAComponentKind_t Code;
    aafUInt8 Size;
    }	aafRGBAComponent_t;

/************************************************************
 *
 * Operation Group Types
 *
 *************************************************************/
typedef aafInt32 aafInterpKind_t;

typedef 
enum _aafInterpKind_e
    {	kAAFConstInterp	= 1,
	kAAFLinearInterp	= 2
    }	aafInterpKind_e;

typedef aafInt32 aafEditHint_t;

typedef 
enum _aafEditHint_e
    {	kAAFNoEditHint	= 0,
	kAAFProportional	= 1,
	kAAFRelativeLeft	= 2,
	kAAFRelativeRight	= 3,
	kAAFRelativeFixed	= 4
    }	aafEditHint_e;

typedef aafInt32 aafProductReleaseType_t;

typedef 
enum _aafProductReleaseType_e
    {	kAAFVersionUnknown	= 0,
	kAAFVersionReleased	= 1,
	kAAFVersionDebug	= 2,
	kAAFVersionPatched	= 3,
	kAAFVersionBeta	= 4,
	kAAFVersionPrivateBuild	= 5
    }	aafProductReleaseType_e;

/************************************************************
 *
 * Structured AAFI types.
 *
 *************************************************************/
typedef struct  _aafVersionType_t
    {
    aafInt8 major;
    aafInt8 minor;
    }	aafVersionType_t;

typedef struct  _aafProductVersion_t
    {
    aafUInt16 major;
    aafUInt16 minor;
    aafUInt16 tertiary;
    aafUInt16 patchLevel;
    aafProductReleaseType_t type;
    }	aafProductVersion_t;

typedef struct  _aafUID_t
    {
    aafUInt32 Data1;
    aafUInt16 Data2;
    aafUInt16 Data3;
    aafUInt8 Data4[ 8 ];
    }	aafUID_t;

typedef struct  _aafMobID_t
    {
    aafUInt8 SMPTELabel[ 12 ];
    aafUInt8 length;
    aafUInt8 instanceHigh;
    aafUInt8 instanceMid;
    aafUInt8 instanceLow;
    struct _aafUID_t material;
    }	aafMobID_t;

typedef struct  _aafProductIdentification_t
    {
    aafCharacter __RPC_FAR *companyName;
    aafCharacter __RPC_FAR *productName;
    aafCharacter __RPC_FAR *productVersionString;
    aafUID_t productID;
    aafCharacter __RPC_FAR *platform;
    aafProductVersion_t __RPC_FAR *productVersion;
    }	aafProductIdentification_t;

extern const aafProductVersion_t AAFReferenceImplementationVersion;
typedef struct  _aafDateStruct_t
    {
    aafInt16 year;
    aafUInt8 month;
    aafUInt8 day;
    }	aafDateStruct_t;

typedef struct  _aafTimeStruct_t
    {
    aafUInt8 hour;
    aafUInt8 minute;
    aafUInt8 second;
    aafUInt8 fraction;
    }	aafTimeStruct_t;

typedef struct  _aafTimeStamp_t
    {
    aafDateStruct_t date;
    aafTimeStruct_t time;
    }	aafTimeStamp_t;

typedef struct  _aafRational_t
    {
    aafInt32 numerator;
    aafInt32 denominator;
    }	aafRational_t;

typedef struct  _aafRect_t
    {
    aafInt32 xOffset;
    aafInt32 yOffset;
    aafInt32 xSize;
    aafInt32 ySize;
    }	aafRect_t;

typedef struct  _aafmMultiXfer_t
    {
    aafUID_t __RPC_FAR *mediaKind;
    aafInt16 subTrackNum;
    aafUInt32 numSamples;
    aafUInt32 buflen;
    aafDataBuffer_t buffer;
    }	aafmMultiXfer_t;

typedef struct  _aafmMultiResult_t
    {
    aafUInt32 bytesXfered;
    aafUInt32 samplesXfered;
    }	aafmMultiResult_t;

typedef struct  _aafmMultiCreate_t
    {
    const aafUID_t __RPC_FAR *mediaKind;
    aafInt16 subTrackNum;
    aafInt32 slotID;
    aafRational_t sampleRate;
    }	aafmMultiCreate_t;

/* Typedefs specific to edgecode and timecode */
typedef aafInt32 aafEdgeType_t;

typedef 
enum _aafEdgeType_e
    {	kAAFEtNull	= 0,
	kAAFEtKeycode	= 1,
	kAAFEtEdgenum4	= 2,
	kAAFEtEdgenum5	= 3,
	kAAFEtHeaderSize	= 8
    }	aafEdgeType_e;

typedef aafInt32 aafFilmType_t;

typedef 
enum _aafFilmType_e
    {	kAAFFtNull	= 0,
	kAAFFt35MM	= 1,
	kAAFFt16MM	= 2,
	kAAFFt8MM	= 3,
	kAAFFt65MM	= 4
    }	aafFilmType_e;

typedef aafInt32 aafDirectionCode_t;

typedef 
enum _aafDirectionCode_e
    {	kAAFDirCodeRight	= 0,
	kAAFDirCodeUpperRight	= 1,
	kAAFDirCodeUp	= 2,
	kAAFDirCodeUpperLeft	= 3,
	kAAFDirCodeLeft	= 4,
	kAAFDirCodeLowerLeft	= 5,
	kAAFDirCodeDown	= 6,
	kAAFDirCodeLowerRight	= 7
    }	aafDirectionCode_e;

typedef aafInt32 aafColorSpace_t;

typedef 
enum _aafColorSpace_e
    {	kAAFColorSpaceRGB	= 0,
	kAAFColorSpaceYUV	= 1,
	kAAFColorSpaceYIQ	= 2,
	kAAFColorSpaceHSI	= 3,
	kAAFColorSpaceHSV	= 4,
	kAAFColorSpaceYCrCb	= 5,
	kAAFColorSpaceYDrDb	= 6,
	kAAFColorSpaceCMYK	= 7
    }	aafColorSpace_e;

typedef struct  _aafTimecode_t
    {
    aafFrameOffset_t startFrame;
    aafDropType_t drop;
    aafUInt16 fps;
    }	aafTimecode_t;

typedef aafUInt8 __RPC_FAR aafEdgecodeHeader_t[ 8 ];

typedef struct  _aafEdgecode_t
    {
    aafFrameOffset_t startFrame;
    aafFilmType_t filmKind;
    aafEdgeType_t codeFormat;
    aafEdgecodeHeader_t header;
    }	aafEdgecode_t;

/*** Types for clips ***/
typedef struct  _aafSourceRef_t
    {
    aafMobID_t sourceID;
    aafSlotID_t sourceSlotID;
    aafPosition_t startTime;
    }	aafSourceRef_t;

typedef struct  _aafDefaultFade_t
    {
    aafLength_t fadeLength;
    aafFadeType_t fadeType;
    aafRational_t fadeEditUnit;
    aafBoolean_t valid;
    }	aafDefaultFade_t;

/************************************************************
 *
 * Multiple Media Representations Criteria types
 *
 *************************************************************/
typedef aafInt32 aafCriteriaType_t;

typedef 
enum _aafCriteriaType_e
    {	kAAFAnyRepresentation	= 0,
	kAAFFastestRepresentation	= 1,
	kAAFBestFidelityRepresentation	= 2,
	kAAFSmallestRepresentation	= 3
    }	aafCriteriaType_e;

typedef struct  _aafMediaCriteria_t
    {
    aafCriteriaType_t type;
    }	aafMediaCriteria_t;

/************************************************************
 *
 * Data Types for Search Criteria and Iterators
 *
 *************************************************************/
typedef aafInt32 aafSearchTag_t;

typedef 
enum _aafSearchTag_e
    {	kAAFNoSearch	= 0,
	kAAFByMobID	= 1,
	kAAFByMobKind	= 2,
	kAAFByName	= 3,
	kAAFByClass	= 4,
	kAAFByDataDef	= 5,
	kAAFByMediaCrit	= 6
    }	aafSearchTag_e;

#if 0
typedef struct  _aafSearchCrit_t
    {
    aafSearchTag_t searchTag;
    /* [switch_is] */ /* [switch_type] */ union 
        {
        /* [case()] */ aafMobID_t mobID;
        /* [case()] */ aafMobKind_t mobKind;
        /* [case()] */ aafString_t name;
        /* [case()] */ aafClassID_t objClass;
        /* [case()] */ aafUID_t datadef;
        /* [case()] */ aafCriteriaType_t mediaCrit;
        /* [default] */  /* Empty union arm */ 
        }	;
    }	aafSearchCrit_t;

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

typedef 
enum _aafDefinitionKind_e
    {	kAAFClassDefinition	= 1,
	kAAFPropertyDefinition	= 2,
	kAAFTypeDefinition	= 4,
	kAAFDataDefinition	= 8,
	kAAFOperationDefinition	= 16,
	kAAFAllDefinitions	= -1
    }	aafDefinitionKind_e;

typedef aafInt32 aafDefinitionCritType_t;

typedef 
enum _aafDefinitionCritType_e
    {	kAAFNoDefSearch	= 0,
	kAAFByDefKind	= 1,
	kAAFByDefName	= 2,
	kAAFByDefClass	= 3
    }	aafDefinitionCritType_e;

typedef struct  _aafDefinitionCrit_t
    {
    aafDefinitionCritType_t type;
    union 
        {
        aafDefinitionKind_t defKind;
        aafString_t name;
        aafClassID_t objClass;
        }	tags;
    }	aafDefinitionCrit_t;

typedef aafInt32 aafIdentificationCritType_t;

typedef 
enum _aafIdentificationCritType_e
    {	kAAFNoIdSearch	= 0,
	kAAFByIdProductID	= 1,
	kAAFByIdGeneration	= 2,
	kAAFByIdRefImplVersion	= 3
    }	aafIdentificationCritType_e;

typedef struct  _aafIdentificationCrit_t
    {
    aafIdentificationCritType_t type;
    union 
        {
        aafUID_t productID;
        aafUID_t generation;
        aafProductVersion_t referenceImplementationVersion;
        }	tags;
    }	aafIdentificationCrit_t;

/************************************************************
 *
 * Data Types for Searching the Class Dictionary
 *
 *************************************************************/
typedef aafInt32 aafSearchDictionaryTag_t;

typedef 
enum _aafSearchDictionaryTag_e
    {	kAAFAllClasses	= 0,
	kAAFByClassID	= 1,
	kAAFByParentClassID	= 2,
	kAAFByPropertyID	= 3,
	kAAFByTypeID	= 4,
	kAAFByClassName	= 5
    }	aafSearchDictionaryTag_e;

typedef aafInt32 eAAFByteOrder_t;

typedef 
enum _eAAFByteOrder_e
    {	kAAFByteOrderLittle	= 0,
	kAAFByteOrderBig	= 1
    }	eAAFByteOrder_e;

#if 0
typedef struct  _aafSearchDictionaryCrit_t
    {
    aafSearchDictionaryTag_t searchTag;
    /* [switch_is] */ /* [switch_type] */ union 
        {
        /* [case()] */ aafUID_t __RPC_FAR *classID;
        /* [case()] */ aafUID_t __RPC_FAR *parentClassID;
        /* [case()] */ aafUID_t __RPC_FAR *propertyID;
        /* [case()] */ aafUID_t __RPC_FAR *typeID;
        /* [case()] */ aafString_t name;
        /* [default] */  /* Empty union arm */ 
        }	;
    }	aafSearchDictionaryCrit_t;

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

typedef 
enum _eAAFTypeCategory_e
    {	kAAFTypeCatUnknown	= 0,
	kAAFTypeCatInt	= 1,
	kAAFTypeCatCharacter	= 2,
	kAAFTypeCatStrongObjRef	= 3,
	kAAFTypeCatWeakObjRef	= 4,
	kAAFTypeCatRename	= 5,
	kAAFTypeCatEnum	= 6,
	kAAFTypeCatFixedArray	= 7,
	kAAFTypeCatVariableArray	= 8,
	kAAFTypeCatSet	= 9,
	kAAFTypeCatRecord	= 10,
	kAAFTypeCatStream	= 11,
	kAAFTypeCatString	= 12,
	kAAFTypeCatExtEnum	= 13,
	kAAFTypeCatIndirect	= 14,
	kAAFTypeCatOpaque	= 15,
	kAAFTypeCatEncrypted	= 16
    }	eAAFTypeCategory_e;

typedef aafBoolean_t aafBool;

typedef aafColorSiting_t aafColorSitingType_t;

typedef aafEditHint_t aafEditHintType_t;

typedef aafFrameLayout_t aafLayoutType_t;

typedef aafTimecodeSourceType_t aafTCSource_t;

typedef aafPulldownDir_t aafPulldownDirectionType_t;

typedef aafPulldownKind_t aafPulldownKindType_t;

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
typedef aafArgIDType_t __RPC_FAR *aafArgIDType_constref;

typedef aafEdgecodeHeader_t __RPC_FAR *aafEdgecodeHeader_constref;

typedef aafEdgecode_t __RPC_FAR *aafEdgecode_constref;

typedef aafEngine_t __RPC_FAR *aafEngine_constref;

typedef aafHardwarePlatform_t __RPC_FAR *aafHardwarePlatform_constref;

typedef aafMediaCriteria_t __RPC_FAR *aafMediaCriteria_constref;

typedef aafPluginAPI_t __RPC_FAR *aafPluginAPI_constref;

typedef aafPosition_t __RPC_FAR *aafPosition_constref;

typedef aafProductIdentification_t __RPC_FAR *aafProductIdentification_constref;

typedef aafProductVersion_t __RPC_FAR *aafProductVersion_constref;

typedef aafRect_t __RPC_FAR *aafRect_constref;

typedef aafSourceRef_t __RPC_FAR *aafSourceRef_constref;

typedef aafTimecode_t __RPC_FAR *aafTimecode_constref;

typedef aafLength_t __RPC_FAR *aafLength_constref;

typedef aafRational_t __RPC_FAR *aafRational_constref;

typedef aafTimeStamp_t __RPC_FAR *aafTimeStamp_constref;

typedef aafUID_t __RPC_FAR *aafUID_constref;

typedef aafMobID_t __RPC_FAR *aafMobID_constref;

typedef aafVersionType_t __RPC_FAR *aafVersionType_constref;

typedef unsigned char __RPC_FAR *aafMemConstPtr_t;

typedef aafCharacter __RPC_FAR *aafCharacter_constptr;

typedef aafMediaCriteria_t __RPC_FAR *aafMediaCriteria_constptr;

typedef aafOperationChoice_t __RPC_FAR *aafOperationChoice_constptr;

typedef aafProductVersion_t __RPC_FAR *aafProductVersion_constptr;

typedef aafProductIdentification_t __RPC_FAR *aafProductIdentification_constptr;

typedef aafSearchCrit_t __RPC_FAR *aafSearchCrit_constptr;

typedef aafUID_t __RPC_FAR *aafUID_constptr;

typedef aafMobID_t __RPC_FAR *aafMobID_constptr;

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


extern RPC_IF_HANDLE IAAFTypes_v0_0_c_ifspec;
extern RPC_IF_HANDLE IAAFTypes_v0_0_s_ifspec;
#endif /* __IAAFTypes_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
