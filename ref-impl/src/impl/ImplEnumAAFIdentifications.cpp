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
 * prior written permission of Avid Technology, Inc.
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


#ifndef __ImplAAFIdentification_h__
#include "ImplAAFIdentification.h"
#endif




#ifndef __ImplEnumAAFIdentifications_h__
#include "ImplEnumAAFIdentifications.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"

extern "C" const aafClassID_t CLSID_EnumAAFIdentifications;

ImplEnumAAFIdentifications::ImplEnumAAFIdentifications ()
{
	_current = 0;
	_enumObj = NULL;
	_enumProp = NULL;
	_enumStrongProp = NULL;
}


ImplEnumAAFIdentifications::~ImplEnumAAFIdentifications ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFIdentifications::NextOne (
      ImplAAFIdentification **ppIdentification)
{
	aafUInt32			numElem;
	ImplAAFDictionary	*dict = NULL;

	if(_enumProp != NULL)
		numElem = _enumProp->size() / sizeof(aafUID_t);
	else if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	else
		return(AAFRESULT_INCONSISTANCY);

	if(ppIdentification == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;
	XPROTECT()
	{
//!!!		if(_enumProp != NULL)
//		{
//			_enumProp->getValueAt(&value, _current);
//			CHECK(GetDictionary (&dict));
//			CHECK(dict->LookupParameterDefinition(&value, ppIdentification));
//			dict->ReleaseReference();
//			dict = NULL;
//		}
//		else
			if(_enumStrongProp != NULL)
		{
			_enumStrongProp->getValueAt(*ppIdentification, _current);
			(*ppIdentification)->AcquireReference();
		}
		else
			RAISE(AAFRESULT_INCONSISTANCY);
		_current++;
		if (dict) {
			dict->ReleaseReference();
			dict = NULL;
		}
	}
	XEXCEPT
	{
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFIdentifications::Next (
      aafUInt32  count,
      ImplAAFIdentification ** ppIdentifications,
      aafUInt32 * pNumFetched)
{
	ImplAAFIdentification**	ppDef;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pNumFetched == NULL && count != 1) || (pNumFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppIdentifications;
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
	
	if (pNumFetched)
		*pNumFetched = numDefs;

	return hr;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFIdentifications::Skip (
      aafUInt32 count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem;

//!!!	if(_enumProp != NULL)
//		numElem = _enumProp->size() / sizeof(aafUID_t);
//	else
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



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFIdentifications::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFIdentifications::Clone (
      ImplEnumAAFIdentifications **ppEnum)
{
	ImplEnumAAFIdentifications	*result;
	AAFRESULT					hr;

	result = (ImplEnumAAFIdentifications *)CreateImpl(CLSID_EnumAAFIdentifications);
	if (result == NULL)
		return E_FAIL;

//!!!	if(_enumProp != NULL)
//		hr = result->SetEnumProperty(_enumObj, _enumProp);
//	else
		if(_enumStrongProp != NULL)
		hr = result->SetEnumStrongProperty(_enumObj, _enumStrongProp);
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
    ImplEnumAAFIdentifications::SetEnumProperty( ImplAAFObject *pObj, identWeakRefArrayProp_t *pProp)
{
	if (_enumObj)
	  _enumObj->ReleaseReference();
	_enumObj = 0;
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	_enumProp = pProp;				// Don't refcount, same lifetime as the object.
	_enumStrongProp = NULL;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFIdentifications::SetEnumStrongProperty( ImplAAFObject *pObj, identStrongRefArrayProp_t *pProp)
{
	if (_enumObj)
	  _enumObj->ReleaseReference();
	_enumObj = 0;
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumStrongProp = pProp;		// Don't refcount, same lifetime as the object.
	_enumProp = NULL;

	return AAFRESULT_SUCCESS;
}
