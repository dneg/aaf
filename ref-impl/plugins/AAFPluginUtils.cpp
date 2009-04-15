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
// The Original Code of this file is Copyright 2008-2009, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFPluginUtils.h"

#include <assert.h>
#include <time.h>




AAFByteOrder GetNativeByteOrder(void)
{
  aafInt16 word = 0x1234;
  aafInt8  byte = *((aafInt8*)&word);
  AAFByteOrder result;

//  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));

  if (byte == 0x12) {
    result = MOTOROLA_ORDER;
  } else {
    result = INTEL_ORDER;
  }
  return result;
}


aafBool	EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? kAAFTrue : kAAFFalse);
}

/************************
 *   Function:	aafIsEqualGUID
 *
 *	Determines whether specified GUIDs are equal.
 *
 *   ReturnValue:
 *	kAAFTrue - The GUIDs are equal.
 *	kAAFFalse - The GUIDs are not equal.
 */
aafBool aafIsEqualGUID( const GUID& guid1, const GUID& guid2 )
{
    return( memcmp( (void*)&guid1, (void*)&guid2, sizeof(GUID) ) == 0 ? 
        kAAFTrue : kAAFFalse );
}



/************************
 * Function: AAFByteSwap16		(INTERNAL)
 *
 * 	Byte swap a short value to convert between big-endian and
 *		little-endian formats.
 *
 * Argument Notes:
 *		See argument comments.
 *
 * ReturnValue:
 *		Modifies the value in place
 *
 * Possible Errors:
 *		none
 */
void AAFByteSwap16(
			aafInt16 * wp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) wp;
	int             t;

	t = cp[1];
	cp[1] = cp[0];
	cp[0] = t;
}

/************************
 * Function: AAFByteSwap32		(INTERNAL)
 *
 * 	Byte swap a long value to convert between big-endian and
 *		little-endian formats.
 *
 * Argument Notes:
 *		See argument comments.
 *
 * ReturnValue:
 *		Modifies the value in place
 *
 * Possible Errors:
 *		none
 */
void AAFByteSwap32(
			aafInt32 *lp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) lp;
	int             t;

	t = cp[3];
	cp[3] = cp[0];
	cp[0] = t;
	t = cp[2];
	cp[2] = cp[1];
	cp[1] = t;
}

/************************
 * Function: AAFByteSwap64		(INTERNAL)
 *
 * 	Byte swap a long value to convert between big-endian and
 *		little-endian formats.
 *
 * Argument Notes:
 *		See argument comments.
 *
 * ReturnValue:
 *		Modifies the value in place
 *
 * Possible Errors:
 *		none
 */
void AAFByteSwap64(
			aafInt64 *lp)	/* IN/OUT -- Byte swap this value */
{
	register unsigned char *cp = (unsigned char *) lp;
	int             t;

	t = cp[7];
	cp[7] = cp[0];
	cp[0] = t;
	t = cp[6];
	cp[6] = cp[1];
	cp[1] = t;
	t = cp[5];
	cp[5] = cp[2];
	cp[2] = t;
	t = cp[4];
	cp[4] = cp[3];
	cp[3] = t;
}


/************************
 * Function: powi		(INTERNAL)
 *
 * 	Return base ^ exponent.
 *
 * Argument Notes:
 * 	<none>
 *
 * ReturnValue:
 *		The result.
 *
 * Possible Errors:
 *		<none>.
 */
static aafInt32 powi(
			aafInt32	base,
			aafInt32	exponent)
{
	aafInt32           result = 1;
	aafInt32           i = exponent;

	if (exponent == 0)
		return (1);
	else if (exponent > 0)
	{
		while (i--)
			result *= base;
		return (result);
	}
	else
	{
		/* negative exponent not good for integer
		 * exponentiation
		 */
		return (0);
	}
}

/************************
 * Function: FloatFromRational (INTERNAL)
 *
 * 	Convert a rational number into a floating point double
 *
 * Argument Notes:
 *		See argument comments and assertions.
 *
 * ReturnValue:
 *		The result, or zero if the denominator was zero.
 *
 * Possible Errors:
 *		Returns zero if the denominator was zero.
 */
double FloatFromRational(
			aafRational_t	e)		/* IN - Convert this into a double */
{
	double          num = (double) e.numerator;
	double          den = (double) e.denominator;

	if(den == 0)
		return(0);
	else
		return (num / den);
}

/************************
 * Function: RationalFromFloat (INTERNAL)
 *
 * 	Translate a floating point number into the nearest rational
 *		number.  Used when rates must be stored as floats.
 *
 * Argument Notes:
 *		See argument comments and assertions.
 *
 * ReturnValue:
 *		A rational number
 *
 * Possible Errors:
 */
aafRational_t RationalFromFloat( //DUPLICATED in AAFUtils.h
			double	f)		/* IN - Convert this number into a rational */
{
	/* chintzy conversion of old code  */
	aafRational_t   rate;
	double          rem;
	aafInt32           i;

	/* special check for NTSC video */
	if (fabs(f - 29.97) < .01)
	{
		rate.numerator = 2997;
		rate.denominator = 100;
		return (rate);
	}
	/* need to normalize to make compare work */
	rem = f - floor(f);
	for (i = 0; (i < 4) && (rem > .001); i++)
	{
		f = f * 10;
		rem = f - floor(f);
	}
	rate.numerator = (aafInt32) f;
	rate.denominator = powi(10L, i);
	return (rate);
}

/************************
 * Function: AAFGetDateTime			(INTERNAL)
 *
 * 	Returns the number of seconds since the standard root date
 *		for the current machine.  The date returned here will be 
 *		converted the canonical date format in the date write routine.
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
void AAFGetDateTime(aafTimeStamp_t *ts)
{
	assert (ts);

	const time_t t = time(0);
	const struct tm * ansitime = gmtime (&t);
	assert (ansitime);

	ts->date.year   = ansitime->tm_year+1900;
	ts->date.month  = ansitime->tm_mon+1;  // AAF months are 1-based
	ts->date.day    = ansitime->tm_mday;   // tm_mday already 1-based
	ts->time.hour   = ansitime->tm_hour;
	ts->time.minute = ansitime->tm_min;
	ts->time.second = ansitime->tm_sec;
	ts->time.fraction = 0;            // not implemented yet!
}
