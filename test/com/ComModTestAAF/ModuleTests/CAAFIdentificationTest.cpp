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




#include "CAAFIdentification.h"
#include "CAAFIdentification.h"
#ifndef __CAAFIdentification_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFIdentification::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFIdentification *pObject = NULL;

  try
	{
	  // Attempt to create an AAFIdentification.
	  hr =  CoCreateInstance(
							 CLSID_AAFIdentification,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFIdentification, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFIdentification::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFIdentification, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFIdentification, ...);" <<
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
	  cerr << "CAAFIdentification::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}



















































