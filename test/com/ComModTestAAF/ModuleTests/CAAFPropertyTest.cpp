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







#include "CAAFProperty.h"
#include "CAAFProperty.h"
#ifndef __CAAFProperty_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFProperty; // generated


HRESULT CAAFProperty::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFProperty *pObject = NULL;

  try
    {
      // Attempt to create an AAFProperty.
      hr =  CoCreateInstance(
                             CLSID_AAFProperty,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFProperty, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFProperty::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFProperty, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFProperty, ...);" <<
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
      cerr << "CAAFProperty::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}








