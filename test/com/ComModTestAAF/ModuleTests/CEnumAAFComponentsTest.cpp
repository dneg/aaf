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








#include "CEnumAAFComponents.h"
#include "CEnumAAFComponents.h"
#ifndef __CEnumAAFComponents_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CEnumAAFComponents::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFComponents *pObject = NULL;

  try
	{
	  // Attempt to create an EnumAAFComponents.
	  hr =  CoCreateInstance(
							 CLSID_EnumAAFComponents,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IEnumAAFComponents, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CEnumAAFComponents::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_EnumAAFComponents, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IEnumAAFComponents, ...);" <<
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
	  cerr << "CEnumAAFComponents::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}













