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



#include "CAAFCompositionMob.h"
#include "CAAFCompositionMob.h"
#ifndef __CAAFCompositionMob_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFCompositionMob::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFCompositionMob *pObject = NULL;

  try
	{
	  // Attempt to create an AAFCompositionMob.
	  hr =  CoCreateInstance(
							 CLSID_AAFCompositionMob,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFCompositionMob, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFCompositionMob::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFCompositionMob, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFCompositionMob, ...);" <<
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
	  cerr << "CAAFCompositionMob::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}


  //@access Public Members

  //@rdesc Error code [see below].


  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to omfiSourceClipGetFade() in all cases.
  //@comm Maps to omfiMobGetDefaultFade
	


  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade


