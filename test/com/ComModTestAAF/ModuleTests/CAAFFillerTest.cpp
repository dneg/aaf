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



#ifndef __CAAFFiller_h__
#include "CAAFFiller.h"
#endif

#include <iostream.h>
#include "AAFResult.h"

HRESULT CAAFFiller::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFFiller *pObject = NULL;

  try
	{
	  // Attempt to create an AAFFiller.
	  hr =  CoCreateInstance(
							 CLSID_AAFFiller,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFFiller, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFFiller::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFFiller, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFFiller, ...);" <<
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
	  cerr << "CAAFFiller::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}





