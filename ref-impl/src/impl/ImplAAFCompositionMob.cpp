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



#ifndef __ImplAAFCompositionMob_h__
#include "ImplAAFCompositionMob.h"
#endif

#include <assert.h>
#include "AAFResult.h"

ImplAAFCompositionMob::ImplAAFCompositionMob ()
{}


ImplAAFCompositionMob::~ImplAAFCompositionMob ()
{}


  //@access Public Members
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::SetInitialValues (aafString_t *  /*name*/,
                           aafBool  /*isPrimary*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //@rdesc Error code [see below].

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::GetDefaultFade (aafDefaultFade_t *  /*result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to omfiSourceClipGetFade() in all cases.
  //@comm Maps to omfiMobGetDefaultFade
	

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::SetDefaultFade (aafLength_t    /*fadeLength  */,
                           aafFadeType_t  /*fadeType    */,
                           aafRational_t  /*fadeEditUnit*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kCompMob;

	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_AAFCompositionMob;

OMDEFINE_STORABLE(ImplAAFCompositionMob, CLSID_AAFCompositionMob);

