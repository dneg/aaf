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


/*
 * Name: omAcces.c
 *
 * Function: The main API file for media layer operations.
 *
 * Audience: Clients writing or reading AAF media data.
 *
 * Public Functions:
 *		AAFMalloc()
 *			Allocate this many bytes
 *		AAFFree()
 *			Free up this buffer
 *		IsPropertyPresent()
 *			Test if a given property is present in a file.
 *		IsTypeOf()
 *			Test if an object is a member of the given class, or
 *			one of its subclasses.
 *		omfsClassFindSuperClass()
 *			Given a pointer to a classID, returns the superclass ID
 *			and a boolean indicating if a superclass was found.
 *		GetByteOrder()
 *			Return the byte ordering of a particular object in the file. 
 *		omfsPutByteOrder()
 *			Sets the byte ordering of a particular object in the file.
 *		omfsFileGetDefaultByteOrder()
 *			Returns the default byte ordering from the given file.
 *		omfsFileSetDefaultByteOrder()
 *			Sets the default byte ordering from the given file.
 *		GetHeadObject()
 *			Given an opaque file handle, return the head object for
 *			that file.
 *		omfsObjectNew()
 *			Create a new object of a given classID in the given file.
 *		omfsSetProgressCallback()
 *			Sets a callback which will be called at intervals
 *			during long operations, in order to allow your application
 *			to pin a watch cursor, move a thermometer, etc...
 *    GetNextProperty()
 *    GetPropertyName()
 *    omfiGetPropertyTypename()
 *    GetNextObject()
 *
 * Public functions used to extend the toolkit API:
 *	(Used to write wrapper functions for new types and properties)
 *
 *		NewClass()
 *			Add a new class to the class definition table.
 *		omfsRegisterType()
 *			Add a new type to the type definition table.  This function takes an explicit
 *			omType_t, and should be called for required & registered types
 *			which are not registered by a media codec.
 *		omfsRegisterProp()
 *			Register a aafProperty code, supplying a name, class, type, and an enum
 *			value indicating which revisions the field is valid for.  This function takes
 *			an explicit aafProperty_t, and should be called for required & registered
 *			properties which are not registered by a media codec.
 *		omfsRegisterDynamicType()
 *			Register a type with a dynamic type code.  This function should be called when
 *			registering a private type, or when a codec requires a type.  The type code used
 *			to refer to the type is returned from this function, and must be saved in a variable
 *			in the client application, or in the persistant data block of a codec.
 *		omfsRegisterDynamicProp()
 *			Register a type with a dynamic property code.  This function should be called when
 *			registering a private property, or when a codec requires a property.  The type code used
 *			to refer to the property is returned from this function, and must be saved in a variable
 *			in the client application, or in the persistant data block of a codec.
 *		OMReadProp()
 *			Internal function to read the given property from the file, and apply
 *			semantic error checking to the input parameters and the result.
 *		OMWriteProp()
 *			Internal function to write the given property to the file, and apply
 *			semantic error checking to the input parameters.
 *		OMIsPropertyContiguous()
 *			Tell if a property is contigous in the file.   This function
 *			is usually applied to media data, when the application wishes
 *			to read the data directly, without the toolkit.
 *		GetArrayLength()
 *			Get the number of elements in an arrayed AAF object.
 *		OMPutNthPropHdr()
 *			Set an indexed elements value in an arrayed AAF object.
 *		OMGetNthPropHdr()
 *			Get an indexed elements value from an arrayed AAF object.
 *		OMReadBaseProp()
 *			Version of OMReadProp which byte-swaps when needed and always reads
 *			from an offset of 0.
 *		OMWriteBaseProp()
 *			Version of OMWriteProp which always reads from an offset of 0.
 *		OMLengthProp()
 *			Internal function to find the length of the given property.
 *		GetReferencedObject()
 *		GetReferenceData()
 *		OMRemoveNthArrayProp()
 *		omfsFileGetValueAlignment()
 *		omfsFileSetValueAlignment()
 *
 * All functions can return the following error codes if the following
 * argument values are NULL:
 *		AAFRESULT_NULL_FHDL -- aafHdl_t was NULL.
 *		AAFRESULT_NULLOBJECT -- aafObject_t was NULL.
 *		AAFRESULT_BADDATAADDRESS -- Data address was NULL.
 *		AAFRESULT_NULL_SESSION -- No session was open.
 *		AAFRESULT_SWAB -- Unable to byte swap the given data type.
 * 
 * Accessor functions can also return the error codes below:
 *		AAFRESULT_BAD_PROP -- aafProperty_t code was out of range.
 *		AAFRESULT_OBJECT_SEMANTIC -- Failed a semantic check on an input obj
 *		AAFRESULT_DATA_IN_SEMANTIC -- Failed a semantic check on an input data
 *		AAFRESULT_DATA_OUT_SEMANTIC -- Failed a semantic check on an output data
 */

