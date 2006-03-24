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
  // the FileSecurityReport property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetFileSecurityReport) (
    // Sets the fileSecurityReport property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails 
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
    // Optional. 
    /*[out]*/ aafUInt32 *  pFileSecurityReport);


  //***********************************************************
  //
  // SetFileSecurityWave()
  //
  // the FileSecurityWave property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetFileSecurityWave) (
    // Sets the FileSecurityWave property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails 
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
  ///   - pQltyFileSecurityWave is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetFileSecurityWave) (
    // Optional. 
    /*[out]*/ aafUInt32 *  pFileSecurityWave);


  //***********************************************************
  //
  // AppendUnknownBWFChunks()
  //
  // Appends a pre-existing RIFFChunk object to the specified
  /// BWFImportDescriptor.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
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
  //
  STDMETHOD (AppendUnknownBWFChunks) (
    // RIFFChunk object 
    /*[in]*/ IAAFRIFFChunk * pData);


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
  // GetUnknownBWFChunks()
  //
  // Return the enumeration for all RIFFChunk objects on this component.  The returned
  /// enumerator is AddRef()ed before it is returned.  The enumerator
  /// is implemented as a EnumAAFRIFFChunks.
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
  // RemoveUnknownBWFChunks()
  //
  // Removes the given RIFFChunk object from this component.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pData pointer is valid.
  /// - the given RIFFChunk object is present in the component.
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
  /// AAFRESULT_OBJECT_NOT_FOUND
  ///   - the given RIFFChunk object is not in this component.
  //
  STDMETHOD (RemoveUnknownBWFChunks) (
    // RIFFChunk object to remove 
    /*[in]*/ IAAFRIFFChunk * pData);

  //***********************************************************
  //
  // SetBextCodingHistory()
  //
  // Sets the BextCodingHistory string property.
  ///
  /// Set the BextCodingHistory property to the value specified in
  /// pBextCodingHistory.  A copy is made of the data so the caller
  /// retains ownership of the *pBextCodingHistory buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pBextCodingHistory pointer is valid.
  /// 
  /// If this method fails the BextCodingHistory property will not be
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
  //   - pBextCodingHistory arg is NULL.
  //
  STDMETHOD (SetBextCodingHistory) (
    // buffer from which BextCodingHistory is to be read 
    /*[in, string]*/ aafCharacter_constptr  pBextCodingHistory);


  //***********************************************************
  //
  // GetBextCodingHistory()
  //
  // Gets the BextCodingHistory string property.
  /// 
  /// Writes the BextCodingHistory property, with a trailing null
  /// character, into the pBextCodingHistory buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the BextCodingHistory property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetBextCodingHistoryBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pBextCodingHistory.
  /// 
  /// Succeeds if:
  /// - The pBextCodingHistory pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   BextCodingHistory.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBextCodingHistory arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold BextCodingHistory.
  //
  STDMETHOD (GetBextCodingHistory) (
    // buffer into which BextCodingHistory is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pBextCodingHistory,

    // size of *pBextCodingHistory buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetBextCodingHistoryBufLen()
  //
  // Returns size of buffer (in bytes) required for GetBextCodingHistory().
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
  STDMETHOD (GetBextCodingHistoryBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQltyBasicData()
  //
  // Sets the QltyBasicData string property.
  ///
  /// Set the QltyBasicData property to the value specified in
  /// pQltyBasicData.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyBasicData buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyBasicData pointer is valid.
  /// 
  /// If this method fails the QltyBasicData property will not be
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
  //   - pQltyBasicData arg is NULL.
  //
  STDMETHOD (SetQltyBasicData) (
    // buffer from which QltyBasicData is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyBasicData);


  //***********************************************************
  //
  // GetQltyBasicData()
  //
  // Gets the QltyBasicData string property.
  /// 
  /// Writes the QltyBasicData property, with a trailing null
  /// character, into the pQltyBasicData buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyBasicData property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyBasicDataBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyBasicData.
  /// 
  /// Succeeds if:
  /// - The pQltyBasicData pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyBasicData.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyBasicData arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyBasicData.
  //
  STDMETHOD (GetQltyBasicData) (
    // buffer into which QltyBasicData is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyBasicData,

    // size of *pQltyBasicData buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyBasicDataBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyBasicData().
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
  STDMETHOD (GetQltyBasicDataBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);

  //***********************************************************
  //
  // SetQltyStartOfModulation()
  //
  // Sets the QltyStartOfModulation string property.
  ///
  /// Set the QltyStartOfModulation property to the value specified in
  /// pQltyStartOfModulation.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyStartOfModulation buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyStartOfModulation pointer is valid.
  /// 
  /// If this method fails the QltyStartOfModulation property will not be
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
  //   - pQltyStartOfModulation arg is NULL.
  //
  STDMETHOD (SetQltyStartOfModulation) (
    // buffer from which QltyStartOfModulation is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyStartOfModulation);


  //***********************************************************
  //
  // GetQltyStartOfModulation()
  //
  // Gets the QltyStartOfModulation string property.
  /// 
  /// Writes the QltyStartOfModulation property, with a trailing null
  /// character, into the pQltyStartOfModulation buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyStartOfModulation property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyStartOfModulationBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyStartOfModulation.
  /// 
  /// Succeeds if:
  /// - The pQltyStartOfModulation pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyStartOfModulation.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyStartOfModulation arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyStartOfModulation.
  //
  STDMETHOD (GetQltyStartOfModulation) (
    // buffer into which QltyStartOfModulation is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyStartOfModulation,

    // size of *pQltyStartOfModulation buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyStartOfModulationBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyStartOfModulation().
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
  STDMETHOD (GetQltyStartOfModulationBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQltyQualityEvent()
  //
  // Sets the QltyQualityEvent string property.
  ///
  /// Set the QltyQualityEvent property to the value specified in
  /// pQltyQualityEvent.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyQualityEvent buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyQualityEvent pointer is valid.
  /// 
  /// If this method fails the QltyQualityEvent property will not be
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
  //   - pQltyQualityEvent arg is NULL.
  //
  STDMETHOD (SetQltyQualityEvent) (
    // buffer from which QltyQualityEvent is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyQualityEvent);


  //***********************************************************
  //
  // GetQltyQualityEvent()
  //
  // Gets the QltyQualityEvent string property.
  /// 
  /// Writes the QltyQualityEvent property, with a trailing null
  /// character, into the pQltyQualityEvent buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyQualityEvent property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyQualityEventBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyQualityEvent.
  /// 
  /// Succeeds if:
  /// - The pQltyQualityEvent pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyQualityEvent.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyQualityEvent arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyQualityEvent.
  //
  STDMETHOD (GetQltyQualityEvent) (
    // buffer into which QltyQualityEvent is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyQualityEvent,

    // size of *pQltyQualityEvent buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyQualityEventBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyQualityEvent().
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
  STDMETHOD (GetQltyQualityEventBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);

  //***********************************************************
  //
  // SetQltyEndOfModulation()
  //
  // Sets the QltyEndOfModulation string property.
  ///
  /// Set the QltyEndOfModulation property to the value specified in
  /// pQltyEndOfModulation.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyEndOfModulation buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyEndOfModulation pointer is valid.
  /// 
  /// If this method fails the QltyEndOfModulation property will not be
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
  //   - pQltyEndOfModulation arg is NULL.
  //
  STDMETHOD (SetQltyEndOfModulation) (
    // buffer from which QltyEndOfModulation is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyEndOfModulation);


  //***********************************************************
  //
  // GetQltyEndOfModulation()
  //
  // Gets the QltyEndOfModulation string property.
  /// 
  /// Writes the QltyEndOfModulation property, with a trailing null
  /// character, into the pQltyEndOfModulation buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyEndOfModulation property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyEndOfModulationBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyEndOfModulation.
  /// 
  /// Succeeds if:
  /// - The pQltyEndOfModulation pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyEndOfModulation.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyEndOfModulation arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyEndOfModulation.
  //
  STDMETHOD (GetQltyEndOfModulation) (
    // buffer into which QltyEndOfModulation is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyEndOfModulation,

    // size of *pQltyEndOfModulation buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyEndOfModulationBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyEndOfModulation().
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
  STDMETHOD (GetQltyEndOfModulationBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQltyQualityParameter()
  //
  // Sets the QltyQualityParameter string property.
  ///
  /// Set the QltyQualityParameter property to the value specified in
  /// pQltyQualityParameter.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyQualityParameter buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyQualityParameter pointer is valid.
  /// 
  /// If this method fails the QltyQualityParameter property will not be
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
  //   - pQltyQualityParameter arg is NULL.
  //
  STDMETHOD (SetQltyQualityParameter) (
    // buffer from which QltyQualityParameter is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyQualityParameter);


  //***********************************************************
  //
  // GetQltyQualityParameter()
  //
  // Gets the QltyQualityParameter string property.
  /// 
  /// Writes the QltyQualityParameter property, with a trailing null
  /// character, into the pQltyQualityParameter buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyQualityParameter property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyQualityParameterBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyQualityParameter.
  /// 
  /// Succeeds if:
  /// - The pQltyQualityParameter pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyQualityParameter.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyQualityParameter arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyQualityParameter.
  //
  STDMETHOD (GetQltyQualityParameter) (
    // buffer into which QltyQualityParameter is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyQualityParameter,

    // size of *pQltyQualityParameter buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyQualityParameterBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyQualityParameter().
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
  STDMETHOD (GetQltyQualityParameterBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQltyOperatorComment()
  //
  // Sets the QltyOperatorComment string property.
  ///
  /// Set the QltyOperatorComment property to the value specified in
  /// pQltyOperatorComment.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyOperatorComment buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyOperatorComment pointer is valid.
  /// 
  /// If this method fails the QltyOperatorComment property will not be
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
  //   - pQltyOperatorComment arg is NULL.
  //
  STDMETHOD (SetQltyOperatorComment) (
    // buffer from which QltyOperatorComment is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyOperatorComment);


  //***********************************************************
  //
  // GetQltyOperatorComment()
  //
  // Gets the QltyOperatorComment string property.
  /// 
  /// Writes the QltyOperatorComment property, with a trailing null
  /// character, into the pQltyOperatorComment buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyOperatorComment property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyOperatorCommentBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyOperatorComment.
  /// 
  /// Succeeds if:
  /// - The pQltyOperatorComment pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyOperatorComment.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyOperatorComment arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyOperatorComment.
  //
  STDMETHOD (GetQltyOperatorComment) (
    // buffer into which QltyOperatorComment is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyOperatorComment,

    // size of *pQltyOperatorComment buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyOperatorCommentBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyOperatorComment().
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
  STDMETHOD (GetQltyOperatorCommentBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);
     
  //***********************************************************
  //
  // SetQltyCueSheet()
  //
  // Sets the QltyCueSheet string property.
  ///
  /// Set the QltyCueSheet property to the value specified in
  /// pQltyCueSheet.  A copy is made of the data so the caller
  /// retains ownership of the *pQltyCueSheet buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pQltyCueSheet pointer is valid.
  /// 
  /// If this method fails the QltyCueSheet property will not be
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
  //   - pQltyCueSheet arg is NULL.
  //
  STDMETHOD (SetQltyCueSheet) (
    // buffer from which QltyCueSheet is to be read 
    /*[in, string]*/ aafCharacter_constptr  pQltyCueSheet);


  //***********************************************************
  //
  // GetQltyCueSheet()
  //
  // Gets the QltyCueSheet string property.
  /// 
  /// Writes the QltyCueSheet property, with a trailing null
  /// character, into the pQltyCueSheet buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the QltyCueSheet property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetQltyCueSheetBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pQltyCueSheet.
  /// 
  /// Succeeds if:
  /// - The pQltyCueSheet pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   QltyCueSheet.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pQltyCueSheet arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold QltyCueSheet.
  //
  STDMETHOD (GetQltyCueSheet) (
    // buffer into which QltyCueSheet is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pQltyCueSheet,

    // size of *pQltyCueSheet buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetQltyCueSheetBufLen()
  //
  // Returns size of buffer (in bytes) required for GetQltyCueSheet().
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
  STDMETHOD (GetQltyCueSheetBufLen) (
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


