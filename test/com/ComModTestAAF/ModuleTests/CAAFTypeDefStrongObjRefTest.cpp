// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefStrongObjectReference

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "CAAFTypeDefStrongObjRef.h"
#include "CAAFTypeDefStrongObjRef.h"
#ifndef __CAAFTypeDefStrongObjRef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefStrongObjRef; // generated


HRESULT CAAFTypeDefStrongObjRef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefStrongObjRef *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefStrongObjRef.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefStrongObjRef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefStrongObjRef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefStrongObjRef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefStrongObjRef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefStrongObjRef, ...);" <<
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
      cerr << "CAAFTypeDefStrongObjRef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}
