// @doc INTERNAL
// @com This file implements the module test for CAAFEdgecode
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
*  Stub only.   Implementation not yet added    *
\***********************************************/







#include "CAAFEdgecode.h"
#include "CAAFEdgecode.h"
#ifndef __CAAFEdgecode_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFEdgecode::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEdgecode *pObject = NULL;

  try
    {
      // Attempt to create an AAFEdgecode.
      hr =  CoCreateInstance(
                             CLSID_AAFEdgecode,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFEdgecode, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFEdgecode::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFEdgecode, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFEdgecode, ...);" <<
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
      cerr << "CAAFEdgecode::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}





