#ifndef __ImplAAFCDCIDescriptor_h__
#define __ImplAAFCDCIDescriptor_h__

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

protected:
	OMFixedSizeProperty<aafInt32>			_componentWidth;
	OMFixedSizeProperty<aafUInt32>			_horizontalSubsampling;
	OMFixedSizeProperty<aafColorSiting_t>	_colorSiting;
	OMFixedSizeProperty<aafUInt32>			_blackReferenceLevel;
	OMFixedSizeProperty<aafUInt32>			_whiteReferenceLevel;
	OMFixedSizeProperty<aafUInt32>			_colorRange;
	OMFixedSizeProperty<aafInt16>			_paddingBits;
};

#endif // ! __ImplAAFCDCIDescriptor_h__
