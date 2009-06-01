//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef _AAF_UTIL_API_
#define _AAF_UTIL_API_ 1

#include <stdio.h>
#include <string.h>  // For use of memcmp() below.

#include "aafErr.h"
#include "AAFTypes.h"

class ImplAAFFile;
class ImplAAFObject;


typedef enum
{
	kRoundCeiling, kRoundFloor
} aafRounding_t;


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

// aafUID_t comparison operators.

inline bool operator == (const aafUID_t& lhs,
                         const aafUID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafUID_t)) == 0;
}

inline bool operator != (const aafUID_t& lhs,
                         const aafUID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafUID_t)) != 0;
}

inline bool operator < (const aafUID_t& lhs,
                        const aafUID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafUID_t)) < 0;
}

inline bool operator > (const aafUID_t& lhs,
                        const aafUID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafUID_t)) > 0;
}

// aafMobID_t comparison operators.

inline bool operator == (const aafMobID_t& lhs,
                         const aafMobID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafMobID_t)) == 0;
}

inline bool operator != (const aafMobID_t& lhs,
                         const aafMobID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafMobID_t)) != 0;
}

inline bool operator < (const aafMobID_t& lhs,
                        const aafMobID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafMobID_t)) < 0;
}

inline bool operator > (const aafMobID_t& lhs,
                        const aafMobID_t& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(aafMobID_t)) > 0;
}

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



#define PROGRESS_UNKNOWN	-1


aafErr_t AAFConvertEditRate(
	aafRational_t srcRate,        /* IN - Source Edit Rate */
	aafPosition_t srcPosition,    /* IN - Source Position */
	aafRational_t destRate,       /* IN - Destination Edit Rate */
	aafRounding_t howRound,	      /* IN - Rounding method (floor or ceiling) */
	aafPosition_t *destPosition) ; /* OUT - Destination Position */

double FloatFromRational(
			aafRational_t	e);		/* IN - Convert this into a double */


// URL utility functions
void wcsconvertURLtoFilepath(wchar_t *url, wchar_t *filepath);
void wcsconvertFilepathtoURL(wchar_t *filepath, wchar_t *url);

#if defined(_DEBUG)
#define CHECK_CLIENT_IMPLEMENTED_QI(pUnknown, IID) \
{\
  IUnknown* p = 0;\
  HRESULT hr = pUnknown->QueryInterface(IID_IUnknown, (void**)&p);\
  if (SUCCEEDED(hr))\
    p->Release();\
  else \
    return hr;\
  hr = pUnknown->QueryInterface(IID, (void**)&p);\
  if (SUCCEEDED(hr))\
    p->Release();\
  else \
    return hr;\
}
#else
#define CHECK_CLIENT_IMPLEMENTED_QI(pUnknown, IID)
#endif


#endif				/* _AAF_UTIL_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
