// @doc INTERNAL
// @com This file implements the OMF exception handling utility. 
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

#ifndef OMFEXCEPT_H_DEFINED
#define OMFEXCEPT_H_DEFINED (1)

#include "ExceptionBase.h"
#include "omErr.h"

/*******************************************************************
Name: 
	 class OMFException
Description:
	OMF exception handling class.

********************************************************************/

class OMFException : public ExceptionBase
{
public:
	OMFException( omfErr_t errCode );
	virtual ~OMFException( void );
	virtual	const char *Type( void );

	// Does a status check on the the error code and throws an
	// exception on failure. 
	static void Check( omfErr_t errCode, const char *fmt = 0, ... ); 
};

inline OMFException::OMFException( omfErr_t errCode ) : ExceptionBase( errCode ) 
{
}	

inline OMFException::~OMFException( void ) 
{
} 

/********************************************************************
Name: 
	OMFException::Type
Description:
Returns:
	A string which IDs this exception type as "OMF."
********************************************************************/

inline const char *OMFException::Type( void )
{ 
	return "OMF"; 
}

/*******************************************************************
Name: 
	 class OMFCheck
Description:
	OMF return status checker. 
	Used to check the return values of OMF calls.
********************************************************************/

class OMFCheck
{
private:
	// These things are not meant to get copied around. They are meant
	// to check the status of function calls. They are private because
	// they are not meant to be used.
	OMFCheck & operator= (OMFCheck &status ); // N/A
	OMFCheck(OMFCheck &status);// N/A
public:
	OMFCheck & operator= ( omfErr_t status );
	OMFCheck( omfErr_t status = OM_ERR_NONE );
};


inline OMFCheck & OMFCheck::operator= ( OMFCheck &/*status*/ )
{ 
	return *this; 
}

inline OMFCheck::OMFCheck( OMFCheck &/*status*/ )
{
}

inline OMFCheck & OMFCheck::operator= ( omfErr_t status )
{ 
	OMFException::Check( status ); 
	return *this;
}
	
inline OMFCheck::OMFCheck( omfErr_t status )
{ 
	OMFException::Check( status ); 
}

#endif // EXCEPTION_H_DEFINED 

