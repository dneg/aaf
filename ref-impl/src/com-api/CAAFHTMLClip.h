//@doc
//@class    AAFHTMLClip | Implementation class for AAFHTMLClip
#ifndef __CAAFHTMLClip_h__
#define __CAAFHTMLClip_h__

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





#ifndef __CAAFTextClip_h__
#include "CAAFTextClip.h"
#endif


class CAAFHTMLClip
  : public IAAFHTMLClip,
    public CAAFTextClip
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFHTMLClip (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFHTMLClip ();

public:


  //***********************************************************
  //
  // SetBeginAnchor()
  //
  // Sets the Begin Anchor string property.
  //
  // Set the BeginAnchor property to the value specified in
  // pBeginAnchor.  A copy is made of the data so the caller
  // retains ownership of the *pBeginAnchor buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pBeginAnchor pointer is valid.
  // 
  // If this method fails the BeginAnchor property will not be
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
  //   - pBeginAnchor arg is NULL.
  //
  STDMETHOD (SetBeginAnchor) (
    // buffer from which BeginAnchor is to be read 
    /*[in, string]*/ aafCharacter_constptr  pBeginAnchor);


  //***********************************************************
  //
  // GetBeginAnchor()
  //
  // Gets the Begin Anchor string property.
  // 
  // Writes the BeginAnchor property, with a trailing null
  // character, into the pBeginAnchor buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the BeginAnchor property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetBeginAnchorBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pBeginAnchor.
  // 
  // Succeeds if:
  // - The pBeginAnchor pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   BeginAnchor.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBeginAnchor arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold BeginAnchor.
  //
  STDMETHOD (GetBeginAnchor) (
    // buffer into which BeginAnchor is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pBeginAnchor,

    // size of *pBeginAnchor buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetBeginAnchorBufLen()
  //
  // Returns size of buffer (in bytes) required for GetBeginAnchor().
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
  STDMETHOD (GetBeginAnchorBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // SetEndAnchor()
  //
  // Sets the End Anchor string property.
  //
  // Set the EndAnchor property to the value specified in
  // pEndAnchor.  A copy is made of the data so the caller
  // retains ownership of the *pEndAnchor buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pEndAnchor pointer is valid.
  // 
  // If this method fails the EndAnchor property will not be
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
  //   - pEndAnchor arg is NULL.
  //
  STDMETHOD (SetEndAnchor) (
    // buffer from which EndAnchor is to be read 
    /*[in, string]*/ aafCharacter_constptr  pEndAnchor);


  //***********************************************************
  //
  // GetEndAnchor()
  //
  // Gets the End Anchor string property.
  // 
  // Writes the EndAnchor property, with a trailing null
  // character, into the pEndAnchor buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the EndAnchor property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetEndAnchorBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pEndAnchor.
  // 
  // Succeeds if:
  // - The pEndAnchor pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   EndAnchor.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEndAnchor arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold EndAnchor.
  //
  STDMETHOD (GetEndAnchor) (
    // buffer into which EndAnchor is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pEndAnchor,

    // size of *pEndAnchor buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetEndAnchorBufLen()
  //
  // Returns size of buffer (in bytes) required for GetEndAnchor().
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
  STDMETHOD (GetEndAnchorBufLen) (
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

#endif // ! __CAAFHTMLClip_h__


