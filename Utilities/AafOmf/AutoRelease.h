// @doc INTERNAL
// @com This file implements the AAF auto resource release handling utility. 
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
