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






#include "CEnumAAFProperties.h"
#include "CEnumAAFProperties.h"
#ifndef __CEnumAAFProperties_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFProperties; // generated


HRESULT CEnumAAFProperties::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFProperties *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFProperties.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFProperties,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFProperties, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFProperties::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFProperties, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFProperties, ...);" <<
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
      cerr << "CEnumAAFProperties::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}

