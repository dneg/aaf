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

#include "GSFStructuredStorage.h"
#include <assert.h>
#include <string.h>

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


// Determine host operating system.
//
#if defined(_WIN32)
#define OM_OS_WINDOWS
#elif defined(__MWERKS__) && defined(__MACH__)
#define OM_OS_MACOSX
#elif defined(__sgi) || defined(__linux__) || defined (__FreeBSD__) || \
      defined (__APPLE__) || defined(__CYGWIN__)
#define OM_OS_UNIX
#elif defined (sun)
#define OM_OS_SOLARIS
#else
#error "Can't determine host operating system"
#endif

// Determine whether or not UNICODE versions of the APIs are in use.
//
#if defined(OM_OS_WINDOWS) && defined(UNICODE)
#define OM_UNICODE_APIS
#endif

// status codes used locally
#define GSTG_OK 0
#define GSTG_ERROR 1


// utility function prototypes
static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x);
static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x);
char* saveString(const char* string);
void convertWideStringToString(char*  result,
                               const wchar_t*  string ,
                               size_t  resultSize);
wchar_t* convertString(const char* string);
static inline HRESULT STDMETHODCALLTYPE makeStatus(int status);



// GSF Structured Storage
HRESULT STDMETHODCALLTYPE 
GSFStructuredStorage::StgOpenStorage(
                                const TCHAR FAR* pwcsName,
                                IStorage* pstgPriority,
                                DWORD grfMode,
                                SNB snbExclude,
                                DWORD reserved,
                                IStorage** ppstgOpen)
{
	assert(STGM_READ == (grfMode & (STGM_READ|STGM_WRITE|STGM_READWRITE)));
    
	GsfStorage *storage = 0;
	*ppstgOpen = 0;
	char storageName[FILENAME_MAX];

#ifndef OM_UNICODE_APIS
	strncpy (storageName, pwcsName, sizeof(storageName) -1);
	storageName[sizeof(storageName) -1] = '\0';
#else
	convertWideStringToString (storageName, pwcsName, FILENAME_MAX);
#endif

	GError *err;

	int status = GSTG_OK;

	GsfInput  *input = GSF_INPUT (gsf_input_stdio_new (storageName, &err));

	if (input != NULL)
	{
		input = gsf_input_uncompress (input);
		storage = GSF_INFILE (gsf_infile_msole_new (input, &err));
        // set name so that when calling gsf_input_name it doesn't return null
        gsf_input_set_name(GSF_INPUT(storage), storageName); 
		g_object_unref (G_OBJECT (input));
	}
	else
    {
		status = GSTG_ERROR;
    }

	if (status == GSTG_OK)
	{
		GSFIStorage *newStorage = new GSFIStorage (storage, GSF_READ);
		*ppstgOpen = newStorage;
	}

	return makeStatus(status);
}                                

HRESULT STDMETHODCALLTYPE 
GSFStructuredStorage::StgIsStorageFile(const TCHAR FAR* pwcsName)
{
	GsfStorage *storage = 0;
	char storageName[FILENAME_MAX];

#ifndef OM_UNICODE_APIS
	strncpy (storageName, pwcsName, sizeof(storageName) -1);
	storageName[sizeof(storageName) -1] = '\0';
#else
	convertWideStringToString (storageName, pwcsName, FILENAME_MAX);
#endif

	GError *err;

	int status = GSTG_OK;

	GsfInput  *input = GSF_INPUT (gsf_input_stdio_new (storageName, &err));

	if (input != NULL)
	{
		input = gsf_input_uncompress (input);
		storage = GSF_INFILE (gsf_infile_msole_new (input, &err));
        g_object_unref (G_OBJECT (input));
        if (storage != NULL) 
        {
            g_object_unref (G_OBJECT(storage));
        } 
        else 
        {
            status = GSTG_ERROR;
        }
	}
	else
    {
		status = GSTG_ERROR;
    }

	return makeStatus(status);
}                                

void 
GSFStructuredStorage::wcsFree(TCHAR FAR*  wcs)
{
    delete [] wcs;
}                                    
                                
GSFStructuredStorage::GSFStructuredStorage(void)
{
    gsf_init();
}

GSFStructuredStorage::~GSFStructuredStorage(void)
{
    gsf_shutdown ();
}    




