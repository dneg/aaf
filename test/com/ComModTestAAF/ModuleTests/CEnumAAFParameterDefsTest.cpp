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








#include "CEnumAAFParameterDefs.h"
#include "CEnumAAFParameterDefs.h"
#ifndef __CEnumAAFParameterDefs_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFParameterDefs; // generated


HRESULT CEnumAAFParameterDefs::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFParameterDefs *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFParameterDefs.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFParameterDefs,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFParameterDefs, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFParameterDefs::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFParameterDefs, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFParameterDefs, ...);" <<
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
      cerr << "CEnumAAFParameterDefs::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















