//@doc
//@class    AAFDigitalImageDescriptor | Implementation class for AAFDigitalImageDescriptor
#ifndef __CAAFDigitalImageDescriptor_h__
#define __CAAFDigitalImageDescriptor_h__

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
// The Original Code of this file is Copyright 1998-2009, licensor of the
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






#ifndef __CAAFFileDescriptor_h__
#include "CAAFFileDescriptor.h"
#endif


class CAAFDigitalImageDescriptor
  : public IAAFDigitalImageDescriptor,
    public IAAFDigitalImageDescriptor2,
    public CAAFFileDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDigitalImageDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDigitalImageDescriptor ();

public:


  //***********************************************************
  //
  // SetCompression()
  //
  // Sets the kind of compression and format of compression
  /// information of the video essence data.  This property is
  /// optional.  If there is no compression, the property is omitted.
  /// 
  /// If this method fails the Data Definition property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetCompression) (
    // Optional. 
    /*[in]*/ aafUID_constref  compression);


  //***********************************************************
  //
  // GetCompression()
  //
  // Gets the kind of compression and format of compression
  /// information of the video essence data.  This property is
  /// optional.
  ///
  /// Succeeds if all of the following are true:
  /// - the pCompression pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pCompression.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pComporession arg is NULL.
  //
  STDMETHOD (GetCompression) (
    // Optional. 
    /*[out]*/ aafUID_t *  pCompression);


  //***********************************************************
  //
  // SetStoredView()
  //
  // Sets the dimension of the stored view.  Typically this includes
  /// leading blank video lines, any VITC lines, as well as the active
  /// picture area.
  /// 
  /// If this method fails the Stored Height and Stored Width
  /// properties will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetStoredView) (
    // Number of pixels in vertical dimension of stored view.
    /*[in]*/ aafUInt32  StoredHeight,

    // Number of pixels in horizontal dimension of stored view.
    /*[in]*/ aafUInt32  StoredWidth);


  //***********************************************************
  //
  // GetStoredView()
  //
  // Gets the dimension of the stored view.  Typically this includes
  /// leading blank video lines, any VITC lines, as well as the active
  /// picture area.
  /// 
  /// Succeeds if all of the following are true:
  /// - pStoredHieght and pStoredWidth are valid pointers.
  /// 
  /// If this method fails, the *pStoredHieght and *pStoredWidth will
  /// not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - either pStoredHeight or pStoredWidth is NULL.
  //
  STDMETHOD (GetStoredView) (
    // Number of pixels in vertical dimension of stored view.
    /*[out]*/ aafUInt32 *  pStoredHeight,

    // Number of pixels in horizontal dimension of stored view.
    /*[out]*/ aafUInt32 *  pStoredWidth);


  //***********************************************************
  //
  // SetSampledView()
  //
  // Sets the dimensions of sampled view.  Typically this includes
  /// any VITC lines as well as the active picture area, but excludes
  /// leading blank video lines.  The offset is specified relative to
  /// the rectangle specified by Set/GetStoredView().
  ///
  /// The following properties are optional:
  ///
  ///     SampledXOffset - The default value is 0.  Use a value of 0 to
  ///                      select the default.
  ///     SampledYOffset - The default value is 0.  Use a value of 0 to
  ///                      select the default.
  ///
  /// Succeeds if all of the following are true:
  /// - The given dimensions exist within the StoredView.
  /// 
  /// If this method fails, the SampledXOffset and SampledYOffset
  /// properties will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - The area specified by SampledHeight and SampledXOffset is
  ///     outside the StoredView, or the area specified by SampledWidth
  ///     and SampledYOffset is outside the StoredView.
  //
  STDMETHOD (SetSampledView) (
    // Number of pixels in vertical dimension of sampled view.
    /*[in]*/ aafUInt32  SampledHeight,

    // Number of pixels in horizontal dimension of sampled view.
    /*[in]*/ aafUInt32  SampledWidth,

    // Number of pixels from top left corner of sampled view. Optional.
    /*[in]*/ aafInt32  SampledXOffset,

    // Number of pixels from top left corner of sampled view. Optional.
    /*[in]*/ aafInt32  SampledYOffset);


  //***********************************************************
  //
  // GetSampledView()
  //
  // Gets the dimensions of sampled view.  Typically this includes
  /// any VITC lines as well as the active picture area, but excludes
  /// leading blank video lines.  The offset is specified relative to
  /// the rectangle specified by Set/GetStoredView().
  ///
  /// Succeeds if all of the following are true:
  /// - pSampledHeight, pSampledWidth, pSampledXOffset and
  ///   pSampledYOffset are valid pointers
  /// 
  /// If any of the input parameters are NULL, the property will not
  /// be returned.
  ///
  /// If this method fails, *pSampledHeight, *pSampledWidth,
  /// *pSampledXOffset, and *pSampledYOffset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - any of pSampledHeight, pSampledWidth, pSampledXOffset, or
  ///     pSampledYOffset are NULL.
  //
  STDMETHOD (GetSampledView) (
    // Number of pixels in vertical dimension of sampled view.
    /*[out]*/ aafUInt32 *  pSampledHeight,

    // Number of pixels in horizontal dimension of sampled view.
    /*[out]*/ aafUInt32 *  pSampledWidth,

    // Number of pixels from top left corner of sampled view. Optional.
    /*[out]*/ aafInt32 *  pSampledXOffset,

    // Number of pixels from top left corner of sampled view. Optional.
    /*[out]*/ aafInt32 *  pSampledYOffset);


  //***********************************************************
  //
  // SetDisplayView()
  //
  // Sets the dimension of display view.  Typically this includes
  /// the active picture area, but excludes leading blank video lines
  /// and any VITC lines.  The offset is specified relative to the
  /// rectangle specified by Set/GetStoredView().
  /// 
  /// The following properties are optional:
  ///
  ///     DisplayHeight  - The default value is the storedHeight. Use
  ///                      storedHeight to select the default.
  ///     DisplayWidth   - The default value is the storedWidth. Use
  ///                      storedWidth to select the default.
  ///     DisplayXOffset - The default value is 0.  Use a value of 0 to
  ///                      select the default.
  ///     DisplayYOffset - The default value is 0.  Use a value of 0 to
  ///                      select the default.
  ///
  /// Note that The specified display rectangle may exist outside the
  /// SampledView or even the StoredView.
  ///
  /// If this method fails the DisplayHeight, DisplayWidth, DisplayXOffset and
  /// DisplayYOffset properties will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - The area specified by DisplayHeight and DisplayXOffset is
  ///     outside the StoredView, or the area specified by DisplayWidth
  ///     and DisplayYOffset is outside the StoredView.
  //
  STDMETHOD (SetDisplayView) (
    // Number of pixels in vertical dimension of display view. Optional.
    /*[in]*/ aafUInt32  DisplayHeight,

    // Number of pixels in horizontal dimension of display view. Optional.
    /*[in]*/ aafUInt32  DisplayWidth,

    // Number of pixels from the top-left corner of the display view. Optional.
    /*[in]*/ aafInt32  DisplayXOffset,

    // Number pixels from the top-left corner of the display view. Optional.
    /*[in]*/ aafInt32  DisplayYOffset);


  //***********************************************************
  //
  // GetDisplayView()
  //
  // Gets the dimension of display view.  Typically this includes
  /// the active picture area, but excludes leading blank video lines
  /// and any VITC lines.  The offset is specified relative to the
  /// rectangle specified by Set/GetStoredView().
  /// 
  /// Note that The specified display rectangle may exist outside the
  /// SampledView or even the StoredView.
  ///
  /// Succeeds if all of the following are true:
  /// - pDisplayHeight, pDisplayWidth, pDisplayXOffset and
  ///   pDisplayYOffset are valid pointers.
  /// 
  /// If this method fails, *pDisplayHeight, *pDisplayWidth,
  /// *pDisplayXOffset, and *pDisplayYOffset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - any of pDisplayHeight, pDisplayWidth, pDisplayXOffset, or
  ///     pDisplayYOffset are NULL.
  //
  STDMETHOD (GetDisplayView) (
    // Number of pixels in vertical dimension of display view. Optional.
    /*[out]*/ aafUInt32 *  pDisplayHeight,

    // Number of pixels in horizontal dimension of display view. Optional.
    /*[out]*/ aafUInt32 *  pDisplayWidth,

    // Number of pixels from the top-left corner of the display view. Optional.
    /*[out]*/ aafInt32 *  pDisplayXOffset,

    // Number pixels from the top-left corner of the display view. Optional.
    /*[out]*/ aafInt32 *  pDisplayYOffset);


  //***********************************************************
  //
  // SetFrameLayout()
  //
  // Sets the frame layout.  The frame layout describes whether all
  /// data for a complete sample is in one frame or is split into more
  /// than/ one field.
  ///
  /// Values are:
  ///
  ///    kNoLayout       - Default; not a valid value.
  ///    kFullFrame      - Each frame contains a full sample in
  ///                      progressive scan lines.
  ///    kSeparateFields - Each sample consists of two fields, which
  ///                      when interlaced produce a full sample.
  ///    kOneField       - Each sample consists of two interlaced
  ///                      fields, but only one field is stored in the
  ///                      data stream.
  ///    kMixedFields    - Similar to FullFrame, except the two fields
  ///                      may have been sampled at different times.
  /// 
  /// Succeeds if all of the following are true:
  /// - frameLayout is a valid value
  /// 
  /// If this method fails, the Frame Layout property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - FrameLayout is not a valid value.
  //
  STDMETHOD (SetFrameLayout) (
    // layout of the frame 
    /*[in]*/ aafFrameLayout_t  FrameLayout);


  //***********************************************************
  //
  // GetFrameLayout()
  //
  // Gets the frame layout.  The frame layout describes whether all
  /// data for a complete sample is in one frame or is split into more
  /// than one field.
  /// 
  /// Values are:
  ///
  ///    kNoLayout       - Default; not a valid value.
  ///    kFullFrame      - Each frame contains a full sample in
  ///                      progressive scan lines.
  ///    kSeparateFields - Each sample consists of two fields, which
  ///                      when interlaced produce a full sample.
  ///    kOneField       - Each sample consists of two interlaced
  ///                      fields, but only one field is stored in the
  ///                      data stream.
  ///    kMixedFields    - Similar to FullFrame, except the two fields
  ///                      may have been sampled at different times.
  /// 
  /// Succeeds if all of the following are true:
  /// - pFrameLayout is a valid pointer
  /// 
  /// If this method fails, *pFrameLayout will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFrameLayout is NULL.
  //
  STDMETHOD (GetFrameLayout) (
    // layout of the frame 
    /*[out]*/ aafFrameLayout_t *  pFrameLayout);


  //***********************************************************
  //
  // SetVideoLineMap()
  //
  // Sets the VideoLineMap property.  The video line map specifies the
  /// scan line in the analog source that corresponds to the beginning
  /// of each digitized field.  For single-field video, there is 1
  /// value in the array.  For interleaved video, there are 2 values
  /// in the array.
  /// 
  /// Succeeds if all of the following are true:
  /// - pVideoLineMap is a valid pointer
  /// 
  /// If this method fails, the Video Line Map property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pVideoLineMap is NULL.
  //
  STDMETHOD (SetVideoLineMap) (
    // The number of elements in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold the Video Line Map information
    /*[in, size_is(numberElements)]*/ aafInt32 *  pVideoLineMap);


  //***********************************************************
  //
  // GetVideoLineMap()
  //
  // Gets the VideoLineMap property.  The video line map specifies the
  /// scan line in the analog source that corresponds to the beginning of each
  /// digitized field.  For single-field video, there is 1 value in the array.
  /// For interleaved video, there are 2 values in the array.
  ///
  /// The values are written to the array specified by pVideoLineMap,
  /// which is of size numberElements.  The required size may be found
  /// by calling GetVideoLineMapSize().
  /// 
  /// Succeeds if all of the following are true:
  /// - pVideoLineMap is a valid pointer.
  /// - numberElements indicates the array is large enough to hold the
  ///   data.
  /// 
  /// If this method fails, videoLineMap will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumberElements is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - numberElements indicates that the array is too small to hold
  ///     the data.
  //
  STDMETHOD (GetVideoLineMap) (
    // The number of elements in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold the Video Line Map information
    /*[out, size_is(numberElements)]*/ aafInt32 *  pVideoLineMap);


  //***********************************************************
  //
  // GetVideoLineMapSize()
  //
  // Get the number of elements in the VideoLineMap property array.
  /// 
  /// Succeeds if all of the following are true:
  /// - pNumberElements is a valid pointer
  /// 
  /// If this method fails, *pNumberElements will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumberElements is NULL.
  //
  STDMETHOD (GetVideoLineMapSize) (
    // The number of elements in the array 
    /*[out]*/ aafUInt32 *  pNumberElements);


  //***********************************************************
  //
  // SetImageAspectRatio()
  //
  // Sets the Image Aspect Ratio property.  This ratio describes the
  /// ratio between the horizontal size and the vertical size in the
  /// intended final image.
  ///
  /// Succeeds if all of the following are true:
  /// - 
  /// 
  /// If this method fails, the Image Access Ratio property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetImageAspectRatio) (
    // Ratio between horizontal and vertical size 
    /*[in]*/ aafRational_t  ImageAspectRatio);


  //***********************************************************
  //
  // GetImageAspectRatio()
  //
  // Gets the Image Aspect Ratio property.  This ratio describes the
  /// ratio between the horizontal size and the vertical size in the
  /// intended final image.
  ///
  /// Succeeds if all of the following are true:
  /// - pImageAspectRatio is a valid pointer
  /// 
  /// If this method fails, *pImageAspectRatio will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pImageAspectRatio is NULL.
  //
  STDMETHOD (GetImageAspectRatio) (
    // Ratio between horizontal and vertical size 
    /*[out]*/ aafRational_t *  pImageAspectRatio);


  //***********************************************************
  //
  // SetAlphaTransparency()
  //
  // Sets the AlphaTransparency property.  This property is optional.
  ///
  /// Valid values:
  ///	kMaxValueTransparent - means the maximum Alpha value is
  ///                           transparent
  ///    kMinValueTransparent - means the minimum Alpha value is
  ///                           transparent
  ///
  /// Succeeds if all of the following are true:
  /// - AlphaTransparency is a valid value.
  /// 
  /// If this method fails, the AlphaTransparency property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - AlphaTransparency is not a valid value.
  //
  STDMETHOD (SetAlphaTransparency) (
    // Alpha Transparency value. 
    /*[in]*/ aafAlphaTransparency_t  AlphaTransparency);


  //***********************************************************
  //
  // GetAlphaTransparency()
  //
  // Gets the AlphaTransparency property.  This property is optional.
  ///
  /// Valid values:
  ///	kMaxValueTransparent - means the maximum Alpha value is transparent
  ///    kMinValueTransparent - means the minimum Alpha value is transparent
  ///
  /// Succeeds if all of the following are true:
  /// - pAlphaTransparency is a valid pointer
  /// 
  /// If this method fails, pAlphaTransparency not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pAlphaTransparency is NULL.
  //
  STDMETHOD (GetAlphaTransparency) (
    // Alpha Transparency value. 
    /*[out]*/ aafAlphaTransparency_t *  pAlphaTransparency);


  //***********************************************************
  //
  // SetGamma()
  //
  // Sets the TransferCharacteristic property.
  ///
  /// Succeeds if all of the following are true:
  /// - 
  /// 
  /// If this method fails, the Gamma property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetGamma) (
    // Optional 
    /*[in]*/ aafUID_t  Gamma);


  //***********************************************************
  //
  // GetGamma()
  //
  // Gets the TransferCharacteristic property.
  ///
  /// Succeeds if all of the following are true:
  /// - pGamma is a valid pointer
  /// 
  /// If this method fails, pGamma will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pGamma is NULL.
  //
  STDMETHOD (GetGamma) (
    // Optional. 
    /*[out]*/ aafUID_t *  pGamma);


  //***********************************************************
  //
  // SetImageAlignmentFactor()
  //
  // Sets the ImageAlignmentFactor property.  Specifies the alignment
  /// when storing the digital essence.  For example, a value of 16
  /// means that the image is stored on 16-byte boundaries.  The
  /// starting point for a field will always be a multiple of 16 bytes.
  /// If the field does not end on a 16-byte boundary, it is padded
  /// out to the next 16-byte boundary.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the ImageAlignmentFactor property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetImageAlignmentFactor) (
    // Optional. 
    /*[in]*/ aafUInt32  ImageAlignmentFactor);


  //***********************************************************
  //
  // GetImageAlignmentFactor()
  //
  // Gets the ImageAlignmentFactor property.  Specifies the alignment when
  /// storing the digital essence.  For example, a value of 16 means that the image
  /// is stored on 16-byte boundaries.  The starting point for a field will always
  /// be a multiple of 16 bytes.  If the field does not end on a 16-byte boundary,
  /// it is padded out to the next 16-byte boundary.
  ///
  ///
  /// Succeeds if all of the following are true:
  /// - pImageAlignmentFactor is a valid pointer
  /// 
  /// If this method fails, pImageAlignmentFactor will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pImageAlignmentFactor is NULL.
  //
  STDMETHOD (GetImageAlignmentFactor) (
    // Optional. 
    /*[out]*/ aafUInt32 *  pImageAlignmentFactor);

  //***********************************************************
  // METHOD NAME: SetFieldDominance()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetFieldDominance |
  // Sets the FieldDominance property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the FieldDominance property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - FieldDominance is not a valid value.
  // @end
  // 
  STDMETHOD (SetFieldDominance)
   (
    // @parm [in] aafFieldNumber_t | fieldDominance | Optional.
    aafFieldNumber_t  fieldDominance
  );

  //***********************************************************
  // METHOD NAME: GetFieldDominance()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetFieldDominance |
  // Gets the FieldDominance property.
  ///
  /// Succeeds if all of the following are true:
  /// - pFieldDominance is a valid pointer
  /// 
  /// If this method fails, pFieldDominance will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFieldDominance is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetFieldDominance)
   (
    // @parm [out] aafFieldNumber_t * | pFieldDominance | Optional.
    aafFieldNumber_t *  pFieldDominance
  );

  //***********************************************************
  // METHOD NAME: SetFieldStartOffset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetFieldStartOffset |
  // Sets the FieldStartOffset property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the FieldStartOffset property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetFieldStartOffset)
   (
    // @parm [in] aafUInt32 | fieldStartOffset | Optional.
    aafUInt32  fieldStartOffset
  );

  //***********************************************************
  // METHOD NAME: GetFieldStartOffset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetFieldStartOffset |
  // Gets the FieldStartOffset property.
  ///
  /// Succeeds if all of the following are true:
  /// - pFieldStartOffset is a valid pointer
  /// 
  /// If this method fails, pFieldStartOffset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFieldStartOffset is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetFieldStartOffset)
   (
    // @parm [out] aafUInt32 * | pFieldStartOffset | Optional.
    aafUInt32 *  pFieldStartOffset
  );

  //***********************************************************
  // METHOD NAME: SetFieldEndOffset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetFieldEndOffset |
  // Sets the FieldEndOffset property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the FieldEndOffset property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetFieldEndOffset)
   (
    // @parm [in] aafUInt32 | fieldEndOffset | Optional.
    aafUInt32  fieldEndOffset
  );

  //***********************************************************
  // METHOD NAME: GetFieldEndOffset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetFieldEndOffset |
  // Gets the FieldEndOffset property.
  ///
  /// Succeeds if all of the following are true:
  /// - pFieldEndOffset is a valid pointer
  /// 
  /// If this method fails, pFieldEndOffset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFieldEndOffset is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetFieldEndOffset)
   (
    // @parm [out] aafUInt32 * | pFieldEndOffset | Optional.
    aafUInt32 *  pFieldEndOffset
  );

  //***********************************************************
  // METHOD NAME: SetTransferCharacteristic()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetTransferCharacteristic |
  // Sets the TransferCharacteristic property.
  ///
  /// Succeeds if all of the following are true:
  /// - 
  /// 
  /// If this method fails, the TransferCharacteristic property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetTransferCharacteristic)
   (
    // @parm [in] aafUID_constref | transferCharacteristic | Optional
    aafUID_constref  transferCharacteristic
  );

  //***********************************************************
  // METHOD NAME: GetTransferCharacteristic()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetTransferCharacteristic |
  // Gets the TransferCharacteristic property.
  ///
  /// Succeeds if all of the following are true:
  /// - pTransferCharacteristic is a valid pointer
  /// 
  /// If this method fails, pTransferCharacteristic will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTransferCharacteristic is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetTransferCharacteristic)
   (
    // @parm [out] aafUID_t * | pTransferCharacteristic | Optional.
    aafUID_t *  pTransferCharacteristic
  );

  //***********************************************************
  // METHOD NAME: SetCodingEquations()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetCodingEquations |
  // Sets the CodingEquations property.
  ///
  /// Succeeds if all of the following are true:
  /// - 
  /// 
  /// If this method fails, the CodingEquations property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetCodingEquations)
   (
    // @parm [in] aafUID_constref | codingEquations | Optional
    aafUID_constref  codingEquations
  );

  //***********************************************************
  // METHOD NAME: GetCodingEquations()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetCodingEquations |
  // Gets the CodingEquations property.
  ///
  /// Succeeds if all of the following are true:
  /// - pCodingEquations is a valid pointer
  /// 
  /// If this method fails, pCodingEquations will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCodingEquations is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetCodingEquations)
   (
    // @parm [out] aafUID_t * | pCodingEquations | Optional.
    aafUID_t *  pCodingEquations
  );

  //***********************************************************
  // METHOD NAME: SetColorPrimaries()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetColorPrimaries |
  // Sets the ColorPrimaries property.
  ///
  /// Succeeds if all of the following are true:
  /// - 
  /// 
  /// If this method fails, the ColorPrimaries property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetColorPrimaries)
   (
    // @parm [in] aafUID_constref | colorPrimaries | Optional
    aafUID_constref  colorPrimaries
  );

  //***********************************************************
  // METHOD NAME: GetColorPrimaries()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetColorPrimaries |
  // Gets the ColorPrimaries property.
  ///
  /// Succeeds if all of the following are true:
  /// - pColorPrimaries is a valid pointer
  /// 
  /// If this method fails, pColorPrimaries will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pColorPrimaries is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetColorPrimaries)
   (
    // @parm [out] aafUID_t * | pColorPrimaries | Optional.
    aafUID_t *  pColorPrimaries
  );

  //***********************************************************
  // METHOD NAME: SetDisplayF2Offset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetDisplayF2Offset |
  // Sets the DisplayF2Offset property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the DisplayF2Offset property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - displayF2Offset is not a valid value.
  // @end
  // 
  STDMETHOD (SetDisplayF2Offset)
   (
    // @parm [in] aafInt32 | displayF2Offset | Optional.
    aafInt32  displayF2Offset
  );

  //***********************************************************
  // METHOD NAME: GetDisplayF2Offset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetDisplayF2Offset |
  // Gets the DisplayF2Offset property.
  ///
  /// Succeeds if all of the following are true:
  /// - pDisplayF2Offset is a valid pointer
  /// 
  /// If this method fails, pDisplayF2Offset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDisplayF2Offset is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetDisplayF2Offset)
   (
    // @parm [out] aafInt32 * | pDisplayF2Offset | Optional.
    aafInt32 *  pDisplayF2Offset
  );

  //***********************************************************
  // METHOD NAME: SetStoredF2Offset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetStoredF2Offset |
  // Sets the StoredF2Offset property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the StoredF2Offset property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - storedF2Offset is not a valid value.
  // @end
  // 
  STDMETHOD (SetStoredF2Offset)
   (
    // @parm [in] aafInt32 | storedF2Offset | Optional.
    aafInt32  storedF2Offset
  );

  //***********************************************************
  // METHOD NAME: GetStoredF2Offset()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetStoredF2Offset |
  // Gets the StoredF2Offset property.
  ///
  /// Succeeds if all of the following are true:
  /// - pStoredF2Offset is a valid pointer
  /// 
  /// If this method fails, pStoredF2Offset will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pStoredF2Offset is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetStoredF2Offset)
   (
    // @parm [out] aafInt32 * | pStoredF2Offset | Optional.
    aafInt32 *  pStoredF2Offset
  );

  //***********************************************************
  // METHOD NAME: SetActiveFormatDescriptor()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetActiveFormatDescriptor |
  // Sets the ActiveFormatDescriptor property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the ActiveFormatDescriptor property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetActiveFormatDescriptor)
   (
    // @parm [in] aafUInt8 | activeFormatDescriptor | Optional.
    aafUInt8  activeFormatDescriptor
  );

  //***********************************************************
  // METHOD NAME: GetActiveFormatDescriptor()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetActiveFormatDescriptor |
  // Gets the ActiveFormatDescriptor property.
  ///
  /// Succeeds if all of the following are true:
  /// - pActiveFormatDescriptor is a valid pointer
  /// 
  /// If this method fails, pActiveFormatDescriptor will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pActiveFormatDescriptor is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetActiveFormatDescriptor)
   (
    // @parm [out] aafUInt8 * | pActiveFormatDescriptor | Optional.
    aafUInt8 *  pActiveFormatDescriptor
  );

  //***********************************************************
  // METHOD NAME: SetSignalStandard()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | SetSignalStandard |
  // Sets the SignalStandard property.  This property is optional.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the SignalStandard property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_AAFRESULT_INVALID_ENUM_VALUE
  ///   - signalStandard is not a valid value.
  // @end
  // 
  STDMETHOD (SetSignalStandard)
   (
    // @parm [in] aafSignalStandard_t | signalStandard | Signal standard value.
    aafSignalStandard_t  signalStandard
  );

  //***********************************************************
  // METHOD NAME: GetSignalStandard()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFDigitalImageDescriptor2 | GetSignalStandard |
  // Gets the SignalStandard property.  This property is optional.
  ///
  /// Succeeds if all of the following are true:
  /// - pSignalStandard is a valid pointer
  /// 
  /// If this method fails, pSignalStandard not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSignalStandard is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetSignalStandard)
   (
    // @parm [out] aafSignalStandard_t * | pSignalStandard | Signal standard value.
    aafSignalStandard_t *  pSignalStandard
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

#endif // ! __CAAFDigitalImageDescriptor_h__

