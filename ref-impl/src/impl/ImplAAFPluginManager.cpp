/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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
#include <iostream.h>
#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

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


// Comment base class for all table entry wrapper classes.
struct AAFPluginEntry
{
  virtual ~AAFPluginEntry() {};
};


// Simple wrapper class for plugin files so that 
class AAFPluginFileEntry : public AAFPluginEntry
{
public:
  AAFPluginFileEntry(ImplAAFPluginFile *pPluginFile);
  virtual ~AAFPluginFileEntry();

  // member access operators (non-const and const)
  ImplAAFPluginFile * GetPluginFile();

private:
  ImplAAFPluginFile *_pPluginFile;

  // Hide the default constructor and copy constructors to prevent
  // invalid copy operations.
  AAFPluginFileEntry();
  AAFPluginFileEntry(const AAFPluginFileEntry&);
  AAFPluginFileEntry& operator =(const AAFPluginFileEntry&);
};

// Simple wrapper class for plugin factory so that 
class AAFPluginFactoryEntry : public AAFPluginEntry
{
public:
  AAFPluginFactoryEntry(IClassFactory *pPluginFactory);
  virtual ~AAFPluginFactoryEntry();

  // member access operators (non-const and const)
  IClassFactory *GetPluginFactory();

private:
  IClassFactory *_pPluginFactory;

  // Hide the default constructor and copy constructors to prevent
  // invalid copy operations.
  AAFPluginFactoryEntry();
  AAFPluginFactoryEntry(const AAFPluginFactoryEntry&);
  AAFPluginFactoryEntry& operator =(const AAFPluginFactoryEntry&);
};




// Dispose proc for values which are AAFPluginEntry objects. 
static void	AAFPluginEntryDisposeProc(void * valuePtr)
{
  if (valuePtr)
	{
		AAFPluginEntry *pPluginEntry = static_cast<AAFPluginEntry *>(valuePtr);
		delete pPluginEntry;
	}
}



ImplAAFPluginManager::ImplAAFPluginManager () :
  _pluginFiles(0),
  _pluginFactories(0),
  _plugins(0),
	_codecDesc(0)
{}


