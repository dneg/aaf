/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFEssenceStream.h"
#include "ImplAAFEssenceStream.h"

#include <assert.h>


#if AS_PLUGIN
// CLSID for AAFEssenceStream 
// {83402901-9146-11d2-8088-006008143e6f}
const CLSID CLSID_AAFEssenceStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


// Default Interface for AAFEssenceStream 
// {83402902-9146-11d2-8088-006008143e6f}
const IID IID_IAAFEssenceStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
#else
extern const CLSID CLSID_AAFEssenceStream;
extern const IID IID_IAAFEssenceStream;
#endif


CAAFEssenceStream::CAAFEssenceStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, AAFFalse)
{
  if (doInit)
    {
#ifdef BOB_TEST
      ImplAAFEssenceStream * newRep;
      newRep = new ImplAAFEssenceStream;
      assert (newRep);
      InitRep (newRep);
#endif
    }
}


CAAFEssenceStream::~CAAFEssenceStream ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);



  hr = ptr->Write (buffer,
    buflen);


  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);




  hr = ptr->Read (buflen,
    buffer,
    bytesRead);



  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::Seek (aafUInt32  byteOffset)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);


  hr = ptr->Seek (byteOffset);

  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::SeekRelative (aafInt32  byteOffset)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);


  hr = ptr->SeekRelative (byteOffset);

  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::IsPosValid (aafUInt32  byteOffset,
        aafBool *  isValid)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);



  hr = ptr->IsPosValid (byteOffset,
    isValid);


  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::GetPosition (aafInt64 *  position)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);


  hr = ptr->GetPosition (position);

  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::GetLength (aafInt64 *  position)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);


  hr = ptr->GetLength (position);

  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::omcFlushCache ()
{
#ifdef BOB_TEST
  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);
  return ptr->omcFlushCache();
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceStream::SetCacheSize (aafInt32  itsSize)
{
#ifdef BOB_TEST
  HRESULT hr;

  ImplAAFEssenceStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceStream*> (pO);
  assert (ptr);


  hr = ptr->SetCacheSize (itsSize);

  return hr;
#else
  return HRESULT_NOT_IMPLEMENTED;
#endif
}


//
// 
// 
HRESULT CAAFEssenceStream::InternalQueryInterface
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
AAF_DEFINE_FACTORY(AAFEssenceStream)

