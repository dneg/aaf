// @doc INTERNAL
// @com This file implements the AAF informational logging utility. 
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

#include "StreamLogger.h"
#include <time.h>

/****************************************************************
Name:
	StreamLogger::SetLevel
Description:
	Set the log level and make note of it in the log.
Returns:
	The previous level.
****************************************************************/
unsigned StreamLogger::SetLevel( unsigned level ) 
{ 
	unsigned temp = LoggerBase::SetLevel( level );
	Log( level, "Log Level changed from %d (0x%x) to %d (0x%x)\n", 
		temp, temp, level, level );
	return temp; 
}

/****************************************************************
Name:
	StreamLogger::Print
Description:
	Print a timestamped log message to the owned stream.
Returns:
	None.
****************************************************************/
void StreamLogger::Print( unsigned level, const char *fmt, va_list args  )
{

	if( iStream != 0 )
	{
		time_t currTime;
		time( &currTime );
		char *timeStr = ctime( &currTime );
		timeStr[24] = 0; // ctime hands back a well defined fixed length string.
		fprintf( iStream, "Level[%d] %s: " , level, timeStr );
		vfprintf( iStream, fmt, args );
	}
   
	return;
}
