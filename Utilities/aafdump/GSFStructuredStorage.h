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
//=---------------------------------------------------------------------=

#ifndef __GSFSTRUCTUREDSTORAGE_H__
#define __GSFSTRUCTUREDSTORAGE_H__

#include <StructuredStorage.h>
#include <stdio.h>  // for FILENAME_MAX

// Check host operating system.
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


// Integral types
//
#if defined(OM_OS_WINDOWS)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef __int64                OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned __int64       OMUInt64;

#else

// Use ISO C99 (also ANSI and POSIX) fixed size integers
#include <inttypes.h>
typedef int8_t                 OMInt8;
typedef int16_t                OMInt16;
typedef int32_t                OMInt32;
typedef int64_t                OMInt64;

typedef uint8_t                OMUInt8;
typedef uint16_t               OMUInt16;
typedef uint32_t               OMUInt32;
typedef uint64_t               OMUInt64;

#endif


typedef enum { GSF_READ, GSF_WRITE, GSF_READWRITE } GsfAccessMode;

typedef void GsfStorage;
typedef void GsfStream;


class GSFStructuredStorage : public StructuredStorage
{
public:
    GSFStructuredStorage();
    virtual ~GSFStructuredStorage();

public:
	virtual HRESULT STDMETHODCALLTYPE StgOpenStorage(
                                const TCHAR FAR* pwcsName,
                                IStorage* pstgPriority,
                                DWORD grfMode,
                                SNB snbExclude,
                                DWORD reserved,
                                IStorage** ppstgOpen);

    virtual HRESULT STDMETHODCALLTYPE StgIsStorageFile(
                                const TCHAR FAR* pwcsName);

public:                                
    virtual void wcsFree(TCHAR FAR* wcs);
};


class GSFIStorage : public IStorage
{
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

public:
    friend class GSFStructuredStorage;

    GsfStorage* GetGsfStorage(void);
    GsfAccessMode GetMode(void);
    
protected:
	// Make default contructor private so that an GSFIStorage object cannot
	// be instantiated
	GSFIStorage(GsfStorage *in_storage, GsfAccessMode _mode);

	// keeping destructor protected means that the object must always be
	// instantiated on the heap;
	virtual ~GSFIStorage();

private:
	GsfStorage*		_storage;
	GsfAccessMode	_mode;				// TODO: does gsf store this somewhere?
	ULONG 			_referenceCount;
};


class GSFIStream : public IStream
{
public:
	// public constructor
	GSFIStream(GsfStream *in_stream, GsfAccessMode mode);

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

	// Make destructor virtual so that this objeact can only be instantiated
	// on heap
	virtual ~GSFIStream();

private:
	int Tell (OMUInt64 *position) const;
	int Seek (OMInt64 offset, DWORD whence);
	int Size (OMUInt64 *ssize) const;


private:
	GsfStream*			_stream;
	GsfAccessMode	_mode;				// TODO: does gsf store this somewhere?
	ULONG 			_referenceCount;
};


class GSFIEnumSTATSTG : public IEnumSTATSTG
{
public:
	// public constructor
	GSFIEnumSTATSTG(GSFIStorage* pstorage);

	// IUnknown interface
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);

	virtual ULONG STDMETHODCALLTYPE Release(void);

	// IEnumSTATSTG interface
	virtual HRESULT STDMETHODCALLTYPE Next(
          ULONG celt, 
          STATSTG FAR * rgelt, 
          ULONG FAR *pceltFetched);
          
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
                       
	virtual HRESULT STDMETHODCALLTYPE Reset(void);
    
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumSTATSTG FAR *FAR *ppenm);
    
    
protected:

	// Make destructor virtual so that this objeact can only be instantiated
	// on heap
	virtual ~GSFIEnumSTATSTG();

private:
    GSFIStorage*    _gsfIStorage;
    int             _index;
    int             _numChildren;

	ULONG  	  	    _referenceCount;
};

#endif
