/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif






#ifndef __ImplEnumAAFLocators_h__
#include "ImplEnumAAFLocators.h"
#endif

#include <assert.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFLocators;

ImplEnumAAFLocators::ImplEnumAAFLocators ()
{
	_current = 0;
	_cEssenceDesc = NULL;
}


ImplEnumAAFLocators::~ImplEnumAAFLocators ()
{
	if (_cEssenceDesc)
	{
		_cEssenceDesc->ReleaseReference();
		_cEssenceDesc = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::NextOne (ImplAAFLocator **ppLocator)
{
	aafNumSlots_t	cur = _current, siz;

    XPROTECT()
	{
		CHECK(_cEssenceDesc->GetNumLocators (&siz));
		if(cur < siz)
		{
			CHECK(_cEssenceDesc->GetNthLocator (cur, ppLocator));
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
    ImplEnumAAFLocators::Next (aafUInt32  /*count*/,
                           ImplAAFLocator ** /*ppLocators*/,
                           aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Skip (aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Clone (ImplEnumAAFLocators **ppEnum)
{
	ImplEnumAAFLocators		*result;
	AAFRESULT		hr;

	result = (ImplEnumAAFLocators *)CreateImpl(CLSID_EnumAAFLocators);
	if (result == NULL)
		return E_FAIL;

	hr = result->SetEssenceDesc(_cEssenceDesc);
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
    ImplEnumAAFLocators::SetEssenceDesc(ImplAAFEssenceDescriptor *pEDesc)
{
	if (_cEssenceDesc)
		_cEssenceDesc->ReleaseReference();

	_cEssenceDesc = pEDesc;

	if (pEDesc)
		pEDesc->AcquireReference();
	return AAFRESULT_SUCCESS;
}


extern "C" const aafClassID_t CLSID_EnumAAFLocators;

OMDEFINE_STORABLE(ImplEnumAAFLocators, CLSID_EnumAAFLocators);

