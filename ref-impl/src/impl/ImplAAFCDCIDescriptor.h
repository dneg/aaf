#ifndef __ImplAAFCDCIDescriptor_h__
#define __ImplAAFCDCIDescriptor_h__

//=---------------------------------------------------------------------=
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

#include "ImplAAFDigitalImageDescriptor.h"


class ImplAAFCDCIDescriptor : public ImplAAFDigitalImageDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCDCIDescriptor ();

protected:
  virtual ~ImplAAFCDCIDescriptor ();

public:

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();

  //****************
  // SetComponentWidth()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetComponentWidth
		// @parm [in] Number of bits.
        (aafInt32  ComponentWidth);

  //****************
  // SetHorizontalSubsampling()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetHorizontalSubsampling
		// @parm [in] Integer value.
        (aafUInt32  HorizontalSubsampling);

  //****************
  // SetVerticalSubsampling()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetVerticalSubsampling
		// @parm [in] Integer value.
        (aafUInt32  VerticalSubsampling);

  //****************
  // SetColorSiting()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetColorSiting
		// @parm [in] Color siting value.
        (aafColorSiting_t  ColorSiting);

  //****************
  // SetBlackReferenceLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBlackReferenceLevel
		// @parm [in] Integer value.
        (aafUInt32  BlackReferenceLevel);

  //****************
  // SetWhiteReferenceLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetWhiteReferenceLevel
		// @parm [in] Integer value.
        (aafUInt32  WhiteReferenceLevel);

  //****************
  // SetColorRange()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetColorRange
		// @parm [in] Integer value.
        (aafUInt32  ColorRange);

  //****************
  // SetPaddingBits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPaddingBits
		// @parm [in] Number of bits.
        (aafInt16  PaddingBits);

  //****************
  // GetComponentWidth()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetComponentWidth
		// @parm [out] Address to store the number of bits.
        (aafInt32 *  pComponentWidth);

  //****************
  // GetHorizontalSubsampling()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetHorizontalSubsampling
		// @parm [out] Address to store the integer value.
        (aafUInt32 *  pHorizontalSubsampling);

  //****************
  // GetVerticalSubsampling()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVerticalSubsampling
		// @parm [out] Address to store the integer value.
        (aafUInt32 *  pVerticalSubsampling);

  //****************
  // GetColorSiting()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetColorSiting
		// @parm [out] Address to store the color siting value.
        (aafColorSiting_t *  pColorSiting);

  //****************
  // GetBlackReferenceLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBlackReferenceLevel
		// @parm [out] Address to store the integer value.
        (aafUInt32 *  pBlackReferenceLevel);

  //****************
  // GetWhiteReferenceLevel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetWhiteReferenceLevel
		// @parm [out] Address to store the integer value.
        (aafUInt32 *  pWhiteReferenceLevel);

  //****************
  // GetColorRange()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetColorRange
		// @parm [out] Address to store the integer value.
        (aafUInt32 *  pColorRange);

  //****************
  // GetPaddingBits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPaddingBits
		// @parm [out] Address to store the number of bits.
        (aafInt16 *  pPaddingBits);

  //****************
  // SetAlphaSamplingWidth()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAlphaSamplingWidth
        // @parm [in] Optional.
        (aafUInt32  alphaSamplingWidth);

  //****************
  // GetAlphaSamplingWidth()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAlphaSamplingWidth
        // @parm [out] Optional.
        (aafUInt32 *  pAlphaSamplingWidth);


protected:
	OMFixedSizeProperty<aafInt32>			_componentWidth;
	OMFixedSizeProperty<aafUInt32>			_horizontalSubsampling;
	OMFixedSizeProperty<aafUInt32>			_verticalSubsampling;
	OMFixedSizeProperty<aafColorSiting_t>	_colorSiting;
	OMFixedSizeProperty<aafUInt32>			_blackReferenceLevel;
	OMFixedSizeProperty<aafUInt32>			_whiteReferenceLevel;
	OMFixedSizeProperty<aafUInt32>			_colorRange;
	OMFixedSizeProperty<aafInt16>			_paddingBits;
	OMFixedSizeProperty<aafUInt32>			_alphaSamplingWidth;
};

#endif // ! __ImplAAFCDCIDescriptor_h__
