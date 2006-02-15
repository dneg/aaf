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
  /// IAAFRIFFChunk-supporting object.  This method must be
  /// called after allocation, and before any other method can be
  /// called.
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
    /*[in]*/ aafUInt32  chunkID,

    // ChunkLength value
    /*[in]*/ aafUInt32  chunkLength,

    // Write this many bytes
    /*[in]*/ aafUInt32  bytes,

    // Data to write
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer);


  //***********************************************************
  //
  // SetChunkID()
  //
  // Sets the ChunkID property.
                        
  ///
  /// Succeeds if all of the following are true:
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
  /// Succeeds if all of the following are true:
  /// - pChunkID is a valid pointer
  /// - the property is present.
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
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetChunkID) (
    // Retrieved ChunkID 
    /*[out]*/ aafUInt32 *  pChunkID);


  //***********************************************************
  //
  // SetChunkLength()
  //
  // Sets the ChunkLength property
                   
  ///
  /// Succeeds if all of the following are true:
  ///
  /// If this method fails, the ChunkLength property will not be
  /// changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetChunkLength) (
    // Length to set 
    /*[in]*/ aafUInt32  chunkLength);

  //***********************************************************
  //
  // GetChunkLength()
  //
  // Gets the ChunkLength property
                      
  ///
  /// Succeeds if all of the following are true:
  /// - pMChunkLength is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pChunkLength will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pChunkLength is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetChunkLength) (
    // Retrieved ChuckLength 
    /*[out]*/ aafUInt32 *  pChunkLength);


  //***********************************************************
  //
  // WriteChunkData()
  //
  // Write the specified bytes to the chunk data stream.
  /// 
  /// Succeeds if all of the following are true:
  /// - the number of bytes to write is non-zero.
  /// - the buffer pointer is valid.
  /// - the pBytesWritten pointer is valid.
  /// - the object is initialized.
  /// - the object is persistent (attached to a file).
  /// 
  /// If this method fails the ChunkData property will
  /// not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_INVALID_PARAM
  ///   - bytes arg is larger than zero.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - buffer arg is NULL.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBytesWritten arg is NULL.
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  ///
  /// AAFRESULT_CONTAINERWRITE
  ///   - writing failed.
  //
  STDMETHOD (WriteChunkData) (
    // Write this many bytes
    /*[in]*/ aafUInt32  bytes,

    // Data to write
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // Number of bytes actually written
    /*[out,ref]*/ aafUInt32 *  pBytesWritten);


  //***********************************************************
  //
  // ReadChunkData()
  //
  // Read the specified number of bytes from the chunk data
  /// stream into buffer.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the number of bytes to read is non-zero.
  /// - the buffer pointer is valid.
  /// - the pBytesRead pointer is valid.
  /// - the ChunkData property is present.
  /// - the object is persistent (attached to a file).
  /// - not yet reached the end of the data stream.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  /// 
  /// AAFRESULT_END_OF_DATA
  ///   - trying to read beyond the end of the data stream.
  /// 
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  /// 
  /// AAFRESULT_INVALID_PARAM
  ///   - bytes arg is larger than zero.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - buffer arg is NULL.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBytesRead arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the ChunkData property is not present.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (ReadChunkData) (
    // Read this many bytes
    /*[in]*/ aafUInt32  bytes,

    // Buffer to read the data to
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // Number of bytes actually read
    /*[out,ref]*/ aafUInt32 *  pBytesRead);


  //***********************************************************
  //
  // SetChunkDataPosition()
  //
  // Sets the offset from the beginning of chunk data.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the object is persistent (attached to a file).
  /// 
  /// If this method fails the position will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (SetChunkDataPosition) (
    // Offset from the beginning of chunk data. 
    /*[in]*/ aafPosition_t  position);


  //***********************************************************
  //
  // GetChunkDataPosition()
  //
  // Gets the offset from the beginning of chunk data.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pPosition pointer is valid.
  /// - the object is persistent (attached to a file).
  /// 
  /// If this method fails nothing will be written to *pPosition.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPosition arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the ChunkData property is not present.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (GetChunkDataPosition) (
    // Offset from the beginning of chunk data. 
    /*[out]*/ aafPosition_t *  pPosition);


  //***********************************************************
  //
  // GetChunkDataSize()
  //
  // Gets the size of chunk data.
  /// ChunkData is optional property.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pSize pointer is valid.
  /// - the ChunkData property is present.
  /// - the object is persistent (attached to a file).
  /// 
  /// If this method fails nothing will be written to *pSize.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSize arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the ChunkData property is not present.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (GetChunkDataSize) (
    // The size of chunk data. 
    /*[out]*/ aafLength_t *  pSize);



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


