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

#ifndef __ImplAAFHTMLClip_h__
#include "ImplAAFHTMLClip.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFHTMLClip::ImplAAFHTMLClip () :
  _beginAnchor(PID_HTMLClip_BeginAnchor, "BeginAnchor"),
  _endAnchor(PID_HTMLClip_EndAnchor, "EndAnchor")
{
  _persistentProperties.put(_beginAnchor.address());
  _persistentProperties.put(_endAnchor.address());
}


ImplAAFHTMLClip::~ImplAAFHTMLClip ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetBeginAnchor (wchar_t *  pName,
								 aafInt32  bufSize)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_beginAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat = _beginAnchor.copyToBuffer(pName, bufSize);
	if (! stat)
		return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	
	return (AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetBeginAnchorBufLen (aafUInt32 *  pLen)
{
	if (NULL == pLen)
		return (AAFRESULT_NULL_PARAM);
	
	if(!_beginAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLen = _beginAnchor.size();
	
	return (AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::SetBeginAnchor (wchar_t *  pName)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	_beginAnchor = pName;
	
	return (AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetEndAnchor (wchar_t *  pName,
							   aafInt32  bufSize)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_endAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat = _endAnchor.copyToBuffer(pName, bufSize);
	if (! stat)
		return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	
	return (AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::GetEndAnchorBufLen (aafUInt32 *  pLen)
{
	if (NULL == pLen)
		return (AAFRESULT_NULL_PARAM);
	
	if (!_endAnchor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pLen = _endAnchor.size();
	
	return (AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFHTMLClip::SetEndAnchor (wchar_t *  pName)
{
	if (NULL == pName)
		return(AAFRESULT_NULL_PARAM);
	
	_endAnchor = pName;
	
	return (AAFRESULT_SUCCESS); 
}






