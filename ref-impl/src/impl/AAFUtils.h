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
#include "Container.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

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
 * Container Functions
 *
 ************************************************************************/

#if FULL_TOOLKIT
aafBool omfsIsPropPresent(
			AAFFile *			file,		/* IN -- For this aaf file */
			AAFObject *		obj,		/* IN -- in this object */
			aafProperty_t	prop,		/* IN -- read this property */
			aafType_t		dataType);/* IN -- check the type */

/************************************************************************
 *
 * Object Creation / Deletion Functions
 *
 ************************************************************************/

aafErr_t AAFObjectNew(
			AAFFile *				file,		/* IN - In this file */
			aafClassID_t	*classID,	/* IN - create an object of this class */
			AAFObject *			*result);	/* OUT - and return the result here. */
#endif

/************************************************************************
 *
 * Utility Functions
 *
 ************************************************************************/

void *AAFMalloc(
			size_t size);	/* Allocate this many bytes */

void AAFFree(
			void *ptr);	/* Free up this buffer */

#if FULL_TOOLKIT
aafErr_t AAFSetProgressCallback(
			AAFFile *				file,		/* IN - For this file */
			aafProgressProc_t aProc);	/* IN - Set this progress callback */
#endif

/* omfsFixShort */
void AAFByteSwap16(
			aafInt16 * wp);	/* IN/OUT -- Byte swap this value */
			
/* omfsFixLong */
void AAFByteSwap32(
			aafInt32 *lp);	/* IN/OUT -- Byte swap this value */

/* omfsFixLong64 */
void AAFByteSwap64(
			aafInt64 *lp);	/* IN/OUT -- Byte swap this value */

#define PROGRESS_UNKNOWN	-1

aafBool isObjFunc(AAFFile * file,
                         ImplAAFObject * obj,
                         void *data);

#if FULL_TOOLKIT
aafErr_t AAFConvertEditRate(
	aafRational_t srcRate,        /* IN - Source Edit Rate */
	aafPosition_t srcPosition,    /* IN - Source Position */
	aafRational_t destRate,       /* IN - Destination Edit Rate */
	aafRounding_t howRound,	      /* IN - Rounding method (floor or ceiling) */
	aafPosition_t *destPosition) ; /* OUT - Destination Position */
	
AAFObject *AAFNewClassFromContainerObj(AAFFile *file, OMLObject obj);
AAFObject *AAFNewClassFromClassID(AAFFile *file, char *classID, OMLObject obj);
#endif

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _AAF_UTIL_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
