#ifndef __AxProperty_h__
#define __AxProperty_h__

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
//=---------------------------------------------------------------------=

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxBaseObj.h"

#include <AAFPropertyDefs.h>

#include <memory>

class AxProperty : public AxBaseObj {
public:
//	AxProperty();
	AxProperty( const AxProperty& other );
	AxProperty( IAAFPropertySP spIaafProperty );

	virtual ~AxProperty();

	AxString GetName() const;
		
	IAAFPropertyValueSP GetValue() const;
	
	aafUID_t GetAUID() const;

	IAAFPropertyDefSP GetDefinition();

	bool isA( const aafUID_t& propertyTypeUID ) const;

private:
	AxProperty& operator=( const AxProperty& );

	mutable IAAFPropertySP _spIaafProperty;
};

inline bool operator==( const AxProperty& property, const aafUID_t& propertyTypeUID )
{
	return property.isA( propertyTypeUID );
}

#endif
