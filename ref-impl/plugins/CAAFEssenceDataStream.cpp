/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFEssenceDataStream.h"

#include <assert.h>
#include "AAFResult.h"

const CLSID CLSID_AAFEssenceDataStream = { 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const IID IID_IAAFEssenceData = { 0x6a33f4e2, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };

// Interface for AAFEssenceDataStream 
// {CDDB6AB1-98DC-11d2-808A-006008143E6F}
const IID IID_IAAFEssenceDataStream = { 0xCDDB6AB1, 0x98DC, 0x11d2, { 0x80, 0x8a, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


CAAFEssenceDataStream::CAAFEssenceDataStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFDefaultStream (pControllingUnknown, AAFFalse)
{
}


CAAFEssenceDataStream::~CAAFEssenceDataStream ()
{
}

HRESULT STDMETHODCALLTYPE
CAAFEssenceDataStream::Init(
            /* [in] */ IUnknown  *essenceData)
{
	return(essenceData->QueryInterface (IID_IAAFEssenceData, (void **)&_data));
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
	aafUInt32	written;

	return(_data->Write (buflen, buffer, &written));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
	return(_data->Read (buflen, buffer, bytesRead));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Seek (aafUInt32  byteOffset)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE    CAAFEssenceDataStream::SeekRelative (aafInt32  byteOffset)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::IsPosValid (aafUInt32  byteOffset,
        aafBool *  isValid)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetPosition (aafInt64 *  position)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetLength (aafInt64 *  position)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::omcFlushCache ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::SetCacheSize (aafInt32  itsSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFEssenceDataStream) 
    { 
        *ppvObj = (IAAFEssenceDataStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFDefaultStream::InternalQueryInterface(riid, ppvObj);
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
