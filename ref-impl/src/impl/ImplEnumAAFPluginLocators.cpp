/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\************************************************/


#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif


#ifndef __ImplEnumAAFPluginLocators_h__
#include "ImplEnumAAFPluginLocators.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginLocators;

ImplEnumAAFPluginLocators::ImplEnumAAFPluginLocators ()
{
	_current = 0;
	_cPluginDesc = NULL;
}


ImplEnumAAFPluginLocators::~ImplEnumAAFPluginLocators ()
{
	if (_cPluginDesc)
	{
		_cPluginDesc->ReleaseReference();
		_cPluginDesc = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginLocators::NextOne (
      ImplAAFLocator **ppAAFLocator)
{
	aafNumSlots_t	cur = _current, siz;

    XPROTECT()
	{
		CHECK(_cPluginDesc->GetNumLocators (&siz));
		if(cur < siz)
		{
			CHECK(_cPluginDesc->GetNthLocator (cur, ppAAFLocator));
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
    ImplEnumAAFPluginLocators::Next (
      aafUInt32  count,
      ImplAAFLocator ** ppAAFLocators,
      aafUInt32 *  pFetched)
{
	ImplAAFLocator**	ppLoc;
	aafUInt32			numLoc;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppLoc = ppAAFLocators;
	for (numLoc = 0; numLoc < count; numLoc++)
	{
		hr = NextOne(ppLoc);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppLoc++;
	}
	
	if (pFetched)
		*pFetched = numLoc;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginLocators::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent, siz;

	newCurrent = _current + count;

    _cPluginDesc->GetNumLocators(&siz);
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
    ImplEnumAAFPluginLocators::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginLocators::Clone (
      ImplEnumAAFPluginLocators **ppEnum)
{
	ImplEnumAAFPluginLocators		*result;
	AAFRESULT		hr;

	result = (ImplEnumAAFPluginLocators *)CreateImpl(CLSID_EnumAAFPluginLocators);
	if (result == NULL)
		return E_FAIL;

	hr = result->SetPluginDescriptor(_cPluginDesc);
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



AAFRESULT
    ImplEnumAAFPluginLocators::SetPluginDescriptor(ImplAAFPluginDescriptor *pPDesc)
{
	if (_cPluginDesc)
	  _cPluginDesc->ReleaseReference();
	_cPluginDesc = 0;

	_cPluginDesc = pPDesc;

	if (pPDesc)
		pPDesc->AcquireReference();
	return AAFRESULT_SUCCESS;
}

