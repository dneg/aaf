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
 * @module AAFDefObject | AAFDefObject is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFDefObject specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/




#include "CAAFDefObject.h"
#include "CAAFDefObject.h"
#ifndef __CAAFDefObject_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFDefObject; // generated


HRESULT CAAFDefObject::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFDefObject *pObject = NULL;

  try
    {
      // Attempt to create an AAFDefObject.
      hr =  CoCreateInstance(
                             CLSID_AAFDefObject,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFDefObject, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFDefObject::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFDefObject, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFDefObject, ...);" <<
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
      cerr << "CAAFDefObject::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}











