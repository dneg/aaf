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
//
//=---------------------------------------------------------------------=
#include "OMAssertions.h"
#include "OMSSSStructuredStorage.h"
#include "StructuredStorage.h"
#include "OMSSSStorageRWInputSource.h"
#include "OMUtilities.h"

typedef int sresult;

static inline HRESULT makeStatus(sresult status)
{
  TRACE("makeStatus");

  if (SSTG_OK!=status) {
    return (status | 0x80000000L);
  }
  return SSTG_OK;
}

static void convertToSTATSTG(StgInfo *info, STATSTG *pstatstg, DWORD grfStatFlag)
{
	TRACE("convertToSTATSTG");
	memset(pstatstg, 0, sizeof(STATSTG));
	
	//FIXME ignoring fields that are not used by the SDK

	if (!(grfStatFlag & STATFLAG_NONAME))
	{
#ifdef OM_UNICODE_APIS
		pstatstg->pwcsName = saveWideString(info->sName);
#else
		pstatstg->pwcsName = convertWideString(info->sName);
#endif
	}
	pstatstg->type = info->type;
	pstatstg->cbSize = fromOMUInt64(info->size);
	convertUnixTimeToFileTime (info->modificationTime, &pstatstg->mtime);
	convertUnixTimeToFileTime (info->creationTime, &pstatstg->ctime);
	//pstatstg->atime
	switch (info->accessMode)
	{
	case STG_READ:
		pstatstg->grfMode |= STGM_READ;
		break;
	case STG_RW:
		pstatstg->grfMode |= STGM_READWRITE;
		break;
	case STG_WRITE:
		pstatstg->grfMode |= STGM_WRITE;
		break;
	default:
		ASSERT("Known  SS access mode", false);
		break;
	}
	//pstatstg->grfLocksSupported
	memcpy (&pstatstg->clsid, &info->clsid, sizeof(pstatstg->clsid));
	//pstatstg->grfStateBits
	//pstatstg->reserved
}

//SSSS Storage
OMSSIStorage::OMSSIStorage(RootStorage *in_root, Storage *in_storage) :
	_root(in_root), _storage(in_storage), _referenceCount(1)
{
	TRACE("OMSSIStorage::OMSSIStorage");
	PRECONDITION("Valid reference count", _referenceCount == 1);
};

