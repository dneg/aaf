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







#include "CAAFStaticMobSlot.h"
#include "CAAFStaticMobSlot.h"
#ifndef __CAAFStaticMobSlot_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFStaticMobSlot; // generated


HRESULT CAAFStaticMobSlot::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFStaticMobSlot *pObject = NULL;

  try
    {
      // Attempt to create an AAFStaticMobSlot.
      hr =  CoCreateInstance(
                             CLSID_AAFStaticMobSlot,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFStaticMobSlot, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFStaticMobSlot::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFStaticMobSlot, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFStaticMobSlot, ...);" <<
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
      cerr << "CAAFStaticMobSlot::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





