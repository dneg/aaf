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














#include "CAAFMob.h"
#include "CAAFMob.h"
#ifndef __CAAFMob_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFMob::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFMob *pObject = NULL;

  try
	{
	  // Attempt to create an AAFMob.
	  hr =  CoCreateInstance(
							 CLSID_AAFMob,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFMob, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFMob::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFMob, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFMob, ...);" <<
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
	  cerr << "CAAFMob::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}






























































////////////////////////////////////////////////////////////////////////////////
