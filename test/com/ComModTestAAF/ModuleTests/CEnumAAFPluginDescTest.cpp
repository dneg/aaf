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







#include "CEnumAAFPluginDescriptors.h"
#include "CEnumAAFPluginDescriptors.h"
#ifndef __CEnumAAFPluginDescriptors_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFPluginDescriptors; // generated


HRESULT CEnumAAFPluginDescriptors::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFPluginDescriptors *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFPluginDescriptors.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFPluginDescriptors,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFPluginDescriptors, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFPluginDescriptors::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFPluginDescriptors, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFPluginDescriptors, ...);" <<
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
      cerr << "CEnumAAFPluginDescriptors::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















