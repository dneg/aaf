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

/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



#include "CEnumAAFLoadedPlugins.h"
#include "CEnumAAFLoadedPlugins.h"
#ifndef __CEnumAAFLoadedPlugins_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFLoadedPlugins; // generated


HRESULT CEnumAAFLoadedPlugins::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFLoadedPlugins *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFLoadedPlugins.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFLoadedPlugins,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFLoadedPlugins, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFLoadedPlugins::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFLoadedPlugins, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFLoadedPlugins, ...);" <<
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
      cerr << "CEnumAAFLoadedPlugins::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















