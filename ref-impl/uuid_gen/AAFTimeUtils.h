#ifndef AAFTimeUtils_h
#define AAFTimeUtils_h

#include "AAFTypes.h"

/*
* Function: greg2jul
* Description: converts  year, month, day to julian date
* Input: date to be converted
* Return: Julian date
*/

double greg2jul(const aafTimeStamp_t *gdate);

/*
* Function: jul2greg
* Description: converts julian date to year, month, day
* Return:
*/
void jul2greg(double jdate, aafTimeStamp_t *gdate);

/*
* Function: modJulianDate
* Description: converts  year, month, day to modified julian date
* Return:
*/
aafUInt32 modJulianDate (const aafTimeStamp_t *gdate );

typedef aafUInt64 aafUUID_Time_t;
/* set the following to the number of 100ns ticks of the actual
resolution of your system's clock */
#define UIDS_PER_TICK 1024

/*
* Function: aafGetSystemTime
* system dependent call to get the current system time.
* Returned as 100ns ticks since Oct 15, 1582, but resolution may be
* less than 100ns.
*/
extern void aafGetSystemTime (aafUUID_Time_t *timestamp);

/*
* Function: aafGetCurrentTime
* get time as 60 bit 100ns ticks since whenever.
* Compensate for the fact that real clock resolution is
* less than 100ns. 
*/
extern void aafGetCurrentTime(aafUUID_Time_t * timestamp);

/*
* Function:  aafTimeIntoDay
* Returns the time into the current day sampled according to the frequency
* specified. For e.g. if frequecy is 1, time into day is returned in seconds
* if frequence is 1000 time into day is returned in milliseconds and so
* on
*/
extern aafUInt32 aafTimeIntoDay (aafUInt32 freq);
#endif
