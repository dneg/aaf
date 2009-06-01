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

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFResult.h"


#include <string.h>
#include <wchar.h>


ImplAAFEvent::ImplAAFEvent ():
  _position(PID_Event_Position, L"Position"),
  _comment(PID_Event_Comment, L"Comment")
{
  _persistentProperties.put(_position.address());
  _persistentProperties.put(_comment.address());
}


ImplAAFEvent::~ImplAAFEvent ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::GetPosition (
      aafPosition_t *  pPosition)
{
  if (NULL == pPosition)
    return AAFRESULT_NULL_PARAM;

  *pPosition = _position;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::SetPosition (
      aafPosition_t  Position)
{
  // Why is the argument pass-by-value?
  _position = Position;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::SetComment (
      const aafCharacter * pComment)
{
	if (NULL == pComment)
		return (AAFRESULT_NULL_PARAM);

	if (wcslen(pComment)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return (AAFRESULT_BAD_SIZE);
	
	_comment = pComment;
	
	return (AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFEvent::GetComment (wchar_t *  pComment,
						  aafUInt32  bufSize)
{
	if (NULL == pComment)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_comment.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat = _comment.copyToBuffer(pComment, bufSize);
	if (!stat)
		return AAFRESULT_SMALLBUF;
	
	return (AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::GetCommentBufLen (
      aafUInt32 *  pBufSize)
{
	if (NULL == pBufSize)
		return (AAFRESULT_NULL_PARAM);
	
	if (!_comment.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pBufSize = _comment.size();
	
	return (AAFRESULT_SUCCESS); 
}




