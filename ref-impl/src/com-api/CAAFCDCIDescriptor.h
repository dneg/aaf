//@doc
//@class    AAFCDCIDescriptor | Implementation class for AAFCDCIDescriptor
#ifndef __CAAFCDCIDescriptor_h__
#define __CAAFCDCIDescriptor_h__

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





#ifndef __CAAFDigitalImageDescriptor_h__
#include "CAAFDigitalImageDescriptor.h"
#endif


class CAAFCDCIDescriptor
  : public IAAFCDCIDescriptor,
    public CAAFDigitalImageDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFCDCIDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFCDCIDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  // IAAFCDCIDescriptor-supporting object.  This method must be called
  // after allocation, and before any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // SetComponentWidth()
  //
  // Sets the ComponentWidth property.  Specifies the number of bits
  // used to store each component.  Typical values can be 8, 10,
  // 12, 14, or 16, but others are permitted by the reference
  // implementation.  Each component in a sample is packed
  // contiguously; the sample is filled with the number of bits
  // specified by the optional PaddingBits property.  If  the
  // PaddingBits property is omitted, samples are packed
  // contiguously.
  //
  // If this method fails, the ComponentWidth property will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetComponentWidth) (
    // Number of bits. 
    /*[in]*/ aafInt32  ComponentWidth);


  //***********************************************************
  //
  // GetComponentWidth()
  //
  // Gets the ComponentWidth property.  Specifies the number of bits
  // used to store each component.  Typical values can be 8, 10,
  // 12, 14, or 16, but others are permitted by the reference
  // implementation.  Each component in a sample is packed
  // contiguously; the sample is filled with the number of bits
  // specified by the optional PaddingBits property.  If the
  // PaddingBits property is omitted, samples are packed
  // contiguously.
  //
  // Succeeds if all of the following are true:
  // - pComponentWidth is a valid pointer.
  // 
  // If this method fails, *pComponentWidth will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pComponentWidth arg is NULL.
  //
  STDMETHOD (GetComponentWidth) (
    // Address to store the number of bits. 
    /*[out]*/ aafInt32 *  pComponentWidth);


  //***********************************************************
  //
  // SetHorizontalSubsampling()
  //
  // Sets the HorizontalSubsampling property.  Specifies the ratio of
  // luminance sampling to chrominance sampling in the horizontal direction.
  // For 4:2:2 video, the value is 2, which means that there are twice as
  // many luminance values as there are color-difference values.
  // Another typical value is 1; however other values are permitted by
  // the reference implementation.
  //
  // If this method fails, the HorizontalSubsampling property will
  // not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetHorizontalSubsampling) (
    // Integer value. 
    /*[in]*/ aafUInt32  HorizontalSubsampling);


  //***********************************************************
  //
  // GetHorizontalSubsampling()
  //
  // Gets the HorizontalSubsampling property.  Specifies the ratio of
  // luminance sampling to chrominance sampling in the horizontal
  // direction.  For 4:2:2 video, the value is 2, which means that
  // there are twice as many luminance values as there are
  // color-difference values.  Another typical value is 1; however
  // other values are permitted by the reference implementation.
  //
  // Succeeds if all of the following are true:
  // - pHorizontalSubsampling is a valid pointer.
  // 
  // If this method fails, *pHorizontalSubsampling will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pHorizontalSubsampling arg is NULL.
  //
  STDMETHOD (GetHorizontalSubsampling) (
    // Address to store the integer value. 
    /*[out]*/ aafUInt32 *  pHorizontalSubsampling);


  //***********************************************************
  //
  // SetVerticalSubsampling()
  //
  // Sets the VerticalSubsampling property.  Specifies the ratio of
  // luminance sampling to chrominance sampling in the vertical direction.
  // For 4:2:2 video, the value is 2, which means that there are twice as
  // many luminance values as there are color-difference values.
  // Another typical value is 1; however other values are permitted by
  // the reference implementation.
  //
  // If this method fails, the VerticalSubsampling property will
  // not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetVerticalSubsampling) (
    // Integer value. 
    /*[in]*/ aafUInt32  VerticalSubsampling);


  //***********************************************************
  //
  // GetVerticalSubsampling()
  //
  // Gets the VerticalSubsampling property.  Specifies the ratio of
  // luminance sampling to chrominance sampling in the vertical
  // direction.  For 4:2:2 video, the value is 2, which means that
  // there are twice as many luminance values as there are
  // color-difference values.  Another typical value is 1; however
  // other values are permitted by the reference implementation.
  //
  // Succeeds if all of the following are true:
  // - pVerticalSubsampling is a valid pointer.
  // 
  // If this method fails, *pVerticalSubsampling will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pVerticalSubsampling arg is NULL.
  //
  STDMETHOD (GetVerticalSubsampling) (
    // Address to store the integer value. 
    /*[out]*/ aafUInt32 *  pVerticalSubsampling);


  //***********************************************************
  //
  // SetColorSiting()
  //
  // Sets the ColorSiting property.  Specifies how to compute subsampled
  // chrominance values.
  //
  // Valid values are:
  //
  //     kCoSiting  - To calculate subsampled pixels, take the preceding's pixels
  //                  color value, discard the other color values and cosite the
  //                  color with the first luminance value.
  //
  //     kAveraging - To calculate subsampled pixels, take the average of the two
  //                  adjacent pixels' color values and site the color in the center
  //                  of the luminance pixels.
  //
  //     kThreeTap  - To calculate subsampled pixels, take 25 percent of the the previous
  //                  pixel's color value, 50 percent of the first value and 25 percent
  //                  of the second value.  For the first value in a row, use 75 percent
  //                  of that value since there is no previous value.  The kThreeTap value
  //                  is only meaningful when the HorizontalSubsampling propert has a value
  //                  of 2.
  //
  // This property is optional.  The default value is kCoSiting.
  //
  // Succeeds if all of the following are true:
  // - ColorSiting is valid
  //
  // If this method fails, the ColorSiting property will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetColorSiting) (
    // Color siting value. 
    /*[in]*/ aafColorSiting_t  ColorSiting);


  //***********************************************************
  //
  // GetColorSiting()
  //
  // Gets the ColorSiting property.  Specifies how to compute
  // subsampled chrominance values.
  //
  // Valid values are:
  //
  //     kCoSiting  - To calculate subsampled pixels, take the
  //                  preceding's pixels color value, discard the
  //                  other color values and cosite the color with the
  //                  first luminance value.
  //
  //     kAveraging - To calculate subsampled pixels, take the
  //                  average of the two adjacent pixels' color values
  //                  and site the color in the center of the
  //                  luminance pixels.
  //
  //     kThreeTap  - To calculate subsampled pixels, take 25 percent
  //                  of the the previous pixel's color value, 50
  //                  percent of the first value and 25 percent of the
  //                  second value.  For the first value in a row,
  //                  use 75 percent of that value since there is no
  //                  previous value.  The kThreeTap value is only
  //                  meaningful when the HorizontalSubsampling
  //                  propert has a value of 2.
  //
  // Succeeds if all of the following are true:
  // - pColorSiting is a valid pointer.
  //
  // If this method fails, *pColorSiting will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pColorSiting arg is NULL.
  //
  STDMETHOD (GetColorSiting) (
    // Address to store the color siting value. 
    /*[out]*/ aafColorSiting_t *  pColorSiting);


  //***********************************************************
  //
  // SetBlackReferenceLevel()
  //
  // Sets the BlackReferenceLevel property.  Specifies the digital luminance
  // component component value associated with black.
  //
  // For CCIR-601/2, the value is 16 for 8-bit video and 64 for
  // 10-bit video.  For YUV, the value is 0.  These are typical
  // values; other values will not be disallowed by the reference
  // implementation.
  //
  // The same value is used in CDCI and RGBA when standard colorspace
  // conversion is used.
  //
  // This property is optional.  The default value is 0.
  //
  // If this method fails, the BlackReferenceLevel property will not
  // be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetBlackReferenceLevel) (
    // Integer value. 
    /*[in]*/ aafUInt32  BlackReferenceLevel);


  //***********************************************************
  //
  // GetBlackReferenceLevel()
  //
  // Gets the BlackReferenceLevel property.  Specifies the digital
  // luminance component value associated with black.
  //
  // For CCIR-601/2, the value is 16 for 8-bit video and 64 for
  // 10-bit video.  For YUV, the value is 0.  These are typical
  // values; other values will not be disallowed by the reference
  // implementation.
  //
  // The same value is used in CDCI and RGBA when standard colorspace
  // conversion is used.
  //
  // Succeeds if all of the following are true:
  // - pBlackReferenceLevel is a valid pointer.
  // 
  // If this method fails, *pBlackReferenceLevel will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pBlackReferenceLevel arg is NULL.
  //
  STDMETHOD (GetBlackReferenceLevel) (
    // Address to store the integer value. 
    /*[out]*/ aafUInt32 *  pBlackReferenceLevel);


  //***********************************************************
  //
  // SetWhiteReferenceLevel()
  //
  // Sets the WhiteReferenceLevel property.  Specifies the digital
  // luminance component component value associated with white.
  //
  // For CCIR-601/2, the value is 235 for 8-bit video and 940 for
  // 10-bit video.  For YUV, the value is 255 for 8-bit video and
  // 1023 for 10-bit video.  These are typical values; other values
  // will not be disallowed by the reference implementation.
  //
  // This property is optional.  The default value is the maximum
  // unsigned interger value for component size.
  //
  // If this method fails, the WhiteReferenceLevel property will not
  // be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetWhiteReferenceLevel) (
    // Integer value. 
    /*[in]*/ aafUInt32  WhiteReferenceLevel);


  //***********************************************************
  //
  // GetWhiteReferenceLevel()
  //
  // Gets the WhiteReferenceLevel property.  Specifies the digital
  // luminance component component value associated with white.
  //
  // For CCIR-601/2, the value is 235 for 8-bit video and 940 for
  // 10-bit video.  For YUV, the value is 255 for 8-bit video and
  // 1023 for 10-bit video.  These are typical values; other values
  // will not be disallowed by the reference implementation.
  //
  // Succeeds if all of the following are true:
  // - pWhiteReferenceLevel is a valid pointer.
  // 
  // If this method fails, *pWhiteReferenceLevel will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pWhiteReferenceLevel arg is NULL.
  //
  STDMETHOD (GetWhiteReferenceLevel) (
    // Address to store the integer value. 
    /*[out]*/ aafUInt32 *  pWhiteReferenceLevel);


  //***********************************************************
  //
  // SetColorRange()
  //
  // Sets the ColorRange property.  Specifies the range of allowable
  // digital chrominance component values.  Chrominance values are
  // unsigned and the range is centered on 128 for 8-bit video and 512
  // for 10-bit video.  This value is used for both chrominance
  // components.
  //
  // For CCIR-601/2, the range is 225 for 8-bit video and 897 for
  // 10-bit video.  For YUV, the range is 255 for 8-bit video and
  // 1023 for 10-bit video.  These are typical values; other values will
  // not be disallowed by the reference implementation.
  //
  // This property is optional.  The default value is the maximum
  // unsigned integer value for component size.
  //
  // Succeeds if all of the following are true:
  // 
  // If this method fails, the ColorRange property will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetColorRange) (
    // Integer value. 
    /*[in]*/ aafUInt32  ColorRange);


  //***********************************************************
  //
  // GetColorRange()
  //
  // Gets the ColorRange property.  Specifies the range of allowable
  // digital chrominance component values.  Chrominance values are
  // unsigned and the range is centered on 128 for 8-bit video and 512
  // for 10-bit video.  This value is used for both chrominance
  // components.
  //
  // For CCIR-601/2, the range is 225 for 8-bit video and 897 for
  // 10-bit video.  For YUV, the range is 255 for 8-bit video and
  // 1023 for 10-bit video.  These are typical values; other values will
  // not be disallowed by the reference implementation.
  //
  // Succeeds if all of the following are true:
  // - pColorRange is a valid pointer.
  // 
  // If this method fails, *pColorRange will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pColorRange arg is NULL.
  //
  STDMETHOD (GetColorRange) (
    // Address to store the integer value. 
    /*[out]*/ aafUInt32 *  pColorRange);


  //***********************************************************
  //
  // SetPaddingBits()
  //
  // Sets the PaddingBits property.  Specifies the number of bits
  // padded to each pixel.
  //
  // This property is optional.  The default value is 0.
  //
  // If this method fails, the PaddingBits property will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPaddingBits) (
    // Number of bits. 
    /*[in]*/ aafInt16  PaddingBits);


  //***********************************************************
  //
  // GetPaddingBits()
  //
  // Gets the PaddingBits property.  Specifies the number of bits
  // padded to each pixel.
  //
  //
  // Succeeds if all of the following are true:
  // - pPaddingBits is a valid pointer.
  // 
  // If this method fails, pPaddingBits will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pPaddingBits arg is NULL.
  //
  STDMETHOD (GetPaddingBits) (
    // Address to store the number of bits. 
    /*[out]*/ aafInt16 *  pPaddingBits);

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

#endif // ! __CAAFCDCIDescriptor_h__


