#ifndef __AxPluginRegistry_h__
#define __AxPluginRegistry_h__

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

#include <AAFCOMPlatformTypes.h>

class AxPluginFctryPrtcl;

#include <memory>
#include <vector>


// AxPluginRegistry is a singleton that is responsible for
// managing the list of components that are implemented by a single
// library.
//
// All components implemented by a single library are responsible for
// registering an implementatino of AxPluginFctryPrtcl with the
// registry at init time (i.e. when the library is loaded.
//
// To avoid memory leaks, component implementors should be sure to
// call RemoveFactory() at fini time (i.e., when the library is unloaded).
//
// When the last factory object is removed, the single instance of 
// AxPluginRegistry will destoy itself (no leaked memory!).  Note,
// subsequent calls to GetInstance() are safe - a new singleton instance
// will be created.
//
// This registration can happen where ever the library and component
// implementor sees fit.  The most convenient is probably in the constructor
// of a static global object that accompanies the component implementation.
// The destructor of the static global is also a logical place to call
// RemoveFactory().
// 
// Please refer the UML.

class AxPluginRegistry {
public:
	
	// It's a singleton, hence...
	static AxPluginRegistry& GetInstance();
		
	// Add and remove AxPluginFctryPrtcl implementations.   Obviously,
	// the clsid passed to RemoveFactory must match the CLSID of the
	// factory object you wish to remove.
	// clsid uniqueness is enfornced by an assertion.
	void AddFactory( std::auto_ptr<AxPluginFctryPrtcl> factory );
	void RemoveFactory( const CLSID& clsid ); 

	// Return the number of registered factory objects.
	int GetClassCount();

	// Return the id of the factory with specified index.
	HRESULT GetClassObjectID( int index, CLSID *pClassID );

	// Look up the factory with rid "cslid".  Create the underlying
	// COM factory and return a pointer to the interface with id "riid".
	HRESULT GetClassObject( const CLSID& clsid, REFIID riid, LPVOID* ppv );

private:

	// It's a singleton... nobody should access these.
	AxPluginRegistry();
	~AxPluginRegistry();

	// Prohibited
	AxPluginRegistry( const AxPluginRegistry& other );
	AxPluginRegistry& operator=( const AxPluginRegistry& rhs );

	// Returns null if not found
	std::vector<AxPluginFctryPrtcl*>::iterator Find( const CLSID& clsid );

	// The registery will de-initialize (i.e. "Fini") automatically when the
	// the last factory object is removed.
	void Fini();
		
	std::vector<AxPluginFctryPrtcl*> _factoryV;
	
	static AxPluginRegistry* _instance;
};


#endif