//#include "masterhd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#if PORT_SYS_MAC
#include <memory.h>		/* For AAFMalloc() and AAFFree() */
#include <OSUtils.h>
#endif
#ifdef _WIN32
#include <time.h>
#elif defined (__sgi) || defined (__FreeBSD__)
  #include <time.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/times.h>
#endif

//#include "omPublic.h"
//#include "omPvt.h" 
//#include "Container.h"
#include "AAFTypes.h"
#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFCOMPlatform.h"

/* Moved math.h down here to make NEXT's compiler happy */
#include <math.h>


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

static aafBool  InitCalled = kAAFFalse;

const aafProductVersion_t AAFReferenceImplementationVersion = {1, 0, 0, 1, kAAFVersionBeta};

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

aafBool	EqualMobID(aafMobID_constref mobID1, aafMobID_constref mobID2)
{
	return(memcmp(&mobID1, &mobID2, sizeof(aafMobID_t)) == 0 ? kAAFTrue : kAAFFalse);
}

static aafInt32 powi(
			aafInt32	base,
			aafInt32	exponent);


	/************************************************************
	 *
	 * Public Functions (Part of the toolkit API)
	 *
	 *************************************************************/



/************************
 * Function: AAFGetDateTime			(INTERNAL)
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

aafErr_t AAFConvertEditRate(
	aafRational_t srcRate,        /* IN - Source Edit Rate */
	aafPosition_t srcPosition,    /* IN - Source Position */
	aafRational_t destRate,       /* IN - Destination Edit Rate */
	aafRounding_t howRound,	      /* IN - Rounding method (floor or ceiling) */
	aafPosition_t *destPosition)  /* OUT - Destination Position */
{
	aafInt64		intPos, destPos;
	aafInt32		remainder;
		
	XPROTECT()
	{
		CvtInt32toInt64(0, destPosition);
		if ((howRound != kRoundCeiling) && (howRound != kRoundFloor))
		{
			RAISE(AAFRESULT_INVALID_ROUNDING);
		}

		if(FloatFromRational(srcRate) != FloatFromRational(destRate))
		{
			CHECK(MultInt32byInt64((srcRate.denominator * destRate.numerator), srcPosition, &intPos));
			CHECK(DivideInt64byInt32(intPos, (srcRate.numerator * destRate.denominator), &destPos, &remainder));
		}
		else
		{
	  		/* JeffB: (1 / 29.97) * 29.97 often doesn't == 1
	  		 * (it seems to be .99999... on the 68K FPU)
	  		 * The debugger says it's 1.0, but if(foo >= 1.0) fails.
	  		 */
	  		destPos = srcPosition;
	  		remainder = 0;
		}

		/* Usually used for lower edit rate to higher edit rate conversion - 
		 * to return the first sample of the higher edit rate that contains a
		 * sample of the lower edit rate. (i.e., video -> audio)
		 */
		if (howRound == kRoundFloor)
		{
			*destPosition = destPos;
		}
		/* Usually used for higher edit rate to lower edit rate conversion -
		 * to return the sample of the lower edit rate that fully contains
		 * the sample of the higher edit rate. (i.e., audio -> video)
		 */
		else if (howRound == kRoundCeiling)
		{
			*destPosition = destPos;
			if(remainder != 0)
				AddInt32toInt64(1, destPosition);
		}
	} /* XPROTECT */

	XEXCEPT
	{
		return(XCODE());
	}
	XEND;

	return(AAFRESULT_SUCCESS);
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
aafRational_t RationalFromFloat(
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

#if defined(_MAC) || defined(macintosh)
#include <OSUtils.h>
#include <events.h>
#elif defined(_WIN32)
#include <time.h>
#define HZ CLK_TCK
#elif defined (__sgi) || defined (__FreeBSD__)
#include <time.h>
#ifdef CLK_TCK
#define HZ CLK_TCK
#else
#define HZ CLOCKS_PER_SEC
#endif
#endif

/*
 * Doug Cooper - 04-04-96
 * Added proper includes for NeXTStep to define HZ:
 */
//#ifdef NEXT
//#include <architecture/ARCH_INCLUDE.h>
//#import ARCH_INCLUDE(bsd/, param.h)
//#endif

//#if defined(PORTKEY_OS_UNIX) || defined(PORTKEY_OS_ULTRIX)
//#if PORT_INC_NEEDS_SYSTIME
//#include <sys/time.h>
//#include <sys/times.h>
//#endif
//#include <sys/param.h>
//#endif
//
//#ifdef sun
//#include <sys/resource.h>
//#endif

/*************************************************************************
 * Function: omfiMobIDNew()
 *
 *      This function can be used to create a new mob ID.  The mob ID
 *      consists of the company specific prefix specified when 
 *      omfsBeginSession() is called.  The major number is the time of day,
 *      and the minor number is the accumulated cpu cycles of the
 *      application.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/

struct SMPTELabel
{
	aafUInt32	MobIDMajor;
	aafUInt16	MobIDMinorLow;
	aafUInt16	MobIDMinorHigh;
	aafUInt8	oid;
	aafUInt8	size;
	aafUInt8	ulcode;
	aafUInt8	SMPTE;
	aafUInt8	Registry;
	aafUInt8	unused;
	aafUInt8	MobIDPrefixLow;
	aafUInt8	MobIDPrefixHigh;
};



struct OMFMobID
{
    aafUInt8			SMPTELabel[12];		// 12-bytes of label prefix
	aafUInt8			length;
    aafUInt8			instanceHigh;
    aafUInt8			instanceMid;
    aafUInt8			instanceLow;
	struct SMPTELabel	material;
};

union MobIDOverlay
{
	aafMobID_t			mobID;
	struct OMFMobID		OMFMobID;
};

AAFRESULT aafMobIDNew(
        aafMobID_t *mobID)     /* OUT - Newly created Mob ID */
{
	aafUInt32	major, minor;
	static aafUInt32 last_part2 = 0;		// Get rid of this!!!
//#ifdef sun
//	struct rusage rusage_struct;
//	int status;
//#endif
	aafTimeStamp_t	timestamp;
	
	AAFGetDateTime(&timestamp);
	// major = (aafUInt32)timestamp.TimeVal;	// Will truncate
	assert (sizeof (aafTimeStruct_t) == sizeof (aafUInt32));
	union
	{
	  aafTimeStruct_t time;
	  aafUInt32       seconds;
	} time_to_int;
	time_to_int.time = timestamp.time;
	major = time_to_int.seconds;

#if defined(_MAC) || defined(macintosh)
	minor = TickCount();
#else
#ifdef _WIN32
	minor = ((unsigned long)(time(NULL)*60/CLK_TCK));
#else 
//#if defined(sun)
//	status = getrusage(RUSAGE_SELF, &rusage_struct);
//
//	/* On the Sun, add system and user time */
//	minor = rusage_struct.ru_utime.tv_sec*60 + 
//	      rusage_struct.ru_utime.tv_usec*60/1000000 +
//		  rusage_struct.ru_stime.tv_sec*60 +
//		  rusage_struct.ru_stime.tv_usec*60/1000000;
//#else
	{
	  static struct tms timebuf;
	  minor = ((unsigned long)(times(&timebuf)*60/HZ));
	}	
//#endif
#endif
#endif

	if (last_part2 >= minor)
	  minor = last_part2 + 1;
		
	last_part2 = minor;

	return(aafMobIDFromMajorMinor(42, major, minor, 4, mobID));		// !!!All toolkit generated, all data
}

AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	prefix,
        aafUInt32	major,
		aafUInt32	minor,
		aafUInt8	UMIDType,
		aafMobID_t *mobID)     /* OUT - Newly created Mob ID */
{
	union MobIDOverlay		aLabel;
	
    aLabel.OMFMobID.SMPTELabel[0]	= 0x06;
    aLabel.OMFMobID.SMPTELabel[1]	= 0x0C;
    aLabel.OMFMobID.SMPTELabel[2]	= 0x2B;
    aLabel.OMFMobID.SMPTELabel[3]	= 0x34;
    aLabel.OMFMobID.SMPTELabel[4]	= 0x02;			// Still Open
    aLabel.OMFMobID.SMPTELabel[5]	= 0x05;			// Still Open
    aLabel.OMFMobID.SMPTELabel[6]	= 0x11;			// Still Open
    aLabel.OMFMobID.SMPTELabel[7]	= 0x01;			// Still Open
    aLabel.OMFMobID.SMPTELabel[8]	= 0x01;			// Still Open
    aLabel.OMFMobID.SMPTELabel[9]	= UMIDType;
    aLabel.OMFMobID.SMPTELabel[10]	= 0x10;			// Still Open
    aLabel.OMFMobID.SMPTELabel[11]	= 0x00;
	aLabel.OMFMobID.length			= 0x13;
    aLabel.OMFMobID.instanceHigh		= 0x00;
    aLabel.OMFMobID.instanceMid		= 0x00;
	aLabel.OMFMobID.instanceLow		= 0x00;
	aLabel.OMFMobID.material.oid				= 0x06;
	aLabel.OMFMobID.material.size				= 0x0E;
	aLabel.OMFMobID.material.ulcode			= 0x2B;
	aLabel.OMFMobID.material.SMPTE				= 0x34;
	aLabel.OMFMobID.material.Registry			= 0x7F;
	aLabel.OMFMobID.material.unused			= 0x7F;
	aLabel.OMFMobID.material.MobIDPrefixHigh	= (aafUInt8)((prefix >> 7L) | 0x80);
	aLabel.OMFMobID.material.MobIDPrefixLow	= (aafUInt8)(prefix & 0x7F);

	aLabel.OMFMobID.material.MobIDMajor		= major;
	aLabel.OMFMobID.material.MobIDMinorLow		= (aafUInt16)(minor & 0xFFFF);
	aLabel.OMFMobID.material.MobIDMinorHigh	=  (aafUInt16)((minor >> 16L) & 0xFFFF);

	*mobID = (aafMobID_t)aLabel.mobID;
	return(AAFRESULT_SUCCESS);
}

#if defined(_MAC) || defined(macintosh)
// For some reason the CoCreateGuid() function is not implemented in the 
// Microsoft Component Library...so we define something that should be
// fairly unique on the mac.

#if defined(_MAC) || defined(macintosh)
#include <Events.h>
#endif
#include <time.h>

static void pvtMacCreateGuid(GUID  *pguid)
{
  // {1994bd00-69de-11d2-b6bc-fcab70ff7331}
  static GUID sTemplate = 
    { 0x1994bd00,
		0x69de,
		0x11d2,
		{ 0xb6, 0xbc, 0xfc, 0xab, 0x70, 0xff, 0x73, 0x31 } };
  static bool sInitializedTemplate = false;
  
  assert (pguid);
  if (!sInitializedTemplate)
  {
    time_t timer = time(NULL);
#ifndef __sgi // temp
    aafUInt32 ticks = TickCount();
    sTemplate.Data1 += timer + ticks;
#else
    sTemplate.Data1 += timer;
#endif
    sInitializedTemplate = true;
  }
  
  // Just bump the first member of the guid to emulate GUIDGEN behavior.
  ++sTemplate.Data1;
  *pguid = sTemplate;
}
#endif


// Initializes a new auid
AAFRESULT aafAUIDNew(aafUID_t * auid)
{
  if (! auid)
	 return AAFRESULT_NULL_PARAM;
  GUID guid;
#if defined(_MAC) || defined(macintosh)
  pvtMacCreateGuid (&guid);
#else
  HRESULT hr = CoCreateGuid (&guid);
  if (FAILED (hr))
	 return hr;
#endif
  memcpy (auid, &guid, sizeof (guid));
  return AAFRESULT_SUCCESS;
}


typedef struct
	{
	aafUInt32	fpMinute;
	aafUInt32   fpHour;
	aafUInt32	dropFpMin;
	aafUInt32	dropFpMin10;
	aafUInt32	dropFpHour;
} frameTbl_t;

/*************************************************************************
 * Private Function: GetFrameInfo()
 *
 *      This function is used by omfsTimecodeToString().  It pulls
 *      apart a frame rate into elements in different units, and
 *      returns a structure containing these units.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
static frameTbl_t GetFrameInfo(
							   aafInt32 fps) /* IN - Frame Rate */
{
	frameTbl_t	result;
	
	result.dropFpMin = (60 * fps) - 2;
	result.dropFpMin10 = (10*result.dropFpMin+2);
	result.dropFpHour = 6 * result.dropFpMin10;

	result.fpMinute = 60 * fps;
	result.fpHour = 60 * result.fpMinute;

	return(result);
}

/*************************************************************************
 * Private Function: PvtOffsetToTimecode()
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
	aafInt16 *frames)        /* OUT - Frames value of timecode */
{
  frameTbl_t info;
  aafUInt32		frames_day;
  aafFrameOffset_t min10, min1;
  aafBool frame_dropped;

  info = GetFrameInfo(frameRate);
  frames_day = (drop ? info.dropFpHour: info.fpHour) *24;

  if (offset < 0L)
	 offset += frames_day;
  if (offset >= frames_day)
	 offset -= frames_day;
  if (drop)
	 {
		*hours = (aafInt16)(offset / info.dropFpHour);
		offset = offset % info.dropFpHour;
		min10 = offset / info.dropFpMin10;
		offset = offset % info.dropFpMin10;
		if (offset < info.fpMinute)
		  {
			 frame_dropped = kAAFFalse;
			 min1 = 0;
		  }
		else
		  {
			 frame_dropped = kAAFTrue;
			 offset -= info.fpMinute;
			 min1 = (offset / info.dropFpMin) + 1;
			 offset = offset % info.dropFpMin;
		  }
		
		*minutes = (aafInt16)((min10 * 10) + min1);
		*seconds = (aafInt16)(offset / frameRate);
		*frames = (aafInt16)(offset % frameRate);
		if (frame_dropped)
		  {
			 (*frames) +=2;
			 if (*frames >= frameRate)
				{
				  (*frames) -= frameRate;
				  (*seconds)++;
				  if (*seconds > 60)
					 {
						(*seconds) -= 60;
						(*minutes)++;
						if (*minutes > 60)
						  {
							 (*minutes) -= 60;
							 (*hours)++;
						  }
					 }
				}
		  }
	 }
  else
	 {
		*hours = (aafInt16)(offset / info.fpHour);
		offset = offset % info.fpHour;
		*minutes = (aafInt16)(offset / info.fpMinute);
		offset = offset % info.fpMinute;
		*seconds = (aafInt16)(offset / frameRate);
		*frames = (aafInt16)(offset % frameRate);
	 }

  return(AAFRESULT_SUCCESS);
}


/*************************************************************************
 * Function: PvtTimecodeToOffset()
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
aafErr_t PvtTimecodeToOffset(
	aafInt16 frameRate,  /* IN - Frame Rate */
	aafInt16 hours,      /* IN - Hours value of Timecode */
	aafInt16 minutes,    /* IN - Minutes value of Timecode */
	aafInt16 seconds,    /* IN - Seconds value of Timecode */
	aafInt16 frames,     /* IN - Frames value of Timecode */
	aafDropType_t drop,  /* IN - Drop of non-drop Timecode */
	aafFrameOffset_t	*result) /* OUT - resulting position */

{
   aafUInt32		val;
	frameTbl_t	info;
	
	info = GetFrameInfo(frameRate);
	if(drop)
		{
		val = (hours * info.dropFpHour);
		val += ((minutes / 10) * info.dropFpMin10);
		val += (minutes % 10) * info.dropFpMin;
		}
	else
		{
		val = hours * info.fpHour;
		val += minutes * info.fpMinute;
		}

	val += seconds * frameRate;
	val += frames;
	
	*result = val;

	return(AAFRESULT_SUCCESS);
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

/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/
