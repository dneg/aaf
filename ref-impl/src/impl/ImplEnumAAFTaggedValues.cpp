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

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#ifndef __ImplEnumAAFTaggedValues_h__
#include "ImplEnumAAFTaggedValues.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"

extern "C" const aafClassID_t CLSID_EnumAAFTaggedValues;


ImplEnumAAFTaggedValues::ImplEnumAAFTaggedValues ()
{
	_current = 0;
	_enumObj = NULL;
	_enumStrongProp = NULL;
}


ImplEnumAAFTaggedValues::~ImplEnumAAFTaggedValues ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTaggedValues::NextOne (ImplAAFTaggedValue** ppTaggedValue)
{
	aafUInt32			numElem;

	if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}

	if(ppTaggedValue == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;

	if(_enumStrongProp != NULL)
		_enumStrongProp->getValueAt(*ppTaggedValue, _current);
	else
		return AAFRESULT_NOT_INITIALIZED;

	(*ppTaggedValue)->AcquireReference();
	_current++;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTaggedValues::Next (aafUInt32 count, ImplAAFTaggedValue** ppTaggedValues, aafUInt32* pFetched)
{
	ImplAAFTaggedValue**	ppTemp;
	aafUInt32				numValues;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppTemp = ppTaggedValues;
	for (numValues = 0; numValues < count; numValues++)
	{
		hr = NextOne(ppTemp);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppTemp++;
	}
	
	if (pFetched)
		*pFetched = numValues;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTaggedValues::Skip (aafUInt32  count)
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
		return AAFRESULT_NOT_INITIALIZED;

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


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTaggedValues::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTaggedValues::Clone (ImplEnumAAFTaggedValues** ppEnum)
{
	ImplEnumAAFTaggedValues	*result;
	AAFRESULT				hr;

	result = (ImplEnumAAFTaggedValues *)CreateImpl(CLSID_EnumAAFTaggedValues);
	if (result == NULL)
		return E_FAIL;

	if(_enumStrongProp != NULL)
		hr = result->SetEnumStrongProperty(_enumObj, _enumStrongProp);
	else
		hr = AAFRESULT_NOT_INITIALIZED;

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
    ImplEnumAAFTaggedValues::SetEnumStrongProperty( ImplAAFObject *pObj, taggedValueStrongRefArrayProp_t *pProp)
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




