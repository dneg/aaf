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

#include "AxObject.h"

#include <AAF.h>

#include "AxEx.h"
#include "AxUtil.h"

//=---------------------------------------------------------------------=

//AxObject::AxObject()
//{}

AxObject::AxObject( const AxObject& other )
:	AxBaseObj( AxQueryInterface<IAAFObject, IUnknown>(other._spIaafObject) ),
	_spIaafObject( other._spIaafObject )
{}

AxObject::AxObject( IAAFObjectSP spIaafObject )
:	AxBaseObj( AxQueryInterface<IAAFObject, IUnknown>(spIaafObject) ),
	_spIaafObject( spIaafObject )
{}

AxObject::~AxObject()
{}

#if 0
AxString AxObject::GetClassName()
{
	return *AxDefNameToString< IAAFObject, IAAFClassDef >( _spIaafObject );
}
#endif

#if 0
IAAFPropertyDefSP AxObject::RegisterOptionalPropertyDef ( aafUID_constref  id,
											 const AxString&  name,
									         IAAFTypeDefSP spTypeDef )
{
	IAAFPropertyDefSP spPropDef;
	CHECK_HRESULT(
		_spIaafObject->RegisterOptionalPropertyDef( id, name, spTypeDef, &spPropDef ) );
	retur spPropDef;
}
#endif

AxPropertyIter AxObject::CreatePropertyIter()
{
	IEnumAAFPropertiesSP spIEnumProperties;

	CHECK_HRESULT( _spIaafObject->GetProperties( &spIEnumProperties ) );
		
	AxPropertyIter propertyIter( spIEnumProperties );

	return propertyIter;
}

//=---------------------------------------------------------------------=
