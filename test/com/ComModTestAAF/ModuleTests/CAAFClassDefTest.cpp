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









#include "CAAFClassDef.h"
#include "CAAFClassDef.h"
#ifndef __CAAFClassDef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFClassDef; // generated


HRESULT CAAFClassDef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFClassDef *pObject = NULL;

  try
    {
      // Attempt to create an AAFClassDef.
      hr =  CoCreateInstance(
                             CLSID_AAFClassDef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFClassDef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFClassDef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFClassDef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFClassDef, ...);" <<
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
      cerr << "CAAFClassDef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}























