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
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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
