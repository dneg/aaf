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




#include "CAAFObject.h"
#include "CAAFObject.h"
#ifndef __CAAFObject_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFObject::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFObject *pObject = NULL;

  try
	{
	  // Attempt to create an AAFObject.
	  hr =  CoCreateInstance(
							 CLSID_AAFObject,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFObject, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFObject::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFObject, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFObject, ...);" <<
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
	  cerr << "CAAFObject::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}






