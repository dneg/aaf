//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

	if( _annotation.isPresent() )
	{
		ImplAAFSourceReference *oldValue = _annotation;
		if (oldValue)
		  {
			if( oldValue == pAnnotation )
				return AAFRESULT_SUCCESS;

			oldValue->ReleaseReference();
			oldValue = 0;
		  }
	}

	if( pAnnotation->attached() )
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_annotation = pAnnotation;
	
	if (pAnnotation)
		pAnnotation->AcquireReference();
	
	return AAFRESULT_SUCCESS;
}





