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
#include "OMGSFStructuredStorage.h"
#include "OMUtilities.h"

#ifdef __cplusplus

extern "C" {
#endif
#include <gsf/gsf-utils.h>
#include <gsf/gsf-input-stdio.h>
#include <gsf/gsf-infile.h>
#include <gsf/gsf-input-impl.h>
#include <gsf/gsf-infile-msole.h>
#include <gsf/gsf-output-stdio.h>
#include <gsf/gsf-outfile.h>
#include <gsf/gsf-outfile-msole.h>
#ifdef __cplusplus
}
#endif


typedef struct tag_GSF_GUID
{
    unsigned int Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GSF_GUID;

typedef GSF_GUID GSF_CLSID;

static inline HRESULT makeStatus(int status)
{
  TRACE("makeStatus");

  if (GSTG_OK!=status) {
    return (status | 0x80000000L);
  }
  return GSTG_OK;
}

static int _GSFIsInitialized; // automatically initialized to 0

void OMGSFInitialize(void)
{
  if (!_GSFIsInitialized)
  {
    gsf_init();
    _GSFIsInitialized = 1;
  }
}

void OMGSFFinalize(void)
{
  if (_GSFIsInitialized)
  {
    gsf_shutdown ();
    _GSFIsInitialized = 0;
  }
}

// GSF Storage
OMGSFIStorage::OMGSFIStorage(GsfStorage *in_storage, GsfAccessMode mode, const char *sname) : _storage(in_storage), _mode(mode), _referenceCount(1)
{
	strcpy(_storageName, sname);
	TRACE("OMGSFIStorage::OMGSFIStorage");
	PRECONDITION("Valid reference count", _referenceCount == 1);
};

OMGSFIStorage::~OMGSFIStorage()
{
	TRACE("OMGSFIStorage::~OMGSFIStorage");
	PRECONDITION("Valid reference count", _referenceCount == 0);
	PRECONDITION("Non-Root storage closed", _storage == 0);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::QueryInterface(REFIID /* riid */, void** ppvObject)
{
	TRACE("OMGSFIStorage::QueryInterface");
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
OMGSFIStorage::AddRef(void)
{
	TRACE("OMGSFIStorage::AddRef");
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE
OMGSFIStorage::Release(void)
{
	TRACE("OMGSFIStorage::Release");
	--_referenceCount;
	if (_referenceCount == 0)
	{
		if (_storage != 0)
		{
			if (GSF_IS_OUTPUT(_storage))
			{
				gsf_output_close (GSF_OUTPUT(_storage));
			}
			g_object_unref (G_OBJECT(_storage));
			_storage = 0;
		}
		delete this;
	}
	return _referenceCount;
}


HRESULT STDMETHODCALLTYPE
OMGSFIStorage::StgCreateStorageEx( const TCHAR FAR* in_filename,
				OMFile::OMAccessMode in_accessMode,
				void **out_storage,
				ULONG in_sectorSize)
{
	TRACE("OMGSFIStorage::StgCreateStorageEx");
	PRECONDITION("Valid access mode", in_accessMode == OMFile::writeOnlyMode);
	GsfStorage *storage = 0;
	*out_storage = 0;
	char storageName[FILENAME_MAX];

#ifndef OM_UNICODE_APIS
	strncpy (storageName, in_filename, sizeof(storageName) -1);
	storageName[sizeof(storageName) -1] = '\0';
#else
	 convertWideStringToString (storageName, in_filename, FILENAME_MAX);
#endif

	int status = GSTG_OK;
	GError *err;

	GsfOutput  *output = GSF_OUTPUT (gsf_output_stdio_new (storageName, &err));

	if (output != NULL)
	{
		storage = GSF_OUTFILE (gsf_outfile_msole_new_full (
									output,
									in_sectorSize,	// sector size - 512 or 4096 bytes
									64));			// mini-sector size always 64 bytes
		g_object_unref (G_OBJECT (output));
	}
	else
		status = GSTG_ERROR;

	if (status == GSTG_OK)
	{
		OMGSFIStorage *newStorage = new OMGSFIStorage (storage, GSF_WRITE, storageName);
		*out_storage = newStorage;
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::StgOpenStorageEx( const TCHAR FAR* in_filename,
				OMFile::OMAccessMode in_accessMode,
				void **out_storage)
{
	TRACE("OMGSFIStorage::StgOpenStorageEx");
	PRECONDITION("Valid access mode", in_accessMode == OMFile::readOnlyMode);
	GsfStorage *storage = 0;
	*out_storage = 0;
	char storageName[FILENAME_MAX];

#ifndef OM_UNICODE_APIS
	strncpy (storageName, in_filename, sizeof(storageName) -1);
	storageName[sizeof(storageName) -1] = '\0';
#else
	convertWideStringToString (storageName, in_filename, FILENAME_MAX);
#endif

	GError *err;

	int status = GSTG_OK;

	GsfInput  *input = GSF_INPUT (gsf_input_stdio_new (storageName, &err));

	if (input != NULL)
	{
		input = gsf_input_uncompress (input);
		storage = GSF_INFILE (gsf_infile_msole_new (input, &err));
		g_object_unref (G_OBJECT (input));
	}
	else
		status = GSTG_ERROR;

	if (status == GSTG_OK)
	{
		OMGSFIStorage *newStorage = new OMGSFIStorage (storage, GSF_READ, storageName);
		*out_storage = newStorage;
	}

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::StgCreateStorageInOMRawStorage(
				const OMRawStorage* in_pRaw,
				OMFile::OMAccessMode in_accessMode,
				void** out_storage,
				ULONG in_sectorSize)
{
	TRACE("OMGSFIStorage::StgCreateStorageInOMRawStorage");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::StgOpenStorageInOMRawStorage(
				const OMRawStorage* in_pRaw,
				OMFile::OMAccessMode in_accessMode,
				void** out_storage)
{
	TRACE("OMGSFIStorage::StgOpenStorageInOMRawStorage");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::CreateStream(
				const TCHAR FAR* pwcsName,
				DWORD grfMode,
				DWORD reserved1,
				DWORD reserved2,
				IStream FAR *FAR *ppstm)
{
	TRACE("OMGSFIStorage::CreateStream");
	// TODO: not interpreting grfMode

	GsfStream *stream;
	int status = GSTG_OK;
	char streamName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	strncpy (streamName, pwcsName, sizeof(streamName) -1);
	streamName[sizeof(streamName) -1] = '\0';
#else
	convertWideStringToString (streamName, pwcsName, FILENAME_MAX);
#endif

	ASSERT ("Creating a stream in a WriteOnly GSF Storage", _mode == GSF_WRITE);

	if ((stream = gsf_outfile_new_child (GSF_OUTFILE(_storage), streamName, false)))
	{
		OMGSFIStream *newStream = new OMGSFIStream (stream, _mode, streamName);
		*ppstm = newStream;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::OpenStream(
				const TCHAR FAR* pwcsName,
				void FAR *reserved1,
				DWORD grfMode,
				DWORD reserved2,
				IStream FAR *FAR *ppstm)
{
	TRACE("OMGSFIStorage::OpenStream");
	// TODO: not interpreting grfMode

	GsfStream *stream;
	int status = GSTG_OK;
	char streamName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	strncpy (streamName, pwcsName, sizeof(streamName) -1);
	streamName[sizeof(streamName) -1] = '\0';
#else
	convertWideStringToString (streamName, pwcsName, FILENAME_MAX);
#endif

	ASSERT ("Opening a stream in a ReadOnly GSF Storage", _mode == GSF_READ);

	if ((stream = gsf_infile_child_by_name (GSF_INFILE(_storage), streamName)))
	{
		OMGSFIStream *newStream = new OMGSFIStream (stream, _mode, streamName);
		*ppstm = newStream;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}



HRESULT STDMETHODCALLTYPE
OMGSFIStorage::CreateStorage(
				const TCHAR FAR* pwcsName,
				DWORD grfMode,
				DWORD reserved1,
				DWORD reserved2,
				IStorage FAR *FAR *ppstg)
{
	TRACE("OMGSFIStorage::CreateStorage");
	// TODO: not interpreting grfMode

	int status = GSTG_OK;
	GsfStorage *storage = 0;
	*ppstg = 0;
	char storageName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	strncpy (storageName, pwcsName, sizeof(storageName) -1);
	storageName[sizeof(storageName) -1] = '\0';
#else
	convertWideStringToString (storageName, pwcsName, FILENAME_MAX);
#endif

	ASSERT ("Creating Storage in WriteOnly GSF Storage", _mode == GSF_WRITE);

	if ((storage = gsf_outfile_new_child (GSF_OUTFILE(_storage), storageName, true)))
	{
		OMGSFIStorage *newStorage = new OMGSFIStorage (storage, _mode, storageName);
		*ppstg = newStorage;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::OpenStorage(
				const TCHAR FAR* pwcsName,
				IStorage FAR *pstgPriority,
				DWORD grfMode,
				SNB snbExclude,
				DWORD reserved,
				IStorage FAR *FAR *ppstg)
{
	TRACE("OMGSFIStorage::OpenStorage");
	// TODO: not interpreting grfMode
	int status = GSTG_OK;
	GsfStorage *storage = 0;
	*ppstg = 0;
	char storageName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
	strncpy (storageName, pwcsName, sizeof(storageName) -1);
	storageName[sizeof(storageName) -1] = '\0';
#else
	convertWideStringToString (storageName, pwcsName, FILENAME_MAX);
#endif

	ASSERT ("Opening Storage in ReadOnly GSF Storage", _mode == GSF_READ);

	if ((storage = gsf_infile_child_by_name (GSF_INFILE(_storage), storageName)))
	{
		OMGSFIStorage *newStorage = new OMGSFIStorage (storage, _mode, storageName);
		*ppstg = newStorage;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::CopyTo(
				DWORD ciidExclude,
				IID const FAR *rgiidExclude,
				SNB snbExclude,
				IStorage FAR *pstgDest)
{
	TRACE("OMGSFIStorage::CopyTo");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::MoveElementTo(
				TCHAR const FAR* lpszName,
				IStorage FAR *pstgDest,
				TCHAR const FAR* lpszNewName,
				DWORD grfFlags)
{
	TRACE("OMGSFIStorage::MoveElementTo");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::Commit (DWORD grfCommitFlags)
{
	TRACE("OMGSFIStorage::Commit");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::Revert (void)
{
	TRACE("OMGSFIStorage::Revert");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::EnumElements(
				DWORD reserved1,
				void FAR *reserved2,
				DWORD reserved3,
				IEnumSTATSTG FAR *FAR *ppenm)
{
	TRACE("OMGSFIStorage::EnumElements");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::DestroyElement(const TCHAR FAR* pwcsName)
{
	TRACE("OMGSFIStorage::DestroyElement");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::RenameElement(
				const TCHAR FAR* pwcsOldName,
				const TCHAR FAR* pwcsNewName
			)
{
	TRACE("OMGSFIStorage::RenameElement");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::SetElementTimes(
				const TCHAR FAR *lpszName,
				FILETIME const FAR *pctime,
				FILETIME const FAR *patime,
				FILETIME const FAR *pmtime
										)
{
	TRACE("OMGSFIStorage::SetElementTimes");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

// libgsf treats clsid as an array of 16 bytes storing it unchanged on disk.
// The container spec requires a little-endian ordering of clsid on disk
// so on bigendian machines we must reorder the class id.
static inline void reorder_clsid(unsigned char *id)
{
	if (hostByteOrder() == littleEndian)
		return;

	unsigned char t[8];
	memmove(t, &id[0], 8);
	id[0] = t[3];		// reorder Data1 int32_t
	id[1] = t[2];
	id[2] = t[1];
	id[3] = t[0];
	id[4] = t[5];		// reorder Data2 int16_t
	id[5] = t[4];
	id[6] = t[7];		// reorder Data3 int16_t
	id[7] = t[6];
	// Data4 is an array of char so remains unchanged
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::SetClass (REFCLSID clsid)
{
	TRACE("OMGSFIStorage::SetClass");
	ASSERT ("Calling SetClass in a WriteOnly GSF Storage", _mode == GSF_WRITE);

	unsigned char tmp_clsid[16];

	memmove(&tmp_clsid, &clsid, sizeof(tmp_clsid));
	reorder_clsid(tmp_clsid);

	int status = GSTG_OK;
	if (!gsf_outfile_msole_set_class_id (GSF_OUTFILE_MSOLE(_storage), tmp_clsid))
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::SetStateBits(
				DWORD grfStateBits,
				DWORD grfMask
				)
{
	TRACE("OMGSFIStorage::SetStateBits");
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStorage::Stat(
				STATSTG FAR *pstatstg,
				DWORD grfStatFlag
				)
{
	TRACE("OMGSFIStorage::DestroyElement");

	int status = GSTG_OK;
	memset(pstatstg, 0, sizeof(STATSTG));

	// TODO: ignoring several fields currently not required by SDK

	if (!(grfStatFlag & STATFLAG_NONAME))
	{
#ifdef OM_UNICODE_APIS
		pstatstg->pwcsName = convertString(_storageName);
#else
		pstatstg->pwcsName = saveString(_storageName);
#endif
	}
	//pstatstg->type
	//pstatstg->cbSize
	//pstatstg->mtime
	//pstatstg->ctime
	//pstatstg->atime

	switch (_mode)
	{
	case GSF_READ:         pstatstg->grfMode |= STGM_READ; break;
	case GSF_READWRITE:   pstatstg->grfMode |= STGM_READWRITE; break;
	case GSF_WRITE:        pstatstg->grfMode |= STGM_WRITE; break;
	default:
		ASSERT ("Known  SS access mode", false);
		break;
	}

	//pstatstg->grfLocksSupported

	unsigned char clsid[16];
	if ( _mode == GSF_READ)
	{
		if (gsf_infile_msole_get_class_id (GSF_INFILE_MSOLE(_storage), clsid))
		{
			reorder_clsid(clsid);
			memmove (&pstatstg->clsid, clsid, sizeof(pstatstg->clsid));
		}
		else
			status = GSTG_ERROR;
	}

	//pstatstg->grfStateBits
	//pstatstg->reserved
	return makeStatus(status);
}

// GSF streams
OMGSFIStream::OMGSFIStream (GsfStream *in_stream, GsfAccessMode mode, const char *sname) :
	_stream(in_stream), _mode(mode), _referenceCount(1)
{
	strcpy(_streamName, sname);
	TRACE("OMGSFIStream::OMGSFIStream");
	PRECONDITION("Valid reference count", _referenceCount == 1);
}

OMGSFIStream::~OMGSFIStream ()
{
	TRACE("OMGSFIStream::~OMGSFIStream");
	PRECONDITION("Valid reference count", _referenceCount == 0);
	PRECONDITION("Stream has been closed", _stream == 0);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::QueryInterface(REFIID /* riid */, void** ppvObject)
{
	TRACE("OMGSFIStream::QueryInterface");
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
OMGSFIStream::AddRef(void)
{
	TRACE("OMGSFIStream::AddRef");
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE
OMGSFIStream::Release(void)
{
	TRACE("OMGSFIStream::Release");
	ULONG result = --_referenceCount;
	if (_referenceCount == 0)
	{
		if (_stream != 0)
		{
			if (GSF_IS_OUTPUT(_stream))
				gsf_output_close (GSF_OUTPUT(_stream));

			g_object_unref (G_OBJECT (_stream));
			_stream = 0;
		}
		delete this;
	}
	return result;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::Read(
				VOID HUGEP *pv,
				ULONG cb,
				ULONG FAR *pcbRead
				)
{
	TRACE("OMGSFIStream::Read");
	ASSERT ("Reading a ReadOnly GSF Stream", _mode == GSF_READ);

	int result = GSTG_OK;
	OMUInt64 bytesToRead = gsf_input_remaining (GSF_INPUT(_stream));
	if (bytesToRead > cb)
		bytesToRead = cb;

	if (bytesToRead > 0)
	{
		if (gsf_input_read(GSF_INPUT(_stream), bytesToRead, (guint8*)pv) == NULL)
			result = GSTG_ERROR;
	}
	*pcbRead = bytesToRead;

	return makeStatus(result);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::Write(
				VOID const HUGEP *pv,
				ULONG cb,
				ULONG FAR *pcbWritten
										)
{
	TRACE("OMGSFIStream::Write");
	ASSERT ("Writing to a WriteOnly GSF stream", _mode == GSF_WRITE);

	int result = GSTG_OK;

	if (!gsf_output_write (GSF_OUTPUT(_stream), cb, (const guint8 *)pv))
		result = GSTG_ERROR;

	*pcbWritten = cb;
	return makeStatus(result);
}

int OMGSFIStream::Seek (OMInt64 offset, DWORD whence)
{
	TRACE("OMGSFIStream::Seek");
	int status = GSTG_OK;

	GSeekType pos;

	switch (whence)
	{
	case STREAM_SEEK_CUR:
		pos = G_SEEK_CUR;
		break;
	case STREAM_SEEK_END:
		pos = G_SEEK_END;
		break;
	case STREAM_SEEK_SET:
	default:
		pos = G_SEEK_SET;
		break;
	}


	if ( GSF_IS_INPUT(_stream))
	{
		if (gsf_input_seek (GSF_INPUT(_stream), offset, pos) < 0)
			status = GSTG_ERROR;
	}
	else if (GSF_IS_OUTPUT(_stream))
	{
		if (!gsf_output_seek (GSF_OUTPUT(_stream), offset, pos))
			status = GSTG_ERROR;
	}
	else
	{
		ASSERT ("Known GSF stream type", false);
		status = GSTG_ERROR;
	}
	return status;
}


HRESULT STDMETHODCALLTYPE
OMGSFIStream::Seek(
				LARGE_INTEGER dlibMove,
				DWORD dwOrigin,
				ULARGE_INTEGER FAR *plibNewPosition
				)
{
	TRACE("OMGSFIStream::Seek");
	int status = GSTG_OK;

	ULARGE_INTEGER offset;
	memcpy (&offset, &dlibMove, sizeof(LARGE_INTEGER));

	status = this->Seek(toOMUInt64(offset), dwOrigin);

	if (status == GSTG_OK && plibNewPosition != 0)
	{
		OMUInt64 newPos = 0;
		if((status = this->Tell(&newPos)) == GSTG_OK)
			*plibNewPosition = fromOMUInt64(newPos);
	}
	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::SetSize( ULARGE_INTEGER libNewSize)
{
	TRACE("OMGSFIStream::SetSize");
	ASSERT ("Calling SetSize for ReadOnly GSF Streams", _mode == GSF_READ);

	OMUInt64 newSize = toOMUInt64(libNewSize);

	int status = GSTG_OK;
	if (!gsf_input_set_size (GSF_INPUT(_stream), newSize))
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::CopyTo(
				IStream FAR *pstm,
				ULARGE_INTEGER cb,
				ULARGE_INTEGER FAR *pcbRead,
				ULARGE_INTEGER FAR *pcbWritten)
{
	TRACE("OMGSFIStream::SetSize");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::Commit(DWORD grfCommitFlags)
{
	TRACE("OMGSFIStream::Commit");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::Revert(void)
{
	TRACE("OMGSFIStream::Revert");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::LockRegion(
				ULARGE_INTEGER libOffset,
				ULARGE_INTEGER cb,
				DWORD dwLockType)
{
	TRACE("OMGSFIStream::LockRegion");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::UnlockRegion(
				ULARGE_INTEGER libOffset,
				ULARGE_INTEGER cb,
				DWORD dwLockType)
{
	TRACE("OMGSFIStream::LockRegion");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

int OMGSFIStream::Tell (OMUInt64 *position) const
{
	TRACE("OMGSFIStream::Tell");
	int status = GSTG_OK;

	*position = 0;

	if (GSF_IS_INPUT(_stream))
		*position = gsf_input_tell (GSF_INPUT(_stream));
	else if (GSF_IS_OUTPUT(_stream))
		*position = gsf_output_tell (GSF_OUTPUT(_stream));
	else
	{
		ASSERT ("Known GSF stream type", false);
	}
	return status;
}

int OMGSFIStream::Size (OMUInt64 *ssize) const
{
	TRACE("OMGSFIStream::Size");
	*ssize = 0;

	if (GSF_IS_INPUT(_stream))
		*ssize =  gsf_input_size (GSF_INPUT(_stream));
	else if (GSF_IS_OUTPUT(_stream))
		*ssize = gsf_output_size (GSF_OUTPUT(_stream));
	else
	{
		ASSERT ("Known GSF stream type", false);
		return GSTG_ERROR;
    }

	return GSTG_OK;
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::Stat(
				STATSTG FAR *pstatstg,
				DWORD grfStatFlag)
{
	TRACE("OMGSFIStream::Stat");
	OMUInt64 curSize;

	memset (pstatstg, 0, sizeof(*pstatstg));

	// TODO: ignoring several fields currently not required by SDK

	if (!(grfStatFlag & STATFLAG_NONAME))
	{
#ifdef OM_UNICODE_APIS
		pstatstg->pwcsName = convertString(_streamName);
#else
		pstatstg->pwcsName = saveString(_streamName);
#endif
	}

	//pstatstg->type

	int status = this->Size (&curSize);
	pstatstg->cbSize = fromOMUInt64(curSize);

	//pstatstg->mtime
	//pstatstg->ctime
	//pstatstg->atime

	switch (_mode)
	{
	case GSF_READ:
		pstatstg->grfMode |= STGM_READ;
		break;
	case GSF_READWRITE:
		pstatstg->grfMode |= STGM_READWRITE;
		break;
	case GSF_WRITE:
		pstatstg->grfMode |= STGM_WRITE;
		break;
	default:
		ASSERT ("Known  SS access mode", false);
		break;
	}

	//pstatstg->grfLocksSupported
	//pstatstg->clsid
	//pstatstg->grfStateBits
	//pstatstg->reserved

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
OMGSFIStream::Clone(IStream FAR * FAR *ppstm)
{
	TRACE("OMGSFIStream::Clone");
	return STG_E_UNIMPLEMENTEDFUNCTION;
}