// GSF Storage
GSFIStorage::GSFIStorage(GsfStorage *in_storage, GsfAccessMode mode) : _storage(in_storage), _mode(mode), _referenceCount(1)
{
};

GSFIStorage::~GSFIStorage()
{
    assert(_referenceCount == 0);
	assert(_storage == 0);
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::QueryInterface(REFIID /* riid */, void** ppvObject)
{
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
GSFIStorage::AddRef(void)
{
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE
GSFIStorage::Release(void)
{
	ULONG result;
    result = --_referenceCount;
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
GSFIStorage::CreateStream(
				const TCHAR FAR* pwcsName,
				DWORD grfMode,
				DWORD reserved1,
				DWORD reserved2,
				IStream FAR *FAR *ppstm)
{
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

    assert(_mode == GSF_WRITE);

	if ((stream = gsf_outfile_new_child (GSF_OUTFILE(_storage), streamName, false)))
	{
		GSFIStream *newStream = new GSFIStream (stream, _mode);
		*ppstm = newStream;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::OpenStream(
				const TCHAR FAR* pwcsName,
				void FAR *reserved1,
				DWORD grfMode,
				DWORD reserved2,
				IStream FAR *FAR *ppstm)
{
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

    assert(_mode == GSF_READ);

	if ((stream = gsf_infile_child_by_name (GSF_INFILE(_storage), streamName)))
	{
		GSFIStream *newStream = new GSFIStream (stream, _mode);
		*ppstm = newStream;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}



HRESULT STDMETHODCALLTYPE
GSFIStorage::CreateStorage(
				const TCHAR FAR* pwcsName,
				DWORD grfMode,
				DWORD reserved1,
				DWORD reserved2,
				IStorage FAR *FAR *ppstg)
{
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

    assert(_mode == GSF_WRITE);

	if ((storage = gsf_outfile_new_child (GSF_OUTFILE(_storage), storageName, true)))
	{
		GSFIStorage *newStorage = new GSFIStorage (storage, _mode);
		*ppstg = newStorage;
	}
	else
    {
		status = GSTG_ERROR;
    }

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::OpenStorage(
				const TCHAR FAR* pwcsName,
				IStorage FAR *pstgPriority,
				DWORD grfMode,
				SNB snbExclude,
				DWORD reserved,
				IStorage FAR *FAR *ppstg)
{
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

    assert(_mode == GSF_READ);

	if ((storage = gsf_infile_child_by_name (GSF_INFILE(_storage), storageName)))
	{
		GSFIStorage *newStorage = new GSFIStorage (storage, _mode);
		*ppstg = newStorage;
	}
	else
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::CopyTo(
				DWORD ciidExclude,
				IID const FAR *rgiidExclude,
				SNB snbExclude,
				IStorage FAR *pstgDest)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::MoveElementTo(
				TCHAR const FAR* lpszName,
				IStorage FAR *pstgDest,
				TCHAR const FAR* lpszNewName,
				DWORD grfFlags)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::Commit (DWORD grfCommitFlags)
{
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::Revert (void)
{
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::EnumElements(
				DWORD reserved1,
				void FAR *reserved2,
				DWORD reserved3,
				IEnumSTATSTG FAR *FAR *ppenm)
{
    *ppenm = new GSFIEnumSTATSTG(this);
    assert(*ppenm != 0);
    
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::DestroyElement(const TCHAR FAR* pwcsName)
{
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::RenameElement(
				const TCHAR FAR* pwcsOldName,
				const TCHAR FAR* pwcsNewName
			)
{
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::SetElementTimes(
				const TCHAR FAR *lpszName,
				FILETIME const FAR *pctime,
				FILETIME const FAR *patime,
				FILETIME const FAR *pmtime
										)
{
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::SetClass (REFCLSID clsid)
{
    assert(_mode == GSF_WRITE);

	unsigned char tmp_clsid[16];

	memmove(&tmp_clsid, &clsid, sizeof(tmp_clsid));

	int status = GSTG_OK;
	if (!gsf_outfile_msole_set_class_id (GSF_OUTFILE_MSOLE(_storage), tmp_clsid))
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::SetStateBits(
				DWORD grfStateBits,
				DWORD grfMask
				)
{
    return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStorage::Stat(
				STATSTG FAR *pstatstg,
				DWORD grfStatFlag
				)
{
	int status = GSTG_OK;
	memset(pstatstg, 0, sizeof(STATSTG));

	// TODO: ignoring several fields currently not required by SDK

	if (!(grfStatFlag & STATFLAG_NONAME))
	{
        const char* storageName = gsf_input_name(GSF_INPUT(_storage));
#ifdef OM_UNICODE_APIS
		pstatstg->pwcsName = convertString(storageName);
#else
		pstatstg->pwcsName = saveString(storageName);
#endif
	}
    
    pstatstg->type = STGTY_STORAGE;

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
		assert(false);
		break;
	}

	//pstatstg->grfLocksSupported

	unsigned char clsid[16];
	if ( _mode == GSF_READ)
	{
		if (gsf_infile_msole_get_class_id (GSF_INFILE_MSOLE(_storage), clsid))
			memmove (&pstatstg->clsid, clsid, sizeof(pstatstg->clsid));
		else
			status = GSTG_ERROR;
	}

	//pstatstg->grfStateBits
	//pstatstg->reserved
	return makeStatus(status);
}

GsfStorage* 
GSFIStorage::GetGsfStorage(void)
{
    return _storage;
}

GsfAccessMode 
GSFIStorage::GetMode(void)
{
    return _mode;
}

// GSF streams
GSFIStream::GSFIStream (GsfStream *in_stream, GsfAccessMode mode) :
	_stream(in_stream), _mode(mode), _referenceCount(1)
{
}

GSFIStream::~GSFIStream ()
{
    assert(_referenceCount == 0);
	assert(_stream == 0);
}

HRESULT STDMETHODCALLTYPE
GSFIStream::QueryInterface(REFIID /* riid */, void** ppvObject)
{
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE
GSFIStream::AddRef(void)
{
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE
GSFIStream::Release(void)
{
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
GSFIStream::Read(
				VOID HUGEP *pv,
				ULONG cb,
				ULONG FAR *pcbRead
				)
{
	assert(_mode == GSF_READ);

	int result = GSTG_OK;
	OMUInt64 bytesToRead = gsf_input_remaining (GSF_INPUT(_stream));
	if (bytesToRead > cb)
		bytesToRead = cb;

	if (bytesToRead > 0)
	{
		if (gsf_input_read(GSF_INPUT(_stream), (ULONG)bytesToRead, (guint8*)pv) == NULL)
			result = GSTG_ERROR;
	}
	*pcbRead = (ULONG)bytesToRead;

	return makeStatus(result);
}

HRESULT STDMETHODCALLTYPE
GSFIStream::Write(
				VOID const HUGEP *pv,
				ULONG cb,
				ULONG FAR *pcbWritten
										)
{
    assert(_mode == GSF_WRITE);

	int result = GSTG_OK;

	if (!gsf_output_write (GSF_OUTPUT(_stream), cb, (const guint8 *)pv))
		result = GSTG_ERROR;

	*pcbWritten = cb;
	return makeStatus(result);
}

int GSFIStream::Seek (OMInt64 offset, DWORD whence)
{
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
		assert(false);
		status = GSTG_ERROR;
	}
	return makeStatus(status);
}


HRESULT STDMETHODCALLTYPE
GSFIStream::Seek(
				LARGE_INTEGER dlibMove,
				DWORD dwOrigin,
				ULARGE_INTEGER FAR *plibNewPosition
				)
{
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
GSFIStream::SetSize( ULARGE_INTEGER libNewSize)
{
	assert(_mode == GSF_READ);

	OMUInt64 newSize = toOMUInt64(libNewSize);

	int status = GSTG_OK;
	if (!gsf_input_set_size (GSF_INPUT(_stream), newSize))
		status = GSTG_ERROR;

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStream::CopyTo(
				IStream FAR *pstm,
				ULARGE_INTEGER cb,
				ULARGE_INTEGER FAR *pcbRead,
				ULARGE_INTEGER FAR *pcbWritten)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStream::Commit(DWORD grfCommitFlags)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStream::Revert(void)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStream::LockRegion(
				ULARGE_INTEGER libOffset,
				ULARGE_INTEGER cb,
				DWORD dwLockType)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

HRESULT STDMETHODCALLTYPE
GSFIStream::UnlockRegion(
				ULARGE_INTEGER libOffset,
				ULARGE_INTEGER cb,
				DWORD dwLockType)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}

int GSFIStream::Tell (OMUInt64 *position) const
{
	int status = GSTG_OK;

	*position = 0;

	if (GSF_IS_INPUT(_stream))
		*position = gsf_input_tell (GSF_INPUT(_stream));
	else if (GSF_IS_OUTPUT(_stream))
		*position = gsf_output_tell (GSF_OUTPUT(_stream));
	else
	{
		assert(false);
        status = GSTG_ERROR;
	}
	return makeStatus(status);
}

int GSFIStream::Size (OMUInt64 *ssize) const
{
	int status = GSTG_OK;

	*ssize = 0;

	if (GSF_IS_INPUT(_stream))
		*ssize =  gsf_input_size (GSF_INPUT(_stream));
	else if (GSF_IS_OUTPUT(_stream))
		*ssize = gsf_output_size (GSF_OUTPUT(_stream));
	else
	{
		assert(false);
		status = GSTG_ERROR;
    }

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStream::Stat(
				STATSTG FAR *pstatstg,
				DWORD grfStatFlag)
{
	OMUInt64 curSize;

	memset (pstatstg, 0, sizeof(*pstatstg));

	// TODO: ignoring several fields currently not required by SDK

	if (!(grfStatFlag & STATFLAG_NONAME))
	{
        const char* streamName = gsf_input_name(GSF_INPUT(_stream));
#ifdef OM_UNICODE_APIS
		pstatstg->pwcsName = convertString(streamName);
#else
		pstatstg->pwcsName = saveString(streamName);
#endif
	}

    pstatstg->type = STGTY_STREAM;

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
		assert(false);
		break;
	}

	//pstatstg->grfLocksSupported
	//pstatstg->clsid
	//pstatstg->grfStateBits
	//pstatstg->reserved

	return makeStatus(status);
}

HRESULT STDMETHODCALLTYPE
GSFIStream::Clone(IStream FAR * FAR *ppstm)
{
	return STG_E_UNIMPLEMENTEDFUNCTION;
}


GSFIEnumSTATSTG::GSFIEnumSTATSTG(GSFIStorage* pstorage) :
    _index(0), _referenceCount(1)
{
    _gsfIStorage = pstorage;
    assert(GSF_IS_INFILE(pstorage->GetGsfStorage()));
    _numChildren = gsf_infile_num_children(GSF_INFILE(pstorage->GetGsfStorage()));
    assert(_numChildren >= 0);
}

GSFIEnumSTATSTG::~GSFIEnumSTATSTG()
{
    assert(_referenceCount == 0);
}

HRESULT STDMETHODCALLTYPE 
GSFIEnumSTATSTG::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = 0;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE 
GSFIEnumSTATSTG::AddRef(void)
{
	++_referenceCount;
	return _referenceCount;
}

ULONG STDMETHODCALLTYPE 
GSFIEnumSTATSTG::Release(void)
{
	ULONG result = --_referenceCount;
	if (_referenceCount == 0)
	{
		delete this;
	}
	return result;
}

HRESULT STDMETHODCALLTYPE 
GSFIEnumSTATSTG::Next(
          ULONG celt, 
          STATSTG FAR * rgelt, 
          ULONG FAR *pceltFetched)
{
	int status = GSTG_OK;

    ULONG numFetch = 0;
    STATSTG* pstatstg = rgelt; 
    for (numFetch=0; numFetch<celt && _index<_numChildren; _index++, numFetch++, pstatstg++) {
        GsfInput* input = gsf_infile_child_by_index(GSF_INFILE(_gsfIStorage->GetGsfStorage()),_index);

        memset (pstatstg, 0, sizeof(*pstatstg));

#ifdef OM_UNICODE_APIS
		pstatstg->pwcsName = convertString(input->name);
#else
		pstatstg->pwcsName = saveString(input->name);
#endif

   	    if (GSF_IS_INFILE (input) &&
		      (gsf_infile_num_children (GSF_INFILE (input)) >= 0)) {
            pstatstg->type = STGTY_STORAGE;
            //size only relevant for streams -- pstatstg->cbSize
        } else {
            pstatstg->type = STGTY_STREAM;
            OMUInt64 curSize;
            curSize = gsf_input_size(input);
            pstatstg->cbSize = fromOMUInt64(curSize);
        }


	    //pstatstg->mtime
	    //pstatstg->ctime
	    //pstatstg->atime

        GsfAccessMode mode = _gsfIStorage->GetMode();
        switch (mode)
        {
            case GSF_READ:         pstatstg->grfMode |= STGM_READ; break;
            case GSF_READWRITE:   pstatstg->grfMode |= STGM_READWRITE; break;
            case GSF_WRITE:        pstatstg->grfMode |= STGM_WRITE; break;
            default:
            assert(false);
            break;
        }

        unsigned char clsid[16];
        if (mode == GSF_READ)
        {
            if (gsf_infile_msole_get_class_id (GSF_INFILE_MSOLE(input), clsid))
                memmove (&pstatstg->clsid, clsid, sizeof(pstatstg->clsid));
        }


	    //pstatstg->grfLocksSupported
	    //pstatstg->grfStateBits
	    //pstatstg->reserved

        g_object_unref (G_OBJECT (input));
    }
    if (pceltFetched != NULL) {
        *pceltFetched = numFetch;
    }

    if (numFetch != celt) {
        status = GSTG_ERROR;
    }

    return makeStatus(status);
}
                       
HRESULT STDMETHODCALLTYPE 
GSFIEnumSTATSTG::Skip(
          ULONG celt)
{
    if (_index + (int)celt >= _numChildren) 
    {
        return makeStatus(GSTG_ERROR);
    }
    _index += celt;
    
    return makeStatus(GSTG_OK);
}

                       
HRESULT STDMETHODCALLTYPE 
GSFIEnumSTATSTG::Reset(void)
{
    _index = 0;

    return makeStatus(GSTG_OK);
}
    
HRESULT STDMETHODCALLTYPE 
GSFIEnumSTATSTG::Clone(
          IEnumSTATSTG FAR *FAR *ppenm)
{
    assert(false);
	return STG_E_UNIMPLEMENTEDFUNCTION;
}
    


    
static inline HRESULT STDMETHODCALLTYPE makeStatus(int status)
{
  if (status != GSTG_OK) {
      return S_FALSE;
  }
  return S_OK;
}

char* saveString(const char* string)
{
  assert(string != 0);
  size_t length = strlen(string) + 1;
  char* result = new char[length];
  assert(result != 0);
  strcpy(result, string);
  return result;
}

// TODO: need proper multibyte string support here
void convertWideStringToString(char*  result,
                               const wchar_t*  string ,
                               size_t  resultSize)
{
  assert(result != 0);
  assert(resultSize > 0);

  size_t length = wcstombs(NULL, string, 1);
  if (length > (resultSize - 1)) {
    length = resultSize - 1;
  }
#if defined(OM_DEBUG)
  size_t status =
#endif
  wcstombs(result, string, resultSize);
  assert(status != (size_t)-1);
}

wchar_t* convertString(const char* string)
{
  assert(string != 0);
  size_t length = strlen(string);
  wchar_t* result = new wchar_t[length + 1];
  assert(result != 0);
#if defined(OM_DEBUG)
  size_t status =
#endif
  mbstowcs(result, string, length + 1);
  assert(status != (size_t)-1);
  return result;
}

#if defined(OM_OS_UNIX)

// The Macintosh and reference implementation declarations
// for LARGE_INTEGER and ULARGE_INTEGER don't have a QuadPart.
// On Macintosh this is probably because the HighPart and LowPart
// components on that platform are not in the natural platform order.
// Here we hide these differences behind a couple of functions.

static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x)
{
  OMUInt64 result = (((OMUInt64)x.HighPart) << 32) + (x.LowPart);
  return result;
}

static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x)
{
  ULARGE_INTEGER result;
  result.HighPart = (OMUInt32)(((OMUInt64)x) >> 32);
  result.LowPart  = (OMUInt32) (0x00000000FFFFFFFF & (OMUInt64)x);
  return result;
}

#else

static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x)
{
  OMUInt64 result;
  result = x.QuadPart;
  return result;
}


static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x)
{
  ULARGE_INTEGER result;
  result.QuadPart = x;
  return result;
}

#endif

