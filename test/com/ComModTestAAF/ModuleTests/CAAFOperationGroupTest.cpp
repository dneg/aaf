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

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/





#ifndef __CAAFGroup_h__
#include "CAAFGroup.h"
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFGroup; // generated


HRESULT CAAFGroup::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFGroup *pObject = NULL;

  try
    {
      // Attempt to create an AAFGroup.
      hr =  CoCreateInstance(
                             CLSID_AAFGroup,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFGroup, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFGroup::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFGroup, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFGroup, ...);" <<
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
      cerr << "CAAFGroup::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}




	//@comm  This function takes an already created effect definition object as an argument.
	//@comm  To add slots to the effect, call AddNewSlot.
	//@comm  To add renderings, call SetRender.


	//@comm Replaces part of omfiEffectGetInfo



	//@comm If this property does not exist the error
	// OM_ERR_PROP_NOT_PRESENT will be returned.
	//@comm Working and final renderings are handled by using
	// a media group as the segment.
	//@comm Replaces omfiEffectGetFinalRender and omfiEffectGetWorkingRender


	//@comm Replaces omfiEffectIsATimeWarp



	//@comm If the property does not exist, the error OM_ERR_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiEffectGetBypassOverride


	//@comm Replaces omfiEffectGetNumSlots



	//@comm Replaces omfiEffectGetNumSlots




	//@comm Replaces part of omfiEffectAddNewSlot


	//@comm Replaces part of omfiEffectAddNewSlot


	//@comm Replaces omfiEffectSetFinalRender and omfiEffectSetWorkingRender


	//@comm Replaces omfiEffectSetBypassOverride







