/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

#ifndef __ImplAAFTextLocator_h__
#include "ImplAAFTextLocator.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "AAFResult.h"
#include <assert.h>
#include <string.h>


ImplAAFTextLocator::ImplAAFTextLocator () :
	_name(PID_TextLocator_Name,		"Text Locator")
{
	_persistentProperties.put(_name.address());
	_name = L"";
}


ImplAAFTextLocator::~ImplAAFTextLocator ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::Initialize ()
{
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::GetName (wchar_t* pNameBuf, aafInt32  bufSize)
{
	bool stat;
	if (! pNameBuf)
	{
		return AAFRESULT_NULL_PARAM;
	}
	stat = _name.copyToBuffer(pNameBuf, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;
	}
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::GetNameBufLen (aafInt32* pLen)
{
	if (! pLen)
	{
		return AAFRESULT_NULL_PARAM;
	}
	*pLen = _name.size();
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::SetName (wchar_t* pNameBuf)
{
	if (! pNameBuf)
	{
		return AAFRESULT_NULL_PARAM;
	}
	_name = pNameBuf;

	return(AAFRESULT_SUCCESS); 
}





