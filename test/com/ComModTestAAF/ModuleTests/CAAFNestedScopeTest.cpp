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
 * @class AAFNestedScope | an AAFNestedScope object defines a scope that 
 *			contains an ordered set of AAFSegments and produces the value
 *			specified by the last AAFSegement in the ordered set.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/







#include "CAAFNestedScope.h"
#include "CAAFNestedScope.h"
#ifndef __CAAFNestedScope_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFNestedScope; // generated


HRESULT CAAFNestedScope::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFNestedScope *pObject = NULL;

  try
    {
      // Attempt to create an AAFNestedScope.
      hr =  CoCreateInstance(
                             CLSID_AAFNestedScope,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFNestedScope, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFNestedScope::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFNestedScope, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFNestedScope, ...);" <<
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
      cerr << "CAAFNestedScope::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}











