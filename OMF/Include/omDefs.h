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

#ifndef _OMDEFS_INCLUDED_
#define _OMDEFS_INCLUDED_ 1

#include "portkey.h"
#include "masterhd.h"

#if PORT_LANG_CPLUSPLUS
extern "C" 
  {
#endif

/* To support DLLs */
#if PORT_SUPPORTS_DLL && defined (OMF_USE_DLL) /* Second expression protects static lib projects */
	#if defined (OMF_BUILDING_TOOLKIT_DLL)
	#define OMF_EXPORT __declspec( dllexport )
	#else
	#define OMF_EXPORT __declspec( dllimport )
	#endif
#else
#define OMF_EXPORT
#endif

#define OMBYTES_PER_UID 12
#define OMBYTES_PER_OBJREF_1X 8
#define OMBYTES_PER_OBJECTTAG 4
#define OMBYTES_PER_CLASSID 4
#define OMUNIQUENAME_SIZE 64
#define OMMOBNAME_SIZE 64

#if defined(TRUE)
#undef TRUE
#endif
#define TRUE 1

#if defined(FALSE)
#undef FALSE
#endif
#define FALSE 0

#define MOTOROLA_ORDER 0x4d4d	/* 'MM' */
#define INTEL_ORDER    0x4949	/* 'II' */

#if !defined( CS_NULL )
#define CS_NULL (omfCharSetType_t)0
#define CS_ASCII (omfCharSetType_t)1
#endif	/* CS_NULL */

#if !defined( TT_NULL )
#define TT_NULL 0		/* 1.5 compatability */
#define TT_PICTURE 1		/* 1.5 compatability */
#define TT_SOUND 2		/* 1.5 compatability */
#define TT_TIMECODE 3		/* 1.5 compatability */
#define TT_EDGECODE 4		/* 1.5 compatability */
#define TT_ATTRIBUTE 5		/* 1.5 compatability */
#define TT_EFFECTDATA 6		/* 1.5 compatability */
#endif	/* TT_NULL */

#if !defined( UC_NULL )
#define UC_NULL		  (omfUsageCode_t)0	/* Null app code; no special use.   */
#define	UC_MASTERMOB	  (omfUsageCode_t)7	/* log mob to group phys mobs. */

/* Avid Media Composer private usage codes that are commonly used */
#define	UC_PRECOMPUTE	(omfUsageCode_t)1	/* master mob to a precompute */
#define UC_SUBCLIP		(omfUsageCode_t)2	/* log mob is a subclip */
#define UC_EFFECT		(omfUsageCode_t)3	/* log mob is an effect holder */
#define UC_GROUP		(omfUsageCode_t)4	/* log mob -a comp of ASelectors */
#define UC_GROUPOOFTER	(omfUsageCode_t)5	/* log mob that back up groups	*/
#define UC_MOTION		(omfUsageCode_t)6	/* log mob - motion effect clip	*/
#define	UC_PRECOMPUTE_FILE	(omfUsageCode_t)9	/* file mob with a precompute*/
#endif	/* UC_NULL */

#if !defined( PT_NULL )
#define PT_NULL (omfPhysicalMobType_t)0
#define PT_FILE_MOB (omfPhysicalMobType_t)1
#define PT_TAPE_MOB (omfPhysicalMobType_t)2
#define PT_FILM_MOB (omfPhysicalMobType_t)3
#define PT_NAGRA_MOB (omfPhysicalMobType_t)4
#endif	/* PT_NULL */

#if !defined( AK_NULL )
#define AK_NULL		kOMFNullAttribute		/* 1.5 compatability */
#define AK_INT		kOMFIntegerAttribute		/* 1.5 compatability */
#define AK_STRING	kOMFStringAttribute		/* 1.5 compatability */
#define AK_OBJECT	kOMFObjectAttribute		/* 1.5 compatability */
#endif	/* AK_NULL */

#if !defined( ET_NULL )
#define ET_NULL (omfEdgeType_t)0
#define ET_KEYCODE (omfEdgeType_t)1
#define ET_EDGENUM4 (omfEdgeType_t)2
#define ET_EDGENUM5 (omfEdgeType_t)3
#endif	/* ET_NULL */

#if !defined( FT_NULL )
#define FT_NULL (omfFilmType_t)0
#define FT_35MM (omfFilmType_t)1
#define FT_16MM (omfFilmType_t)2
#define FT_8MM (omfFilmType_t)3
#define FT_65MM (omfFilmType_t)4
#endif	/* FT_NULL */

#if !defined( OMFI_LAYOUT_FULL )
#define OMFI_LAYOUT_FULL 0
#define OMFI_LAYOUT_SEPARATE 1
#define OMFI_LAYOUT_SINGLE 2
#define OMFI_LAYOUT_MIXED 3
#endif	/* OMFI_LAYOUT_FULL */


/************************************************************************
 *
 *              P R O P E R T I E S
 *
 ************************************************************************/

#if !defined( OMF_PROPERTY_T )
#define OMF_PROPERTY_T

typedef enum
{
	OMNoProperty=0,
	OMObjID, OMAuthor, OMByteOrder, OMCharacterSet, OMCopyright, OMDate,
	OMATCPVersion, OMATCPAttributes,
	OMATTBVersion, OMATTBKind, OMATTBName, OMATTBIntAttribute, 
	OMATTBStringAttribute, OMATTBObjAttribute,
	OMATTRVersion, OMATTRAttrRefs,
	OMCLIPVersion, OMCLIPLength,
	OMCLSDVersion, OMCLSDClassID, OMCLSDParentClass, OMCLSDClass,  
	OMCPNTVersion, OMCPNTTrackKind, OMCPNTEditRate, OMCPNTName, OMCPNTEffectID,
	OMCPNTPrecomputed, OMCPNTAttributes, OMCPNTDatakind, OMCPNTLength,
	OMDDEFDatakindID,  
	OMDOSLVersion, OMDOSLPathName,
	OMECCPVersion, OMECCPFilmKind, OMECCPCodeFormat, OMECCPStartEC,
	OMECCPStart,
	OMFILLVersion,
	OMVersion, OMLastModified, OMClassDictionary, OMSourceMobs, 
	OMCompositionMobs,
	OMMediaData, OMExternalFiles, OMAttributes, OMObjectSpine,
	OMMACLVersion, OMMACLVRef, OMMACLDirID, OMMACLFileName, OMMACLVName,   
	OMMASKVersion, OMMASKMaskBits, OMMASKIsDouble,
	OMMDESVersion, OMMDESMobKind, OMMDESLocator,
	OMMDFLVersion, OMMDFLIsOMFI, OMMDFLSampleRate, OMMDFLLength,
	OMMOBJVersion, OMMOBJMobID, OMMOBJLastModified, OMMOBJUsageCode, 
	OMMOBJStartPosition, OMMOBJPhysicalMedia,
	OMMOBJName, OMMOBJSlots, OMMOBJCreationTime,
	OMMOBJUserAttributes, OMCMOBDefFadeLength, OMCMOBDefFadeType, OMCMOBDefFadeEditUnit,
	OMMSLTSegment, OMMSLTEditRate, OMMSLTTrackDesc,	
	OMREPTVersion,
	OMSCLPVersion, OMSCLPSourceID, OMSCLPSourceTrack, OMSCLPSourcePosition,
	OMSCLPFadeInLength, OMSCLPFadeInType, OMSCLPFadeOutLength, 
	OMSCLPFadeOutType, OMSCLPSourceTrackID, OMSCLPStartTime,
	OMSEQUVersion, OMSEQUSequence, OMSEQUComponents,
	OMSLCTVersion, OMSLCTIsGanged, OMSLCTSelectedTrack,
	OMSPEDVersion, OMSPEDNumerator, OMSPEDDenominator,
	OMTCCPVersion, OMTCCPFlags, OMTCCPFPS, OMTCCPStartTC, OMTCCPStart,	
	OMTCCPDrop,
	OMTRAKVersion, OMTRAKLabelNumber, OMTRAKAttributes, OMTRAKFillerProxy, 
	OMTRAKTrackComponent,
	OMTRANVersion, OMTRANCutPoint, OMTRANEffect,
	OMTRKGVersion, OMTRKGGroupLength, OMTRKGTracks,
	OMTRKRVersion, OMTRKRRelativeScope, OMTRKRRelativeTrack,
	OMTXTLVersion, OMTXTLName,
	OMUNXLVersion, OMUNXLPathName,
	OMWARPVersion, OMWARPEditRate, OMWARPPhaseOffset,
	OMToolkitVersion, OMECCPHeader, OMHEADPrimaryMobs,
	OMEFFEEffectKind, OMEFFEEffectSlots, OMEFFEBypassOverride,
	OMEFFEFinalRendering, OMEFFEWorkingRendering,
	OMESLTArgID, OMESLTArgValue,
	OMEDEFEffectID, OMEDEFEffectName, OMEDEFEffectDescription,
	OMEDEFBypass, OMEDEFIsTimeWarp,
	OMCVALValue,					
	OMVVALInterpolation, OMVVALPointList,	
	OMCTLPTime, OMCTLPDatakind, OMCTLPValue, OMCTLPEditHint,
	OMNESTSlots,
	OMSREFRelativeScope, OMSREFRelativeSlot,	
	OMSLCTSelected, OMSLCTAlternates,
	OMOOBJObjClass,						
	OMHEADDefinitionObjects, OMHEADMobs, 
	OMERATInputSegment, OMERATInputEditRate, OMERATInputOffset, 
	OMERATResultOffset,
	OMMDATMobID,
	OMMDFMFilmFormat, OMMDFMFrameRate, OMMDFMPerforationsPerFrame,
	OMMDFMFilmAspectRatio, OMMDFMManufacturer, OMMDFMModel,
	OMMGRPChoices, OMMGRPStillFrame,	
	OMMDTPFormFactor, OMMDTPVideoSignal, OMMDTPTapeFormat,	
	OMMDTPLength, OMMDTPManufacturer, OMMDTPModel,	
	OMSMOBMediaDescription,
	OMTRKDOrigin, OMTRKDTrackID, OMTRKDTrackName, OMTRKDPhysTrack,	
	OMDIDDCompression, OMDIDDStoredHeight, OMDIDDStoredWidth,
	OMDIDDSampledHeight, OMDIDDSampledWidth, OMDIDDSampledXOffset,
	OMDIDDSampledYOffset, OMDIDDDisplayHeight, OMDIDDDisplayWidth,
	OMDIDDDisplayXOffset, OMDIDDDisplayYOffset, OMDIDDFrameLayout,
	OMDIDDAlphaTransparency, OMDIDDVideoLineMap, 
	OMDIDDImageAspectRatio,	OMDIDDClientFillStart, OMDIDDClientFillEnd,
	OMWINLPathName,
	OMWINLShortcut,	OMHEADByteOrder, OMHEADLastModified, 
	OMHEADClassDictionary, OMHEADMediaData, OMHEADIdentList,
	OMIDATImageData,
	OMMDESCodecID, OMMDESCodecDescription,
	OMDIDDFieldAlignment, OMDIDDGamma, 
	OMIDNTCompanyName, OMIDNTProductName, OMIDNTProductVersion, 
	OMIDNTProductVersionString, OMIDNTProductID, OMIDNTDate, 
	OMIDNTToolkitVersion, OMIDNTPlatform, OMIDNTByteOrder,
	OMNETLURLString,
	OMPDWNInputSegment, OMPDWNPulldownKind, OMPDWNPhaseFrame,
	OMPDWNPhaseField, OMPDWNDirection,
	
	/* Add 1.x Media Properties (TIFF, AIFC, WAVE) */
	OMTIFDVersion, OMTIFDIsUniform, OMTIFDIsContiguous, OMTIFDTrailingLines,
	OMTIFDLeadingLines, OMTIFDJPEGTableID, OMTIFDSummary,
	OMTIFFVersion, OMTIFFMobID, OMTIFFData, 
	OMAIFDVersion, OMAIFDSummary, 
	OMAIFCVersion, OMAIFCMobID, OMAIFCData, 
	OMWAVDVersion, OMWAVDSummary, 
	OMWAVEVersion, OMWAVEMobID, OMWAVEData, 
	
	/* Add all new properties BEFORE here */
	OMLASTPROP,
	OMPrivatePropStart=(1 << ((sizeof(int)*8)-2))
} omfProperty_t;
#endif /* OMF_PROPERTY_T */


/************************************************************************
 *
 *              T Y P E S
 *
 ************************************************************************/

#if !defined( OMF_TYPE_T )
#define OMF_TYPE_T
 
typedef enum 
{
  OMNoType, OMAttrKind, OMBoolean, OMInt8, OMCharSetType, OMEdgeType,
  OMExactEditRate, OMFilmType, OMJPEGTableIDType, OMInt32, OMMobIndex,
  OMObjRefArray, OMObjRef, OMObjectTag, OMPhysicalMobType, OMInt16,
  OMString, OMTimeStamp, OMTrackType, OMUID, OMUInt8, OMUInt32, OMUInt16,
  OMUsageCodeType, OMVarLenBytes, OMVersionType, OMPosition32, OMLength32,
  OMPosition64, OMLength64,
  OMClassID, OMUniqueName, OMFadeType, OMInterpKind, OMArgIDType,
  OMEditHintType, OMDataValue, OMTapeCaseType,
  OMVideoSignalType, OMTapeFormatType, OMCompCodeArray, OMCompSizeArray,
  OMColorSitingType, OMRational, OMCompressionType, OMLayoutType,
  OMPosition32Array, OMDirectionCode, OMColorSpace, OMInt32Array,
  OMInt64, OMProductVersion, OMPosition64Array,
  OMPulldownKindType, OMPhaseFrameType, OMPulldownDirectionType,

  /* Add all new types BEFORE here */
  OMLASTTYPE,
  OMPrivateTypeStart=(1 << ((sizeof(int)*8)-2))
} omfType_t;

#endif /* OMF_TYPE_T */

/************************************************************************
 *
 *				C L A S S E S
 *
 ************************************************************************/
#define OMClassNone	""
#define OMClassAny	OMClassNone
#define OMClassHEAD "HEAD"
#define OMClassATCP	"ATCP"
#define OMClassATTB	"ATTB"
#define OMClassATTR	"ATTR"
#define OMClassCLIP	"CLIP"
#define OMClassCLSD	"CLSD"
#define OMClassCPNT	"CPNT"
#define OMClassDOSL	"DOSL"
#define OMClassECCP	"ECCP"
#define OMClassFILL	"FILL"
#define OMClassMACL	"MACL"
#define OMClassMASK	"MASK"
#define OMClassMDES	"MDES"
#define OMClassMDFL	"MDFL"
#define OMClassMOBJ	"MOBJ"
#define OMClassREPT	"REPT"
#define OMClassSCLP	"SCLP"
#define OMClassSEQU	"SEQU"
#define OMClassSLCT	"SLCT"
#define OMClassSPED	"SPED"
#define OMClassTCCP	"TCCP"
#define OMClassTRAK	"TRAK"
#define OMClassTRAN	"TRAN"
#define OMClassTRKG	"TRKG"
#define OMClassTRKR	"TRKR"
#define OMClassTXTL	"TXTL"
#define OMClassUNXL	"UNXL"
#define OMClassWARP	"WARP"
#define OMClassOOBJ	"OOBJ"
#define OMClassDDEF	"DDEF"
#define OMClassMSLT	"MSLT"
#define OMClassSEGM	"SEGM"
#define OMClassRATE	"RATE"
#define OMClassEFFE	"EFFE"
#define OMClassESLT	"ESLT"
#define OMClassEDEF	"EDEF"
#define OMClassCVAL	"CVAL"
#define OMClassVVAL	"VVAL"
#define OMClassCTLP	"CTLP"
#define OMClassNEST	"NEST"
#define OMClassSREF	"SREF"
#define OMClassCMOB	"CMOB"
#define OMClassIDNT	"IDNT"

#define OMClassCDCI "CDCI"
#define OMClassDIDD "DIDD"
#define OMClassERAT "ERAT"
#define OMClassIDAT "IDAT"
#define OMClassJPEG "JPEG"
#define OMClassLOCR "LOCR"
#define OMClassMMOB "MMOB"
#define OMClassMDAT "MDAT"
#define OMClassSMOB "SMOB"
#define OMClassMDFM "MDFM"
#define OMClassMGRP "MGRP"
#define OMClassMDTP "MDTP"
#define OMClassRGBA "RGBA"
#define OMClassTRKD "TRKD"
#define OMClassWINL "WINL"
#define OMClassNETL "NETL"
#define OMClassPDWN "PDWN"
#define OMClassSD2D "SD2D" /* Added for Avid Media Composer support of Sound
							* Designer II files for 1.x. 
							*/
#define OMClassMDNG "MDNG" /* Added for Avid Media Composer Nagra Descriptor */

#define OMClassTIFD	"TIFD"
#define OMClassTIFF	"TIFF"
#define OMClassWAVD	"WAVD"
#define OMClassWAVE	"WAVE"
#define OMClassIDAT "IDAT"
#define OMClassRGBA "RGBA"
#define OMClassAIFC	"AIFC"
#define OMClassAIFD	"AIFD"
#define OMClassCDCI	"CDCI"
#define OMClassJPEG	"JPEG"

#define NODATAKIND (omfUniqueNamePtr_t)"omfi:data:NoDatakind"
#define CHARKIND (omfUniqueNamePtr_t)"omfi:data:Char"
#define UINT8KIND (omfUniqueNamePtr_t)"omfi:data:UInt8"
#define UINT16KIND (omfUniqueNamePtr_t)"omfi:data:UInt16"
#define UINT32KIND (omfUniqueNamePtr_t)"omfi:data:UInt32"
#define INT8KIND (omfUniqueNamePtr_t)"omfi:data:Int8"
#define INT16KIND (omfUniqueNamePtr_t)"omfi:data:Int16"
#define INT32KIND (omfUniqueNamePtr_t)"omfi:data:Int32"
#define RATIONALKIND (omfUniqueNamePtr_t)"omfi:data:Rational"
#define BOOLEANKIND (omfUniqueNamePtr_t)"omfi:data:Boolean"
#define STRINGKIND (omfUniqueNamePtr_t)"omfi:data:String"
#define COLORSPACEKIND (omfUniqueNamePtr_t)"omfi:data:ColorSpace"
#define COLORKIND (omfUniqueNamePtr_t)"omfi:data:Color"
#define DISTANCEKIND (omfUniqueNamePtr_t)"omfi:data:Distance"
#define POINTKIND (omfUniqueNamePtr_t)"omfi:data:Point"
#define DIRECTIONKIND (omfUniqueNamePtr_t)"omfi:data:DirectionCode"
#define POLYKIND (omfUniqueNamePtr_t)"omfi:data:Polynomial"

#define PICTUREKIND (omfUniqueNamePtr_t)"omfi:data:Picture"
#define SOUNDKIND (omfUniqueNamePtr_t)"omfi:data:Sound"
#define MATTEKIND (omfUniqueNamePtr_t)"omfi:data:Matte"
#define PICTWITHMATTEKIND (omfUniqueNamePtr_t)"omfi:data:PictureWithMatte"

#define STEREOKIND (omfUniqueNamePtr_t)"omfi:data:StereoSound"
#define TIMECODEKIND (omfUniqueNamePtr_t)"omfi:data:Timecode"
#define EDGECODEKIND (omfUniqueNamePtr_t)"omfi:data:Edgecode"

#if PORT_LANG_CPLUSPLUS
  }
#endif
#endif				/* _OMDEFS_INCLUDED_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
