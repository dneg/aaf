//=---------------------------------------------------------------------=
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


#ifndef _AAF_UTIL_API_
#define _AAF_UTIL_API_ 1

#include <stdio.h>

//removed by Ian Baker 20031219
//not needed for GCC Solaris build so is it ever used??
//#if defined(sun)
//#include <sys/stdtypes.h>
//#endif

#include "aafErr.h"
#include "AAFTypes.h"

class ImplAAFFile;
class ImplAAFObject;


typedef enum
{
	kRoundCeiling, kRoundFloor
} aafRounding_t;

typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola

AAFByteOrder GetNativeByteOrder(void);

aafBool	EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2);
aafBool	EqualMobID(aafMobID_constref mobID1, aafMobID_constref mobID2);
aafBool aafIsEqualGUID( const GUID& guid1, const GUID& guid2 );
#define aafIsEqualIID(x, y) aafIsEqualGUID(x, y)
#define aafIsEqualCLSID(x, y) aafIsEqualGUID(x, y)

AAFRESULT aafMobIDNew(aafMobID_t *mobID);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	prefix,
        aafUInt32	major,
		aafUInt32	minor,
		aafUInt8	UMIDType,	// 0x01-picture, 0x02-audio, 0x03-sound, 0x04-data
		aafMobID_t *mobID);     /* OUT - Newly created Mob ID */
void aafCreateGUID( GUID *p_guid );
aafUInt32 aafGetTickCount();


// Initializes a new auid
AAFRESULT aafAUIDNew(aafUID_t * auid);

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
