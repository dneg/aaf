/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

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
  _annotation(PID_CommentMarker_Annotation, "Annotation")
{
  _persistentProperties.put(_annotation.address());
}


ImplAAFCommentMarker::~ImplAAFCommentMarker ()
{
  // Cleanup references to contained objects.
  ImplAAFSourceReference *annotation = _annotation.setValue(0);
  if (annotation)
    annotation->ReleaseReference();
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
	ImplAAFSourceReference *oldValue = _annotation.setValue(0);
	if (oldValue)
		oldValue->ReleaseReference();
	
	_annotation = pAnnotation;
	
	if (pAnnotation)
		pAnnotation->AcquireReference();
	
	return AAFRESULT_SUCCESS;
}



OMDEFINE_STORABLE(ImplAAFCommentMarker, AUID_AAFCommentMarker);


