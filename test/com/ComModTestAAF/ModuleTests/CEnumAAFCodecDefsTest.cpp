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







#include "CEnumAAFCodecDefs.h"
#include "CEnumAAFCodecDefs.h"
#ifndef __CEnumAAFCodecDefs_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFCodecDefs; // generated


HRESULT CEnumAAFCodecDefs::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFCodecDefs *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFCodecDefs.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFCodecDefs,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFCodecDefs, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFCodecDefs::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFCodecDefs, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFCodecDefs, ...);" <<
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
      cerr << "CEnumAAFCodecDefs::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















