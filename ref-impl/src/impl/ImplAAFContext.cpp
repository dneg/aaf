
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/






#ifndef __ImplAAFContext_h__
#include "ImplAAFContext.h"
#endif

#include "ImplAAFObjectCreation.h"

#include "AAFResult.h"

#include <assert.h>
#include "ImplAAFPluginManager.h"

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
const aafClassID_t CLSID_AAFContext = { 0x1b5d0f20, 0xfcc5, 0x11d2, { 0xbf, 0xbc, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
