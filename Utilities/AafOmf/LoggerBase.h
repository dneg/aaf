// @doc INTERNAL
// @com This file implements the AAF informational logging utility. 
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

#ifndef LOGGERBASE_H_DEFINED
#define LOGGERBASE_H_DEFINED (1)

#include <stdio.h>
#include <stdarg.h>

/****************************************************************
Name:
	class LoggerBase
Description:
	Base class for run-time logging system.
	The idea is to be able to set different levels of
	logging verbosity one wants by setting the log level.

Notes:

	There are two different modes: Mask and Level.

	Using the level mode:
		The level parameter is to represent a range of levels
		from (0 - level).
		To increase verbosity, set the log level higher.
		Messages logged with a lower level are more urgent
		than messages with a higher level. 
		For example, in a five-level error logging system (0-4)
		messages logged at level 0 represent severe errrors and
		exceptions, while messages logged at level 4 represent
		informational messages. Setting the log level to 0
		guarantees that *only* level 0 (severe errors) are logged. 
		Setting the level to 4 guarantees that *all* messages logged 
		with levels in {0, 1, 2, 3, 4} (sever thru informational) 
		will get logged.


	Using the mask mode:
		The level parameter is used as a mask to break up messages 
		into logically discrete categories with each bit representing a
		category.  To increase verbosity, by logging messages of a 
		given category, set the bits on for the category in question.

  	The level can be changed at any time, but isMask
	is set once when the object is constructed. The idea is
	that you must make up your mind up front on whether to 
	use mask vs. levels and not mix and match them in mid-stream. 

  The helper class VaList is there because the definition of va_list
  is:

  typedef char *va_list;

  This causes problems with calls like

  Log( level, "bla, bla, bla %s\n", "A string" );

  because the compiler can't distinguish between "A string" and
  a va_list.

****************************************************************/

class VaList
{
public:
	va_list args;
	VaList( va_list a );
};

inline VaList::VaList( va_list a ): args( a ) 
{
}

class LoggerBase
{
protected:
	unsigned iLevel; // The log level or mask to test against.
	bool iIsMask; // Is the log level used as a mask?
	virtual void Print( unsigned level, const char *fmt, va_list args  ) = 0;
public:
	virtual void Log( unsigned level, const char *fmt, ... );
	virtual void Log( unsigned level, const char *fmt, VaList &valist  );
	virtual bool LogThisMsg( unsigned level );
	virtual unsigned SetLevel( unsigned level );
	virtual unsigned GetLevel( void );
	LoggerBase( unsigned level = 0, bool isMask = false );
	virtual ~LoggerBase( void );
};


inline LoggerBase::LoggerBase( unsigned level,  bool isMask ) 
	: iLevel( level ), iIsMask( isMask ) 
{
}

inline LoggerBase::~LoggerBase( void ) 
{
}

/****************************************************************
Name:
	LoggerBase::LogThisMsg
Description:
Returns:
	Returns false if the level falls outside the logging range 
	and true otherwise.
	
****************************************************************/
inline bool LoggerBase::LogThisMsg( unsigned level ) 
{ 
	return iIsMask ? (iLevel & level) != 0 : (level <= iLevel); 
} 

/****************************************************************
Name:
	LoggerBase::SetLevel
Description:
	Set/Get the logging level.
Returns:
	The previous level (Set).
	
****************************************************************/
inline unsigned LoggerBase::SetLevel( unsigned level ) 
{ 
	unsigned temp = iLevel; 
	iLevel = level; 
	return temp; 
}

inline unsigned LoggerBase::GetLevel( void )
{ 
	return iLevel; 
}

#endif // LOGGER_H_DEFINED
