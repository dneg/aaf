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



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/




#include "CAAFPulldown.h"
#include "CAAFPulldown.h"
#ifndef __CAAFPulldown_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFPulldown; // generated


HRESULT CAAFPulldown::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFPulldown *pObject = NULL;

  try
    {
      // Attempt to create an AAFPulldown.
      hr =  CoCreateInstance(
                             CLSID_AAFPulldown,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFPulldown, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFPulldown::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFPulldown, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFPulldown, ...);" <<
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
      cerr << "CAAFPulldown::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}




























