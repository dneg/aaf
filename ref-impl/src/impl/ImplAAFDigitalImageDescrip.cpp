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
#include "ImplAAFDigitalImageDescriptor.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFDefUIDs.h"
#include "AAFCodecDefs.h"

#include <string.h>

ImplAAFDigitalImageDescriptor::ImplAAFDigitalImageDescriptor ()
:	_compression(PID_DigitalImageDescriptor_Compression,					L"Compression"),
	_storedHeight(PID_DigitalImageDescriptor_StoredHeight,					L"StoredHeight"),
	_storedWidth(PID_DigitalImageDescriptor_StoredWidth,					L"StoredWidth"),
	_sampledHeight(PID_DigitalImageDescriptor_SampledHeight,				L"SampledHeight"),
	_sampledWidth(PID_DigitalImageDescriptor_SampledWidth,					L"SampledWidth"),
	_sampledXOffset(PID_DigitalImageDescriptor_SampledXOffset,				L"SampledXOffset"),
	_sampledYOffset(PID_DigitalImageDescriptor_SampledYOffset,				L"SampledYOffset"),
	_displayHeight(PID_DigitalImageDescriptor_DisplayHeight,				L"DisplayHeight"),
	_displayWidth(PID_DigitalImageDescriptor_DisplayWidth,					L"DisplayWidth"),
	_displayXOffset(PID_DigitalImageDescriptor_DisplayXOffset,				L"DisplayXOffset"),
	_displayYOffset(PID_DigitalImageDescriptor_DisplayYOffset,				L"DisplayYOffset"),
	_frameLayout(PID_DigitalImageDescriptor_FrameLayout,					L"FrameLayout"),
	_videoLineMap(PID_DigitalImageDescriptor_VideoLineMap,					L"VideoLineMap"),
	_imageAspectRatio(PID_DigitalImageDescriptor_ImageAspectRatio,				L"ImageAspectRatio"),
	_alphaTransparency(PID_DigitalImageDescriptor_AlphaTransparency,			L"AlphaTransparency"),
	_transferCharacteristic(PID_DigitalImageDescriptor_TransferCharacteristic,		L"TransferCharacteristic"),
	_colorPrimaries(PID_DigitalImageDescriptor_ColorPrimaries,				L"ColorPrimaries"),
	_codingEquations(PID_DigitalImageDescriptor_CodingEquations,				L"CodingEquations"),
	_imageAlignmentFactor(PID_DigitalImageDescriptor_ImageAlignmentFactor,			L"ImageAlignmentFactor"),
	_fieldDominance(PID_DigitalImageDescriptor_FieldDominance,				L"FieldDominance"),
	_fieldStartOffset(PID_DigitalImageDescriptor_FieldStartOffset,				L"FieldStartOffset"),
	_fieldEndOffset(PID_DigitalImageDescriptor_FieldEndOffset,				L"FieldEndOffset"),
	_signalStandard(PID_DigitalImageDescriptor_SignalStandard,			L"SignalStandard"),
	_storedF2Offset(PID_DigitalImageDescriptor_StoredF2Offset,				L"StoredF2Offset"),
	_displayF2Offset(PID_DigitalImageDescriptor_DisplayF2Offset,				L"DisplayF2Offset"),
	_activeFormatDescriptor(PID_DigitalImageDescriptor_ActiveFormatDescriptor,				L"ActiveFormatDescriptor")
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
	_persistentProperties.put(_transferCharacteristic.address());
	_persistentProperties.put(_colorPrimaries.address());
	_persistentProperties.put(_codingEquations.address());
	_persistentProperties.put(_imageAlignmentFactor.address());
	_persistentProperties.put(_fieldDominance.address());
	_persistentProperties.put(_fieldStartOffset.address());
	_persistentProperties.put(_fieldEndOffset.address());
	_persistentProperties.put(_signalStandard.address());
	_persistentProperties.put(_storedF2Offset.address());
	_persistentProperties.put(_displayF2Offset.address());
	_persistentProperties.put(_activeFormatDescriptor.address());

	aafRational_t	zero;
	zero.numerator = 0;
	zero.denominator = 1;

	aafUID_t zeroID;
	zeroID.Data1 = 0;
	zeroID.Data2 = 0;
	zeroID.Data3 = 0;
	zeroID.Data4[0] = 0;
	zeroID.Data4[1] = 0;
	zeroID.Data4[2] = 0;
	zeroID.Data4[3] = 0;
	zeroID.Data4[4] = 0;
	zeroID.Data4[5] = 0;
	zeroID.Data4[6] = 0;
	zeroID.Data4[7] = 0;
	
	// Initialize Required properties
	_storedHeight = 0;
	_storedWidth = 0;
	_frameLayout = kAAFFullFrame;
	_imageAspectRatio = zero;
	videoLineMap[0] = 0;
	videoLineMap[1] = 1;
	_videoLineMap.setValue(videoLineMap, 2*sizeof(aafInt32));
}


