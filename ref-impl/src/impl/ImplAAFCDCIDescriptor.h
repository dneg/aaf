#ifndef __ImplAAFCDCIDescriptor_h__
#define __ImplAAFCDCIDescriptor_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

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



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFCDCIDescriptor)

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
