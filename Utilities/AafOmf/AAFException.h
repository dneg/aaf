// @doc INTERNAL
// @com This file implements the AAF exception handling utility. 
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef AAFEXCEPT_H_DEFINED
#define AAFEXCEPT_H_DEFINED (1)

#include "ExceptionBase.h"
#include "AAFTypes.h"
#include "AAFResult.h"

/*******************************************************************
Name: 
	 class AAFException
Description:
	AAF exception handling class.

********************************************************************/
class AAFException : public ExceptionBase
{
public:
	AAFException( AAFRESULT errCode );
	virtual ~AAFException( void );
	virtual	const char *Type( void );

	// Does a status check on the the error code and throws an
	// exception on failure. 
	static void Check( AAFRESULT errCode, const char *fmt = 0, ... ); 
};

inline AAFException::AAFException( AAFRESULT errCode ) : ExceptionBase( errCode )
{
}

inline AAFException::~AAFException( void )
{
}

/********************************************************************
Name: 
	AAFException::Type
Description:
Returns:
	A string which IDs this exception type as "AAF."
********************************************************************/

inline	const char *AAFException::Type( void ) 
{ 
	return "AAF"; 
}

class AAFCheck
{
private:
	// These things are not meant to get copied around. They are meant
	// to check the status of function calls. They are private because
	// they are not meant to be used.
	AAFCheck & operator= (AAFCheck &status ); // N/A
	AAFCheck(AAFCheck &status); // N/A
public:
	AAFCheck& operator= (AAFRESULT status );
	AAFCheck(AAFRESULT status = AAFRESULT_SUCCESS );
};

inline AAFCheck & AAFCheck::operator= ( AAFCheck &/*status*/ )
{ 
	return *this; 
}

inline AAFCheck::AAFCheck(AAFCheck &/*status*/)
{
}

inline AAFCheck& AAFCheck::operator= ( AAFRESULT status )
{ 
	AAFException::Check( status ); 
	return *this;
}
	
inline AAFCheck::AAFCheck( AAFRESULT status )
{ 
	AAFException::Check( status ); 
}

#endif 

