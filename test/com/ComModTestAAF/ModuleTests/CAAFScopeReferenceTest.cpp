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


/*************************************************************************
 * 
 * @class AAFScopeReference | AAFScopeReference refers to a section in the 
 *			specified AAFNestedScope slo tor AAFMobSLot.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/



#include "CAAFScopeReference.h"
#include "CAAFScopeReference.h"
#ifndef __CAAFScopeReference_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFScopeReference; // generated


HRESULT CAAFScopeReference::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFScopeReference *pObject = NULL;

  try
    {
      // Attempt to create an AAFScopeReference.
      hr =  CoCreateInstance(
                             CLSID_AAFScopeReference,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFScopeReference, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFScopeReference::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFScopeReference, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFScopeReference, ...);" <<
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
      cerr << "CAAFScopeReference::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}











