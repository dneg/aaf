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

/*
 * Name: omMobGet.c
 *
 * Overall Function: The main API file for traversing mobs.
 *
 * Audience: Clients writing or reading OMFI compositions.
 *
 * Public Functions:
 *     omfiIsAXXX() - IsA calls for classes
 *     omfiIsXXXKind() - IsA calls for datakinds
 *     omfiIsAXXXMob() - IsA calls for mobs
 *     omfiXXXGetInfo() - Get property values for individual objects
 *     omfiSourceClipResolveRef()
 *     omfiGetNumXXX() - Get number of components of a non-atomic object
 *     omfiIteratorAlloc(), omfiIteratorDispose(), omfiIteratorClear()
 *     omfiGetNextXXX() - iterators
 *     omfiMobMatchAndExecute()
 *     omfiConvertEditRate()
 *
 *     omfiMobOpenSearch() -  Open a search iterator on a particular track of a sequence
 *     omfiMobCloseSearch() -  Close a search iterator
 *     omfiMobGetNextSource() - Go to the next value using a search iterator
 *     omfiMobGetThisSource() - Go more information from the current value
 *     omfiMobSearchSource() - The replacement for omfiMobFindSource().  No longer
 *								accepts sequences
 *
 * Obsoleted functions, still present but will be removed:
 *     omfiMobFindSource()
 *
 * General error codes returned:
 *      OM_ERR_NONE
 *             Success
 * 		OM_ERR_BAD_FHDL
 *				Bad file handle passed in
 * 		OM_ERR_BENTO_PROBLEM
 *				Bento returned an error, check BentoErrorNumber.
 *      OM_ERR_NULLOBJECT
 *              Null object passed in as an argument
 *      OM_ERR_INVALID_DATAKIND
 *              An invalid datakind was passed in
 *      OM_ERR_NULL_PARAM
 *              A required parameter was NULL
 *      OM_ERR_NOT_IN_20
 *              This function is not supported in 2.0 (GetSequLength)
 *      OM_ERR_INVALID_OBJ
 *              An object of the wrong class was passed in
 *      OM_ERR_INTERN_TOO_SMALL
 *              Buffer size is too small (GetDataValue)
 *      OM_ERR_MOB_NOT_FOUND
 *              A source clip references a mob that is not in the file
 *      OM_ERR_REQUIRED_POSITIVE
 *              The value (e.g., length) must be positive)
 *      OM_ERR_TRAVERSAL_NOT_POSS
 *              An attempt to traverse a mob failed
 *      OM_ERR_TRACK_NOT_FOUND
 *              A source clip references a track that does not exist in a mob
 *      OM_ERR_NO_MORE_MOBS
 *              The end of the mob chain was reached
 *      OM_ERR_INVALID_MOBTYPE
 *              This operation does not apply to this mob type
 *      OM_ERR_NOMEMORY
 *              We ran out of memory (omfsMalloc failed)
 *      OM_ERR_ITER_WRONG_TYPE
 *              An iterator of the wrong type was passed in
 *      OM_ERR_INVALID_SEARCH_CRIT
 *              This search criteria does not apply to this iterator
 *      OM_ERR_INTERNAL_ITERATOR
 *              An internal iterator error occurred
 */

#ifndef _OMF_MOB_GET_
#define _OMF_MOB_GET_ 1

#include "omErr.h"
#include "omTypes.h"

