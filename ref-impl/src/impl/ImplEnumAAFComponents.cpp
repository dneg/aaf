/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
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
#include "ImplAAFComponent.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFSequence.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

#include <assert.h>


extern "C" const aafClassID_t CLSID_EnumAAFComponents;

ImplEnumAAFComponents::ImplEnumAAFComponents ()
{
	_current = 0;
	_enumObj = NULL;
	_enumStrongProp = NULL;
}


ImplEnumAAFComponents::~ImplEnumAAFComponents ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
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
	aafUInt32			numElem;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	else
		return(AAFRESULT_INCONSISTANCY);

	if(ppComponent == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;
	XPROTECT()
	{
		if(_enumStrongProp != NULL)
		{
			_enumStrongProp->getValueAt(*ppComponent, _current);
			(*ppComponent)->AcquireReference();
		}
		else
			RAISE(AAFRESULT_INCONSISTANCY);

		_current++;
		if (head) {
			head->ReleaseReference();
			head = NULL;
		}
		if (dict) {
			dict->ReleaseReference();
			dict = NULL;
		}
	}
	XEXCEPT
	{
		if(head)
		  head->ReleaseReference();
		head = 0;
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
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
	ImplAAFComponent**	ppDef;
	aafUInt32			numDefs;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppComponents;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(ppDef);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppDef++;
	}
	
	if (pFetched)
		*pFetched = numDefs;

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
	aafUInt32	newCurrent;
	aafUInt32	numElem;
	if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	else
		return(AAFRESULT_INCONSISTANCY);

	newCurrent = _current + count;

	if(newCurrent < numElem)
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
	ImplEnumAAFComponents	*result;
	AAFRESULT				hr;

	result = (ImplEnumAAFComponents *)CreateImpl(CLSID_EnumAAFComponents);
	if (result == NULL)
		return E_FAIL;

	if(_enumStrongProp != NULL)
		hr = result->SetEnumStrongProperty(_enumObj, _enumStrongProp);
	else
		return(AAFRESULT_INCONSISTANCY);

	if (SUCCEEDED(hr))
	{
		result->_current = _current;
		*ppEnum = result;
	}
	else
	{
	  result->ReleaseReference();
	  result = 0;
	  *ppEnum = NULL;
	}
	
	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::SetEnumStrongProperty( ImplAAFObject *pObj, ComponentStrongRefArrayProp_t *pProp)
{
	if (_enumObj)
	  _enumObj->ReleaseReference();
	_enumObj = 0;
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumStrongProp = pProp;				// Don't refcount, same lifetime as the object.

	return AAFRESULT_SUCCESS;
}




