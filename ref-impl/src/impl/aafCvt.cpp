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

/*
 * Name: omCvt.c
 *
 * Function:
 *		Contains conversion routines, including 64-bit unsigned 
 *              math package, used by the media and composition layers,
 *              and the timecode conversion routines.
 *
 *              The 64-bit math packages are wrapped with a 
 *              #if PORT_USE_NATIVE64 in case someone wants to replace
 *              them with their own platform-dependent versions.  Define
 *				PORT_USE_NATIVE64 as 1 for your platform in portkey.h, and
 *				define PORTKEY_INT64_TYPE to the type name (ie: long long).
 *
 *				The code uses the #define 'PORT_USE_NATIVE64'
 *				which is keyed off of PORTKEY_INT64_NATIVE, with the addiition
 *				that the Makefile may define 'AAF_DONT_USE_NATIVE64' in order to disable
 *				64-bit native math on a machine which would otherwise support it
 *				(see the bottom of portvars.h).
 *
 * Audience:
 *		Generally internal to the toolkit, although clients may
 *		use these if needed.
 *
 * Function Summary:
 *      IsInt64Positive(aafInt64)
 *      CvtInt32toLength(in, out)
 *      CvtInt32toPosition(in, out)
 *      CvtInt32toInt64(aafUInt32, aafInt64 *)
 *      AddInt32toInt64(aafUInt32, aafInt64 *)
 *      AddInt64toInt64(aafInt64, aafInt64 *)
 *      SubInt64fromInt64(aafInt64, aafInt64 *)
 *      MultInt32byInt64(aafInt32, aafInt64, aafInt64 *)
 *      TruncInt64toInt32(aafInt64, aafInt32 *)		 OK
 *      TruncInt64toUInt32(aafInt64, aafUInt32 *)	 OK
 *      Int64Less(aafInt64, aafInt64)
 *      Int64Greater(aafInt64, aafInt64)
 *      Int64LessEqual(aafInt64, aafInt64)
 *      Int64GreaterEqual(aafInt64, aafInt64)
 *      Int64Equal(aafInt64, aafInt64)
 *      Int64NotEqual(aafInt64, aafInt64)
 *
 *      TimecodeToString()
 *      StringToTimecode()
 *
 * General error codes returned:
 *		<see below>
 */

//#include "masterhd.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 
#include <wchar.h>

//#include "omPublic.h"
#include "AAFTypes.h"
#include "aafCvt.h" 
//#include "omPvt.h" 
//#include "AAFPrivate.h"
#include "AAFUtils.h"
#include "AAFResult.h"

/* Moved math.h down here to make NEXT's compiler happy */
#include <math.h>

/* Definitions for Timecode calculations */
#define FPSEC		((aafInt32)(30))
#define FPMIN		((aafInt32)(60*FPSEC))
#define FPHR		((aafInt32)(60*FPMIN))
#define PALFPSEC	((aafInt32)(25))
#define PALFPMIN	((aafInt32)(60*PALFPSEC))
#define PALFPHR		((aafInt32)(60*PALFPMIN))

#define DFPMIN	((aafInt32)(FPMIN-2))				/* minutes */
#define DFP10M	((aafInt32)(10*(FPMIN-2)+2))		/* df 10 minutes */
#define DFPHR	((aafInt32)(6*(10*(FPMIN-2)+2)))	/* hours */

static aafInt32 nondropTbl[] = { 
  1L, 10L, FPSEC, FPSEC*10, FPMIN, FPMIN*10, FPHR, FPHR*10 };
static aafInt32 dropTbl[] = { 
  1L, 10L, FPSEC, FPSEC*10, DFPMIN, DFP10M, DFPHR, DFPHR*10 };
static aafInt32 PALnondropTbl[] = { 
  1L, 10L, PALFPSEC, PALFPSEC*10, PALFPMIN, PALFPMIN*10, PALFPHR, PALFPHR*10 };
//static aafInt32 PALdropTbl[]    = { 
//  1L, 10L, PALFPSEC, PALFPSEC*10, PALFPMIN, PALFPMIN*10, PALFPHR, PALFPHR*10 };
/* End of Timecode definitions */

aafErr_t MakeInt64(aafInt32 high, aafInt32 low, aafInt64 *out)
{
	if(out != NULL)
	{
#if PORT_USE_NATIVE64
	  *out = ((PORTKEY_INT64_TYPE)high << (PORTKEY_INT64_TYPE)32L) | (PORTKEY_INT64_TYPE)low;
#else
		out->words[0] = (aafUInt16)((high >> 16L) & 0xFFFF);
		out->words[1] = (aafUInt16)(high & 0xFFFF);
		out->words[2] = (aafUInt16)((low >> 16L) & 0xFFFF);
		out->words[3] = (aafUInt16)(low & 0xFFFF);
#endif
		return(AAFRESULT_SUCCESS);
	}
	else
		return(AAFRESULT_NULL_PARAM);
}

