/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFDefaultStream.h"

#include <assert.h>

const CLSID CLSID_AAFDefaultStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

#if AS_PLUGIN


// Default Interface for AAFDefaultStream 
// {83402902-9146-11d2-8088-006008143e6f}
const IID IID_IAAFDefaultStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
#else
extern const CLSID CLSID_AAFDefaultStream;
extern const IID IID_IAAFDefaultStream;
#endif


CAAFDefaultStream::CAAFDefaultStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, AAFFalse)
{
}


CAAFDefaultStream::~CAAFDefaultStream ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Seek (aafUInt32  byteOffset)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::SeekRelative (aafInt32  byteOffset)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::IsPosValid (aafUInt32  byteOffset,
        aafBool *  isValid)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::GetPosition (aafInt64 *  position)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::GetLength (aafInt64 *  position)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::omcFlushCache ()
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::SetCacheSize (aafInt32  itsSize)
{
  return HRESULT_NOT_IMPLEMENTED;
}


//
// 
// 
HRESULT CAAFDefaultStream::InternalQueryInterface
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
HRESULT CAAFDefaultStream::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFDefaultStream *pAAFDefaultStream = new CAAFDefaultStream(pUnkOuter);
 	if (NULL == pAAFDefaultStream)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceStream *>(pAAFDefaultStream);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }
