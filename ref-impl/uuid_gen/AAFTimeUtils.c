#include <time.h>
#ifdef OS_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include "AAFTimeUtils.h"	/*	time structures	*/

/*  The following routines implement conversions from/to Modified Julian
 *	Copyright (c) 1986-2002, Hiram Clawson - curator@hiram.ws.NoSpam
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or
 *	without modification, are permitted provided that the following
 *	conditions are met:
 *
 *		Redistributions of source code must retain the above
 *		copyright notice, this list of conditions and the
 *		following disclaimer.
 *
 *		Redistributions in binary form must reproduce the
 *		above copyright notice, this list of conditions and
 *		the following disclaimer in the documentation and/or
 *		other materials provided with the distribution.
 *
 *		Neither name of The Museum of Hiram nor the names of
 *		its contributors may be used to endorse or promote products
 *		derived from this software without specific prior
 *		written permission. 
 *
 */
/*
 *	jul2greg computes the day of the week, the day of the year
 *	the gregorian (or julian) calendar date and the universal
 *	time from the julian decimal date.
 *	for astronomical purposes, The Gregorian calendar reform occurred
 *	on 15 Oct. 1582.  This is 05 Oct 1582 by the julian calendar.

 *	Input:	a double jdate set to the julian date
 *
 *	output:  will set all the elements of the structure aafTimeStamp_t.
 *
 *	Reference: Astronomial formulae for calculators, meeus, p 23
 *	from fortran program by F. Espenak - April 1982 Page 277,
 *	50 Year canon of solar eclipses: 1986-2035
 *
 */


void jul2greg( double jdate, aafTimeStamp_t *gdate )
{
	double frac;
	long jd;
	long ka;
	long kb;
	long kc;
	long kd;
	long ke;
	long ialp;
	double d_hour;
	double d_min;
	double d_sec;

	jd = (long) (jdate + 0.5);	/* integer julian date */
	frac = jdate + 0.5 - (double) jd + 1.0e-10; /* day fraction */
	ka = (long) jd;
	if ( jd >= 2299161L )
	{
		ialp = (long)(( (double) jd - 1867216.25 ) / ( 36524.25 ));
		ka = jd + 1L + ialp - ( ialp >> 2 );
	}
	kb = ka + 1524L;
	kc =  (long)(( (double) kb - 122.1 ) / 365.25);
	kd = (long)((double) kc * 365.25);
	ke = (long)((double) ( kb - kd ) / 30.6001);
	gdate->date.day = kb - kd - ((long) ( (double) ke * 30.6001 ));
	if ( ke > 13L )
		gdate->date.month = ke - 13L;
	else
		gdate->date.month = ke - 1L;
	if ( (gdate->date.month == 2) && (gdate->date.day > 28) )
		gdate->date.day = 29;
	if ( (gdate->date.month == 2) && (gdate->date.day == 29) && (ke == 3L) )
		gdate->date.year = kc - 4716L;
	else if ( gdate->date.month > 2 )
		gdate->date.year = kc - 4716L;
	else
		gdate->date.year = kc - 4715L;
	d_hour = frac * 24.0;	/* hour */
	gdate->time.hour = (long)d_hour;	/* hour */
	d_min = ( d_hour - (double) gdate->time.hour ) * 60.0; /* minute */
	gdate->time.minute = (long)d_min; /*minute*/
	gdate->time.second =
		(long)(( d_min - (double) gdate->time.minute ) * 60.0);/* second */
	gdate->time.fraction = 0;
}

/*
 *	greg2jul computes the julian decimal date (j_date) from
 *	the gregorian (or Julian) calendar date.
 *	for astronomical purposes, The Gregorian calendar reform occurred
 *	on 15 Oct. 1582.  This is 05 Oct 1582 by the julian calendar.
 *	Input:  an aafTimeStamp_t structure pointer where date.day, date.month, 
 *	date.year and time.hour, time.minute, and time.second have been set for 
 *	the date in question.
 *
 *	Output: the calculated julian date.
 *
 *	Reference: Astronomial formulae for calculators, meeus, p 23
 *	from fortran program by F. Espenak - April 1982 Page 276,
 *	50 Year canon of solar eclipses: 1986-2035
 */

double greg2jul( const aafTimeStamp_t *gdate )
{
	double frac, gyr;
	long iy0, im0;
	long ia, ib;
	long jd;
	double j_date;

	/* decimal day fraction	*/
	frac = (( double)gdate->time.hour/ 24.0)
		+ ((double) gdate->time.minute / 1440.0)
		+ (gdate->time.second / 86400.0);
	/* convert gdate to format YYYY.MMDDdd	*/
	gyr = (double) gdate->date.year
		+ (0.01 * (double) gdate->date.month)
		+ (0.0001 * (double) gdate->date.day)
		+ (0.0001 * frac) + 1.0e-9;
	/* conversion factors */
	if ( gdate->date.month <= 2 )
	{
		iy0 = gdate->date.year - 1L;
		im0 = gdate->date.month + 12;
	}
	else
	{
		iy0 = gdate->date.year;
		im0 = gdate->date.month;
	}
	ia = iy0 / 100L;
	ib = 2L - ia + (ia >> 2);
	/* calculate julian date	*/
	if ( gdate->date.year <= 0L )
		jd = (long) ((365.25 * (double) iy0) - 0.75)
			+ (long) (30.6001 * (im0 + 1L) )
			+ (long) gdate->date.day + 1720994L;
	else
		jd = (long) (365.25 * (double) iy0)
			+ (long) (30.6001 * (double) (im0 + 1L))
			+ (long) gdate->date.day + 1720994L;
	if ( gyr >= 1582.1015 )	/* on or after 15 October 1582	*/
		jd += ib;
	j_date = (double) jd + frac + 0.5;
	return j_date ;
}

