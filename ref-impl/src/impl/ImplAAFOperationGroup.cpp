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

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
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

#ifndef __ImplAAFEffect_h__
#include "ImplAAFEffect.h"
#endif

#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFHeader.h"

#include <assert.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "AAFDefUIDs.h"

extern "C" const aafClassID_t CLSID_AAFSourceReference;
extern "C" const aafClassID_t CLSID_AAFParameter;
extern "C" const aafClassID_t CLSID_AAFSegment;


const aafUID_t kNullID = {0};

ImplAAFEffect::ImplAAFEffect ()
: _effectDefinition( PID_Effect_EffectDefinition, "EffectDefinition"),
  _inputSegments( PID_Effect_InputSegments, "InputSegments"),
  _parameters( PID_Effect_Parameters, "Parameters"),
  _bypassOverride( PID_Effect_BypassOverride, "BypassOverride"),
  _rendering( PID_Effect_Rendering, "Rendering")
{
	_persistentProperties.put(_effectDefinition.address());
	_persistentProperties.put(_inputSegments.address());
	_persistentProperties.put(_parameters.address());
	_persistentProperties.put(_bypassOverride.address());
	_persistentProperties.put(_rendering.address());
}


ImplAAFEffect::~ImplAAFEffect ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::Initialize(aafUID_t*		pDatadef,
							 aafLength_t    length,
                             ImplAAFEffectDef* pEffectDef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	ImplAAFHeader*			pHeader = NULL;
	ImplAAFDictionary*		pDictionary = NULL;
//	ImplAAFEffectDef*		pOldEffectDef = NULL;
	aafUID_t				EffectDefAUID;
	aafUID_t	uid;

	if (pDatadef == NULL || pEffectDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		// Get the Header and the dictionary objects for this file.
		CHECK(pEffectDef->MyHeadObject(&pHeader));
		CHECK(pHeader->GetDictionary(&pDictionary));
		pHeader->ReleaseReference();
		pHeader = NULL;

		CHECK(SetNewProps(length, pDatadef));
		CHECK(pEffectDef->GetAUID(&uid));
		_effectDefinition = uid;
		// Lookup the effect definition's AUID
		CHECK(pEffectDef->GetAUID(&EffectDefAUID));
		// find out if this effectdef is already set
// !!!JeffB: Not handling weak references as OM references yet.
//		if (pDictionary->LookupEffectDefinition(&EffectDefAUID, &pOldEffectDef) == AAFRESULT_SUCCESS)
//			pOldEffectDef->ReleaseReference();
		_effectDefinition = EffectDefAUID;
		pEffectDef->AcquireReference();
		pDictionary->ReleaseReference();
	}
	XEXCEPT
	{
		if(pHeader != NULL)
			pHeader->ReleaseReference();
		if(pDictionary)
			pDictionary->ReleaseReference();
	}
	XEND;

	return rc;
}

	//@comm  This function takes an already created effect definition object as an argument.
	//@comm  To add slots to the effect, call AddNewSlot.
	//@comm  To add renderings, call SetRender.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetEffectDefinition (ImplAAFEffectDef **effectDef)
{
	aafUID_t			defUID;
	ImplAAFDictionary	*dict = NULL;
	ImplAAFHeader		*head = NULL;

	if(effectDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		defUID = _effectDefinition;
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		CHECK(dict->LookupEffectDefinition(&defUID, effectDef));
	}
	XEXCEPT
	{
		if(dict != NULL)
			dict->ReleaseReference();
		if(head != NULL)
			head->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiEffectGetInfo


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetRender (ImplAAFSourceReference **sourceRef)
{
	if(sourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

	*sourceRef = _rendering;
	_rendering->AcquireReference();
	return AAFRESULT_SUCCESS;
}

	//@comm If this property does not exist the error
	// AAFRESULT_PROP_NOT_PRESENT will be returned.
	//@comm Working and final renderings are handled by using
	// a media group as the segment.
	//@comm Replaces omfiEffectGetFinalRender and omfiEffectGetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::IsATimeWarp (aafBool *isTimeWarp)
{
	ImplAAFEffectDef	*def;
	
	XPROTECT()
	{
		if(isTimeWarp == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		CHECK(GetEffectDefinition(&def));
		CHECK(def->IsTimeWarp (isTimeWarp));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiEffectIsATimeWarp


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetBypassOverride (aafUInt32* pBypassOverride)
{
	if(pBypassOverride == NULL)
		return AAFRESULT_NULL_PARAM;
	*pBypassOverride = _bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm If the property does not exist, the error AAFRESULT_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiEffectGetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetNumSourceSegments (aafInt32 *pNumSources)
{
   size_t numSlots;

	if(pNumSources == NULL)
		return AAFRESULT_NULL_PARAM;
	_inputSegments.getSize(numSlots);
	
	*pNumSources = numSlots;

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces omfiEffectGetNumSlots


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetNumParameters (aafInt32 * pNumParameters)
{
   size_t numSlots;

	if(pNumParameters == NULL)
		return AAFRESULT_NULL_PARAM;
	_parameters.getSize(numSlots);
	
	*pNumParameters = numSlots;

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces omfiEffectGetNumSlots

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::IsValidTranEffect (aafBool * validTransition)
{
	ImplAAFEffectDef	*def;
	aafInt32			numInputs;
	
	XPROTECT()
	{
		if(validTransition == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		CHECK(GetEffectDefinition(&def));
		CHECK(def->GetNumberInputs (&numInputs));
		*validTransition = (numInputs == 2 ? AAFTrue : AAFFalse);
		//!!!Must also have a "level" parameter (Need definition for this!)
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::AddNewParameter (ImplAAFParameter *pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_parameters.appendValue(pValue);
	pValue->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces part of omfiEffectAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::AppendNewInputSegment (ImplAAFSegment * value)
{
	_inputSegments.appendValue(value);
	value->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiEffectAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::SetRender (ImplAAFSourceReference *sourceRef)
{
	if(sourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

	_rendering = sourceRef;
	_rendering->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiEffectSetFinalRender and omfiEffectSetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::SetBypassOverride (aafUInt32  bypassOverride)
{
	_bypassOverride = bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiEffectSetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetParameterByArgID (aafArgIDType_t  argID,
                           ImplAAFParameter ** ppParameter)
{
	ImplAAFParameter	*parm = NULL;
	ImplAAFParameterDef	*parmDef = NULL;
	aafInt32			numParm, n;
	aafUID_t			testAUID;

	XPROTECT()
	{
		if(ppParameter == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
	
		CHECK(GetNumParameters (&numParm))
		for(n = 0; n < numParm; n++)
		{
			_parameters.getValueAt(parm, n);
			if (parm)
			{
				CHECK(parm->GetParameterDefinition (&parmDef));	
				CHECK(parmDef->GetAUID(&testAUID));
				if(EqualAUID(&testAUID, &argID))
				{
					parm->AcquireReference();
					*ppParameter = parm;
					break;
				}
			}
		}
	}
	XEXCEPT
	{
		if(parm != NULL)
			parm->ReleaseReference();
		if(parmDef != NULL)
			parmDef->ReleaseReference();
	}
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffect::GetIndexedInputSegment (aafInt32  index,
                           ImplAAFSegment ** ppInputSegment)
{
	ImplAAFSegment	*obj;

	XPROTECT()
	{
		_inputSegments.getValueAt(obj, index);
		if (obj)
			obj->AcquireReference();
		else
			RAISE(AAFRESULT_NO_MORE_OBJECTS); // AAFRESULT_BADINDEX ???
		*ppInputSegment = obj;
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}



OMDEFINE_STORABLE(ImplAAFEffect, AUID_AAFEffect);


