//@doc
//@class    AAFPluginManager | Implementation class for AAFPluginManager
#ifndef __ImplAAFPluginManager_h__
#define __ImplAAFPluginManager_h__


/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\************************************************/



class ImplEnumAAFLoadedPlugins;
class ImplAAFPluginDescriptor;
class ImplAAFDictionary;
class ImplAAFDefObject;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "AAFPlugin.h"
#include "aafTable.h"

class ImplAAFDictionary;

class ImplAAFPluginManager : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPluginManager ();

protected:
  virtual ~ImplAAFPluginManager ();

public:


  //****************
  // EnumLoadedPlugins()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumLoadedPlugins
        (// @parm [in] Which category ID do we want to enumerate
         aafUID_t  categoryID,

         // @parm [out,retval] Loaded Plugin Enumeration
         ImplEnumAAFLoadedPlugins ** ppEnum);



public:
	//Toolkit internal only
static ImplAAFPluginManager *GetPluginManager(void);

// Create an object contained within one of the loaded plugin files.
AAFRESULT CreateInstance(
			REFCLSID rclsid, 
			IUnknown* pUnkOuter, 
			REFIID riid, 
			void ** result);


AAFRESULT GetPluginInstance(
			aafUID_t		pluginID,
			IAAFPlugin		**result);

AAFRESULT MakeCodecFromEssenceDesc(
			aafUID_t		essenceDesc,	// Stored class ID
			IAAFEssenceCodec **codec);

AAFRESULT RegisterPlugin(
			CLSID		plugin);

AAFRESULT Init(void);

// Internal to the toolkit functions
AAFRESULT GetFirstLoadedPlugin (aafTableIterate_t *iter, ImplAAFPluginDescriptor **ppDesc);
AAFRESULT GetNextLoadedPlugin (aafTableIterate_t *iter, ImplAAFPluginDescriptor **ppDesc);

AAFRESULT CreatePluginDefinition (aafUID_t  pluginDefID,
        ImplAAFDictionary * pDictionary,
        IAAFDefObject ** pPluginDesc);

private:
  aafTable_t	*_pluginFiles;
	aafTable_t	*_plugins;
	aafTable_t	*_codecDesc;
};

#endif // ! __ImplAAFPluginManager_h__

//!!! Clean out below here
// The implementation of this class should contain a structure like:
//typedef struct
//{
//	aafPluginType_t			type;
//	AAFEssenceCodec			*dispatch;
//	aafCodecMetaInfo_t		metaInfo;
//} codecTableEntry_t;
