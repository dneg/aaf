// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/



#include "CAAFDictionary.h"
#include "CAAFDictionary.h"
#ifndef __CAAFDictionary_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFDictionary; // generated


HRESULT CAAFDictionary::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFDictionary *pObject = NULL;

  try
    {
      // Attempt to create an AAFDictionary.
      hr =  CoCreateInstance(
                             CLSID_AAFDictionary,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFDictionary, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFDictionary::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFDictionary, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFDictionary, ...);" <<
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
      cerr << "CAAFDictionary::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}



