aafUInt32 modJulianDate (const aafTimeStamp_t *gdate )
{
	double jdate;
	jdate = greg2jul(gdate);
	return (long)(jdate - 2400000.5);
}

/*
* The following routines return time in 100ns since Oct 15, 1582
*/
/*
  ** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
  ** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
  ** Digital Equipment Corporation, Maynard, Mass.
  ** Copyright (c) 1998 Microsoft.
  ** To anyone who acknowledges that this file is provided "AS IS"
  ** without any express or implied warranty: permission to use, copy,
  ** modify, and distribute this file for any purpose is hereby
  ** granted without fee, provided that the above copyright notices and
  ** this notice appears in all source code copies, and that none of
  ** the names of Open Software Foundation, Inc., Hewlett-Packard
  ** Company, or Digital Equipment Corporation be used in advertising
  ** or publicity pertaining to distribution of the software without
  ** specific, written prior permission.  Neither Open Software
  ** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital
  Equipment
  ** Corporation makes any representations about the suitability of
  ** this software for any purpose.
  */



extern void aafGetSystemTime (aafUUID_Time_t *timestamp)
{
/* system dependent call to get the current system time.
   Returned as 100ns ticks since Oct 15, 1582, but resolution may be
   less than 100ns.
*/
#if defined( OS_WINDOWS )
	ULARGE_INTEGER time;

	GetSystemTimeAsFileTime((FILETIME *)&time);

	/* NT keeps time in FILETIME format which is 100ns ticks since
	 Jan 1, 1601.  UUIDs use time in 100ns ticks since Oct 15, 1582.
	 The difference is 17 Days in Oct + 30 (Nov) + 31 (Dec)
	 + 18 years and 5 leap days.
	*/

	time.QuadPart +=
	(unsigned __int64) (1000*1000*10)       // seconds
	* (unsigned __int64) (60 * 60 * 24)       // days
	* (unsigned __int64) (17+30+31+365*18+5); // # of days
	
	*timestamp = time.QuadPart;
#else
	struct timeval tp;

	gettimeofday(&tp, (struct timezone *)0);

	/* Offset between UUID formatted times and Unix formatted times.
		UUID UTC base time is October 15, 1582.
		Unix base time is January 1, 1970.
	*/
	*timestamp = (tp.tv_sec * 10000000) + (tp.tv_usec * 10) +
						AAFCONSTINT64(0x01B21DD213814000);
#endif
}

/* get-current_time -- get time as 60 bit 100ns ticks since whenever.
  Compensate for the fact that real clock resolution is
  less than 100ns. */
extern void aafGetCurrentTime( aafUUID_Time_t * timestamp)
{
	aafUUID_Time_t         time_now;
	static aafUUID_Time_t  time_last;
	static aafUInt16  uids_this_tick;
	static int        inited = 0;
	
	if (!inited)
	{
		aafGetSystemTime(&time_now);
		uids_this_tick = UIDS_PER_TICK;
		inited = 1;
	}
	
	while (1)
	{
		aafGetSystemTime (&time_now);
	
		/* if clock reading changed since last UUID generated... */
		if (time_last != time_now)
		{
			/* reset count of uuids gen'd with this clock reading */
			uids_this_tick = 0;
			break;
		}
		if (uids_this_tick < UIDS_PER_TICK)
		{
			uids_this_tick++;
			break;
		};
		/* going too fast for our clock; spin */
	}
	/* add the count of uuids to low order bits of the clock reading */
	*timestamp = time_now + uids_this_tick;
}

/*
*Returns the time into the current day sampled according to the frequency
*specified. For e.g. if frequecy is 1, time into day is returned in seconds
*if frequence is 1000 time into day is returned in milliseconds and so
*on
*/
extern aafUInt32 aafTimeIntoDay (aafUInt32 freq)
{
#ifdef OS_WINDOWS
#else
	time_t t, midnight_t;
	struct tm * ansitime;
	struct timeval tv;

	t = time(0);
	ansitime = gmtime (&t);

	/* get time at midnight UTC of current day */
	ansitime->tm_hour = ansitime->tm_min = ansitime->tm_sec = 0;
	ansitime->tm_isdst = -1;
	midnight_t = mktime(ansitime);

	/*get current time of day in seconds and micro seconds*/
	gettimeofday (&tv, 0);

	/* sample time into day as per the frequency specified */
	aafUInt32 sample = (tv.tv_sec - midnight_t) * freq + (tv.tv_usec*freq)/1000000;

	return sample;
#endif
}

