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

 


#include "CAAFParameterDef.h"
#include "CAAFParameterDef.h"
#ifndef __CAAFParameterDef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFParameterDef; // generated


HRESULT CAAFParameterDef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFParameterDef *pObject = NULL;

  try
    {
      // Attempt to create an AAFParameterDef.
      hr =  CoCreateInstance(
                             CLSID_AAFParameterDef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFParameterDef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFParameterDef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFParameterDef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFParameterDef, ...);" <<
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
      cerr << "CAAFParameterDef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





















