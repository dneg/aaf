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





#include "CAAFTypeDefObjectRef.h"
#include "CAAFTypeDefObjectRef.h"
#ifndef __CAAFTypeDefObjectRef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefObjectRef; // generated


HRESULT CAAFTypeDefObjectRef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefObjectRef *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefObjectRef.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefObjectRef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefObjectRef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefObjectRef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefObjectRef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefObjectRef, ...);" <<
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
      cerr << "CAAFTypeDefObjectRef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}

















