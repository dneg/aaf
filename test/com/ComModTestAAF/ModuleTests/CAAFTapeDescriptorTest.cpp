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




#include "CAAFTapeDescriptor.h"
#include "CAAFTapeDescriptor.h"
#ifndef __CAAFTapeDescriptor_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFTapeDescriptor::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTapeDescriptor *pObject = NULL;

  try
	{
	  // Attempt to create an AAFTapeDescriptor.
	  hr =  CoCreateInstance(
							 CLSID_AAFTapeDescriptor,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFTapeDescriptor, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFTapeDescriptor::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFTapeDescriptor, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFTapeDescriptor, ...);" <<
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
	  cerr << "CAAFTapeDescriptor::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}























