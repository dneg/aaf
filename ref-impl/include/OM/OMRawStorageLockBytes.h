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
#ifndef OMRAWSTORAGELOCKBYTES_H
#define OMRAWSTORAGELOCKBYTES_H

#include "OMMSStructuredStorage.h"

class OMRawStorage;

  // @class An implementation of the Microsoft Structured Storage interface
  //        ILockBytes in terms of <c OMRawStorage>.
  //        This class is an adapter from the ILockBytes interface to the
  //        exported <c OMRawStorage> interface. Object Manager clients
  //        may use the <c OMRawStorage> interface to access or to control
  //        the storage of the raw bytes of a file.
  //   @base public | ILockBytes
class OMRawStorageLockBytes : public ILockBytes {
public:
  // @access Public members.

  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
                                                   void** ppvObject);

  virtual ULONG STDMETHODCALLTYPE AddRef(void);

  virtual ULONG STDMETHODCALLTYPE Release(void);

    // @cmember Constructor.
  OMRawStorageLockBytes(OMRawStorage* rawStorage);

    // @cmember Destructor.
  virtual ~OMRawStorageLockBytes(void);

    // @cmember Read bytes (see Microsoft documentation for details).
  virtual HRESULT STDMETHODCALLTYPE ReadAt(ULARGE_INTEGER ulOffset,
                                           void *pv,
                                           ULONG cb,
                                           ULONG *pcbRead);

    // @cmember Write bytes (see Microsoft documentation for details).
  virtual HRESULT STDMETHODCALLTYPE WriteAt(ULARGE_INTEGER ulOffset,
                                            const void *pv,
                                            ULONG cb,
                                            ULONG *pcbWritten);

    // @cmember Flush any buffered bytes (see Microsoft documentation
    //          for details).
  virtual HRESULT STDMETHODCALLTYPE Flush(void);

    // @cmember Set the size, either grow or shrink (see Microsoft
    //          documentation for details).
  virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER cb);

    // @cmember See Microsoft documentation for details.
  virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset,
                                               ULARGE_INTEGER cb,
                                               DWORD dwLockType);

    // @cmember See Microsoft documentation for details.
  virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset,
                                                 ULARGE_INTEGER cb,
                                                 DWORD dwLockType);

    // @cmember See Microsoft documentation for details.
  virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG *pstatstg,
                                         DWORD grfStatFlag);

private:
  // @access Private members.

  OMRawStorage* _rawStorage;
  ULONG _referenceCount;

};

#endif
