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




#include "CAAFPluginDescriptor.h"
#include "CAAFPluginDescriptor.h"
#ifndef __CAAFPluginDescriptor_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFPluginDescriptor; // generated


HRESULT CAAFPluginDescriptor::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFPluginDescriptor *pObject = NULL;

  try
    {
      // Attempt to create an AAFPluginDescriptor.
      hr =  CoCreateInstance(
                             CLSID_AAFPluginDescriptor,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFPluginDescriptor, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFPluginDescriptor::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFPluginDescriptor, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFPluginDescriptor, ...);" <<
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
      cerr << "CAAFPluginDescriptor::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}













































































































  







