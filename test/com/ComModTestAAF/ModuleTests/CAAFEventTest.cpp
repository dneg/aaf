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







#include "CAAFEvent.h"
#include "CAAFEvent.h"
#ifndef __CAAFEvent_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEvent; // generated


HRESULT CAAFEvent::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEvent *pObject = NULL;

  try
    {
      // Attempt to create an AAFEvent.
      hr =  CoCreateInstance(
                             CLSID_AAFEvent,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFEvent, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFEvent::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFEvent, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFEvent, ...);" <<
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
      cerr << "CAAFEvent::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}


















