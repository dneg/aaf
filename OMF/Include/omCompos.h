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

/* Name: omCompos.c
 * 
 * Overall Function: The high level API for building simple (flat) compositions
 *
 * Audience: Clients writing OMFI compositions.
 *
 * Public Functions:
 *    omfcSimpleTrackNew(), omfcSimpleTrackClose()
 *    omfcSimpleAppendFiller(), omfcSimpleAppendSourceClip()
 *    omfcSimpleTimecodeTrackNew(), omfcSimpleEdgecodeTrackNew()
 *    omfcSimpleTrackGetLength()
 *    omfcSimpleAppendTransition(), omfcSimpleAppendEffect()
 *    omfcSimpleAppendXXX() - functions for appending specific effects
 * 
 * General error codes returned:
 *    OM_ERR_TRACK_EXISTS
 *        A Track with this trackID already exists
 *    OM_ERR_NOMEMORY
 *        Memory allocation failed, no more heap memory
 *    OM_ERR_STRACK_APPEND_ILLEGAL
 *        This track does not contain a sequence, appending is illegal
 *    OM_ERR_BAD_SLOTLENGTH
 *        Bad Slot Length
 *    OM_ERR_INVALID_DATAKIND
 *        The track datakind does not match the appended component's datakind.
 */

#ifndef _OMFI_COMPOSE_H_
#define _OMFI_COMPOSE_H_ 1

#include "omErr.h"
#include "omTypes.h"
#include "omEffect.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

/* start of prototypes */
OMF_EXPORT omfErr_t omfcSimpleTrackNew(
	omfHdl_t      file,              /* IN - File Handle */
	omfMobObj_t   mob,               /* IN - Mob object */
	omfTrackID_t  trackID,           /* IN - Track ID */
	omfString     trackName,         /* IN - Track Name (optional) */
	omfRational_t editRate,          /* IN - Editrate */
    omfUniqueNamePtr_t datakindName, /* IN - Datakind Name */
	omfTrackHdl_t *trackHdl);        /* OUT - Track Handle */

OMF_EXPORT omfErr_t omfcSimpleTrackClose(
    omfTrackHdl_t trackHdl);      /* IN/OUT - Track Handle */

OMF_EXPORT omfErr_t omfcSimpleAppendFiller(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length);      /* IN - Fill Length */

OMF_EXPORT omfErr_t omfcSimpleAppendSourceClip(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length,       /* IN - Length of source clip */
	omfSourceRef_t sourceRef);/* IN - Source Reference (sourceID, 
							   *      sourceTrackID, startTime)
							   */

OMF_EXPORT omfErr_t omfcSimpleTimecodeTrackNew(
    omfHdl_t      file,        /* IN - File Handle */
    omfMobObj_t   mob,         /* IN - Mob object */
    omfTrackID_t  trackID,     /* IN - Track ID */
    omfString     trackName,   /* IN - Track Name (optional) */
    omfRational_t editRate,    /* IN - Editrate */
    omfLength_t   length,      /* IN - Length of timecode */
    omfTimecode_t timecode,    /* IN - Initial timecode (startFrame, drop,
								*      fps) 
								*/
    omfTrackHdl_t *trackHdl);  /* OUT - Track Handle */

OMF_EXPORT omfErr_t omfcSimpleEdgecodeTrackNew(
    omfHdl_t      file,        /* IN - File Handle */
    omfMobObj_t   mob,         /* IN - Mob object */
    omfTrackID_t  trackID,     /* IN - Track ID */
    omfString     trackName,   /* IN - Track Name (optional) */
    omfRational_t editRate,    /* IN - Editrate */
    omfLength_t   length,      /* IN - Length of edgecode */
	omfEdgecode_t edgecode,    /* IN - Initial edgecode (startFrame, filmKind,
								*      codeFormat)
								*/
    omfTrackHdl_t *trackHdl);  /* OUT - Track Handle */

OMF_EXPORT omfErr_t omfcSimpleTrackGetLength(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t *length);     /* OUT - Length of the track */

OMF_EXPORT omfErr_t omfcSimpleAppendTransition(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length,       /* IN - Length of transition */
	omfPosition_t cutPoint,       /* IN - Cut point */
	omfEffObj_t effect);      /* IN - Effect object */

OMF_EXPORT omfErr_t omfcSimpleAppendEffect(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length,       /* IN - Length of effect */
	omfEffObj_t effect);      /* IN - Effect object */

OMF_EXPORT omfErr_t omfcSimpleAppendMonoAudioDissolve(
    omfTrackHdl_t trackHdl,     /* IN - Track Handle */
	omfLength_t length);        /* IN - Length of transition */

OMF_EXPORT omfErr_t omfcSimpleAppendStereoAudioDissolve(
    omfTrackHdl_t trackHdl,     /* IN - Track Handle */
	omfLength_t length);        /* IN - Length of transition */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoDissolve(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length);      /* IN - Effect length */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoSpeedControl(
    omfTrackHdl_t trackHdl,            /* IN - Track Handle */
	omfLength_t length,                /* IN - Effect Length */
	const omfSegObj_t	inputSegment,  /* IN - Input Segment */
	const omfRational_t	speedRatio);   /* IN - Speed Ratio */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoWipe(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
    omfLength_t length,       /* IN - Effect Length */
    omfInt32 wipeNumber);     /* IN - SMPTE Wipe Number */

OMF_EXPORT omfErr_t omfcSimpleAppendMonoAudioPan(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length,       /* IN - Effect Length */
    omfSegObj_t inputSegment, /* IN - Input segment */
    omfRational_t ratio);     /* IN - Ratio */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoRepeat(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
    omfLength_t length,       /* IN - Effect length */
    omfSegObj_t inputSegment);/* IN - Input segment */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoFadeToBlack(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
    omfLength_t length,       /* IN - Effect length */
    omfSegObj_t inputSegment);/* IN - Input segment */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoFadeFromBlack(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
    omfLength_t length,       /* IN - Effect length */
    omfSegObj_t inputSegment);/* IN - Input segment */

OMF_EXPORT omfErr_t omfcSimpleAppendMonoAudioGain(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length,       /* IN - Effect Length */
    omfSegObj_t inputSegment, /* IN - Input segment */
    omfRational_t multiplier);/* IN - Amplitude Multiplier */

OMF_EXPORT omfErr_t omfcSimpleAppendStereoAudioGain(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
	omfLength_t length,       /* IN - Effect Length */
    omfSegObj_t inputSegment, /* IN - Input segment */
    omfRational_t multiplier);/* IN - Multiplier */

OMF_EXPORT omfErr_t omfcSimpleAppendVideoMask(
    omfTrackHdl_t trackHdl,   /* IN - Track Handle */
    omfLength_t length,       /* IN - Effect length */
    omfSegObj_t inputSegment, /* IN - Input segment */
    omfUInt32 mask,           /* IN - Mask Bits */
    omfFxFrameMaskDirection_t addOrDrop);/* IN - kOmfFxFrameMaskDirNone,
										*      kOmfFxFrameMaskDirAdd,
										*      kOmfFxFrameMaskDirDrop
										*/
/* end of prototypes */

#if PORT_LANG_CPLUSPLUS
}
#endif

#endif /* _OMFI_COMPOSE_H_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/
