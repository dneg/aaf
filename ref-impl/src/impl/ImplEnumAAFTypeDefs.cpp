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


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplEnumAAFTypeDefs_h__
#include "ImplEnumAAFTypeDefs.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFTypeDefs;

ImplEnumAAFTypeDefs::ImplEnumAAFTypeDefs ()
: _enumObj(0), _iterator(0)
{
}


ImplEnumAAFTypeDefs::~ImplEnumAAFTypeDefs ()
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = 0;

	delete _iterator;
	_iterator = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTypeDefs::NextOne (
      ImplAAFTypeDef **ppTypeDef)
{
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;
	
	if (_iterator->before() || _iterator->valid())
	{
		if (++(*_iterator))
		{
			*ppTypeDef = _iterator->value();
			(*ppTypeDef)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
	}
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTypeDefs::Next (
      aafUInt32  count,
      ImplAAFTypeDef **ppTypeDefs,
      aafUInt32 *pFetched)
{
	aafUInt32 numDefs;
	AAFRESULT  ar = AAFRESULT_SUCCESS;
	
	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;
	
	// Point at the first component in the array.
	// Point at the first component in the array.
	ImplAAFTypeDef** ppDef = ppTypeDefs;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		ar = NextOne(&ppDef[numDefs]);
		if (FAILED(ar))
			break;
	}
	
	if (pFetched)
		*pFetched = numDefs;
	
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTypeDefs::Skip (
      aafUInt32  count)
{
	AAFRESULT	ar = AAFRESULT_SUCCESS;
	aafUInt32	n;
	
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

	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTypeDefs::Reset ()
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	_iterator->reset();
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFTypeDefs::Clone (
      ImplEnumAAFTypeDefs **ppEnum)
{
	AAFRESULT				ar = AAFRESULT_SUCCESS;
	ImplEnumAAFTypeDefs		*result;

	result = (ImplEnumAAFTypeDefs *)CreateImpl(CLSID_EnumAAFTypeDefs);
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
    ImplEnumAAFTypeDefs::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator<ImplAAFTypeDef>* iterator)
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