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







#include "CAAFEventMobSlot.h"
#include "CAAFEventMobSlot.h"
#ifndef __CAAFEventMobSlot_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEventMobSlot; // generated


HRESULT CAAFEventMobSlot::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEventMobSlot *pObject = NULL;

  try
    {
      // Attempt to create an AAFEventMobSlot.
      hr =  CoCreateInstance(
                             CLSID_AAFEventMobSlot,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFEventMobSlot, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFEventMobSlot::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFEventMobSlot, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFEventMobSlot, ...);" <<
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
      cerr << "CAAFEventMobSlot::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















