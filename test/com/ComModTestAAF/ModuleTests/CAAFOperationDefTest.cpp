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






//AxD_FORWARD_DECLARATION(EnumAAFEffectDefs)


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


 


#include "CAAFEffectDef.h"
#include "CAAFEffectDef.h"
#ifndef __CAAFEffectDef_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEffectDef; // generated


HRESULT CAAFEffectDef::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEffectDef *pObject = NULL;

  try
    {
      // Attempt to create an AAFEffectDef.
      hr =  CoCreateInstance(
                             CLSID_AAFEffectDef,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFEffectDef, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFEffectDef::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFEffectDef, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFEffectDef, ...);" <<
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
      cerr << "CAAFEffectDef::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}

















//AxD_XMETHOD1(AppendDegradeToEffects,
//			[in],
//			objIn,
//			AAFEffectDef,
//			pEffectDef, 
//			Degrade To Effect Definition Object,
//			Add the effect definition object to the DegradeTo list of definitions.)
//
//AxD_XMETHOD1(RemoveDegradeToEffects,
//			[in], 
//			objIn,
//			AAFEffectDef,
//			pEffectDef, 
//			Degrade To Effect Definition Object,
//			Remove the effect definition object from the DegradeTo list of definitions.)
//
//AxD_XMETHOD1(GetDegradeToEffects,
//			[out\,retval], 
//			objOut,
//			EnumAAFEffectDefs,
//			ppEnum, 
//			Effect Definition Enumeration,
//			Return an enumerator for aaf effect definitions.)































