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







#include "CAAFTimecodeStream12M.h"
#include "CAAFTimecodeStream12M.h"
#ifndef __CAAFTimecodeStream12M_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTimecodeStream12M; // generated


HRESULT CAAFTimecodeStream12M::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTimecodeStream12M *pObject = NULL;

  try
    {
      // Attempt to create an AAFTimecodeStream12M.
      hr =  CoCreateInstance(
                             CLSID_AAFTimecodeStream12M,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTimecodeStream12M, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTimecodeStream12M::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTimecodeStream12M, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTimecodeStream12M, ...);" <<
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
      cerr << "CAAFTimecodeStream12M::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}



/****/







/****/


/****/




