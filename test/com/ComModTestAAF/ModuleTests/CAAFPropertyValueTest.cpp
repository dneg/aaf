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






#include "CAAFPropertyValue.h"
#include "CAAFPropertyValue.h"
#ifndef __CAAFPropertyValue_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFPropertyValue; // generated


HRESULT CAAFPropertyValue::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFPropertyValue *pObject = NULL;

  try
    {
      // Attempt to create an AAFPropertyValue.
      hr =  CoCreateInstance(
                             CLSID_AAFPropertyValue,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFPropertyValue, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFPropertyValue::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFPropertyValue, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFPropertyValue, ...);" <<
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
      cerr << "CAAFPropertyValue::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}








