//@doc
//@class    AAFEvent | Implementation class for AAFEvent
#ifndef __CAAFEvent_h__
#define __CAAFEvent_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif







#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFEvent
  : public IAAFEvent,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEvent (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEvent ();

public:



  //***********************************************************
  //
  // GetPosition()
  //
  // This method will return the Position of this event.
  // 
  // Succeeds if all of the following are true:
  // - the pPosition pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pPosition arg is NULL.
  //
  STDMETHOD (GetPosition) (
    // Position property value 
    /*[out,retval]*/ aafPosition_t *  pPosition);

  //***********************************************************
  //
  // SetPosition()
  //
  // This method will set the Position of this event.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.).
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPosition) (
    // Position property value 
    /*[in]*/ aafPosition_t  Position);


  //***********************************************************
  //
  // SetComment()
  //
  // Sets specifies the purpose of the event.
  //
  // Set the Comment property to the value specified in
  // pComment.  A copy is made of the data so the caller
  // retains ownership of the *pComment buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pComment pointer is valid.
  // 
  // If this method fails the Comment property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pComment arg is NULL.
  //
  STDMETHOD (SetComment) (
    // buffer from which Comment is to be read 
    /*[in, string]*/ aafCharacter_constptr  pComment);


  //***********************************************************
  //
  // GetComment()
  //
  // Gets specifies the purpose of the event.
  // 
  // Writes the Comment property, with a trailing null
  // character, into the pComment buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Comment property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetCommentBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pComment.
  // 
  // Succeeds if:
  // - The pComment pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Comment.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pComment arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Comment.
  //
  STDMETHOD (GetComment) (
    // buffer into which Comment is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pComment,

    // size of *pComment buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetCommentBufLen()
  //
  // Returns size of buffer (in bytes) required for GetComment().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetCommentBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFEvent_h__


