#ifndef __ImplAAFDigitalImageDescriptor_h__
#define __ImplAAFDigitalImageDescriptor_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

class ImplAAFDigitalImageDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDigitalImageDescriptor ();

protected:
  virtual ~ImplAAFDigitalImageDescriptor ();

public:
  //****************
  // SetCompression()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCompression
		// @parm [in] Identifies the compression and format of compression information
        (aafUID_t *  pCompression);

  //****************
  // SetStoredView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStoredView
	    (// @parm [in] Number of pixels in vertical dimension of stored view.
         aafUInt32  StoredHeight,

		 // @parm [in] Number of pixels in horizontal dimension of stored view.
		 aafUInt32  StoredWidth);

  //****************
  // SetSampledView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampledView
        (// @parm [in] Number of pixels in vertical dimension of sampled view.
		 aafUInt32  SampledHeight,

		 // @parm [in] Number of pixels in horizontal dimension of sampled view.
		 aafUInt32  SampledWidth,

		 // @parm [in] Number of pixels from top left corner of sampled view. Optional.
         aafInt32  SampledXOffset,

		 // @parm [in] Number of pixels from top left corner of sampled view. Optional.
		 aafInt32  SampledYOffset);

  //****************
  // SetDisplayView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayView
        (// @parm [in] Number of pixels in vertical dimension of display view. Optional.
		 aafUInt32  DisplayHeight,

		 // @parm [in] Number of pixels in horizontal dimension of display view. Optional.
		 aafUInt32  DisplayWidth,

		 // @parm [in] Number of pixels from the top-left corner of the display view. Optional.
         aafInt32  DisplayXOffset,

		 // @parm [in] Number pixels from the top-left corner of the display view. Optional.
		 aafInt32  DisplayYOffset);

  //****************
  // SetFrameLayout()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFrameLayout
		// @parm [in] layout of the frame
        (aafFrameLayout_t  FrameLayout);

  //****************
  // SetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetVideoLineMap
	    (// @parm [in] The number of elements in the array
         aafUInt32  numberElements,

		 // @parm [in, size_is(size)] Array to hold the Video Line Map information
		 aafInt32 *  pVideoLineMap);

  //****************
  // SetImageAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetImageAspectRatio
		// @parm [in] Ratio between horizontal and vertical size
        (aafRational_t  ImageAspectRatio);

  //****************
  // SetAlphaTransparency()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAlphaTransparency
		// @parm [in] Optional.
        (aafAlphaTransparency_t  AlphaTransparency);

  //****************
  // SetGamma()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetGamma
		// @parm [in] Optional
        (aafRational_t  Gamma);

  //****************
  // SetImageAlignmentFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetImageAlignmentFactor
		// @parm [in] Optional.
        (aafInt32  ImageAlignmentFactor);

  //****************
  // GetCompression()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompression
		// @parm [out] Identifies the compression and format of compression information
        (aafUID_t *  pCompression);

  //****************
  // GetStoredView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStoredView
	    (// @parm [out] Number of pixels in vertical dimension of stored view.
         aafUInt32 *  pStoredHeight,

		 // @parm [out] Number of pixels in horizontal dimension of stored view.
		 aafUInt32 *  pStoredWidth);

  //****************
  // GetSampledView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampledView
        (// @parm [out] Number of pixels in vertical dimension of sampled view.
		 aafUInt32 *  pSampledHeight,

		 // @parm [out] Number of pixels in horizontal dimension of sampled view.
		 aafUInt32 *  pSampledWidth,

		 // @parm [out] Number of pixels from top left corner of sampled view. Optional.
         aafInt32 *  pSampledXOffset,

		 // @parm [out] Number of pixels from top left corner of sampled view. Optional.
		 aafInt32 *  pSampledYOffset);

  //****************
  // GetDisplayView()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayView
        (// @parm [out] Number of pixels in vertical dimension of display view. Optional.
		 aafUInt32 *  pDisplayHeight,

		 // @parm [out] Number of pixels in horizontal dimension of display view. Optional.
		 aafUInt32 *  pDisplayWidth,

		 // @parm [out] Number of pixels from the top-left corner of the display view. Optional.
         aafInt32 *  pDisplayXOffset,

		 // @parm [out] Number pixels from the top-left corner of the display view. Optional.
		 aafInt32 *  pDisplayYOffset);

  //****************
  // GetFrameLayout()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFrameLayout
		// @parm [out] layout of the frame
        (aafFrameLayout_t *  pFrameLayout);

  //****************
  // GetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoLineMapSize
	    (// @parm [out] The number of elements in the array
         aafUInt32 *  pNumberElements);

  //****************
  // GetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoLineMap
	    (// @parm [in] The number of elements in the array
         aafUInt32  numberElements,

		 // @parm [out, size_is(numberElements)] Array to hold the Video Line Map information
		 aafInt32 *  pVideoLineMap);

  //****************
  // GetImageAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetImageAspectRatio
		// @parm [out] Ratio between horizontal and vertical size
        (aafRational_t *  pImageAspectRatio);

  //****************
  // GetAlphaTransparency()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAlphaTransparency
		// @parm [out] Optional.
        (aafAlphaTransparency_t *  pAlphaTransparency);

  //****************
  // GetGamma()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGamma
		// @parm [out] Optional.
        (aafRational_t *  pGamma);

  //****************
  // GetImageAlignmentFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetImageAlignmentFactor
		// @parm [out] Optional.
        (aafInt32 *  pImageAlignmentFactor);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFDigitalImageDescriptor)

protected:
	OMFixedSizeProperty<aafUID_t>				_compression;
	OMFixedSizeProperty<aafUInt32>				_storedHeight;
	OMFixedSizeProperty<aafUInt32>				_storedWidth;
	OMFixedSizeProperty<aafUInt32>				_sampledHeight;
	OMFixedSizeProperty<aafUInt32>				_sampledWidth;
	OMFixedSizeProperty<aafInt32>				_sampledXOffset;
	OMFixedSizeProperty<aafInt32>				_sampledYOffset;
	OMFixedSizeProperty<aafUInt32>				_displayHeight;
	OMFixedSizeProperty<aafUInt32>				_displayWidth;
	OMFixedSizeProperty<aafInt32>				_displayXOffset;
	OMFixedSizeProperty<aafInt32>				_displayYOffset;
	OMFixedSizeProperty<aafFrameLayout_t>		_frameLayout;
	OMVariableSizeProperty<aafInt32>			_videoLineMap;
	OMFixedSizeProperty<aafRational_t>			_imageAspectRatio;
	OMFixedSizeProperty<aafRational_t>			_alphaTransparency;
	OMFixedSizeProperty<aafRational_t>			_gamma;
	OMFixedSizeProperty<aafUInt32>				_imageAlignmentFactor;
};

#endif // ! __ImplAAFDigitalImageDescriptor_h__

