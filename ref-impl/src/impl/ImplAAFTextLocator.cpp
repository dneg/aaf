//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTextLocator_h__
#include "ImplAAFTextLocator.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "AAFResult.h"
#include <string.h>
#include <wchar.h>


ImplAAFTextLocator::ImplAAFTextLocator () :
	_name(PID_TextLocator_Name,		L"Name")
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
    ImplAAFTextLocator::GetName (aafCharacter* pNameBuf, aafUInt32  bufSize)
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
    ImplAAFTextLocator::GetNameBufLen (aafUInt32* pLen)
{
	if (! pLen)
	{
		return AAFRESULT_NULL_PARAM;
	}
	*pLen = _name.size();
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::SetName (const aafCharacter* pNameBuf)
{
	if (! pNameBuf)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if (wcslen(pNameBuf)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		return AAFRESULT_BAD_SIZE;
	}
	_name = pNameBuf;

	return(AAFRESULT_SUCCESS); 
}





