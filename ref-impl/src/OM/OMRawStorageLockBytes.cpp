//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMRawStorageLockBytes

#include "OMRawStorageLockBytes.h"

#include "OMAssertions.h"
#include "OMMSStructuredStorage.h"

#include "OMRawStorage.h"

// The Object Manager expects only one thread per file.
// This implementation of the ILockBytes interface is designed for
// single process (aka in proc) and single thread use only.

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

  ULONG result = --_referenceCount;
  if (_referenceCount == 0) {
    delete this;
  }
  return result;
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

  OMUInt32 bytesRead = 0;
  _rawStorage->readAt(toOMUInt64(ulOffset),
                      static_cast<OMByte*>(pv),
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

  OMUInt32 bytesWritten = 0;
  _rawStorage->writeAt(toOMUInt64(ulOffset),
                       static_cast<const OMByte*>(pv),
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

  _rawStorage->extend(toOMUInt64(cb));
  return NOERROR;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::LockRegion(ULARGE_INTEGER /* libOffset */,
                                  ULARGE_INTEGER /* cb */,
                                  DWORD /* dwLockType */)
{
  TRACE("OMRawStorageLockBytes::LockRegion");

  // Function not supported.
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::UnlockRegion(ULARGE_INTEGER /* libOffset */,
                                    ULARGE_INTEGER /* cb */,
                                    DWORD /* dwLockType */)
{
  TRACE("OMRawStorageLockBytes::UnlockRegion");

  // Function not supported.
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::Stat(STATSTG *pstatstg,
                            DWORD /* grfStatFlag */)
{
  TRACE("OMRawStorageLockBytes::Stat");

  // TBS tjb
  memset(pstatstg, 0, sizeof(STATSTG));
  return NOERROR;
}
