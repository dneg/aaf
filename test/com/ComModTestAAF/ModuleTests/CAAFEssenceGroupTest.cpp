// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceGroup
/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/




#include "CAAFEssenceGroup.h"
#include "CAAFEssenceGroup.h"
#ifndef __CAAFEssenceGroup_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEssenceGroup; // generated


HRESULT CAAFEssenceGroup::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEssenceGroup *pObject = NULL;

  try
    {
      // Attempt to create an AAFEssenceGroup.
      hr =  CoCreateInstance(
                             CLSID_AAFEssenceGroup,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFEssenceGroup, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFEssenceGroup::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFEssenceGroup, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFEssenceGroup, ...);" <<
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
      cerr << "CAAFEssenceGroup::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}




























