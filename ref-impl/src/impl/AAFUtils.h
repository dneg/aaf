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

#ifndef _AAF_UTIL_API_
#define _AAF_UTIL_API_ 1

#include <stdio.h>

#if defined(sun)
#include <sys/stdtypes.h>
#endif

#include "aafErr.h"
#include "AAFTypes.h"

class ImplAAFFile;
class ImplAAFObject;
#include "OMUtilities.h"

typedef enum
{
	kRoundCeiling, kRoundFloor
} aafRounding_t;

typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola

AAFByteOrder GetNativeByteOrder(void);

aafBool	EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2);

AAFRESULT aafMobIDNew(aafUID_t *mobID);

/************************************************************************
 *
 * Class, type, and property registration and Information
 *
 ************************************************************************/


/************************************************************************
 *
 * General Object and Property Information Calls
 *
 ************************************************************************/

/*************************************************************************
 * Private Function: OffsetToTimecode()
 *
 *      Given an offset into a track and a frame rate, this function
 *      calculates a timecode value.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
//!!! These should be somewhere else (AAFUtils??)
/* omfmTimecodeToOffset */
aafErr_t PvtOffsetToTimecode(
	aafFrameOffset_t offset, /* IN - Offset into a track */
	aafInt16 frameRate,      /* IN - Frame rate */
	aafDropType_t drop,     /* OUT - Drop or non-drop Timecode */
	aafInt16 *hours,         /* OUT - Hours value of timecode */
	aafInt16 *minutes,       /* OUT - Minutes value of timecode */
	aafInt16 *seconds,       /* OUT - Seconds value of timecode */
	aafInt16 *frames)  ;      /* OUT - Frames value of timecode */
/*************************************************************************
 * Function: TimecodeToOffset()
 *
 *      Given a timecode and a frame rate, this function returns a
 *      position relative to the beginning of a track.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
/* TimecodeToOffset */
aafErr_t PvtTimecodeToOffset(
	aafInt16 frameRate,  /* IN - Frame Rate */
	aafInt16 hours,      /* IN - Hours value of Timecode */
	aafInt16 minutes,    /* IN - Minutes value of Timecode */
	aafInt16 seconds,    /* IN - Seconds value of Timecode */
	aafInt16 frames,     /* IN - Frames value of Timecode */
	aafDropType_t drop,  /* IN - Drop of non-drop Timecode */
	aafFrameOffset_t	*result); /* OUT - resulting position */

/************************
 * Function: omfsGetDateTime			(INTERNAL)
 *
 * 	Returns the number of seconds since the standard root date
 *		for the current machine.  The date returned here will be converted
 *		to the canonical date format in the date write routine.
 *
 * Argument Notes:
 *		Time - is NATIVE format.  That is relative to 1/1/1904 for the
 *			Mac and 1/1/70? for everyone else.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
/* omfsGetDateTime */
void AAFGetDateTime(aafTimeStamp_t *time);

/************************************************************************
 *
 * Utility Functions
 *
 ************************************************************************/

void AAFByteSwap16(
			aafInt16 * wp);	/* IN/OUT -- Byte swap this value */
			
void AAFByteSwap32(
			aafInt32 *lp);	/* IN/OUT -- Byte swap this value */

void AAFByteSwap64(
			aafInt64 *lp);	/* IN/OUT -- Byte swap this value */

#define PROGRESS_UNKNOWN	-1

aafBool isObjFunc(ImplAAFFile * file,
                         ImplAAFObject * obj,
                         void *data);

aafErr_t AAFConvertEditRate(
	aafRational_t srcRate,        /* IN - Source Edit Rate */
	aafPosition_t srcPosition,    /* IN - Source Position */
	aafRational_t destRate,       /* IN - Destination Edit Rate */
	aafRounding_t howRound,	      /* IN - Rounding method (floor or ceiling) */
	aafPosition_t *destPosition) ; /* OUT - Destination Position */

double FloatFromRational(
			aafRational_t	e);		/* IN - Convert this into a double */
aafRational_t RationalFromFloat(
			double	f);		/* IN - Convert this number into a rational */

#endif				/* _AAF_UTIL_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
