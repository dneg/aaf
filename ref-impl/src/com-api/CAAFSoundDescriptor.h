//@doc
//@class    AAFSoundDescriptor | Implementation class for AAFSoundDescriptor
#ifndef __CAAFSoundDescriptor_h__
#define __CAAFSoundDescriptor_h__

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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
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





#ifndef __CAAFFileDescriptor_h__
#include "CAAFFileDescriptor.h"
#endif


class CAAFSoundDescriptor
  : public IAAFSoundDescriptor,
    public CAAFFileDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSoundDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSoundDescriptor ();

public:


  //***********************************************************
  //
  // SetCompression()
  //
  // /// Sets the kind of compression and format of compression
  /// information of the sound essence data. This property is
  /// optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the Compression property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetCompression) (
    // Identifies kind of compression and format of
	/// compression information. 
    /*[in]*/ aafUID_constref  compression);


  //***********************************************************
  //
  // GetCompression()
  //
  // /// Gets the kind of compression and format of compression
  /// information of the sound essence data. This property is
  /// optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pCompression pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pCompression.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCompression arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetCompression) (
    // Compression of the sound essence data. 
    /*[out]*/ aafUID_t *  pCompression);


  //***********************************************************
  //
  // SetChannelCount()
  //
  // /// Sets the number of channels of sound represented by
  /// this descriptor. This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the Channels property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetChannelCount) (
    // The number of channels of sound represented
	/// by this descriptor. 
    /*[in]*/ aafUInt32  channelCount);


  //***********************************************************
  //
  // GetChannelCount()
  //
  // /// Gets the number of channels of sound represented by
  /// this descriptor. This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pChannelCount pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pChannelCount.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pChannelCount arg is NULL.
  //
  STDMETHOD (GetChannelCount) (
    // The number of channels of sound represented
	/// by this descriptor. 
    /*[out]*/ aafUInt32 *  pChannelCount);


  //***********************************************************
  //
  // SetAudioSamplingRate()
  //
  // /// Sets the sample rate of audio represented by this descriptor.
  /// This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the AudioSamplingRate property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetAudioSamplingRate) (
    // Sample rate of audio essence. 
    /*[in]*/ aafRational_t  rate);


  //***********************************************************
  //
  // GetAudioSamplingRate()
  //
  // /// Gets the sample rate of audio represented by this descriptor.
  /// This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pRate pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pRate.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pRate arg is NULL.
  //
  STDMETHOD (GetAudioSamplingRate) (
    // Sample rate of audio essence. 
    /*[out]*/ aafRational_t *  pRate);


  //***********************************************************
  //
  // SetIsLocked()
  //
  // /// Sets the Locked flag which indicates whether the number of
  /// samples per frame is locked or unlocked. This property is
  /// optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the Locked property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetIsLocked) (
    // Is number of samples per frame locked to video? 
    /*[in]*/ aafBoolean_t  locked);


  //***********************************************************
  //
  // IsLocked()
  //
  // /// Gets the Locked flag which indicates whether the number of
  /// samples per frame is locked or unlocked. This property is
  /// optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pLocked pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pLocked.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLocked arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (IsLocked) (
    // Is number of samples per frame locked to video? 
    /*[out]*/ aafBoolean_t *  pLocked);


  //***********************************************************
  //
  // SetElectroSpatialFormulation()
  //
  // /// Sets the electro-spatial form of the signal. This property is
  /// optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the ElectroSpatial property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetElectroSpatialFormulation) (
    // Electro-spatial form of the signal. 
    /*[in]*/ aafElectroSpatialFormulation_t  formulation);


  //***********************************************************
  //
  // GetElectroSpatialFormulation()
  //
  // /// Gets the electro-spatial form of the signal. This property is
  /// optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pFormulation pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pFormulation.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormulation arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetElectroSpatialFormulation) (
    // Electro-spatial form of the signal. 
    /*[out]*/ aafElectroSpatialFormulation_t *  pFormulation);


  //***********************************************************
  //
  // SetAudioRefLevel()
  //
  // /// Sets the audio reference level. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the AudioRefLevel property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetAudioRefLevel) (
    // Audio reference level. 
    /*[in]*/ aafInt8  level);


  //***********************************************************
  //
  // GetAudioRefLevel()
  //
  // /// Gets the audio reference level. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pLevel pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pLevel.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLevel arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetAudioRefLevel) (
    // Audio reference level. 
    /*[out]*/ aafInt8 *  pLevel);


  //***********************************************************
  //
  // SetDialNorm()
  //
  // /// Sets the dial norm. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the DialNorm property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetDialNorm) (
    // Dial norm. 
    /*[in]*/ aafInt8  dialNorm);


  //***********************************************************
  //
  // GetDialNorm()
  //
  // /// Gets the dial norm. This property is optional.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pDialNorm pointer is valid.
  /// - the property is present.
  /// 
  /// If this method fails nothing will be written to *pDialNorm.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDialNorm arg is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetDialNorm) (
    // Dial norm. 
    /*[out]*/ aafInt8 *  pDialNorm);


  //***********************************************************
  //
  // SetQuantizationBits()
  //
  // /// Sets the number of quantization bits. This property is required.
  ///  
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// 
  /// If this method fails the QuantizationBits property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetQuantizationBits) (
    // Number of quantization bits. 
    /*[in]*/ aafUInt32  bitsCount);


  //***********************************************************
  //
  // GetQuantizationBits()
  //
  // /// Gets the number of quantization bits. This property is required.
  /// 
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pBitsCount pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pBitsCount.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBitsCount arg is NULL.
  //
  STDMETHOD (GetQuantizationBits) (
    // Number of quantization bits. 
    /*[out]*/ aafUInt32 *  pBitsCount);

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

#endif // ! __CAAFSoundDescriptor_h__


