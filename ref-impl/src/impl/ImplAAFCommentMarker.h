//@doc
//@class    AAFCommentMarker | Implementation class for AAFCommentMarker
#ifndef __ImplAAFCommentMarker_h__
#define __ImplAAFCommentMarker_h__

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


// Forward declaration
class ImplAAFSourceReference;


class ImplAAFCommentMarker : public ImplAAFEvent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCommentMarker ();

protected:
  virtual ~ImplAAFCommentMarker ();

public:


  //****************
  // GetAnnotation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAnnotation
        // @parm [out,retval] Annotation property value
        (ImplAAFSourceReference ** ppResult);

  //****************
  // SetAnnotation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAnnotation
        // @parm [in] Annotation property value
        (ImplAAFSourceReference * pAnnotation);

protected:
  // The stored object model defines the annotation to be a 
  // an optional strong reference.
  OMStrongReferenceProperty<ImplAAFSourceReference> _annotation;
};

#endif // ! __ImplAAFCommentMarker_h__


