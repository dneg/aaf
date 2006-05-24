//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif



/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



#ifndef __ImplEnumAAFLoadedPlugins_h__
#include "ImplEnumAAFLoadedPlugins.h"
#endif

#include <string.h>
#include "aafTable.h"
#include "ImplAAFPluginManager.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafUID_t CLSID_EnumAAFLoadedPlugins;

ImplEnumAAFLoadedPlugins::ImplEnumAAFLoadedPlugins () :
	_manager(NULL)
{
	memset(&_category, 0, sizeof(_category));
	_manager = ImplAAFPluginManager::GetPluginManager();
	_isFirst = kAAFTrue;
}


ImplEnumAAFLoadedPlugins::~ImplEnumAAFLoadedPlugins ()
{
	if (_manager)
	{
		_manager->ReleaseReference();
		_manager = NULL;
	}
}


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
	HRESULT				hr = E_FAIL;

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
			if(n == 0)
			{
				CHECK(_manager->GetFirstLoadedPlugin (_category, &_tableIter, &pJunk));
				_isFirst = kAAFFalse;
			}
			else
			{
				CHECK(_manager->GetNextLoadedPlugin (_category, &_tableIter, &pJunk));
			}
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




