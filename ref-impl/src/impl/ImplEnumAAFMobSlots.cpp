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


#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif






#ifndef __ImplEnumAAFMobSlots_h__
#include "ImplEnumAAFMobSlots.h"
#endif

#include <assert.h>

#include "ImplAAFMob.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFMobSlots;

ImplEnumAAFMobSlots::ImplEnumAAFMobSlots ()
{
	_current = 0;
	_mob = NULL;
}


ImplEnumAAFMobSlots::~ImplEnumAAFMobSlots ()
{
	if (_mob)
	{
		_mob->ReleaseReference();
		_mob = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::NextOne (ImplAAFMobSlot **ppMobSlot)
{
	AAFRESULT		result;
	aafNumSlots_t	cur = _current, siz;

    _mob->CountSlots (&siz);
	if(cur < siz)
	{
		result = _mob->GetNthMobSlot (cur, ppMobSlot);
		_current = ++cur;
	}
	else
		result = AAFRESULT_NO_MORE_OBJECTS;

	return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Next (aafUInt32  count,
                           ImplAAFMobSlot **ppMobSlots,
                           aafUInt32 *pFetched)
{
	ImplAAFMobSlot**	ppMobSlot;
	aafUInt32			numSlots;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppMobSlot = ppMobSlots;
	for (numSlots = 0; numSlots < count; numSlots++)
	{
		hr = NextOne(ppMobSlot);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppMobSlot++;
	}
	
	if (pFetched)
		*pFetched = numSlots;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Skip (aafUInt32 count)
{
	AAFRESULT	hr;
	aafNumSlots_t	newCurrent, siz;

	newCurrent = _current + count;

    _mob->CountSlots(&siz);
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
    ImplEnumAAFMobSlots::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Clone (ImplEnumAAFMobSlots **ppEnum)
{
	ImplEnumAAFMobSlots*	theEnum;
	HRESULT					hr;
	
	theEnum = (ImplEnumAAFMobSlots *)CreateImpl(CLSID_EnumAAFMobSlots);
	if (theEnum == NULL)
		return E_FAIL;
		
	hr = theEnum->SetEnumMob(_mob);
	if (SUCCEEDED(hr))
	{
		theEnum->Reset();
		theEnum->Skip(_current);
		*ppEnum = theEnum;
	}
	else
	{
	  theEnum->ReleaseReference();
	  theEnum = 0;
	  *ppEnum = NULL;
	}

	return hr;
}


//Internal
AAFRESULT
    ImplEnumAAFMobSlots::SetEnumMob(ImplAAFMob *aMob)
{
	if (_mob)
	  _mob->ReleaseReference();
	_mob = 0;

	_mob = aMob;

	if (aMob)
		aMob->AcquireReference();

	return AAFRESULT_SUCCESS;
}
