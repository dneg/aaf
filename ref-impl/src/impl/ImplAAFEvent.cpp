/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include <AAFResult.h>


#include <assert.h>
#include <string.h>


ImplAAFEvent::ImplAAFEvent ():
  _position(PID_Event_Position, "Position"),
  _comment(PID_Event_Comment, "Comment")
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
      wchar_t *  pComment)
{
	if (NULL == pComment)
		return (AAFRESULT_NULL_PARAM);
	
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


OMDEFINE_STORABLE(ImplAAFEvent, AUID_AAFEvent);


