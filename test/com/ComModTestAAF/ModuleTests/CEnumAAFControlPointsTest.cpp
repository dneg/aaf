// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CEnumAAFControlPoints.h"
#include "CEnumAAFControlPoints.h"
#ifndef __CEnumAAFControlPoints_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFControlPoints; // generated


HRESULT CEnumAAFControlPoints::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFControlPoints *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFControlPoints.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFControlPoints,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFControlPoints, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFControlPoints::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFControlPoints, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFControlPoints, ...);" <<
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
      cerr << "CEnumAAFControlPoints::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}
