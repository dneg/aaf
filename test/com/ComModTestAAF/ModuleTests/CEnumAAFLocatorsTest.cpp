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







#include "CEnumAAFLocators.h"
#include "CEnumAAFLocators.h"
#ifndef __CEnumAAFLocators_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFLocators;


HRESULT CEnumAAFLocators::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFLocators *pObject = NULL;

  try
	{
	  // Attempt to create an EnumAAFLocators.
	  hr =  CoCreateInstance(
							 CLSID_EnumAAFLocators,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IEnumAAFLocators, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CEnumAAFLocators::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_EnumAAFLocators, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IEnumAAFLocators, ...);" <<
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
	  cerr << "CEnumAAFLocators::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}













