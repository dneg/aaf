/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFEffectDef_h__
#include "ImplAAFEffectDef.h"
#endif

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#ifndef __ImplEnumAAFEffectDefs_h__
#include "ImplEnumAAFEffectDefs.h"
#endif

#ifndef __ImplEnumAAFParameterDefs_h__
#include "ImplEnumAAFParameterDefs.h"
#endif

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


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







#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFGroup_h__
#include "ImplAAFGroup.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFGroup::ImplAAFGroup ()
{}


ImplAAFGroup::~ImplAAFGroup ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::Initialize (ImplAAFDataDef * /*datadef*/,
                           aafLength_t  /*length*/,
                           ImplAAFEffectDef * /*effectDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm  This function takes an already created effect definition object as an argument.
	//@comm  To add slots to the effect, call AddNewSlot.
	//@comm  To add renderings, call SetRender.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetEffectDefinition (ImplAAFEffectDef ** /*effectDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces part of omfiEffectGetInfo


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetRender (ImplAAFSegment ** /*segment*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm If this property does not exist the error
	// OM_ERR_PROP_NOT_PRESENT will be returned.
	//@comm Working and final renderings are handled by using
	// a media group as the segment.
	//@comm Replaces omfiEffectGetFinalRender and omfiEffectGetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::IsATimeWarp (aafBool **  /*sourceClip*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces omfiEffectIsATimeWarp


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetBypassOverride (aafArgIDType_t *  /*bypassOverride*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm If the property does not exist, the error OM_ERR_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiEffectGetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetNumSourceSegments (aafInt32 *  /*numSources*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces omfiEffectGetNumSlots


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetNumParameters (aafInt32 *  /*numParameters*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces omfiEffectGetNumSlots

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::IsValidTranEffect (aafBool *  /*validTransition*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::AddNewParameter (aafArgIDType_t  /*argID*/,
                           ImplAAFParameter * /*value*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces part of omfiEffectAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::AddNewInputSegment (aafInt32  /*index*/,
                           ImplAAFSegment * /*value*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces part of omfiEffectAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::SetRender (ImplAAFSegment * /*segment*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces omfiEffectSetFinalRender and omfiEffectSetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::SetBypassOverride (aafArgIDType_t  /*bypassOverride*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Replaces omfiEffectSetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetParameterByArgID (aafArgIDType_t  /*argID*/,
                           ImplAAFParameter ** /*parameter*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::GetIndexedInputSegment (aafInt32  /*index*/,
                           ImplAAFSegment ** /*inputSegment*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFGroup, AUID_AAFGroup);


