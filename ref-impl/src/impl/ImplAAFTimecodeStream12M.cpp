/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
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


ImplAAFTimecodeStream12M::ImplAAFTimecodeStream12M ()
{}


ImplAAFTimecodeStream12M::~ImplAAFTimecodeStream12M ()
{}


#if 0
/****/
  // Override from AAFTimecodeStream
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::GetPositionTimecode (/*[in]*/ aafPosition_t  /*position*/,
      /*[out]*/ aafTimecode_t *  /*timecode*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFTimecodeStream
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::SetPositionTimecode (/*[in]*/ aafPosition_t  /*position*/,
      /*[in]*/ aafTimecode_t  /*timecode*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


  // Override from AAFTimecodeStream
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::GetUserDataLength (/*[out]*/ aafInt32 *length)
{
    if(length == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*length = 4;		// It's always 8 groups of 4 bits
	return AAFRESULT_SUCCESS;
}

  // Override from AAFTimecodeStream
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::GetUserDataAtPosition (/*[in]*/ aafPosition_t  position,
      /*[in]*/ aafInt32  buflen,
      /*[out]*/ aafDataBuffer_t  buffer)
{
    return AAFRESULT_NOT_IMPLEMENTED;
}

  // Override from AAFTimecodeStream
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::SetUserDataAtPosition (/*[in]*/ aafPosition_t  position,
      /*[in]*/ aafInt32  /*buflen*/,
      /*[in]*/ aafDataBuffer_t  /*buffer*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


/****/
  // Override from AAFSegment
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::SegmentOffsetToTC (/*[in]*/ aafPosition_t *  /*pOffset*/,
      /*[out]*/ aafTimecode_t *  /*pTimecode*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSegment
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream12M::SegmentTCToOffset (/*[in]*/ aafTimecode_t *  /*pTimecode*/,
      /*[in]*/ aafRational_t *  /*pEditRate*/,
      /*[out]*/ aafFrameOffset_t *  /*pOffset*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }
#endif

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
	ImplAAFTimecodeStream12M::UnpackTimecode(aafUInt8 *buffer, aafUInt32 buflen, aafTimecode_t *tc)
{
#if 0
	  smpte_12MFormat_t	fmt;
	aafInt16			hours, minutes, seconds, frames;

	if(unpacked == NULL || packed == NULL)
		return AAFRESULT_NULL_PARAM;
    if(unpackedBuflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    if(packedBuflen < 4)
		return AAFRESULT_SMALLBUF;
    XPROTECT()
	{
		memcpy((aafUInt8 *)&fmt, packed, sizeof(smpte_12MFormat_t));
		hours = (((foo.bits56_63 >> 6L) & 0x03) * 10) + ((foo.bits48_55 >> 4L) & 0x0F);
		minutes = (((foo.bits40_47 >> 5L) & 0x07) * 10) + ((foo.bits32_39 >> 4L) & 0x0F);
		seconds = (((foo.bits24_31 >> 5L) & 0x07) * 10) + ((foo.bits16_23 >> 4L) & 0x0F);
		frames = (((foo.bits8_15 >> 6L) & 0x03) * 10) + ((foo.bits0_7 >> 4L) & 0x0F);
		tc->drop = ((foo.bits8_15 >> 5L) & 0x01 ? kTcDrop : kTcNonDrop);
		CHECK(PvtTimecodeToOffset(fps, hours, minutes, seconds, frames, tc->drop, tc->startFrame));
	}
	XEXCEPT
	{
	}
	XEND;
    return AAFRESULT_SUCCESS;
#else
    return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream12M::PackTimecode(aafTimecode_t *tc, aafUInt8 *buffer, aafUInt32 buflen)
{
#if 0
	smpte_12MFormat_t	fmt;
	aafInt16			hours, minutes, seconds, frames;

    if(unpacked == NULL || packed == NULL)
		return AAFRESULT_NULL_PARAM;
    if(unpackedBuflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    if(packedBuflen < 4)
		return AAFRESULT_SMALLBUF;

	CHECK(PvtOffsetToTimecode(tc->startFrame, tc->fps. tc->drop, &hours, &seconds, &seconds, &frames));

	memcpy((aafUInt8 *)&fmt, packed, sizeof(smpte_12MFormat_t));
	fmt.bits0_7 = (fmt.bits0_7 & ~0xF0)		| (((frames % 10) << 4L) & 0xF0);
	fmt.bits8_15 = (fmt.bits8_15 & ~0xC0	| (((frames / 10) << 4L) & 0xC0);
	fmt.bits16_23 = (fmt.bits16_23 & ~0xF0) | (((seconds % 10) << 4L) & 0xF0);
	fmt.bits24_31 = (fmt.bits24_31 & ~0xE0) | (((seconds / 10) << 4L) & 0xE0);
	fmt.bits32_39 = (fmt.bits32_39 & ~0xF0) | (((minutes % 10) << 4L) & 0xF0);
	fmt.bits40_47 = (fmt.bits40_47 & ~0xE0) | (((minutes / 10) << 4L) & 0xE0);
	fmt.bits48_55 = (fmt.bits48_55 & ~0xF0) | (((hours % 10) << 4L) & 0xF0);
	fmt.bits56_63 = (fmt.bits56_63 & ~0xC0) | (((hours / 10) << 4L) & 0xC0);
	fmt.bits8_15 = (fmt.bits8_15 & ~0x20	| (drop == kTcDrop ? 0x20 : 0x00);
    memcpy(packed, (aafUInt8 *)&fmt, sizeof(smpte_12MFormat_t));
	
	return AAFRESULT_SUCCESS;
#else
    return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream12M::UnpackUserBits(aafUInt8 *packed, aafUInt32 packedBuflen,
									  aafUInt8 *unpacked, aafUInt32 unpackedBuflen)
{
	smpte_12MFormat_t	fmt;

	if(unpacked == NULL || packed == NULL)
		return AAFRESULT_NULL_PARAM;
    if(unpackedBuflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    if(packedBuflen < 4)
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
    if(unpackedBuflen < sizeof(smpte_12MFormat_t))
		return AAFRESULT_SMALLBUF;
    if(packedBuflen < 4)
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


