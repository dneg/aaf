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

/* Name: omMobBld.c
 *
 * Overall Function: The API for building compositions.
 *
 * Audience: Clients writing OMFI compositions.
 *
 * Public Functions:
 *     MobID Creation: omfiMobIDNew()
 *     Mob Root Creation: omfiCompMobNew(), omfiMobSetModTime(), 
 *              omfiMobSetPrimary(), omfiMobSetIdentity(), omfiMobSetName()
 *              omfiMobAppendNewSlot(), omfiMobAppendNewTrack()
 *     Clip Creation: omfiSourceClipNew(), omfiSourceClipSetRef(), 
 *              omfiSourceClipSetFade(), omfiTimecodeNew(),
 *              omfiEdgecodeNew(), omfiFillerNew()
 *     Const/Vary Value Creation: omfiConstValueNew(), omfiVaryValueNew(),
 *              omfiVaryValueAddPoint()
 *     Sequence Creation: omfiSequenceNew(), omfiSequenceAppendCpnt()
 *     Scope Creation: omfiNestedScopeNew(), omfiNestedScopeAppendSlot(), 
 *              omfiScopeRefNew()
 *     Selector Creation: omfiSelectorNew(), omfiSelectorSetSelected(),
 *              omfiSelectorAddAlt()
 *     Transition Creation: omfiTransitionNew()
 *     Effect Creation: omfiEffectDefNew(), omfiEffectAddNewSlot(),
 *              omfiEffectNew()
 *              omfiEffectSetFinalRender(), omfiEffectSetWorkingRender()
 *              omfiEffectSetBypassOverride(), omfiEffectDefLookup()
 *     Media Group Creation: omfiMediaGroupNew(), omfiMediaGroupSetStillFrame()
 *              omfiMediaGroupAddChoice()
 *     Definition Creation: omfiDatakindNew(), omfiDatakindLookup(),
 *
 *
 * General error codes returned:
 *      OM_ERR_INVALID_OBJ
 *              An object of the wrong class was passed in
 *      OM_ERR_DUPLICATE_MOBID
 *              MobID already exists in the file
 *      OM_ERR_INVALID_MOBTYPE
 *              This operation does not apply to this mob type
 *      OM_ERR_NULLOBJECT
 *              An invalid null object was passed in as a parameter
 *      OM_ERR_INVALID_DATAKIND
 *              An invalid datakind was passed in
 *      OM_ERR_INVALID_EFFECTDEF
 *              An invalid effectdef was passed in
 *      OM_ERR_INVALID_TRAN_EFFECT
 *              The effect is not a transition effect
 *      OM_ERR_ADJACENT_TRAN
 *              Adjacent transitions in a sequence are illegal
 *      OM_ERR_LEADING_TRAN
 *              Beginning a sequence with a transition is illegal
 *      OM_ERR_INSUFF_TRAN_MATERIAL
 *              Not enough material as input to transition
 *      OM_ERR_BAD_SLOTLENGTH
 *              The length of the slot is not valid
 *      OM_ERR_EFFECTDEF_EXIST
 *              An effectdef with this ID already exists
 *      OM_ERR_DATAKIND_EXIST
 *              A datakind with this ID already exists
 *      OM_ERR_NOT_SOURCE_CLIP
 *              The property must be a source clip.
 */

#ifndef _OMF_MOB_BUILD_
#define _OMF_MOB_BUILD_ 1

#include "omErr.h"
#include "omTypes.h"

