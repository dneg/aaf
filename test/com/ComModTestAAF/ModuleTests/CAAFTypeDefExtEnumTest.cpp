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






#include "CAAFTypeDefExtEnum.h"
#include "CAAFTypeDefExtEnum.h"
#ifndef __CAAFTypeDefExtEnum_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefExtEnum; // generated


HRESULT CAAFTypeDefExtEnum::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefExtEnum *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefExtEnum.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefExtEnum,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefExtEnum, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefExtEnum::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefExtEnum, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefExtEnum, ...);" <<
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
      cerr << "CAAFTypeDefExtEnum::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}



































