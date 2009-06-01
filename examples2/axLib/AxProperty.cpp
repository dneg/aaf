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

#include "AxProperty.h"
#include "AxUtil.h"
#include "AxEx.h"

//AxProperty::AxProperty()
//{}

AxProperty::AxProperty( const AxProperty& other )
:	AxBaseObj( AxQueryInterface<IAAFProperty,IUnknown>(other._spIaafProperty) ),
	_spIaafProperty( other._spIaafProperty )
{}

AxProperty::AxProperty( IAAFPropertySP spIaafProperty )
:	AxBaseObj( AxQueryInterface<IAAFProperty,IUnknown>(spIaafProperty) ),
	_spIaafProperty( spIaafProperty )
{}

AxProperty::~AxProperty()
{}

IAAFPropertyValueSP AxProperty::GetValue() const
{
	IAAFPropertyValueSP spIaafPropertyValue;

	CHECK_HRESULT( _spIaafProperty->GetValue( &spIaafPropertyValue ) );

	return spIaafPropertyValue;
}

AxString AxProperty::GetName() const
{
	return AxDefNameToString< IAAFProperty, IAAFPropertyDef >( _spIaafProperty );
}

aafUID_t AxProperty::GetAUID() const
{
	IAAFPropertyDefSP spIaafPropertyDef;
	IAAFMetaDefinitionSP spIaafMetaDefintion;
	aafUID_t uid;

	CHECK_HRESULT( _spIaafProperty->GetDefinition( &spIaafPropertyDef ) );
		
	CHECK_HRESULT( spIaafPropertyDef->QueryInterface( IID_IAAFMetaDefinition,
					reinterpret_cast<void**>( &spIaafMetaDefintion ) ) );

	CHECK_HRESULT( spIaafMetaDefintion->GetAUID( &uid ) );

	return uid;
}

IAAFPropertyDefSP AxProperty::GetDefinition()
{
	IAAFPropertyDefSP spDefinition;

	CHECK_HRESULT( _spIaafProperty->GetDefinition( &spDefinition ) );

	return spDefinition;
}

bool AxProperty::isA( const aafUID_t& propertyTypeUID ) const
{
	// FIXME - a class to encapsulate aafUID_t that provides an
	// operator== implementation would be nicer.
	return GetAUID() == propertyTypeUID;
}
