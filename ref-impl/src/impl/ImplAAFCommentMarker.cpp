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

#ifndef __ImplAAFCommentMarker_h__
#include "ImplAAFCommentMarker.h"
#endif

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFResult.h"
#include "AAFClassIDs.h"


#include <assert.h>
#include <string.h>


ImplAAFCommentMarker::ImplAAFCommentMarker () :
  _annotation(PID_CommentMarker_Annotation, L"Annotation")
{
  _persistentProperties.put(_annotation.address());
}


ImplAAFCommentMarker::~ImplAAFCommentMarker ()
{
  // Cleanup references to contained objects.
  ImplAAFSourceReference *annotation = _annotation.clearValue();
  if (annotation)
	{
	  annotation->ReleaseReference();
	  annotation = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCommentMarker::GetAnnotation (
      ImplAAFSourceReference ** ppAnnotation)
{
	if (NULL == ppAnnotation)
		return (AAFRESULT_NULL_PARAM);
	
	if (!_annotation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*ppAnnotation = _annotation;
	
	if (*ppAnnotation)
		(*ppAnnotation)->AcquireReference();
	else
		return (AAFRESULT_NULLOBJECT);
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCommentMarker::SetAnnotation (
      ImplAAFSourceReference * pAnnotation)
{
	if( pAnnotation == NULL )
		return AAFRESULT_NULL_PARAM;

	if( pAnnotation->attached() )
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	ImplAAFSourceReference *oldValue = _annotation.clearValue();
	if (oldValue)
	  {
		oldValue->ReleaseReference();
		oldValue = 0;
	  }
	
	_annotation = pAnnotation;
	
	if (pAnnotation)
		pAnnotation->AcquireReference();
	
	return AAFRESULT_SUCCESS;
}





