/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/
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
	_isFirst = AAFTrue;
	_manager = ImplAAFPluginManager::GetPluginManager();
}


ImplEnumAAFLoadedPlugins::~ImplEnumAAFLoadedPlugins ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::NextOne (
      ImplAAFPluginDescriptor **ppAAFPluginDescriptor)
{
	XPROTECT()
	{
		if(_isFirst)
		{
			CHECK(_manager->GetFirstLoadedPlugin (&_tableIter, ppAAFPluginDescriptor));
		}
		else
		{
			CHECK(_manager->GetNextLoadedPlugin (&_tableIter, ppAAFPluginDescriptor));
		}
	}
	XEXCEPT
		XEND

		return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::Next (
      aafUInt32  count,
      ImplAAFPluginDescriptor **ppAAFPluginDesc,
      aafUInt32 *pFetched)
{
	ImplAAFPluginDescriptor**	ppDesc;
	aafUInt32			numDesc;
	HRESULT				hr;

	if (pFetched == NULL && count != 1)
		return AAFRESULT_NULL_PARAM;

	// Point at the first component in the array.
	ppDesc = ppAAFPluginDesc;
	for (numDesc = 0; numDesc < count; numDesc++)
	{
		hr = NextOne(ppDesc);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numSegments == count-1, but the for loop should
		// prevent access to this location.
		ppDesc++;
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
	ImplAAFPluginDescriptor	*pJunk;
	
	XPROTECT()
	{
		for(n = 0; n < count; n++)
		{
			CHECK(_manager->GetNextLoadedPlugin (&_tableIter, &pJunk));
		}
	}
	XEXCEPT
	XEND
		
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLoadedPlugins::Reset ()
{
	_isFirst = AAFTrue;
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




