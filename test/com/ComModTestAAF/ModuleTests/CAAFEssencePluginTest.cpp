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

#include "CAAFEssencePlugin.h"
#include "CAAFEssencePlugin.h"
#ifndef __CAAFEssencePlugin_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEssencePlugin;


HRESULT CAAFEssencePlugin::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEssencePlugin *pObject = NULL;

  try
	{
	  // Attempt to create an AAFEssencePlugin.
	  hr =  CoCreateInstance(
							 CLSID_AAFEssencePlugin,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFEssencePlugin, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFEssencePlugin::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFEssencePlugin, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFEssencePlugin, ...);" <<
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
	  cerr << "CAAFEssencePlugin::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}







