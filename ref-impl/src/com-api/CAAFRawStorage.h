//@doc
//@class    AAFRawStorage | Implementation class for AAFRawStorage
#ifndef __CAAFRawStorage_h__
#define __CAAFRawStorage_h__

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


class CAAFRawStorage
  : public IAAFRawStorage,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRawStorage (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRawStorage ();

public:


  //***********************************************************
  //
  // IsReadable()
  //
  // Sets *pResult to true if this storage is readable; sets it to
  // false otherwise.
  //
  // Succeeds if:
  // - The pResult pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  STDMETHOD (IsReadable) (
    // set to true if readable 
    /*[out]*/ aafBoolean_t *  pResult);


  //***********************************************************
  //
  // Read()
  //
  // Attempts to read bufsize bytes from this stream.  Places the data
  // into buf, and puts the actual number of bytes read into
  // *pNumRead.  If the end of the stream is encountered before
  // bufSize bytes can be read, the value written into *pNumRead may
  // be smaller than bufSize.
  //
  // This call will advance the current position by *pNumRead bytes.
  //
  // Succeeds if:
  // - The pNumBytes pointer is valid.
  // - This stream is open for read or read/write.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - buf or pNumRead arg is NULL.
  //
  // AAFRESULT_NOT_READABLE
  //   - This stream is not open for read or read/write.
  //
  STDMETHOD (Read) (
    // Buffer into which data is read
    /*[out, size_is(bufSize), length_is(*pNumRead)]*/ aafMemPtr_t  buf,

    // Size of buf in bytes
    /*[in]*/ aafUInt32  bufSize,

    // Number of bytes read
    /*[out]*/ aafUInt32 *  pNumRead);


  //***********************************************************
  //
  // IsWriteable()
  //
  // Sets *pResult to true if this storage is writeable; sets it to
  // false otherwise.
  //
  // Succeeds if:
  // - The pResult pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pResult arg is NULL.
  //
  STDMETHOD (IsWriteable) (
    // set to true if writeable 
    /*[out]*/ aafBoolean_t *  pResult);


  //***********************************************************
  //
  // Write()
  //
  // Writes bufsize bytes into this stream.  Obtains the data
  // from buf.
  //
  // This call may fail if the stream has insufficient capacity to
  // complete the request.  If this storage supports the
  // IAAFRandomRawStorage interface, the client can call
  // IAAFRandomRawStorage::SetSize() to attempt to reserve capacity
  // in the stream; if the SetSize() call succeeds, subsequent
  // Write() calls within that capacity are guaranteed to succeed.
  //
  // This call will advance the current position by bufSize bytes.
  //
  // Succeeds if:
  // - This stream is open for write or read/write.
  // - Sufficient capacity exists in this stream to perform the
  //   write.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_WRITEABLE
  //   - This stream is not open for write or read/write.
  //
  // AAFRESULT_SMALLBUF
  //   - This stream has insufficient capacity to perform the write.
  //
  STDMETHOD (Write) (
    // Buffer from which data is written
    /*[in, size_is(bufSize)]*/ aafMemConstPtr_t  buf,

    // Number of bytes to write
    /*[in]*/ aafUInt32  bufSize,

    // Number of bytes written
    /*[out]*/ aafUInt32 *  pNumWritten);


  //***********************************************************
  //
  // Synchronize()
  //
  // Synchronizes any cached data with the underlying storage.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (Synchronize)
     ();


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

#endif // ! __CAAFRawStorage_h__


