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

#ifndef __ImplAAFSelector_h__
#include "ImplAAFSelector.h"
#endif


#ifndef __ImplEnumAAFSegments_h__
#include "ImplEnumAAFSegments.h"
#endif

#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>

extern "C" const aafClassID_t CLSID_EnumAAFSegments;


ImplEnumAAFSegments::ImplEnumAAFSegments ()
{
	_current = 0;
	_pSelector = NULL;
}


ImplEnumAAFSegments::~ImplEnumAAFSegments ()
{
	if (_pSelector)
		_pSelector->ReleaseReference();
}


//***********************************************************
//
// NextOne()
//
// Enumerates to the next element in the enumerators list. The
// caller is responsible for properly releasing the returned pointer
// when it is no longer needed.
// 
// Succeeds if all of the following are true:
// - the ppSegment pointer is valid.
// - there are Segment objects remaining to be returned.
// 
// If this method fails nothing is written to *ppSegment.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSegment is null.
//
// AAFRESULT_NO_MORE_OBJECTS
//   - no Segments remaining to be returned.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFSegments::NextOne (ImplAAFSegment** ppSegment)
{
	AAFRESULT	result;
	aafInt32	cur = _current, siz;
		
	assert(_pSelector);

	if (ppSegment == NULL)
		return AAFRESULT_NULL_PARAM;

    _pSelector->GetNumAlternateSegments(&siz);
	if(cur < siz)
	{
		result = _pSelector->GetNthSegment(cur, ppSegment);
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
  // Enumerates the next count elements (AAFSegment pointers) in the
  // enumerator's list, returning them in the given array along with
  // the actual number of enumerated elements in pNumFetched. The caller
  // is responsible for properly releasing the returned pointers.
  // 
  // Succeeds if all of the following are true:
  // - The ppSegments pointer is valid.
  // - The pNumFetched pointer is valid. If count is 1, pNumFetched can be NULL.
  // - There are Mob objects remaining to be returned.
  // 
  // If this method fails nothing is written to *ppSegments or
  // pNumFetched.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - either ppSegments or pNumFetched is null.
  // 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFSegments::Next  (aafUInt32			count,
								ImplAAFSegment**	ppSegments,
								aafUInt32*			pFetched)
{
	ImplAAFSegment**	ppSegment;
	aafUInt32			numSegments;
	HRESULT				hr;

	if (pFetched == NULL && count != 1)
		return AAFRESULT_NULL_PARAM;

	// Point at the first component in the array.
	ppSegment = ppSegments;
	for (numSegments = 0; numSegments < count; numSegments++)
	{
		hr = NextOne(ppSegment);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numSegments == count-1, but the for loop should
		// prevent access to this location.
		ppSegment++;
	}
	
	if (pFetched)
		*pFetched = numSegments;

	return hr;
}


//***********************************************************
//
// Skip()
//
// Instructs the enumerator to skip the next count elements in the
// enumeration so that the next call to Next will not
// return those elements.
// 
// Succeeds if all of the following are true:
// - count is less than or equal to the number of remaining objects.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
//
// AAFRESULT_SUCCESS
//   - succeeded.
//
// AAFRESULT_NO_MORE_OBJECTS
//   - count exceeded number of remaining objects.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFSegments::Skip (aafUInt32 count)
{
	AAFRESULT	hr;
	aafInt32	newCurrent, size;

	newCurrent = _current + count;

    _pSelector->GetNumAlternateSegments(&size);
	if(newCurrent < size)
	{
		_current = newCurrent;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = AAFRESULT_NO_MORE_OBJECTS;
	}

	return hr;
}


//***********************************************************
//
// Reset()
//
// Instructs the enumerator to position itself at the beginning of the list of elements.
//
// There is no guarantee that the same set of elements will be enumerated on 
// each pass through the list, nor will the elements necessarily be enumerated in 
// the same order. The exact behavior depends on the collection being enumerated.
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFSegments::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


//***********************************************************
//
// Clone()
//
// Creates another segment enumerator with the same state as the current
// enumerator to iterate over the same list. This method makes it
// possible to record a point in the enumeration sequence in order
// to return to that point at a later time.
//
// Note: The caller must release this new enumerator separately from
// the first enumerator.
// 
// Succeeds if all of the following are true:
// - the ppEnum pointer is valid.
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
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFSegments::Clone (ImplEnumAAFSegments** ppEnum)
{
	ImplEnumAAFSegments*	theEnum;
	HRESULT					hr;

	if (ppEnum == NULL)
		return AAFRESULT_NULL_PARAM;
	
	theEnum = (ImplEnumAAFSegments *)CreateImpl(CLSID_EnumAAFSegments);
	if (theEnum == NULL)
		return E_FAIL;
		
	hr = theEnum->SetEnumSelector(_pSelector);
	if (SUCCEEDED(hr))
	{
		theEnum->Reset();
		theEnum->Skip(_current);
		*ppEnum = theEnum;
	}
	else
	{
		theEnum->ReleaseReference();
		*ppEnum = NULL;
	}

	return hr;
}

//***********************************************************
//
// SetEnumSelector()
//
// Internal method for use by the SDK.  Set the selector which
// will be enumerated over.
// 
// AAFRESULT_SUCCESS
//   - Successfuly set the selector.
// 
AAFRESULT
    ImplEnumAAFSegments::SetEnumSelector(ImplAAFSelector * pSelector)
{
	if (_pSelector)
		_pSelector->ReleaseReference();

	_pSelector = pSelector;
	_pSelector->AcquireReference();

	return AAFRESULT_SUCCESS;
}