ImplAAFDigitalImageDescriptor::~ImplAAFDigitalImageDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetCompression (const aafUID_t & compression)
{
	_compression = compression;

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

	if ((numberElements * sizeof(aafInt32)) > OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

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
	if ((AlphaTransparency != kAAFMinValueTransparent) && (AlphaTransparency != kAAFMaxValueTransparent))
	{
		return AAFRESULT_INVALID_TRANSPARENCY;
	}

	_alphaTransparency = AlphaTransparency;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetGamma (aafUID_t gamma)
{
	return SetTransferCharacteristic( gamma );
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

	if (!_compression.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

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
		!_displayXOffset.isPresent() || !_displayYOffset.isPresent())
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
	
	*pAlphaTransparency = _alphaTransparency;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetGamma (aafUID_t* pGamma)
{
	return GetTransferCharacteristic( pGamma );
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetImageAlignmentFactor (aafUInt32* pImageAlignmentFactor)
{
	if (pImageAlignmentFactor == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_imageAlignmentFactor.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pImageAlignmentFactor = _imageAlignmentFactor;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetTransferCharacteristic (
      const aafUID_t & transferCharacteristic)
{
	_transferCharacteristic = transferCharacteristic;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetTransferCharacteristic (
      aafUID_t *pTransferCharacteristic)
{
	if (pTransferCharacteristic == NULL)
	  return AAFRESULT_NULL_PARAM;

	if (!_transferCharacteristic.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pTransferCharacteristic = _transferCharacteristic;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetCodingEquations (
      const aafUID_t & codingEquations)
{
	_codingEquations = codingEquations;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetCodingEquations (
      aafUID_t *pCodingEquations)
{
	if (pCodingEquations == NULL)
	  return AAFRESULT_NULL_PARAM;

	if (!_codingEquations.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pCodingEquations = _codingEquations;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetColorPrimaries (
      const aafUID_t & colorPrimaries)
{
	_colorPrimaries = colorPrimaries;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetColorPrimaries (
      aafUID_t *pColorPrimaries)
{
	if (pColorPrimaries == NULL)
	  return AAFRESULT_NULL_PARAM;

	if (!_colorPrimaries.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pColorPrimaries = _colorPrimaries;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetFieldStartOffset (
      aafUInt32  fieldStartOffset)
{
	_fieldStartOffset = fieldStartOffset;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetFieldStartOffset (
      aafUInt32 *pFieldStartOffset)
{
	if (pFieldStartOffset == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_fieldStartOffset.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pFieldStartOffset = _fieldStartOffset;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetFieldEndOffset (
      aafUInt32 fieldEndOffset)
{
	_fieldEndOffset = fieldEndOffset;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetFieldEndOffset (
      aafUInt32 *pFieldEndOffset)
{
	if (pFieldEndOffset == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_fieldEndOffset.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pFieldEndOffset = _fieldEndOffset;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetFieldDominance (
      aafFieldNumber_t  fieldDominance)
{
        AAFRESULT       hr;

        switch (fieldDominance)
        {
        case kAAFFieldOne:
        case kAAFFieldTwo:
		_fieldDominance = fieldDominance;
                hr = AAFRESULT_SUCCESS;
                break;

        default:
                hr = AAFRESULT_ILLEGAL_VALUE;
        }

        return hr;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetFieldDominance (
      aafFieldNumber_t *pFieldDominance)
{
	if (pFieldDominance == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_fieldDominance.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pFieldDominance = _fieldDominance;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetDisplayF2Offset (
      aafInt32  displayF2Offset)
{
        AAFRESULT       hr;

        switch (displayF2Offset)
        {
        case 0:
        case 1:
		_displayF2Offset = displayF2Offset;
                hr = AAFRESULT_SUCCESS;
                break;

        default:
                hr = AAFRESULT_ILLEGAL_VALUE;
        }

        return hr;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetDisplayF2Offset (
      aafInt32 *pDisplayF2Offset)
{
	if (pDisplayF2Offset == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_displayF2Offset.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pDisplayF2Offset = _displayF2Offset;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetStoredF2Offset (
      aafInt32  storedF2Offset)
{
        AAFRESULT       hr;

        switch (storedF2Offset)
        {
        case -1:
        case 0:
		_storedF2Offset = storedF2Offset;
                hr = AAFRESULT_SUCCESS;
                break;

        default:
                hr = AAFRESULT_ILLEGAL_VALUE;
        }

        return hr;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetStoredF2Offset (
      aafInt32 *pStoredF2Offset)
{
	if (pStoredF2Offset == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_storedF2Offset.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pStoredF2Offset = _storedF2Offset;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetActiveFormatDescriptor (
      aafUInt8  activeFormatDescriptor)
{
	_activeFormatDescriptor = activeFormatDescriptor;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetActiveFormatDescriptor (
      aafUInt8 *pActiveFormatDescriptor)
{
	if (pActiveFormatDescriptor == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_activeFormatDescriptor.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pActiveFormatDescriptor = _activeFormatDescriptor;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::SetSignalStandard (
      aafSignalStandard_t signalStandard)
{
	_signalStandard = signalStandard;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDigitalImageDescriptor::GetSignalStandard (
      aafSignalStandard_t *pSignalStandard)
{
	if (pSignalStandard == NULL)
	  return AAFRESULT_NULL_PARAM;

	if (!_signalStandard.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pSignalStandard = _signalStandard;

	return AAFRESULT_SUCCESS;
}
