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
#ifndef OMBUFFEREDISTREAM_H
#define OMBUFFEREDISTREAM_H

#include "OMMSStructuredStorage.h"
#include "OMBufferedStream.h"
#include "OMDataTypes.h"

struct IStream;

  // @class IStream with buffering.
  //
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMBufferedIStream : public IStream, public OMBufferedStream {
public:
  // @access Public members.

  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
                                                   void** ppvObject);

  virtual ULONG STDMETHODCALLTYPE AddRef(void);

  virtual ULONG STDMETHODCALLTYPE Release(void);

  virtual HRESULT STDMETHODCALLTYPE Read(void *pv,
                                         ULONG cb,
                                         ULONG *pcbRead);

  virtual HRESULT STDMETHODCALLTYPE Write(const void *pv,
                                          ULONG cb,
                                          ULONG *pcbWritten);

  virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove,
                                         DWORD dwOrigin,
                                         ULARGE_INTEGER *plibNewPosition);

  virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize);

  virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream *pstm,
                                           ULARGE_INTEGER cb,
                                           ULARGE_INTEGER *pcbRead,
                                           ULARGE_INTEGER *pcbWritten);

  virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);

  virtual HRESULT STDMETHODCALLTYPE Revert(void);

  virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset,
                                               ULARGE_INTEGER cb,
                                               DWORD dwLockType);

  virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset,
                                                 ULARGE_INTEGER cb,
                                                 DWORD dwLockType);

  virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG *pstatstg,
                                         DWORD grfStatFlag);

  virtual HRESULT STDMETHODCALLTYPE Clone(IStream **ppstm);

    // @cmember Constructor.
  OMBufferedIStream(IStream* stream, size_t bufferSize);

    // @cmember Destructor.
  virtual ~OMBufferedIStream(void);

private:
  // @access Private members.

  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead);

  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

  IStream* _stream;
  ULONG _referenceCount;

};

#endif
