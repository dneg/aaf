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

#include <axFileGen.h>

#include <AxPluginMgr.h>
#include <AxEssence.h>
#include <AxDictionary.h>
#include <AxIterator.h>

#include "ParamMaps.h"

#include <sstream>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  RegisterPluginFile,           
  L"RegisterPluginFile",
  L"Load and register a plugin.",
  L"plugin_file_name",
  L"The plugin is referenced using it AUID, or the codec name.",
  2,
  2 ) 

RegisterPluginFile::~RegisterPluginFile()
{}

void RegisterPluginFile::Execute( const std::vector<AxString>& argv )
{
	AxString pluginFileName = argv[1];

	AxPluginMgr axPluginMgr;

	axPluginMgr.RegisterPluginFile( pluginFileName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  UpdateCodecParamMap,        
  L"UpdateCodecParamMap",
  L"Add codec AUID to the codec id parameter map. Codecs can then be access using their uid.",
  L"",
  L"",
  1,
  1 ) 

UpdateCodecParamMap::~UpdateCodecParamMap()
{}

void UpdateCodecParamMap::Execute( const std::vector<AxString>& argv )
{
	AxPluginMgr axPluginMgr;

	// Register all plugins in the ParamMap
 	AxLoadedPluginsIter axLoadedPluginsIter(
		axPluginMgr.EnumLoadedPlugins(AUID_AAFCodecDef) );

	aafUID_t nextPluginId;
	bool notAtEnd;
	for( notAtEnd = axLoadedPluginsIter.NextOne( nextPluginId );
		 notAtEnd;
		 notAtEnd = axLoadedPluginsIter.NextOne( nextPluginId ) ) {

			 std::wostringstream s;

			 s << nextPluginId;

			 CodecIDParams::GetInstance().Add( s.str().c_str(), nextPluginId );
	}

}


} // end of namespace


