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
// Enumerates to the next element in the enumerators list. The
// caller is responsible for properly releasing the returned pointer
// when it is no longer needed.
// 
// Succeeds if all of the following are true:
// - the ppComponent pointer is valid.
// - there are Component objects remaining to be returned.
// 
// If this method fails nothing is written to *ppComponent.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppComponent is null.
//
// AAFRESULT_NO_MORE_OBJECTS
//   - no Components remaining to be returned.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::NextOne (ImplAAFComponent** ppComponent)
{
	AAFRESULT	result;
	aafInt32	cur = _current, siz;

	if (ppComponent == NULL)
		return AAFRESULT_NULL_PARAM;

    _pSequence->GetNumComponents(&siz);
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
  // Enumerates the next count elements (AAFComponent pointers) in the
  // enumerator's list, returning them in the given array along with
  // the actual number of enumerated elements in pNumFetched. The caller
  // is responsible for properly releasing the returned pointers.
  // 
  // Succeeds if all of the following are true:
  // - The ppMobs pointer is valid.
  // - The pNumFetched pointer is valid. If count is 1, pNumFetched can be NULL.
  // - There are Mob objects remaining to be returned.
  // 
  // If this method fails nothing is written to *ppComponents or
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
  //   - either ppCompoents or pNumFetched is null.
  // 
AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Next (aafUInt32			count,
								 ImplAAFComponent**	ppComponents,
								 aafUInt32*			pFetched)
{
	ImplAAFComponent**	ppComponent;
	aafUInt32			numComps;
	HRESULT				hr;

	if (pFetched == NULL && count != 1)
		return AAFRESULT_NULL_PARAM;

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
    ImplEnumAAFComponents::Skip (aafUInt32  count)
{
	AAFRESULT	hr;
	aafInt32	newCurrent, siz;

	newCurrent = _current + count;

    _pSequence->GetNumComponents(&siz);
	if(newCurrent < siz)
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
    ImplEnumAAFComponents::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
// Clone()
//
// Creates another component enumerator with the same state as the current
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
    ImplEnumAAFComponents::Clone (ImplEnumAAFComponents ** ppEnum)
{
	ImplEnumAAFComponents*	theEnum;
	HRESULT					hr;

	if (ppEnum == NULL)
		return AAFRESULT_NULL_PARAM;
	
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
		theEnum->ReleaseReference();
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
		_pSequence->ReleaseReference();

	_pSequence = pSequence;
	_pSequence->AcquireReference();

	return AAFRESULT_SUCCESS;
}
