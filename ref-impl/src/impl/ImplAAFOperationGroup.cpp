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

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif



#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIds.h"

#ifndef __ImplAAFGroup_h__
#include "ImplAAFGroup.h"
#endif

#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "aafUtils.h"
#include "AAFDefUIDs.h"

extern "C" const aafClassID_t CLSID_AAFSourceReference;
extern "C" const aafClassID_t CLSID_AAFParameter;
extern "C" const aafClassID_t CLSID_AAFSegment;

const aafUID_t kNullID = {0};

ImplAAFGroup::ImplAAFGroup ()
//_effectDefinition( PID_Group_EffectDefinition, "Effect Definition"),
//_inputSegments( PID_Group_InputSegments, "Input Segments"),
//_parameters( PID_Group_Parameters, "Parameters"),
//_bypassOverride( PID_Group_BypassOverride, "Bypass Override"),
//_rendering( PID_Group_Rendering, "Rendering")
{
//	_persistentProperties.put(_effectDefinition.address());
//	_persistentProperties.put(_inputSegments.address());
//	_persistentProperties.put(_parameters.address());
//	_persistentProperties.put(_bypassOverride.address());
//	_persistentProperties.put(_rendering.address());
}


ImplAAFGroup::~ImplAAFGroup ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGroup::Initialize(aafUID_t*		pDatadef,
							 aafLength_t    length,
                             ImplAAFEffectDef* pEffectDef)
{
	HRESULT		rc = AAFRESULT_SUCCESS;

//	if (pDatadef == NULL || pEffectDef == NULL)
//		return AAFRESULT_NULL_PARAM;
	// Only validate data def for now
	if (pDatadef == NULL )
		return AAFRESULT_NULL_PARAM;

	rc = SetNewProps(length, pDatadef);
/*
	XPROTECT()
	{
		CHECK(SetNewProps(length, pDatadef));
		// ***********************************************************************
		// ************************************************************************
		// Optional arguments are set here only as a way to NOT 
		// fail inside the OM during initial development.
		// Once optional properties are properly supported
		// this setting instructions MUST BE REMOVED !!!
		ImplAAFSourceReference*		pSourceRef = NULL;
		ImplAAFParameter*			pParms = NULL;
		ImplAAFSegment*				pSeg = NULL;
														  
		pSourceRef = (ImplAAFSourceReference *)CreateImpl(CLSID_AAFSourceReference);
		if (pSourceRef == NULL)
			return (E_FAIL);
		pParms = (ImplAAFParameter *)CreateImpl(CLSID_AAFParameter);
		if (pParms == NULL)
			return (E_FAIL);
		pSeg = (ImplAAFSegment *)CreateImpl(CLSID_AAFSegment);
		if (pSeg == NULL)
			return (E_FAIL);
		_inputSegments.appendValue(pSeg);
		_parameters.appendValue(pParms);
		pSourceRef->SetSourceID(*pDatadef);				
		pSourceRef->SetSourceMobSlotID((aafSlotID_t)1); 
		_bypassOverride = (aafUInt32)1;							
		_rendering = pSourceRef;
		// Here we have to find the effect definition auid from the 
		// Effect definition object pointer we got as an input 
		// EffectDef is NOT implemented yet - therefore:
//		_effectDefinition = kNullID;
		// ************************************************************************
		// ************************************************************************
	}
	XEXCEPT
	XEND;
*/	
	return rc;
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


