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

#include "AxPluginRegistry.h"

#include "AxPluginFctryPrtcl.h"

#include <assert.h>

AxPluginRegistry* AxPluginRegistry::_instance = 0;

AxPluginRegistry& AxPluginRegistry::GetInstance()
{
	if ( !_instance ) {
		_instance = new AxPluginRegistry;
	}

	assert( _instance );

	return *_instance;
}

AxPluginRegistry::AxPluginRegistry()
{}

AxPluginRegistry::~AxPluginRegistry()
{	
	// Destructor assumes all entries have been removed.  Assert this...
	assert( _factoryV.empty() );
}

void AxPluginRegistry::Fini()
{
	assert( _instance );
	delete _instance;
	_instance = 0;
}

std::vector<AxPluginFctryPrtcl*>::iterator AxPluginRegistry::Find( const CLSID& clsid )
{
	// Simple linear search for rclsid.
	std::vector<AxPluginFctryPrtcl*>::iterator iter;
	for( iter = _factoryV.begin(); iter != _factoryV.end(); ++iter ) {
		if ( (*iter)->IsClassId( clsid ) ) {
			break;
		}
	}

	return iter;
}

void AxPluginRegistry::AddFactory( std::auto_ptr<AxPluginFctryPrtcl> factory )
{
	assert( Find( factory->GetClassID() ) == _factoryV.end() );	
	
	_factoryV.push_back( factory.release() );
}

void AxPluginRegistry::RemoveFactory( const CLSID& clsid )
{
	std::vector<AxPluginFctryPrtcl*>::iterator iter;

	iter = Find( clsid );
	assert( iter != _factoryV.end() );

	delete *iter;
	_factoryV.erase( iter );

	// Self destruct if empty.
	// This ensures memory used by this (singleton) object will
	// be released when the last item is removed from the registry.
	if ( _factoryV.empty() ) {
		Fini();
	}
}

int AxPluginRegistry::GetClassCount()
{
	return _factoryV.size();
}

HRESULT AxPluginRegistry::GetClassObjectID( int index, CLSID *pClassID )
{
	if ( _factoryV.empty() ) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	else if ( index >= 0  &&  index < _factoryV.size() ) {
		*pClassID = _factoryV[index]->GetClassID();
		return S_OK;
	}
	else {
		return E_INVALIDARG;
	}
}


HRESULT AxPluginRegistry::GetClassObject( const CLSID& clsid, REFIID riid, LPVOID* ppv )
{	
	if ( !ppv ) {
		return E_INVALIDARG;
	}
	
	// First, find the clsid;
	std::vector<AxPluginFctryPrtcl*>::const_iterator iter;
	iter = Find( clsid );
	if ( iter == _factoryV.end() ) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	AxPluginFctryPrtcl* factory = *iter;

	IUnknown* comClassFactory;

	// Create the COM factory.  Note, this creates yet another factory (IClassFactory),
	// not the actual COM object that implements the plugin interface.
	HRESULT hr = factory->CreateClassObject( &comClassFactory );
	
	if ( S_OK != hr ) {
		return hr;
	}

	hr = comClassFactory->QueryInterface( riid, ppv );

	comClassFactory->Release();

	return hr;
}
