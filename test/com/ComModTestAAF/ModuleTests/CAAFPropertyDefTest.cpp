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





 


#include "CAAFPropertyDef.h"
#include "CAAFPropertyDef.h"
#ifndef __CAAFPropertyDef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFPropertyDef; // generated


HRESULT CAAFPropertyDef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFPropertyDef *pObject = NULL;

  try
    {
      // Attempt to create an AAFPropertyDef.
      hr =  CoCreateInstance(
                             CLSID_AAFPropertyDef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFPropertyDef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFPropertyDef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFPropertyDef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFPropertyDef, ...);" <<
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
      cerr << "CAAFPropertyDef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















































