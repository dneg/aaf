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

#ifndef __ImplEnumAAFCodecFlavours_h__
#include "ImplEnumAAFCodecFlavours.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFCodecFlavours;

ImplEnumAAFCodecFlavours::ImplEnumAAFCodecFlavours ()
{
	_current = 0;
	_codec = NULL;
}


ImplEnumAAFCodecFlavours::~ImplEnumAAFCodecFlavours ()
{
	if (_codec)
	{
		_codec->Release();
		_codec = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::NextOne (
      aafUID_t *pAAFCodecFlavour)
{
	aafUInt32			numElem;

	XPROTECT()
	{
		CHECK(_codec->CountFlavours(&numElem))
		if(pAAFCodecFlavour == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		if(_current >= numElem)
			RAISE(AAFRESULT_NO_MORE_OBJECTS);
		CHECK(_codec->GetIndexedFlavourID (_current, pAAFCodecFlavour));

		_current++;
	}
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Next (
      aafUInt32  count,
      aafUID_t *  pAAFCodecFlavours,
      aafUInt32 *  pFetched)
{
	aafUID_t*			pDef;
	aafUInt32			numDefs;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	pDef = pAAFCodecFlavours;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(pDef);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		pDef++;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem;

	hr = _codec->CountFlavours(&numElem);
	if(hr != AAFRESULT_SUCCESS)
		return hr;

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
    ImplEnumAAFCodecFlavours::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecFlavours::Clone (
      ImplEnumAAFCodecFlavours **ppEnum)
{
	ImplEnumAAFCodecFlavours	*result;
	AAFRESULT				hr;

	result = (ImplEnumAAFCodecFlavours *)CreateImpl(CLSID_EnumAAFCodecFlavours);
	if (result == NULL)
		return E_FAIL;

	hr = result->SetEnumCodec(_codec);

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
    ImplEnumAAFCodecFlavours::SetEnumCodec( IAAFEssenceCodec *pCodec)
{
	if (_codec)
		_codec->Release();
	_codec = pCodec;
	if (pCodec)
		pCodec->AddRef();
	/**/

	return AAFRESULT_SUCCESS;
}



