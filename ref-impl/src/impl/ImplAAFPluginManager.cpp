/************************************************\
*	                                               *
* Advanced Authoring Format                      *
*                                                *
* Copyright (c) 1998-1999 Avid Technology, Inc.  *
* Copyright (c) 1998-1999 Microsoft Corporation  *
*                                                *
\************************************************/

#ifndef __ImplAAFPluginManager_h__
#include "ImplAAFPluginManager.h"
#endif

#ifndef __ImplEnumAAFLoadedPlugins_h__
#include "ImplEnumAAFLoadedPlugins.h"
#endif

#ifndef __ImplAAFPluginFile_h__
#include "ImplAAFPluginFile.h"
#endif


#include "ImplAAFContext.h"
#include "ImplAAFDictionary.h"

#include <assert.h>
#include <string.h>

#include "AAFUtils.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

#include "AAFPlugin_i.c"


extern "C" const aafClassID_t CLSID_EnumAAFLoadedPlugins;



// Dispose proc for values in pluginFiles table.
static void	ImplAAFPluginFileDisposeProc(void *valuePtr)
{
  if (valuePtr)
	{
		ImplAAFPluginFile *pPluginFile = (ImplAAFPluginFile *)valuePtr;
    pPluginFile->ReleaseReference();
	}
}


