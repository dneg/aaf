//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMSSSSTRUCTUREDSTORAGE_H
#define OMSSSSTRUCTUREDSTORAGE_H
#include "OMMSStructuredStorage.h"
#include "OMRawStorage.h"
#include "OMFile.h"
#include "StructuredStorage.h"

class OMSSIStorage : public IStorage
{
	public:
	// function to create root storage in a compound file
	static HRESULT STDMETHODCALLTYPE StgCreateStorageEx(
								const TCHAR FAR* in_filename,
								const OMFile::OMAccessMode in_accessMode,
								void **out_storage,
								unsigned long in_sectorSize);


	// function to open a compound file
	static HRESULT STDMETHODCALLTYPE StgOpenStorageEx(
								const TCHAR FAR* in_filename,
								const OMFile::OMAccessMode in_accessMode,
								void **out_storage);


	// function to create root storage in raw storage
	static HRESULT STDMETHODCALLTYPE StgCreateStorageInOMRawStorage(
								const OMRawStorage* in_pRaw,
								const OMFile::OMAccessMode in_accessMode,
								void** out_storage,
								unsigned long in_sectorSize);

	// function to open root storage in raw storage
	static HRESULT STDMETHODCALLTYPE StgOpenStorageInOMRawStorage(
								const OMRawStorage* in_pRaw,
								const OMFile::OMAccessMode in_accessMode,
								void** out_storage);

	// @access Public members
public:
	// IUnknown interface
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);

	virtual ULONG STDMETHODCALLTYPE Release(void);

	// IStorage interface
	virtual HRESULT STDMETHODCALLTYPE CreateStream(
										const TCHAR FAR* pwcsName,
										DWORD grfMode,
										DWORD reserved1,
										DWORD reserved2,
										IStream FAR *FAR *ppstm
										);

	virtual HRESULT STDMETHODCALLTYPE OpenStream (
										const TCHAR FAR* pwcsName,
										void FAR *reserved1,
										DWORD grfMode,
										DWORD reserved2,
										IStream FAR *FAR *ppstm
										);

	virtual HRESULT STDMETHODCALLTYPE CreateStorage(
										const TCHAR FAR* pwcsName,
										DWORD grfMode,
										DWORD reserved1,
										DWORD reserved2,
										IStorage FAR *FAR *ppstg
										);

	virtual HRESULT STDMETHODCALLTYPE OpenStorage (
										const TCHAR FAR* pwcsName,
										IStorage FAR *pstgPriority,
										DWORD grfMode,
										SNB snbExclude,
										DWORD reserved,
										IStorage FAR *FAR *ppstg
										);

	virtual HRESULT STDMETHODCALLTYPE CopyTo(
										DWORD ciidExclude,
										IID const FAR *rgiidExclude,
										SNB snbExclude,
										IStorage FAR *pstgDest);

	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(
										TCHAR const FAR* lpszName,
										IStorage FAR *pstgDest,
										TCHAR const FAR* lpszNewName,
										DWORD grfFlags);

	virtual HRESULT STDMETHODCALLTYPE Commit (DWORD grfCommitFlags);

	virtual HRESULT STDMETHODCALLTYPE Revert (void);

	virtual HRESULT STDMETHODCALLTYPE EnumElements(
										DWORD reserved1,
										void FAR *reserved2,
										DWORD reserved3,
										IEnumSTATSTG FAR *FAR *ppenm);

	virtual HRESULT STDMETHODCALLTYPE DestroyElement(const TCHAR FAR* pwcsName);

	virtual HRESULT STDMETHODCALLTYPE RenameElement(
										const TCHAR FAR* pwcsOldName,
										const TCHAR FAR* pwcsNewName
										);

	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(
										const TCHAR FAR *lpszName,
										FILETIME const FAR *pctime,
										FILETIME const FAR *patime,
										FILETIME const FAR *pmtime
										);

	virtual HRESULT STDMETHODCALLTYPE SetClass (REFCLSID clsid);

	virtual HRESULT STDMETHODCALLTYPE SetStateBits(
										DWORD grfStateBits,
										DWORD grfMask
										);

	virtual HRESULT STDMETHODCALLTYPE Stat(
										STATSTG FAR *pstatstg,
										DWORD grfStatFlag
										);

protected:
	// Make default contructor private so that an OMSSIStorage object cannot
	// be instantiated
	OMSSIStorage(RootStorage *in_root, Storage *in_storage);

	// keeping destructor protected means that the object must always be
	// instantiated on the heap;
	virtual ~OMSSIStorage();

private:
	RootStorage*	_root;
	Storage*		_storage;
	ULONG 			_referenceCount;
};


class OMSSIStream : public IStream
{
public:
	// public constructor
	OMSSIStream(Stream *in_stream);
	// IUnknown interface
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);

	virtual ULONG STDMETHODCALLTYPE Release(void);

	// IStream interface
	virtual HRESULT STDMETHODCALLTYPE Read(
										VOID HUGEP *pv,
										ULONG cb,
										ULONG FAR *pcbRead
										);

	virtual HRESULT STDMETHODCALLTYPE Write(
										VOID const HUGEP *pv,
										ULONG cb,
										ULONG FAR *pcbWritten
										);

	virtual HRESULT STDMETHODCALLTYPE Seek(
										LARGE_INTEGER dlibMove,
										DWORD dwOrigin,
										ULARGE_INTEGER FAR *plibNewPosition
										);

	virtual HRESULT STDMETHODCALLTYPE SetSize(
										 ULARGE_INTEGER libNewSize
										 );

	virtual HRESULT STDMETHODCALLTYPE CopyTo(
										IStream FAR *pstm,
										ULARGE_INTEGER cb,
										ULARGE_INTEGER FAR *pcbRead,
										ULARGE_INTEGER FAR *pcbWritten);

	virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);

	virtual HRESULT STDMETHODCALLTYPE Revert(void);

	virtual HRESULT STDMETHODCALLTYPE LockRegion(
										ULARGE_INTEGER libOffset,
										ULARGE_INTEGER cb,
										DWORD dwLockType
										);

	virtual HRESULT STDMETHODCALLTYPE UnlockRegion(
										ULARGE_INTEGER libOffset,
										ULARGE_INTEGER cb,
										DWORD dwLockType
										);

	virtual HRESULT STDMETHODCALLTYPE Stat(
										STATSTG FAR *pstatstg,
										DWORD grfStatFlag
										);

	virtual HRESULT STDMETHODCALLTYPE Clone(IStream FAR * FAR *ppstm);


protected:

	// Make destructor virtual so that this object can only be instantiated
	// on the heap
	virtual ~OMSSIStream();

private:
	Stream*			_stream;
	ULONG 			_referenceCount;
};
#endif
