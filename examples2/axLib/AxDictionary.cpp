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

#include "AxDictionary.h"

#include <AAFResult.h>

AxDictionary::AxDictionary( IAAFDictionarySP spIaafDictionary )
: AxObject( AxQueryInterface<IAAFDictionary, IAAFObject>( spIaafDictionary ) ),
    _spIaafDictionary( AxQueryInterface<IAAFDictionary, IAAFDictionary2>( spIaafDictionary ) )
{}

AxDictionary::AxDictionary( IAAFDictionary2SP spIaafDictionary )
:	AxObject( AxQueryInterface<IAAFDictionary2, IAAFObject>(spIaafDictionary) ),
	_spIaafDictionary( spIaafDictionary )
{}

AxDictionary::~AxDictionary()
{}

bool AxDictionary::isKnownTypeDef( const aafUID_t& typeId )
{
	IAAFTypeDefSP spIaafTypeDef;

	HRESULT hr = _spIaafDictionary->LookupTypeDef( typeId, &spIaafTypeDef );

	bool result = true;

	// We accept AAFRESULT_NO_MORE_OBJECTS (i.e. not known), or 
	// AAFRESULT_SUCCESS (known).  Anything else throws.

	// FIXME - This is not a documented return value!  (FIXDOCS)
	if ( AAFRESULT_NO_MORE_OBJECTS == hr ) {
		result = false;
	}
	else if ( AAFRESULT_SUCCESS != hr ) {
		CHECK_HRESULT( hr );
	}

	return result;
}

bool AxDictionary::isKnownOperationDef( const aafUID_t& opId )
{
	IAAFOperationDefSP spIaafOperationDef;

	HRESULT hr = _spIaafDictionary->LookupOperationDef( opId, &spIaafOperationDef );

	bool result = true;

	// We accept AAFRESULT_NO_MORE_OBJECTS (i.e. not known), or 
	// AAFRESULT_SUCCESS (known).  Anything else throws.

	// FIXME - This is not a documented return value!  (FIXDOCS)
	if ( AAFRESULT_NO_MORE_OBJECTS == hr ) {
		result = false;
	}
	else if ( AAFRESULT_SUCCESS != hr ) {
		CHECK_HRESULT( hr );
	}

	return result;
}

bool AxDictionary::isKnownParameterDef( const aafUID_t& paramId )
{
	IAAFParameterDefSP spIaafParameterDef;

	HRESULT hr = _spIaafDictionary->LookupParameterDef( paramId, &spIaafParameterDef );

	bool result = true;
	if ( AAFRESULT_NO_MORE_OBJECTS == hr ) {
		result = false;
	}
	else if ( AAFRESULT_SUCCESS != hr ) {
		CHECK_HRESULT( hr );
	}

	return result;
}

bool AxDictionary::isKnownInterpolationDef( const aafUID_t& intId )
{
    IAAFInterpolationDefSP spIaafInterpolationDef;

    HRESULT hr = _spIaafDictionary->LookupInterpolationDef( intId, &spIaafInterpolationDef );

    bool result = true;
    if ( AAFRESULT_NO_MORE_OBJECTS == hr ) {
        result = false;
    }
    else if ( AAFRESULT_SUCCESS != hr ) {
        CHECK_HRESULT( hr );
    }

    return result;
}

void AxDictionary::RegisterClassDef( IAAFClassDefSP spIaafClassDef )
{
	CHECK_HRESULT( _spIaafDictionary->RegisterClassDef( spIaafClassDef ) );
}

IAAFTypeDefSP AxDictionary::LookupTypeDef( const aafUID_t& typeId )
{
	IAAFTypeDefSP spTypeDef;

	CHECK_HRESULT( _spIaafDictionary->LookupTypeDef( typeId, &spTypeDef ) );

	return spTypeDef;
}

IAAFClassDefSP AxDictionary::LookupClassDef( aafUID_constref  classId )
{
	IAAFClassDefSP spIaafClassDef;

	CHECK_HRESULT( _spIaafDictionary->LookupClassDef( classId, &spIaafClassDef ) );

	return spIaafClassDef;
}

IAAFDataDefSP AxDictionary::LookupDataDef( aafUID_constref  dataDefinitionId )
{
	IAAFDataDefSP spDataDef;

	CHECK_HRESULT( _spIaafDictionary->LookupDataDef( dataDefinitionId, &spDataDef ) );

	return spDataDef;
}

