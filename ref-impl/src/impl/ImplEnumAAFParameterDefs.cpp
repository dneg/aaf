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
: _iterator(0)
{
	_enumObj = NULL;
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
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;

	if(_iterator == NULL)
		return(AAFRESULT_INCONSISTANCY);

	if(ppParameterDef == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_iterator->before() || _iterator->valid())
	{
		if (++(*_iterator))
		{
			OMObject* object = _iterator->currentObject();
			ImplAAFParameterDef* obj = dynamic_cast<ImplAAFParameterDef*>(object);
			// assert(obj != 0); // tjb - consistent way to handle this ?
			*ppParameterDef = obj;
			(*ppParameterDef)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
	}
	
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

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Skip (
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
    ImplEnumAAFParameterDefs::Reset ()
{
	AAFRESULT ar = AAFRESULT_SUCCESS;
	_iterator->reset();
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
    ImplEnumAAFParameterDefs::SetIterator(
                        ImplAAFObject *pObj,
                        OMReferenceContainerIterator* iterator)
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
