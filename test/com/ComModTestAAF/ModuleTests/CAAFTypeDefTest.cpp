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






#include "CAAFTypeDef.h"
#include "CAAFTypeDef.h"
#ifndef __CAAFTypeDef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDef; // generated


HRESULT CAAFTypeDef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDef *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDef.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDef, ...);" <<
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
      cerr << "CAAFTypeDef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





























