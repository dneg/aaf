/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "ImplAAFComponent.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFSequence.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>


extern "C" const aafClassID_t CLSID_EnumAAFComponents;

ImplEnumAAFComponents::ImplEnumAAFComponents ()
{
	_current = 0;
	_pSequence = NULL;
}


ImplEnumAAFComponents::~ImplEnumAAFComponents ()
{
	if (_pSequence)
		_pSequence->ReleaseReference();
}

//***********************************************************
//
// NextOne()
//
// Retrieves the next component in the sequence.
//
// ppComponents
//   - Points to the buffer in which to return the component.
//
//
// AAFRESULT_SUCCESS
//   - The next component is returned
//
// AAFRESULT_NO_MORE_OBJECTS
//   - There are no more components in the sequence
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::NextOne (ImplAAFComponent** ppComponent)
{
	AAFRESULT	result;
	aafInt32	cur = _current, siz;

    _pSequence->GetNumCpnts(&siz);
	if(cur < siz)
	{
		result = _pSequence->GetNthComponent(cur, ppComponent);
		_current = ++cur;
	}
	else
		result = AAFRESULT_NO_MORE_OBJECTS;

	return result;
}

//***********************************************************
//
// Next()
//
// Retrieves the specified number of items in the
// enumeration sequence.
// 
// count
//   - Specifies the number of components to return. If the number
//     of components requested is more than remains in the sequence,
//     only the remaining components are returned. The number of
//     components returned is passed through the ppComponents parameter
//     (unless it is NULL).
//
// ppComponents
//   - Points to the array in which to return the components.
//
// pFetched
//   - Points to the number of components actually returned in ppComponents.
//     The pFetched parameter cannot be NULL if count is greater than one.
//     If pFetched is NULL, count must be one.
//
// AAFRESULT_SUCCESS
//   - Returned requested number of elements
//
// S_FALSE/AAFRESULT_NO_MORE_OBJECTS
//   - Returned fewer elements than requested by count. In this case,
//     unused slots in the enumeration are not set to NULL and *pFetched
//     holds the number of valid entries, even if zero is returned.
//
// E_INVALIDARG
//   - Value of count is invalid. 
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Next (aafUInt32			count,
								 ImplAAFComponent**	ppComponents,
								 aafUInt32*			pFetched)
{
	ImplAAFComponent**	ppComponent;
	aafUInt32			numComps;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppComponent = ppComponents;
	for (numComps = 0; numComps < count; numComps++)
	{
		hr = NextOne(ppComponent);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppComponent++;
	}
	
	if (pFetched)
		*pFetched = numComps;

	return hr;
}

//***********************************************************
//
// Skip()
//
// Skips over a specified number of elements in the
// enumeration sequence.
// 
// AAFRESULT_SUCCESS
//   - The number of elements skipped is equal to count
//
// E_FAIL
//   - Not enough elements left in the sequence.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Skip (aafUInt32  count)
{
	AAFRESULT	hr;
	aafInt32	newCurrent, siz;

	newCurrent = _current + count;

    _pSequence->GetNumCpnts(&siz);
	if(newCurrent < siz)
	{
		_current = newCurrent;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

//***********************************************************
//
// Reset()
//
// Resets the enumeration sequence back to the beginning.
// 
// AAFRESULT_SUCCESS
//   - The enumeration sequence was reset to the beginning.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
// Clone()
//
// Returns another enumerator containing the same enumeration
// state as the current one.
// 
// AAFRESULT_SUCCESS
//   - Successfuly cloned the enumerator.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Clone (ImplEnumAAFComponents ** ppEnum)
{
	ImplEnumAAFComponents*	theEnum;
	HRESULT					hr;
	
	theEnum = (ImplEnumAAFComponents *)CreateImpl(CLSID_EnumAAFComponents);
	if (theEnum == NULL)
		return E_FAIL;
		
	hr = theEnum->SetEnumSequence(_pSequence);
	if (SUCCEEDED(hr))
	{
		theEnum->Reset();
		theEnum->Skip(_current);
		*ppEnum = theEnum;
	}
	else
	{
		theEnum->ReleaseRef();
		*ppEnum = NULL;
	}

	return hr;
}


//***********************************************************
//
// SetEnumSequence()
//
// Internal method for use by the SDK.  Set the sequence which
// will be enumerated over.
// 
// AAFRESULT_SUCCESS
//   - Successfuly set the sequence.
// 
AAFRESULT
    ImplEnumAAFComponents::SetEnumSequence(ImplAAFSequence * pSequence)
{
	if (_pSequence)
		return E_UNEXPECTED;

	_pSequence = pSequence;
	_pSequence->AcquireReference();

	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_EnumAAFComponents;

OMDEFINE_STORABLE(ImplEnumAAFComponents, CLSID_EnumAAFComponents);