#if PORT_LANG_CPLUSPLUS
extern "C" 
  {
#endif

/* start of prototypes */
OMF_EXPORT omfErr_t omfiMobIDNew(
        omfHdl_t file,       /* IN - File Handle */
        omfUID_t *mobID);    /* OUT - Newly created Mob ID */

OMF_EXPORT omfErr_t omfiCompMobNew(
	omfHdl_t file,       /* IN - File Handle */
	omfString name,      /* IN - Mob name (optional) */
	omfBool	isPrimary,   /* IN - Whether or not the mob is a primary mob */
	omfMobObj_t *compMob);/* OUT - Created composition mob */

OMF_EXPORT omfErr_t omfiMobSetModTime(	
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t	mob,     /* IN - Mob object */
	omfTimeStamp_t modTime); /* IN - New Modification Time */

OMF_EXPORT omfErr_t omfiMobSetPrimary(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfBool	isPrimary);   /* IN - Whether or not the mob is a primary mob */

OMF_EXPORT omfErr_t omfiMobSetIdentity(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob Object */
	omfUID_t newMobID);   /* IN - New Mob ID */

OMF_EXPORT omfErr_t omfiMobSetName(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob Object */
	omfString name);      /* IN - Mob Name */

OMF_EXPORT omfErr_t omfiMobAppendNewSlot(
	omfHdl_t file,             /* IN - File Handle */
	omfMobObj_t mob,           /* IN - Input Mob */
	omfRational_t editRate,    /* IN - Edit rate property value */
	omfSegObj_t segment,       /* IN - Segment to append as slot component */
	omfMSlotObj_t *newSlot);    /* OUT - Newly created slot */

OMF_EXPORT omfErr_t omfiMobAppendNewTrack(
	omfHdl_t file,           /* IN - File Handle */
	omfMobObj_t mob,         /* IN - Input Mob Object */
	omfRational_t editRate,  /* IN - Edit rate property value */
	omfSegObj_t segment,     /* IN - Segment to append as track component */
	omfPosition_t origin,    /* IN - Track Origin */
	omfTrackID_t trackID,    /* IN - Track ID */
	omfString trackName,     /* IN - Track Name (optional) */
	omfMSlotObj_t *newTrack); /* OUT - Newly created track */

OMF_EXPORT omfErr_t omfiTrackSetPhysicalNum(
	omfHdl_t		file,      			/* IN - File Handle */
	omfMSlotObj_t	track,				/* IN - An existing track */
	omfUInt32		physicalTrackNum); 	/* IN - A physical track number */

OMF_EXPORT omfErr_t omfiMobAppendComment(
	omfHdl_t		file,      	/* IN - File Handle */
	omfMobObj_t		mob,        /* IN - Input Mob Object */
	char			*category,  /* IN - Comment heading */
	char			*comment);  /* IN - Comment value */
	
OMF_EXPORT omfErr_t omfiMobSetDefaultFade(
	omfHdl_t		file,      		/* IN - File Handle */
	omfMobObj_t		mob,        	/* IN - Input Mob Object */
	omfLength_t		fadeLength, 	/* IN - Default fade length */
	omfFadeType_t	fadeType,		/* IN - default fade type */
	omfRational_t	fadeEditUnit);	/* IN - default fade type */

OMF_EXPORT omfErr_t omfiSourceClipNew(
	omfHdl_t file,              /* IN - File Handle */
	omfDDefObj_t datakind,      /* IN - Datakind object */
	omfLength_t length,         /* IN - Length property value */
	omfSourceRef_t sourceRef,   /* IN - Source Reference (sourceID, 
								 *      sourceTrackID, startTime)
								 */
	omfSegObj_t *newSourceClip); /* OUT - New Source Clip object */

OMF_EXPORT omfErr_t omfiSourceClipSetRef(
	omfHdl_t file,             /* IN - File Handle */
	omfSegObj_t sourceClip,    /* IN - Source Clip object */
	omfSourceRef_t sourceRef);  /* IN - Source Reference (sourceID,
								*      sourceTrackID, startTime)
								*/

OMF_EXPORT omfErr_t omfiSourceClipSetFade(
	omfHdl_t file,              /* IN - File Handle */
	omfSegObj_t sourceClip,     /* IN - Source Clip object */
	omfInt32 fadeInLen,         /* IN - Fade In Length */
	omfFadeType_t fadeInType,   /* IN - Fade In Type */
	omfInt32 fadeOutLen,        /* IN - Fade Out Length */
	omfFadeType_t fadeOutType);  /* IN - Fade Out Type */

OMF_EXPORT omfErr_t omfiTimecodeNew(
    omfHdl_t file,               /* IN - File Handle */
	omfLength_t length,          /* IN - Length Property Value */
	omfTimecode_t timecode,      /* IN - Timecode Value (startFrame,
								  *      drop, fps)
								  */
	omfSegObj_t *timecodeClip);   /* OUT - New Timecode Clip */

OMF_EXPORT omfErr_t omfiEdgecodeNew(
    omfHdl_t file,             /* IN - File Handle */
	omfLength_t length,        /* IN - Length Property Value */
	omfEdgecode_t edgecode,    /* IN - Edgecode Value (startFrame,
							    *      filmKind, codeFormat)
							    */
	omfSegObj_t *edgecodeClip); /* OUT - New Edgecode Clip */

OMF_EXPORT omfErr_t omfiFillerNew(	
	omfHdl_t file,          /* IN - File Handle */
	omfDDefObj_t datakind,  /* IN - Datakind Object */
	omfLength_t length,     /* IN - Length Property Value */
	omfSegObj_t *newFiller); /* OUT - New Filler Object */

OMF_EXPORT omfErr_t omfiConstValueNew(
	omfHdl_t file,          /* IN - File Handle */
	omfDDefObj_t datakind,  /* IN - Datakind object */
	omfLength_t length,     /* IN - Length property value */
	omfInt32 valueSize,     /* IN - Size of the value data */
	void *value,            /* IN - Value data */
	omfSegObj_t *constValueSeg); /* OUT - Constant value object */

OMF_EXPORT omfErr_t omfiVaryValueNew(
    omfHdl_t file,                 /* IN - File Handle */
	omfDDefObj_t datakind,         /* IN - Datakind object */
	omfLength_t length,            /* IN - Length property value */
	omfInterpKind_t interpolation, /* IN - Interpolation method */
	omfSegObj_t *varyValueSeg);     /* OUT - Varying value object */

OMF_EXPORT omfErr_t omfiVaryValueAddPoint(
    omfHdl_t file,          /* IN - File Handle */
	omfSegObj_t varyValue,  /* IN - VaryValue object */
	omfRational_t time,     /* IN - Time for control Point */
	omfEditHint_t editHint, /* IN - Edit Hint */
	omfDDefObj_t datakind,  /* IN - Datakind object */
	omfInt32 valueSize,     /* IN - Size of value data */
	void *value);            /* IN - Value data */

OMF_EXPORT omfErr_t omfiSequenceNew(
	omfHdl_t file,             /* IN - File Handle */
	omfDDefObj_t datakind,     /* IN - Datakind object */
	omfSegObj_t *newSequence);  /* OUT - Sequence object */

OMF_EXPORT omfErr_t omfiTransitionNew(
	omfHdl_t file,             /* IN - File Handle */
	omfDDefObj_t datakind,     /* IN - Datakind object */
	omfLength_t length,        /* IN - Length property value */
	omfPosition_t cutPoint,        /* IN - Cut Point */
	omfEffObj_t effect,        /* IN - Effect object */
	omfCpntObj_t *newTransition); /* OUT - New Transition */

OMF_EXPORT omfErr_t omfiSequenceAppendCpnt(
	omfHdl_t file,             /* IN - File Handle */
	omfSegObj_t sequence,      /* IN - Sequence object */
	omfCpntObj_t component);    /* IN - Component to append to the sequence */

OMF_EXPORT omfErr_t omfiNestedScopeNew(
	omfHdl_t file,          /* IN - File Handle */
	omfDDefObj_t datakind,  /* IN - Datakind object */
	omfLength_t length,     /* IN - Length property value */
	omfSegObj_t *newScope);  /* OUT - New nested scope object */

OMF_EXPORT omfErr_t omfiNestedScopeAppendSlot(
	omfHdl_t file,       /* IN - File Handle */
	omfSegObj_t scope,   /* IN - Scope object */
	omfSegObj_t value);   /* IN - Segment to place in scope slot */

OMF_EXPORT omfErr_t omfiScopeRefNew(
	omfHdl_t file,          /* IN - File Handle */
	omfDDefObj_t datakind,  /* IN - Datakind object */
	omfLength_t length,     /* IN - Length property value */
	omfUInt32 relScope,     /* IN - Relative Scope */
	omfUInt32 relSlot,      /* IN - Relative slot */
	omfSegObj_t *newScopeRef); /* OUT - Scope Reference */

OMF_EXPORT omfErr_t omfiSelectorNew(
	omfHdl_t file,              /* IN - File Handle */
	omfDDefObj_t datakind,      /* IN - Datakind object */
	omfLength_t length,         /* IN - Length property value */
	omfSegObj_t *newSelector);   /* OUT - New Selector object */

OMF_EXPORT omfErr_t omfiSelectorSetSelected(
    omfHdl_t file,        /* IN - File Handle */
	omfSegObj_t selector, /* IN - Selector object */
	omfSegObj_t value);    /* IN - Selected segment in selector */

OMF_EXPORT omfErr_t omfiSelectorAddAlt(
	omfHdl_t file,         /* IN - File Handle */
	omfSegObj_t selector,  /* IN - Selector object */
	omfSegObj_t value);     /* IN - Alternate segment in selector */

OMF_EXPORT omfErr_t omfiEffectDefNew(
	omfHdl_t file,               /* IN - File Handle */
	omfUniqueNamePtr_t effectID, /* IN - Effect identifier */
	omfString effectName,        /* IN - Effect Name (optional) */
	omfString effectDesc,        /* IN - Effect Description (optional) */
	omfArgIDType_t *bypass,      /* IN - Bypass (optional) */
	omfBool isTimeWarp,          /* IN - Is the effect a timewarp */
	omfEDefObj_t *newEffectDef);  /* OUT - New effect definition object */

OMF_EXPORT omfErr_t omfiDatakindNew(
	omfHdl_t file,                   /* IN - File Handle */
	omfUniqueNamePtr_t datakindName, /* IN - Datakind Unique Name */
    omfDDefObj_t *newDatakind);       /* OUT - New Datakind object */

OMF_EXPORT omfErr_t omfiEffectNew(
	omfHdl_t file,          /* IN - File Handle */
	omfDDefObj_t datakind,  /* IN - Datakind object */
	omfLength_t length,     /* IN - Length property value */
	omfEDefObj_t effectDef, /* IN - Effect Definition object */
	omfEffObj_t *newEffect); /* OUT - New Effect object */

OMF_EXPORT omfBool omfiDatakindLookup(
	omfHdl_t file,           /* IN - File Handle */
    omfUniqueNamePtr_t name, /* IN - Datakind Unique Name */
	omfDDefObj_t *defObject, /* OUT - Datakind object */
    omfErr_t     *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiEffectDefLookup(
	omfHdl_t file,           /* IN - File Handle */
    omfUniqueNamePtr_t name, /* IN - Effect Definition Unique Name */
	omfEDefObj_t *defObject, /* OUT - Effect Definition object */
	omfErr_t     *omfError); /* OUT - Error code */

OMF_EXPORT omfErr_t omfiEffectAddNewSlot(
	omfHdl_t file,              /* IN - File Handle */
	omfEffObj_t effect,         /* IN - Effect object */
	omfArgIDType_t argID,       /* IN - Argument ID for the slot */
	omfSegObj_t value,          /* IN - Segment to place in effect slot */
	omfESlotObj_t *effectSlot);  /* OUT - Effect slot object */

OMF_EXPORT omfErr_t omfiEffectSetFinalRender(
	omfHdl_t file,       /* IN - File Handle */
	omfEffObj_t effect,  /* IN - Effect object */
	omfSegObj_t sourceClip); /* IN - Source clip object */
OMF_EXPORT omfErr_t omfiEffectSetWorkingRender(
	omfHdl_t file,       /* IN - File Handle */
	omfEffObj_t effect,  /* IN - Effect object */
	omfSegObj_t sourceClip); /* IN - Source clip object */

OMF_EXPORT omfErr_t omfiEffectSetBypassOverride(
	omfHdl_t file,       /* IN - File Handle */
	omfEffObj_t effect,  /* IN - Effect Object */
	omfArgIDType_t bypassOverride); /* IN - Bypass override */

OMF_EXPORT omfErr_t omfiMediaGroupNew(
    omfHdl_t file,            /* IN - File Handle */
	omfDDefObj_t datakind,    /* IN - Datakind object */
    omfLength_t length,       /* IN - Length property value */
    omfObject_t *mediaGroup);  /* OUT - New Media Group */

OMF_EXPORT omfErr_t omfiMediaGroupSetStillFrame(
    omfHdl_t file,             /* IN - File Handle */
    omfObject_t mediaGroup,    /* IN - Media Group object */
    omfSegObj_t stillFrame);    /* IN - Still Frame source clip */

OMF_EXPORT omfErr_t omfiMediaGroupAddChoice(
    omfHdl_t file,          /* IN - File Handle */
    omfObject_t mediaGroup, /* IN - Media Group object */
    omfSegObj_t choice);     /* IN - Source clip to add as a choice */

/* end of prototypes */

#if PORT_LANG_CPLUSPLUS
  }
#endif
#endif /* _OMF_MOB_BUILD_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
