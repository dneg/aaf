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





#ifndef __CAAFTransition_h__
#include "CAAFTransition.h"
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTransition; // generated


HRESULT CAAFTransition::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTransition *pObject = NULL;

  try
    {
      // Attempt to create an AAFTransition.
      hr =  CoCreateInstance(
                             CLSID_AAFTransition,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTransition, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTransition::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTransition, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTransition, ...);" <<
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
      cerr << "CAAFTransition::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}



	 


	//@comm Replaces part of omfsTransitionGetInfo


	//@comm Replaces part of omfsTransitionGetInfo







