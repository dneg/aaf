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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFDefUIDs.h"
#include "AAFCodecDefs.h"

#include <assert.h>
#include <string.h>

ImplAAFDigitalImageDescriptor::ImplAAFDigitalImageDescriptor ()
:	_compression(PID_DigitalImageDescriptor_Compression,					"Compression"),
	_storedHeight(PID_DigitalImageDescriptor_StoredHeight,					"StoredHeight"),
	_storedWidth(PID_DigitalImageDescriptor_StoredWidth,					"StoredWidth"),
	_sampledHeight(PID_DigitalImageDescriptor_SampledHeight,				"SampledHeight"),
	_sampledWidth(PID_DigitalImageDescriptor_SampledWidth,					"SampledWidth"),
	_sampledXOffset(PID_DigitalImageDescriptor_SampledXOffset,				"SampledXOffset"),
	_sampledYOffset(PID_DigitalImageDescriptor_SampledYOffset,				"SampledYOffset"),
	_displayHeight(PID_DigitalImageDescriptor_DisplayHeight,				"DisplayHeight"),
	_displayWidth(PID_DigitalImageDescriptor_DisplayWidth,					"DisplayWidth"),
	_displayXOffset(PID_DigitalImageDescriptor_DisplayXOffset,				"DisplayXOffset"),
	_displayYOffset(PID_DigitalImageDescriptor_DisplayYOffset,				"DisplayYOffset"),
	_frameLayout(PID_DigitalImageDescriptor_FrameLayout,					"FrameLayout"),
	_videoLineMap(PID_DigitalImageDescriptor_VideoLineMap,					"VideoLineMap"),
	_imageAspectRatio(PID_DigitalImageDescriptor_ImageAspectRatio,			"ImageAspectRatio"),
	_alphaTransparency(PID_DigitalImageDescriptor_AlphaTransparency,		"AlphaTransparency"),
	_gamma(PID_DigitalImageDescriptor_Gamma,								"Gamma"),
	_imageAlignmentFactor(PID_DigitalImageDescriptor_ImageAlignmentFactor,	"ImageAlignmentFactor")
{
	aafInt32	videoLineMap[2];

	_persistentProperties.put(_compression.address());
	_persistentProperties.put(_storedHeight.address());
	_persistentProperties.put(_storedWidth.address());
	_persistentProperties.put(_sampledHeight.address());
	_persistentProperties.put(_sampledWidth.address());
	_persistentProperties.put(_sampledXOffset.address());
	_persistentProperties.put(_sampledYOffset.address());
	_persistentProperties.put(_displayHeight.address());
	_persistentProperties.put(_displayWidth.address());
	_persistentProperties.put(_displayXOffset.address());
	_persistentProperties.put(_displayYOffset.address());
	_persistentProperties.put(_frameLayout.address());
	_persistentProperties.put(_videoLineMap.address());
	_persistentProperties.put(_imageAspectRatio.address());
	_persistentProperties.put(_alphaTransparency.address());
	_persistentProperties.put(_gamma.address());
	_persistentProperties.put(_imageAlignmentFactor.address());

	aafRational_t zero;
	zero.numerator = 0;
	zero.denominator = 1;
	
	// Initialize Required properties
	_storedHeight = 0;
	_storedWidth = 0;
	_frameLayout = kFullFrame;
	_imageAspectRatio = zero;
	videoLineMap[0] = 0;
	videoLineMap[1] = 1;
	_videoLineMap.setValue(videoLineMap, 2*sizeof(aafInt32));
	// Initialize Optional properties

	_compression = NoCodec;
	_sampledHeight = 0;
	_sampledWidth = 0;
	_sampledXOffset = 0;
	_sampledYOffset = 0;
	_displayHeight = 0;
	_displayWidth = 0;
	_displayXOffset = 0;
	_displayYOffset = 0;
	_alphaTransparency = zero;
	_gamma = zero;
	_imageAlignmentFactor = 0;
}