#if PORT_LANG_CPLUSPLUS
extern "C" 
  {
#endif

/* start of prototypes */

OMF_EXPORT omfBool omfiIsAComponent(
    omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
 	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsASegment(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsASourceClip(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAnEdgecodeClip(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsATimecodeClip(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAVaryValue(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAConstValue(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAFiller(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAnEffect(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsANestedScope(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAScopeRef(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsASelector(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsASequence(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsATransition(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsADatakind(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAnEffectDef(
	omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAMediaGroup(
    omfHdl_t file,       /* IN - File Handle */
    omfObject_t obj,     /* IN - Object to operate on */
    omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfErr_t omfiDatakindGetName(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
	omfInt32 nameSize,   /* IN - Size of name buffer */
	omfUniqueNamePtr_t datakindName);/* IN/OUT - preallocated buffer to
									  * return name string
									  */

OMF_EXPORT omfBool omfiIsCharKind(
	omfHdl_t file,        /* IN - File Handle */
	omfDDefObj_t dataDef, /* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError);  /* OUT - Error code */

OMF_EXPORT omfBool omfiIsUInt8Kind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsUInt16Kind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsUInt32Kind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsInt8Kind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsInt16Kind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsInt32Kind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsRationalKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsBooleanKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsStringKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsColorSpaceKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsColorKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsDistanceKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsPointKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsDirectionCodeKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsPolynomialKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsPictureKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsMatteKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsPictureWithMatteKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsSoundKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsStereoSoundKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsTimecodeKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsEdgecodeKind(
	omfHdl_t file,       /* IN - File Handle */
	omfDDefObj_t dataDef,/* IN - Datakind definition object */
    omfDatakindMatch_t matchKind, /* IN - kExactMatch, kConvertTo, kConvertFrom */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiMobSlotIsTrack(
	omfHdl_t file,       /* IN - File Handle */
	omfMSlotObj_t slot,  /* IN - Mob Slot object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfErr_t omfiComponentGetInfo(
	omfHdl_t file,          /* IN - File Handle */
    omfCpntObj_t component, /* IN - Component object */
    omfDDefObj_t *datakind, /* OUT - Datakind definition object */
    omfLength_t *length);   /* OUT - Length of component */

OMF_EXPORT omfErr_t omfiDataValueGetSize(
    omfHdl_t file,       /* IN - File Handle */
	omfObject_t value,   /* IN - Value object */
    omfLength_t *valueSize); /* OUT - Value size */

OMF_EXPORT omfErr_t omfiMobSlotGetInfo(
	omfHdl_t file,           /* IN - File Handle */
	omfMSlotObj_t slot,      /* IN - Mob Slot Object */
   	omfRational_t *editRate, /* OUT - Edit Rate property value */
   	omfSegObj_t *segment);   /* OUT - Segment property value (an objref)*/

OMF_EXPORT omfErr_t omfiTrackGetInfo(
	omfHdl_t file,           /* IN - File Handle */
	omfMobObj_t mob,         /* IN - Mob object (for 1.x trackID compat)*/
	omfMSlotObj_t slot,      /* IN - Mob Slot object */
   	omfRational_t *editRate, /* OUT - Editrate property value */
   	omfInt32 nameSize,       /* IN - Size of name buffer */
	omfString name,          /* IN/OUT - preallocated buffer for name */
	omfPosition_t *origin,   /* OUT - Origin property value */
	omfTrackID_t *trackID,   /* OUT - TrackID property value */
   	omfSegObj_t *segment);   /* OUT - Segment property value (objref)*/

OMF_EXPORT omfErr_t omfiTrackGetPhysicalNum(
	omfHdl_t		file,      			/* IN - File Handle */
	omfMSlotObj_t	track,				/* IN - An existing track */
	omfUInt32		*physicalTrackNum); 	/* IN - A physical track number */

OMF_EXPORT omfErr_t omfiFillerGetInfo(
	omfHdl_t file,          /* IN - File Handle */
    omfCpntObj_t component, /* IN - Filler object */
    omfDDefObj_t *datakind, /* OUT - Datakind definition object */
    omfLength_t *length);   /* OUT - Length of filler */

OMF_EXPORT omfErr_t omfiSequenceGetInfo(
	omfHdl_t file,          /* IN - File Handle */
    omfCpntObj_t component, /* IN - Sequence object */
    omfDDefObj_t *datakind, /* OUT - Datakind definition object */
    omfLength_t *length);   /* OUT - Length of sequence */

OMF_EXPORT omfErr_t omfiSourceClipGetInfo(
	omfHdl_t file,             /* IN - File Handle */
    omfCpntObj_t component,    /* IN - Source Clip object */
    omfDDefObj_t *datakind,    /* OUT - Datakind definition object */
    omfLength_t *length,       /* OUT - Length of source clip */
	omfSourceRef_t *sourceRef);/* OUT - Source reference (sourceID, 
								*       sourceTrackID, startTime)
								*/

OMF_EXPORT omfErr_t omfiNestedScopeGetInfo(
	omfHdl_t file,          /* IN - File Handle */
    omfCpntObj_t component, /* IN - Nested Scope object */
    omfDDefObj_t *datakind, /* OUT - Datakind definition object */
    omfLength_t *length);   /* OUT - Length of nested scope */

OMF_EXPORT omfErr_t omfiConstValueGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfObject_t constValue, /* IN - Constant value object */
	omfDDefObj_t *datakind, /* OUT - Datakind definition object */
	omfLength_t *length,    /* OUT - Length of constant value */
	omfInt64 valueSize,     /* IN - Size of preallocated buffer */
	omfInt64 *bytesRead,    /* OUT - Number of actual bytes read */
	void *value);           /* IN/OUT - Preallocated buffer to hold value */

OMF_EXPORT omfErr_t omfiVaryValueGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfSegObj_t varyValue,  /* IN - Varying Value Object */
	omfDDefObj_t *datakind, /* OUT - Datakind definition object */
	omfLength_t *length,    /* OUT - Length of varying value object */
	omfInterpKind_t *interpolation);/* OUT - Interpolation method */

OMF_EXPORT omfErr_t omfiControlPtGetInfo(
    omfHdl_t file,            /* IN - File Handle */
	omfCntlPtObj_t controlPt, /* IN - Control Point object */
	omfRational_t *time,      /* OUT - Time of control point */
	omfEditHint_t *editHint,  /* OUT - Edit hint */
	omfDDefObj_t *datakind,   /* OUT - Datakind definition object */
	omfInt32 valueSize,       /* IN - Size of preallocated buffer */
    omfInt32 *bytesRead,      /* OUT - Number of actual bytes read */
	void *value);             /* IN/OUT - Preallocated buffer to hold value */

OMF_EXPORT omfErr_t omfiMobGetInfo(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfUID_t *mobID,     /* OUT - Mob ID */
	omfInt32 strSize,    /* IN - Size of name buffer */
	omfString name,      /* IN/OUT - Preallocated buffer to hold name */
	omfTimeStamp_t *lastModified, /* OUT - Last Modified date */
	omfTimeStamp_t *creationTime);/* OUT - Creation Time */

OMF_EXPORT omfErr_t omfiTimecodeGetInfo(
    omfHdl_t file,            /* IN - File Handle */
    omfSegObj_t timecodeClip, /* IN - Timecode Clip object */
    omfDDefObj_t *datakind,   /* OUT - Datakind definition object */
	omfLength_t *length,      /* OUT - Length of timecode clip */
	omfTimecode_t *timecode); /* OUT - Timecode (startFrame, drop, fps) */

OMF_EXPORT omfErr_t omfiEdgecodeGetInfo(
    omfHdl_t file,            /* IN - File Handle */
	omfSegObj_t edgecodeClip, /* IN - Edgecode Clip object */
    omfDDefObj_t *datakind,   /* OUT - Datakind definition object */
	omfLength_t *length,      /* OUT - Length of edgecode clip */
	omfEdgecode_t *edgecode); /* OUT - Edgecode (startFrame, filmKind, 
							   * codeFormat)
							   */

OMF_EXPORT omfErr_t omfiEffectGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfEffObj_t effect,     /* IN - Effect object */
	omfDDefObj_t *datakind, /* OUT - Datakind definition object */
	omfLength_t *length,    /* OUT - Length of Effect object */
	omfEDefObj_t *effectDef);/* OUT - Effect definition object (objref)*/

OMF_EXPORT omfErr_t omfiEffectDefGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfEDefObj_t effectDef, /* IN - Effect definition object */
	omfInt32 idSize,        /* IN - Size of EffectID buffer */
	omfUniqueNamePtr_t effectID, /* IN/OUT - Preallocated buffer for effectID*/
    omfInt32 nameSize,      /* IN - Size of name buffer */
    omfString nameBuf,      /* IN/OUT - Preallocated buffer for name */
    omfInt32 descSize,      /* IN - Size of description buffer */
    omfString descBuf,      /* IN/OUT - Preallocated buffer for description */
    omfArgIDType_t *bypass, /* OUT - Bypass */
    omfBool *isTimeWarp);   /* OUT - Whether or not a timewarp object */

OMF_EXPORT omfErr_t omfiEffectSlotGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfESlotObj_t slot,     /* IN - Effect Slot object */
    omfArgIDType_t *argID,  /* OUT - ArgID */
	omfSegObj_t *argValue); /* OUT - Arg Value object */

OMF_EXPORT omfErr_t omfiSelectorGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfSegObj_t selector,   /* IN - Selector object */
	omfDDefObj_t *datakind, /* OUT - Datakind definition object */
	omfLength_t *length,    /* OUT - Length of selector */
	omfSegObj_t *selected); /* OUT - Selected slot (objref)*/

OMF_EXPORT omfErr_t omfiTransitionGetInfo(
    omfHdl_t file,          /* IN - File Handle */
	omfSegObj_t transition, /* IN - Transition object */
	omfDDefObj_t *datakind, /* OUT - Datakind definition object */
	omfLength_t *length,    /* OUT - Length of transition */
   	omfPosition_t *cutPoint,    /* OUT - Cut Point */
    omfEffObj_t *effObj);   /* OUT - Effect used by transition (objref)*/

OMF_EXPORT omfErr_t omfiScopeRefGetInfo(
	omfHdl_t file,          /* IN - File Handle */
	omfSegObj_t scopeRef,   /* IN - Scope reference object */
	omfDDefObj_t *datakind, /* OUT - Datakind definition object */
	omfLength_t *length,    /* OUT - Length of scope reference */
	omfUInt32      *relScope, /* OUT - Relative scope */
    omfUInt32      *relSlot); /* OUT - Relative slot */

OMF_EXPORT omfErr_t omfiMediaGroupGetInfo(
	omfHdl_t file,          /* IN - File Handle */
    omfSegObj_t segment,    /* IN - Media Group object */
    omfDDefObj_t *datakind, /* OUT - Datakind definition object */
    omfLength_t *length);   /* OUT - Length of media group */

OMF_EXPORT omfErr_t omfiMobGetMobID(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfUID_t *mobID);    /* OUT - Mob ID */

OMF_EXPORT omfErr_t omfiSourceClipGetFade(
    omfHdl_t file,             		/* IN - File Handle */
	omfSegObj_t sourceClip,   		/* IN - Source clip object */
	omfInt32 *fadeInLen,       		/* OUT - Fade In Length */
	omfFadeType_t *fadeInType, 		/* OUT - Fade In Type */
	omfBool			*fadeInPresent, 	/* OUT - Fade In Type */
	omfInt32 *fadeOutLen,      		/* OUT - Fade Out Length */
	omfFadeType_t *fadeOutType,		/* OUT - Fade Out Type */
	omfBool		*fadeOutPresent); 	/* OUT - Fade In Type */

OMF_EXPORT omfErr_t omfiEffectGetFinalRender(
    omfHdl_t file,          /* IN - File Handle */
	omfEffObj_t effect,     /* IN - Effect object */
	omfSegObj_t *sourceClip);/* OUT - Final Rendering Source Clip */

OMF_EXPORT omfErr_t omfiEffectGetWorkingRender(
    omfHdl_t file,           /* IN - File Handle */
	omfEffObj_t effect,      /* IN - Effect object */
	omfSegObj_t *sourceClip);/* OUT - Working Rendering Source Clip */

OMF_EXPORT omfBool omfiEffectIsATimeWarp(
    omfHdl_t file,       /* IN - File Handle */
	omfEffObj_t effect,  /* IN - Effect object */
 	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfErr_t omfiEffectGetBypassOverride(
    omfHdl_t file,       /* IN - File Handle */
	omfEffObj_t effect,  /* IN - Effect object */
	omfArgIDType_t *bypassOverride);/* OUT - Bypass override property value */

OMF_EXPORT omfBool omfiIsAPrimaryMob(
    omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAMob(
	omfHdl_t file,       /* IN - File Handle */
    omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsASourceMob(
	omfHdl_t file,       /* IN - File Handle */
    omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsACompositionMob(
	omfHdl_t file,       /* IN - File Handle */
    omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAMasterMob(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAFileMob(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsATapeMob(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfBool omfiIsAFilmMob(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfErr_t *omfError); /* OUT - Error code */

OMF_EXPORT omfErr_t omfiSourceClipResolveRef(
    omfHdl_t file,           /* IN - File Handle */
	omfSegObj_t sourceClip,  /* IN - Source Clip object */
	omfMobObj_t *mob);       /* OUT - Referenced mob */

OMF_EXPORT omfErr_t omfiMobFindSource(
    omfHdl_t file,           /* IN - File Handle */
	omfMobObj_t rootMob,     /* IN - Root Mob to search */
	omfTrackID_t trackID,    /* IN - Track ID to search */
	omfPosition_t offset,    /* IN - Offset into Track */
	omfMobKind_t mobKind,    /* IN - Kind of mob to look for */
	omfMediaCriteria_t *mediaCrit, /* IN - Media Criteria, if find media group*/
	omfUID_t *mobID,         /* OUT - Found mob's Mob ID */
	omfTrackID_t *mobTrack,  /* OUT - Found Mob's Track */
	omfPosition_t *mobOffset,/* OUT - Found Mob's Track offset */
	omfLength_t *minLength,  /* OUT - Minimum clip length found in chain */
	omfMobObj_t *mob);       /* OUT - Pointer to found mob */

OMF_EXPORT omfErr_t omfiGetNumMobs(
    omfHdl_t file,           /* IN - File Handle */
	omfMobKind_t mobKind,    /* IN - Kind of mob to count */
	omfNumSlots_t *numMobs); /* OUT - Total number of mobs of kind mobKind */

OMF_EXPORT omfErr_t omfiMobGetNumTracks(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfNumTracks_t *numTracks);/* OUT - Number of tracks in the mob */

OMF_EXPORT omfErr_t omfiMobGetNumSlots(
	omfHdl_t file,       /* IN - File Handle */
	omfMobObj_t mob,     /* IN - Mob object */
	omfNumSlots_t *numSlots);/* OUT - Number of slots in the mob */

OMF_EXPORT omfErr_t omfiNestedScopeGetNumSlots(
    omfHdl_t file,       /* IN - File Handle */
	omfSegObj_t scope,   /* IN - Scope object */
	omfNumSlots_t *numSlots);/* OUT - Number of slots in the scope */

OMF_EXPORT omfErr_t omfiSelectorGetNumAltSlots(
    omfHdl_t file,         /* IN - File Handle */
	omfSegObj_t selector,  /* IN - Selector object */
	omfNumSlots_t *numSlots);/* OUT - Number of slots in the selector */

OMF_EXPORT omfErr_t omfiSequenceGetNumCpnts(
    omfHdl_t file,        /* IN - File Handle */
	omfSegObj_t sequence, /* IN - Sequence object */
	omfInt32 *numCpnts);  /* OUT - Number of components in sequence */

OMF_EXPORT omfErr_t omfiEffectGetNumSlots(
    omfHdl_t file,       /* IN - File Handle */
	omfSegObj_t effect,  /* IN - Effect object */
	omfNumSlots_t *numSlots);/* OUT - Number of slots in the effect */

OMF_EXPORT omfErr_t omfiEffectGetNumWorkingRenders(
    omfHdl_t file,       /* IN - File Handle */
	omfSegObj_t effect,  /* IN - Effect object */
	omfInt32 *numRenderings);/* OUT - Number of working renderings */

OMF_EXPORT omfErr_t omfiEffectGetNumFinalRenders(
    omfHdl_t file,       /* IN - File Handle */
	omfSegObj_t effect,  /* IN - Effect object */
	omfInt32 *numRenderings);/* OUT - Number of final renderings */

OMF_EXPORT omfErr_t omfiVaryValueGetNumPoints(
    omfHdl_t file,       /* IN - File Handle */
	omfSegObj_t value,   /* IN - Varying value object */
	omfInt32 *numPoints);/* OUT - Number of control points */

OMF_EXPORT omfErr_t omfiIteratorAlloc(
	omfHdl_t file,           /* IN - File Handle */
	omfIterHdl_t *iterHdl);  /* OUT - Newly allocated iterator handle */

OMF_EXPORT omfErr_t omfiIteratorDispose(
	omfHdl_t file,        /* IN - File Handle */
	omfIterHdl_t iterHdl);/* IN/OUT - Iterator handle */

OMF_EXPORT omfErr_t omfiIteratorClear(
	omfHdl_t file,         /* IN - File Handle */
	omfIterHdl_t iterHdl); /* IN/OUT - Iterator Handle */

OMF_EXPORT omfErr_t omfiGetNextMob(
						omfIterHdl_t iterHdl,        /* IN - Iterator handle */
						omfSearchCrit_t *searchCrit, /* IN - Search Criteria */
						omfMobObj_t *mob);           /* OUT - Found Mob */

OMF_EXPORT omfErr_t omfiMobGetNextSlot(
	omfIterHdl_t iterHdl,         /* IN - Iterator Handle */
	omfMobObj_t mob,              /* IN - Mob to search */
	omfSearchCrit_t *searchCrit,  /* IN - Search Criteria */
	omfMSlotObj_t *slot);         /* OUT - Found Track/Slot */

OMF_EXPORT omfErr_t omfiMobGetNextTrack(
	omfIterHdl_t iterHdl,         /* IN - Iterator Handle */
	omfMobObj_t mob,              /* IN - Mob to search */
	omfSearchCrit_t *searchCrit,  /* IN - Search Criteria */
	omfMSlotObj_t *track);        /* OUT - Found Track/Slot */

OMF_EXPORT omfErr_t omfiEffectGetNextSlot(
    omfIterHdl_t iterHdl,        /* IN - Iterator Handle */
	omfCpntObj_t effect,         /* IN - Effect to search */
	omfSearchCrit_t *searchCrit, /* IN - Search Criteria */
	omfESlotObj_t *effSlot);     /* OUT - Found effect slot */

OMF_EXPORT omfErr_t omfiSelectorGetNextAltSlot(
    omfIterHdl_t iterHdl,         /* IN - Iterator Handle */
	omfCpntObj_t selector,        /* IN - Selector object to search */
	omfSearchCrit_t *searchCrit,  /* IN - Search Criteria */
	omfSegObj_t *selSlot);        /* OUT - Found Selector slot */

OMF_EXPORT omfErr_t omfiNestedScopeGetNextSlot(
    omfIterHdl_t iterHdl,        /* IN - Iterator Handle */
	omfCpntObj_t scope,          /* IN - Scope object to search */
	omfSearchCrit_t *searchCrit, /* IN - Search Criteria */
	omfSegObj_t *scopeSlot);     /* OUT - Found Scope Slot */

OMF_EXPORT omfErr_t omfiSequenceGetNextCpnt(
 	omfIterHdl_t iterHdl,         /* IN - Iterator Handle */
	omfCpntObj_t sequence,        /* IN - Sequence to Search */
	omfSearchCrit_t *searchCrit,  /* IN - Search Criteria */
	omfPosition_t *offset,        /* OUT - Offset into sequence for cpnt */
	omfCpntObj_t *component);     /* OUT - Found component */

OMF_EXPORT omfErr_t omfiEffectGetNextWorkingRender(
    omfIterHdl_t iterHdl,
	omfCpntObj_t effect,
	omfSearchCrit_t *searchCrit,
	omfMobObj_t *fileMob);

OMF_EXPORT omfErr_t omfiEffectGetNextFinalRender(
    omfIterHdl_t iterHdl,
	omfCpntObj_t effect,
	omfSearchCrit_t *searchCrit,
	omfMobObj_t *fileMob);

OMF_EXPORT omfErr_t omfiVaryValueGetNextPoint(
    omfIterHdl_t iterHdl,         /* IN - Iterator Handle */
	omfCpntObj_t varyValue,       /* IN - Varying value to search */
	omfSearchCrit_t *searchCrit,  /* IN - Search Criteria */
	omfCntlPtObj_t *control);     /* OUT - Found control point */

typedef omfErr_t (CallbackFunc)(
								omfHdl_t file,   /* IN - File Handle */
								omfObject_t obj, /* IN - Object to Execute */
								omfInt32 level,  /* IN - Depth Level */
								void *data);     /* IN/OUT - Execute data */

typedef omfBool (MatchFunc)(
							omfHdl_t file,   /* IN - File Handle */
							omfObject_t obj, /* IN - Object to Match */ 
							void *data);     /* IN/OUT - Match data */

OMF_EXPORT omfErr_t omfiMobMatchAndExecute(
	omfHdl_t     file,          /* IN - File Handle */
	omfObject_t  rootObj,       /* IN - Root object */
	omfInt32 	 level,         /* IN - Depth Level in the Hierarchy */
	MatchFunc    *matchFunc,    /* IN - Match Callback Function */
	void         *matchData,    /* IN - Data for Match Callback */
	CallbackFunc *callbackFunc, /* IN - Execute Callback Function */
    void         *callbackData, /* IN - Data for Execute Callback */
	omfInt32     *matches);     /* OUT - Number of matches found */

/*************************************************************************
 * Function: omfiMobGetDefaultFade()
 * NOTE: If there is no default fade, this function returns with no error,
 *		but the VALID field of the structure is false.  This allows you to
 *		pass this struct to omfiSourceClipGetFade() in all cases.
 *************************************************************************/
OMF_EXPORT omfErr_t omfiMobGetDefaultFade(
	omfHdl_t			file,       /* IN - File Handle */
	omfMobObj_t			mob,   		/* IN - Mob object */
	omfDefaultFade_t	*result);	/* OUT - a default fade struct */

OMF_EXPORT omfErr_t omfiMobGetNumComments(
	omfHdl_t	file,
	omfMobObj_t	mob,
	omfInt32	*numComments);

OMF_EXPORT omfErr_t omfiMobGetNextComment(
	omfIterHdl_t	iterHdl,
	omfMobObj_t		mob,
	omfInt32		categoryBufferSize,
	char			*category,
	omfInt32		commentBufferSize,
	char			*comment);

OMF_EXPORT omfErr_t omfiConvertEditRate(
	omfRational_t srcRate,        /* IN - Source Edit Rate */
	omfPosition_t srcPosition,    /* IN - Source Position */
	omfRational_t destRate,       /* IN - Destination Edit Rate */
	omfRounding_t howRound,	      /* IN - Rounding method (floor or ceiling)*/
	omfPosition_t *destPosition); /* OUT - Destination Position */

OMF_EXPORT omfErr_t omfiMobPurge(
	omfHdl_t file,   /* IN - File Handle */
	omfObject_t mob);  /* IN - Mob ID of mob to purge */

OMF_EXPORT omfErr_t omfiPulldownMapOffset(omfHdl_t file,      /* IN */
			   omfObject_t pulldown,		/* IN */
			   omfPosition_t offset,		/* IN */
			   omfBool reverse,
			   omfLength_t *numFrames,
			   omfInt32 *srcPhase);

OMF_EXPORT omfErr_t omfiMobOpenSearch(	omfHdl_t file,	/* IN */
	omfMobObj_t mob,    					/* IN */
	omfObject_t track,         				/* IN */
	omfMobFindHdl_t *mobFindHdl);			/* OUT */

OMF_EXPORT omfErr_t omfiMobCloseSearch(omfHdl_t file,	/* IN */
	omfMobFindHdl_t mobFindHdl);			/* IN */

OMF_EXPORT omfErr_t omfiMobGetNextSource(
    omfMobFindHdl_t mobFindHdl,       /* IN */
	omfMobKind_t mobKind,             /* IN */
	omfMediaCriteria_t *mediaCrit,    /* IN */
	omfEffectChoice_t *effectChoice,  /* IN */
	omfCpntObj_t *thisCpnt,           /* OUT */
	omfPosition_t *thisPosition,      /* OUT */
	omfFindSourceInfo_t *sourceInfo,  /* OUT */
    omfBool *foundTransition);        /* OUT */

OMF_EXPORT omfErr_t omfiMobGetThisSource(
    omfMobFindHdl_t hdl,       /* IN */
	omfMobKind_t mobKind,             /* IN */
	omfMediaCriteria_t *mediaCrit,    /* IN */
	omfEffectChoice_t *effectChoice,  /* IN */
	omfCpntObj_t *thisCpnt,           /* OUT */
	omfPosition_t *thisPosition,      /* OUT */
	omfFindSourceInfo_t *sourceInfo,  /* OUT */
    omfBool *foundTransition);        /* OUT */

OMF_EXPORT omfErr_t omfiMobSearchSource(	
    omfHdl_t file,                    /* IN */
    omfMobObj_t rootMob,              /* IN */
    omfTrackID_t trackID,             /* IN */
	omfPosition_t offset,             /* IN */
	omfMobKind_t mobKind,             /* IN */
	omfMediaCriteria_t *mediaCrit,    /* IN */
	omfEffectChoice_t *effectChoice,  /* IN */
	omfCpntObj_t *thisCpnt,           /* OUT */
	omfFindSourceInfo_t *sourceInfo); /* OUT */

OMF_EXPORT omfErr_t omfiOffsetToMobTimecode(
    omfHdl_t file,            		/* IN */
	omfObject_t mob,          		/* IN */
	omfSegObj_t		tcSeg,         	/* IN */
	omfPosition_t offset,     		/* IN */
	omfTimecode_t	*result);  		/* OUT */

/* end of prototypes */

#if PORT_LANG_CPLUSPLUS
  }
#endif
#endif /* _OMF_MOB_GET_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
