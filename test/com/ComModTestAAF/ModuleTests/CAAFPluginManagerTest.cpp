// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/




#include "CAAFPluginManager.h"
#include "CAAFPluginManager.h"
#ifndef __CAAFPluginManager_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFPluginManager; // generated


HRESULT CAAFPluginManager::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFPluginManager *pObject = NULL;

  try
    {
      // Attempt to create an AAFPluginManager.
      hr =  CoCreateInstance(
                             CLSID_AAFPluginManager,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFPluginManager, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFPluginManager::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFPluginManager, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFPluginManager, ...);" <<
              endl;
          return hr;
        }

      // module-specific tests go here

      if (pObject)
        pObject->Release();
      return AAFRESULT_NOT_IMPLEMENTED;

    }
  catch (...)
    {
      cerr << "CAAFPluginManager::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}







