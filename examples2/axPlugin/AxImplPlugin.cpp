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

#include "AxImplPlugin.h"

#include "AxImplNullEssenceCodec.h"

#include <AxTypes.h>
#include <AxEssence.h>
#include <AxDictionary.h>

#include <AAFCodecDefs.h>
#include <AAFResult.h>

AxImplPlugin::AxImplPlugin()
{}

AxImplPlugin::AxImplPlugin( const aafUID_t& pluginDescAUID )
: _pluginDescriptorAUID( pluginDescAUID )
{}

AxImplPlugin::~AxImplPlugin()
{}

void AxImplPlugin::AddDefinitionParameters( const DefinitionParameters& params )
{
	_definitionParams.push_back( params );
}

void AxImplPlugin::SetDescriptorParameters( const DescriptorParameters& params )
{
	_descriptorParams = params;
}

void AxImplPlugin::SetPluginDescriptorAUID( const aafUID_t& pluginDescAUID )
{
	_pluginDescriptorAUID = pluginDescAUID;
}

//
//  IAAFPlugin methods
//

void AxImplPlugin::CountDefinitions(
		aafUInt32 *pDefCount )
{
	*pDefCount = _definitionParams.size();
}

void AxImplPlugin::GetIndexedDefinitionID(
		aafUInt32 index, aafUID_t *result )
{
	if ( index > _definitionParams.size() ) {
		CHECK_HRESULT( AAFRESULT_BADINDEX );
	}

	*result = _definitionParams[index].GetCodecDefID();
}

void AxImplPlugin::GetPluginDescriptorID(
		aafUID_t *result )
{
	*result = _pluginDescriptorAUID;
}


void AxImplPlugin::GetIndexedDefinitionObject(
		aafUInt32 index,
		IAAFDictionary *dict, 
		IAAFDefObject **def )
{
	if ( index >= _definitionParams.size() ) {
		CHECK_HRESULT( AAFRESULT_BADINDEX );
	}

	IAAFDictionarySP spDict( dict );
	AxDictionary axDict( spDict );

	// FIXME - The should be parameterized.  Not all plugins are
	// or codecs.
	IAAFCodecDefSP spCodecDef;
	AxCreateInstance( spDict, spCodecDef );
	AxCodecDef axCodecDef( spCodecDef );

	// Initialize the new object.
	axCodecDef.Initialize( _definitionParams[index].GetCodecDefID(),
						   _definitionParams[index].GetName(),
						   _definitionParams[index].GetDesc() );

	// Set class type.
	IAAFSmartPointer<IAAFClassDef> spFileClass =
		axDict.LookupClassDef( _definitionParams[index].GetEssenceDescriptorAUID() );
	axCodecDef.SetFileDescriptorClass( spFileClass );
	
	// Add an essence kind.
	axCodecDef.AddEssenceKind(
		axDict.LookupDataDef( _definitionParams[index].GetDataDefID() ) );

	// Finally get an IAAFDefObject pointer to return to the caller.
	// The easiest way to get a bare pointer out of the smart pointer
	// is to simply call QueryInterface.
	axCodecDef.QueryInterface( def );
}

void AxImplPlugin::CreateDescriptor(
		IAAFDictionary *dict,
		IAAFPluginDef **desc )
{
	IAAFDictionarySP spDict( dict );

	IAAFNetworkLocatorSP spNetLoc;
	AxCreateInstance( spDict, spNetLoc );
	AxNetworkLocator axNetLoc( spNetLoc );

	axNetLoc.Initialize();
	axNetLoc.SetPath( _descriptorParams.GetManufacturerLocatorPath() );

	IAAFPluginDefSP spPluginDef;
	AxCreateInstance( spDict, spPluginDef );
	AxPluginDef axPluginDef( spPluginDef );

	axPluginDef.Initialize( _descriptorParams.GetCodecDefID(),
							_descriptorParams.GetCodecName(),
							_descriptorParams.GetCodecDesc() );
	axPluginDef.SetCategoryClass( _descriptorParams.GetCategoryClassAUID() );
	axPluginDef.SetPluginVersionString( _descriptorParams.GetVersionString() );
	axPluginDef.SetManufacturerID( _descriptorParams.GetManufacturerID() );
	axPluginDef.SetPluginManufacturerName( _descriptorParams.GetManufacturerName() );
	axPluginDef.SetIsSoftwareOnly( _descriptorParams.GetIsSoftwareOnly() );
	axPluginDef.SetIsAccelerated( _descriptorParams.GetIsAccellerated() );
	axPluginDef.SetSupportsAuthentication( _descriptorParams.GetSupportsAuthentication() );
	axPluginDef.SetManufacturerInfo( axNetLoc );	

	// Use QueryInterface to get a bare pointer out of the smart pointer.
	axPluginDef.QueryInterface( desc );
}

