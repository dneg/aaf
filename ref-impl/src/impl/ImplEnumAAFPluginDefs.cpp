
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


#ifndef __ImplAAFPluginDescriptor_h__
#include "ImplAAFPluginDescriptor.h"
#endif





#ifndef __ImplEnumAAFPluginDescriptors_h__
#include "ImplEnumAAFPluginDescriptors.h"
#endif

#include "ImplAAFDefObject.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"
#include "aafUtils.h"
#include "AAFDefUIDs.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginDescriptors;

ImplEnumAAFPluginDescriptors::ImplEnumAAFPluginDescriptors ()
: _enumObj(0), _iterator(0)
{
	_current = 0;
	_enumProp = NULL;
}


ImplEnumAAFPluginDescriptors::~ImplEnumAAFPluginDescriptors ()
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = NULL;

	if (_iterator)
		delete _iterator;
	_iterator = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::NextOne (
      ImplAAFPluginDescriptor **ppAAFPluginDescriptor)
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

	if(ppAAFPluginDescriptor == NULL)
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
			CHECK(dict->LookupPluginDef(value, ppAAFPluginDescriptor));
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
					*ppAAFPluginDescriptor = _iterator->value();
					(*ppAAFPluginDescriptor)->AcquireReference();
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

	return(AAFRESULT_SUCCESS); 
	
	return ar;}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Next (
      aafUInt32  count,
      ImplAAFPluginDescriptor ** ppAAFPluginDescriptors,
      aafUInt32 * pFetched)
{
	ImplAAFPluginDescriptor**	ppDesc;
	aafUInt32			numDesc;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDesc = ppAAFPluginDescriptors;
	for (numDesc = 0; numDesc < count; numDesc++)
	{
		hr = NextOne(&ppDesc[numDesc]);
		if (FAILED(hr))
			break;
	}
	
	if (pFetched)
		*pFetched = numDesc;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Skip (
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
				// Off the end, decrement n and iterator back to the starting position
				while(n >= 1)
				{
					--(*_iterator);
					n--;
				}
				ar = AAFRESULT_NO_MORE_OBJECTS;
				break;
			}
		}
	}

	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Reset ()
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
    ImplEnumAAFPluginDescriptors::Clone (
      ImplEnumAAFPluginDescriptors **ppEnum)
{
	ImplEnumAAFPluginDescriptors	*result;
	AAFRESULT			ar = AAFRESULT_SUCCESS;
	
	result = (ImplEnumAAFPluginDescriptors *)CreateImpl(CLSID_EnumAAFPluginDescriptors);
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
    ImplEnumAAFPluginDescriptors::SetEnumProperty( ImplAAFObject *pObj, DataDefWeakRefArrayProp_t *pProp)
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
    ImplEnumAAFPluginDescriptors::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator<ImplAAFPluginDescriptor>* iterator)
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	
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