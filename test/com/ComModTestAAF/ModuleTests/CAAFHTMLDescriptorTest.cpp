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







#include "CAAFHTMLDescriptor.h"
#include "CAAFHTMLDescriptor.h"
#ifndef __CAAFHTMLDescriptor_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFHTMLDescriptor; // generated


HRESULT CAAFHTMLDescriptor::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFHTMLDescriptor *pObject = NULL;

  try
    {
      // Attempt to create an AAFHTMLDescriptor.
      hr =  CoCreateInstance(
                             CLSID_AAFHTMLDescriptor,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFHTMLDescriptor, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFHTMLDescriptor::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFHTMLDescriptor, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFHTMLDescriptor, ...);" <<
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
      cerr << "CAAFHTMLDescriptor::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





