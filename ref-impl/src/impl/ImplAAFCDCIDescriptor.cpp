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

#ifndef __ImplAAFCDCIDescriptor_h__
#include "ImplAAFCDCIDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFCDCIDescriptor::ImplAAFCDCIDescriptor ()
:	_componentWidth(PID_CDCIDescriptor_ComponentWidth,	L"ComponentWidth"),
	_horizontalSubsampling(PID_CDCIDescriptor_HorizontalSubsampling,	L"HorizontalSubsampling"),
	_verticalSubsampling(PID_CDCIDescriptor_VerticalSubsampling,	L"VerticalSubsampling"),
	_colorSiting(PID_CDCIDescriptor_ColorSiting,	L"ColorSiting"),
	_blackReferenceLevel(PID_CDCIDescriptor_BlackReferenceLevel,	L"BlackReferenceLevel"),
	_whiteReferenceLevel(PID_CDCIDescriptor_WhiteReferenceLevel,	L"WhiteReferenceLevel"),
	_colorRange(PID_CDCIDescriptor_ColorRange,	L"ColorRange"),
	_paddingBits(PID_CDCIDescriptor_PaddingBits,	L"PaddingBits"),
	_alphaSamplingWidth(PID_CDCIDescriptor_AlphaSamplingWidth,	L"AlphaSamplingWidth"),
	_reversedByteOrder(PID_CDCIDescriptor_ReversedByteOrder,	L"ReversedByteOrder")
{
	_persistentProperties.put(_componentWidth.address());
	_persistentProperties.put(_horizontalSubsampling.address());
	_persistentProperties.put(_verticalSubsampling.address());
	_persistentProperties.put(_colorSiting.address());
	_persistentProperties.put(_blackReferenceLevel.address());
	_persistentProperties.put(_whiteReferenceLevel.address());
	_persistentProperties.put(_colorRange.address());
	_persistentProperties.put(_paddingBits.address());
	_persistentProperties.put(_alphaSamplingWidth.address());
	_persistentProperties.put(_reversedByteOrder.address());

	// Initialize Required properties
	_componentWidth = 8;	// valid values are 8, 10, and 16 ?
	_horizontalSubsampling = 1; // valid values are 1 and 2?
}


ImplAAFCDCIDescriptor::~ImplAAFCDCIDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetComponentWidth (aafInt32  ComponentWidth)
{
	AAFRESULT	hr;

	if (ComponentWidth >= 0)
	{
		_componentWidth = ComponentWidth;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = AAFRESULT_BAD_PROP;
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFCDCIDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetHorizontalSubsampling (aafUInt32 HorizontalSubsampling)
{
	AAFRESULT	hr;

	switch (HorizontalSubsampling)
	{
	case 1:
	case 2:
	case 4:
		_horizontalSubsampling = HorizontalSubsampling;
		hr = AAFRESULT_SUCCESS;
		break;

	default:
		hr = AAFRESULT_BAD_PROP;
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetVerticalSubsampling (aafUInt32 VerticalSubsampling)
{
	AAFRESULT	hr;

	switch (VerticalSubsampling)
	{
	case 1:
	case 2:
		_verticalSubsampling = VerticalSubsampling;
		hr = AAFRESULT_SUCCESS;
		break;

	default:
		hr = AAFRESULT_BAD_PROP;
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetColorSiting (aafColorSiting_t ColorSiting)
{
	_colorSiting = ColorSiting;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetBlackReferenceLevel (aafUInt32 BlackReferenceLevel)
{
	_blackReferenceLevel = BlackReferenceLevel;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetWhiteReferenceLevel (aafUInt32 WhiteReferenceLevel)
{
	_whiteReferenceLevel = WhiteReferenceLevel;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetColorRange (aafUInt32 ColorRange)
{
	_colorRange = ColorRange;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetPaddingBits (aafInt16 PaddingBits)
{
	_paddingBits = PaddingBits;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetComponentWidth (aafInt32* pComponentWidth)
{
	if (pComponentWidth == NULL)
		return AAFRESULT_NULL_PARAM;

	*pComponentWidth = _componentWidth;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetHorizontalSubsampling (aafUInt32* pHorizontalSubsampling)
{
	if (pHorizontalSubsampling == NULL)
		return AAFRESULT_NULL_PARAM;

	*pHorizontalSubsampling = _horizontalSubsampling;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetVerticalSubsampling (aafUInt32* pVerticalSubsampling)
{
	if (pVerticalSubsampling == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_verticalSubsampling.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pVerticalSubsampling = _verticalSubsampling;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetColorSiting (aafColorSiting_t* pColorSiting)
{
	if (pColorSiting == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_colorSiting.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pColorSiting = _colorSiting;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetBlackReferenceLevel (aafUInt32* pBlackReferenceLevel)
{
	if (pBlackReferenceLevel == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_blackReferenceLevel.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBlackReferenceLevel = _blackReferenceLevel;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetWhiteReferenceLevel (aafUInt32* pWhiteReferenceLevel)
{
	if (pWhiteReferenceLevel == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_whiteReferenceLevel.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pWhiteReferenceLevel = _whiteReferenceLevel;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetColorRange (aafUInt32* pColorRange)
{
	if (pColorRange == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_colorRange.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pColorRange = _colorRange;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetPaddingBits (aafInt16* pPaddingBits)
{
	if (pPaddingBits == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_paddingBits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pPaddingBits = _paddingBits;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetAlphaSamplingWidth (
      aafUInt32 alphaSamplingWidth)
{
	AAFRESULT	hr;

	if (alphaSamplingWidth >= 0)
	{
		_alphaSamplingWidth = alphaSamplingWidth;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = AAFRESULT_BAD_PROP;
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetAlphaSamplingWidth (
      aafUInt32 *pAlphaSamplingWidth)
{
	if (pAlphaSamplingWidth == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_alphaSamplingWidth.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pAlphaSamplingWidth = _alphaSamplingWidth;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetReversedByteOrder (
      aafBoolean_t  reversedByteOrder)
{
	_reversedByteOrder = reversedByteOrder;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::GetReversedByteOrder (
      aafBoolean_t *  pReversedByteOrder)
{
	if (pReversedByteOrder == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_reversedByteOrder.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pReversedByteOrder = _reversedByteOrder;

	return AAFRESULT_SUCCESS;
}
