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






#include "CAAFVaryingValue.h"
#include "CAAFVaryingValue.h"
#ifndef __CAAFVaryingValue_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFVaryingValue; // generated


HRESULT CAAFVaryingValue::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFVaryingValue *pObject = NULL;

  try
    {
      // Attempt to create an AAFVaryingValue.
      hr =  CoCreateInstance(
                             CLSID_AAFVaryingValue,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFVaryingValue, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFVaryingValue::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFVaryingValue, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFVaryingValue, ...);" <<
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
      cerr << "CAAFVaryingValue::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















