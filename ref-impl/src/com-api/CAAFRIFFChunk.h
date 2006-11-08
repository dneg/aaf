//@doc
//@class    AAFRIFFChunk | Implementation class for AAFRIFFChunk
#ifndef __CAAFRIFFChunk_h__
#define __CAAFRIFFChunk_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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


class CAAFRIFFChunk
  : public IAAFRIFFChunk,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRIFFChunk (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRIFFChunk ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  /// IAAFRIFFChunk-supporting object. The data content is initially
  /// empty.
  ///
  /// This method must be called after allocation, and before any other
  /// method can be called.
  ///
  /// Succeeds if:
  /// - Initialize() has not yet been called on this object.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ALREADY_INITIALIZED
  ///   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize) (
    // ChunkID value 
    /*[in]*/ aafUInt32  chunkID);


  //***********************************************************
  //
  // SetChunkID()
  //
  // Sets the ChunkID property.
                        
  ///
  /// Succeeds if all of the following are true:
  /// - This object has already been had Initialize() called on it.
  ///
  /// If this method fails, the ChunkID property will not be
  /// changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetChunkID) (
    // ChunkID to set 
    /*[in]*/ aafUInt32  chunkID);

  //***********************************************************
  //
  // GetChunkID()
  //
  // Gets the ChunkID property.
  ///
  ///
  /// Succeeds if all of the following are true:
  /// - pChunkID is a valid pointer
  ///
  /// If this method fails, pChunkID will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pChunkID is NULL.
  //
  STDMETHOD (GetChunkID) (
    // Retrieved ChunkID 
    /*[out]*/ aafUInt32 *  pChunkID);


  //***********************************************************
  //
  // Read()
  //
  // Reads data from this RIFFChunk at the current position. Advances the position
  // by the number of bytes read.
  //
  STDMETHOD (Read) (
    // read this many bytes
    /*[in]*/ aafUInt32  bytes,

    // buffer to receive chunk data
    /*[out, size_is(bytes), length_is(*pBytesRead)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  pBytesRead);


  //***********************************************************
  //
  // Write()
  //
  // Write data to this RIFFChunk at the current position. Advances the position
  // by the number of bytes written. Any data beyond the new position is lost;
  // that is, the buffer is truncated.
  //
  STDMETHOD (Write) (
    // write this many bytes
    /*[in]*/ aafUInt32  bytes,

    // chunk data to write
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // actual number of bytes written
    /*[out,ref]*/ aafUInt32 *  pBytesWritten);


  //***********************************************************
  //
  // SetPosition()
  //
  // Seeks to absolute position within the RIFFChunk data. The next Read or Write
  // call will operate from the given position.
  //
  STDMETHOD (SetPosition) (
    // offset from beginning of the RIFFChunk data 
    /*[in]*/ aafPosition_t  offset);


  //***********************************************************
  //
  // GetPosition()
  //
  // Gets the current position within the RIFFChunk data. This is the position at
  // which the next Read or Write call will operate.
  //
  STDMETHOD (GetPosition) (
    // current offset from beginning of the RIFFChunk data 
    /*[out]*/ aafPosition_t*  pOffset);


  //***********************************************************
  //
  // GetLength()
  //
  // Returns the total size of the RIFFChunk data, in bytes.
  //
  STDMETHOD (GetLength) (
    // length of the RIFFChunk data 
    /*[out]*/ aafLength_t *  pLength);



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

#endif // ! __CAAFRIFFChunk_h__


