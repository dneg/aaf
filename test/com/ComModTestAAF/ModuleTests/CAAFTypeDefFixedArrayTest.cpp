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





#include "CAAFTypeDefFixedArray.h"
#include "CAAFTypeDefFixedArray.h"
#ifndef __CAAFTypeDefFixedArray_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefFixedArray; // generated


HRESULT CAAFTypeDefFixedArray::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefFixedArray *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefFixedArray.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefFixedArray,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefFixedArray, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefFixedArray::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefFixedArray, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefFixedArray, ...);" <<
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
      cerr << "CAAFTypeDefFixedArray::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





























