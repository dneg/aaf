/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

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
	aafInt32			numElem;

	XPROTECT()
	{
		CHECK(_codec->GetFlavourCount(&numElem))
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
	aafInt32	newCurrent;
	aafInt32	numElem;

	hr = _codec->GetFlavourCount(&numElem);
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



