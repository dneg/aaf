/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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
#include "ImplEnumAAFParameters.h"

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
extern "C" const aafClassID_t CLSID_EnumAAFParameters;


const aafUID_t kNullID = {0};

ImplAAFOperationGroup::ImplAAFOperationGroup ()
: _operationDefinition( PID_OperationGroup_OperationDefinition, "OperationDefinition", "/Dictionary/OperationDefinitions"),
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
		  pSeg = 0;
		}
	}
	OMStrongReferenceSetIterator<ImplAAFParameter>parameters(_parameters);
	while(++parameters)
	{
		ImplAAFParameter *pParm = parameters.setValue(0);
		if (pParm)
		{
		  pParm->ReleaseReference();
		  pParm = 0;
		}
	}
	ImplAAFSourceReference *ref = _rendering.setValue(0);
	if (ref)
	{
	  ref->ReleaseReference();
	  ref = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::Initialize(ImplAAFDataDef * pDataDef,
							 aafLength_t    length,
                             ImplAAFOperationDef* pOperationDef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	ImplAAFOperationDef*	pOldOperationDef = NULL;

	if (pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (pDataDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(SetNewProps(length, pDataDef));
		// find out if this OperationDef is already set
		if(!_operationDefinition.isVoid())
		{
			pOldOperationDef = _operationDefinition;
			if (pOldOperationDef != 0)
				pOldOperationDef->ReleaseReference();
		}
		_operationDefinition = pOperationDef;
		pOperationDef->AcquireReference();
	}
	XEXCEPT
	{
	}
	XEND;

	return rc;
}

	//@comm  This function takes an already created OperationGroup definition object as an argument.
	//@comm  To add slots to the OperationGroup, call AddSlot.
	//@comm  To add renderings, call SetRender.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetOperationDefinition (ImplAAFOperationDef **OperationDef)
{
  ImplAAFOperationDef *pOpDef = _operationDefinition;
  if(!pOpDef)
		return AAFRESULT_OBJECT_NOT_FOUND;

  *OperationDef = pOpDef;
  assert (*OperationDef);
  (*OperationDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::SetOperationDefinition (ImplAAFOperationDef *OperationDef)
{
	if(OperationDef == NULL)
		return AAFRESULT_NULL_PARAM;

	assert(_operationDefinition.isVoid());
	_operationDefinition = OperationDef;

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiOperationGroupGetInfo


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetRender (ImplAAFSourceReference **sourceRef)
{
	if(sourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (_rendering.isPresent())
	{
		if (_rendering)
		{
			*sourceRef = _rendering;
			_rendering->AcquireReference();
		}
		else
			return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
		return AAFRESULT_PROP_NOT_PRESENT;

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
		def = 0;
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

	if (!_bypassOverride.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBypassOverride = _bypassOverride;

	return AAFRESULT_SUCCESS;
}

	//@comm If the property does not exist, the error AAFRESULT_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiOperationGroupGetBypassOverride

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::CountSourceSegments (aafInt32 *pNumSources)
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
    ImplAAFOperationGroup::CountParameters (aafInt32 * pNumParameters)
{
   size_t numSlots;

	if(pNumParameters == NULL)
		return AAFRESULT_NULL_PARAM;
	numSlots = _parameters.count();
	
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
		*validTransition = (numInputs == 2 ? kAAFTrue : kAAFFalse);
		//!!!Must also have a "level" parameter (Need definition for this!)
		def->ReleaseReference();
		def = NULL;
	}
	XEXCEPT
	{
		if(def)
		  def->ReleaseReference();
		def = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::AddParameter (ImplAAFParameter *pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_parameters.appendValue(pValue);
	pValue->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

	//@comm Replaces part of omfiOperationGroupAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::AppendInputSegment (ImplAAFSegment * value)
{
	_inputSegments.appendValue(value);
	value->AcquireReference();

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces part of omfiOperationGroupAddNewSlot

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::PrependInputSegment (ImplAAFSegment * value)
{
  if (! value)
	return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::InsertInputSegmentAt
      (aafUInt32 index,
	   ImplAAFSegment * value)
{
  if (! value)
	return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}



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
    ImplAAFOperationGroup::LookupParameter (aafArgIDType_t  argID,
                           ImplAAFParameter ** ppParameter)
{
#if 1
	if (!ppParameter) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_parameters.find((*reinterpret_cast<const OMObjectIdentification *>(&argID)),
                             *ppParameter))
	{
		assert(NULL != *ppParameter);
		(*ppParameter)->AcquireReference();
	}
	else
	{
	// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);

#else
	ImplAAFParameter	*parm = NULL;
	ImplAAFParameterDef	*parmDef = NULL;
	aafInt32			numParm, n;
	aafUID_t			testAUID;
	aafBool				found;

	XPROTECT()
	{
		if(ppParameter == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
	
		found = kAAFFalse;
		CHECK(CountParameters (&numParm))
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
					found = kAAFTrue;
					break;
				}

			}
		}
		if(!found)
			RAISE(AAFRESULT_PARAMETER_NOT_FOUND);
	}
	XEXCEPT
	{
//		if(parm != NULL)
//		  parm->ReleaseReference();
//		parm = 0;
		if(parmDef != NULL)
		  parmDef->ReleaseReference();
		parmDef = 0;
	}
	XEND

	return AAFRESULT_SUCCESS;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetParameters
        (// @parm [out] enumerator across parameters
         ImplEnumAAFParameters ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFParameters *theEnum = (ImplEnumAAFParameters *)CreateImpl (CLSID_EnumAAFParameters);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<ImplAAFParameter>* iter = 
			new OMStrongReferenceSetIterator<ImplAAFParameter>(_parameters);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::GetInputSegmentAt (aafUInt32  index,
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


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationGroup::RemoveInputSegmentAt (aafUInt32  index)
{
	aafInt32 count;
	AAFRESULT hr;
	hr = CountSourceSegments (&count);
	if (AAFRESULT_FAILED (hr)) return hr;
	if (index >= (aafUInt32)count)
		return AAFRESULT_BADINDEX;
	
	_inputSegments.removeAt(index);
	return AAFRESULT_SUCCESS;
}
