
//=---------------------------------------------------------------------=
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






#ifndef __ImplAAFContext_h__
#include "ImplAAFContext.h"
#endif

#include "ImplAAFFile.h"
#include "ImplAAFObjectCreation.h"

#include "AAFResult.h"

#include <assert.h>
#include "ImplAAFPluginManager.h"

#include "OMObjectManager.h"


extern "C" const aafClassID_t CLSID_AAFPluginManager;



// Make private helper class a friend so that it
// may call the destructor. This helper class is used
// to ensure that the singleton context instance is 
// cleaned up properly.

class ImplAAFContextHelper
{
public:
  ImplAAFContextHelper();
  ~ImplAAFContextHelper();
};





// single instance of this class; initialized by first call to GetInstance().
/*static*/ ImplAAFContext * ImplAAFContext::_singleton = 0;

// Create an instance of the context helper. The helper's destructor will delete the context.
static ImplAAFContextHelper g_ContextHelper;




// Create and save the context.
ImplAAFContextHelper::ImplAAFContextHelper()
{
}

// Cleanup the context singleton if one exists.
ImplAAFContextHelper::~ImplAAFContextHelper()
{
  if (ImplAAFContext::_singleton)
  {
    delete ImplAAFContext::_singleton;
  }
}


/*static*/ ImplAAFContext * ImplAAFContext::GetInstance ()
{
  if (! _singleton)
  {
    _singleton = new ImplAAFContext;
    assert(_singleton);
    if (_singleton)
      _singleton->InitPluginManager(); 
  }
  return _singleton;
}

ImplAAFContext::ImplAAFContext ()
{
  // There Can Be Only One!
  assert(NULL == _singleton);
  _singleton = this;

  _plugins = NULL;
  OMObjectManager::initialize();
  ImplAAFFile::registerFactories();
}

ImplAAFContext::~ImplAAFContext ()
{
  if (_plugins)
  {
    _plugins->ReleaseReference();
    _plugins = 0;
  }
  
  // Thare Can Be Only One!
  _singleton = 0;
  ImplAAFFile::removeFactories();
  OMObjectManager::finalize();
}

void ImplAAFContext::InitPluginManager (void)
{
  if(_plugins == NULL)
  {
    _plugins = (ImplAAFPluginManager *)CreateImpl(CLSID_AAFPluginManager);
    assert(NULL != _plugins);
    if (_plugins)
      _plugins->Init();
  }
}

class ImplAAFPluginManager *ImplAAFContext::GetPluginManager (void)
{
  if (_plugins)
    _plugins->AcquireReference();
  return(_plugins);
}

// extern "C" const aafClassID_t CLSID_AAFContext;
// CLSID for AAFContext
// {1B5D0F20-FCC5-11d2-BFBC-00104BC9156D}
// const aafClassID_t CLSID_AAFContext = { 0x1b5d0f20, 0xfcc5, 0x11d2, { 0xbf, 0xbc, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
