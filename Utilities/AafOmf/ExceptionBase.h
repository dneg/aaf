// @doc INTERNAL
// @com This file implements the AAF base exception handling utility. 
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
	virtual	const char *Type( void ); // A string which IDs the exception.
	virtual long	Code( void );	// The return code.
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

/*******************************************************************
Name:
	ExceptionBase::Type
Description:
Returns:
	Return the type of exception. 
	
	  This method really *should* be overridden by the child class.
	  This is not enforced with a pure virtual function specification
	  because an ExceptionBase object couldn't be constructed by value
	  in a catch() statement.
********************************************************************/
inline	const char *ExceptionBase::Type( void )
{
	return "Base";
}
#endif
