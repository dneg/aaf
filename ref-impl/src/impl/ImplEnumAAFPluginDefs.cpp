
/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/


#ifndef __ImplAAFPluginDescriptor_h__
#include "ImplAAFPluginDescriptor.h"
#endif





#ifndef __ImplEnumAAFPluginDescriptors_h__
#include "ImplEnumAAFPluginDescriptors.h"
#endif

#include "ImplAAFPluggableDef.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginDescriptors;

ImplEnumAAFPluginDescriptors::ImplEnumAAFPluginDescriptors ()
{
	_current = 0;
	_cPluggableDef = NULL;
}


ImplEnumAAFPluginDescriptors::~ImplEnumAAFPluginDescriptors ()
{
	if (_cPluggableDef)
	{
		_cPluggableDef->ReleaseReference();
		_cPluggableDef = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::NextOne (
      ImplAAFPluginDescriptor **ppAAFPluginDescriptor)
{
	aafNumSlots_t	cur = _current, siz;

    XPROTECT()
	{
		CHECK(_cPluggableDef->GetNumDescriptors (&siz));
		if(cur < siz)
		{
			CHECK(_cPluggableDef->GetNthDescriptor (cur, ppAAFPluginDescriptor));
			_current = ++cur;
		}
		else
			RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


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
		hr = NextOne(ppDesc);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppDesc++;
	}
	
	if (pFetched)
		*pFetched = numDesc;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafInt32	newCurrent, siz;

	newCurrent = _current + count;

    _cPluggableDef->GetNumDescriptors(&siz);
	if(newCurrent < siz)
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
    ImplEnumAAFPluginDescriptors::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Clone (
      ImplEnumAAFPluginDescriptors **ppEnum)
{
	ImplEnumAAFPluginDescriptors		*result;
	AAFRESULT		hr;

	result = (ImplEnumAAFPluginDescriptors *)CreateImpl(CLSID_EnumAAFPluginDescriptors);
	if (result == NULL)
		return E_FAIL;

	hr = result->SetPluggableDef(_cPluggableDef);
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


AAFRESULT
    ImplEnumAAFPluginDescriptors::SetPluggableDef(ImplAAFPluggableDef *pEDesc)
{
	if (_cPluggableDef)
		_cPluggableDef->ReleaseReference();

	_cPluggableDef = pEDesc;

	if (pEDesc)
		pEDesc->AcquireReference();
	return AAFRESULT_SUCCESS;
}
