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
#include "CAAFJPEGDescriptorHelper.h"

#include <assert.h>
#include "AAFResult.h"

#include "AAF.h"

#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"

const aafUID_t kAAFPropID_CDCIOffsetToFrameIndexes = { 0x9d15fca3, 0x54c5, 0x11d3, { 0xa0, 0x29, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameIndexByteOrder = { 0xb57e925d, 0x170d, 0x11d4, { 0xa0, 0x8f, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDResolutionID = { 0xce2aca4d, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFirstFrameOffset = { 0xce2aca4e, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };
const aafUID_t kAAFPropID_DIDFrameSampleSize = { 0xce2aca50, 0x51ab, 0x11d3, { 0xa0, 0x24, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };


// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw r;
}

inline void checkExpression(bool test, AAFRESULT r)
{
  if (!test)
    throw r;
}


inline void checkAssertion(bool test)
{
  if (!test)
    throw HRESULT(AAFRESULT_ASSERTION_VIOLATION);
}



CAAFJPEGDescriptorHelper::CAAFJPEGDescriptorHelper() :
	_filemob_unk(NULL),
	_filemob(NULL),
	_edes(NULL),
	_filedes(NULL),
	_dides(NULL),
	_cdcides(NULL)
{
}

CAAFJPEGDescriptorHelper::~CAAFJPEGDescriptorHelper()
{
	Clear();
}

void CAAFJPEGDescriptorHelper::Clear(void)
{
	// Release all of our references...
	if (NULL != _cdcides)
	{
		_cdcides->Release();
		_cdcides = NULL;
	}
	if (NULL != _dides)
	{
		_dides->Release();
		_dides = NULL;
	}
	if (NULL != _filedes)
	{
		_filedes->Release();
		_filedes = NULL;
	}
	if (NULL != _edes)
	{
		_edes->Release();
		_edes = NULL;
	}
	if (NULL != _filemob)
	{
		_filemob->Release();
		_filemob = NULL;
	}

	if (NULL != _filemob_unk)
	{
		_filemob_unk->Release();
		_filemob_unk = NULL;
	}
}

HRESULT CAAFJPEGDescriptorHelper::Initialize(IAAFSourceMob *filemob)
{
	HRESULT hr = S_OK;


	try
	{
		// Release all previous interface pointers if any exist...
		Clear();

		// Save the unknown interface pointer for equality testing.
		// NOTE: When testing objects for equality only IUnknown pointers can be
		// compared since they are on the only implemented interface pointers that
		// cannot be tearoff interfaces (implemented by another object).
		checkResult(filemob->QueryInterface(IID_IUnknown, (void **)&_filemob_unk));
		

		// Get the file mob interface, we need it in order to get the associated
		// essence descriptor.
		checkResult(_filemob_unk->QueryInterface(IID_IAAFSourceMob, (void **)&_filemob));
		checkResult(_filemob->GetEssenceDescriptor(&_edes));

		// Get the interfaces needed for the rest of the descriptor methods.
		checkResult(_edes->QueryInterface(IID_IAAFFileDescriptor, (void **)&_filedes));
		checkResult(_edes->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&_dides));
		checkResult(_edes->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&_cdcides));
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	return hr;
}


bool CAAFJPEGDescriptorHelper::operator==(const CAAFJPEGDescriptorHelper& rhs)
{
	// Assertion: Objects must have been successfully initialized.
	checkAssertion((NULL != _filemob_unk) && (NULL != rhs._filemob_unk));
	return (_filemob_unk == rhs._filemob_unk);
}


bool CAAFJPEGDescriptorHelper::operator!=(const CAAFJPEGDescriptorHelper& rhs)
{
	// Assertion: Objects must have been successfully initialized.
	checkAssertion((NULL != _filemob_unk) && (NULL != rhs._filemob_unk));
	return (_filemob_unk != rhs._filemob_unk);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetNumLocators (aafUInt32 *  pCount)
{
	checkAssertion(NULL != _edes);
	return _edes->CountLocators(pCount);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::AppendLocator (IAAFLocator * pLocator)
{
	checkAssertion(NULL != _edes);
	return _edes->AppendLocator (pLocator);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::PrependLocator (IAAFLocator * pLocator)
{
	checkAssertion(NULL != _edes);
	return _edes->PrependLocator (pLocator);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::EnumAAFAllLocators (IEnumAAFLocators ** ppEnum)
{
	checkAssertion(NULL != _edes);
	return _edes->GetLocators (ppEnum);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetLength (aafLength_t  length)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetLength(length);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetLength (aafLength_t *  pLength)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetLength(pLength);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetCodecDef (IAAFCodecDef *pCodecDef)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetCodecDef (pCodecDef);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetCodecDef (IAAFCodecDef **ppCodecDef)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetCodecDef (ppCodecDef);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetSampleRate (const aafRational_t & rate)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetSampleRate (rate);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetSampleRate (aafRational_t*  pRate)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetSampleRate (pRate);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetContainerFormat (IAAFContainerDef *pFormat)
{
	checkAssertion(NULL != _filedes);
	return _filedes->SetContainerFormat (pFormat);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetContainerFormat (IAAFContainerDef **ppFormat)
{
	checkAssertion(NULL != _filedes);
	return _filedes->GetContainerFormat (ppFormat);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetCompression (const aafUID_t & compression)
{
  checkAssertion(NULL != _dides);
	return _dides->SetCompression (compression);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetCompression (aafUID_t *  pCompression)
{
  checkAssertion(NULL != _dides);
	return _dides->GetCompression (pCompression);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetStoredView (aafUInt32  StoredHeight,
        aafUInt32  StoredWidth)
{
  checkAssertion(NULL != _dides);
	return _dides->SetStoredView (StoredHeight, StoredWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetStoredView (aafUInt32 *  pStoredHeight,
        aafUInt32 *  pStoredWidth)
{
  checkAssertion(NULL != _dides);
	return _dides->GetStoredView (pStoredHeight, pStoredWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetSampledView (aafUInt32  SampledHeight,
        aafUInt32  SampledWidth,
        aafInt32  SampledXOffset,
        aafInt32  SampledYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->SetSampledView (SampledHeight, SampledWidth, SampledXOffset, SampledYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetSampledView (aafUInt32 *  pSampledHeight,
        aafUInt32 *  pSampledWidth,
        aafInt32 *  pSampledXOffset,
        aafInt32 *  pSampledYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->GetSampledView (pSampledHeight, pSampledWidth, pSampledXOffset, pSampledYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetDisplayView (aafUInt32  DisplayHeight,
        aafUInt32  DisplayWidth,
        aafInt32  DisplayXOffset,
        aafInt32  DisplayYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->SetDisplayView (DisplayHeight, DisplayWidth, DisplayXOffset, DisplayYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetDisplayView (aafUInt32 *  pDisplayHeight,
        aafUInt32 *  pDisplayWidth,
        aafInt32 *  pDisplayXOffset,
        aafInt32 *  pDisplayYOffset)
{
  checkAssertion(NULL != _dides);
	return _dides->GetDisplayView (pDisplayHeight, pDisplayWidth, pDisplayXOffset, pDisplayYOffset);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetFrameLayout (aafFrameLayout_t  FrameLayout)
{
  checkAssertion(NULL != _dides);
	return _dides->SetFrameLayout (FrameLayout);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetFrameLayout (aafFrameLayout_t *  pFrameLayout)
{
  checkAssertion(NULL != _dides);
	return _dides->GetFrameLayout (pFrameLayout);
}


HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetVideoLineMap (aafUInt32  numberElements,
        aafInt32 *  pVideoLineMap)
{
  checkAssertion(NULL != _dides);
	return _dides->SetVideoLineMap (numberElements, pVideoLineMap);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetVideoLineMap (aafUInt32  numberElements,
        aafInt32 *  pVideoLineMap)
{
  checkAssertion(NULL != _dides);
	return _dides->GetVideoLineMap (numberElements, pVideoLineMap);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetVideoLineMapSize (aafUInt32 *  pNumberElements)
{
  checkAssertion(NULL != _dides);
	return _dides->GetVideoLineMapSize (pNumberElements);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetImageAspectRatio (aafRational_t  ImageAspectRatio)
{
  checkAssertion(NULL != _dides);
	return _dides->SetImageAspectRatio (ImageAspectRatio);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetImageAspectRatio (aafRational_t *  pImageAspectRatio)
{
  checkAssertion(NULL != _dides);
	return _dides->GetImageAspectRatio (pImageAspectRatio);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetAlphaTransparency (aafAlphaTransparency_t  AlphaTransparency)
{
  checkAssertion(NULL != _dides);
	return _dides->SetAlphaTransparency (AlphaTransparency);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetAlphaTransparency (aafAlphaTransparency_t *  pAlphaTransparency)
{
  checkAssertion(NULL != _dides);
	return _dides->GetAlphaTransparency (pAlphaTransparency);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetGamma (aafUID_t  Gamma)
{
  checkAssertion(NULL != _dides);
	return _dides->SetGamma (Gamma);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetGamma (aafUID_t *  pGamma)
{
  checkAssertion(NULL != _dides);
	return _dides->GetGamma (pGamma);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetImageAlignmentFactor (aafUInt32  ImageAlignmentFactor)
{
  checkAssertion(NULL != _dides);
	return _dides->SetImageAlignmentFactor (ImageAlignmentFactor);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetImageAlignmentFactor (aafUInt32 *  pImageAlignmentFactor)
{
  checkAssertion(NULL != _dides);
	return _dides->GetImageAlignmentFactor (pImageAlignmentFactor);
}





HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetComponentWidth (aafInt32  ComponentWidth)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetComponentWidth (ComponentWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetComponentWidth (aafInt32 *  pComponentWidth)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetComponentWidth (pComponentWidth);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetHorizontalSubsampling (aafUInt32  HorizontalSubsampling)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetHorizontalSubsampling (HorizontalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetHorizontalSubsampling (aafUInt32 *  pHorizontalSubsampling)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetHorizontalSubsampling (pHorizontalSubsampling);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetColorSiting (aafColorSiting_t  ColorSiting)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetColorSiting (ColorSiting);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetColorSiting (aafColorSiting_t *  pColorSiting)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetColorSiting (pColorSiting);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetBlackReferenceLevel (aafUInt32  BlackReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetBlackReferenceLevel (BlackReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetBlackReferenceLevel (aafUInt32 *  pBlackReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetBlackReferenceLevel (pBlackReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetWhiteReferenceLevel (aafUInt32  WhiteReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetWhiteReferenceLevel (WhiteReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetWhiteReferenceLevel (aafUInt32 *  pWhiteReferenceLevel)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetWhiteReferenceLevel (pWhiteReferenceLevel);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetColorRange (aafUInt32  ColorRange)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetColorRange (ColorRange);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetColorRange (aafUInt32 *  pColorRange)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetColorRange (pColorRange);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetPaddingBits (aafInt16  PaddingBits)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->SetPaddingBits (PaddingBits);
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetPaddingBits (aafInt16 *  pPaddingBits)
{
	checkAssertion(NULL != _cdcides);
	return _cdcides->GetPaddingBits (pPaddingBits);
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetOffsetFrameIndexes(aafUInt32 *pOffset)
{
	IAAFClassDef		*pClassDef = NULL;
	IAAFObject			*pObj = NULL;
	IAAFPropertyDef		*pPropertyDef = NULL;
	IAAFPropertyValue	*pPropValue = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	IAAFTypeDefInt		*pTypeDefInt = NULL;
	aafInt32			val;

	HRESULT				hr = S_OK;

	checkAssertion(NULL != _dides);
	checkAssertion(NULL != pOffset);
	try
	{
		checkResult(_dides->QueryInterface(IID_IAAFObject, (void **)&pObj));
		checkResult(pObj->GetDefinition (&pClassDef));
		checkResult(pClassDef->LookupPropertyDef(kAAFPropID_CDCIOffsetToFrameIndexes, &pPropertyDef));
		checkResult(pObj->GetPropertyValue (pPropertyDef, &pPropValue));
		checkResult(pPropValue->GetType(&pTypeDef));
		checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
		checkResult(pTypeDefInt->GetInteger(pPropValue, (aafMemPtr_t) &val, sizeof (val)));
		*pOffset  = val;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	if( pClassDef != NULL )
	    pClassDef->Release();
	if( pObj != NULL )
	    pObj->Release();
	if( pPropertyDef != NULL )
	    pPropertyDef->Release();
	if( pPropValue != NULL )
	    pPropValue->Release();
	if( pTypeDef != NULL )
	    pTypeDef->Release();
	if( pTypeDefInt != NULL )
	    pTypeDefInt->Release();

	return hr;
}
				
HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetFrameIndexByteOrder(aafUInt16 *byteOrder)
{
	IAAFClassDef		*pClassDef = NULL;
	IAAFObject			*pObj = NULL;
	IAAFPropertyDef		*pPropertyDef = NULL;
	IAAFPropertyValue	*pPropValue = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	IAAFTypeDefInt		*pTypeDefInt = NULL;
	aafInt16			val;

	HRESULT				hr = S_OK;

	checkAssertion(NULL != _dides);
	checkAssertion(NULL != byteOrder);
	try
	{
		checkResult(_dides->QueryInterface(IID_IAAFObject, (void **)&pObj));
		checkResult(pObj->GetDefinition (&pClassDef));
		checkResult(pClassDef->LookupPropertyDef(kAAFPropID_DIDFrameIndexByteOrder, &pPropertyDef));
		checkResult(pObj->GetPropertyValue (pPropertyDef, &pPropValue));
		checkResult(pPropValue->GetType(&pTypeDef));
		checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
		checkResult(pTypeDefInt->GetInteger(pPropValue, (aafMemPtr_t) &val, sizeof (val)));
		*byteOrder  = val;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	if( pClassDef != NULL )
	    pClassDef->Release();
	if( pObj != NULL )
	    pObj->Release();
	if( pPropertyDef != NULL )
	    pPropertyDef->Release();
	if( pPropValue != NULL )
	    pPropValue->Release();
	if( pTypeDef != NULL )
	    pTypeDef->Release();
	if( pTypeDefInt != NULL )
	    pTypeDefInt->Release();

	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::PutFrameIndexProperties(aafUInt32 offset, aafUInt16 byteOrder)
{
	IAAFClassDef		*pClassDef = NULL;
	IAAFObject			*pObj = NULL;
	IAAFPropertyDef		*pPropertyDef = NULL;
	IAAFPropertyValue	*pPropValue = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	IAAFTypeDefInt		*pTypeDefInt = NULL;
	IAAFDictionary		*pDictionary = NULL;

	HRESULT				hr = S_OK;

	checkAssertion(NULL != _dides);

	try
	{
		checkResult(_dides->QueryInterface(IID_IAAFObject, (void **)&pObj));
		checkResult(pObj->GetDefinition (&pClassDef));
		checkResult(pObj->GetDictionary(&pDictionary));
		if(pClassDef->LookupPropertyDef(kAAFPropID_CDCIOffsetToFrameIndexes, &pPropertyDef) != AAFRESULT_SUCCESS)
		{
			checkResult(pDictionary->LookupTypeDef(kAAFTypeID_Int32, &pTypeDef));
			checkResult(pClassDef->RegisterOptionalPropertyDef (kAAFPropID_CDCIOffsetToFrameIndexes,
												L"OffsetToFrameIndexes",
												pTypeDef, &pPropertyDef));
		}
		else
		{
			checkResult(pPropertyDef->GetTypeDef(&pTypeDef));
		}
		checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
		checkResult(pTypeDefInt->CreateValue ((aafMemPtr_t) &offset, sizeof (offset), &pPropValue));
		checkResult(pObj->SetPropertyValue (pPropertyDef, pPropValue));
		pPropertyDef->Release();
		pPropertyDef = NULL;
		pPropValue->Release();
		pPropValue = NULL;
		pTypeDefInt->Release();
		pTypeDefInt = NULL;
		pTypeDef->Release();
		pTypeDef = NULL;

		if(pClassDef->LookupPropertyDef(kAAFPropID_DIDFrameIndexByteOrder, &pPropertyDef) != AAFRESULT_SUCCESS)
		{
			checkResult(pDictionary->LookupTypeDef(kAAFTypeID_Int16, &pTypeDef));
			checkResult(pClassDef->RegisterOptionalPropertyDef (kAAFPropID_DIDFrameIndexByteOrder,
												L"FrameIndexByteOrder",
												pTypeDef, &pPropertyDef));
		}
		else
		{
			checkResult(pPropertyDef->GetTypeDef(&pTypeDef));
		}

		checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
		checkResult(pTypeDefInt->CreateValue ((aafMemPtr_t) &byteOrder, sizeof (byteOrder), &pPropValue));
		checkResult(pObj->SetPropertyValue (pPropertyDef, pPropValue));
		pPropertyDef->Release();
		pPropertyDef = NULL;
		pPropValue->Release();
		pPropValue = NULL;
		pTypeDefInt->Release();
		pTypeDefInt = NULL;
		pTypeDef->Release();
		pTypeDef = NULL;
		/**/
		pClassDef->Release();
		pClassDef = NULL;
		pObj->Release();
		pObj = NULL;
		pDictionary->Release();
		pDictionary = NULL;
	}
	catch (HRESULT& rhr)
	{
		if(pPropertyDef != NULL)
			pPropertyDef->Release();
		if(pPropValue != NULL)
			pPropValue->Release();
		if(pTypeDefInt != NULL)
			pTypeDefInt->Release();
		if(pTypeDef != NULL)
			pTypeDef->Release();
		if(pClassDef != NULL)
			pClassDef->Release();
		if(pObj != NULL)
			pObj->Release();
		if(pDictionary != NULL)
			pDictionary->Release();
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::GetResolutionID( aafInt32 *p_resid )
{
	IAAFClassDef		*pClassDef = NULL;
	IAAFObject		*pObj = NULL;
	IAAFPropertyDef		*pPropertyDef = NULL;
	IAAFPropertyValue	*pPropValue = NULL;
	IAAFTypeDef		*pTypeDef = NULL;
	IAAFTypeDefInt		*pTypeDefInt = NULL;
	aafInt32			val;

	HRESULT				hr = S_OK;

	checkAssertion(NULL != _dides);
	checkAssertion(NULL != p_resid);
	try
	{
		checkResult(_dides->QueryInterface(IID_IAAFObject, (void **)&pObj));
		checkResult(pObj->GetDefinition (&pClassDef));
		checkResult(pClassDef->LookupPropertyDef(kAAFPropID_DIDResolutionID, &pPropertyDef));
		checkResult(pObj->GetPropertyValue (pPropertyDef, &pPropValue));
		checkResult(pPropValue->GetType(&pTypeDef));
		checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
		checkResult(pTypeDefInt->GetInteger(pPropValue, (aafMemPtr_t) &val, sizeof (val)));
		*p_resid  = val;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	if( pClassDef != NULL )
	    pClassDef->Release();
	if( pObj != NULL )
	    pObj->Release();
	if( pPropertyDef != NULL )
	    pPropertyDef->Release();
	if( pPropValue != NULL )
	    pPropValue->Release();
	if( pTypeDef != NULL )
	    pTypeDef->Release();
	if( pTypeDefInt != NULL )
	    pTypeDefInt->Release();

	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetResolutionID( aafInt32  resolutionID )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    // Get DigitalImageDescriptor class definition
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );


	    // Get ResolutionID property definition
	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDResolutionID, &p_propdef ) );

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );


	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&resolutionID), 
			sizeof(resolutionID), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&resolutionID), 
		    sizeof(resolutionID) ) );
	    }

	    // Set modified property value.
	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetFirstFrameOffset( aafInt32  firstFrameOffset )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDFirstFrameOffset, &p_propdef));

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );

	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&firstFrameOffset), 
			sizeof(firstFrameOffset), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		// Property value exists, modify it.
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, reinterpret_cast<aafMemPtr_t>(&firstFrameOffset), 
		    sizeof(firstFrameOffset) ) );
	    }

	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFJPEGDescriptorHelper::SetFrameSampleSize( aafInt32 frameSampleSize )
{
	HRESULT			hr = S_OK;
	IAAFObject		*p_obj = NULL;
	IAAFClassDef		*p_classdef = NULL;
	IAAFPropertyDef		*p_propdef = NULL;
	IAAFPropertyValue	*p_propval = NULL;
	IAAFTypeDef		*p_typedef = NULL;
	IAAFTypeDefInt		*p_typedef_int = NULL;


	checkAssertion(NULL != _dides);

	try
	{
	    checkResult( _dides->QueryInterface( IID_IAAFObject, 
		(void**)&p_obj ) );

	    checkResult( p_obj->GetDefinition( &p_classdef ) );

	    checkResult( p_classdef->LookupPropertyDef( 
		kAAFPropID_DIDFrameSampleSize, &p_propdef));

	    checkResult( p_propdef->GetTypeDef( &p_typedef ) );

	    checkResult( p_typedef->QueryInterface( IID_IAAFTypeDefInt, 
		(void**)&p_typedef_int ) );

	    // Try to get prop value. If it doesn't exist, create it.
	    hr = p_obj->GetPropertyValue( p_propdef, &p_propval );
	    if( hr != AAFRESULT_SUCCESS )
	    {
		if( hr == AAFRESULT_PROP_NOT_PRESENT )
		{
		    checkResult( p_typedef_int->CreateValue( 
			reinterpret_cast<aafMemPtr_t>(&frameSampleSize), 
			sizeof(frameSampleSize), &p_propval ) );

		    // At this point hr equals AAFRESULT_PROP_NOT_PRESENT.
		    // Reset it.
		    hr = AAFRESULT_SUCCESS;
		}
		else
		    throw hr;
	    }
	    else
	    {
		checkResult( p_typedef_int->SetInteger( 
		    p_propval, 
		    reinterpret_cast<aafMemPtr_t>(&frameSampleSize),
		    sizeof(frameSampleSize) ) );
	    }

	    checkResult( p_obj->SetPropertyValue( p_propdef, p_propval ) );
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}


	// Cleanup
	if( p_obj != NULL )
	    p_obj->Release();
	if( p_classdef != NULL )
	    p_classdef->Release();
	if( p_propdef != NULL )
	    p_propdef->Release();
	if( p_propval != NULL )
	    p_propval->Release();
	if( p_typedef != NULL )
	    p_typedef->Release();
	if( p_typedef_int != NULL )
	    p_typedef_int->Release();


	return hr;
}



