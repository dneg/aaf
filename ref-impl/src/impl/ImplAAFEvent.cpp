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


