
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


////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafBool,
// aafRational_t,
// aafPosition_t,
// aafTrackID_t,




#include "CAAFTimelineMobSlot.h"
#include "CAAFTimelineMobSlot.h"
#ifndef __CAAFTimelineMobSlot_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFTimelineMobSlot::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFTimelineMobSlot *pObject = NULL;

  try
	{
	  // Attempt to create an AAFTimelineMobSlot.
	  hr =  CoCreateInstance(
							 CLSID_AAFTimelineMobSlot,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFTimelineMobSlot, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFTimelineMobSlot::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFTimelineMobSlot, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFTimelineMobSlot, ...);" <<
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
	  cerr << "CAAFTimelineMobSlot::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}











