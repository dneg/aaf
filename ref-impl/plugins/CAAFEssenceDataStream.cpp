/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFObjectStream.h"

#include <assert.h>

const CLSID CLSID_AAFObjectStream = { 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

#if AS_PLUGIN


// Default Interface for AAFObjectStream 
// {83402902-9146-11d2-8088-006008143e6f}
const IID IID_IAAFObjectStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
#else
extern const CLSID CLSID_AAFObjectStream;
extern const IID IID_IAAFObjectStream;
#endif


CAAFObjectStream::CAAFObjectStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFDefaultStream (pControllingUnknown, AAFFalse)
{
}


CAAFObjectStream::~CAAFObjectStream ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::Seek (aafUInt32  byteOffset)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::SeekRelative (aafInt32  byteOffset)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::IsPosValid (aafUInt32  byteOffset,
        aafBool *  isValid)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::GetPosition (aafInt64 *  position)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::GetLength (aafInt64 *  position)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::omcFlushCache ()
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFObjectStream::SetCacheSize (aafInt32  itsSize)
{
  return HRESULT_NOT_IMPLEMENTED;
}


//
// 
// 
HRESULT CAAFObjectStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFEssenceStream) 
    { 
        *ppvObj = (IAAFEssenceStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
//
// Define the contrete object support implementation.
// 
HRESULT CAAFObjectStream::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFObjectStream *pAAFObjectStream = new CAAFObjectStream(pUnkOuter);
 	if (NULL == pAAFObjectStream)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceStream *>(pAAFObjectStream);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }
