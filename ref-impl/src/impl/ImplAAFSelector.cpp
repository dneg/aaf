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
	ImplEnumAAFSegments*	theEnum = NULL;
	HRESULT					hr = AAFRESULT_SUCCESS;

	if (ppEnum == NULL)
	{
		hr = AAFRESULT_NULL_PARAM;
	}
	else
	{
		theEnum = (ImplEnumAAFSegments *)CreateImpl(CLSID_EnumAAFSegments);
		if (theEnum == NULL)
		{
			hr = E_FAIL;
		}
		else
		{
			hr = theEnum->SetEnumSelector(this);
			if (SUCCEEDED(hr))
			{
				theEnum->Reset();
				*ppEnum = theEnum;
			}
			else
			{
				theEnum->ReleaseReference();
				*ppEnum = NULL;
			}
		}
	}

	return hr;
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



OMDEFINE_STORABLE(ImplAAFSelector, AUID_AAFSelector);


