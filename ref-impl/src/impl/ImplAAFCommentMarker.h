//@doc
//@class    AAFCommentMarker | Implementation class for AAFCommentMarker
#ifndef __ImplAAFCommentMarker_h__
#define __ImplAAFCommentMarker_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




class ImplAAFSourceReference;






#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


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



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFCommentMarker)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFCommentMarkerTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFCommentMarker_h__


