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
	_paddingBits(PID_CDCIDescriptor_PaddingBits,	L"PaddingBits")
{
	_persistentProperties.put(_componentWidth.address());
	_persistentProperties.put(_horizontalSubsampling.address());
	_persistentProperties.put(_verticalSubsampling.address());
	_persistentProperties.put(_colorSiting.address());
	_persistentProperties.put(_blackReferenceLevel.address());
	_persistentProperties.put(_whiteReferenceLevel.address());
	_persistentProperties.put(_colorRange.address());
	_persistentProperties.put(_paddingBits.address());

	// Initialize Required properties
	_componentWidth = 8;	// valid values are 8, 10, and 16 ?
	_horizontalSubsampling = 1; // valid values are 1 and 2?
	_verticalSubsampling = 1; // valid values are 1 and 2?

	// Initialize Optional properties
	_colorSiting = kAAFCoSiting;
	_blackReferenceLevel = 0;
	_whiteReferenceLevel = 255;
	_colorRange = 255;
	_paddingBits = 0;
}


ImplAAFCDCIDescriptor::~ImplAAFCDCIDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCDCIDescriptor::SetComponentWidth (aafInt32  ComponentWidth)
{
	AAFRESULT	hr;

	switch (ComponentWidth)
	{
	case 8:
	case 10:
	case 16:
		_componentWidth = ComponentWidth;
		hr = AAFRESULT_SUCCESS;
		break;

	default:
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

  AAFRESULT stat = AAFRESULT_INTERNAL_ERROR;

	switch (HorizontalSubsampling)
	{
	case 1:
	case 2:
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

  AAFRESULT stat = AAFRESULT_INTERNAL_ERROR;

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