aafErr_t DecomposeInt64(aafInt64 in, aafInt32 *high, aafInt32 *low)
{
#if PORT_USE_NATIVE64
	if(high != NULL)
		*high = (aafInt32)((in >> (PORTKEY_INT64_TYPE)32L) & (PORTKEY_INT64_TYPE)0xFFFFFFFF);
	if(low != NULL)
		*low = (aafInt32)(in & 0xFFFFFFFF);
#else
	if(high != NULL)
		*high = ((aafUInt32)in.words[0] << 16L) | in.words[1];
	if(low != NULL)
		*low = ((aafUInt32)in.words[2] << 16L) | in.words[3];
#endif

	return(AAFRESULT_SUCCESS);
}

/************************
 * Function: CvtInt32toInt64
 *
 * 		Initialized a 64-bit integer with a value from an aafUInt32.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).  The error code in the opaque handle
 *		is not updated.
 *
 * Possible Errors:
 * 		AAFRESULT_NULL_PARAM -- A NULL result pointer.
 */
#if !PORT_USE_NATIVE64
aafErr_t CvtInt32toInt64(
			aafInt32		in,		/* IN -- Convert this aafInt32 */
			aafInt64		*out)	/* OUT -- into this aafInt64 */
{
	if(out != NULL)
	{
		if(in < 0)
		{
			out->words[0] = 0xFFFF;
			out->words[1] = 0xFFFF;
		}
		else
		{
			out->words[0] = 0;
			out->words[1] = 0;
		}
		out->words[2] = (aafUInt16)((in >> 16L) & 0xFFFF);
		out->words[3] = (aafUInt16)(in & 0xFFFF);
		return(AAFRESULT_SUCCESS);
	}
	else
		return(AAFRESULT_NULL_PARAM);
}

aafErr_t CvtUInt32toInt64(
			aafUInt32		in,		/* IN -- Convert this aafInt32 */
			aafInt64		*out)	/* OUT -- into this aafInt64 */
{
	if(out != NULL)
	{
		out->words[0] = 0;
		out->words[1] = 0;
		out->words[2] = (aafUInt16)((in >> 16L) & 0xFFFF);
		out->words[3] = (aafUInt16)(in & 0xFFFF);
		return(AAFRESULT_SUCCESS);
	}
	else
		return(AAFRESULT_NULL_PARAM);
}
#endif

/************************
 * Function: AddInt32toInt64
 *
 * 		Adds a aafUInt32 value to a aafInt64 value.
 *
 * Argument Notes:
 *		That the second parameter is overwritten with the result.
 *
 * ReturnValue:
 *		Error code (see below).  The error code in the opaque handle
 *		is not updated.
 *
 * Possible Errors:
 *		AAFRESULT_OVERFLOW64 - Arithmetic overflow.
 * 		AAFRESULT_NULL_PARAM -- A NULL result pointer.
 */
aafErr_t AddInt32toInt64(
			aafUInt32	in,			/* IN - Add this aafUInt32 to */
			aafInt64	*out)		/* IN/OUT - this existing aafInt64 */
{
#if !PORT_USE_NATIVE64
	aafInt64          src;
#endif

	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);
		
#if PORT_USE_NATIVE64
	*out += in;
	return(AAFRESULT_SUCCESS);
#else
	CvtInt32toInt64(in, &src);
	return (AddInt64toInt64(src, out));
#endif
}

/************************
 * Function: AddInt64toInt64
 *
 * 		Adds a aafInt64 value to a aafInt64 value.
 *
 * Argument Notes:
 *		That the second parameter is overwritten with the result.
 *
 * ReturnValue:
 *		Error code (see below).  The error code in the opaque handle
 *		is not updated.
 *
 * Possible Errors:
 *		AAFRESULT_OVERFLOW64 - Arithmetic overflow.
 * 		AAFRESULT_NULL_PARAM -- A NULL result pointer.
 */
aafErr_t AddInt64toInt64(
			aafInt64	in,		/* IN - Add this aafInt64 to */
			aafInt64	*out)		/* IN/OUT - this existing aafInt64 */
{
#if !PORT_USE_NATIVE64
	aafUInt32          temp, carry;
	aafInt32			n, inSign, in2Sign, outSign;
#endif

	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);

#if PORT_USE_NATIVE64
	*out += in;
	return(AAFRESULT_SUCCESS);
#else
	inSign = in.words[0] & 0x8000;
	in2Sign = out->words[0] & 0x8000;
	for (n = 3, carry = 0; n >= 0; n--)
	{
		temp = (aafUInt32)in.words[n] + (aafUInt32)out->words[n] + carry;
		out->words[n] = (aafUInt16)(temp & 0xFFFF);
		carry = (temp >> 16L) & 0xFFFF;
	}
	outSign = out->words[0] & 0x8000;

	if((inSign == in2Sign) && (outSign != in2Sign))
		return (AAFRESULT_OVERFLOW64);
	else
		return (AAFRESULT_SUCCESS);
