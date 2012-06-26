//@doc
//@class    AAFBWFImportDescriptor | Implementation class for AAFBWFImportDescriptor
#ifndef __CAAFBWFImportDescriptor_h__
#define __CAAFBWFImportDescriptor_h__

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







#ifndef __CAAFImportDescriptor_h__
#include "CAAFImportDescriptor.h"
#endif


class CAAFBWFImportDescriptor
  : public IAAFBWFImportDescriptor,
    public CAAFImportDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFBWFImportDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFBWFImportDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  /// IAAFBWFImportDescriptor-supporting object.  This method must be
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
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // SetFileSecurityReport()
  //
  // Sets the fileSecurityReport property. This is an optional property.
  /// 
  /// If this method fails, the FileSecurityReport property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetFileSecurityReport) (
    // value to assign to the FileSecurityReport property 
    /*[in]*/ aafUInt32  fileSecurityReport);


  //***********************************************************
  //
  // GetFileSecurityReport()
  //
  // Gets the FileSecurityReport property.
  ///
  /// Succeeds if all of the following are true:
  /// - pFileSecurityReport is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pFileSecurityReport will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileSecurityReport is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetFileSecurityReport) (
    // value of the FileSecurityReport property 
    /*[out]*/ aafUInt32 *  pFileSecurityReport);


  //***********************************************************
  //
  // SetFileSecurityWave()
  //
  // Sets the FileSecurityWave property. This is an optional property.
  /// 
  /// If this method fails, the FileSecurityWave property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetFileSecurityWave) (
    // value to assign to the FileSecurityWave property 
    /*[in]*/ aafUInt32  fileSecurityWave);


  //***********************************************************
  //
  // GetFileSecurityWave()
  //
  // Gets the FileSecurityWave property.
  ///
  /// Succeeds if all of the following are true:
  /// - pFileSecurityWave is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pFileSecurityWave will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileSecurityWave is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetFileSecurityWave) (
    // value of the FileSecurityWave property 
    /*[out]*/ aafUInt32 *  pFileSecurityWave);


  //***********************************************************
  //
  // CountUnknownBWFChunks()
  //
  // Return total number of RIFFChunk objects attached to this component.
  ///
  /// Succeeds if all of the following are true:
  /// - the pNumData pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pNumComments.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumData arg is NULL.
  //
  STDMETHOD (CountUnknownBWFChunks) (
    // Number  of RIFFChunk objects 
    /*[out]*/ aafUInt32 *  pNumData);

  //***********************************************************
  //
  // AppendUnknownBWFChunk()
  //
  // Appends a pre-existing RIFFChunk object to end of this
  /// BWFImportDescriptor's list of UnknownBWFChunks.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
  /// - the pData pointer indicates an object which is not already
  ///   owned by any object
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
  ///   - the pData arg is NULL.
  ///
  /// AAFRESULT_OBJECT_ALREADY_ATTACHED
  ///   - the object pointed to by pData is already owned by this
  ///      or another object.
  //
  STDMETHOD (AppendUnknownBWFChunk) (
    // RIFFChunk object to append 
    /*[in]*/ IAAFRIFFChunk * pData);
  

  //***********************************************************
  //
  // PrependUnknownBWFChunk()
  //
  // Prepends a pre-existing RIFFChunk object to the
  /// beginning of this BWFImportDescriptor's list of UnknownBWFChunks.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
  /// - the pData pointer indicates an object which is not already
  ///   owned by any object
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
  ///   - pData is null.
  ///
  /// AAFRESULT_OBJECT_ALREADY_ATTACHED
  ///   - the object pointed to by pData is already owned by this
  ///      or another object.
  //
  STDMETHOD (PrependUnknownBWFChunk) (
    // RIFFChunk object to prepend 
    /*[in]*/ IAAFRIFFChunk * pData);



  //***********************************************************
  //
  // InsertUnknownBWFChunkAt()
  //
  // Inserts the given RIFFChunk at the given index in this 
  /// BWFImportDescriptor's list of UnknownBWFChunks.  Chunks already
  /// existing at the given and higher indices will be moved to the
  /// next higher index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
  /// - the pData pointer indicates an object which is not already
  ///   owned by any object
  /// - index is less than or equal to the value returned by
  ///   CountUnknownBWFChunks().
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
  ///   - pData is null.
  ///
  /// AAFRESULT_OBJECT_ALREADY_ATTACHED
  ///   - the object pointed to by pData is already owned by this
  ///      or another object.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than the value returned by
  ///     CountUnknownBWFChunks().
  //
  STDMETHOD (InsertUnknownBWFChunkAt) (
    // index at which chunk is to be inserted
    /*[in]*/ aafUInt32  index,

    // RIFFChunk to append
    /*[in]*/ IAAFRIFFChunk * pData);


  //***********************************************************
  //
  // GetUnknownBWFChunkAt()
  //
  // Retrieves the RIFFChunk at the given index in this BWFImportDescriptor's
  /// list of UnknownBWFChunks.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
  /// - index is less than the value returned by CountUnknownBWFChunks().
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
  ///   - ppData is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountUnknownBWFChunks().
  //
  STDMETHOD (GetUnknownBWFChunkAt) (
    // index of chunk to retrieve
    /*[in]*/ aafUInt32  index,

    // returned RIFFChunk
    /*[out, retval]*/ IAAFRIFFChunk ** ppData);


  //***********************************************************
  //
  // RemoveUnknownBWFChunkAt()
  //
  // Removes the RIFFChunk at the given index in this BWFImportDescriptor's
  /// list of UnknownBWFChunks.  Chunks already
  /// existing at indices higher than the given index will be moved to
  /// the next lower index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - index is less than the value returned by CountUnknownBWFChunks().
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
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountUnknownBWFChunks().
  //
  STDMETHOD (RemoveUnknownBWFChunkAt) (
    // index of chunk to remove 
    /*[in]*/ aafUInt32  index);


  //***********************************************************
  //
  // GetUnknownBWFChunks()
  //
  // Return the enumeration for all RIFFChunk objects on this component.  The returned
  /// enumerator is AddRef()ed before it is returned.  The enumerator
  /// is implemented as an EnumAAFRIFFChunks.
  /// 
  /// Succeeds if all of the following are true:
  /// - the ppEnum pointer is valid.
  /// 
  /// If this method fails nothing will be written to *ppEnum.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppEnum is null.
  //
  STDMETHOD (GetUnknownBWFChunks) (
    // RIFFChunk objects 
    /*[out]*/ IEnumAAFRIFFChunks ** ppEnum);


  //***********************************************************
  //
  // SetCodingHistory()
  //
  // Sets the CodingHistory string property.
  ///
  /// Set the CodingHistory property to the value specified in
  /// pCodingHistory.  A copy is made of the data so the caller
  /// retains ownership of the *pCodingHistory buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pCodingHistory pointer is valid.
  /// 
  /// If this method fails the CodingHistory property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCodingHistory arg is NULL.
  //
  STDMETHOD (SetCodingHistory) (
    // buffer from which CodingHistory is to be read 
    /*[in, string]*/ aafCharacter_constptr  pCodingHistory);


  //***********************************************************
  //
  // GetCodingHistory()
  //
  // Gets the CodingHistory string property.
  /// 
  /// Writes the CodingHistory property, with a trailing null
  /// character, into the pCodingHistory buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the CodingHistory property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetCodingHistoryBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pCodingHistory.
  /// 
  /// Succeeds if:
  /// - The pCodingHistory pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   CodingHistory.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCodingHistory arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold CodingHistory.
  //
  STDMETHOD (GetCodingHistory) (
    // buffer into which CodingHistory is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pCodingHistory,

    // size of *pCodingHistory buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetCodingHistoryBufLen()
  //
  // Returns size of buffer (in bytes) required for GetCodingHistory().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetCodingHistoryBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetBasicData()
  //
  // Sets the BasicData string property.
  ///
  /// Set the BasicData property to the value specified in
  /// pBasicData.  A copy is made of the data so the caller
  /// retains ownership of the *pBasicData buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pBasicData pointer is valid.
  /// 
  /// If this method fails the BasicData property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBasicData arg is NULL.
  //
  STDMETHOD (SetBasicData) (
    // buffer from which BasicData is to be read 
    /*[in, string]*/ aafCharacter_constptr  pBasicData);


  //***********************************************************
  //
  // GetBasicData()
  //
  // Gets the BasicData string property.
  /// 
  /// Writes the BasicData property, with a trailing null
  /// character, into the pBasicData buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the BasicData property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetBasicDataBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pBasicData.
  /// 
  /// Succeeds if:
  /// - The pBasicData pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   BasicData.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBasicData arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold BasicData.
  //
  STDMETHOD (GetBasicData) (
    // buffer into which BasicData is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pBasicData,

    // size of *pBasicData buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetBasicDataBufLen()
  //
  // Returns size of buffer (in bytes) required for GetBasicData().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetBasicDataBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);

  //***********************************************************
  //
  // SetStartOfModulation()
  //
  // Sets the StartOfModulation string property.
  ///
  /// Set the StartOfModulation property to the value specified in
  /// pStartOfModulation.  A copy is made of the data so the caller
  /// retains ownership of the *pStartOfModulation buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pStartOfModulation pointer is valid.
  /// 
  /// If this method fails the StartOfModulation property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pStartOfModulation arg is NULL.
  //
  STDMETHOD (SetStartOfModulation) (
    // buffer from which StartOfModulation is to be read 
    /*[in, string]*/ aafCharacter_constptr  pStartOfModulation);


  //***********************************************************
  //
  // GetStartOfModulation()
  //
  // Gets the StartOfModulation string property.
  /// 
  /// Writes the StartOfModulation property, with a trailing null
  /// character, into the pStartOfModulation buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the StartOfModulation property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetStartOfModulationBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pStartOfModulation.
  /// 
  /// Succeeds if:
  /// - The pStartOfModulation pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   StartOfModulation.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pStartOfModulation arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold StartOfModulation.
  //
  STDMETHOD (GetStartOfModulation) (
    // buffer into which StartOfModulation is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pStartOfModulation,

    // size of *pStartOfModulation buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetStartOfModulationBufLen()
  //
  // Returns size of buffer (in bytes) required for GetStartOfModulation().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetStartOfModulationBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQualityEvent()
  //
  // Sets the QualityEvent string property.
  ///
  /// Set the QualityEvent property to the value specified in
  /// pQualityEvent.  A copy is made of the data so the caller
  /// retains ownership of the *pQualityEvent buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQualityEvent pointer is valid.
  /// 
  /// If this method fails the QualityEvent property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQualityEvent arg is NULL.
  //
  STDMETHOD (SetQualityEvent) (
    // buffer from which QualityEvent is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQualityEvent);


  //***********************************************************
  //
  // GetQualityEvent()
  //
  // Gets the QualityEvent string property.
  /// 
  /// Writes the QualityEvent property, with a trailing null
  /// character, into the pQualityEvent buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QualityEvent property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQualityEventBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQualityEvent.
  /// 
  /// Succeeds if:
  /// - The pQualityEvent pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QualityEvent.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQualityEvent arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QualityEvent.
  //
  STDMETHOD (GetQualityEvent) (
    // buffer into which QualityEvent is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQualityEvent,

    // size of *pQualityEvent buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQualityEventBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQualityEvent().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetQualityEventBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);

  //***********************************************************
  //
  // SetEndOfModulation()
  //
  // Sets the EndOfModulation string property.
  ///
  /// Set the EndOfModulation property to the value specified in
  /// pEndOfModulation.  A copy is made of the data so the caller
  /// retains ownership of the *pEndOfModulation buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pEndOfModulation pointer is valid.
  /// 
  /// If this method fails the EndOfModulation property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEndOfModulation arg is NULL.
  //
  STDMETHOD (SetEndOfModulation) (
    // buffer from which EndOfModulation is to be read 
    /*[in, string]*/ aafCharacter_constptr  pEndOfModulation);


  //***********************************************************
  //
  // GetEndOfModulation()
  //
  // Gets the EndOfModulation string property.
  /// 
  /// Writes the EndOfModulation property, with a trailing null
  /// character, into the pEndOfModulation buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the EndOfModulation property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetEndOfModulationBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pEndOfModulation.
  /// 
  /// Succeeds if:
  /// - The pEndOfModulation pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   EndOfModulation.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEndOfModulation arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold EndOfModulation.
  //
  STDMETHOD (GetEndOfModulation) (
    // buffer into which EndOfModulation is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pEndOfModulation,

    // size of *pEndOfModulation buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetEndOfModulationBufLen()
  //
  // Returns size of buffer (in bytes) required for GetEndOfModulation().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetEndOfModulationBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQualityParameter()
  //
  // Sets the QualityParameter string property.
  ///
  /// Set the QualityParameter property to the value specified in
  /// pQualityParameter.  A copy is made of the data so the caller
  /// retains ownership of the *pQualityParameter buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQualityParameter pointer is valid.
  /// 
  /// If this method fails the QualityParameter property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQualityParameter arg is NULL.
  //
  STDMETHOD (SetQualityParameter) (
    // buffer from which QualityParameter is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQualityParameter);


  //***********************************************************
  //
  // GetQualityParameter()
  //
  // Gets the QualityParameter string property.
  /// 
  /// Writes the QualityParameter property, with a trailing null
  /// character, into the pQualityParameter buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QualityParameter property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQualityParameterBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQualityParameter.
  /// 
  /// Succeeds if:
  /// - The pQualityParameter pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QualityParameter.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQualityParameter arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QualityParameter.
  //
  STDMETHOD (GetQualityParameter) (
    // buffer into which QualityParameter is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQualityParameter,

    // size of *pQualityParameter buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQualityParameterBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQualityParameter().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetQualityParameterBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetOperatorComment()
  //
  // Sets the OperatorComment string property.
  ///
  /// Set the OperatorComment property to the value specified in
  /// pOperatorComment.  A copy is made of the data so the caller
  /// retains ownership of the *pOperatorComment buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pOperatorComment pointer is valid.
  /// 
  /// If this method fails the OperatorComment property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pOperatorComment arg is NULL.
  //
  STDMETHOD (SetOperatorComment) (
    // buffer from which OperatorComment is to be read 
    /*[in, string]*/ aafCharacter_constptr  pOperatorComment);


  //***********************************************************
  //
  // GetOperatorComment()
  //
  // Gets the OperatorComment string property.
  /// 
  /// Writes the OperatorComment property, with a trailing null
  /// character, into the pOperatorComment buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the OperatorComment property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetOperatorCommentBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pOperatorComment.
  /// 
  /// Succeeds if:
  /// - The pOperatorComment pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   OperatorComment.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pOperatorComment arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold OperatorComment.
  //
  STDMETHOD (GetOperatorComment) (
    // buffer into which OperatorComment is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pOperatorComment,

    // size of *pOperatorComment buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetOperatorCommentBufLen()
  //
  // Returns size of buffer (in bytes) required for GetOperatorComment().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetOperatorCommentBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetCueSheet()
  //
  // Sets the CueSheet string property.
  ///
  /// Set the CueSheet property to the value specified in
  /// pCueSheet.  A copy is made of the data so the caller
  /// retains ownership of the *pCueSheet buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pCueSheet pointer is valid.
  /// 
  /// If this method fails the CueSheet property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCueSheet arg is NULL.
  //
  STDMETHOD (SetCueSheet) (
    // buffer from which CueSheet is to be read 
    /*[in, string]*/ aafCharacter_constptr  pCueSheet);


  //***********************************************************
  //
  // GetCueSheet()
  //
  // Gets the CueSheet string property.
  /// 
  /// Writes the CueSheet property, with a trailing null
  /// character, into the pCueSheet buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the CueSheet property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetCueSheetBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pCueSheet.
  /// 
  /// Succeeds if:
  /// - The pCueSheet pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   CueSheet.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCueSheet arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold CueSheet.
  //
  STDMETHOD (GetCueSheet) (
    // buffer into which CueSheet is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pCueSheet,

    // size of *pCueSheet buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetCueSheetBufLen()
  //
  // Returns size of buffer (in bytes) required for GetCueSheet().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetCueSheetBufLen) (
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

#endif // ! __CAAFBWFImportDescriptor_h__


