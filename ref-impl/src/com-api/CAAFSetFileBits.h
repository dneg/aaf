//@doc
//@class    AAFSetFileBits | Implementation class for AAFSetFileBits
#ifndef __CAAFSetFileBits_h__
#define __CAAFSetFileBits_h__

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





#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFSetFileBits
  : public IAAFSetFileBits,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSetFileBits (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSetFileBits ();

public:


  //***********************************************************
  //
  // WriteAt()
  //
  // Writes bufsize bytes into this file at the given byte offset.
  // Obtains the data from buf.  A value of zero for position
  // indicates the beginning of the stream.
  //
  // Succeeds if sufficient space is available in the file to write
  // the given number of bytes at the given position.  It's possible
  // to guarantee that this method will succeed if SetSize() is first
  // called to reserve the required space.  If SetSize() is not first
  // called to reserve the space, this method will attempt to request
  // it, but this attempt may fail.
  //
  // Succeeds if:
  // - The buf argument is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - buf arg is NULL.
  //
  // AAFRESULT_DATA_SIZE
  //   - The requested space is not available.
  //
  STDMETHOD (WriteAt) (
    // Buffer from which data is written
    /*[in, size_is(bufSize)]*/ aafMemPtr_t  buf,

    // Number of bytes
    /*[in]*/ aafUInt32  bufSize,

    // The position in bytes at which to read
    /*[in]*/ aafUInt64  position);


  //***********************************************************
  //
  // SetSize()
  //
  // Attempts to reserve size bytes of storage.
  //
  // Succeeds if:
  // - The requested space is available.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_DATA_SIZE
  //   - size bytes are not available.
  //
  STDMETHOD (SetSize) (
    // The requested file size 
    /*[in]*/ aafUInt64  size);

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

#endif // ! __CAAFSetFileBits_h__


