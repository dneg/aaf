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








#include "CAAFConstValue.h"
#include "CAAFConstValue.h"
#ifndef __CAAFConstValue_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFConstValue; // generated


HRESULT CAAFConstValue::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFConstValue *pObject = NULL;

  try
    {
      // Attempt to create an AAFConstValue.
      hr =  CoCreateInstance(
                             CLSID_AAFConstValue,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFConstValue, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFConstValue::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFConstValue, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFConstValue, ...);" <<
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
      cerr << "CAAFConstValue::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}



	
/****/





/****/

  //
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  \\\(This is the only code indicating success.\\\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\\\(\\\) called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pValue is null.)



