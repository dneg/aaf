/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

// @doc OMINTERNAL
#include "OMRawStorageLockBytes.h"

#include "OMAssertions.h"
#include "OMMSStructuredStorage.h"

#include "OMRawStorage.h"

HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::QueryInterface(REFIID /* riid */,
                                      void** ppvObject)
{
  TRACE("OMRawStorageLockBytes::QueryInterface");

  *ppvObject = 0;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
OMRawStorageLockBytes::AddRef(void)
{
  TRACE("OMRawStorageLockBytes::AddRef");

  ++_referenceCount;
  return _referenceCount;
}

ULONG STDMETHODCALLTYPE
OMRawStorageLockBytes::Release(void)
{
  TRACE("OMRawStorageLockBytes::Release");

  _referenceCount--;
  if (_referenceCount == 0) {
    delete this;
  }
  return _referenceCount;
}

  // @mfunc Constructor.
OMRawStorageLockBytes::OMRawStorageLockBytes(OMRawStorage* rawStorage)
: _rawStorage(rawStorage),
  _referenceCount(1)
{
  TRACE("OMRawStorageLockBytes::OMRawStorageLockBytes");

  PRECONDITION("Valid raw storage", _rawStorage != 0);
  PRECONDITION("Valid reference count", _referenceCount == 1);
}

  // @mfunc Destructor.
OMRawStorageLockBytes::~OMRawStorageLockBytes(void)
{
  TRACE("OMRawStorageLockBytes::~OMRawStorageLockBytes");

  PRECONDITION("Valid reference count", _referenceCount == 0);

  _rawStorage = 0;
}

  // @mfunc Read bytes (see Microsoft documentation for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::ReadAt(ULARGE_INTEGER ulOffset,
                              void *pv,
                              ULONG cb,
                              ULONG *pcbRead)
{
  TRACE("OMRawStorageLockBytes::ReadAt");

  _rawStorage->setPosition(toOMUInt64(ulOffset));
  OMUInt32 bytesRead = 0;
  _rawStorage->read(static_cast<OMByte*>(pv),
                    cb,
                    bytesRead);
  *pcbRead = bytesRead;
  return NOERROR;
}

  // @mfunc Write bytes (see Microsoft documentation for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::WriteAt(ULARGE_INTEGER ulOffset,
                               const void *pv,
                               ULONG cb,
                               ULONG *pcbWritten)
{
  TRACE("OMRawStorageLockBytes::WriteAt");

  _rawStorage->setPosition(toOMUInt64(ulOffset));
  OMUInt32 bytesWritten = 0;
  _rawStorage->write(static_cast<const OMByte*>(pv),
                     cb,
                     bytesWritten);
  *pcbWritten = bytesWritten;
  return NOERROR;
}

  // @mfunc Flush any buffered bytes (see Microsoft documentation
  //        for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::Flush(void)
{
  TRACE("OMRawStorageLockBytes::Flush");

  _rawStorage->synchronize();
  return NOERROR;
}

  // @mfunc Set the size, either grow or shrink (see Microsoft
  //        documentation for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::SetSize(ULARGE_INTEGER cb)
{
  TRACE("OMRawStorageLockBytes::SetSize");

  _rawStorage->setSize(toOMUInt64(cb));
  return NOERROR;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::LockRegion(ULARGE_INTEGER /* libOffset */,
                                  ULARGE_INTEGER /* cb */,
                                  DWORD /* dwLockType */)
{
  TRACE("OMRawStorageLockBytes::LockRegion");

  // TBS
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::UnlockRegion(ULARGE_INTEGER /* libOffset */,
                                    ULARGE_INTEGER /* cb */,
                                    DWORD /* dwLockType */)
{
  TRACE("OMRawStorageLockBytes::UnlockRegion");

  // TBS
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::Stat(STATSTG *pstatstg,
                            DWORD /* grfStatFlag */)
{
  TRACE("OMRawStorageLockBytes::Stat");

  // TBS
  memset(pstatstg, 0, sizeof(STATSTG));
  return NOERROR;
}
