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




#include "CAAFFileDescriptor.h"
#include "CAAFFileDescriptor.h"
#ifndef __CAAFFileDescriptor_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFFileDescriptor;


HRESULT CAAFFileDescriptor::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFFileDescriptor *pObject = NULL;

  try
	{
	  // Attempt to create an AAFFileDescriptor.
	  hr =  CoCreateInstance(
							 CLSID_AAFFileDescriptor,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFFileDescriptor, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFFileDescriptor::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFFileDescriptor, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFFileDescriptor, ...);" <<
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
	  cerr << "CAAFFileDescriptor::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}


//@access Public Members

/****/


/****/


/****/


/****/


/****/


/****/


/****/


/****/



