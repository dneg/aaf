//@doc
//@class    AAFGetFileBits | Implementation class for AAFGetFileBits
#ifndef __CAAFGetFileBits_h__
#define __CAAFGetFileBits_h__

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


class CAAFGetFileBits
  : public IAAFGetFileBits,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFGetFileBits (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFGetFileBits ();

public:


  //***********************************************************
  //
  // ReadAt()
  //
  // Attempts to read bufsize bytes from this stream at the given byte
  // offset.  Places the data into buf.  Any attempt to read beyond
  // the size of this file, as reported by GetSize(), will be
  // considered an error.
  //
  // A value of zero for position indicates the beginning of the
  // stream.
  //
  // Succeeds if:
  // - The pNumBytes pointer is valid.
  // - position + buSize is not larger than the size of this file, as
  //   reported by GetSize().
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
  // AAFRESULT_OFFSET_SIZE
  //   - position + size is larger than the size of this file.
  //
  STDMETHOD (ReadAt) (
    // Buffer into which data is read
    /*[out, size_is(bufSize)]*/ aafMemPtr_t  buf,

    // Size of buf in bytes
    /*[in]*/ aafUInt32  bufSize,

    // The position in bytes at which to read
    /*[in]*/ aafUInt64  position);


  //***********************************************************
  //
  // GetSize()
  //
  // Returns the size of this file, in bytes, in *pSize.
  //
  // Succeeds if:
  // - The pSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pSize arg is NULL.
  //
  STDMETHOD (GetSize) (
    // The size of this file in bytes 
    /*[out]*/ aafUInt64 *  pSize);

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

#endif // ! __CAAFGetFileBits_h__


