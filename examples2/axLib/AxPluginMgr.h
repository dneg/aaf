#ifndef __AxPluginMgr_h__
#define __AxPluginMgrr_h__

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

class AxPluginMgr {
public:

	// The default ctor will call the global function "AAFGetPluginManager()" to get 
	// the plugin manager COM interface.  Alternatively, use the other ctor to pass
	// a pointer to this interface.
	AxPluginMgr();
	AxPluginMgr( IAAFPluginManagerSP spIaafPluginMgr );
	virtual ~AxPluginMgr();
	
	void RegisterSharedPlugins();

	IEnumAAFLoadedPluginsSP EnumLoadedPlugins( const aafUID_t& categoryID );

	IAAFDefObjectSP CreatePluginDefinition( const aafUID_t& pluginDefID,
						  				    IAAFDictionarySP dictionary );

	void RegisterPluginFile( AxString pluginFileName );

private:
	
	// prohibited
	AxPluginMgr( const AxPluginMgr& );
	AxPluginMgr& operator=( const AxPluginMgr& );
	
	IAAFPluginManagerSP _spIaafPluginMgr;
};

#endif