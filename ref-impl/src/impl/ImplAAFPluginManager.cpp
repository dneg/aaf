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

#include "aafrdli.h"

#include "AAFPlugin_i.c"


extern "C" const aafClassID_t CLSID_EnumAAFLoadedPlugins;

// These routines must be defined in an appropriate place in the 
// reference implementation shared library/DLL (Check the platform 
// dependent entrypoint...
extern "C" const char * AAFGetLibraryDirectory();
extern "C" const char * AAFGetLibraryPath();


// Dispose proc for values in pluginFiles table.
inline void	ImplAAFPluginFileDisposeProc(void *valuePtr)
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




// SEE HACK below...
const CLSID CLSID_AAFEssenceDataStream = 
{ 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };





// Common structure used to share data with the following testLibraryProc
typedef struct _AAFTestLibraryProcData
{
	ImplAAFPluginManager *plugins;
  aafTable_t	*pluginFiles;
	const char* currentLibraryPath;
} AAFTestLibraryProcData;


AAFRDLIRESULT testLibraryProc(const char* name, char isDirectory, void * userData)
{
	AAFTestLibraryProcData *pData = (AAFTestLibraryProcData *)userData;
	assert(pData && pData->plugins && pData->pluginFiles && pData->currentLibraryPath);



	if (!isDirectory && 0 != strcmp(pData->currentLibraryPath, name))
	{
		AAFRESULT rc = AAFRESULT_SUCCESS;
		ImplAAFPluginFile *pPluginFile = NULL;

		rc = ImplAAFPluginFile::CreatePluginFile(name, &pPluginFile);
		if (AAFRESULT_SUCCESS == rc)
		{
			ULONG classIDs = 0, index = 0;
			CLSID classID;

			classIDs = pPluginFile->GetClassCount();
			for (index = 0; index < classIDs; ++index)
			{
				rc = pPluginFile->GetClassObjectID(index, &classID);
				if (AAFRESULT_FAILED(rc))
					break;

				// Save the classID -> plugin file association.
				rc = TableAddValuePtr(
						pData->pluginFiles,
						&classID,
						sizeof(CLSID),
						pPluginFile,
						kAafTableDupError);
				if (AAFRESULT_FAILED(rc))
					break;

				// We are saving a reference to the plugin file so bump its reference count.
				pPluginFile->AcquireReference();

				// Now attempt to register this class id for the current plugin file.
				// HACK: Problem CAAFEssenceDataStream is NOT a plugin!
				if (!IsEqualCLSID(CLSID_AAFEssenceDataStream, classID))
				{
					rc = (pData->plugins)->RegisterPlugin(classID);
					if (AAFRESULT_FAILED(rc))
						break;
				}
			}

			pPluginFile->ReleaseReference();
			pPluginFile = NULL;
		}
	}

	// Ignore error results and continue processing plugins...
	return 0;
}


AAFRESULT ImplAAFPluginManager::Init(void)
{
	XPROTECT()
	{
		CHECK(NewUIDTable(NULL, 20, &_pluginFiles));
		CHECK(NewUIDTable(NULL, 20, &_plugins));
		CHECK(NewUIDTable(NULL, 20, &_codecDesc));
		
		//
		// Find all AAF plugin files and register them with 
		// the reference implementation.
		//		
		const char* libraryDirectory = AAFGetLibraryDirectory();
		assert(NULL != libraryDirectory);

		// Setup common data needed by testLibraryProc callback.
		AAFTestLibraryProcData testLibraryData;
		testLibraryData.plugins = this;
		testLibraryData.pluginFiles = _pluginFiles;
		testLibraryData.currentLibraryPath = AAFGetLibraryPath();
		assert(NULL != testLibraryData.currentLibraryPath);

		CHECK(AAFFindLibrary(libraryDirectory, testLibraryProc, &testLibraryData));
	}
	XEXCEPT
	{
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