#endif
}

/************************
 * Function: SubInt64fromInt64
 *
 * 		Subtracts a aafInt64 value (in) from an existing aafInt64 value
 *		(out), returning the result in "out".
 *
 * Argument Notes:
 *		The second parameter is overwritten with the result.
 *
 * ReturnValue:
 *		Error code (see below).  The error code in the opaque handle
 *		is not updated.
 *
 * Possible Errors:
 *		AAFRESULT_INTERNAL_NEG64 - Result would be negative.
 * 		AAFRESULT_NULL_PARAM -- A NULL result pointer.
 */
aafErr_t SubInt64fromInt64(
			aafInt64	in,		/* IN - Subtract this aafInt64 from */
			aafInt64 *out)		/* IN/OUT - this existing aafInt64 */
{
#if !PORT_USE_NATIVE64
	aafInt32		temp;
	aafInt32		borrow;
	aafInt16		j;
	aafInt32		inSign, in2Sign, outSign;
#endif

	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);

#if PORT_USE_NATIVE64
	*out -= in;
	return(AAFRESULT_SUCCESS);
#else
	inSign = in.words[0] & 0x8000;
	in2Sign = out->words[0] & 0x8000;
	for (j = 3, borrow = 0; j >= 0; j--)
	{
		temp = ((aafInt32)out->words[j] - (aafInt32)in.words[j]) - borrow;
		if (temp < 0)
			borrow = 1;
		else
			borrow = 0;
		out->words[j] = temp & 0xFFFF;
	}

	outSign = out->words[0] & 0x8000;
	if((inSign != in2Sign) && (outSign != in2Sign))
		return (AAFRESULT_OVERFLOW64);
	else
		return (AAFRESULT_SUCCESS);
#endif
}

aafErr_t SubInt32fromInt64(
			aafInt32	in,		/* IN - Subtract this aafInt64 from */
			aafInt64 *out)		/* IN/OUT - this existing aafInt64 */
{
#if PORT_USE_NATIVE64
	*out -= in;
	return(AAFRESULT_SUCCESS);
#else
	aafInt64		largeIn;

	CvtInt32toInt64(in, &largeIn);
	return(SubInt64fromInt64(largeIn, out));
#endif
}


static void NegateInt64(aafInt64 *out)
{
#if PORT_USE_NATIVE64
	*out = -1 * (*out);
#else
	out->words[0] = ~out->words[0];
	out->words[1] = ~out->words[1];
	out->words[2] = ~out->words[2];
	out->words[3] = ~out->words[3];
	AddInt32toInt64(1, out);
#endif
}

/************************
 * Function: MultInt32byInt64
 *
 * 		Multiplies "in" * "in2", and puts the result in the variable
 *		pointed to by "out".
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).  The error code in the opaque handle
 *		is not updated.
 *
 * Possible Errors:
 *		AAFRESULT_OVERFLOW64 - Arithmetic overflow.
 * 		AAFRESULT_NULL_PARAM -- A NULL result pointer.
 */
aafErr_t MultInt32byInt64(
			aafInt32	in,	/* IN - Multiply this aafInt32 */
			aafInt64		in2,	/* IN - by this aafInt64 */
			aafInt64		*out)	/* OUT - and put the result here. */
{
#if !PORT_USE_NATIVE64
	aafInt32           i, j, temp, carry, n;
	aafInt64          in1;
	aafUInt16			result[8];		/* Must be twice as large as either input */
	aafInt32			inSign, in2Sign;
#endif
	
	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);

#if PORT_USE_NATIVE64
	*out = ((long)in) * in2;
	return(AAFRESULT_SUCCESS);
#else
	CvtInt32toInt64(in, &in1);
	inSign = in1.words[0] & 0x8000;
	in2Sign = in2.words[0] & 0x8000;
	if(inSign != 0)
		NegateInt64(&in1);
	if(in2Sign != 0)
		NegateInt64(&in2);

	for(n = 0; n < sizeof(result)/sizeof(aafUInt16); n++)
		result[n] = 0;
		
	for (j = 3; j >= 0; j--)
	{
		if (in2.words[j] == 0)
			result[j] = 0;
		else
		{
			for (i = 3, carry = 0; i >= 0; i--)
			{
				/* i+j+1 is because we want i+j from a 1-based array
				 * +1 each for i & j to convert to 1-based array, 
				 * -1 to convert back to zero-based array
				 */
				 temp = (aafInt32)in1.words[i] * (aafInt32)in2.words[j] +
						result[i+j+1] + carry;
				result[i+j+1] = temp & 0xFFFF;
				carry = (temp >> 16L) & 0xFFFF;
			}
			result[j] = (aafUInt16)carry;
		}
	}

	for(n = 4; n < sizeof(result)/sizeof(aafUInt16); n++)
		out->words[n-4] = result[n];
	if(inSign != in2Sign)
	{
		NegateInt64(out);
	}
	
	if(result[0] == 0 && result[1] == 0 && result[2] == 0 && result[3] == 0)
		return(AAFRESULT_SUCCESS);
	else
		return(AAFRESULT_OVERFLOW64);
