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

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/








#include "CEnumAAFEssenceData.h"
#include "CEnumAAFEssenceData.h"
#ifndef __CEnumAAFEssenceData_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CEnumAAFEssenceData::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IEnumAAFEssenceData *pObject = NULL;

  try
	{
	  // Attempt to create an EnumAAFEssenceData.
	  hr =  CoCreateInstance(
							 CLSID_EnumAAFEssenceData,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IEnumAAFEssenceData, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CEnumAAFEssenceData::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_EnumAAFEssenceData, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IEnumAAFEssenceData, ...);" <<
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
	  cerr << "CEnumAAFEssenceData::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}













