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







#include "CAAFHTMLClip.h"
#include "CAAFHTMLClip.h"
#ifndef __CAAFHTMLClip_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFHTMLClip; // generated


HRESULT CAAFHTMLClip::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFHTMLClip *pObject = NULL;

  try
    {
      // Attempt to create an AAFHTMLClip.
      hr =  CoCreateInstance(
                             CLSID_AAFHTMLClip,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFHTMLClip, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFHTMLClip::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFHTMLClip, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFHTMLClip, ...);" <<
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
      cerr << "CAAFHTMLClip::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}

