#endif
}

#if !PORT_USE_NATIVE64
static aafErr_t shiftRight(aafInt64 *in)
{
	aafInt32	carry0, carry1, carry2;
	
	carry0 = in->words[0] & 0x01;
	carry1 = in->words[1] & 0x01;
	carry2 = in->words[2] & 0x01;
	in->words[0] >>= 1;
	in->words[1] = (aafUInt16)((in->words[1] >> 1) | (carry0 << 15L));
	in->words[2] = (aafUInt16)((in->words[2] >> 1) | (carry1 << 15L));
	in->words[3] = (aafUInt16)((in->words[3] >> 1) | (carry2 << 15L));
	return(AAFRESULT_SUCCESS);
}
#endif

aafErr_t DivideInt64byInt32(
		       aafInt64		in, 		/* IN - Divide this aafInt64 */
		       aafUInt32	in2,		/* IN - by this aafInt32 */
		       aafInt64		*result,	/* OUT - and put the result here. */
		       aafInt32		*remainder)	/* OUT - and put the remainder here. */
{
#if !PORT_USE_NATIVE64
	aafInt64	num64, den64, localResult, tmp, tmp2;
	aafInt32	j, num32, den, test;
	aafInt32	inSign;
#endif
	
	XPROTECT()
	{
		XASSERT(in2 != 0, AAFRESULT_ZERO_DIVIDE);
#if PORT_USE_NATIVE64
		if(result != NULL)
			*result = in / ((long)in2);
		if(remainder != NULL)
			*remainder = (aafInt32)(in % ((long)in2));
#else
		if(result == NULL)
			result = &localResult;

		DecomposeInt64(in, &test, &num32);
		if(test == 0)	/* fits in 32-bit number */
		{
			CvtInt32toInt64((aafUInt32)num32 / in2, result);
			if(remainder != NULL)
			  *remainder = (aafUInt32)num32 % in2;
		}
		else
		{
			inSign = in.words[0] & 0x8000;
			if(inSign != 0)
				NegateInt64(&in);

			CvtInt32toInt64(0, result);
		
			for (j = 0, den = in2; (den & 0x80000000) == 0; j++)
				den <<= 1;
			CHECK(MakeInt64(den, 0, &den64));
			j += 32;
			num64 = in;
			
			do
			{
				XASSERT (j >= 0, AAFRESULT_INTERNAL_DIVIDE);
				AddInt64toInt64(*result, result);
				if (Int64GreaterEqual(num64, den64))
				{
					CHECK(AddInt32toInt64(1, result));
					CHECK(SubInt64fromInt64(den64, &num64));
					XASSERT(Int64Less(num64, den64), AAFRESULT_INTERNAL_DIVIDE);
				}
				CHECK(shiftRight(&den64));
			}
			while (j--);
	
			if(remainder != NULL)
			{
				CHECK(MultInt32byInt64(in2, *result, &tmp));
				tmp2 = in;
				CHECK(SubInt64fromInt64(tmp, &tmp2));
				CHECK(TruncInt64toInt32(tmp2, remainder));	/* OK (remainder must be < divisor) */
			}
			if(inSign != 0)
			{
				NegateInt64(result);
			}
		}
#endif
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * Function: TruncInt64toInt32/TruncInt64toUInt32	OK
 *
 * 	Truncate a 64-bit integer to a 32-bit value, and tell the
 *		caller whether siginifcant digits were truncated
 *		(TRUE == no significant truncation).
 *
 *		Some formats have 32-bit limits on offsets and lengths, and
 *		so would use this to truncate 64-bit offsets and lengths.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).  The error code in the opaque handle
 *		is not updated.
 *
 * Possible Errors:
 *		AAFRESULT_OFFSET_SIZE -- Truncation occurred.
 * 		AAFRESULT_NULL_PARAM -- A NULL result pointer.
 */
aafErr_t TruncInt64toInt32(				/* OK */
			aafInt64		in,		/* IN - Truncate this value */
			aafInt32		*out)		/* OUT - to 32-bits and put the result here */
{
	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);

#if PORT_USE_NATIVE64
	*out = (aafInt32)(in & 0xFFFFFFFF);
	if(*out != in)
	{
		if((aafInt32)(-in & 0xFFFFFFFF) != *out)
			return (AAFRESULT_OFFSET_SIZE);
	}
	return(AAFRESULT_SUCCESS);
#else
	if ((in.words[0] == 0) && (in.words[1] == 0) && ((aafInt16)in.words[2] >= 0))
	{
		*out = ((aafInt32) (in.words[2] & 0x7FFF) << 16L) | (in.words[3]);
		return (AAFRESULT_SUCCESS);
	}
	else if ((in.words[0] == 0xFFFF) && (in.words[1] == 0xFFFF) && ((aafInt16)in.words[2] <= 0))
	{
		*out = ((aafInt32) in.words[2] << 16L) | (in.words[3]);
		return (AAFRESULT_SUCCESS);
	}
	else
		return (AAFRESULT_OFFSET_SIZE);
#endif
}

