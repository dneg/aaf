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







#include "CEnumAAFContainerDefs.h"
#include "CEnumAAFContainerDefs.h"
#ifndef __CEnumAAFContainerDefs_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFContainerDefs; // generated


HRESULT CEnumAAFContainerDefs::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFContainerDefs *pObject = NULL;

  try
    {
      // Attempt to create an EnumAAFContainerDefs.
      hr =  CoCreateInstance(
                             CLSID_EnumAAFContainerDefs,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IEnumAAFContainerDefs, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CEnumAAFContainerDefs::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_EnumAAFContainerDefs, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IEnumAAFContainerDefs, ...);" <<
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
      cerr << "CEnumAAFContainerDefs::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















