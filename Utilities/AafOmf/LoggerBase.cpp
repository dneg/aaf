// @doc INTERNAL
// @com This file implements the AAF informational logging utility. 
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

#include "LoggerBase.h"
#include <time.h>

/****************************************************************
Name:
	LoggerBase::Log
Description:
	Logs a message using printf() style format and args.
	if the message level falls meets the verbosity test.
Returns:
	None
	
****************************************************************/
void LoggerBase::Log( unsigned level, const char *fmt, const VaList &valist )
{
	if( LogThisMsg( level ) )
	{
		Print( level, fmt, valist.args );
	}
}

void LoggerBase::Log( unsigned level, const char *fmt, ... )
{
	if( LogThisMsg( level ) )
	{
		va_list args;
		va_start( args, fmt );
		Print( level, fmt, args );
		va_end( args );
	}
}
