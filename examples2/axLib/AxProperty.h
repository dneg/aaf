#ifndef __AxProperty_h__
#define __AxProperty_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
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
