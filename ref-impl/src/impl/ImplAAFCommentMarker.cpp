/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFCommentMarker_h__
#include "ImplAAFCommentMarker.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFCommentMarker::ImplAAFCommentMarker ()
{}


ImplAAFCommentMarker::~ImplAAFCommentMarker ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCommentMarker::GetAnnotation (
      ImplAAFSourceReference ** /*ppResult*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCommentMarker::SetAnnotation (
      ImplAAFSourceReference * /*pAnnotation*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFCommentMarker, AUID_AAFCommentMarker);


