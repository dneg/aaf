/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
\***********************************************/

// The byte stream is layed out as a sequence of bits numbered 0-79 with bit 0 being
// the first bit.  The data is stored as a struct containing characters, and bit 0 is
// the MSB of the first character.
//
// The bit definitions are as follows:
//	0-3		Units of frames				(foo.bits0_7 >> 4L) & 0x0F
//	4-7		First Binary Group			foo.bits0_7 & 0x0F
//	8-9		Tens of frames				(foo.bits8_15 >> 6L) & 0x03
//	10		Drop Frame					(foo.bits8_15 >> 5L) & 0x01
//	11		Color Frame					(foo.bits8_15 >> 4L) & 0x01
//	12-15	Second Binary Group			foo.bits8_15 & 0x0F
//	16-19	Units of Seconds			(foo.bits16_23 >> 4L) & 0x0F
//	20-23	Third Binary Group			foo.bits16_23 & 0x0F
//	24-26	Tens of Seconds				(foo.bits24_31 >> 5L) & 0x07
//	27		Bi-phase mark phase correction bit	(foo.bits24_31 >> 7L) & 0x01
//	28-31	Fourth Binary Group			foo.bits24_31 & 0x0F
//	32-35	Units of minutes			(foo.bits32_39 >> 4L) & 0x0F
//	36-39	Fifth Binary Group			foo.bits32_39 & 0x0F
//	40-42	Tens of Minutes				(foo.bits40_47 >> 5L) & 0x07
//	43		Binary Group Flag Bit		(foo.bits40_47 >> 4L) & 0x01
//	44-47	Sixth Binary Group			foo.bits40_47 & 0x0F
//	48-51	Units of Hours				(foo.bits48_55 >> 4L) & 0x0F
//	52-55	Seventh Binary Group		foo.bits48_55 & 0x0F
//	56-57	Tens of Hours				(foo.bits56_63 >> 6L) & 0x03
//	58		Unassigned address bit		(foo.bits56_63 >> 5L) & 0x01
//	59		Binary Group Flag Bit		(foo.bits56_63 >> 4L) & 0x01
//	60-63	Eighth Binary Group			foo.bits56_63 & 0x0F
//	64-71	Synch Word (Fixed 0x3F)
//	72-79	Synch Word (Fixed 0xFD)

typedef struct
{
	char	bits0_7;
	char	bits8_15;
	char	bits16_23;
	char	bits24_31;
	char	bits32_39;
	char	bits40_47;
	char	bits48_55;
	char	bits56_63;
	char	bits64_71;
	char	bits72_79;
} smpte_12MFormat_t;


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTimecodeStream12M_h__
#include "ImplAAFTimecodeStream12M.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "aafUtils.h"

ImplAAFTimecodeStream12M::ImplAAFTimecodeStream12M ()
{}


ImplAAFTimecodeStream12M::~ImplAAFTimecodeStream12M ()
{}


