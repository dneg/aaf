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

#include "ImplAAFContext.h"

#include <assert.h>
#include <string.h>

#include "AAFUtils.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

extern "C" const IID IID_IAAFEssenceStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
extern "C" const IID IID_IAAFEssenceCodec = { 0x3631F7A2, 0x9121, 0x11D2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
extern "C" const IID IID_IAAFEssenceSampleStream = { 0x63E12802, 0xCCE5, 0x11D2, { 0x80, 0x98, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
extern "C" const IID IID_IAAFEssenceContainer = {0xa7337031,0xc103,0x11d2,{0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f}};
extern "C" const IID IID_IAAFPlugin = {0x3631F7A4,0x9121,0x11d2,{0x80,0x88,0x00,0x60,0x08,0x14,0x3e,0x6f}};
extern "C" const IID IID_IAAFEssenceDataStream = {0xCDDB6AB1,0x98DC,0x11d2,{0x80,0x8a,0x00,0x60,0x08,0x14,0x3e,0x6f}};

const CLSID CLSID_AAFWaveCodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const CLSID CLSID_AAFEssenceFileContainer = { 0xa7337030, 0xc103, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
extern "C" const aafClassID_t CLSID_EnumAAFLoadedPlugins;

ImplAAFPluginManager::ImplAAFPluginManager () :
  _plugins(0),
	_codecDesc(0)
{}


ImplAAFPluginManager::~ImplAAFPluginManager ()
{
	// Cleanup the non-persistent data...
  if (_plugins)
  {
	  TableDispose(_plugins);
	  _plugins = 0;
  }
  if (_codecDesc)
  {
	  TableDispose(_codecDesc);
	  _codecDesc = 0;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginManager::EnumLoadedPlugins (
      aafUID_t categoryID,
      ImplEnumAAFLoadedPlugins **ppEnum)
{
	ImplEnumAAFLoadedPlugins		*theEnum = (ImplEnumAAFLoadedPlugins *)CreateImpl (CLSID_EnumAAFLoadedPlugins);
		
	XPROTECT()
	{
		CHECK(theEnum->SetCategory(&categoryID));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
			theEnum->ReleaseReference();
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFPluginManager::Init(void)
{
	XPROTECT()
	{
		CHECK(NewUIDTable(NULL, 20, &_plugins));
		CHECK(NewUIDTable(NULL, 20, &_codecDesc));
		//!!! Here is where we should check the registry in a loop
		// looking for plugins to load
		// for each plugin found
		//	RegisterPlugin()
		CHECK(RegisterPlugin(CLSID_AAFEssenceFileContainer));
		CHECK(RegisterPlugin(CLSID_AAFWaveCodec));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

ImplAAFPluginManager *ImplAAFPluginManager::GetPluginManager()
{
  //	ImplAAFPluginManager	*mgr;

	return(ImplAAFContext::GetInstance()->GetPluginManager());
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
		CHECK(plugin->GetPluggableID(&uid));
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
			codec->Release();
		if(plugin != NULL)
			plugin->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;

}

// Internal to the toolkit functions
AAFRESULT
    ImplAAFPluginManager::GetFirstLoadedPlugin (aafTableIterate_t *iter, ImplAAFPluginDescriptor **ppDesc)
{
	aafBool		found;
	AAFRESULT	status;

	if(ppDesc == NULL || iter == NULL)
		return(AAFRESULT_NULL_PARAM);


	status = TableFirstEntry(_plugins, iter, &found);
	if(!found)
		return AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???
	*ppDesc = (ImplAAFPluginDescriptor *)iter->valuePtr;

	return status;
}
AAFRESULT
    ImplAAFPluginManager::GetNextLoadedPlugin (aafTableIterate_t *iter, ImplAAFPluginDescriptor **ppDesc)
{
	aafBool		found;
	AAFRESULT	status;

	if(ppDesc == NULL || iter == NULL)
		return(AAFRESULT_NULL_PARAM);


	status = TableNextEntry(iter, &found);
	if(!found)
		return AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???
	*ppDesc = (ImplAAFPluginDescriptor *)iter->valuePtr;
	return status;
}

