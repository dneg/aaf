// @doc INTERNAL
// @com This file implements the AAF base exception handling utility. 
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

#ifndef EXCEPTBASE_H_DEFINED
#define EXCEPTBASE_H_DEFINED (1)

#include "LoggerBase.h"

/*******************************************************************
Name: 
	 class ExceptionBase
Description:
	Base exception handling class. Can be set up to log exceptions
	a stream, etc. Inheriting classes error codes must be able to
	fit into a long.

********************************************************************/
class ExceptionBase
{
protected:
	static LoggerBase *iLogger; // Where to log errors...
	long	iErrorCode; // All exception status values must fit in a long.
public:
	ExceptionBase( long errCode );
	virtual ~ExceptionBase( void);
	virtual	const char *Type( void ) = 0; // A string which IDs the exception.
	virtual long	Code( void );
	static LoggerBase *SetLogger( LoggerBase *logger );
};


inline ExceptionBase::ExceptionBase( long errCode ) : iErrorCode( errCode ) 
{
}

inline ExceptionBase::~ExceptionBase( void) 
{
}

/*******************************************************************
Name:
	ExceptionBase::SetLogger
Description:
	Set the current exception handling logger.
Returns:
	The previous logger.
********************************************************************/
inline LoggerBase * ExceptionBase::SetLogger( LoggerBase *logger ) 
{ 
	LoggerBase *temp = iLogger;
	iLogger = logger; 
	return temp;
}

/*******************************************************************
Name:
	ExceptionBase::Code
Description:
Returns:
	Return the error code. All children of this class should be able
	to store their error codes in a long without loss so that a caller
	can catch the base class exception and display the error code.
********************************************************************/
inline long	ExceptionBase::Code( void ) 
{ 
	return iErrorCode; 
}

#endif
