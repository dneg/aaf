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





#include "CAAFTypeDefInt.h"
#include "CAAFTypeDefInt.h"
#ifndef __CAAFTypeDefInt_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefInt; // generated


HRESULT CAAFTypeDefInt::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefInt *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefInt.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefInt,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefInt, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefInt::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefInt, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefInt, ...);" <<
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
      cerr << "CAAFTypeDefInt::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}




