IAAFParameterDefSP AxDictionary::LookupParameterDef( const aafUID_t& opDefId )
{
	IAAFParameterDefSP spIaafParameterDef;

	CHECK_HRESULT( _spIaafDictionary->LookupParameterDef( opDefId, &spIaafParameterDef ) );

	return spIaafParameterDef;
}

IAAFOperationDefSP AxDictionary::LookupOperationDef( const aafUID_t& opDefId )
{
	IAAFOperationDefSP spIaafOperationDef;

	CHECK_HRESULT( _spIaafDictionary->LookupOperationDef( opDefId, &spIaafOperationDef ) );

	return spIaafOperationDef;
}

IAAFContainerDefSP AxDictionary::LookupContainerDef( const aafUID_t& contDefId )
{
	IAAFContainerDefSP spIaafContainerDef;

	CHECK_HRESULT( _spIaafDictionary->LookupContainerDef( contDefId, &spIaafContainerDef ) );

	return spIaafContainerDef;
}

IAAFInterpolationDefSP AxDictionary::LookupInterpolationDef( const aafUID_t& interpDefId )
{
	IAAFInterpolationDefSP spIaafInterpolationDef;

	CHECK_HRESULT( _spIaafDictionary->LookupInterpolationDef( interpDefId, &spIaafInterpolationDef ) );

	return spIaafInterpolationDef;
}

IAAFCodecDefSP AxDictionary::LookupCodecDef( const aafUID_t& codecDefId )
{
	IAAFCodecDefSP spIaafCodecDef;

	CHECK_HRESULT( _spIaafDictionary->LookupCodecDef( codecDefId, &spIaafCodecDef ) );

	return spIaafCodecDef;
}

IEnumAAFClassDefsSP AxDictionary::GetClassDefs()
{
	IEnumAAFClassDefsSP spIEnumAAFClassDefs;

	CHECK_HRESULT( _spIaafDictionary->GetClassDefs( &spIEnumAAFClassDefs ) );

	return spIEnumAAFClassDefs;
}

IEnumAAFTypeDefsSP AxDictionary::GetTypeDefs()
{
	IEnumAAFTypeDefsSP spIEnumAAFTypeDefs;

	CHECK_HRESULT( _spIaafDictionary->GetTypeDefs( &spIEnumAAFTypeDefs ) );

	return spIEnumAAFTypeDefs;
}

IEnumAAFDataDefsSP AxDictionary::GetDataDefs()
{
	IEnumAAFDataDefsSP spIEnumAAFDataDefs;

	CHECK_HRESULT( _spIaafDictionary->GetDataDefs( &spIEnumAAFDataDefs ) );

	return spIEnumAAFDataDefs;
}

IEnumAAFOperationDefsSP AxDictionary::GetOperationDefs()
{
	IEnumAAFOperationDefsSP spIEnumAAFOperationDefs;

	CHECK_HRESULT( _spIaafDictionary->GetOperationDefs( &spIEnumAAFOperationDefs ) );

	return spIEnumAAFOperationDefs;
}

IEnumAAFParameterDefsSP AxDictionary::GetParameterDefs()
{
	IEnumAAFParameterDefsSP spIEnumAAFParameterDefs;

	CHECK_HRESULT( _spIaafDictionary->GetParameterDefs( &spIEnumAAFParameterDefs ) );

	return spIEnumAAFParameterDefs;
}

IEnumAAFPluginDefsSP AxDictionary::GetPluginDefs()
{
    IEnumAAFPluginDefsSP spIEnumAAFPluginDefs;

    CHECK_HRESULT( _spIaafDictionary->GetPluginDefs( &spIEnumAAFPluginDefs ) );

    return spIEnumAAFPluginDefs;
}

IEnumAAFContainerDefsSP AxDictionary::GetContainerDefs()
{
    IEnumAAFContainerDefsSP spIEnumAAFContainerDefs;

    CHECK_HRESULT( _spIaafDictionary->GetContainerDefs( &spIEnumAAFContainerDefs ) );

    return spIEnumAAFContainerDefs;
}

IEnumAAFInterpolationDefsSP AxDictionary::GetInterpolationDefs()
{
    IEnumAAFInterpolationDefsSP spIEnumAAFInterpolationDefs;

    CHECK_HRESULT( _spIaafDictionary->GetInterpolationDefs( &spIEnumAAFInterpolationDefs ) );

    return spIEnumAAFInterpolationDefs;
}

