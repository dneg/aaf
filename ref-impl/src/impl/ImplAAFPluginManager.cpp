/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/

#ifndef __ImplEnumAAFLoadedPlugins_h__
#include "ImplEnumAAFLoadedPlugins.h"
#endif

#ifndef __ImplAAFPluginManager_h__
#include "ImplAAFPluginManager.h"
#endif

#include "ImplAAFSession.h"

#include <assert.h>
#include <string.h>

#include "aafUtils.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "aafErr.h"

const CLSID CLSID_AAFWaveCodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const CLSID CLSID_AAFEssenceFileContainer = { 0xa7337030, 0xc103, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

ImplAAFPluginManager::ImplAAFPluginManager ()
{}


ImplAAFPluginManager::~ImplAAFPluginManager ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginManager::EnumLoadedPlugins (
      aafUID_t  /*categoryID*/,
      ImplEnumAAFLoadedPlugins ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT ImplAAFPluginManager::Init(void)
{
	XPROTECT()
	{
		CHECK(NewUIDTable(NULL, 20, &_plugins));
		CHECK(NewUIDTable(NULL, 20, &_codecDesc));
		CHECK(RegisterPlugin(CLSID_AAFEssenceFileContainer));
		CHECK(RegisterPlugin(CLSID_AAFWaveCodec));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

ImplAAFPluginManager *ImplAAFPluginManager::GetPluginManager()
{
	return(ImplAAFSession::GetInstance()->GetPluginManager());
}

AAFRESULT ImplAAFPluginManager::GetPluginInstance(
			aafUID_t		pluginID,
			IAAFPlugin		**result)
{
	CLSID		codecCLSID;
	aafBool		found;

	XPROTECT()
	{
		TableUIDLookupBlock(_plugins, pluginID, sizeof(CLSID), &codecCLSID, &found);
		if(!found)
			return(AAFRESULT_CODEC_INVALID);
		CHECK(CoCreateInstance(codecCLSID,
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IAAFPlugin, 
			(void **)result));
	}
	XEXCEPT
	XEND

	return(AAFRESULT_SUCCESS);
}


AAFRESULT ImplAAFPluginManager::MakeCodecFromEssenceDesc(
			aafUID_t		essenceDescriptor,	// Stored classID
			IAAFEssenceCodec **codec)
{
	CLSID		codecCLSID;
	aafBool		found;

	XPROTECT()
	{
		TableUIDLookupBlock(_codecDesc, essenceDescriptor, sizeof(CLSID), &codecCLSID, &found);
		if(!found)
			return(AAFRESULT_CODEC_INVALID);
		CHECK(CoCreateInstance(codecCLSID,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFEssenceCodec, 
               (void **)codec));
	}
	XEXCEPT
	XEND

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPluginManager::RegisterPlugin(CLSID pluginClass)
{
	IAAFPlugin	*plugin			= NULL;
	IAAFEssenceCodec	*codec	= NULL;
	aafUID_t	uid;

	XPROTECT()
	{
		CHECK(CoCreateInstance(pluginClass,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFPlugin, 
               (void **)&plugin));
		CHECK(plugin->GetPluginID(&uid));
		CHECK(TableAddUIDBlock(
				_plugins,
				uid,
				&pluginClass,
				sizeof(CLSID),
				kAafTableDupError));
		if(plugin->QueryInterface(IID_IAAFEssenceCodec, (void **)&codec) == AAFRESULT_SUCCESS)
		{
			CHECK(codec->GetEssenceDescriptorID(&uid));
			CHECK(TableAddUIDBlock(_codecDesc, uid, &pluginClass, sizeof(CLSID), kAafTableDupError));
			codec->Release();
			codec = NULL;
		}
		plugin->Release();
		plugin = NULL;
	}
	XEXCEPT
	{
		if(codec != NULL)
			plugin->Release();
		if(codec != NULL)
			plugin->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;

}
