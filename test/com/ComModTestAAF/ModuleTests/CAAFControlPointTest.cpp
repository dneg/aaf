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






#include "CAAFControlPoint.h"
#include "CAAFControlPoint.h"
#ifndef __CAAFControlPoint_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFControlPoint; // generated


HRESULT CAAFControlPoint::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFControlPoint *pObject = NULL;

  try
    {
      // Attempt to create an AAFControlPoint.
      hr =  CoCreateInstance(
                             CLSID_AAFControlPoint,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFControlPoint, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFControlPoint::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFControlPoint, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFControlPoint, ...);" <<
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
      cerr << "CAAFControlPoint::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}






