//@doc
//@class    AAFEvent | Implementation class for AAFEvent
#ifndef __ImplAAFEvent_h__
#define __ImplAAFEvent_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFEvent : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEvent ();

protected:
  virtual ~ImplAAFEvent ();

public:


  //****************
  // GetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
        // @parm [out,retval] Position property value
        (aafPosition_t *  pPosition);

  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition
        // @parm [in] Position property value
        (aafPosition_t  Position);


  //****************
  // SetComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetComment
        // @parm [in, string] buffer from which Comment is to be read
        (wchar_t *  pComment);


  //****************
  // GetComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComment
        (// @parm [out, string, size_is(bufSize)] buffer into which Comment is to be written
         wchar_t *  pComment,

         // @parm [in] size of *pComment buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetCommentBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCommentBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEvent)

protected:
  OMFixedSizeProperty<aafPosition_t> _position;
  OMWideStringProperty _comment;
};

#endif // ! __ImplAAFEvent_h__