aafErr_t TruncInt64toUInt32(				/* OK */
			aafInt64		in,		/* IN - Truncate this value */
			aafUInt32	*out)		/* OUT - to 32-bits and put the result here */
{
	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);

#if PORT_USE_NATIVE64
	*out = (aafUInt32)(in & 0xFFFFFFFF);
	if(*out != in)
		return (AAFRESULT_OFFSET_SIZE);
	return(AAFRESULT_SUCCESS);
#else
	if ((in.words[0] == 0) && (in.words[1] == 0))
	{
		*out = ((aafUInt32) in.words[2] << 16L) | (in.words[3]);
		return (AAFRESULT_SUCCESS);
	} else
		return (AAFRESULT_OFFSET_SIZE);
#endif
}

aafErr_t Int64AndInt64(
			aafInt64	in,		/* IN - Subtract this aafInt64 from */
			aafInt64 *out)		/* IN/OUT - this existing aafInt64 */
{
#if !PORT_USE_NATIVE64
	aafInt16		j;
#endif

	if(out == NULL)
		return(AAFRESULT_NULL_PARAM);

#if PORT_USE_NATIVE64
	*out &= in;
	return(AAFRESULT_SUCCESS);
#else
	for (j = 3; j >= 0; j--)
	{
		out->words[j] &= in.words[j];
	}
	return (AAFRESULT_SUCCESS);
#endif
}

/************************
 * Function: Int64ToString
 *
 * 		64-bit to string conversions.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		AAFRESULT_NULL_PARAM - Buffer pointer is NULL.
 *		AAFRESULT_NOT_IMPLEMENTED - Currently only handles bases 10 & 16.
 */
