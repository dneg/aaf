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






#include "CAAFContentStorage.h"
#include "CAAFContentStorage.h"
#ifndef __CAAFContentStorage_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFContentStorage::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFContentStorage *pObject = NULL;

  try
	{
	  // Attempt to create an AAFContentStorage.
	  hr =  CoCreateInstance(
							 CLSID_AAFContentStorage,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFContentStorage, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFContentStorage::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFContentStorage, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFContentStorage, ...);" <<
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
	  cerr << "CAAFContentStorage::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}



