// @doc INTERNAL
// @com This file implements the AAF auto resource release handling utility. 
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

#ifndef AUTORELEASE_H_DEFINED
#define AUTORELEASE_H_DEFINED (1)

#include "Assertion.h"

/**************************************************************************
Name:
	template <class resource> class AutoRelease
Description:
	This template is used to handle the automatic releasing of
	resources like com resources when the resource goes out of scope.
	The template can be used to simplify logic in the presence of
	exceptions.
**************************************************************************/

template <class resource> class AutoRelease
{
private:

	// This helper member is used to keep track of
	// a resource that needs releasing when it goes
	// out of scope.
	struct Resource
	{
		resource *ptr;
		Resource( void ) 
		{ 
			ptr = 0; 
		}
		~Resource( void ) 
		{ 
			if( ptr != 0 ) 
			{
				ptr->Release(); 
			}
		}
	} iResource;

	// AutoRelease objects are not meant to get copied around. They are meant
	// to be initialized once and to release their resource pointers when
	// they go out of scope. They are private because they are not meant 
	// to be used.
	AutoRelease & operator= ( AutoRelease &ar ) { return *this; } // N/A
	AutoRelease( AutoRelease &ar ){};// N/A
public:
	AutoRelease<resource>& operator= ( resource * ptr )
	{
		Assert( iResource.ptr == 0 );
		iResource.ptr = ptr;

		return *this;
	}

	AutoRelease( resource *ptr )
	{
		Assert( iResource.ptr == 0 );
		iResource.ptr = ptr;
	}

	AutoRelease( void )
	{
	}
};

#endif
