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





#include "CAAFSegment.h"
#include "CAAFSegment.h"
#ifndef __CAAFSegment_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFSegment::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFSegment *pObject = NULL;

  try
	{
	  // Attempt to create an AAFSegment.
	  hr =  CoCreateInstance(
							 CLSID_AAFSegment,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFSegment, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFSegment::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFSegment, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFSegment, ...);" <<
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
	  cerr << "CAAFSegment::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}







 

