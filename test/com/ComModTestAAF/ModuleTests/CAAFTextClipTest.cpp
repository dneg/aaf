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







#include "CAAFTextClip.h"
#include "CAAFTextClip.h"
#ifndef __CAAFTextClip_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTextClip; // generated


HRESULT CAAFTextClip::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTextClip *pObject = NULL;

  try
    {
      // Attempt to create an AAFTextClip.
      hr =  CoCreateInstance(
                             CLSID_AAFTextClip,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTextClip, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTextClip::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTextClip, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTextClip, ...);" <<
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
      cerr << "CAAFTextClip::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





