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

#ifndef _OMF_CVT_API_
#define _OMF_CVT_API_ 1

#include "omErr.h"
#include "omTypes.h"

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
#define omfsIsInt64Positive(a) ((a) >= 0)
#define omfsCvtInt32toLength(in, out)		((out) = (in))
#define omfsCvtInt32toPosition(in, out)		((out) = (in))
#define omfsCvtInt32toInt64(in, out)		(*out = (in), OM_ERR_NONE)
#define omfsCvtUInt32toInt64(in, out)		(*out = (in), OM_ERR_NONE)
#else
#define omfsIsInt64Positive(a) (((omfInt16)(a).words[0]) >= 0)
#define omfsCvtInt32toLength(in, out)		\
	 (out.words[0] = 0, out.words[1] = 0, \
	  out.words[2] = (omfUInt16)((in >> 16L) & 0xFFFF), \
	  out.words[3] = (omfUInt16)(in & 0xFFFF), out)

#define omfsCvtInt32toPosition(in, out)	\
  (out.words[0] = 0, out.words[1] = 0, \
	out.words[2] = (omfUInt16)((in >> 16L) & 0xFFFF), \
	out.words[3] = (omfUInt16)(in & 0xFFFF), out)
OMF_EXPORT omfErr_t omfsCvtInt32toInt64(
		   omfInt32 in,	/* IN -- Convert this omfInt32 */
		   omfInt64  *out);	/* OUT -- into this omfInt64 */
			
OMF_EXPORT omfErr_t omfsCvtUInt32toInt64(
			omfUInt32		in,		/* IN -- Convert this omfInt32 */
			omfInt64		*out);	/* OUT -- into this omfInt64 */
			
#endif

OMF_EXPORT omfErr_t omfsMakeInt64(omfInt32 high, omfInt32 low, omfInt64 *out);
OMF_EXPORT omfErr_t omfsDecomposeInt64(omfInt64 in, omfInt32 *high, omfInt32 *low);

OMF_EXPORT omfErr_t omfsAddInt32toInt64(
		      omfUInt32	in,	/* IN - Add this omfUInt32 to */
		      omfInt64		*out);	/* IN/OUT - this existing omfInt64 */
			
OMF_EXPORT omfErr_t omfsAddInt64toInt64(
		      omfInt64	in,	/* IN - Add this omfInt64 to */
		      omfInt64	*out);	/* IN/OUT - this existing omfInt64 */
			
OMF_EXPORT omfErr_t omfsSubInt64fromInt64(
			omfInt64	in,     /* IN - Subtract this omfInt64 from */
			omfInt64 *out);		/* IN/OUT - this existing omfInt64 */
			
OMF_EXPORT omfErr_t omfsSubInt32fromInt64(
			omfInt32	in,     /* IN - Subtract this omfInt64 from */
			omfInt64 *out);		/* IN/OUT - this existing omfInt64 */
			
OMF_EXPORT omfErr_t omfsMultInt32byInt64(
		       omfInt32	in, /* IN - Multiply this omfInt32 */
		       omfInt64		in2,	/* IN - by this omfInt64 */
		       omfInt64		*out);	/* OUT - and put the result here. */
			
OMF_EXPORT omfErr_t omfsDivideInt64byInt32(
		       omfInt64		in, /* IN - Divide this omfInt64 */
		       omfUInt32	in2,	/* IN - by this omfUInt32 */
		       omfInt64		*result,	/* OUT - and put the result here. */
		       omfInt32		*remainder);	/* OUT - and put the remainder here. */

OMF_EXPORT omfErr_t omfsTruncInt64toInt32(
			   omfInt64	in,	/* IN - Truncate this value */
			   omfInt32	*out);	/* OUT - to signed 32-bits and put it here */

OMF_EXPORT omfErr_t omfsTruncInt64toUInt32(
			omfInt64 in,		/* IN - Truncate this value */
			omfUInt32 *out);	/* OUT - to unsigned 32-bits and put it here */
			
OMF_EXPORT omfErr_t omfsInt64AndInt64(
			omfInt64	in,		/* IN - Subtract this omfInt64 from */
			omfInt64 *out);		/* IN/OUT - this existing omfInt64 */

OMF_EXPORT omfErr_t omfsInt64ToString(
			omfInt64	in,			/* IN - Print this number */
			omfInt16	base,		/* IN - in this numerical base */
			omfInt16	bufSize,	/* IN - into a buffer of this length */
			char 		*buf);		/* IN/OUT - here is the buffer. */

OMF_EXPORT omfInt16 compare64(		/* Returns -1 if a<b, 0 if a == b, 1 if a > b) */
			omfInt64	a,
			omfInt64 b);

/*****/
#if PORT_USE_NATIVE64
#define omfsInt64Greater(a, b) ((a) > (b))
#define omfsInt64GreaterEqual(a, b) ((a) >= (b))
#define omfsInt64Less(a, b) ((a) < (b))
#define omfsInt64LessEqual(a, b) ((a) <= (b))
#define omfsInt64Equal(a, b) ((a) == (b))
#define omfsInt64NotEqual(a, b) ((a) != (b))
#else
#define omfsInt64Greater(a, b) (compare64(a, b) > 0)
#define omfsInt64GreaterEqual(a, b) (compare64(a, b) >= 0)
#define omfsInt64Less(a, b) (compare64(a, b) < 0)
#define omfsInt64LessEqual(a, b) (compare64(a, b) <= 0)

OMF_EXPORT omfBool omfsInt64Equal(
			omfInt64 a,		/* IN - Is a == b */
			omfInt64 b);
			
OMF_EXPORT omfBool omfsInt64NotEqual(
			omfInt64 a,		/* IN - Is a != b */
			omfInt64 b);
#endif
			

#ifdef OMFI_SELF_TEST
void			testUint64(void);
#endif

OMF_EXPORT omfErr_t omfsTimecodeToString(
	omfTimecode_t timeCode,   /* IN - Timecode Value */
	omfInt32 strLen,          /* IN - Length of string to hold timecode */
	omfString tcString);      /* IN/OUT - Pre-allocated buffer to hold
				   *          timecode string
				   */

OMF_EXPORT omfErr_t omfsStringToTimecode(
	omfString timecodeString, /* IN - Timecode String */
	omfRational_t frameRate,  /* IN - Frame Rate */
	omfTimecode_t *timecode); /* OUT - Timecode Value */


#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_CVT_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
