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


#include "CAAFTypeDefEnum.h"
#include "CAAFTypeDefEnum.h"
#ifndef __CAAFTypeDefEnum_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTypeDefEnum; // generated


HRESULT CAAFTypeDefEnum::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTypeDefEnum *pObject = NULL;

  try
    {
      // Attempt to create an AAFTypeDefEnum.
      hr =  CoCreateInstance(
                             CLSID_AAFTypeDefEnum,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTypeDefEnum, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTypeDefEnum::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTypeDefEnum, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTypeDefEnum, ...);" <<
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
      cerr << "CAAFTypeDefEnum::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}












  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of
  //     this type.)



  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of this
  //     type.)






  // AAFRESULT_BAD_TYPE
  //   - pPropValIn's type doesn't match GetElementType\(\))


