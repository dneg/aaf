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


#ifndef __ImplAAFCodecDef_h__
#include "ImplAAFCodecDef.h"
#endif





#ifndef __ImplEnumAAFCodecDefs_h__
#include "ImplEnumAAFCodecDefs.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"

extern "C" const aafClassID_t CLSID_EnumAAFCodecDefs;

ImplEnumAAFCodecDefs::ImplEnumAAFCodecDefs ()
: _enumObj(0), _iterator(0)
{
}


ImplEnumAAFCodecDefs::~ImplEnumAAFCodecDefs ()
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = NULL;

	delete _iterator;
	_iterator = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::NextOne (
      ImplAAFCodecDef **ppCodecDef)
{
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;
	
	if (_iterator->before() || _iterator->valid())
	{
		if (++(*_iterator))
		{
			*ppCodecDef = _iterator->value();
			(*ppCodecDef)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
	}
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Next (
      aafUInt32  count,
      ImplAAFCodecDef **ppCodecDefs,
      aafUInt32 *pFetched)
{
	ImplAAFCodecDef**	ppDef;
	aafUInt32			numDefs;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppCodecDefs;
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
    ImplEnumAAFCodecDefs::Skip (
      aafUInt32 count)
{
	AAFRESULT	ar = AAFRESULT_SUCCESS;
	aafUInt32	n;
	bool		offEnd;

	for(n = 1; n <= count; n++)
	{
		// Defined behavior of skip is to NOT advance at all if it would push us off of the end
		offEnd = ++(*_iterator);
		if(!offEnd)
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

	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Reset ()
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	_iterator->reset();
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Clone (
      ImplEnumAAFCodecDefs **ppEnum)
{
	AAFRESULT				ar = AAFRESULT_SUCCESS;
	ImplEnumAAFCodecDefs	*result;

	result = (ImplEnumAAFCodecDefs *)CreateImpl(CLSID_EnumAAFCodecDefs);
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
    ImplEnumAAFCodecDefs::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator<ImplAAFCodecDef>* iterator)
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