ImplAAFPluginManager::ImplAAFPluginManager () :
  _pluginFiles(0),
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

  if (_pluginFiles)
  {
		// We need to cleanup the elements in the list manually.
		aafBool found = AAFFalse;
		aafTableIterate_t iter = {0};
	  AAFRESULT status = ::TableFirstEntry(_pluginFiles, &iter, &found);
		while((AAFRESULT_SUCCESS == status) && found)
		{
			::ImplAAFPluginFileDisposeProc(iter.valuePtr);
			iter.valuePtr = NULL;

			::TableNextEntry(&iter,&found);
		}


	  TableDispose(_pluginFiles);
	  _pluginFiles = 0;
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
	// SEE HACK below...
	const CLSID CLSID_AAFEssenceDataStream = 
	{ 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


	// Currently the names of the standard plugins are hardcoded.
	// The next version will search a "known directory" for all
	// for all files where ImplAAFPluginFile::CreatePluginFile succeeds
	// (i.e. all files that can be loaded as a shared library and 
	// contains all of the expected exported functions.)
	const char *pluginFileNames[] = 
	{
#if defined(_WIN32) || defined(WIN32)
		"AAFPGAPI.DLL",
		"AAFINTP.DLL",
#elif defined(macintosh) || defined(_MAC)
		"AAFPGAPI.DLL (PPC)",
		"AAFINTP.DLL (PPC)",
#else
		"aafpgapi.so",
		"aafintp.so",
#endif
		NULL
	};
	ImplAAFPluginFile *pPluginFile = NULL;


	XPROTECT()
	{
		CHECK(NewUIDTable(NULL, 20, &_pluginFiles));
		CHECK(NewUIDTable(NULL, 20, &_plugins));
		CHECK(NewUIDTable(NULL, 20, &_codecDesc));
		//!!! Here is where we should check the registry in a loop
		// looking for plugins to load
		// for each plugin found
		//	RegisterPlugin()
		
		for (int pluginIndex = 0; pluginFileNames[pluginIndex]; ++pluginIndex)
		{
			ULONG classIDs = 0, index = 0;
			CLSID classID;
			CHECK(ImplAAFPluginFile::CreatePluginFile(pluginFileNames[pluginIndex], &pPluginFile));
			classIDs = pPluginFile->GetClassCount();
			for (index = 0; index < classIDs; ++index)
			{
				CHECK(pPluginFile->GetClassObjectID(index, &classID));

				// Save the classID -> plugin file association.
				CHECK(TableAddValuePtr(
						_pluginFiles,
						&classID,
						sizeof(CLSID),
						pPluginFile,
						kAafTableDupError));
				
				// We are saving a reference to the plugin file so bump its reference count.
				pPluginFile->AcquireReference();

				// Now attempt to register this class id for the current plugin file.
				// HACK: Problem CAAFEssenceDataStream is NOT a plugin!
				if (!IsEqualCLSID(CLSID_AAFEssenceDataStream, classID))
					CHECK(RegisterPlugin(classID));
			}

			pPluginFile->ReleaseReference();
			pPluginFile = NULL;
		}

//		CHECK(RegisterPlugin(CLSID_AAFEssenceFileContainer));
//		CHECK(RegisterPlugin(CLSID_AAFWaveCodec));
//		CHECK(RegisterPlugin(CLSID_AAFBasicInterp));
	}
	XEXCEPT
	{
		if (pPluginFile)
			pPluginFile->ReleaseReference();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

ImplAAFPluginManager *ImplAAFPluginManager::GetPluginManager()
{
  //	ImplAAFPluginManager	*mgr;

	return(ImplAAFContext::GetInstance()->GetPluginManager());
}


AAFRESULT ImplAAFPluginManager::CreateInstance(REFCLSID rclsid, IUnknown* pUnkOuter, REFIID riid, void ** result)
{
	AAFRESULT rc = AAFRESULT_SUCCESS;
	ImplAAFPluginFile *pPluginFile = (ImplAAFPluginFile *)::TableLookupPtr(_pluginFiles, (void *)&rclsid);

	if (NULL == pPluginFile)
		rc = AAFRESULT_CODEC_INVALID;
	else
	{
		// The reference implementation must be "self-contained". We do
		// not want any user supplied classes to be created and used
		// instead on one our built-in classes.
		//
		// The simplest change is to just simulate a call to 
		// CoCreateInstance:
		//
		// This code is invoked within the current module so we
		// should just be able to call the DllGetClassObject entry point
		// instead of calling CoCreateInstance and searching the 
		// registry.
		IClassFactory *pFactory = NULL;
		rc = pPluginFile->GetClassObject(rclsid, IID_IClassFactory, (void **)&pFactory);
		if (SUCCEEDED(rc))
		{
			rc = pFactory->CreateInstance(pUnkOuter, riid, (void **)result);
			pFactory->Release();
		}
	}

	return rc;
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
		CHECK(CreateInstance(codecCLSID,
			NULL, 
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
		CHECK(CreateInstance(codecCLSID,
               NULL, 
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
		CHECK(CreateInstance(pluginClass,
               NULL, 
               IID_IAAFPlugin, 
               (void **)&plugin));
		CHECK(plugin->GetIndexedDefinitionID(/*!!!*/0, &uid));
		CHECK(TableAddUIDBlock(
				_plugins,
				uid,
				&pluginClass,
				sizeof(CLSID),
				kAafTableDupError));
		if(plugin->QueryInterface(IID_IAAFEssenceCodec, (void **)&codec) == AAFRESULT_SUCCESS)
		{
			CHECK(codec->GetEssenceDescriptorID(&uid));

			CHECK(TableAddUIDBlock(
				_codecDesc,
				uid,
				&pluginClass,
				sizeof(CLSID),
				kAafTableDupError));
			
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

AAFRESULT
    ImplAAFPluginManager::CreatePluginDefinition (aafUID_t  pluginDefID,
        ImplAAFDictionary * pDictionary,
        IAAFDefObject	** ppPluginDef)
{
	IAAFPlugin				*plugin = NULL;
	IAAFPluginDescriptor	*desc = NULL;
	IUnknown				*iUnk = NULL;
	IAAFDictionary			*iDictionary = NULL;
	aafInt32				n, count;
	aafUID_t				testID;
	aafBool					found;

	XPROTECT()
	{
		iUnk = static_cast<IUnknown *> (pDictionary->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFDictionary, (void **)&iDictionary));
		CHECK(GetPluginInstance(pluginDefID, &plugin));
		CHECK(plugin->GetNumDefinitions (&count));
		found = AAFFalse;
		for(n = 0; n < count && !found; n++)
		{
			CHECK(plugin->GetIndexedDefinitionID (n, &testID));
			if(EqualAUID(&pluginDefID, &testID))
			{
				CHECK(plugin->GetIndexedDefinitionObject(n, iDictionary, ppPluginDef));
				CHECK(plugin->CreateDescriptor(iDictionary, &desc));
				CHECK((*ppPluginDef)->AppendPluginDescriptor (desc));
				CHECK(iDictionary->RegisterPluginDescriptor(desc));

				desc->Release();
				desc = NULL;
				found = AAFTrue;
			}
		}
		//!!!Assert found
		plugin->Release();
		plugin = NULL;
		iDictionary->Release();
		iDictionary = NULL;
	}
	XEXCEPT
	{
		if(desc)
			desc->Release();
		if(plugin)
			plugin->Release();
		if(iDictionary)
			iDictionary->Release();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}
