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





#include "CAAFTypeDefStream.h"
#include "CAAFTypeDefStream.h"
#ifndef __CAAFTypeDefStream_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefStream; // generated


HRESULT CAAFTypeDefStream::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefStream *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefStream.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefStream,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefStream, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefStream::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefStream, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefStream, ...);" <<
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
      cerr << "CAAFTypeDefStream::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}























