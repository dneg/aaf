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

#include "AxInit.h"

#include "AxSmartPointer.h"
#include "AxEx.h"
#include "AxHrMap.h"

#include <AAF.h>

#include <stdlib.h>

bool AxInit::once = false;

AxInit::AxInit()
{
	if ( once ) {
		throw AxEx( L"Multiple AxInit instances not permited." );
	}
	
	// Force the AxHrMap singleton to init to that it doesn't happen
	// the first time an exception is handled.
	AxHrMap::getInstance();

	const char *dllname = getenv("AX_AAF_COMAPI");

	CHECK_HRESULT( AAFLoad( dllname ) );


	IAAFPluginManagerSP	mgr;

	// Load the plugin manager 
	CHECK_HRESULT( AAFGetPluginManager(&mgr) );

	// Attempt load and register all of the plugins
	// in the shared plugin directory.
	// FIXME - How does it know where the shared plugin directory
	// is?
	CHECK_HRESULT( (mgr->RegisterSharedPlugins() ) );
 
	once = true;
}

AxInit::~AxInit()
{
	AAFUnload();

	once = false;
}