ImplAAFPluginManager::~ImplAAFPluginManager ()
{
  
  // Make sure that we have unregistered all of the plugins.
  //
  UnregisterAllPlugins();


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


  if (_pluginFactories)
  {
    // Release all of the registered class factories. We need to do this
    // before we attempt to unload and release the pluginFiles (aka dlls)
    // that may contain the code for the class factories and 
    TableDisposeAll(_pluginFactories);
	  _pluginFactories = 0;
  }

  if (_pluginFiles)
  {
		// Release all of the plugin files.
    TableDisposeAll(_pluginFiles);
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
  AAFRESULT rc = AAFRESULT_SUCCESS;
	AAFTestLibraryProcData *pData = (AAFTestLibraryProcData *)userData;
	assert(pData && pData->plugins && pData->pluginFiles && pData->currentLibraryPath);

  //
  // If the current name is not a directory and not equal to the 
  // path this dll (the reference implementation dll) then
  // attempt to register the file and any contained plugins.
  //
	if (!isDirectory && 0 != strcmp(pData->currentLibraryPath, name))
		rc = (pData->plugins)->RegisterPluginFile(name);

	// Ignore error results and continue processing plugins...
	return 0;
}


AAFRESULT ImplAAFPluginManager::Init(void)
{
	XPROTECT()
	{
		CHECK(NewUIDTable(NULL, 20, &_pluginFiles));
    CHECK(SetTableDispose(_pluginFiles, AAFPluginEntryDisposeProc));
		CHECK(NewUIDTable(NULL, 40, &_pluginFactories));
    CHECK(SetTableDispose(_pluginFactories, AAFPluginEntryDisposeProc));
		CHECK(NewUIDTable(NULL, 40, &_plugins));
		CHECK(NewUIDTable(NULL, 40, &_codecDesc));
		
		//
		// Find all the installed AAF plugin files and register them with 
		// the reference implementation.
		//		
		//CHECK(RegisterSharedPlugins());
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


AAFRESULT ImplAAFPluginManager::CreateInstance(
  REFCLSID rclsid, 
  IUnknown* pUnkOuter, 
  REFIID riid, 
  void ** result)
{
	AAFRESULT rc = AAFRESULT_SUCCESS;
  AAFPluginFactoryEntry *pPluginFactoryEntry = NULL;
	IClassFactory *pFactory = NULL;


	// The reference implementation must be "self-contained". We do
	// not want any user supplied classes to be created and used
	// instead on one our built-in classes.

  // Check to see if the class factory has already been registered or cached.
  pPluginFactoryEntry = static_cast<AAFPluginFactoryEntry *>(::TableLookupPtr(_pluginFactories, (void *)&rclsid));
	if (pPluginFactoryEntry)
    pFactory = pPluginFactoryEntry->GetPluginFactory();

	//
  // Use the registered plugin class factory to create an instance of
  // of a plugin with the given interface pointer.
  // 
	if (NULL != pFactory)
	{
    rc = pFactory->CreateInstance(pUnkOuter, riid, (void **)result);

    pFactory->Release();
    pFactory = NULL; 
  }
  else
  { // The requested plugin could not be found!
    rc = AAFRESULT_CODEC_INVALID;
  }

	return rc;
}


// Implemenent a visitor pattern for plugins with a given definition id.
AAFRESULT ImplAAFPluginManager::ForEachPluginWithDefinitionDo(
  aafUID_constref defID, 
  AAFDOTOPLUGINWITHDEFINTIONPROC proc, 
  void *refCon)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  int count = 0;

  if (NULL == proc)
    return AAFRESULT_NULL_PARAM;

  if (NULL != _plugins)
  {
    aafTableIterate_t iter;
    aafBool found = AAFFalse;
   

    result = ::TableFirstEntryMatching(_plugins, &iter, (void *)&defID, &found);
    while (AAFRESULT_SUCCESS == result && found)
    {
      assert(NULL != iter.valuePtr);
      REFCLSID clsid = *static_cast<const CLSID *>(iter.valuePtr);
      ++count;

      // Call the provided function for each plugin that has the given
      // definition id.
      result = proc ( defID, clsid, refCon);
      if (AAFRESULT_SUCCESS != result)
        break;

      result = ::TableNextEntry(&iter, &found);
    }
  }

  // Return a more meaningful result code.
  if (0 == count && AAFRESULT_SUCCESS == result)
    result = AAFRESULT_EXTENSION_NOT_FOUND;

  return result;
}


bool ImplAAFPluginManager::FindPluginFromDefintion(
  aafUID_constref	pluginID,
  CLSID& clsid)
{
	aafBool		found = AAFFalse;

  if (NULL != _plugins)
	  TableUIDLookupBlock(_plugins, pluginID, sizeof(CLSID), &clsid, &found);

  return (AAFTrue == found);
}


bool ImplAAFPluginManager::FindPluginFromEssenceDesc(
  aafUID_constref	essenceDesc,
  CLSID& clsid)
{
	aafBool		found = AAFFalse;

  if (NULL != _codecDesc)
	  TableUIDLookupBlock(_codecDesc, essenceDesc, sizeof(CLSID), &clsid, &found);

  return (AAFTrue == found);
}



AAFRESULT ImplAAFPluginManager::CreateInstanceFromDefinition(
			aafUID_constref		pluginID,
			IUnknown* pUnkOuter,
			REFIID riid,
			void	**result)
{
	CLSID		codecCLSID;

	XPROTECT()
	{
		if(!FindPluginFromDefintion(pluginID, codecCLSID))
			return(AAFRESULT_CODEC_INVALID);
		CHECK(CreateInstance(codecCLSID,
			pUnkOuter, 
			riid, 
			(void **)result));
	}
	XEXCEPT
	XEND

	return(AAFRESULT_SUCCESS);
}



AAFRESULT ImplAAFPluginManager::GetPluginInstance(
			aafUID_t		pluginID,
			IAAFPlugin		**result)
{
	return(CreateInstanceFromDefinition(pluginID, NULL, IID_IAAFPlugin, (void **)result));
}


AAFRESULT ImplAAFPluginManager::MakeCodecFromEssenceDesc(
			aafUID_t		essenceDescriptor,	// Stored classID
			IAAFEssenceCodec **codec)
{
	CLSID		codecCLSID;


	XPROTECT()
	{
		if(!FindPluginFromEssenceDesc(essenceDescriptor, codecCLSID))
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



// Attempt to register all of the plugin files in the installation directory.
AAFRESULT ImplAAFPluginManager::RegisterSharedPlugins(void)
{
  // The current impelementation uses the directory that contains
  // the loaded and running AAF dll as the "installation directory".
  // This may change in a future version to some other directory.
	const char* libraryDirectory = AAFGetLibraryDirectory();
	assert(NULL != libraryDirectory);

  return RegisterPluginDirectory(libraryDirectory);
}


// Attempt to register all of the plugin files in the given directory.
AAFRESULT ImplAAFPluginManager::RegisterPluginDirectory(const char *directoryName)
{
  if (NULL == directoryName)
    return AAFRESULT_NULL_PARAM;

	// Setup common data needed by testLibraryProc callback.
	AAFTestLibraryProcData testLibraryData;
	testLibraryData.plugins = this;
	testLibraryData.pluginFiles = _pluginFiles;
	testLibraryData.currentLibraryPath = AAFGetLibraryPath();
	assert(NULL != testLibraryData.currentLibraryPath);

  return AAFFindLibrary(directoryName, testLibraryProc, &testLibraryData);
}

// Utility to create an ascii character string from a wide character string.
static char * NewStringFromAAFString(aafCharacter_constptr wName)
{
  char * name = NULL;
  int wNameLength = wcslen(wName);
  int nameLegth = (wNameLength * MB_CUR_MAX) + 1;
  name = new char[nameLegth];
  if (NULL != name)
  {
    size_t status = wcstombs(name, wName, wNameLength + 1);
    if (status == (size_t)-1)
    {
      delete [] name;
      name = NULL;
    }
    assert (status != (size_t)-1);
  }

  return name;
}

AAFRESULT ImplAAFPluginManager::RegisterPluginDirectory(aafCharacter_constptr wdirectoryName)
{
  AAFRESULT rc = AAFRESULT_SUCCESS;

  if (NULL == wdirectoryName)
    return AAFRESULT_NULL_PARAM;

  char *directoryName = NewStringFromAAFString(wdirectoryName);
  if (directoryName)
  {
    rc = RegisterPluginDirectory(directoryName);
    delete [] directoryName;
  }
  else
  {
    rc = AAFRESULT_NOMEMORY;
  }

  return rc;
}



// Attempt to register all of the plugins in the given file.
AAFRESULT ImplAAFPluginManager::RegisterPluginFile(const char *fileName)
{
	AAFRESULT rc = AAFRESULT_SUCCESS;
  AAFPluginFileEntry *pPluginFileEntry = NULL;
	ImplAAFPluginFile *pPluginFile = NULL;
  IClassFactory *pFactory = NULL;

  if (NULL == fileName)
    return AAFRESULT_NULL_PARAM;



  //
  // Attempt to create a plugin file object to the given 
  // fileName. If this fails then given file was not a valid
  // dll or did not support all of the required exported
  // functions.
  //
	rc = ImplAAFPluginFile::CreatePluginFile(fileName, &pPluginFile);
	if (AAFRESULT_SUCCESS == rc)
	{
		ULONG classIDs = 0, index = 0;
		CLSID classID;

    //
    // For each classID in the plugin file attempt to register
    // the corresponding class factory.
    //
		classIDs = pPluginFile->GetClassCount();
		for (index = 0; index < classIDs; ++index)
		{
			rc = pPluginFile->GetClassObjectID(index, &classID);
			if (AAFRESULT_FAILED(rc))
				break;

      // Abort if the plugin has already been registered.
      //
      if (TableIncludesKey(_pluginFiles, (void *)&classID))
      {
        rc = AAFRESULT_PLUGIN_ALREADY_REGISTERED;
        break;
      }

      // Test to see if class has been registered but not with
      // the current plugin files. This would be possible if
      // RegisterPluginFactory() had been called directly.
      // Code with a unique CLSID should not be registered from
      // more than one source so return an error.
      //
      if (TableIncludesKey(_pluginFactories, (void *)&classID))
      {
        rc = AAFRESULT_PLUGIN_ALREADY_REGISTERED;
        break;
      }

      // Create the wrapper for the plugin file.
      //
      pPluginFileEntry = new AAFPluginFileEntry(pPluginFile);
			if (NULL == pPluginFileEntry)
      {
        rc = AAFRESULT_NOMEMORY;
				break;
      }

      // Get the associated class factory by calling the DllGetClassObject
      // method in the plugin's dll.
      //
		  rc = pPluginFile->GetClassObject(classID, IID_IClassFactory, (void **)&pFactory);
			if (AAFRESULT_FAILED(rc))
      { // *** Need to output some sort of warning! ***
				break;
      }

      // Attempt to register the class factory. This may fail.
      //
      rc = RegisterPluginFactory(classID, pFactory);
      if (AAFRESULT_FAILED(rc))
      { // *** Need to output some sort of warning! ***
				break;
      }
      pFactory->Release();
      pFactory = NULL;

			// Save the classID -> plugin file association.
      //
			rc = TableAddValuePtr(
            _pluginFiles,
            &classID,
            sizeof(CLSID),
            pPluginFileEntry,
            kAafTableDupError);
			if (AAFRESULT_FAILED(rc))
      { // *** Need to output some sort of warning! ***
        if (AAFRESULT_TABLE_DUP_KEY == rc)
          rc = AAFRESULT_PLUGIN_ALREADY_REGISTERED;
				break;
      }
      pPluginFileEntry = NULL; // don't need this value.

    } // for (index = 0; index < classIDs; ++index)


    //
    // Cleanup
    //
    if (pFactory)
      pFactory->Release();

    if (pPluginFileEntry)
      delete pPluginFileEntry;

		pPluginFile->ReleaseReference();
		pPluginFile = NULL;
	}

  return rc;
}


AAFRESULT ImplAAFPluginManager::RegisterPluginFile(aafCharacter_constptr wfileName)
{
  AAFRESULT rc = AAFRESULT_SUCCESS;

  if (NULL == wfileName)
    return AAFRESULT_NULL_PARAM;

  char *fileName = NewStringFromAAFString(wfileName);
  if (fileName)
  {
    rc = RegisterPluginFile(fileName);
    delete [] fileName;
  }
  else
  {
    rc = AAFRESULT_NOMEMORY;
  }

  return rc;
}


// Associate a factory object with a code class id.
AAFRESULT ImplAAFPluginManager::RegisterPluginFactory(
			REFCLSID rclsid,
      IClassFactory *pFactory)
{
  AAFRESULT rc = AAFRESULT_SUCCESS;
  AAFPluginFactoryEntry *pPluginFactoryEntry = NULL;
  bool factoryInTable = false;


  XPROTECT()
  {
    // Create a new factory entry.
    pPluginFactoryEntry = new AAFPluginFactoryEntry(pFactory);
    if (NULL == pPluginFactoryEntry)
      RAISE(AAFRESULT_NOMEMORY);

    // Cache/register the factory.
		rc = TableAddValuePtr(
				_pluginFactories,
				(void *)(&rclsid), // key
				sizeof(CLSID), // keyLen
				pPluginFactoryEntry, // value
				kAafTableDupError);
    if (AAFRESULT_TABLE_DUP_KEY == rc)
      rc = AAFRESULT_PLUGIN_ALREADY_REGISTERED;
    CHECK(rc);

    // The entry will be cleaned up with the table.
    factoryInTable = true;
    pPluginFactoryEntry = NULL;

    // Attempt to register this plugin.
    CHECK(RegisterPlugin(rclsid));
  }
  XEXCEPT
  {
    // If we failed then we need to remove the factory from our table.
    if (factoryInTable)
      TableRemove(_pluginFactories, (void *)&rclsid);
    if (pPluginFactoryEntry)
      delete pPluginFactoryEntry;
  }
  XEND

  return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFPluginManager::RegisterPlugin(CLSID pluginClass)
{
	IAAFPlugin	*plugin			= NULL;
	IAAFEssenceCodec	*codec	= NULL;
	aafUID_t	uid;
	aafInt32 defIndex, defCount;


  // Now attempt to register this class id for the current plugin file.
  // HACK: Problem CAAFEssenceDataStream is NOT a plugin! This class
  // should be INSIDE the reference implementation not in an external
  // plugin!
	if (IsEqualCLSID(CLSID_AAFEssenceDataStream, pluginClass))
    return AAFRESULT_SUCCESS;


	XPROTECT()
	{
    // Attempt to create the plugin using the registered plugin factory.
    // This may fail.
		CHECK(CreateInstance(pluginClass,
               NULL, 
               IID_IAAFPlugin, 
               (void **)&plugin));

    //
    // NOTE: This version supports a multiple plugins per definition (id).
    //
    CHECK(plugin->GetNumDefinitions (&defCount));
    for(defIndex = 0; defIndex < defCount; ++defIndex)
    {
      CHECK(plugin->GetIndexedDefinitionID(defIndex, &uid));
      CHECK(TableAddUIDBlock(
            _plugins,
            uid,
            &pluginClass,
            sizeof(CLSID),
            kAafTableDupAddDup));
    }


		if(plugin->QueryInterface(IID_IAAFEssenceCodec, (void **)&codec) == AAFRESULT_SUCCESS)
		{
			CHECK(codec->GetEssenceDescriptorID(&uid));

			CHECK(TableAddUIDBlock(
				_codecDesc,
				uid,
				&pluginClass,
				sizeof(CLSID),
				kAafTableDupAddDup));
			
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


// Attempt to unregister all plugins. This may fail if any plugins
// are still loaded. This method should only be called after all
// files have been closed, all client created plugins have been
// released but before AAFUnload() has been called.
AAFRESULT ImplAAFPluginManager::UnregisterAllPlugins(void)
{
  if (_plugins)
  { // Clear out the items in the table that maps definitions to plugins.
    //
	  TableDisposeItems(_plugins);
  }
  if (_codecDesc)
  { // Clear out the items in the table that maps codec descriptors to plugins.
    //
	  TableDisposeItems(_codecDesc);
  }


  if (_pluginFactories)
  { // Release all of the registered class factories. We need to do this
    // before we attempt to unload and release the pluginFiles (aka dlls)
    // that may contain the code for the class factories and 
    //
    TableDisposeItems(_pluginFactories);
  }

  if (_pluginFiles)
  { // Release all of the plugin files.
    //
    TableDisposeItems(_pluginFiles);
  }

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
				CHECK((*ppPluginDef)->AppendPluginDef (desc));
				CHECK(iDictionary->RegisterPluginDef(desc));

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



AAFPluginFileEntry::AAFPluginFileEntry(ImplAAFPluginFile *pPluginFile) :
  _pPluginFile(pPluginFile)
{
  if (_pPluginFile)
    _pPluginFile->AcquireReference();
}

AAFPluginFileEntry::~AAFPluginFileEntry()
{
  if (_pPluginFile)
  {
#ifdef NDEBUG // symbol for ansi assert
    // If we are about to 
    if (1 == _pPluginFile->ReferenceCount() && S_OK != _pPluginFile->CanUnloadNow())
    {
      cerr << "WARNING: Unloading a plugin dll when the CanUnloadNow()\n"
              "callback has failed. This may not be an error if the dll\n"
              "was loaded outside of the AAF Plugin Manager.\n";
    }
#endif
    _pPluginFile->ReleaseReference();
    _pPluginFile = NULL;
  }
}

// member access operators (non-const and const)
ImplAAFPluginFile * AAFPluginFileEntry::GetPluginFile()
{
  assert(_pPluginFile);
  _pPluginFile->AcquireReference();
  return _pPluginFile;
}


AAFPluginFactoryEntry::AAFPluginFactoryEntry(IClassFactory *pPluginFactory) :
  _pPluginFactory(pPluginFactory)
{
  if (_pPluginFactory)
    _pPluginFactory->AddRef();
}

AAFPluginFactoryEntry::~AAFPluginFactoryEntry()
{
  if (_pPluginFactory)
  {
    _pPluginFactory->Release();
    _pPluginFactory = NULL;
  }
}

// member access operators (non-const and const)
IClassFactory *AAFPluginFactoryEntry::GetPluginFactory()
{
  assert(_pPluginFactory);
  _pPluginFactory->AddRef();
  return _pPluginFactory;
}

