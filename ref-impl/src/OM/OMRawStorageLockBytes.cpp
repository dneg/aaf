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

  // @mfunc Constructor.
OMRawStorageLockBytes::OMRawStorageLockBytes(OMRawStorage* rawStorage)
{
  TRACE("OMRawStorageLockBytes::OMRawStorageLockBytes");

  // TBS
}

  // @mfunc Destructor.
OMRawStorageLockBytes::~OMRawStorageLockBytes(void)
{
  TRACE("OMRawStorageLockBytes::~OMRawStorageLockBytes");

  // TBS
}

  // @mfunc Read bytes (see Microsoft documentation for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::ReadAt(ULARGE_INTEGER ulOffset,
                              void *pv,
                              ULONG cb,
                              ULONG *pcbRead)
{
  TRACE("OMRawStorageLockBytes::ReadAt");

  // TBS
  return E_FAIL;
}

  // @mfunc Write bytes (see Microsoft documentation for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::WriteAt(ULARGE_INTEGER ulOffset,
                               const void *pv,
                               ULONG cb,
                               ULONG *pcbWritten)
{
  TRACE("OMRawStorageLockBytes::WriteAt");

  // TBS
  return E_FAIL;
}

  // @mfunc Flush any buffered bytes (see Microsoft documentation
  //        for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::Flush(void)
{
  TRACE("OMRawStorageLockBytes::Flush");

  // TBS
  return E_FAIL;
}

  // @mfunc Set the size, either grow or shrink (see Microsoft
  //        documentation for details).
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::SetSize(ULARGE_INTEGER cb)
{
  TRACE("OMRawStorageLockBytes::SetSize");

  // TBS
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::LockRegion(ULARGE_INTEGER libOffset,
                                  ULARGE_INTEGER cb,
                                  DWORD dwLockType)
{
  TRACE("OMRawStorageLockBytes::LockRegion");

  // TBS
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::UnlockRegion(ULARGE_INTEGER libOffset,
                                    ULARGE_INTEGER cb,
                                    DWORD dwLockType)
{
  TRACE("OMRawStorageLockBytes::UnlockRegion");

  // TBS
  return E_FAIL;
}

  // @mfunc See Microsoft documentation for details.
HRESULT STDMETHODCALLTYPE
OMRawStorageLockBytes::Stat(STATSTG *pstatstg,
                            DWORD grfStatFlag)
{
  TRACE("OMRawStorageLockBytes::Stat");

  // TBS
  return E_FAIL;
}
