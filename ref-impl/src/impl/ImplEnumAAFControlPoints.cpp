/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/

#ifndef __ImplAAFControlPoint_h__
#include "ImplAAFControlPoint.h"
#endif





#ifndef __ImplEnumAAFControlPoints_h__
#include "ImplEnumAAFControlPoints.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"

extern "C" const aafClassID_t CLSID_EnumAAFControlPoints;

ImplEnumAAFControlPoints::ImplEnumAAFControlPoints ()
{
	_current = 0;
	_enumObj = NULL;
	_enumStrongProp = NULL;
}


ImplEnumAAFControlPoints::~ImplEnumAAFControlPoints ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFControlPoints::NextOne (
      ImplAAFControlPoint **ppControlPoint)
{
	aafUInt32			numElem;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;
	size_t	siz;
		
	_enumStrongProp->getSize(siz);
	numElem = siz;

	if(ppControlPoint == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;
	_enumStrongProp->getValueAt(*ppControlPoint, _current);
	(*ppControlPoint)->AcquireReference();
	_current++;
	if (head)
	{
		head->ReleaseReference();
		head = NULL;
	}
	if (dict)
	{
		dict->ReleaseReference();
		dict = NULL;
	}

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFControlPoints::Next (
      aafUInt32  count,
      ImplAAFControlPoint ** ppControlPoints,
      aafUInt32 * pFetched)
{
	ImplAAFControlPoint**	ppDef;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppControlPoints;
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


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFControlPoints::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem;
	size_t	siz;
		
	_enumStrongProp->getSize(siz);
	numElem = siz;

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
    ImplEnumAAFControlPoints::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFControlPoints::Clone (
      ImplEnumAAFControlPoints **ppEnum)
{
	ImplEnumAAFControlPoints	*result;
	AAFRESULT				hr;

	result = (ImplEnumAAFControlPoints *)CreateImpl(CLSID_EnumAAFControlPoints);
	if (result == NULL)
		return E_FAIL;

	hr = result->SetEnumStrongProperty(_enumObj, _enumStrongProp);
	if (SUCCEEDED(hr))
	{
		result->_current = _current;
		*ppEnum = result;
	}
	else
	{
		result->ReleaseReference();
		*ppEnum = NULL;
	}
	
	return hr;
}




AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFControlPoints::SetEnumStrongProperty( ImplAAFObject *pObj, ControlPointStrongRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumStrongProp = pProp;				// Don't refcount, same lifetime as the object.

	return AAFRESULT_SUCCESS;
}




