// @doc INTERNAL
// @com This file implements the module test for CEnumAAFOperationDefs
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








#include "CEnumAAFOperationDefs.h"
#include "CEnumAAFOperationDefs.h"
#ifndef __CEnumAAFOperationDefs_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFOperationDefs; // generated


HRESULT CEnumAAFOperationDefs::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFOperationDefs *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFOperationDefs.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFOperationDefs,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFOperationDefs, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFOperationDefs::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFOperationDefs, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFOperationDefs, ...);" <<
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
      cerr << "CEnumAAFOperationDefs::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















