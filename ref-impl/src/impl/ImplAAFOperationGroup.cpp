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

#ifndef __ImplAAFOperationDef_h__
#include "ImplAAFOperationDef.h"
#endif

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#ifndef __ImplEnumAAFOperationDefs_h__
#include "ImplEnumAAFOperationDefs.h"
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

#ifndef __ImplAAFOperationGroup_h__
#include "ImplAAFOperationGroup.h"
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

ImplAAFOperationGroup::ImplAAFOperationGroup ()
: _operationDefinition( PID_OperationGroup_OperationDefinition, "OperationDefinition"),
  _inputSegments( PID_OperationGroup_InputSegments, "InputSegments"),
  _parameters( PID_OperationGroup_Parameters, "Parameters"),
  _bypassOverride( PID_OperationGroup_BypassOverride, "BypassOverride"),
  _rendering( PID_OperationGroup_Rendering, "Rendering")
{
	_persistentProperties.put(_operationDefinition.address());
	_persistentProperties.put(_inputSegments.address());
	_persistentProperties.put(_parameters.address());
	_persistentProperties.put(_bypassOverride.address());
	_persistentProperties.put(_rendering.address());
}


ImplAAFOperationGroup::~ImplAAFOperationGroup ()
{
	// Release all of the mob slot pointers.
	size_t size = _inputSegments.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFSegment *pSeg = _inputSegments.setValueAt(0, i);
		if (pSeg)
		{
			pSeg->ReleaseReference();
		}
	}
	// Release all of the mob slot pointers.
	size_t size2 = _parameters.getSize();
	for (size_t j = 0; j < size2; j++)
	{
		ImplAAFParameter *pParm = _parameters.setValueAt(0, j);
		if (pParm)
		{
			pParm->ReleaseReference();
		}
	}
	ImplAAFSourceReference *ref = _rendering.setValue(0);
	if (ref)
	{
		ref->ReleaseReference();
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::Initialize(aafUID_t*		pDatadef,
							 aafLength_t    length,
                             ImplAAFOperationDef* pOperationDef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	ImplAAFHeader*			pHeader = NULL;
	ImplAAFDictionary*		pDictionary = NULL;
//	ImplAAFOperationDef*		pOldOperationDef = NULL;
	aafUID_t				OperationDefAUID;
	aafUID_t	uid;

	if (pDatadef == NULL || pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		// Get the Header and the dictionary objects for this file.
		CHECK(pOperationDef->MyHeadObject(&pHeader));
		CHECK(pHeader->GetDictionary(&pDictionary));
		pHeader->ReleaseReference();
		pHeader = NULL;

		CHECK(SetNewProps(length, pDatadef));
		CHECK(pOperationDef->GetAUID(&uid));
		_operationDefinition = uid;
		// Lookup the OperationGroup definition's AUID
		CHECK(pOperationDef->GetAUID(&OperationDefAUID));
		// find out if this OperationDef is already set
// !!!JeffB: Not handling weak references as OM references yet.
//		if (pDictionary->LookupOperationDefinition(&OperationDefAUID, &pOldOperationDef) == AAFRESULT_SUCCESS)
//			pOldOperationDef->ReleaseReference();
		_operationDefinition = OperationDefAUID;
//		pOperationDef->AcquireReference();
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

	//@comm  This function takes an already created OperationGroup definition object as an argument.
	//@comm  To add slots to the OperationGroup, call AddNewSlot.
	//@comm  To add renderings, call SetRender.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetOperationDefinition (ImplAAFOperationDef **OperationDef)
{
	aafUID_t			defUID;
	ImplAAFDictionary	*dict = NULL;
	ImplAAFHeader		*head = NULL;

	if(OperationDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		defUID = _operationDefinition;
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		CHECK(dict->LookupOperationDefinition(&defUID, OperationDef));
		dict->ReleaseReference();
		head->ReleaseReference();
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

	//@comm Replaces part of omfiOperationGroupGetInfo


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetRender (ImplAAFSourceReference **sourceRef)
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
	//@comm Replaces omfiOperationGroupGetFinalRender and omfiOperationGroupGetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::IsATimeWarp (aafBool *isTimeWarp)
{
	ImplAAFOperationDef	*def = NULL;
	
	XPROTECT()
	{
		if(isTimeWarp == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		CHECK(GetOperationDefinition(&def));
		CHECK(def->IsTimeWarp (isTimeWarp));
		def->ReleaseReference();
		def = NULL;
	}
	XEXCEPT
	{
		if(def)
				def->ReleaseReference();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiOperationGroupIsATimeWarp


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetBypassOverride (aafUInt32* pBypassOverride)
{
	if(pBypassOverride == NULL)
		return AAFRESULT_NULL_PARAM;
	*pBypassOverride = _bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm If the property does not exist, the error AAFRESULT_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiOperationGroupGetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetNumSourceSegments (aafInt32 *pNumSources)
{
   size_t numSlots;

	if(pNumSources == NULL)
		return AAFRESULT_NULL_PARAM;
	_inputSegments.getSize(numSlots);
	
	*pNumSources = numSlots;

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces omfiOperationGroupGetNumSlots


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetNumParameters (aafInt32 * pNumParameters)
{
   size_t numSlots;

	if(pNumParameters == NULL)
		return AAFRESULT_NULL_PARAM;
	_parameters.getSize(numSlots);
	
	*pNumParameters = numSlots;

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces omfiOperationGroupGetNumSlots

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::IsValidTranOperation (aafBool * validTransition)
{
	ImplAAFOperationDef	*def = NULL;
	aafInt32			numInputs;
	
	XPROTECT()
	{
		if(validTransition == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		CHECK(GetOperationDefinition(&def));
		CHECK(def->GetNumberInputs (&numInputs));
		*validTransition = (numInputs == 2 ? AAFTrue : AAFFalse);
		//!!!Must also have a "level" parameter (Need definition for this!)
		def->ReleaseReference();
		def = NULL;
	}
	XEXCEPT
	{
		if(def)
				def->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::AddNewParameter (ImplAAFParameter *pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_parameters.appendValue(pValue);
	pValue->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces part of omfiOperationGroupAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::AppendNewInputSegment (ImplAAFSegment * value)
{
	_inputSegments.appendValue(value);
	value->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiOperationGroupAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::SetRender (ImplAAFSourceReference *sourceRef)
{
	if(sourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

	_rendering = sourceRef;
	_rendering->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiOperationGroupSetFinalRender and omfiOperationGroupSetWorkingRender

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::SetBypassOverride (aafUInt32  bypassOverride)
{
	_bypassOverride = bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfiOperationGroupSetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetParameterByArgID (aafArgIDType_t  argID,
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
				parmDef->ReleaseReference();
				parmDef = NULL;
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
    ImplAAFOperationGroup::GetIndexedInputSegment (aafInt32  index,
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



OMDEFINE_STORABLE(ImplAAFOperationGroup, AUID_AAFOperationGroup);


