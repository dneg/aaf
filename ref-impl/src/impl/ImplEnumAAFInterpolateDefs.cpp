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

#ifndef __ImplAAFInterpolationDef_h__
#include "ImplAAFInterpolationDef.h"
#endif

#ifndef __ImplEnumAAFInterpolationDefs_h__
#include "ImplEnumAAFInterpolationDefs.h"
#endif

#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFInterpolationDefs;

ImplEnumAAFInterpolationDefs::ImplEnumAAFInterpolationDefs ()
: _enumObj(0), _iterator(0)
{
}


ImplEnumAAFInterpolationDefs::~ImplEnumAAFInterpolationDefs ()
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = NULL;

	delete _iterator;
	_iterator = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFInterpolationDefs::NextOne (
      ImplAAFInterpolationDef **ppInterpDef)
{
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;
	
	if (_iterator->before() || _iterator->valid())
	{
		if (++(*_iterator))
		{
			*ppInterpDef = _iterator->value();
			(*ppInterpDef)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
	}
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFInterpolationDefs::Next (
      aafUInt32  count,
      ImplAAFInterpolationDef **ppInterpDefs,
      aafUInt32 *pFetched)
{
	ImplAAFInterpolationDef**	ppDef;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppInterpDefs;
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
    ImplEnumAAFInterpolationDefs::Skip (
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
    ImplEnumAAFInterpolationDefs::Reset ()
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	_iterator->reset();
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFInterpolationDefs::Clone (
      ImplEnumAAFInterpolationDefs ** ppEnum)
{
	AAFRESULT				ar = AAFRESULT_SUCCESS;
	ImplEnumAAFInterpolationDefs		*result;

	result = (ImplEnumAAFInterpolationDefs *)CreateImpl(CLSID_EnumAAFInterpolationDefs);
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
    ImplEnumAAFInterpolationDefs::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator<ImplAAFInterpolationDef>* iterator)
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