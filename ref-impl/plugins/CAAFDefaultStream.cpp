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

#include "CAAFDefaultStream.h"

#include <assert.h>
#include "AAFResult.h"

const CLSID CLSID_AAFDefaultStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

// Default Interface for AAFDefaultStream 
// {83402902-9146-11d2-8088-006008143e6f}
const IID IID_IAAFEssenceStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


CAAFDefaultStream::CAAFDefaultStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFUnknown (pControllingUnknown)
{
}


CAAFDefaultStream::~CAAFDefaultStream ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Write (aafDataBuffer_t  buffer,
        aafInt32  buflen)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::Seek (aafInt64  byteOffset)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::SeekRelative (aafInt32  byteOffset)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::IsPosValid (aafInt64  byteOffset,
        aafBool *  isValid)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::GetPosition (aafInt64 *  position)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::GetLength (aafInt64 *  position)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::omcFlushCache ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFDefaultStream::SetCacheSize (aafInt32  itsSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
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
