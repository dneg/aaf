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
 *  prior written permission of Avid Technology, Inc.
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
	  selected = 0;
	}

	size_t size = _alternates.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFSegment* pSegment = _alternates.setValueAt(0, i);
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
		  pPrevSelected = 0;
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



