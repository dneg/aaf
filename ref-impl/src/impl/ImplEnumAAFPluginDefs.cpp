
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
      aafUInt32  /*count*/,
      ImplAAFPluginDescriptor ** /*ppAAFPluginDescriptors*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginDescriptors::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
