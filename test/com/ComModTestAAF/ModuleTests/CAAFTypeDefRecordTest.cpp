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







#include "CAAFTypeDefRecord.h"
#include "CAAFTypeDefRecord.h"
#ifndef __CAAFTypeDefRecord_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefRecord; // generated


HRESULT CAAFTypeDefRecord::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefRecord *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefRecord.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefRecord,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefRecord, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefRecord::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefRecord, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefRecord, ...);" <<
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
      cerr << "CAAFTypeDefRecord::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}






