aafErr_t Int64ToString(
			aafInt64	in,			/* IN - Print this number */
			aafInt16	base,		/* IN - in this numerical base */
			aafInt16	bufSize,	/* IN - into a buffer of this length */
			char 		*buf)		/* IN/OUT - here is the buffer. */
{
	aafInt64		workval, zero;
	char			tmpBuf[64];
	aafInt32		numDigits, remainder, src, dest;
	aafBool			negative = kAAFFalse;
	
	if(buf == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CvtInt32toInt64(0, &zero);
		numDigits = 0;
		workval = in;
		if(Int64Equal(workval, zero))
		{
			buf[0] = '0';
			buf[1] = '\0';
		}
		else
		{
			if(Int64Less(workval, zero))
			{
				NegateInt64(&workval);
				negative = kAAFTrue;
			}
			while(Int64Greater(workval, zero))
			{
				CHECK(DivideInt64byInt32(workval, base, &workval, &remainder));
				tmpBuf[numDigits++] = remainder + '0';
			}
			if(negative)
				tmpBuf[numDigits++] = '-';
			if(numDigits < bufSize)		/* Allow one byte for '\0' */
			{
				for(src = numDigits-1, dest = 0; src >= 0; src--, dest++)
					buf[dest] = tmpBuf[src];
				buf[numDigits] = '\0';
			}
			else
			{
				strncpy(buf, "*err*", bufSize-1);
				buf[bufSize-1] = '\0';
			}
		}
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * Function:Int64Less
 * Function: Int64Greater
 * Function: Int64LessEqual
 * Function: Int64GreaterEqual
 * Function: Int64Equal
 * Function: Int64NotEqual
 *
 * 		64-bit comparison tests.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Boolean TRUE if the comparison is true (ie: Int64Less(val2, val3)).
 *
 * Possible Errors:
 *		<none>
 */
#if !PORT_USE_NATIVE64
aafInt16 compare64(		/* Returns -1 if a<b, 0 if a == b, 1 if a > b) */
			aafInt64	a,
			aafInt64 b)
{
	aafInt16	n, signA, signB;
	aafInt32	extendA, extendB;
	
	signA = ((aafInt16)a.words[0] < 0 ? -1 : 1);
	signB = ((aafInt16)b.words[0] < 0 ? -1 : 1);
	if(signA < signB)
		return(-1);
	if(signB > signA)
		return(1);
	for (n = 0; n <= 3; n++)
	{
		extendA = a.words[n];
		extendB = b.words[n];
		if(extendA < extendB)
			return(-1);
		else if(extendA > extendB)
			return(1);
	}
	return(0);
}

/*****/
aafBool Int64Equal(
			aafInt64 a,		/* IN - Is a == b */
			aafInt64 b)
{
	aafInt16	n;

	for(n = 0; n <= 3; n++)
	{
		if(a.words[n] != b.words[n])
			return kAAFFalse;
	}
	return(kAAFTrue);
}

/*****/
aafBool Int64NotEqual(
			aafInt64 a,		/* IN - Is a != b */
			aafInt64 b)
{
	return((aafBool)!Int64Equal(a, b));
}
#endif

#ifdef AAF_SELF_TEST
/************************
 * Function: testUint64
 *
 * 		WhatItDoes
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
#include <stdlib.h>
#if PORT_USE_NATIVE64
	static aafInt64 test4 = 	0x4444444444444444;
	static aafInt64 test3a =	0x4000000000000000;
	static aafInt64 test3b =	0x0CCCCCCCCCCCCCCD;
	static aafInt64 test3 =		0x3333333333333333;
	static aafInt64 test2 =		0x2222222222222222;
	static aafInt64 test1 =		0x1111111111111111;
	static aafInt64 testF =		0x7FFFFFFFFFFFFFFF;
	static aafInt64 test0 =		0x0000000000000000;
	static aafInt64 testNeg1 =	0xFFFFFFFFFFFFFFFF;
	static aafInt64 testNeg2 =	0xFFFFFFFFFFFFFFFE;
	static aafInt64 test1p1 = 	0x1111111111111112;
	static aafInt64 test1m1 =	0x1111111111111110;
	static aafInt64 testRoll =	0x0000000080000000;
	static aafInt64 testRoll2 =	0x0000000100000000;
	static aafInt64 test1Neg =	0xEEEEEEEEEEEEEEEF;
	static aafInt64 test2Neg =	0xDDDDDDDDDDDDDDDE;
	static aafInt64 testJust31 = 0x000000000000001F;
#else
	static aafInt64 test4 = { 0x4444, 0x4444, 0x4444, 0x4444 };
	static aafInt64 test3a = { 0x4000, 0x0000, 0x0000, 0x0000 };
	static aafInt64 test3b = { 0x0CCC, 0xCCCC, 0xCCCC, 0xCCCD };
	static aafInt64 test3 = { 0x3333, 0x3333, 0x3333, 0x3333 };
	static aafInt64 test2 = { 0x2222, 0x2222, 0x2222, 0x2222 };
	static aafInt64 test1 = { 0x1111, 0x1111, 0x1111, 0x1111 };
	static aafInt64 testF = { 0x7FFF, 0xFFFF, 0xFFFF, 0xFFFF };
	static aafInt64 test0 = { 0x0000, 0x0000, 0x0000, 0x0000 };
	static aafInt64 testNeg1 = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
	static aafInt64 testNeg2 = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFE };
	static aafInt64 test1p1 = { 0x1111, 0x1111, 0x1111, 0x1112 };
	static aafInt64 test1m1 = { 0x1111, 0x1111, 0x1111, 0x1110 };
	static aafInt64 testRoll = { 0x0000, 0x0000, 0x8000, 0x0000 };
	static aafInt64 testRoll2 = { 0x0000, 0x0001, 0x0000, 0x0000 };
	static aafInt64 test1Neg = { 0xEEEE, 0xEEEE, 0xEEEE, 0xEEEF };
	static aafInt64 test2Neg = { 0xDDDD, 0xDDDD, 0xDDDD, 0xDDDE };
	static aafInt64 testJust31 = { 0x0000, 0x0000, 0x0000, 0x001F };
#endif
		
#define testErr(msg) { printf(msg); exit(1); }
	
void testUint64(void)
{
	aafInt64	tmp, result, testPrint;
	aafInt32	remainder;
	char		buf[16];
	
	/********************************************/
	/***** Check some simple add conditions *****/
	/* Both #'s positive */
	tmp = test1;
	if(AddInt64toInt64(test2, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed add (error).\n");
	if(Int64NotEqual(tmp, test3))
		testErr("failed add value.\n");

	/* Positive + negative */
	tmp = test1;
	if(AddInt64toInt64(testNeg1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed add positive + negative (error).\n");
	if(Int64NotEqual(tmp, test1m1))
		testErr("failed add positive + negative value.\n");

	/* Negative + negative */
	tmp = testNeg1;
	if(AddInt64toInt64(testNeg1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed add negative + negative (error).\n");
	if(Int64NotEqual(tmp, testNeg2))
		testErr("failed add negative + negative value.\n");

	/* Check for expected overflow */
/* !!!	tmp = test1;
	if(AddInt64toInt64(testF, &tmp) != AAFRESULT_OVERFLOW64)
		testErr("missing expected overflow.\n");
*/	
	/*************************************************/
	/***** Check some simple subtract conditions *****/
	/* (pos - pos = pos) */
	tmp = test4;
	if(SubInt64fromInt64(test3, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed subtract (pos - pos = pos) (error).\n");
	if(Int64NotEqual(tmp, test1))
		testErr("failed subtract (pos - pos = pos) value.\n");

	/* (pos - pos = neg) */
	tmp = test1;
	if(SubInt64fromInt64(test1p1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed subtract (pos - pos = neg) (error).\n");
	if(Int64NotEqual(tmp, testNeg1))
		testErr("failed subtract (pos - pos = neg) value.\n");

	/* (pos - neg = pos) */
	tmp = test1;
	if(SubInt64fromInt64(testNeg1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed subtract (pos - neg = pos) (error).\n");
	if(Int64NotEqual(tmp, test1p1))
		testErr("failed subtract (pos - neg = pos) value.\n");

	/* (pos - pos = zero) */
	tmp = test1;
	if(SubInt64fromInt64(test1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed subtract (error).\n");
	if(Int64NotEqual(tmp, test0))
		testErr("failed subtract value.\n");

	/* (neg - neg = zero) */
	tmp = testNeg1;
	if(SubInt64fromInt64(testNeg1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed subtract (neg - neg = zero) (error).\n");
	if(Int64NotEqual(tmp, test0))
		testErr("failed subtract (neg - neg = zero) value.\n");
	
	/* Check for borrow without error */
	tmp = test3a;
	if(SubInt64fromInt64(test3, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed borrow without error (error).\n");
	if(Int64NotEqual(tmp, test3b))
		testErr("failed borrow without error (value).\n");
			
	/********************************************/
	/* Check some simple multiply conditions */
	/* (pos * pos = pos) */
	if(MultInt32byInt64(2, test2, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed simple multiply (error).\n");
	if(Int64NotEqual(tmp, test4))
		testErr("failed simple multiply (value).\n");

	/* (pos * neg = neg) */
	if(MultInt32byInt64(2, testNeg1, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed (pos * neg = neg) multiply (error).\n");
	if(Int64NotEqual(tmp, testNeg2))
		testErr("failed (pos * neg = neg) multiply (value).\n");

	/* (neg * pos = neg) */
	if(MultInt32byInt64(-1, test2, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed (neg * pos = neg) multiply (error).\n");
	if(Int64NotEqual(tmp, test2Neg))
		testErr("failed (neg * pos = neg) multiply (value).\n");
	
	/* (neg * neg = pos) */
	if(MultInt32byInt64(-2, test2Neg, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed (neg * neg = pos) multiply (error).\n");
	if(Int64NotEqual(tmp, test4))
		testErr("failed (neg * neg = pos) multiply (value).\n");
	
	/* Test 31-bit rollover */
	if(MultInt32byInt64(2, testRoll, &tmp) != AAFRESULT_SUCCESS)
		testErr("failed multiply 31 bit rollover (error).\n");
	if(Int64NotEqual(tmp, testRoll2))
		testErr("failed multiply 31 bit rollover (value).\n");

	/********************************************/
	/* Check some simple divide conditions */
	/* (pos / pos = pos) */
	if(DivideInt64byInt32(test4, 2, &result, &remainder))
		testErr("failed (pos / pos = pos) divide (error).\n");
	if(Int64NotEqual(result, test2) || (remainder != 0))
		testErr("failed (pos / pos = pos) divide (value).\n");

	/* (neg / pos = neg) */
	if(DivideInt64byInt32(test2Neg, 2, &result, &remainder))
		testErr("failed (neg / pos = neg) divide (error).\n");
	if(Int64NotEqual(result, test1Neg) || (remainder != 0))
		testErr("failed (neg / pos = neg) divide (value).\n");

	/* Positive remainder */
	if(DivideInt64byInt32(test1, 32, &result, &remainder))
		testErr("failed (neg / neg = pos) divide (error).\n");
	if(remainder != 17)
		testErr("failed (neg / neg = pos) divide (value).\n");
		
	CvtInt32toInt64(0x12345678, &testPrint);			
	if(Int64ToString(testPrint, 16, sizeof(buf), buf))
		testErr("failed base 16 print (error).\n");
	printf("Printing 0x12345678 in hexadecimal = %s\n", buf);
	if(strcmp(buf, "12345678") != 0)
		testErr("failed base 16 print (value).\n");
	if(Int64ToString(testPrint, 10, sizeof(buf), buf))
		testErr("failed base 16 print).\n");
	printf("Printing 0x12345678 in decimal (should be 305419896) = %s\n", buf);
	if(strcmp(buf, "305419896") != 0)
		testErr("failed base 16 print).\n");
}

#endif


/*************************************************************************
 * Function: TimecodeToString()/StringToTimecode()
 *
 *      TimecodeToString converts a timecode value to a string
 *      representation of the timecode.  StringToTimecode converts 
 *      a string representation of timecode to a timecode value.
 *      Drop and non-drop are distinguished with the delimiters in
 *      the string: drop uses a ';' and non-drop uses a ':'.
 *      These functions do not take a file handle as an argument.
 *
 *      These function support both 1.x and 2.x files.
 *
 * Argument Notes:
 *      TimecodeToString() assumes that an already allocated character
 *      array is passed in as tcString.  The length of the array is
 *      in strLen.  The function will return an error (AAFRESULT_INTERN_TOO_SMALL)
 *      if strLen is too small to hold the string representation of the
 *      timecode.
 *
 *      StringToTimecode() also takes a frameRate argument.  It will
 *      calculate the given timecode taking the rate into account.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
aafErr_t TimecodeToString(
	aafTimecode_t timeCode,   /* IN - Timecode Value */
	aafInt32 strLen,          /* IN - Length of string to hold timecode */
	wchar_t *tcString)       /* IN/OUT - Pre-allocated buffer to hold
							   *          timecode string
							   */
{
	register int i, ten;
  aafInt16 hours, minutes, seconds, frames;

  XPROTECT()
	{
	  if ((tcString == NULL) || (strLen < 12))
		{
		  RAISE(AAFRESULT_INTERN_TOO_SMALL);
		}

	  if (timeCode.drop == kAAFTcDrop)
		wcscpy(tcString, L"00;00;00;00");
	  else 
		wcscpy(tcString, L"00:00:00:00");

	  CHECK(PvtOffsetToTimecode(timeCode.startFrame, timeCode.fps, timeCode.drop, &hours, 
							  &minutes, &seconds, &frames));

	  ten = 10;
	  i = hours;
	  if (i > 0)
		{
		  tcString[0] += i/ten;
		  tcString[1] += i%ten;
		}
	  i = minutes;
	  if (i > 0)
		{
		  tcString[3] += i/ten;
		  tcString[4] += i%ten;
		}
	  i = seconds;
	  if (i > 0)
		{
		  tcString[6] += i/ten;
		  tcString[7] += i%ten;
		}
	  i = frames;
	  if (i > 0)
		{
		  tcString[9] += i/ten;
		  tcString[10] += i%ten;
		}
	} /* XPROTECT */

  XEXCEPT
	{
	}
  XEND;

  return(AAFRESULT_SUCCESS);
}

/*************************************************************************
 * Private Function: roundFrameRate()
 *************************************************************************/
static aafInt32 roundFrameRate(aafRational_t frameRate)
{
	aafInt32 intRate;
  double tmpFloatRate, floatRate;

  tmpFloatRate = FloatFromRational(frameRate);
  floatRate = (float)ceil(tmpFloatRate);  /* To make 29.97 into 30 */

  if (floatRate == 30.0)
	intRate = 30;
  else if (floatRate == 29.97) /* For timecode, 29.97 should be 30 */
	intRate = 30;
  else if (floatRate == 25.0)
	intRate = 25;
  else if (floatRate == 24.0)
	intRate = 24;
  else
	intRate = 0;
  return(intRate);
}

/*************************************************************************
 * Function: StringToTimecode()
 *************************************************************************/
aafErr_t StringToTimecode(
	const wchar_t *timecodeString, /* IN - Timecode String */
	aafRational_t frameRate,  /* IN - Frame Rate */
	aafTimecode_t *timecode)  /* OUT - Timecode Value */
{
	char *c;
	aafInt32 *multiplier, total = 0;
	aafDropType_t drop;
	aafInt32 len, k = 0;
	aafInt32 intFRate;
	char tcString[36];
	
	XPROTECT()
	{
		len = wcslen(timecodeString);
		
		if (len == 0 || len > 12)
		{
			RAISE(AAFRESULT_INVALID_TIMECODE);
		}
		
		wcstombs(tcString, timecodeString, sizeof(tcString));
		intFRate = roundFrameRate(frameRate);
		if (intFRate == 0)
		{
			RAISE(AAFRESULT_INVALID_TIMECODE);
		}
		
		/* Prescan for drop/nondrop */
		drop = kAAFTcNonDrop;
		for (c = &tcString[len-1]; c >= tcString; c--)
			if (*c == ';')
			{
				drop = kAAFTcDrop;
				break;
			}
			
			multiplier = (drop ? dropTbl : nondropTbl);
			if (intFRate == 25)
			{
				multiplier = PALnondropTbl;
				drop = kAAFTcNonDrop;
			}
			
			for (c = &tcString[len-1]; c >= tcString; c--)
			{
				if (isdigit(*c))
				{
				/* If start of a minute which is not a multiple of 10, and frames
				* are 0 or 1, then the user typed a bad, move forward
					*/
					if ((k == 4) && (*c != '0') && (total >= 0) && (total <= 1) 
						&& drop)
					{
						if (total == 0)
							total += 2;
						else if (total == 1)
							total++;
					}
					total += atoi(c) * multiplier[k];
					*c = '\0';
					k++;
				}
				else if (*c != ' ' && *c != ':' && *c != '.' && *c != ';' && *c != '+')
				{
					total = 0;		/* error condition */
					RAISE(AAFRESULT_INVALID_TIMECODE);
				}
			}
			
			if (!drop)
				(*timecode).drop = kAAFTcNonDrop;
			else 
				(*timecode).drop = kAAFTcDrop;
			
			(*timecode).fps = (aafUInt16)intFRate;
			(*timecode).startFrame = total;
	} /* XPROTECT */
	
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
