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






#include "CAAFTimecodeStream.h"
#include "CAAFTimecodeStream.h"
#ifndef __CAAFTimecodeStream_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFTimecodeStream; // generated


HRESULT CAAFTimecodeStream::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTimecodeStream *pObject = NULL;

  try
    {
      // Attempt to create an AAFTimecodeStream.
      hr =  CoCreateInstance(
                             CLSID_AAFTimecodeStream,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFTimecodeStream, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFTimecodeStream::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFTimecodeStream, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFTimecodeStream, ...);" <<
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
      cerr << "CAAFTimecodeStream::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}















							 





