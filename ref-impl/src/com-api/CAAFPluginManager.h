//@doc
//@class    AAFPluginManager | Implementation class for AAFPluginManager
#ifndef __CAAFPluginManager_h__
#define __CAAFPluginManager_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Initial Developer of the Original Code of this file and the 
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif







#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFPluginManager
  : public IAAFPluginManager,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPluginManager (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPluginManager ();

public:


  //***********************************************************
  //
  // RegisterSharedPlugins()
  //
  // Attempts to load and register all of the AAF plugin files found
  // in shared installation directory.
  //
  STDMETHOD (RegisterSharedPlugins)
     ();



  //***********************************************************
  //
  // RegisterPluginDirectory()
  //
  // Attempts to load and register all of the AAF plugin files found
  // in the given directory.
  //
  STDMETHOD (RegisterPluginDirectory) (
    // Pointer to the name of directory to look for plugins. 
    /*[in]*/ aafCharacter_constptr  directoryName);


  //***********************************************************
  //
  // RegisterPluginFile()
  //
  // Attempts to load and register all of the AAF plugins found
  // in the given file.
  //
  STDMETHOD (RegisterPluginFile) (
    // Pointer to the name of plugin file to register. 
    /*[in]*/ aafCharacter_constptr  fileName);


  //***********************************************************
  //
  // EnumLoadedPlugins()
  //
  // Returns an enumerator which enumerates over all of the loaded
  // pluigin choices through the *ppEnum argument.  The returned
  // enumerator is AddRef()ed before it is returned.
  // 
  // Succeeds if all of the following are true:
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - if ppEnum is null.
  //
  STDMETHOD (EnumLoadedPlugins) (
    // Which category ID do we want to enumerate
    /*[in, ref]*/ aafUID_constref  categoryID,

    // Loaded Plugin Enumeration
    /*[out,retval]*/ IEnumAAFLoadedPlugins ** ppEnum);


  //***********************************************************
  //
  // CreatePluginDefinition()
  //
  // Given a plugin definition ID, find a plugin and manufactures a
  // plugin descriptor of the correct class for this plugin, filling
  // in the values, and returning the definition through the
  // *pPluginDesc argument.  The returned definition is AddRef()ed
  // before it is returned.  You must call QueryInterface on the
  // result in order to find the correct interface, and are
  // responsible for adding the definition to the correct place in the
  // dictionary, as well as preventing duplicates. The resulting
  // definiton has the plugin descriptor already attached.
  // 
  // Succeeds if all of the following are true:
  // - the pPluginDesc pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - if pPluginDesc is null.
  //
  STDMETHOD (CreatePluginDefinition) (
    // Which plugin definition do you want to create
    /*[in, ref]*/ aafUID_constref  pluginDefID,

    // The dictionary of the file where the descriptor is to be created
    /*[in]*/ IAAFDictionary * pDictionary,

    // The interface of the returned definition
    /*[out]*/ IAAFDefObject**  ppPluginDef);


  //***********************************************************
  //
  // CreateInstance()
  //
  // Create an object contained within one of the loaded plugin
  // files.
  // 
  // Succeeds if all of the following are true:
  // - the rclsid was found by the plugin manager
  // - the interface corresponding to riid is supported by the plugin
  // - if pUnkOuter is not NULL and given plugin supports aggregation
  //   and riid must be a reference to IID_IUnknown.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // E_INVALIDARG
  //   - if ppPlugin is null.
  //   - pUnkOuter is not NULL and riid is not IID_IUnknown.
  //
  // AAFRESULT_EXTENSION_NOT_FOUND
  //   - The given rclsid could not be found by the Plugin Manager.
  //
  // E_NOINTERFACE
  //   - if initial interface given by riid is not supported by the
  //     plugin.
  //
  STDMETHOD (CreateInstance) (
    // The class id of the plugin object do you want to create
    /*[in]*/ REFCLSID  rclsid,

    // The controlling unknown of the new instance
    /*[in]*/ IUnknown *  pUnkOuter,

    // The IID of the initialial interface for the new plugin instance
    /*[in]*/ REFIID  riid,

    // The IID of the initialial interface for the new plugin instance
    /*[out,iid_is(riid)]*/ void **  ppPlugin);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFPluginManager_h__


