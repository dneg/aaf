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

#include "CAAFEssenceDataStream.h"

#include "AAF.h" // need declaration for IAAFEssenceData.

#include <assert.h>
#include "AAFResult.h"

const CLSID CLSID_AAFEssenceDataStream = { 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
//const IID IID_IAAFEssenceData = { 0x6a33f4e2, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };


CAAFEssenceDataStream::CAAFEssenceDataStream (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown),
  _data(NULL)
{
}


CAAFEssenceDataStream::~CAAFEssenceDataStream ()
{
	if(_data != NULL)
		_data->Release();
	_data = NULL;
}

HRESULT STDMETHODCALLTYPE
CAAFEssenceDataStream::Init(
            /* [in] */ IUnknown  *essenceData)
{
  if (NULL == essenceData)
    return AAFRESULT_NULL_PARAM;

  if (NULL != _data)
    return AAFRESULT_NOT_INITIALIZED;

	return(essenceData->QueryInterface (IID_IAAFEssenceData, (void **)&_data));
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Write (
    aafUInt32 bytes,
    aafDataBuffer_t  buffer,
    aafUInt32  *pBytesWritten)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  else if (NULL == buffer || NULL == pBytesWritten)
    return AAFRESULT_NULL_PARAM;

  return(_data->Write (bytes, buffer, pBytesWritten));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  else if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;


	return(_data->Read (buflen, buffer, bytesRead));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Seek (aafPosition_t  byteOffset)
{
	HRESULT		hr = S_OK;
	aafBool		valid;

	if (NULL == _data)
		return AAFRESULT_NOT_INITIALIZED;

  	hr = IsPosValid (byteOffset, &valid);
	if(hr != S_OK)
		return(hr);

	if(!valid)
		return(AAFRESULT_BADSAMPLEOFFSET);

	return(_data->SetPosition(byteOffset));
}

// NOTE: If the GetPosition and SetPosition methods are changed 
// to use a aafUInt64 then the following code will have to check
// for sign wrapping...

HRESULT STDMETHODCALLTYPE    CAAFEssenceDataStream::SeekRelative (aafInt32  byteOffset)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

  HRESULT hr = S_OK;

  // Get the current position and then just add the given offset 
  // and attempt to set the new position.
  aafPosition_t pos;
  hr = GetPosition(&pos);
  if (SUCCEEDED(hr))
  {
    aafPosition_t newPos = pos + byteOffset;
    hr = Seek(newPos);
  }

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::IsPosValid (aafPosition_t  byteOffset,
        aafBool *  isValid)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == isValid)
    return E_INVALIDARG;
  
  *isValid = kAAFFalse;

  if (0 <= byteOffset)
  {
    aafLength_t length = 0;
    HRESULT hr = GetLength(&length);
    if (AAFRESULT_SUCCESS != hr)
      return hr;

    if (byteOffset < length)
    {
      *isValid = kAAFTrue;
    }
    else if (byteOffset == length)
    {
      // Since we don't currently save the mode then
      // we don't know whether or not the next
      // file operation will be a read or a write
      // so we just return true.
      *isValid = kAAFTrue;
    }
  }

  return S_OK;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetPosition (aafPosition_t *  position)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	return(_data->GetPosition (position));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetLength (aafLength_t *  position)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	aafLength_t	result;
	HRESULT		status;

	status = _data->GetSize (&result);
	*position = result;
	return status;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::FlushCache ()
{
  return S_OK; //AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::SetCacheSize (aafUInt32  itsSize)
{
  return S_OK; //AAFRESULT_NOT_IMPLEMENTED;
}


//
// 
// 
HRESULT CAAFEssenceDataStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IID_IAAFEssenceDataStream interface 
    if (riid == IID_IAAFEssenceDataStream) 
    { 
        *ppvObj = (IAAFEssenceDataStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (riid == IID_IAAFEssenceStream) 
    { 
        *ppvObj = (IAAFEssenceStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (riid == IID_IAAFPlugin) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFEssenceDataStream)