ImplAAFDigitalImageDescriptor::~ImplAAFDigitalImageDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetCompression (aafUID_t*  pCompression)
{
	if (pCompression == NULL)
		return AAFRESULT_NULL_PARAM;

	_compression = *pCompression;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetStoredView (aafUInt32	StoredHeight, aafUInt32 StoredWidth)
{
	_storedHeight = StoredHeight;
	_storedWidth = StoredWidth;

	return AAFRESULT_SUCCESS;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFDigitalImageDescriptor::SetSampledView (aafUInt32 SampledHeight,
												  aafUInt32 SampledWidth,
												  aafInt32 SampledXOffset,
												  aafInt32 SampledYOffset)
{
	_sampledHeight = SampledHeight;
	_sampledWidth = SampledWidth;
	_sampledXOffset = SampledXOffset;
	_sampledYOffset = SampledYOffset;

	return AAFRESULT_SUCCESS;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFDigitalImageDescriptor::SetDisplayView (aafUInt32 DisplayHeight,
												  aafUInt32 DisplayWidth,
												  aafInt32 DisplayXOffset,
												  aafInt32 DisplayYOffset)
{
	_displayHeight = DisplayHeight;
	_displayWidth = DisplayWidth;
	_displayXOffset = DisplayXOffset;
	_displayYOffset = DisplayYOffset;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetFrameLayout (aafFrameLayout_t FrameLayout)
{
	_frameLayout = FrameLayout;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetVideoLineMap (aafUInt32 numberElements, aafInt32* pVideoLineMap)
{
	if(pVideoLineMap == NULL)
		return(AAFRESULT_NULL_PARAM);

	_videoLineMap.setValue(pVideoLineMap, numberElements * sizeof(aafInt32));

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetImageAspectRatio (aafRational_t ImageAspectRatio)
{
	_imageAspectRatio = ImageAspectRatio;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetAlphaTransparency (aafAlphaTransparency_t AlphaTransparency)
{
	if ((AlphaTransparency != kMinValueTransparent) && (AlphaTransparency != kMaxValueTransparent))
	{
		return AAFRESULT_INVALID_TRANSPARENCY;
	}

	aafRational_t transparency;

	if (AlphaTransparency == kMinValueTransparent)
	{
		transparency.numerator = 0;
		transparency.denominator = 1;
	}
	else
	{
		transparency.numerator = 1;
		transparency.denominator = 1;
	}
	_alphaTransparency = transparency;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetGamma (aafRational_t Gamma)
{
	_gamma = Gamma;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetImageAlignmentFactor (aafInt32 ImageAlignmentFactor)
{
	_imageAlignmentFactor = ImageAlignmentFactor;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetCompression (aafUID_t* pCompression)
{
	if (pCompression == NULL)
		return AAFRESULT_NULL_PARAM;

	*pCompression = _compression;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetStoredView (aafUInt32* pStoredHeight,
												  aafUInt32* pStoredWidth)
{
	if ((pStoredHeight == NULL)|| (pStoredWidth == NULL))
		return AAFRESULT_NULL_PARAM;

	*pStoredHeight = _storedHeight;
	*pStoredWidth = _storedWidth;

	return AAFRESULT_SUCCESS;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFDigitalImageDescriptor::GetSampledView (aafUInt32* pSampledHeight,
												  aafUInt32* pSampledWidth,
												  aafInt32* pSampledXOffset,
												  aafInt32* pSampledYOffset)
{
	if ((pSampledHeight == NULL) || (pSampledWidth == NULL) ||
		(pSampledXOffset == NULL) || (pSampledYOffset == NULL))
		return(AAFRESULT_NULL_PARAM);
	
	if (!_sampledHeight.isPresent()  || !_sampledWidth.isPresent()   || 
		!_sampledXOffset.isPresent() ||	!_sampledYOffset.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
		
		*pSampledHeight = _sampledHeight;
	*pSampledWidth = _sampledWidth;
	*pSampledXOffset = _sampledXOffset;
	*pSampledYOffset = _sampledYOffset;

	return AAFRESULT_SUCCESS;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFDigitalImageDescriptor::GetDisplayView (aafUInt32* pDisplayHeight,
												  aafUInt32* pDisplayWidth,
												  aafInt32*  pDisplayXOffset,
												  aafInt32* pDisplayYOffset)
{
	if ((pDisplayHeight == NULL)  || (pDisplayWidth == NULL) ||
		(pDisplayXOffset == NULL) || (pDisplayYOffset == NULL))
		return(AAFRESULT_NULL_PARAM);
	
	if (!_displayHeight.isPresent()	 || !_displayWidth.isPresent()	 ||
		!_displayXOffset.isPresent() || !_displayXOffset.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pDisplayHeight = _displayHeight;
	*pDisplayWidth = _displayWidth;
	*pDisplayXOffset = _displayXOffset;
	*pDisplayYOffset = _displayYOffset;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetFrameLayout (aafFrameLayout_t* pFrameLayout)
{
	if (pFrameLayout == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pFrameLayout = _frameLayout;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetVideoLineMapSize (aafUInt32* pNumberElements)
{
	if(pNumberElements == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pNumberElements = _videoLineMap.size() / sizeof(aafInt32);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetVideoLineMap (aafUInt32 numberElements, aafInt32* pVideoLineMap)
{
	if(pVideoLineMap == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_videoLineMap.size() /  sizeof(aafInt32) > numberElements)
	  return AAFRESULT_SMALLBUF;

	_videoLineMap.copyToBuffer(pVideoLineMap, _videoLineMap.size());

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetImageAspectRatio (aafRational_t* pImageAspectRatio)
{
	if (pImageAspectRatio == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pImageAspectRatio = _imageAspectRatio;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetAlphaTransparency (aafAlphaTransparency_t* pAlphaTransparency)
{
	if (pAlphaTransparency == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_alphaTransparency.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	aafRational_t transparency = _alphaTransparency;

	if (transparency.numerator == 0)
	{
		*pAlphaTransparency = kMinValueTransparent;
	}
	else
	{
		*pAlphaTransparency = kMaxValueTransparent;
	}

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetGamma (aafRational_t* pGamma)
{
	if (pGamma == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if (!_gamma.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pGamma = _gamma;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetImageAlignmentFactor (aafInt32* pImageAlignmentFactor)
{
	if (pImageAlignmentFactor == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_imageAlignmentFactor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pImageAlignmentFactor = _imageAlignmentFactor;

	return AAFRESULT_SUCCESS;
}



OMDEFINE_STORABLE(ImplAAFDigitalImageDescriptor, AUID_AAFDigitalImageDescriptor);
