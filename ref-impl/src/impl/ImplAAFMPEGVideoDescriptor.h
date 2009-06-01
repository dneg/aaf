//@doc
//@class    AAFMPEGVideoDescriptor | Implementation class for AAFMPEGVideoDescriptor
#ifndef __ImplAAFMPEGVideoDescriptor_h__
#define __ImplAAFMPEGVideoDescriptor_h__

//=---------------------------------------------------------------------=
//
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
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFCDCIDescriptor_h__
#include "ImplAAFCDCIDescriptor.h"
#endif


class ImplAAFMPEGVideoDescriptor : public ImplAAFCDCIDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMPEGVideoDescriptor ();

protected:
  virtual ~ImplAAFMPEGVideoDescriptor ();
 
private:

	OMFixedSizeProperty<aafBoolean_t>                   _singleSequence;
  	OMFixedSizeProperty<aafBoolean_t>                   _constantBPictureCount;
  	OMFixedSizeProperty<aafContentScanningType_t>		_codedContentScanning;
  	OMFixedSizeProperty<aafBoolean_t>                   _lowDelay;
  	OMFixedSizeProperty<aafBoolean_t>                   _closedGOP;
  	OMFixedSizeProperty<aafBoolean_t>                   _identicalGOP;
  	OMFixedSizeProperty<aafUInt16>                      _maxGOP;
  	OMFixedSizeProperty<aafUInt16> 						_maxBPictureCount;
    OMFixedSizeProperty<aafUInt32>                      _bitRate;
    OMFixedSizeProperty<aafInt8>                        _profileAndLevel;
    
    

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();



  //****************
  // SetSingleSequence()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSingleSequence
        // @parm [in] Sets the SingleSequence property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafBoolean_t  singleSequence);


  //****************
  // GetSingleSequence()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSingleSequence
        // @parm [out] Optional.
        (aafBoolean_t *  pSingleSequence);


  //****************
  // SetConstantBPictureCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetConstantBPictureCount
        // @parm [in] Sets the ConstantBPictureCount property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafBoolean_t  constantBPictureCount);


  //****************
  // GetConstantBPictureCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetConstantBPictureCount
        // @parm [out] Optional.
        (aafBoolean_t *  pConstantBPictureCount);


  //****************
  // SetCodedContentScanning()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCodedContentScanning
        // @parm [in] Coded Content Scanning value.
        (aafContentScanningType_t  codedContentScanning);


  //****************
  // GetCodedContentScanning()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodedContentScanning
        // @parm [out] Coded content scanning value.
        (aafContentScanningType_t *  pContentScanning);


  //****************
  // SetLowDelay()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLowDelay
        // @parm [in] Sets the LowDelay property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafBoolean_t  lowDelay);


  //****************
  // GetLowDelay()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLowDelay
        // @parm [out] Optional.
        (aafBoolean_t *  pLowDelay);


  //****************
  // SetClosedGOP()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetClosedGOP
        // @parm [in] Sets the ClosedGOP property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafBoolean_t  closedGOP);


  //****************
  // GetClosedGOP()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetClosedGOP
        // @parm [out] Optional.
        (aafBoolean_t *  pClosedGOP);


  //****************
  // SetIdenticalGOP()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIdenticalGOP
        // @parm [in] Sets the IdenticalGOP property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafBoolean_t  identicalGOP);


  //****************
  // GetIdenticalGOP()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIdenticalGOP
        // @parm [out] Optional.
        (aafBoolean_t *  pIdenticalGOP);


  //****************
  // SetMaxGOP()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMaxGOP
        // @parm [in] Optional.
        (aafUInt16  maxGOP);

  //****************
  // GetMaxGOP()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMaxGOP
        // @parm [out] Optional.
        (aafUInt16 *  pMaxGOP);


  //****************
  // SetMaxBPictureCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMaxBPictureCount
        // @parm [in] Optional.
        (aafUInt16  maxBPictureCount);

  //****************
  // GetMaxBPictureCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMaxBPictureCount
        // @parm [out] Optional.
        (aafUInt16 *  pMaxBPictureCount);


  //****************
  // SetBitRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBitRate
        // @parm [in] Optional.
        (aafUInt32  bitRate);

  //****************
  // GetBitRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBitRate
        // @parm [out] Optional.
        (aafUInt32 *  pBitRate);


  //****************
  // SetProfileAndLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProfileAndLevel
        // @parm [in] Optional.
        (aafUInt8  profileAndLevel);

  //****************
  // GetProfileAndLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProfileAndLevel
        // @parm [out] Optional.
        (aafUInt8 *  pProfileAndLevel);

};

#endif // ! __ImplAAFMPEGVideoDescriptor_h__


