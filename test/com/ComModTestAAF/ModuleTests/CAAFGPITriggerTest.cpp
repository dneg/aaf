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







#include "CAAFGPITrigger.h"
#include "CAAFGPITrigger.h"
#ifndef __CAAFGPITrigger_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFGPITrigger; // generated


HRESULT CAAFGPITrigger::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFGPITrigger *pObject = NULL;

  try
    {
      // Attempt to create an AAFGPITrigger.
      hr =  CoCreateInstance(
                             CLSID_AAFGPITrigger,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFGPITrigger, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFGPITrigger::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFGPITrigger, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFGPITrigger, ...);" <<
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
      cerr << "CAAFGPITrigger::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}