IEnumAAFCodecDefsSP AxDictionary::GetCodecDefs()
{
    IEnumAAFCodecDefsSP spIEnumAAFCodecDefs;

    CHECK_HRESULT( _spIaafDictionary->GetCodecDefs( &spIEnumAAFCodecDefs ) );

    return spIEnumAAFCodecDefs;
}

IEnumAAFKLVDataDefsSP AxDictionary::GetKLVDataDefs()
{
    IEnumAAFKLVDataDefsSP spIEnumAAFKLVDataDefs;

    CHECK_HRESULT( _spIaafDictionary->GetKLVDataDefs( &spIEnumAAFKLVDataDefs ) );

    return spIEnumAAFKLVDataDefs;
}

IEnumAAFTaggedValueDefsSP AxDictionary::GetTaggedValueDefs()
{
    IEnumAAFTaggedValueDefsSP spIEnumAAFTaggedValueDefs;

    CHECK_HRESULT( _spIaafDictionary->GetTaggedValueDefs( &spIEnumAAFTaggedValueDefs ) );

    return spIEnumAAFTaggedValueDefs;
}

void AxDictionary::RegisterOpaqueTypeDef( IAAFTypeDefSP spTypeDef )
{
	CHECK_HRESULT( _spIaafDictionary->RegisterOpaqueTypeDef( spTypeDef ) );
}

void AxDictionary::RegisterKLVDataKey( const aafUID_t& uuid,
				       IAAFTypeDefSP spTypeDef )
{
	CHECK_HRESULT( _spIaafDictionary->RegisterKLVDataKey( uuid, spTypeDef ) );
}

void AxDictionary::RegisterOperationDef( IAAFOperationDefSP spIaafOperationDef )
{
	CHECK_HRESULT( _spIaafDictionary->RegisterOperationDef( spIaafOperationDef ) );
}

void AxDictionary::RegisterParameterDef( IAAFParameterDefSP spIaafParameterDef )
{
	CHECK_HRESULT( _spIaafDictionary->RegisterParameterDef( spIaafParameterDef ) );
}

void AxDictionary::RegisterCodecDef( IAAFCodecDefSP spIaafCodecDef)
{
	CHECK_HRESULT( _spIaafDictionary->RegisterCodecDef( spIaafCodecDef ) );
}

void AxDictionary::RegisterDataDef( IAAFDataDefSP spIaafDataDef )
{
    CHECK_HRESULT( _spIaafDictionary->RegisterDataDef( spIaafDataDef ) );
}

void AxDictionary::RegisterPluginDef( IAAFPluginDefSP spIaafPluginDef )
{
    CHECK_HRESULT( _spIaafDictionary->RegisterPluginDef( spIaafPluginDef ) );
}

void AxDictionary::RegisterContainerDef( IAAFContainerDefSP spIaafContainerDef )
{
    CHECK_HRESULT( _spIaafDictionary->RegisterContainerDef( spIaafContainerDef ) );
}

void AxDictionary::RegisterInterpolationDef( IAAFInterpolationDefSP spIaafInterpolationDef )
{
    CHECK_HRESULT( _spIaafDictionary->RegisterInterpolationDef( spIaafInterpolationDef ) );
}

void AxDictionary::RegisterKLVDataDef( IAAFKLVDataDefinitionSP spIaafKLVDataDef )
{
    CHECK_HRESULT( _spIaafDictionary->RegisterKLVDataDef( spIaafKLVDataDef ) );
}

void AxDictionary::RegisterTaggedValueDef( IAAFTaggedValueDefinitionSP spIaafTaggedValueDef )
{
    CHECK_HRESULT( _spIaafDictionary->RegisterTaggedValueDef( spIaafTaggedValueDef ) );
}

IUnknownSP AxDictionary::CreateInstance( const aafUID_t& auid,
				  					     const IID& iid )
{
	IUnknownSP spIUnknown;

	CHECK_HRESULT( _spIaafDictionary->CreateInstance( auid, iid, &spIUnknown ) );

	return spIUnknown;
}

IUnknownSP AxDictionary::CreateMetaInstance( const aafUID_t& auid,
				  				 		     const IID& iid )
{
	IUnknownSP spIUnknown;

	CHECK_HRESULT( _spIaafDictionary->CreateMetaInstance( auid, iid, &spIUnknown ) );

	return spIUnknown;
}

aafUInt32 AxDictionary::CountOpaqueTypeDefs()
{
	aafUInt32 count;

	CHECK_HRESULT( _spIaafDictionary->CountOpaqueTypeDefs( &count ) );

	return count;
}

