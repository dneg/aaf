//@doc
//@class    AAFPluginManager | Implementation class for AAFPluginManager
#ifndef __ImplAAFPluginManager_h__
#define __ImplAAFPluginManager_h__


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



class ImplEnumAAFLoadedPlugins;
class ImplAAFPluginDef;
class ImplAAFDictionary;
class ImplAAFDefObject;
class ImplAAFSourceMob;
class ImplAAFEssenceStream;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


#include "AAFPlugin.h"
#include "aafTable.h"

// Implemenent a visitor pattern for plugins with a given definition id.
// Used with ForEachPluginWithDefinitionDo.
typedef aafBool	(*AAFDOTOPLUGINWITHDEFINTIONPROC)(
  aafUID_constref defID, 
  REFCLSID clsid, 
  void *refCon);



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

// Implemenent a visitor pattern for plugins with a given definition id.
AAFRESULT ForEachPluginWithDefinitionDo(
  aafUID_constref defID, 
  AAFDOTOPLUGINWITHDEFINTIONPROC proc, 
  void *refCon);

bool FindPluginFromDefintion(
  aafUID_constref	pluginID,
  CLSID& clsid);

// Create an object contained within one of the loaded plugin files.
AAFRESULT CreateInstance(
			REFCLSID rclsid, 
			IUnknown* pUnkOuter, 
			REFIID riid, 
			void ** result);

AAFRESULT CreateInstanceFromDefinition(
			aafUID_constref		pluginID,
			IUnknown* pUnkOuter, 
			REFIID riid, 
			void ** result);

AAFRESULT GetPluginInstance(
			aafUID_t		pluginID,
			IAAFPlugin		**result);

AAFRESULT MakeCodecFromEssenceDesc(
			ImplAAFSourceMob		*fileMob,
			IAAFEssenceStream		*stream,
			IAAFEssenceCodec		**codec);

// Attempt to register all of the plugin files in the installation directory.
AAFRESULT RegisterSharedPlugins(void);

// Attempt to register all of the plugin files in the given directory.
AAFRESULT RegisterPluginDirectory(const char *directoryName);
AAFRESULT RegisterPluginDirectory(aafCharacter_constptr wdirectoryName);

// Attempt to register all of the plugins in the given file.
AAFRESULT RegisterPluginFile(const char *fileName);
AAFRESULT RegisterPluginFile(aafCharacter_constptr wfileName);

// Associate a factory object with a code class id.
AAFRESULT RegisterPluginFactory(
			REFCLSID rclsid,
      IClassFactory *pFactory);

AAFRESULT RegisterPlugin(
			CLSID		plugin);

// Attempt to unregister all plugins. This may fail if any plugins
// are still loaded. This method should only be called after all
// files have been closed, all client created plugins have been
// released but before AAFUnload() has been called.
AAFRESULT UnregisterAllPlugins(void);

AAFRESULT Init(void);

// Internal to the toolkit functions
AAFRESULT GetFirstLoadedPlugin (aafUID_t category, aafTableIterate_t *iter, aafUID_t *pDesc);
AAFRESULT GetNextLoadedPlugin (aafUID_t category, aafTableIterate_t *iter, aafUID_t *pDesc);

AAFRESULT CreatePluginDefinition (aafUID_t  pluginDefID,
        ImplAAFDictionary * pDictionary,
        IAAFDefObject ** pPluginDesc);

private:
  aafTable_t	*_pluginFiles;
  aafTable_t  *_pluginFactories;
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