OMSSIStorage::~OMSSIStorage()
{
	TRACE("OMSSIStorage::~OMSSIStorage");
	PRECONDITION("Valid reference count", _referenceCount == 0);
	PRECONDITION("Root storage closed", _root == 0);
	PRECONDITION("Non-Root storage closed", _storage == 0);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::QueryInterface(REFIID /* riid */, void** ppvObject)
{
	TRACE("OMSSIStorage::QueryInterface");
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
OMSSIStorage::AddRef(void)
{
	TRACE("OMSSIStorage::AddRef");
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE
OMSSIStorage::Release(void)
{
	TRACE("OMSSIStorage::Release");
	ULONG result = --_referenceCount;
	if (_referenceCount == 0)
	{
		if (_root != 0)
		{
			//close the root storage
			::closeStructuredStorage(&_root);
			_root = 0;
			_storage = 0;
		}
		else if (_storage != 0)
		{
			//close non-root storage
			::closeStorage(&_storage);
			_storage = 0;
		}
		delete this; 
	}
	return result;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::StgCreateStorageEx( const TCHAR FAR* in_filename,
				const OMFile::OMAccessMode in_accessMode,
				void **out_storage,
				unsigned long in_sectorSize)
{
	TRACE("OMSSIStorage::StgCreateStorageEx");
	PRECONDITION("Valid mode", (in_accessMode == OMFile::modifyMode));

	RootStorage* rootstorage = 0;
	Storage *storage = 0;
	*out_storage = 0;
	wchar_t storageName[FILENAME_MAX];

#ifndef OM_UNICODE_APIS
	convertStringToWideString(storageName, in_filename, FILENAME_MAX);
#else
	copyWideString (storageName, in_filename, FILENAME_MAX);
#endif
	sresult status = createStructuredStorageEx(
						storageName,
						STG_RW,
						&rootstorage,
						in_sectorSize);
	
	if (status == SSTG_OK)
	{
		status =::getStorageFromRoot( rootstorage, &storage );
		if (status == SSTG_OK)
		{
			OMSSIStorage *newStorage = new OMSSIStorage (rootstorage, storage);
			*out_storage = newStorage;
		}
	}

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::StgOpenStorageEx( const TCHAR FAR* in_filename,
				const OMFile::OMAccessMode in_accessMode,
				void **out_storage)
{
	TRACE("OMSSIStorage::StgOpenStorageEx");
	PRECONDITION("Valid mode", (in_accessMode == OMFile::modifyMode) ||
                             (in_accessMode == OMFile::readOnlyMode));

	StgMode openMode;
  	if (in_accessMode == OMFile::modifyMode) {
    	openMode = STG_RW;
  	} else if (in_accessMode == OMFile::readOnlyMode) {
    	openMode = STG_READ;
  	}
	RootStorage* rootstorage = 0;
	Storage *storage = 0;
	*out_storage = 0;
	wchar_t storageName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	convertStringToWideString(storageName, in_filename, FILENAME_MAX);
#else
	copyWideString (storageName, in_filename, FILENAME_MAX);
#endif
	sresult status = openStructuredStorageEx(
						storageName,
						openMode,
						&rootstorage);
	
	if (status == SSTG_OK)
	{
		status =::getStorageFromRoot( rootstorage, &storage );
		if (status == SSTG_OK)
		{
			OMSSIStorage *newStorage = new OMSSIStorage (rootstorage, storage);
			*out_storage = newStorage;
		}
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::StgCreateStorageInOMRawStorage(
				const OMRawStorage* in_pRaw,
				const OMFile::OMAccessMode in_accessMode,
				void** out_storage,
				unsigned long in_sectorSize)
{
	TRACE("OMSSIStorage::StgCreateStorageInOMRawStorage");
	PRECONDITION("Valid mode", (in_accessMode == OMFile::modifyMode));
	RootStorage* rootstorage = 0;
	Storage *storage = 0;
	*out_storage = 0;
	sresult status = createStructuredStorageInOMRawStorageEx(
						in_pRaw,
						STG_RW,
						&rootstorage,
						in_sectorSize);
	
	if (status == SSTG_OK)
	{
		status =::getStorageFromRoot( rootstorage, &storage );
		if (status == SSTG_OK)
		{
			OMSSIStorage *newStorage = new OMSSIStorage (rootstorage, storage);
			*out_storage = newStorage;
		}
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::StgOpenStorageInOMRawStorage(
				const OMRawStorage* in_pRaw,
				const OMFile::OMAccessMode in_accessMode,
				void** out_storage)
{
	TRACE("OMSSIStorage::StgOpenStorageInOMRawStorage");
	PRECONDITION("Valid mode", (in_accessMode == OMFile::modifyMode) ||
                             (in_accessMode == OMFile::readOnlyMode));

	StgMode openMode;
  	if (in_accessMode == OMFile::modifyMode) {
    	openMode = STG_RW;
  	} else if (in_accessMode == OMFile::readOnlyMode) {
    	openMode = STG_READ;
  	}
	RootStorage* rootstorage = 0;
	Storage *storage = 0;
	*out_storage = 0;
	sresult status = openStructuredStorageInOMRawStorage(
						in_pRaw,
						openMode,
						&rootstorage);
	
	if (status == SSTG_OK)
	{
		status =::getStorageFromRoot( rootstorage, &storage );
		if (status == SSTG_OK)
		{
			OMSSIStorage *newStorage = new OMSSIStorage (rootstorage, storage);
			*out_storage = newStorage;
		}
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::CreateStream(
				const TCHAR FAR* pwcsName,
				DWORD grfMode,
				DWORD reserved1,
				DWORD reserved2,
				IStream FAR *FAR *ppstm)
{
	TRACE("OMSSIStorage::CreateStream");
	//FIXME: not interpreting grfMode

	Stream *stream;
	sresult status = SSTG_OK;
	wchar_t streamName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	convertStringToWideString(streamName, pwcsName, FILENAME_MAX);
#else
	copyWideString (streamName, pwcsName, FILENAME_MAX);
#endif
	if( (status = ::openStream( _storage, streamName, &stream)) == SSTG_OK )
	{
		if ((status = ::streamSeek( stream, 0L, STG_START )) == SSTG_OK)
		{
			OMSSIStream *newStream = new OMSSIStream (stream);
			*ppstm = newStream;
		}
	}
	else if( (status =::createStream( _storage, streamName, &stream)) == SSTG_OK )
	{
		OMSSIStream *newStream = new OMSSIStream (stream);
		*ppstm = newStream;
	}

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::OpenStream(
				const TCHAR FAR* pwcsName,
				void FAR *reserved1,
				DWORD grfMode,
				DWORD reserved2,
				IStream FAR *FAR *ppstm)
{
	TRACE("OMSSIStorage::OpenStream");
	//FIXME: not interpreting grfMode

	Stream *stream;
	sresult status = SSTG_OK;
	wchar_t streamName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	convertStringToWideString(streamName, pwcsName, FILENAME_MAX);
#else
	copyWideString (streamName, pwcsName, FILENAME_MAX);
#endif
	if( (status = ::openStream( _storage, streamName, &stream)) == SSTG_OK )
	{
		if ((status = ::streamSeek( stream, 0L, STG_START )) == SSTG_OK)
		{
			OMSSIStream *newStream = new OMSSIStream (stream);
			*ppstm = newStream;
		}
	}

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::CreateStorage(
				const TCHAR FAR* pwcsName,
				DWORD grfMode,
				DWORD reserved1,
				DWORD reserved2,
				IStorage FAR *FAR *ppstg)
{
	TRACE("OMSSIStorage::CreateStorage");
	//FIXME: not interpreting grfMode
	sresult status = SSTG_OK;
	Storage *storage;
	wchar_t storageName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	convertStringToWideString(storageName, pwcsName, FILENAME_MAX);
#else
	copyWideString (storageName, pwcsName, FILENAME_MAX);
#endif

	//If storage already exists reopen, else create new storage
	if( 
		SSTG_OK == (status = ::openStorage( _storage, storageName, &storage)) ||
		SSTG_OK == (status = ::createStorage( _storage, storageName, &storage))
	  )
	{
		OMSSIStorage *newStorage = new OMSSIStorage (0, storage);
		*ppstg = newStorage;
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::OpenStorage(
				const TCHAR FAR* pwcsName,
				IStorage FAR *pstgPriority,
				DWORD grfMode,
				SNB snbExclude,
				DWORD reserved,
				IStorage FAR *FAR *ppstg)
{
	TRACE("OMSSIStorage::OpenStorage");
	//FIXME: not interpreting grfMode
	sresult status = SSTG_OK;
	Storage *storage;
	wchar_t storageName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	convertStringToWideString(storageName, pwcsName, FILENAME_MAX);
#else
	copyWideString (storageName, pwcsName, FILENAME_MAX);
#endif

	//If storage already exists reopen, else create new storage
	if( SSTG_OK == (status = ::openStorage( _storage, storageName, &storage)) )
	{
		OMSSIStorage *newStorage = new OMSSIStorage (0, storage);
		*ppstg = newStorage;
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::CopyTo(
				DWORD ciidExclude,
				IID const FAR *rgiidExclude,
				SNB snbExclude,
				IStorage FAR *pstgDest)
{
	TRACE("OMSSIStorage::CopyTo");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE 
OMSSIStorage::MoveElementTo(
				TCHAR const FAR* lpszName,
				IStorage FAR *pstgDest,
				TCHAR const FAR* lpszNewName,
				DWORD grfFlags)
{
	TRACE("OMSSIStorage::MoveElementTo");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::Commit (DWORD grfCommitFlags)
{
	TRACE("OMSSIStorage::Commit");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::Revert (void)
{
	TRACE("OMSSIStorage::Revert");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::EnumElements(
				DWORD reserved1,
				void FAR *reserved2,
				DWORD reserved3,
				IEnumSTATSTG FAR *FAR *ppenm)
{
	TRACE("OMSSIStorage::EnumElements");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::DestroyElement(const TCHAR FAR* pwcsName)
{
	TRACE("OMSSIStorage::DestroyElement");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::RenameElement(
				const TCHAR FAR* pwcsOldName,
				const TCHAR FAR* pwcsNewName
			)
{
	TRACE("OMSSIStorage::RenameElement");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::SetElementTimes(
				const TCHAR FAR *lpszName,
				FILETIME const FAR *pctime,
				FILETIME const FAR *patime,
				FILETIME const FAR *pmtime
										)
{
	TRACE("OMSSIStorage::SetElementTimes");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::SetClass (REFCLSID clsid)
{
	TRACE("OMSSIStorage::SetClass");
	sresult status = ::storageSetClass( _storage, reinterpret_cast<const SSRW_GUID *>(&clsid) );

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::SetStateBits(
				DWORD grfStateBits,
				DWORD grfMask
				)
{
	TRACE("OMSSIStorage::SetStateBits");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStorage::Stat(
				STATSTG FAR *pstatstg,
				DWORD grfStatFlag
				)
{
	TRACE("OMSSIStorage::DestroyElement");
	StgInfo* info;
	sresult status = getStorageInfo( _storage, &info );
	if (status == SSTG_OK)
	{
		convertToSTATSTG(info, pstatstg, grfStatFlag);
		status = freeInfo(&info, 1);
	}
	return makeStatus(status);
}

//SSSS streams
OMSSIStream::OMSSIStream (Stream *in_stream) : 
	_stream(in_stream), _referenceCount(1)
{
	TRACE("OMSSIStream::OMSSIStream");
	PRECONDITION("Valid reference count", _referenceCount == 1);
}

OMSSIStream::~OMSSIStream ()
{
	TRACE("OMSSIStream::~OMSSIStream");
	PRECONDITION("Valid reference count", _referenceCount == 0);
	PRECONDITION("Stream has been closed", _stream == 0);
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::QueryInterface(REFIID /* riid */, void** ppvObject)
{
	TRACE("OMSSIStream::QueryInterface");
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
OMSSIStream::AddRef(void)
{
	TRACE("OMSSIStream::AddRef");
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE
OMSSIStream::Release(void)
{
	TRACE("OMSSIStream::Release");
	ULONG result = --_referenceCount;
	sresult status = SSTG_OK;
	if (_referenceCount == 0)
	{
		if (_stream != 0)
		{
			OMUInt64 p, sz;
			// save position for later test
			status = streamGetPos64(_stream, &p);
			// seek to the end of the stream
			// SchemaSoft resizes the stream upon close
			status = streamSeek64( _stream, 0, STG_END );
	
			// test position
			status = streamGetPos64(_stream, &sz);
	
			if( p != sz )
				OMUInt64 err = sz-p;
	
			status = ::closeStream(&_stream);
			_stream = 0;
		}
		delete this;
	}
	return status;
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::Read(
				VOID HUGEP *pv,
				ULONG cb,
				ULONG FAR *pcbRead
				)
{
	TRACE("OMSSIStream::Read");
	*pcbRead = cb;
	sresult result = streamRead( _stream, pv, pcbRead);

	// Dealing with end of stream cases:
	// 1. Read starts and ends before the end of the stream.
	// 2. Read starts before and ends past the end of the stream.
	// 3. Read starts and ends past the end of the stream.
	// Microsoft Structured Storage implementation of IStream::Read()
	// returns S_OK in all 3 cases setting the number of bytes
	// actually read in pcbRead.
	// Schemasoft's streamRead() behaves differently in the 3rd
	// case for which it returns an error (SSTG_ERROR_END_OF_STREAM).
	// Here the error is overwritten to keep the behavior of
	// IStream::Read() identical across all implementations.
	if (result == SSTG_ERROR_END_OF_STREAM)
	{
		if (*pcbRead == 0)
		{
			result = SSTG_OK;
		}
	}
	return makeStatus(result);
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::Write(
				VOID const HUGEP *pv,
				ULONG cb,
				ULONG FAR *pcbWritten
										)
{
	TRACE("OMSSIStream::Write");
	*pcbWritten = cb;
	sresult result = streamWrite( _stream, pv, pcbWritten);
	return makeStatus(result);
}

HRESULT STDMETHODCALLTYPE 
OMSSIStream::Seek(
				LARGE_INTEGER dlibMove,
				DWORD dwOrigin,
				ULARGE_INTEGER FAR *plibNewPosition
				)
{
	TRACE("OMSSIStream::Seek");
	SeekPos pos;

	switch (dwOrigin)
	{
	case STREAM_SEEK_CUR:
		pos = STG_CUR;
		break;
	case STREAM_SEEK_END:
		pos = STG_END;
		break;
	case STREAM_SEEK_SET:
	default:
		pos = STG_START;
		break;
	}
	ULARGE_INTEGER offset;
	memcpy (&offset, &dlibMove, sizeof(LARGE_INTEGER));
	sresult status = streamSeek64( _stream, toOMUInt64(offset), pos);
	if (status == SSTG_OK && plibNewPosition != 0)
	{
		OMUInt64 newPos = 0;
		status = streamGetPos64 (_stream, &newPos);
		*plibNewPosition = fromOMUInt64(newPos);
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::SetSize( ULARGE_INTEGER libNewSize)
{
	TRACE("OMSSIStream::SetSize");
	OMUInt64 newSize = toOMUInt64(libNewSize);
	sresult status = streamResize( _stream, newSize );

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::CopyTo(
				IStream FAR *pstm,
				ULARGE_INTEGER cb,
				ULARGE_INTEGER FAR *pcbRead,
				ULARGE_INTEGER FAR *pcbWritten)
{
	TRACE("OMSSIStream::SetSize");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::Commit(DWORD grfCommitFlags)
{
	TRACE("OMSSIStream::Commit");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::Revert(void)
{
	TRACE("OMSSIStream::Revert");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::LockRegion(
				ULARGE_INTEGER libOffset,
				ULARGE_INTEGER cb,
				DWORD dwLockType)
{
	TRACE("OMSSIStream::LockRegion");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::UnlockRegion(
				ULARGE_INTEGER libOffset,
				ULARGE_INTEGER cb,
				DWORD dwLockType)
{
	TRACE("OMSSIStream::LockRegion");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMSSIStream::Stat(
				STATSTG FAR *pstatstg,
				DWORD grfStatFlag)
{
	TRACE("OMSSIStream::Stat");
	StgInfo* info;
	sresult status = getStreamInfo( _stream, &info );
	if (status == SSTG_OK)
	{
		//apparently, info->size isn't updated after write()
		// these four lines get the actual end of stream in sz
		OMUInt64 p, sz;
		if ( SSTG_OK == (status = streamGetPos64 ( _stream, &p)) &&
		     SSTG_OK == (status = streamSeek64( _stream, 0, STG_END )))
		{
			if (SSTG_OK == (status = streamGetPos64 ( _stream, &sz)) &&
				SSTG_OK ==  (status = streamSeek64( _stream, p, STG_START )))
			{
				info->size = sz;
				convertToSTATSTG(info, pstatstg, grfStatFlag);
				status = freeInfo(&info, 1);
			}
		}
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE 
OMSSIStream::Clone(IStream FAR * FAR *ppstm)
{
	TRACE("OMSSIStream::Clone");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