/****/
  // Override from AAFTimecodeStream
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::GetSampleSize (/*[out]*/ aafUInt32 *pSampleSize)
{
	if(pSampleSize == NULL)
		return AAFRESULT_NULL_PARAM;

	*pSampleSize = sizeof(smpte_12MFormat_t);
	return AAFRESULT_SUCCESS;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream12M::UnpackTimecode(aafUInt8 *buffer, aafUInt32 buflen, aafUInt32 fps, aafTimecode_t *tc)
{
	smpte_12MFormat_t	fmt;
	aafInt16			hours, minutes, seconds, frames;

	if(buffer == NULL || tc == NULL)
		return AAFRESULT_NULL_PARAM;
    if(buflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    XPROTECT()
	{
		memcpy((aafUInt8 *)&fmt, buffer, sizeof(smpte_12MFormat_t));
		hours = (((fmt.bits56_63 >> 6L) & 0x03) * 10) + ((fmt.bits48_55 >> 4L) & 0x0F);
		minutes = (((fmt.bits40_47 >> 5L) & 0x07) * 10) + ((fmt.bits32_39 >> 4L) & 0x0F);
		seconds = (((fmt.bits24_31 >> 5L) & 0x07) * 10) + ((fmt.bits16_23 >> 4L) & 0x0F);
		frames = (((fmt.bits8_15 >> 6L) & 0x03) * 10) + ((fmt.bits0_7 >> 4L) & 0x0F);
		tc->drop = ((fmt.bits8_15 >> 5L) & 0x01 ? kTcDrop : kTcNonDrop);
		CHECK(PvtTimecodeToOffset((aafInt16)fps, hours, minutes, seconds, frames, tc->drop, &tc->startFrame));
	}
	XEXCEPT
	{
	}
	XEND;
    return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream12M::PackTimecode(aafTimecode_t *tc, aafUInt8 *buffer, aafUInt32 buflen)
{
	smpte_12MFormat_t	fmt;
	aafInt16			hours, minutes, seconds, frames;

    if(tc == NULL || buffer == NULL)
		return AAFRESULT_NULL_PARAM;
    if(buflen < sizeof(fmt))
		return AAFRESULT_SMALLBUF;

	XPROTECT()
	{
		CHECK(PvtOffsetToTimecode(tc->startFrame, tc->fps, tc->drop, &hours, &minutes, &seconds, &frames));
		
		memcpy((aafUInt8 *)&fmt, buffer, sizeof(smpte_12MFormat_t));
		fmt.bits0_7 = (fmt.bits0_7 & ~0xF0)		| (((frames % 10) << 4L) & 0xF0);
		fmt.bits8_15 = (fmt.bits8_15 & ~0xC0)	| (((frames / 10) << 6L) & 0xC0);
		fmt.bits16_23 = (fmt.bits16_23 & ~0xF0) | (((seconds % 10) << 4L) & 0xF0);
		fmt.bits24_31 = (fmt.bits24_31 & ~0xE0) | (((seconds / 10) << 5L) & 0xE0);
		fmt.bits32_39 = (fmt.bits32_39 & ~0xF0) | (((minutes % 10) << 4L) & 0xF0);
		fmt.bits40_47 = (fmt.bits40_47 & ~0xE0) | (((minutes / 10) << 5L) & 0xE0);
		fmt.bits48_55 = (fmt.bits48_55 & ~0xF0) | (((hours % 10) << 4L) & 0xF0);
		fmt.bits56_63 = (fmt.bits56_63 & ~0xC0) | (((hours / 10) << 6L) & 0xC0);
		fmt.bits8_15 = (fmt.bits8_15 & ~0x20)	| (tc->drop == kTcDrop ? 0x20 : 0x00);
		memcpy(buffer, (aafUInt8 *)&fmt, sizeof(smpte_12MFormat_t));
	}
	XEXCEPT
	XEND;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream12M::UnpackUserBits(aafUInt8 *packed, aafUInt32 packedBuflen,
									  aafUInt8 *unpacked, aafUInt32 unpackedBuflen)
{
	smpte_12MFormat_t	fmt;

	if(unpacked == NULL || packed == NULL)
		return AAFRESULT_NULL_PARAM;
    if(packedBuflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    if(unpackedBuflen < 4)
		return AAFRESULT_SMALLBUF;
    memcpy((aafUInt8 *)&fmt, packed, sizeof(smpte_12MFormat_t));
	unpacked[0] = ((fmt.bits0_7 & 0x0F) << 4L) | (fmt.bits8_15 & 0x0F);
	unpacked[1] = ((fmt.bits16_23 & 0x0F) << 4L) | (fmt.bits24_31 & 0x0F);
	unpacked[2] = ((fmt.bits32_39 & 0x0F) << 4L) | (fmt.bits40_47 & 0x0F);
	unpacked[3] = ((fmt.bits48_55 & 0x0F) << 4L) | (fmt.bits56_63 & 0x0F);
    return AAFRESULT_SUCCESS;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream12M::PackUserBits(aafUInt8 *unpacked, aafUInt32 unpackedBuflen,
									  aafUInt8 *packed, aafUInt32 packedBuflen)
{
	smpte_12MFormat_t	fmt;

    if(unpacked == NULL || packed == NULL)
		return AAFRESULT_NULL_PARAM;
    if(unpackedBuflen < 4)
		return AAFRESULT_SMALLBUF;
    if(packedBuflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    memcpy((aafUInt8 *)&fmt, packed, sizeof(smpte_12MFormat_t));
	fmt.bits0_7 = (fmt.bits0_7 & ~0x0F)		| ((unpacked[0] >> 4L) & 0x0F);
	fmt.bits8_15 = (fmt.bits8_15 & ~0x0F)	| (unpacked[0] & 0x0F);
	fmt.bits16_23 = (fmt.bits16_23 & ~0x0F) | ((unpacked[1] >> 4L) & 0x0F);
	fmt.bits24_31 = (fmt.bits24_31 & ~0x0F) | (unpacked[1] & 0x0F);
	fmt.bits32_39 = (fmt.bits32_39 & ~0x0F) | ((unpacked[2] >> 4L) & 0x0F);
	fmt.bits40_47 = (fmt.bits40_47 & ~0x0F) | (unpacked[2] & 0x0F);
	fmt.bits48_55 = (fmt.bits48_55 & ~0x0F) | ((unpacked[3] >> 4L) & 0x0F);
	fmt.bits56_63 = (fmt.bits56_63 & ~0x0F) | (unpacked[3] & 0x0F);
    memcpy(packed, (aafUInt8 *)&fmt, sizeof(smpte_12MFormat_t));
	
	return AAFRESULT_SUCCESS;
}

OMDEFINE_STORABLE(ImplAAFTimecodeStream12M, AUID_AAFTimecodeStream12M);


