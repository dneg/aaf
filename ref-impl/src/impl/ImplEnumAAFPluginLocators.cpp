/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
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
      aafUInt32  /*count*/,
      ImplAAFLocator ** /*ppAAFLocators*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginLocators::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
		*ppEnum = NULL;
	}
	
	return hr;
}



AAFRESULT
    ImplEnumAAFPluginLocators::SetPluginDescriptor(ImplAAFPluginDescriptor *pPDesc)
{
	if (_cPluginDesc)
		_cPluginDesc->ReleaseReference();

	_cPluginDesc = pPDesc;

	if (pPDesc)
		pPDesc->AcquireReference();
	return AAFRESULT_SUCCESS;
}

