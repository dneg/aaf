/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


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
	_selected(		PID_Selector_Selected,		"Selected"),
	_alternates(	PID_Selector_Alternates,	"Alternates")
{
	_persistentProperties.put(_selected.address());
	_persistentProperties.put(_alternates.address());
}


ImplAAFSelector::~ImplAAFSelector ()
{
	ImplAAFSegment *selected = _selected.setValue(0);
	if (selected != NULL)
	{
		selected->ReleaseReference();
	}

	size_t size = _alternates.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFSegment* pSegment = _alternates.setValueAt(0, i);
		if (pSegment)
		{
			pSegment->ReleaseReference();
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
	HRESULT				hr = AAFRESULT_SUCCESS;
	ImplAAFSegment*		pPrevSelected = NULL;

	if (pSelSegment == NULL)
	{
		hr = AAFRESULT_NULL_PARAM;
	}
	else
	{
		pPrevSelected = _selected;
		if (pPrevSelected)
		{
			pPrevSelected->ReleaseReference();
		}
		_selected = pSelSegment;
		_selected->AcquireReference();
	}

	return hr;
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
	size_t	numSegments;

	if (pNumSegments == NULL)
	{
		hr = AAFRESULT_NULL_PARAM;
	}
	else
	{
		_alternates.getSize(numSegments);
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
	(*ppEnum)->SetEnumStrongProperty(this, &_alternates);

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
	size_t			numSegments;
	HRESULT			hr;

	_alternates.getSize(numSegments);
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
#if 0
aafErr_t ImplAAFSelector::Verify(char *buf, validateData_t *result)
{

	AAFSegment * slot = NULL;
	aafLength_t parentLen, slotLen, selectedLen;
	char	 parentLenBuf[32], slotLenBuf[32], selectedLenBuf[32];
	aafInt32 numSlots, loop;
	
	AAFIterate	*iter;
	
	XPROTECT(_file)
	{
		/* Verify length of slots == length of parent */
			CHECK(GetLength(&parentLen));
			CHECK(GetLength(&selectedLen));
		if (Int64NotEqual(selectedLen, parentLen))
		{
		      CHECK(Int64ToString(selectedLen, 10, sizeof(selectedLenBuf), selectedLenBuf));  
		      CHECK(Int64ToString(parentLen, 10, sizeof(parentLenBuf), parentLenBuf));  
			fprintf(result->textOut, "*** ERROR: %s Selector length (%s) != length of selected slot (%s)\n", 
				buf, selectedLenBuf, parentLenBuf);
			result->numErrors++;
		}

		iter = new AAFIterate(_file);
		CHECK(GetNumAltSlots(&numSlots));
		for (loop = 1; loop <= numSlots; loop++)
		{
			CHECK(iter->SelectorGetNextAltSlot(this, NULL, &slot));
			if (slot)
			{
			    CHECK(slot->GetLength(&slotLen));
				if (Int64NotEqual(parentLen, slotLen))
				{
		      		CHECK(Int64ToString(parentLen, 10, sizeof(parentLenBuf), parentLenBuf));  
		      		CHECK(Int64ToString(slotLen, 10, sizeof(slotLenBuf), slotLenBuf));  
					fprintf(result->textOut, "*** ERROR: %s Selector length (%s) != length of Selector Alternate slot (%s)\n", 
						buf, parentLenBuf, slotLenBuf);
					result->numErrors++;
				}
			}
		} /* for */

		delete iter;
		iter = NULL;
	 	} /* XPROTECT */
  XEXCEPT
	{
	  if (iter)
		delete iter;
	}
  XEND;

  return(AAFRESULT_SUCCESS);
}
#endif

OMDEFINE_STORABLE(ImplAAFSelector, AUID_AAFSelector);


