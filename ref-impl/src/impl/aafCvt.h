//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef _AAF_CVT_API_
#define _AAF_CVT_API_ 1

#include "aafErr.h"
#include "AAFTypes.h"

#if PORT_LANG_CPLUSPLUS
extern "C" 
  {
#endif

/************************************************************************
 *
 * 64-bit Unsigned Math Functions
 *
 ************************************************************************/

#if PORT_USE_NATIVE64
#define IsInt64Positive(a) ((a) >= 0)
#define CvtInt32toLength(in, out)		((out) = (in))
#define CvtInt32toPosition(in, out)		((out) = (in))
#define CvtInt32toInt64(in, out)		(*out = (in), AAFRESULT_SUCCESS)
#define CvtUInt32toInt64(in, out)		(*out = (in), AAFRESULT_SUCCESS)
#else
#define IsInt64Positive(a) (((aafInt16)(a).words[0]) >= 0)
#define CvtInt32toLength(in, out)		\
	 (out.words[0] = 0, out.words[1] = 0, \
	  out.words[2] = (aafUInt16)((in >> 16L) & 0xFFFF), \
	  out.words[3] = (aafUInt16)(in & 0xFFFF), out)

#define CvtInt32toPosition(in, out)	\
  (out.words[0] = 0, out.words[1] = 0, \
	out.words[2] = (aafUInt16)((in >> 16L) & 0xFFFF), \
	out.words[3] = (aafUInt16)(in & 0xFFFF), out)
aafErr_t CvtInt32toInt64(
		   aafInt32 in,	/* IN -- Convert this aafInt32 */
		   aafInt64  *out);	/* OUT -- into this aafInt64 */
			
aafErr_t CvtUInt32toInt64(
			aafUInt32		in,		/* IN -- Convert this aafInt32 */
			aafInt64		*out);	/* OUT -- into this aafInt64 */
			
#endif

aafErr_t MakeInt64(aafInt32 high, aafInt32 low, aafInt64 *out);
aafErr_t DecomposeInt64(aafInt64 in, aafInt32 *high, aafInt32 *low);

aafErr_t AddInt32toInt64(
		      aafUInt32	in,	/* IN - Add this aafUInt32 to */
		      aafInt64		*out);	/* IN/OUT - this existing aafInt64 */
			
aafErr_t AddInt64toInt64(
		      aafInt64	in,	/* IN - Add this aafInt64 to */
		      aafInt64	*out);	/* IN/OUT - this existing aafInt64 */
			
aafErr_t SubInt64fromInt64(
			aafInt64	in,     /* IN - Subtract this aafInt64 from */
			aafInt64 *out);		/* IN/OUT - this existing aafInt64 */
			
aafErr_t SubInt32fromInt64(
			aafInt32	in,     /* IN - Subtract this aafInt64 from */
			aafInt64 *out);		/* IN/OUT - this existing aafInt64 */
			
aafErr_t MultInt32byInt64(
		       aafInt32	in, /* IN - Multiply this aafInt32 */
		       aafInt64		in2,	/* IN - by this aafInt64 */
		       aafInt64		*out);	/* OUT - and put the result here. */
			
aafErr_t DivideInt64byInt32(
		       aafInt64		in, /* IN - Divide this aafInt64 */
		       aafUInt32	in2,	/* IN - by this aafUInt32 */
		       aafInt64		*result,	/* OUT - and put the result here. */
		       aafInt32		*remainder);	/* OUT - and put the remainder here. */

aafErr_t TruncInt64toInt32(
			   aafInt64	in,	/* IN - Truncate this value */
			   aafInt32	*out);	/* OUT - to signed 32-bits and put it here */

aafErr_t TruncInt64toUInt32(
			aafInt64 in,		/* IN - Truncate this value */
			aafUInt32 *out);	/* OUT - to unsigned 32-bits and put it here */
			
aafErr_t Int64AndInt64(
			aafInt64	in,		/* IN - Subtract this aafInt64 from */
			aafInt64 *out);		/* IN/OUT - this existing aafInt64 */

aafErr_t Int64ToString(
			aafInt64	in,			/* IN - Print this number */
			aafInt16	base,		/* IN - in this numerical base */
			aafInt16	bufSize,	/* IN - into a buffer of this length */
			char 		*buf);		/* IN/OUT - here is the buffer. */

aafInt16 compare64(		/* Returns -1 if a<b, 0 if a == b, 1 if a > b) */
			aafInt64	a,
			aafInt64 b);

/*****/
#if PORT_USE_NATIVE64
#define Int64Greater(a, b) ((a) > (b))
#define Int64GreaterEqual(a, b) ((a) >= (b))
#define Int64Less(a, b) ((a) < (b))
#define Int64LessEqual(a, b) ((a) <= (b))
#define Int64Equal(a, b) ((a) == (b))
#define Int64NotEqual(a, b) ((a) != (b))
#else
#define Int64Greater(a, b) (compare64(a, b) > 0)
#define Int64GreaterEqual(a, b) (compare64(a, b) >= 0)
#define Int64Less(a, b) (compare64(a, b) < 0)
#define Int64LessEqual(a, b) (compare64(a, b) <= 0)

aafBool Int64Equal(
			aafInt64 a,		/* IN - Is a == b */
			aafInt64 b);
			
aafBool Int64NotEqual(
			aafInt64 a,		/* IN - Is a != b */
			aafInt64 b);
#endif
			

#ifdef AAF_SELF_TEST
void			testUint64(void);
#endif

aafErr_t TimecodeToString(
	aafTimecode_t timeCode,   /* IN - Timecode Value */
	aafInt32 strLen,          /* IN - Length of string to hold timecode */
	wchar_t *tcString);      /* IN/OUT - Pre-allocated buffer to hold
				   *          timecode string
				   */

aafErr_t StringToTimecode(
	const wchar_t *timecodeString, /* IN - Timecode String */
	aafRational_t frameRate,  /* IN - Frame Rate */
	aafTimecode_t *timecode); /* OUT - Timecode Value */


#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _AAF_CVT_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
