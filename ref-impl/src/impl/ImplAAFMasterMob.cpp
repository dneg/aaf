/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFSourceClip_h__
#include "ImplAAFSourceClip.h"
#endif

#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif

#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFFindSourceInfo_h__
#include "ImplAAFFindSourceInfo.h"
#endif




#ifndef __ImplAAFMasterMob_h__
#include "ImplAAFMasterMob.h"
#endif

#include <assert.h>
#include "AAFResult.h"

ImplAAFMasterMob::ImplAAFMasterMob ()
{}


ImplAAFMasterMob::~ImplAAFMasterMob ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::AddMasterSlot (ImplAAFDataDef * /*pMediaKind*/,
                           aafSlotID_t  /*slotID*/,
                           aafSlotID_t  /*fileSlotID*/,
                           aafWChar *  /*pSlotName*/,
                           ImplAAFSourceMob * /*pFileMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeName (aafInt32  /*masterSlotID*/,
                           aafWChar *  /*pTapeName*/,
                           aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeNameBufLen (aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetNumRepresentations (aafSlotID_t  /*slotID*/,
                           aafNumSlots_t *  /*pNumReps*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetRepresentationSourceClip (aafSlotID_t  /*slotID*/,
                           aafInt32  /*index*/,
                           ImplAAFSourceClip ** /*ppSourceClip*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetCriteriaSourceClip (aafSlotID_t  /*slotID*/,
                           aafMediaCriteria_t *  /*pCriteria*/,
                           ImplAAFSourceClip ** /*ppSourceClip*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::SearchSource (aafSlotID_t  /*slotID*/,
                           aafPosition_t  /*offset*/,
                           aafMobKind_t  /*mobKind*/,
                           aafMediaCriteria_t *  /*pMediaCrit*/,
                           aafEffectChoice_t *  /*pEffectChoice*/,
                           ImplAAFComponent ** /*ppThisCpnt*/,
                           ImplAAFFindSourceInfo ** /*ppSourceInfo*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kMasterMob;

	return AAFRESULT_SUCCESS;
}


extern "C" const aafClassID_t CLSID_AAFMasterMob;

OMDEFINE_STORABLE(ImplAAFMasterMob, CLSID_AAFMasterMob);

