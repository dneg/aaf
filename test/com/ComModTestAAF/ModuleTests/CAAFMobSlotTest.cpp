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
// AAFSegment,
// aafPosition_t,
// aafSlotID_t,
// aafUInt32,
// AAFDataDef








#include "CAAFMobSlot.h"
#include "CAAFMobSlot.h"
#ifndef __CAAFMobSlot_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFMobSlot::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFMobSlot *pObject = NULL;

  try
	{
	  // Attempt to create an AAFMobSlot.
	  hr =  CoCreateInstance(
							 CLSID_AAFMobSlot,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFMobSlot, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFMobSlot::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFMobSlot, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFMobSlot, ...);" <<
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
	  cerr << "CAAFMobSlot::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}























