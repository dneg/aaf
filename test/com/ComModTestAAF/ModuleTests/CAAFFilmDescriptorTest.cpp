// @doc INTERNAL
// @com This file implements the module test for CAAFFilmDescriptor

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/




#include "CAAFFilmDescriptor.h"
#include "CAAFFilmDescriptor.h"
#ifndef __CAAFFilmDescriptor_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFFilmDescriptor; // generated


HRESULT CAAFFilmDescriptor::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFFilmDescriptor *pObject = NULL;

  try
    {
      // Attempt to create an AAFFilmDescriptor.
      hr =  CoCreateInstance(
                             CLSID_AAFFilmDescriptor,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFFilmDescriptor, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFFilmDescriptor::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFFilmDescriptor, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFFilmDescriptor, ...);" <<
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
      cerr << "CAAFFilmDescriptor::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}




























