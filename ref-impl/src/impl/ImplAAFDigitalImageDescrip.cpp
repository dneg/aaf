/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/
#include "ImplAAFDigitalImageDescriptor.h"

#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>

ImplAAFDigitalImageDescriptor::ImplAAFDigitalImageDescriptor ()
:	_compression(PID_DigitalImageDescriptor_Compression,					"Compression"),
	_storedHeight(PID_DigitalImageDescriptor_StoredHeight,					"Stored Height"),
	_storedWidth(PID_DigitalImageDescriptor_StoredWidth,					"Stored Width"),
	_sampledHeight(PID_DigitalImageDescriptor_SampledHeight,				"Sampled Height"),
	_sampledWidth(PID_DigitalImageDescriptor_SampledWidth,					"Sampled Width"),
	_sampledXOffset(PID_DigitalImageDescriptor_SampledXOffset,				"Sampled XOffset"),
	_sampledYOffset(PID_DigitalImageDescriptor_SampledYOffset,				"Sampled YOffset"),
	_displayHeight(PID_DigitalImageDescriptor_DisplayHeight,				"Display Height"),
	_displayWidth(PID_DigitalImageDescriptor_DisplayWidth,					"Display Width"),
	_displayXOffset(PID_DigitalImageDescriptor_DisplayXOffset,				"Display XOffset"),
	_displayYOffset(PID_DigitalImageDescriptor_DisplayYOffset,				"Display YOffset"),
	_frameLayout(PID_DigitalImageDescriptor_FrameLayout,					"Frame Layout"),
	_videoLineMap(PID_DigitalImageDescriptor_VideoLIneMap,					"Video Line Map"),
	_imageAspectRatio(PID_DigitalImageDescriptor_ImageAspectRatio,			"Image Aspect Ratio"),
	_alphaTransparency(PID_DigitalImageDescriptor_AlphaTransparency,		"Alpha Transparency"),
	_gamma(PID_DigitalImageDescriptor_Gamma,								"Gamma"),
	_imageAlignmentFactor(PID_DigitalImageDescriptor_ImageAlignmentFactor,	"Image Alignment Factor")
{
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

	// Initialize Required properties
	_storedHeight = 0;
	_storedWidth = 0;
	//_frameLayout = kFullFrame;
	//_imageAspectRatio = ;
	//_videoLineMap =

	// Initialize Optional properties
	//_compression = ;
	_sampledHeight = 0;
	_sampledWidth = 0;
	_sampledXOffset = 0;
	_sampledYOffset = 0;
	_displayHeight = 0;
	_displayWidth = 0;
	_displayXOffset = 0;
	_displayYOffset = 0;
	_alphaTransparency = kMinValueTransparent;
	//_gamma = 0;
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
	_alphaTransparency = AlphaTransparency;

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
	if ((pDisplayHeight == NULL) || (pDisplayWidth == NULL) ||
		(pDisplayXOffset == NULL) || (pDisplayYOffset == NULL))
		return(AAFRESULT_NULL_PARAM);

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

	*pAlphaTransparency = _alphaTransparency;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetGamma (aafRational_t* pGamma)
{
	if (pGamma == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pGamma = _gamma;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetImageAlignmentFactor (aafInt32* pImageAlignmentFactor)
{
	if (pImageAlignmentFactor == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pImageAlignmentFactor = _imageAlignmentFactor;

	return AAFRESULT_SUCCESS;
}


extern "C" const aafClassID_t CLSID_AAFDigitalImageDescriptor;

OMDEFINE_STORABLE(ImplAAFDigitalImageDescriptor, CLSID_AAFDigitalImageDescriptor);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFDigitalImageDescriptor::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFDigitalImageDescriptor, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}


