//@doc
//@class    AAFPlainEssenceData | Implementation class for AAFPlainEssenceData
#ifndef __CAAFPlainEssenceData_h__
#define __CAAFPlainEssenceData_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif






#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFPlainEssenceData
  : public IAAFPlainEssenceData,
    public IAAFKLVEssenceDataParameters,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPlainEssenceData (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPlainEssenceData ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Associates a weak reference to the given file mob with the
  /// essence data.
  ///
  /// Succeeds if all of the following are true:
  /// - the pFileMob pointer is valid and points to 
  /// a file mob (contains a file descriptor).
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileMob is null.
  //
  STDMETHOD (Initialize) (
    // reference to a file mob 
    /*[in]*/ IAAFSourceMob * pFileMob);


  //***********************************************************
  //
  // Write()
  //
  // Write pre-interleaved data to a essence stream.
  //
  STDMETHOD (Write) (
    // write this many bytes
    /*[in]*/ aafUInt32  bytes,

    // here is the buffer
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  bytesWritten);


  //***********************************************************
  //
  // Read()
  //
  // Read pre-interleaved data from a essence stream.
  //
  STDMETHOD (Read) (
    // read this many bytes
    /*[in]*/ aafUInt32  bytes,

    // here is the buffer
    /*[out, size_is(bytes), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  bytesRead);


  //***********************************************************
  //
  // SetPosition()
  //
  // Seek to absolute position within the essence data.
  //
  STDMETHOD (SetPosition) (
    // offset from beginning of essence 
    /*[in]*/ aafPosition_t  offset);


  //***********************************************************
  //
  // GetPosition()
  //
  // Get the absolute position within the essence data.
  //
  STDMETHOD (GetPosition) (
    // offset from beginning of essence 
    /*[out]*/ aafPosition_t*  pOffset);


  //***********************************************************
  //
  // GetSize()
  //
  // Return the total size of the essence data.
  //
  STDMETHOD (GetSize) (
    // size of essence data 
    /*[out]*/ aafLength_t *  pSize );


  //***********************************************************
  //
  // WriteSampleIndex()
  //
  // Write pre-interleaved data to a sample index stream.
  //
  STDMETHOD (WriteSampleIndex) (
    // write this many bytes to the sample index stream
    /*[in]*/ aafUInt32  bytes,

    // here is the buffer
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  bytesWritten);


  //***********************************************************
  //
  // ReadSampleIndex()
  //
  // Read pre-interleaved data from a sample index stream.
  //
  STDMETHOD (ReadSampleIndex) (
    // read this many bytes from the sample index stream
    /*[in]*/ aafUInt32  bytes,

    // here is the buffer
    /*[out, size_is(bytes), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer,

    // 
    /*[out,ref]*/ aafUInt32 *  bytesRead);


  //***********************************************************
  //
  // SetSampleIndexPosition()
  //
  // Seek to absolute position within the sample index data.
  //
  STDMETHOD (SetSampleIndexPosition) (
    // offset from beginning of sample index 
    /*[in]*/ aafPosition_t  offset);


  //***********************************************************
  //
  // GetSampleIndexPosition()
  //
  // Get the absolute position within the sample index data.
  //
  STDMETHOD (GetSampleIndexPosition) (
    // offset from beginning of sample index 
    /*[out]*/ aafPosition_t*  pOffset);


  //***********************************************************
  //
  // GetSampleIndexSize()
  //
  // Return the total size of the sample index data.
  //
  STDMETHOD (GetSampleIndexSize) (
    // size of sample index data 
    /*[out]*/ aafLength_t *  pSize );


  //***********************************************************
  //
  // SetFileMob()
  //
  // Associates a weak reference to the given file mob with the
  /// essence data.
  ///
  /// Succeeds if all of the following are true:
  /// - the pFileMob pointer is valid and points to 
  /// a file mob (contains a file descriptor).
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileMob is null.
  //
  STDMETHOD (SetFileMob) (
    // reference to a file mob 
    /*[in]*/ IAAFSourceMob * pFileMob);


  //***********************************************************
  //
  // GetFileMob()
  //
  // Associates a weak reference to the given file mob with the
  /// essence data.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppFileMob pointer is valid and a weak
  /// reference to the associated file mob can be
  /// resolved.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppFileMob is null.
  //
  STDMETHOD (GetFileMob) (
    // reference to a file mob 
    /*[in]*/ IAAFSourceMob ** ppFileMob);


  //***********************************************************
  //
  // GetFileMobID()
  //
  // Return the mob id used to find the file mob associated with this
  /// essence.  The file mob must exist in the same file as this
  /// essence data.
  //
  STDMETHOD (GetFileMobID) (
    // the file mob id associated with essence 
    /*[out]*/ aafMobID_t *  pFileMobID);


  //***********************************************************
  // METHOD NAME: GetEssenceElementKey()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFKLVEssenceDataParameters | GetEssenceElementKey |
  // This method returns essence element key associated
  /// with the essence stream.
  ///
  /// Succeeds if all of the following are true:
  /// - pEssenceElementKey is a valid pointer.
  /// - the essence stream supports essence element keys.
  ///
  /// If this method fails nothing will be written to *pEssenceElementKey.
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
  //   - pEssenceElementKey arg is NULL.
  ///
  /// AAFRESULT_OPERATION_NOT_PERMITTED
  ///   - the essence stream does not support essence element keys.
  // @end
  // 
  STDMETHOD (GetEssenceElementKey)
   (
    // @parm [out] aafUID_t * | pEssenceElementKey | Essence element key.
    aafUID_t *  pEssenceElementKey
  );

  //***********************************************************
  // METHOD NAME: SetEssenceElementKey()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFKLVEssenceDataParameters | SetEssenceElementKey |
  // Set the essence element key on the essence stream.
  /// 
  /// Succeeds if all of the following are true:
  /// - the essence stream supports essence element keys.
  /// 
  /// If this method fails, the property will not be changed.
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
  ///
  /// AAFRESULT_OPERATION_NOT_PERMITTED
  ///   - the essence stream does not support essence element keys.
  // @end
  // 
  STDMETHOD (SetEssenceElementKey)
   (
    // @parm [in, ref] aafUID_constref | key | essence element key
    aafUID_constref  key
  );


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

#endif // ! __CAAFPlainEssenceData_h__

