//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Schema Software Inc.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMGSFSTRUCTUREDSTORAGE_H
#define OMGSFSTRUCTUREDSTORAGE_H

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMMSStructuredStorage.h"

#if defined(OM_USE_GSF_SS)

#include "OMRawStorage.h"
#include "OMFile.h"

// TODO: revisit all these typedefs
typedef OMInt32 HRESULT;

typedef enum { GSF_READ, GSF_WRITE, GSF_READWRITE } GsfAccessMode;

typedef void GsfStorage;
typedef void GsfStream;

#define GSTG_OK 0
#define GSTG_ERROR 1

class OMGSFIStorage : public IStorage
{
	public:
	// function to create root storage in a compound file
	static HRESULT STDMETHODCALLTYPE StgCreateStorageEx(
								const TCHAR FAR* in_filename,
								OMFile::OMAccessMode in_accessMode,
								void **out_storage,
								ULONG in_sectorSize);

	// function to open a compound file
	static HRESULT STDMETHODCALLTYPE StgOpenStorageEx(
								const TCHAR FAR* in_filename,
								OMFile::OMAccessMode in_accessMode,
								void **out_storage);

	// function to create root storage in raw storage
	static HRESULT STDMETHODCALLTYPE StgCreateStorageInOMRawStorage(
								const OMRawStorage* in_pRaw,
								OMFile::OMAccessMode in_accessMode,
								void** out_storage,
								ULONG in_sectorSize);

	// function to open root storage in raw storage
	static HRESULT STDMETHODCALLTYPE StgOpenStorageInOMRawStorage(
								const OMRawStorage* in_pRaw,
								OMFile::OMAccessMode in_accessMode,
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
	// Make default contructor private so that an OMGSFIStorage object cannot
	// be instantiated
	OMGSFIStorage(GsfStorage *in_storage, GsfAccessMode _mode, const char *sname);

	// keeping destructor protected means that the object must always be
	// instantiated on the heap;
	virtual ~OMGSFIStorage();

private:
	GsfStorage*		_storage;
	GsfAccessMode	_mode;				// TODO: does gsf store this somewhere?
	ULONG 			_referenceCount;
	char 			_storageName[FILENAME_MAX];	// TODO: does gsf store this somewhere?
};


class OMGSFIStream : public IStream
{
public:
	// public constructor
	OMGSFIStream(GsfStream *in_stream, GsfAccessMode mode, const char *sname);

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
	virtual ~OMGSFIStream();

private:
	int Tell (OMUInt64 *position) const;
	int Seek (OMInt64 offset, DWORD whence);
	int Size (OMUInt64 *ssize) const;


private:
	GsfStream*		_stream;
	GsfAccessMode	_mode;						// TODO: does gsf store this somewhere?
	ULONG 			_referenceCount;
	char 			_streamName[FILENAME_MAX];	// TODO: does gsf store this somewhere?
};

void OMGSFInitialize();

void OMGSFFinalize();

#endif // OM_USE_GSF_SS

#endif // !OM_NO_STRUCTURED_STORAGE

#endif // !OMGSFSTRUCTUREDSTORAGE_H
