// @doc INTERNAL
// @com This file implements the AAF exception handling utility. 
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
	operator= (AAFRESULT status );
	AAFCheck(AAFRESULT status = AAFRESULT_SUCCESS );
};

inline AAFCheck & AAFCheck::operator= ( AAFCheck &status )
{ 
	return *this; 
}

inline AAFCheck::AAFCheck(AAFCheck &status)
{
}

inline AAFCheck::operator= ( AAFRESULT status )
{ 
	AAFException::Check( status ); 
}
	
inline AAFCheck::AAFCheck( AAFRESULT status )
{ 
	AAFException::Check( status ); 
}

#endif 
