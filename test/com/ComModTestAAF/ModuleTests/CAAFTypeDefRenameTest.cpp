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





#include "CAAFTypeDefRename.h"
#include "CAAFTypeDefRename.h"
#ifndef __CAAFTypeDefRename_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefRename; // generated


HRESULT CAAFTypeDefRename::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefRename *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefRename.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefRename,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefRename, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefRename::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefRename, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefRename, ...);" <<
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
      cerr << "CAAFTypeDefRename::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}

















