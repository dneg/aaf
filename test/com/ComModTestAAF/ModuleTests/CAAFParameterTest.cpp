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


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/





 


#include "CAAFParameter.h"
#include "CAAFParameter.h"
#ifndef __CAAFParameter_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFParameter; // generated


HRESULT CAAFParameter::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFParameter *pObject = NULL;

  try
    {
      // Attempt to create an AAFParameter.
      hr =  CoCreateInstance(
                             CLSID_AAFParameter,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFParameter, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFParameter::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFParameter, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFParameter, ...);" <<
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
      cerr << "CAAFParameter::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}







	










