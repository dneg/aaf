/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

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
	if(_data == NULL)
		_data->Release();
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
    CAAFEssenceDataStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	aafUInt32	written;

	return(_data->Write (buflen, buffer, &written));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	return(_data->Read (buflen, buffer, bytesRead));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Seek (aafInt64  byteOffset)
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
  aafInt64 pos;
  hr = GetPosition(&pos);
  if (SUCCEEDED(hr))
  {
    aafInt64 newPos = pos + byteOffset;
    hr = Seek(newPos);
  }

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::IsPosValid (aafInt64  byteOffset,
        aafBool *  isValid)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == isValid)
    return E_INVALIDARG;
  
  *isValid = AAFFalse;

  if (0 <= byteOffset)
  {
    aafInt64 length = 0;
    HRESULT hr = GetLength(&length);
    if (AAFRESULT_SUCCESS != hr)
      return hr;

    if (byteOffset < length)
    {
      *isValid = AAFTrue;
    }
    else if (byteOffset == length)
    {
      // Since we don't currently save the mode then
      // we don't know whether or not the next
      // file operation will be a read or a write
      // so we just return true.
      *isValid = AAFTrue;
    }
  }

  return S_OK;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetPosition (aafInt64 *  position)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	return(_data->GetPosition (position));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetLength (aafInt64 *  position)
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
    CAAFEssenceDataStream::omcFlushCache ()
{
  return S_OK; //AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::SetCacheSize (aafInt32  itsSize)
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
//
// Define the contrete object support implementation.
// 
HRESULT CAAFEssenceDataStream::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFEssenceDataStream *pAAFEssenceDataStream = new CAAFEssenceDataStream(pUnkOuter);
 	if (NULL == pAAFEssenceDataStream)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceStream *>(pAAFEssenceDataStream);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }
