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





#include "CAAFTypeDefVariableArray.h"
#include "CAAFTypeDefVariableArray.h"
#ifndef __CAAFTypeDefVariableArray_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefVariableArray; // generated


HRESULT CAAFTypeDefVariableArray::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefVariableArray *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefVariableArray.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefVariableArray,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefVariableArray, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefVariableArray::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefVariableArray, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefVariableArray, ...);" <<
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
      cerr << "CAAFTypeDefVariableArray::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}
