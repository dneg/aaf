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






#include "CAAFFindSourceInfo.h"
#include "CAAFFindSourceInfo.h"
#ifndef __CAAFFindSourceInfo_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFFindSourceInfo;


HRESULT CAAFFindSourceInfo::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFFindSourceInfo *pObject = NULL;

  try
	{
	  // Attempt to create an AAFFindSourceInfo.
	  hr =  CoCreateInstance(
							 CLSID_AAFFindSourceInfo,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFFindSourceInfo, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFFindSourceInfo::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFFindSourceInfo, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFFindSourceInfo, ...);" <<
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
	  cerr << "CAAFFindSourceInfo::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}


