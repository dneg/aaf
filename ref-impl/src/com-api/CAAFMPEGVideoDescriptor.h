//@doc
//@class    AAFMPEGVideoDescriptor | Implementation class for AAFMPEGVideoDescriptor
#ifndef __CAAFMPEGVideoDescriptor_h__
#define __CAAFMPEGVideoDescriptor_h__

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







#ifndef __CAAFCDCIDescriptor_h__
#include "CAAFCDCIDescriptor.h"
#endif


class CAAFMPEGVideoDescriptor
  : public IAAFMPEGVideoDescriptor,
    public CAAFCDCIDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFMPEGVideoDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFMPEGVideoDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  /// IAAFMPEGVideoDescriptor-supporting object.  This method must be
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
  // SetSingleSequence()
  //
  // Sets the SingleSequence property. This property is optional.
  /// 
  /// If this method fails, the SingleSequence property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetSingleSequence) (
    // value to assign to the SingleSequence property 
    /*[in]*/ aafBoolean_t  singleSequence);


  //***********************************************************
  //
  // GetSingleSequence()
  //
  // Gets the SingleSequence property.
  ///
  /// Succeeds if all of the following are true:
  /// - pSingleSequence is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pSingleSequence will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSingleSequence is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetSingleSequence) (
    // The value of the SingleSequence property 
    /*[out]*/ aafBoolean_t *  pSingleSequence);


  //***********************************************************
  //
  // SetConstantBPictureCount()
  //
  // Sets the ConstantBPictureCount property. This property is optional.
  /// 
  /// If this method fails, the ConstantBPictureCount property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetConstantBPictureCount) (
    // value to assign to the ConstantBPictureCount property 
    /*[in]*/ aafBoolean_t  constantBPictureCount);


  //***********************************************************
  //
  // GetConstantBPictureCount()
  //
  // Gets the ConstantBPictureCount property.
  ///
  /// Succeeds if all of the following are true:
  /// - pConstantBPictureCount is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pConstantBPictureCount will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSingleSequence is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetConstantBPictureCount) (
    // value of the ConstantBPictureCount property 
    /*[out]*/ aafBoolean_t *  pConstantBPictureCount);


  //***********************************************************
  //
  // SetCodedContentScanning()
  //
  // Sets the CodedContentScanning property.  This property is optional.
  ///
  /// Succeeds if all of the following are true:
  /// - codedContentDirection is a valid value.
  /// 
  /// If this method fails, the CodedContentScanning property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_AAFRESULT_INVALID_ENUM_VALUE
  ///   - codedContentDirection is not a valid value.
  //
  STDMETHOD (SetCodedContentScanning) (
    // Coded Content Scanning value. 
    /*[in]*/ aafContentScanningType_t  codedContentScanning);


  //***********************************************************
  //
  // GetCodedContentScanning()
  //
  // Gets the CodedContentScanning property.  This property is optional.
  ///
  /// Succeeds if all of the following are true:
  /// - pContentScanning is a valid pointer
  /// - the property is present.
  /// 
  /// If this method fails, pSContentScanning not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pContentScanning is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetCodedContentScanning) (
    // Coded content scanning value. 
    /*[out]*/ aafContentScanningType_t *  pContentScanning);


  //***********************************************************
  //
  // SetLowDelay()
  //
  // Sets the LowDelay property.  This property is optional.
  ///
  /// If this method fails, the LowDelay property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetLowDelay) (
    // value to assign to the LowDelay property 
    /*[in]*/ aafBoolean_t  lowDelay);


  //***********************************************************
  //
  // GetLowDelay()
  //
  // Gets the LowDelay property.
  ///
  /// Succeeds if all of the following are true:
  /// - pLowDelay is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pLowDelay will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLowDelay is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetLowDelay) (
    // value of the LowDelay property 
    /*[out]*/ aafBoolean_t *  pLowDelay);


  //***********************************************************
  //
  // SetClosedGOP()
  //
  // Sets the ClosedGOP property. This property is optional.
  /// 
  /// If this method fails, the ClosedGOP property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetClosedGOP) (
    // value to assign to the ClosedGOP property 
    /*[in]*/ aafBoolean_t  closedGOP);


  //***********************************************************
  //
  // GetClosedGOP()
  //
  // Gets the ClosedGOP property.
  ///
  /// Succeeds if all of the following are true:
  /// - pClosedGOP is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pClosedGOP will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pClosedGOP is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetClosedGOP) (
    // value of the ClosedGOP property 
    /*[out]*/ aafBoolean_t *  pClosedGOP);


  //***********************************************************
  //
  // SetIdenticalGOP()
  //
  // Sets the IdenticalGOP property. This is an optional property.
  /// 
  /// If this method fails, the IdenticalGOP property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.
  ///
  //
  STDMETHOD (SetIdenticalGOP) (
    // value to assign to the IdenticalGOP property 
    /*[in]*/ aafBoolean_t  identicalGOP);


  //***********************************************************
  //
  // GetIdenticalGOP()
  //
  // Gets the IdenticalGOP property.
  ///
  /// Succeeds if all of the following are true:
  /// - pIdenticalGOP is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pIdenticalGOP will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pIdenticalGOP is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetIdenticalGOP) (
    // value of the IdenticalGOP property 
    /*[out]*/ aafBoolean_t *  pIdenticalGOP);


  //***********************************************************
  //
  // SetMaxGOP()
  //
  // Sets the MaxGOP property. This is an optional property.
  ///
  /// If this method fails, the MaxGOP property will not be
  /// changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetMaxGOP) (
    // value to assign to the MaxGOP property 
    /*[in]*/ aafUInt16  maxGOP);

  //***********************************************************
  //
  // GetMaxGOP()
  //
  // Gets the MaxGOP property.
  ///
  /// Succeeds if all of the following are true:
  /// - pMaxGOP is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pMaxGOP will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pMaxGOP is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetMaxGOP) (
    // value of the MaxGOP property 
    /*[out]*/ aafUInt16 *  pMaxGOP);


  //***********************************************************
  //
  // SetMaxBPictureCount()
  //
  // Sets the MaxBPictureCount property. This is an optional property.
  ///
  /// If this method fails, the MaxBPictureCount property will not be
  /// changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetMaxBPictureCount) (
    // value to assign to the MaxBPictureCount property 
    /*[in]*/ aafUInt16  maxBPictureCount);

  //***********************************************************
  //
  // GetMaxBPictureCount()
  //
  // Gets the MaxBPictureCount property.
  ///
  /// Succeeds if all of the following are true:
  /// - pMaxBPictureCount is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pMaxBPictureCount will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pMaxBPictureCount is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetMaxBPictureCount) (
    // valud of the MaxBPictureCount property 
    /*[out]*/ aafUInt16 *  pMaxBPictureCount);


  //***********************************************************
  //
  // SetBitRate()
  //
  // Sets the BitRate property. This is an optional property.
  ///
  /// If this method fails, the BitRate property will not be
  /// changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetBitRate) (
    // value to assign to the BitRate property 
    /*[in]*/ aafUInt32  bitRate);

  //***********************************************************
  //
  // GetBitRate()
  //
  // Gets the BitRate property.
  ///
  /// Succeeds if all of the following are true:
  /// - pBitRate is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pBitRate will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBitRate is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetBitRate) (
    // value of the BitRate property 
    /*[out]*/ aafUInt32 *  pBitRate);


  //***********************************************************
  //
  // SetProfileAndLevel()
  //
  // Sets the ProfileAndLevel property. This is an optional property.
  ///
  /// If this method fails, the ProfileAndLevel property will not be
  /// changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetProfileAndLevel) (
    // value to assign to the ProfileAndLevel property 
    /*[in]*/ aafUInt8  profileAndLevel);

  //***********************************************************
  //
  // GetProfileAndLevel()
  //
  // Gets the ProfileAndLevele property.
  ///
  /// Succeeds if all of the following are true:
  /// - pProfileAndLevel is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pProfileAndLevel will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pProfileAndLevel is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  //
  STDMETHOD (GetProfileAndLevel) (
    // value of the ProfileAndLevel property 
    /*[out]*/ aafUInt8 *  pProfileAndLevel);


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

#endif // ! __CAAFMPEGVideoDescriptor_h__


