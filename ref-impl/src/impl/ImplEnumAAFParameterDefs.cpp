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


#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#ifndef __ImplEnumAAFParameterDefs_h__
#include "ImplEnumAAFParameterDefs.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFParameterDefs;

ImplEnumAAFParameterDefs::ImplEnumAAFParameterDefs ()
: _enumProp(0), _iterator(0)
{
	_current = 0;
	_enumObj = NULL;
	_enumProp = NULL;
}


ImplEnumAAFParameterDefs::~ImplEnumAAFParameterDefs ()
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = NULL;
	if (_iterator)
		delete _iterator;
	_iterator = NULL;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::NextOne (
      ImplAAFParameterDef **ppParameterDef)
{
	aafUInt32			numElem;
	aafUID_t			value;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;

	if(_enumProp != NULL)
		numElem = _enumProp->size() / sizeof(aafUID_t);
	else if(_iterator == NULL)
		return(AAFRESULT_INCONSISTANCY);

	if(ppParameterDef == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		if(_enumProp != NULL)
		{
			if(_current >= numElem)
				return AAFRESULT_NO_MORE_OBJECTS;
			_enumProp->getValueAt(&value, _current);
			CHECK(_enumObj->MyHeadObject(&head));
			CHECK(head->GetDictionary (&dict));
			CHECK(dict->LookupParameterDef (value, ppParameterDef));
			head->ReleaseReference();
			head = NULL;
			dict->ReleaseReference();
			dict = NULL;
			_current++;
			ar = AAFRESULT_SUCCESS;
		}
		else
		{
			if (_iterator->before() || _iterator->valid())
			{
				if (++(*_iterator))
				{
					*ppParameterDef = _iterator->value();
					(*ppParameterDef)->AcquireReference();
					ar = AAFRESULT_SUCCESS;
				}
			}
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
	
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Next (
      aafUInt32  count,
      ImplAAFParameterDef **ppParameterDefs,
      aafUInt32 * pFetched)
{
	ImplAAFParameterDef**	ppDef;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppParameterDefs;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(&ppDef[numDefs]);
		if (FAILED(hr))
			break;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Skip (
      aafUInt32  count)
{
	AAFRESULT	ar = AAFRESULT_SUCCESS;
	aafUInt32	newCurrent;
	aafUInt32	numElem, n;

	if(_enumProp != NULL)
	{
		numElem = _enumProp->size() / sizeof(aafUID_t);
		
		newCurrent = _current + count;
		
		if(newCurrent < numElem)
		{
			_current = newCurrent;
		}
		else
		{
			ar = E_FAIL;
		}
	}
	else
	{
		
		for(n = 1; n <= count; n++)
		{
			// Defined behavior of skip is to NOT advance at all if it would push us off of the end
			if(!++(*_iterator))
			{
				// Off the end, increment 'n' to match the iterator, then
				// decrement both back to the starting position
				n++;
				while(n >= 1)
				{
					--(*_iterator);
					n--;
				}
				break;
			}
		}
	}

	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Reset ()
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	if(_enumProp != NULL)
	{
		_current = 0;
	}
	else
	{
		_iterator->reset();
	}
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Clone (
      ImplEnumAAFParameterDefs **ppEnum)
{
	ImplEnumAAFParameterDefs	*result;
	AAFRESULT					ar = AAFRESULT_SUCCESS;
	
	result = (ImplEnumAAFParameterDefs *)CreateImpl(CLSID_EnumAAFParameterDefs);
	if (result == NULL)
		return E_FAIL;
	
	if(_enumProp != NULL)
	{
		ar = result->SetEnumProperty(_enumObj, _enumProp);
		if (SUCCEEDED(ar))
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
	}
	else
	{
		ar = result->SetIterator(_enumObj,_iterator->copy());
		if (SUCCEEDED(ar))
		{
			*ppEnum = result;
		}
		else
		{
			result->ReleaseReference();
			result = 0;
			*ppEnum = NULL;
		}
	}
	
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::SetEnumProperty( ImplAAFObject *pObj, parmDefWeakRefArrayProp_t *pProp)
{
	if (_enumObj)
	  _enumObj->ReleaseReference();
	_enumObj = 0;
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	_enumProp = pProp;				// Don't refcount, same lifetime as the object.

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator<ImplAAFParameterDef>* iterator)
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	
	_enumProp = NULL;
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = 0;
	
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	
	delete _iterator;
	_iterator = iterator;
	return ar;
}
