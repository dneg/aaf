//@doc
//@class    AAFRandomRawStorage | Implementation class for AAFRandomRawStorage
#ifndef __CAAFRandomRawStorage_h__
#define __CAAFRandomRawStorage_h__

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





#ifndef __CAAFRawStorage_h__
#include "CAAFRawStorage.h"
#endif


class CAAFRandomRawStorage
  : public IAAFRandomRawStorage,
    public CAAFRawStorage
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRandomRawStorage (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRandomRawStorage ();

public:


  //***********************************************************
  //
  // ReadAt()
  //
  // Attempts to read bufsize bytes from this storage.  Places the
  // data into buf, and puts the actual number of bytes read into
  // *pNumRead.  If the end of the storage is encountered before
  // bufSize bytes can be read, the value written into *pNumRead may
  // be smaller than bufSize.
  //
  // This call will advance the current position by *pNumRead bytes.
  //
  // Succeeds if:
  // - The pNumRead pointer is valid.
  // - This storage is open for read or read/write.
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
  //   - This storage is not open for read or read/write.
  //
  STDMETHOD (ReadAt) (
    // position in this storage from which data is read
    /*[in]*/ aafUInt64  position,

    // Buffer into which data is read
    /*[out, size_is(bufSize), length_is(*pNumRead)]*/ aafMemPtr_t  buf,

    // Size of buf in bytes
    /*[in]*/ aafUInt32  bufSize,

    // Number of bytes read
    /*[out]*/ aafUInt32 *  pNumRead);


  //***********************************************************
  //
  // WriteAt()
  //
  // Writes bufsize bytes into this storage.  Obtains the data
  // from buf.
  //
  // This call may fail if the capacity of this storage cannot be
  // extended sufficiently to complete the request.  The client can
  // call SetSize() to attempt to reserve capacity for the storage;
  // if the SetSize() call succeeds, subsequent Write() calls
  // within requested that capacty are guaranteed to succeed.
  //
  // This call will advance the current position by bufSize bytes.
  //
  // Succeeds if:
  // - The pNumWritten pointer is valid.
  // - This storage is open for write or read/write.
  // - Sufficient capacity exists in this storage to perform the
  //   write.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumWritten arg is NULL.
  //
  // AAFRESULT_NOT_WRITEABLE
  //   - This storage is not open for write or read/write.
  //
  // AAFRESULT_OFFSET_SIZE
  //   - It is not possible to extend the allocated size of this
  //     storage.
  //
  STDMETHOD (WriteAt) (
    // position in this storage into which data is written
    /*[in]*/ aafUInt64  position,

    // Buffer from which data is written
    /*[in, size_is(bufSize)]*/ aafMemConstPtr_t  buf,

    // Number of bytes
    /*[in]*/ aafUInt32  bufSize,

    // Number of bytes written
    /*[out]*/ aafUInt32 *  pNumWritten);


  //***********************************************************
  //
  // GetSize()
  //
  // Returns the highest byte position in this storage which has been
  // written so far.
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
    // Number of bytes currently in this storage 
    /*[out]*/ aafUInt64 *  pSize);


  //***********************************************************
  //
  // IsExtendable()
  //
  // Sets *pResult to true if the memory allocated to this storage can
  // be dynamically extened; sets it to false otherwise.
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
  STDMETHOD (IsExtendable) (
    // set to true if extendable 
    /*[out]*/ aafBoolean_t *  pResult);


  //***********************************************************
  //
  // GetExtent()
  //
  // Returns the number of bytes currently allocated storage.
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
  //   - pExtent arg is NULL.
  //
  STDMETHOD (GetExtent) (
    // Number of bytes currently allocated for this storage 
    /*[out]*/ aafUInt64 *  pExtent);


  //***********************************************************
  //
  // SetExtent()
  //
  // Requests the given capacity for this storage.  If successful,
  // subsequent calls to WriteAt() and IAAFRawStorage::Write()
  // within that capacity are guaranteed to succeed.
  //
  // Succeeds if:
  // - It is permissible to set the extent of this storage.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_OFFSET_SIZE
  //   - This storage has insufficient capacity to perform the write.
  //
  STDMETHOD (SetExtent) (
    // Number of bytes capacity requested for this storage 
    /*[in]*/ aafUInt64  extent);

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

#endif // ! __CAAFRandomRawStorage_h__


