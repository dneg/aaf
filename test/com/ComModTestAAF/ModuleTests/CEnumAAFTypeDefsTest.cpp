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

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/








#include "CEnumAAFTypeDefs.h"
#include "CEnumAAFTypeDefs.h"
#ifndef __CEnumAAFTypeDefs_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFTypeDefs; // generated


HRESULT CEnumAAFTypeDefs::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFTypeDefs *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFTypeDefs.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFTypeDefs,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFTypeDefs, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFTypeDefs::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFTypeDefs, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFTypeDefs, ...);" <<
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
      cerr << "CEnumAAFTypeDefs::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















