//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplEnumAAFSegments_h__
#include "ImplEnumAAFSegments.h"
#endif

#include "ImplAAFObjectCreation.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFSelector_h__
#include "ImplAAFSelector.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "ImplAAFMob.h"

extern "C" const aafClassID_t CLSID_EnumAAFSegments;

ImplAAFSelector::ImplAAFSelector () :
	_selected(		PID_Selector_Selected,		L"Selected"),
	_alternates(	PID_Selector_Alternates,	L"Alternates")
{
	_persistentProperties.put(_selected.address());
	_persistentProperties.put(_alternates.address());
}


ImplAAFSelector::~ImplAAFSelector ()
{
	ImplAAFSegment *selected = _selected.clearValue();
	if (selected != NULL)
	{
	  selected->ReleaseReference();
	  selected = 0;
	}

	size_t count = _alternates.count();
	for (size_t i = 0; i < count; i++)
	{
		ImplAAFSegment* pSegment = _alternates.clearValueAt(i);
		if (pSegment)
		{
		  pSegment->ReleaseReference();
		  pSegment = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSelector::GetSelectedSegment (ImplAAFSegment** ppSelSegment)
{
	HRESULT				hr = AAFRESULT_SUCCESS;

	if (ppSelSegment == NULL)
		hr = AAFRESULT_NULL_PARAM;
	else
	{
		*ppSelSegment = _selected;

		if (*ppSelSegment)
		{
			(*ppSelSegment)->AcquireReference();
		}
		else
		{
			hr = AAFRESULT_NULLOBJECT;
		}
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSelector::SetSelectedSegment (ImplAAFSegment* pSelSegment)
{
	ImplAAFSegment*		pPrevSelected = NULL;

	if (pSelSegment == NULL)
		return AAFRESULT_NULL_PARAM;

	pPrevSelected = _selected;
	if (pPrevSelected)
	{
	  if( pPrevSelected == pSelSegment )
		return AAFRESULT_SUCCESS;

	  pPrevSelected->ReleaseReference();
	  pPrevSelected = 0;
	}

	if (pSelSegment->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_selected = pSelSegment;
	_selected->AcquireReference();


	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSelector::AppendAlternateSegment (ImplAAFSegment* pSegment)
{
	HRESULT				hr = AAFRESULT_SUCCESS;

	if (pSegment == NULL)
	{
		hr = AAFRESULT_NULL_PARAM;
	}
	else
	{
		if (pSegment->attached())
		{
			hr =  AAFRESULT_OBJECT_ALREADY_ATTACHED;
		}
		else
		{
			pSegment->AcquireReference();
			_alternates.appendValue(pSegment);
		}
	}

	return hr;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFSelector::RemoveAlternateSegment (ImplAAFSegment* pSegment)
{
	if (pSegment == NULL)
		return AAFRESULT_NULL_PARAM;

	if( !pSegment->attached() )
		return AAFRESULT_SEGMENT_NOT_FOUND;

	if (!_alternates.containsValue(pSegment))
	  return AAFRESULT_SEGMENT_NOT_FOUND;

	_alternates.removeValue(pSegment);
	pSegment->ReleaseReference();

	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
// GetNumComponents()
//
// This function returns the number of alternate segments in this 
// selector.
// 
// Succeeds if all of the following are true:
// - the pNumSegments pointer is valid.
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pNumSegments is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSelector::GetNumAlternateSegments (aafInt32* pNumSegments)
{
	HRESULT	hr = AAFRESULT_SUCCESS;

	if (pNumSegments == NULL)
	{
		hr = AAFRESULT_NULL_PARAM;
	}
	else
	{
		size_t	numSegments = _alternates.count();
		*pNumSegments = numSegments;
	}

	return hr;
}



//***********************************************************
//
// EnumAlternateSegments()
//
// Places an IEnumAlternateSegments enumerator for the alterante segmemts contained 
// in the selector into the *ppEnum argument.
// 
// Succeeds if all of the following are true:
// - the ppEnum pointer is valid.
// 
// If this method fails nothing will be written to *ppEnum.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppEnum is null.
//
// E_FAIL
//   - Failed to create the enumerator.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSelector::EnumAlternateSegments (ImplEnumAAFSegments** ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFSegments *)CreateImpl(CLSID_EnumAAFSegments);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);

	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFSegment>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFSegment>(_alternates);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK((*ppEnum)->Initialize(&CLSID_EnumAAFSegments, this, iter));
	}
	XEXCEPT
	{
		if (*ppEnum)
		  (*ppEnum)->ReleaseReference();
		(*ppEnum) = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

//***********************************************************
//
// GetNthSegment()
//
// Get the component at the position specified by index from
// the components vector.  This is used by the ImplEnumAAFSegments
// to retrieve the components from the sequence.
//
// NOTES:
//
//    - The vector is 0-based.
//    - AddRef the object being returned.
// 
AAFRESULT
    ImplAAFSelector::GetNthSegment (aafUInt32 index, ImplAAFSegment** ppSegment)
{
	ImplAAFSegment*	obj;
	HRESULT			hr;

	size_t numSegments = _alternates.count();
	if (index < numSegments)
	{
		_alternates.getValueAt(obj, index);
		obj->AcquireReference();
		*ppSegment = obj;
		hr =  AAFRESULT_SUCCESS;
	}
	else
		hr = AAFRESULT_NO_MORE_OBJECTS;

	return hr;
}



AAFRESULT
	ImplAAFSelector::GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
										ImplAAFMob *mob, ImplAAFMobSlot *track,
										aafMediaCriteria_t *mediaCrit,
										aafPosition_t currentObjPos,
										aafOperationChoice_t *effectChoice,
										ImplAAFComponent	*prevObject,
										ImplAAFComponent *nextObject,
										ImplAAFScopeStack *scopeStack,
										aafPosition_t *diffPos, aafLength_t *minLength,
										ImplAAFOperationGroup **effeObject, aafInt32	*nestDepth,
										ImplAAFComponent **foundObj, aafBool *foundTransition)
{  
	ImplAAFSegment		*selected = NULL;
	ImplAAFComponent	*tmpFound = NULL;
	aafLength_t	tmpMinLen;
	
	XPROTECT()
	{
	*foundObj = NULL;
	
	  CHECK(GetSelectedSegment(&selected));
	  if (selected)
		{
		  CHECK(mob->MobFindLeaf(track, mediaCrit, effectChoice,
							selected, rootPos, rootLen,
							prevObject, nextObject, 
							scopeStack, 
							currentObjPos, &tmpFound, &tmpMinLen, foundTransition,
							effeObject, nestDepth, diffPos));
		  if (tmpFound)
			{
			  *foundObj = tmpFound;
			  if (Int64Less(tmpMinLen, rootLen))
				*minLength = tmpMinLen;
			  else
				*minLength = rootLen;
			}
		  else
			{
			  RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
			}
		}
	  else
		{
		  RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
		}
	} /* XPROTECT */
  XEXCEPT
	{
	}
  XEND;

  return(AAFRESULT_SUCCESS);
}



AAFRESULT ImplAAFSelector::ChangeContainedReferences(aafMobID_constref from,
													aafMobID_constref to)
{
	aafInt32			n, count;
	ImplAAFSegment		*seg = NULL;
	ImplAAFComponent	*selected;
	
	XPROTECT()
	{
		CHECK(GetNumAlternateSegments (&count));
		for(n = 0; n < count; n++)
		{
			CHECK(GetNthSegment (n, &seg));
			CHECK(seg->ChangeContainedReferences(from, to));
			seg->ReleaseReference();
			seg = NULL;
		}

		selected = _selected;
		if(selected != NULL)
		{
			CHECK(selected->ChangeContainedReferences(from, to));
		}
	}
	XEXCEPT
	{
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}
