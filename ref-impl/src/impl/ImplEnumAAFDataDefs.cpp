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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif







#ifndef __ImplEnumAAFDataDefs_h__
#include "ImplEnumAAFDataDefs.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFHeader.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"

extern "C" const aafClassID_t CLSID_EnumAAFDataDefs;

ImplEnumAAFDataDefs::ImplEnumAAFDataDefs ()
: _enumObj(0), _iterator(0)
{
}


ImplEnumAAFDataDefs::~ImplEnumAAFDataDefs ()
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = NULL;
	if (_iterator)
		delete _iterator;
	_iterator = NULL;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::NextOne (
      ImplAAFDataDef **ppDataDef)
{
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;

	if(_iterator == NULL)
		return(AAFRESULT_INCONSISTANCY);

	if(ppDataDef == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_iterator->before() || _iterator->valid())
	{
		if (++(*_iterator))
		{
			*ppDataDef = _iterator->value();
			(*ppDataDef)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
	}
	
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Next (
      aafUInt32  count,
      ImplAAFDataDef ** ppDataDefs,
      aafUInt32 *pFetched)
{
	ImplAAFDataDef**		ppDef;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppDataDefs;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(&ppDef[numDefs]);
		if (FAILED(hr))
			break;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Skip (
      aafUInt32  count)
{
	AAFRESULT	ar = AAFRESULT_SUCCESS;
	aafUInt32	n;

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

	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Reset ()
{
	AAFRESULT ar = AAFRESULT_SUCCESS;

	_iterator->reset();

	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::Clone (
      ImplEnumAAFDataDefs **ppEnum)
{
	ImplEnumAAFDataDefs	*result;
	AAFRESULT			ar = AAFRESULT_SUCCESS;
	
	result = (ImplEnumAAFDataDefs *)CreateImpl(CLSID_EnumAAFDataDefs);
	if (result == NULL)
		return E_FAIL;
	
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
	
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFDataDefs::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator<ImplAAFDataDef>* iterator)
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
