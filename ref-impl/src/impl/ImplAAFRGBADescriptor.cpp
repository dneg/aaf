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
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=










#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFRGBADescriptor_h__
#include "ImplAAFRGBADescriptor.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFRGBADescriptor::ImplAAFRGBADescriptor ()
:	_pixelLayout(PID_RGBADescriptor_PixelLayout,					L"PixelLayout"),
	_palette(PID_RGBADescriptor_Palette,							L"Palette"),
	_paletteLayout(PID_RGBADescriptor_PaletteLayout,				L"PaletteLayout"),
	_componentMaxRef(PID_RGBADescriptor_ComponentMaxRef,				L"ComponentMaxRef"),
	_componentMinRef(PID_RGBADescriptor_ComponentMinRef,				L"ComponentMinRef"),
	_alphaMaxRef(PID_RGBADescriptor_AlphaMaxRef,				L"AlphaMaxRef"),
	_alphaMinRef(PID_RGBADescriptor_AlphaMinRef,				L"AlphaMinRef"),
	_scanningDirection(PID_RGBADescriptor_ScanningDirection,				L"ScanningDirection")
{
	RGBComponentArray	comp;
	aafUInt16			n;

	_persistentProperties.put(_pixelLayout.address());
	_persistentProperties.put(_palette.address());
	_persistentProperties.put(_paletteLayout.address());
	_persistentProperties.put(_componentMaxRef.address());
	_persistentProperties.put(_componentMinRef.address());
	_persistentProperties.put(_alphaMaxRef.address());
	_persistentProperties.put(_alphaMinRef.address());
	_persistentProperties.put(_scanningDirection.address());

	// Initialize Required properties
	for(n = 0; n < MAX_NUM_RGBA_COMPS; n++)
	{
		comp.comps[n].Code = kAAFCompNone;
		comp.comps[n].Size = 0;
	}
	_pixelLayout.setValue(comp);
}


ImplAAFRGBADescriptor::~ImplAAFRGBADescriptor ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetPixelLayout (
       aafInt32	numberElements,
	aafRGBAComponent_t*  PixelLayoutArray)
{
	aafInt32	n;
	RGBComponentArray	local;
	if(PixelLayoutArray == NULL)
		return(AAFRESULT_NULL_PARAM);
	if (numberElements > MAX_NUM_RGBA_COMPS)
	  return AAFRESULT_SMALLBUF;

	memcpy(&local, PixelLayoutArray, numberElements*sizeof(aafRGBAComponent_t));

	for(n = numberElements; n < MAX_NUM_RGBA_COMPS; n++)
	{
		local.comps[n].Code = kAAFCompNone;
		local.comps[n].Size = 0;
	}
	_pixelLayout = local;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::CountPixelLayoutElements (aafUInt32 *  pResult)
{
	aafUInt32			n, result;
	RGBComponentArray	local;

	local = _pixelLayout;
	for( n = 0, result = 0; n < MAX_NUM_RGBA_COMPS; n++)
	{
		if(local.comps[n].Code != kAAFCompNone)
		{
			result++;
		}
	}

	*pResult = result;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetPixelLayout (
      aafUInt32  numberElements,
      aafRGBAComponent_t*  PixelLayoutArray)
{
	RGBComponentArray	local;

	if(PixelLayoutArray == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (MAX_NUM_RGBA_COMPS < numberElements)
	  return AAFRESULT_SMALLBUF;

	local = _pixelLayout;
	memcpy(PixelLayoutArray, local.comps, numberElements * sizeof(aafRGBAComponent_t));

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetPalette (
      aafUInt32  numberElements,
      aafUInt8*  pPalette)
{
	if(pPalette == NULL)
		return(AAFRESULT_NULL_PARAM);

	if ((numberElements * sizeof(aafUInt8)) > OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_palette.setValue(pPalette, numberElements * sizeof(aafUInt8));

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetPalette (
      aafUInt32  numberBytes,
      aafUInt8 *pPalette)
{
	if(pPalette == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_palette.size() < numberBytes)
	  return AAFRESULT_SMALLBUF;

	_palette.copyToBuffer(pPalette, _palette.size());

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetPaletteSize (
      aafUInt32 *pNumberBytes)
{
	if(pNumberBytes == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pNumberBytes = _palette.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetPaletteLayout (
       aafInt32	numberElements,
     aafRGBAComponent_t*  PaletteLayoutArray)
{
	aafInt32	n;
	RGBComponentArray	local;
	if(PaletteLayoutArray == NULL)
		return(AAFRESULT_NULL_PARAM);
	if (numberElements > MAX_NUM_RGBA_COMPS)
	  return AAFRESULT_SMALLBUF;
	memcpy(local.comps, PaletteLayoutArray, numberElements*sizeof(aafRGBAComponent_t));
	for(n = numberElements; n < MAX_NUM_RGBA_COMPS; n++)
	{
		local.comps[n].Code = kAAFCompNone;
		local.comps[n].Size = 0;
	}
	_paletteLayout.setValue(local);

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::CountPaletteLayoutElements (aafUInt32 *  pResult)
{
	aafUInt32			n, result;
	RGBComponentArray	local;

	local = _paletteLayout;
	for( n = 0, result = 0; n < MAX_NUM_RGBA_COMPS; n++)
	{
		if(local.comps[n].Code != kAAFCompNone)
		{
			result++;
		}
	}

	*pResult = result;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetPaletteLayout (
      aafUInt32  numberElements,
      aafRGBAComponent_t*  PaletteLayoutArray)
{
	RGBComponentArray	local;

	if(PaletteLayoutArray == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_paletteLayout.size() < numberElements)
	  return AAFRESULT_SMALLBUF;

	_paletteLayout.getValue(local);
	memcpy(PaletteLayoutArray, local.comps, numberElements * sizeof(aafRGBAComponent_t));

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetComponentMaxRef (
      aafUInt32  componentMaxRef)
{
	_componentMaxRef = componentMaxRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetComponentMaxRef (
      aafUInt32 *  pComponentMaxRef)
{
	if (pComponentMaxRef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_componentMaxRef.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pComponentMaxRef = _componentMaxRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetComponentMinRef (
      aafUInt32  componentMinRef)
{
	_componentMinRef = componentMinRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetComponentMinRef (
      aafUInt32 *  pComponentMinRef)
{
	if (pComponentMinRef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_componentMinRef.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pComponentMinRef = _componentMinRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetAlphaMaxRef (
      aafUInt32  alphaMaxRef)
{
	_alphaMaxRef = alphaMaxRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetAlphaMaxRef (
      aafUInt32 *  pAlphaMaxRef)
{
	if (pAlphaMaxRef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_alphaMaxRef.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pAlphaMaxRef = _alphaMaxRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetAlphaMinRef (
      aafUInt32  alphaMinRef)
{
	_alphaMinRef = alphaMinRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetAlphaMinRef (
      aafUInt32 *  pAlphaMinRef)
{
	if (pAlphaMinRef == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_alphaMinRef.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pAlphaMinRef = _alphaMinRef;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::SetScanningDirection (
      aafScanningDirection_t scanningDirection)
{
	_scanningDirection = scanningDirection;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRGBADescriptor::GetScanningDirection (
      aafScanningDirection_t *pScanningDirection)
{
	if (pScanningDirection == NULL)
	  return AAFRESULT_NULL_PARAM;

	if (!_scanningDirection.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pScanningDirection = _scanningDirection;

	return AAFRESULT_SUCCESS;
}
