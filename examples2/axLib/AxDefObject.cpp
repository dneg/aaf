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
//=---------------------------------------------------------------------=

#include <AxDefObject.h>

//=---------------------------------------------------------------------=

AxDefObject::AxDefObject( IAAFDefObjectSP spIaafDefObject )
:	AxObject( AxQueryInterface<IAAFDefObject, IAAFObject>(spIaafDefObject) ),
	_spIaafDefObject( spIaafDefObject )
{}

AxDefObject::~AxDefObject()
{}

aafUID_t AxDefObject::GetAUID()
{
	aafUID_t uid;

	CHECK_HRESULT( _spIaafDefObject->GetAUID( &uid ) );

	return uid;
}

AxString AxDefObject::GetName()
{
	return AxNameToString( _spIaafDefObject );
}

AxString AxDefObject::GetDescription()
{
	return AxDescriptionToString( _spIaafDefObject );
}


//=---------------------------------------------------------------------=

AxContainerDef::AxContainerDef( IAAFContainerDefSP spIaafContainerDef )
  :	AxDefObject( AxQueryInterface<IAAFContainerDef, IAAFDefObject>(spIaafContainerDef) ),
	_spIaafContainerDef( spIaafContainerDef )
{}

AxContainerDef::~AxContainerDef()
{}

