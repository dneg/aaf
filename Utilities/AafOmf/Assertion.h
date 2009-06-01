// @doc INTERNAL
// @com This file implements the Assertion handling utility. 
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

#ifndef ASSERTION_H_DEFINED
#define ASSERTION_H_DEFINED (1)

#include "ExceptionBase.h"

/*******************************************************************
Name: 
	 class Assertion
Description:
	Assertion exception handling class.

********************************************************************/

class Assertion : public ExceptionBase
{
public:
	Assertion( long errCode ); 
	virtual ~Assertion( void );
	virtual	const char *Type( void );

	// Does a status check on the assertion and throws an
	// exception on failure. 
	static void Check( bool assertion, const char *fmt = 0, ... ); 
};

inline Assertion::Assertion( long errCode ) : ExceptionBase( errCode ) 
{
}	

inline Assertion::~Assertion( void ) 
{
} 

/********************************************************************
Name: 
	Assertion::Type
Description:
Returns:
	A string which IDs this exception type as "Assertion."
********************************************************************/

inline const char *Assertion::Type( void )
{ 
	return "Assertion"; 
}

#define	Assert( x ) Assertion::Check( (x), "Assertion failed: %s, file %s, line %d\n",  #x, __FILE__, __LINE__ )

#endif 
