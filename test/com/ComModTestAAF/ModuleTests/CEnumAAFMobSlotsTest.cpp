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







#include "CEnumAAFMobSlots.h"
#include "CEnumAAFMobSlots.h"
#ifndef __CEnumAAFMobSlots_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_EnumAAFMobSlots;


HRESULT CEnumAAFMobSlots::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFMobSlots *pObject = NULL;

  try
	{
	  // Attempt to create an EnumAAFMobSlots.
	  hr =  CoCreateInstance(
							 CLSID_EnumAAFMobSlots,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IEnumAAFMobSlots, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CEnumAAFMobSlots::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_EnumAAFMobSlots, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IEnumAAFMobSlots, ...);" <<
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
	  cerr << "CEnumAAFMobSlots::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}