void AxContainerDef::Initialize( const aafUID_t& uid,
				 const AxString& name,
				 const AxString& desc )
{
  CHECK_HRESULT( _spIaafContainerDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

//=---------------------------------------------------------------------=

AxInterpolationDef::AxInterpolationDef( IAAFInterpolationDefSP spIaafInterpolationDef )
  :	AxDefObject( AxQueryInterface<IAAFInterpolationDef, IAAFDefObject>(spIaafInterpolationDef) ),
	_spIaafInterpolationDef( spIaafInterpolationDef )
{}

AxInterpolationDef::~AxInterpolationDef()
{}

void AxInterpolationDef::Initialize( const aafUID_t& uid,
				     const AxString& name,
				     const AxString& desc )
{
  CHECK_HRESULT( _spIaafInterpolationDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

//=---------------------------------------------------------------------=

AxParameterDef::AxParameterDef( IAAFParameterDefSP spIaafParameterDef )
  :	AxDefObject( AxQueryInterface<IAAFParameterDef, IAAFDefObject>(spIaafParameterDef) ),
	_spIaafParameterDef( spIaafParameterDef )
{}

AxParameterDef::~AxParameterDef()
{}

void AxParameterDef::Initialize( const aafUID_t& uid,
				 const AxString& name,
				 const AxString& desc,
				 IAAFTypeDefSP spIaafTypeDef )
{
  CHECK_HRESULT( _spIaafParameterDef->Initialize( uid, name.c_str(), desc.c_str(), spIaafTypeDef ) );
}

//=---------------------------------------------------------------------=

AxPluginDef::AxPluginDef( IAAFPluginDefSP spIaafPluginDef )
:	AxDefObject( AxQueryInterface<IAAFPluginDef, IAAFDefObject>(spIaafPluginDef) ),
	_spIaafPluginDef( spIaafPluginDef )
{}

AxPluginDef::~AxPluginDef()
{}

void AxPluginDef::Initialize( const aafUID_t& uid,
							 const AxString& name,
							 const AxString& desc )
{
	CHECK_HRESULT( _spIaafPluginDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxPluginDef::SetCategoryClass( const aafUID_t& uid )
{
	CHECK_HRESULT( _spIaafPluginDef->SetCategoryClass( uid ) );
}

void AxPluginDef::SetPluginVersionString( const AxString& ver )
{
	CHECK_HRESULT( _spIaafPluginDef->SetPluginVersionString( ver.c_str() ) );
}

void AxPluginDef::SetManufacturerID( const aafUID_t& uid )
{
	CHECK_HRESULT( _spIaafPluginDef->SetManufacturerID( uid ) );
}

void AxPluginDef::SetPluginManufacturerName( const AxString& name)
{
	CHECK_HRESULT( _spIaafPluginDef->SetPluginManufacturerName( name.c_str() ) );
}

void AxPluginDef::SetIsSoftwareOnly( bool isSoftware )
{
	CHECK_HRESULT( _spIaafPluginDef->SetIsSoftwareOnly( isSoftware ) );
}

void AxPluginDef::SetIsAccelerated( bool isAccel )
{
	CHECK_HRESULT( _spIaafPluginDef->SetIsAccelerated( isAccel ) );
}

void AxPluginDef::SetSupportsAuthentication( bool supportsAuth )
{
	CHECK_HRESULT( _spIaafPluginDef->SetSupportsAuthentication( supportsAuth ) );
}

void AxPluginDef::SetManufacturerInfo( IAAFNetworkLocatorSP manuInfo )
{
	CHECK_HRESULT( _spIaafPluginDef->SetManufacturerInfo( manuInfo ) );
}

//=---------------------------------------------------------------------=

AxCodecDef::AxCodecDef( IAAFCodecDefSP spIaafCodecDef )
:	AxDefObject( AxQueryInterface<IAAFCodecDef, IAAFDefObject>(spIaafCodecDef) ),
	_spIaafCodecDef( spIaafCodecDef )
{}

AxCodecDef::~AxCodecDef()
{}

void AxCodecDef::Initialize( const aafUID_t& uid,
							 const AxString& name,
							 const AxString& desc )
{
	CHECK_HRESULT( _spIaafCodecDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxCodecDef::SetFileDescriptorClass( IAAFClassDefSP spClass )
{
	CHECK_HRESULT( _spIaafCodecDef->SetFileDescriptorClass( spClass ) );
}

void AxCodecDef::AddEssenceKind( IAAFDataDefSP spEssenceKind )
{
	CHECK_HRESULT( _spIaafCodecDef->AddEssenceKind( spEssenceKind ) );
}

aafBoolean_t AxCodecDef::IsEssenceKindSupported( IAAFDataDefSP spIaafDataDef )
{
	aafBoolean_t isSupported;

	CHECK_HRESULT( _spIaafCodecDef->IsEssenceKindSupported( spIaafDataDef, &isSupported ) );

	return isSupported;
}

aafUInt32 AxCodecDef::CountEssenceKinds()
{
	aafUInt32 count;

	CHECK_HRESULT( _spIaafCodecDef->CountEssenceKinds( &count ) );

	return count;
}

IEnumAAFDataDefsSP AxCodecDef::GetEssenceKinds()
{
	IEnumAAFDataDefsSP spIEnumDataDefs;

	CHECK_HRESULT( _spIaafCodecDef->GetEssenceKinds( &spIEnumDataDefs ) );

	return spIEnumDataDefs;
}

aafBoolean_t AxCodecDef::AreThereFlavours()
{
	aafBoolean_t areThereFlavours;

	CHECK_HRESULT( _spIaafCodecDef->AreThereFlavours( &areThereFlavours ) );

	return areThereFlavours;
}

IEnumAAFCodecFlavoursSP AxCodecDef::EnumCodecFlavours()
{
	IEnumAAFCodecFlavoursSP spIEnumCodecFlavours;

	CHECK_HRESULT( _spIaafCodecDef->EnumCodecFlavours( &spIEnumCodecFlavours ) );

	return spIEnumCodecFlavours;
}

//=---------------------------------------------------------------------=

AxDataDef::AxDataDef( IAAFDataDefSP spIaafDataDef )
:	AxDefObject( AxQueryInterface<IAAFDataDef, IAAFDefObject>(spIaafDataDef) ),
	_spIaafDataDef( spIaafDataDef )
{}

AxDataDef::~AxDataDef()
{}


bool AxDataDef::IsSoundKind()
{
	aafBoolean_t isSoundKind;

	CHECK_HRESULT( _spIaafDataDef->IsSoundKind( &isSoundKind ) );

	return isSoundKind ? true : false;
}

bool AxDataDef::IsPictureKind()
{
	aafBoolean_t isPictureKind;

	CHECK_HRESULT( _spIaafDataDef->IsPictureKind( &isPictureKind ) );

	return isPictureKind ? true : false;
}

//=---------------------------------------------------------------------=

AxOperationDef::AxOperationDef( IAAFOperationDefSP spIaafOperationDef )
:	AxDefObject( AxQueryInterface<IAAFOperationDef, IAAFDefObject>(spIaafOperationDef) ),
	_spIaafOperationDef( spIaafOperationDef )
{}

AxOperationDef::~AxOperationDef()
{}

void AxOperationDef::Initialize( const aafUID_t& uid,
				 const AxString& name,
				 const AxString& desc )
{
	CHECK_HRESULT( _spIaafOperationDef->Initialize( uid, name.c_str(), desc.c_str() ) );
}

void AxOperationDef::SetDataDef( IAAFDataDefSP spIaafDataDef  )
{
	CHECK_HRESULT( _spIaafOperationDef->SetDataDef( spIaafDataDef ) );
}

void AxOperationDef::SetIsTimeWarp( aafBoolean_t isWarp )
{
	CHECK_HRESULT( _spIaafOperationDef->SetIsTimeWarp( isWarp ) );
}

void AxOperationDef::SetCategory( const aafUID_t& category_auid )
{
	CHECK_HRESULT( _spIaafOperationDef->SetCategory( category_auid ) );
}

void AxOperationDef::SetNumberInputs( aafInt32 numInputs )
{
	CHECK_HRESULT( _spIaafOperationDef->SetNumberInputs( numInputs ) );
}

void AxOperationDef::SetBypass( aafUInt32 bypassTrack )
{
	CHECK_HRESULT( _spIaafOperationDef->SetBypass( bypassTrack ) );
}

IAAFDataDefSP AxOperationDef::GetDataDef()
{
  IAAFDataDefSP spDataDef;
  CHECK_HRESULT( _spIaafOperationDef->GetDataDef( &spDataDef ) );
  return spDataDef;
}

aafBoolean_t AxOperationDef::IsTimeWarp()
{
  aafBoolean_t isTimeWarp;
  CHECK_HRESULT( _spIaafOperationDef->IsTimeWarp( &isTimeWarp ) );
  return isTimeWarp;
}

aafUID_t AxOperationDef::GetCategory()
{
  aafUID_t uid;
  CHECK_HRESULT( _spIaafOperationDef->GetCategory( &uid ) );
  return uid;
}

aafInt32 AxOperationDef::GetNumberInputs()
{
  aafInt32 numInputs;
  CHECK_HRESULT( _spIaafOperationDef->GetNumberInputs( &numInputs ) );
  return numInputs;
}

aafUInt32 AxOperationDef::GetBypass()
{
  aafUInt32 bypass;
  CHECK_HRESULT( _spIaafOperationDef->GetBypass( &bypass ) );
  return bypass;
}
