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



#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif



/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



#ifndef __ImplEnumAAFLoadedPlugins_h__
#include "ImplEnumAAFLoadedPlugins.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafTable.h"
#include "ImplAAFPluginManager.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafUID_t CLSID_EnumAAFLoadedPlugins;

ImplEnumAAFLoadedPlugins::ImplEnumAAFLoadedPlugins ()
{
	_isFirst = kAAFTrue;
	_manager = ImplAAFPluginManager::GetPluginManager();
}


ImplEnumAAFLoadedPlugins::~ImplEnumAAFLoadedPlugins ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::NextOne (
      aafUID_t *ppAAFPluginID)
{
	XPROTECT()
	{
		if(_isFirst)
		{
			CHECK(_manager->GetFirstLoadedPlugin (_category, &_tableIter, ppAAFPluginID));
			_isFirst = kAAFFalse;
		}
		else
		{
			CHECK(_manager->GetNextLoadedPlugin (_category, &_tableIter, ppAAFPluginID));
		}
	}
	XEXCEPT
		XEND

		return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::Next (
      aafUInt32  count,
      aafUID_t *pAAFPluginID,
      aafUInt32 *pFetched)
{
	aafUID_t			*pDesc;
	aafUInt32			numDesc;
	HRESULT				hr;

	if (pFetched == NULL && count != 1)
		return AAFRESULT_NULL_PARAM;

	// Point at the first component in the array.
	pDesc = pAAFPluginID;
	for (numDesc = 0; numDesc < count; numDesc++)
	{
		hr = NextOne(pDesc);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numSegments == count-1, but the for loop should
		// prevent access to this location.
		pDesc++;
	}
	
	if (pFetched)
		*pFetched = numDesc;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::Skip (
      aafUInt32  count)
{
	aafUInt32	n;
	aafUID_t	pJunk;
	
	XPROTECT()
	{
		for(n = 0; n < count; n++)
		{
			CHECK(_manager->GetNextLoadedPlugin (_category, &_tableIter, &pJunk));
		}
	}
	XEXCEPT
	XEND
		
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::Reset ()
{
	_isFirst = kAAFTrue;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::Clone (
      ImplEnumAAFLoadedPlugins **ppEnum)
{
	ImplEnumAAFLoadedPlugins*	theEnum;
	HRESULT						hr;

	if (ppEnum == NULL)
		return AAFRESULT_NULL_PARAM;
	
	theEnum = (ImplEnumAAFLoadedPlugins *)CreateImpl(CLSID_EnumAAFLoadedPlugins);
	if (theEnum == NULL)
		return E_FAIL;
		
	hr = theEnum->SetCategory(&_category);
	if (SUCCEEDED(hr))
	{
		theEnum->Reset();
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


AAFRESULT
    ImplEnumAAFLoadedPlugins::SetCategory (
      aafUID_t *pCategory)
{
	if(pCategory == NULL)
		return AAFRESULT_NULL_PARAM;

	_category = *pCategory;
	 
	return AAFRESULT_SUCCESS;
}




