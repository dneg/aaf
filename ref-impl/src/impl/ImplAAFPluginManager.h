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
