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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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










#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFRGBADescriptor_h__
#include "ImplAAFRGBADescriptor.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFRGBADescriptor::ImplAAFRGBADescriptor ()
:	_pixelLayout(PID_RGBADescriptor_PixelLayout,					"PixelLayout"),
	_palette(PID_RGBADescriptor_Palette,							"Palette"),
	_paletteLayout(PID_RGBADescriptor_PaletteLayout,				"PaletteLayout")
{
	RGBComponentArray	comp;
	aafUInt16			n;

	_persistentProperties.put(_pixelLayout.address());
	_persistentProperties.put(_palette.address());
	_persistentProperties.put(_paletteLayout.address());

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
    ImplAAFRGBADescriptor::SetPallete (
      aafUInt32  numberElements,
      aafUInt8*  pPalette)
{
	if(pPalette == NULL)
		return(AAFRESULT_NULL_PARAM);

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




