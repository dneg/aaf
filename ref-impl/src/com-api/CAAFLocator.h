//@doc
//@class    AAFLocator | Implementation class for AAFLocator
#ifndef __CAAFLocator_h__
#define __CAAFLocator_h__

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





#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFLocator
  : public IAAFLocator,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFLocator (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFLocator ();

public:


  //***********************************************************
  //
  // SetPath()
  //
  // Sets the Path string property.
  //
  // Set the Path property to the value specified in
  // pPath.  A copy is made of the data so the caller
  // retains ownership of the *pPath buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pPath pointer is valid.
  // 
  // If this method fails the Path property will not be
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
  //   - pPath arg is NULL.
  //
  STDMETHOD (SetPath) (
    // buffer from which Path is to be read 
    /*[in, string]*/ aafCharacter_constptr  pPath);


  //***********************************************************
  //
  // GetPath()
  //
  // Gets the Path string property.
  // 
  // Writes the Path property, with a trailing null
  // character, into the pPath buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Path property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetPathBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pPath.
  // 
  // Succeeds if:
  // - The pPath pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Path.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPath arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Path.
  //
  STDMETHOD (GetPath) (
    // buffer into which Path is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pPath,

    // size of *pPath buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetPathBufLen()
  //
  // Returns size of buffer (in bytes) required for GetPath().
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
  STDMETHOD (GetPathBufLen) (
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

#endif // ! __CAAFLocator_h__


