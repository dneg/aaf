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
*  Stub only.   Implementation not yet added    *
\***********************************************/






#include "CAAFEssenceData.h"
#include "CAAFEssenceData.h"
#ifndef __CAAFEssenceData_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFEssenceData::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEssenceData *pObject = NULL;

  try
	{
	  // Attempt to create an AAFEssenceData.
	  hr =  CoCreateInstance(
							 CLSID_AAFEssenceData,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFEssenceData, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFEssenceData::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFEssenceData, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFEssenceData, ...);" <<
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
	  cerr << "CAAFEssenceData::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}


/****/


/****/


/****/


/****/


/****/


/****/


/****/



