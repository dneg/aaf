// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefWeakObjectReference

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "CAAFTypeDefWeakObjRef.h"
#include "CAAFTypeDefWeakObjRef.h"
#ifndef __CAAFTypeDefWeakObjRef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefWeakObjRef; // generated


HRESULT CAAFTypeDefWeakObjRef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefWeakObjRef *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefWeakObjRef.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefWeakObjRef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefWeakObjRef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefWeakObjRef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefWeakObjRef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefWeakObjRef, ...);" <<
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
      cerr << "CAAFTypeDefWeakObjRef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}
