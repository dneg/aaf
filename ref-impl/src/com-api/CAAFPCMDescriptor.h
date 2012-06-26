//@doc
//@class    AAFPCMDescriptor | Implementation class for AAFPCMDescriptor
#ifndef __CAAFPCMDescriptor_h__
#define __CAAFPCMDescriptor_h__

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





#ifndef __CAAFSoundDescriptor_h__
#include "CAAFSoundDescriptor.h"
#endif


class CAAFPCMDescriptor
  : public IAAFPCMDescriptor,
    public CAAFSoundDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPCMDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPCMDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, IAAFPCMDescriptor-supporting
  /// object. This method must be called after allocation, and before
  /// any other method can be called.
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
  // SetBlockAlign()
  //
  // Sets the number of bytes used to store one sample of all channels.
  /// This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the BlockAlign property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetBlockAlign) (
    // The number of bytes used to store one sample of all channels. 
    /*[in]*/ aafUInt16  blockAlign);


  //***********************************************************
  //
  // GetBlockAlign()
  //
  // Gets the number of bytes used to store one sample of all channels.
  /// This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pBlockAlign pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pBlockAlign.
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
  ///   - pBlockAlign arg is NULL.
  //
  STDMETHOD (GetBlockAlign) (
    // The number of bytes used to store one sample of all channels. 
    /*[out]*/ aafUInt16 *  pBlockAlign);


  //***********************************************************
  //
  // SetSequenceOffset()
  //
  // Sets the frame number of the beginning of the essence data
  /// within a five-frame sequence. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the SequenceOffset property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetSequenceOffset) (
    // Zero-based ordinal frame number of the beginning of
	/// the essence data within a five-frame sequence. 
    /*[in]*/ aafUInt8  offset);


  //***********************************************************
  //
  // GetSequenceOffset()
  //
  // Gets the frame number of the beginning of the essence data
  /// within a five-frame sequence. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pOffset pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pOffset.
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
  ///   - pOffset arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetSequenceOffset) (
    // Zero-based ordinal frame number of the beginning of
	/// the essence data within a five-frame sequence. 
    /*[out]*/ aafUInt8 *  pOffset);


  //***********************************************************
  //
  // SetAverageBPS()
  //
  // Sets the average bytes per second of the essence stream.
  /// This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the AverageBPS property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetAverageBPS) (
    // Average bytes per second of the essence stream. 
    /*[in]*/ aafUInt32  bps);


  //***********************************************************
  //
  // GetAverageBPS()
  //
  // Gets the average bytes per second of the essence stream.
  /// This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pBps pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pBps.
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
  ///   - pBps arg is NULL.
  //
  STDMETHOD (GetAverageBPS) (
    // Average bytes per second of the essence stream. 
    /*[out]*/ aafUInt32 *  pBps);


  //***********************************************************
  //
  // SetChannelAssignment()
  //
  // Sets the channel assignment scheme. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the ChannelAssignment property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetChannelAssignment) (
    // The channel assignment to use. 
    /*[in, ref]*/ aafUID_constref  channelAssignment);


  //***********************************************************
  //
  // GetChannelAssignment()
  //
  // Gets the channel assignment scheme. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pChannelAssignment pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pChannelAssignment.
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
  ///   - pChannelAssignment arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetChannelAssignment) (
    // The channel assignment in use. 
    /*[out]*/ aafUID_t *  pChannelAssignment);


  //***********************************************************
  //
  // AreAllPeakEnvelopePropertiesPresent()
  //
  // Places TRUE into *pArePresent if the following optional
  /// properties are set on the descriptor:
  ///   PeakEnvelopeVersion
  ///   PeakEnvelopeFormat
  ///   PointsPerPeakValue
  ///   PeakEnvelopeBlockSize
  ///   PeakChannels
  ///   PeakFrames
  ///   PeakOfPeaksPosition
  ///   PeakEnvelopeTimestamp
  ///   PeakEnvelopeData
  ///
  /// Succeeds if all of the following are true:
  /// - the pArePresent pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pIsPresent.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pArePresent arg is NULL.
  //
  STDMETHOD (AreAllPeakEnvelopePropertiesPresent) (
    // The flag indicating presence of the optional properties
	/// that form peak envelope. 
    /*[out]*/ aafBoolean_t *  pArePresent);


  //***********************************************************
  //
  // SetPeakEnvelopeVersion()
  //
  // Sets the version of the peak envelope data.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the version will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakEnvelopeVersion) (
    // Version of the peak envelope data. 
    /*[in]*/ aafUInt32  version);


  //***********************************************************
  //
  // GetPeakEnvelopeVersion()
  //
  // Gets the version of the peak envelope data.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pVersion pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pVersion.
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
  ///   - pVersion arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakEnvelopeVersion) (
    // Version of the peak envelope data. 
    /*[out]*/ aafUInt32 *  pVersion);


  //***********************************************************
  //
  // SetPeakEnvelopeFormat()
  //
  // Sets the format of the peak point.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the format will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakEnvelopeFormat) (
    // Format of the peak point. 
    /*[in]*/ aafUInt32  format);


  //***********************************************************
  //
  // GetPeakEnvelopeFormat()
  //
  // Gets the format of the peak point.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pFormat pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pFormat.
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
  ///   - pFormat arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakEnvelopeFormat) (
    // Format of the peak point. 
    /*[out]*/ aafUInt32 *  pFormat);


  //***********************************************************
  //
  // SetPointsPerPeakValue()
  //
  // Sets the number of peak points per peak value.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the PointsPerPeakValue property will
  /// not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPointsPerPeakValue) (
    // The number of peak points per peak value. 
    /*[in]*/ aafUInt32  pointCount);


  //***********************************************************
  //
  // GetPointsPerPeakValue()
  //
  // Gets the number of peak points per peak value.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pPointCount pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pPointCount.
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
  ///   - pPointCount arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPointsPerPeakValue) (
    // The number of peak points per peak value. 
    /*[out]*/ aafUInt32 *  pPointCount);


  //***********************************************************
  //
  // SetPeakEnvelopeBlockSize()
  //
  // Sets the number of audio samples used to generate each peak frame.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the PeakEnvelopeBlockSize property will
  /// not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakEnvelopeBlockSize) (
    // The number of audio samples used to generate each peak frame. 
    /*[in]*/ aafUInt32  blockSize);


  //***********************************************************
  //
  // GetPeakEnvelopeBlockSize()
  //
  // Gets the number of audio samples used to generate each peak frame.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pBlockSize pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pBlockSize.
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
  ///   - pBlockSize arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakEnvelopeBlockSize) (
    // The number of audio samples used to generate each peak frame. 
    /*[out]*/ aafUInt32 *  pBlockSize);


  //***********************************************************
  //
  // SetPeakChannelCount()
  //
  // Sets the number of peak channels.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the channel count will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakChannelCount) (
    // The number of peak channels. 
    /*[in]*/ aafUInt32  channelCount);


  //***********************************************************
  //
  // GetPeakChannelCount()
  //
  // Gets the number of peak channels.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pChannelCount pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pChannelCount.
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
  ///   - pChannelCount arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakChannelCount) (
    // The number of peak channels. 
    /*[out]*/ aafUInt32 *  pChannelCount);


  //***********************************************************
  //
  // SetPeakFrameCount()
  //
  // Sets the number of peak frames.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the frame count will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakFrameCount) (
    // The number of peak frames. 
    /*[in]*/ aafUInt32  frameCount);


  //***********************************************************
  //
  // GetPeakFrameCount()
  //
  // Gets the number of peak frames.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pFrameCount pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pFrameCount.
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
  ///   - pFrameCount arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakFrameCount) (
    // The number of peak frames. 
    /*[out]*/ aafUInt32 *  pFrameCount);


  //***********************************************************
  //
  // SetPeakOfPeaksPosition()
  //
  // Sets the offset to the first audio sample whose absolute
  /// value is the maximum value of the entire audio file.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the PeakOfPeaksPosition property will
  /// not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakOfPeaksPosition) (
    // The offset to peak of peaks 
    /*[in]*/ aafPosition_t  position);


  //***********************************************************
  //
  // GetPeakOfPeaksPosition()
  //
  // Gets the offset to the first audio sample whose absolute
  /// value is the maximum value of the entire audio file.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pPosition pointer is valid.
  /// - the property is present.
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
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakOfPeaksPosition) (
    // The offset to peak of peaks. 
    /*[out]*/ aafPosition_t *  pPosition);


  //***********************************************************
  //
  // SetPeakEnvelopeTimestamp()
  //
  // Sets the time stamp of the creation of the peak data.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the time stamp will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  //
  STDMETHOD (SetPeakEnvelopeTimestamp) (
    // The time stamp of the creation of the peak data. 
    /*[in]*/ aafTimeStamp_constref  timeStamp);


  //***********************************************************
  //
  // GetPeakEnvelopeTimestamp()
  //
  // Gets the time stamp of the creation of the peak data.
  /// This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pTimeStamp pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pTimeStamp.
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
  ///   - pTimeStamp arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetPeakEnvelopeTimestamp) (
    // The time stamp of the creation of the peak data. 
    /*[out]*/ aafTimeStamp_t *  pTimeStamp);


  //***********************************************************
  //
  // SetPeakEnvelopeDataPosition()
  //
  // Sets the offset from the beginning of peak envelope data.
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
  STDMETHOD (SetPeakEnvelopeDataPosition) (
    // Offset from the beginning of peak envelope data. 
    /*[in]*/ aafPosition_t  position);


  //***********************************************************
  //
  // GetPeakEnvelopeDataPosition()
  //
  // Gets the offset from the beginning of peak envelope data.
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
  ///   - the PeakEnvelopeData property is not present.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (GetPeakEnvelopeDataPosition) (
    // Offset from the beginning of peak envelope data. 
    /*[out]*/ aafPosition_t *  pPosition);


  //***********************************************************
  //
  // GetPeakEnvelopeDataSize()
  //
  // Gets the size of peak envelope data.
  /// PeakEnvelopeData is optional property.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pSize pointer is valid.
  /// - the PeakEnvelopeData property is present.
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
  ///   - the PeakEnvelopeData property is not present.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (GetPeakEnvelopeDataSize) (
    // The size of peak envelope data. 
    /*[out]*/ aafLength_t *  pSize);


  //***********************************************************
  //
  // WritePeakEnvelopeData()
  //
  // Write the specified bytes to the peak envelope data stream.
  /// 
  /// Succeeds if all of the following are true:
  /// - the number of bytes to write is non-zero.
  /// - the buffer pointer is valid.
  /// - the pBytesWritten pointer is valid.
  /// - the object is initialized.
  /// - the object is persistent (attached to a file).
  /// 
  /// If this method fails the PeakEnvelopeData property will
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
  STDMETHOD (WritePeakEnvelopeData) (
    // Write this many bytes
    /*[in]*/ aafUInt32  bytes,

    // Data to write
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // Number of bytes actually written.
    /*[out,ref]*/ aafUInt32 *  pBytesWritten);


  //***********************************************************
  //
  // ReadPeakEnvelopeData()
  //
  // Read the specified number of bytes from the peak envelope data
  /// stream into buffer.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the number of bytes to read is non-zero.
  /// - the buffer pointer is valid.
  /// - the pBytesRead pointer is valid.
  /// - the PeakEnvelopeData property is present.
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
  ///   - the PeakEnvelopeData property is not present.
  ///
  /// AAFRESULT_OBJECT_NOT_PERSISTENT
  ///   - the object is not persistent.
  //
  STDMETHOD (ReadPeakEnvelopeData) (
    // Read this many bytes
    /*[in]*/ aafUInt32  bytes,

    // Buffer to read the data to
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // Number of bytes actually read.
    /*[out,ref]*/ aafUInt32 *  pBytesRead);

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

#endif // ! __CAAFPCMDescriptor_h__


