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









#include "CAAFCommentMarker.h"
#include "CAAFCommentMarker.h"
#ifndef __CAAFCommentMarker_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFCommentMarker; // generated


HRESULT CAAFCommentMarker::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFCommentMarker *pObject = NULL;

  try
    {
      // Attempt to create an AAFCommentMarker.
      hr =  CoCreateInstance(
                             CLSID_AAFCommentMarker,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFCommentMarker, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFCommentMarker::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFCommentMarker, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFCommentMarker, ...);" <<
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
      cerr << "CAAFCommentMarker::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}









