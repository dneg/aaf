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
		CHECK(_cPluginDesc->CountLocators (&siz));
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

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFPluginLocators::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent, siz;

	newCurrent = _current + count;

    _cPluginDesc->CountLocators(&siz);
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
    ImplEnumAAFPluginLocators::SetPluginDescriptor(ImplAAFPluginDef *pPDesc)
{
	if (_cPluginDesc)
	  _cPluginDesc->ReleaseReference();
	_cPluginDesc = 0;

	_cPluginDesc = pPDesc;

	if (pPDesc)
		pPDesc->AcquireReference();
	return AAFRESULT_SUCCESS;
}

