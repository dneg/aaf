//@doc
//@class    AAFTextLocator | Implementation class for AAFTextLocator
#ifndef __CAAFTextLocator_h__
#define __CAAFTextLocator_h__

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






#ifndef __CAAFLocator_h__
#include "CAAFLocator.h"
#endif


class CAAFTextLocator
  : public IAAFTextLocator,
    public CAAFLocator
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTextLocator (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTextLocator ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  // IAAFNetworkLocator-supporting object.  This method must be called
  // after allocation, and before any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // SetName()
  //
  // Sets the Name string property.
  //
  // Set the Name property to the value specified in
  // pName.  A copy is made of the data so the caller
  // retains ownership of the *pName buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pName pointer is valid.
  // 
  // If this method fails the Name property will not be
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
  //   - pName arg is NULL.
  //
  STDMETHOD (SetName) (
    // buffer from which Name is to be read 
    /*[in, string]*/ aafCharacter_constptr  pName);


  //***********************************************************
  //
  // GetName()
  //
  // Gets the Name string property.
  // 
  // Writes the Name property, with a trailing null
  // character, into the pName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Name property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pName.
  // 
  // Succeeds if:
  // - The pName pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Name.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pName arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Name.
  //
  STDMETHOD (GetName) (
    // buffer into which Name is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pName,

    // size of *pName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetName().
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
  STDMETHOD (GetNameBufLen) (
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

#endif // ! __CAAFTextLocator_h__


