/*****************************************************************************\
*                                                                             *
* storage.h -   Definitions for the strutured storage system
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/


#if !defined(__STORAGE__) && !defined( _STORAGE_H_ )
#define __STORAGE__
#define _STORAGE_H_

#if !defined( __MACPUB__ )
#include <macpub.h>
#endif

#ifndef __COMPOBJ__
#include <compobj.h>
#endif

/****** Storage Error Codes *************************************************/

#ifndef _WINERROR_

/* DOS-based error codes */
#define STG_E_INVALIDFUNCTION \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x01)

#define STG_E_FILENOTFOUND \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x02)

#define STG_E_PATHNOTFOUND \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x03)

#define STG_E_TOOMANYOPENFILES \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x04)

#define STG_E_ACCESSDENIED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x05)

#define STG_E_INVALIDHANDLE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x06)

#define STG_E_INSUFFICIENTMEMORY \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x08)

#define STG_E_INVALIDPOINTER \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x09)

#define STG_E_NOMOREFILES \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x12)

#define STG_E_DISKISWRITEPROTECTED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x13)

#define STG_E_SEEKERROR \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x19)

#define STG_E_WRITEFAULT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x1d)

#define STG_E_READFAULT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x1e)

/* 20a */

#define STG_E_SHAREVIOLATION \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x20)

/* 20a */
#define STG_E_LOCKVIOLATION \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x21)

#define STG_E_FILEALREADYEXISTS \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x50)

#define STG_E_INVALIDPARAMETER \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x57)

#define STG_E_MEDIUMFULL \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x70)

#define STG_E_ABNORMALAPIEXIT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfa)

#define STG_E_INVALIDHEADER \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfb)

#define STG_E_INVALIDNAME \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfc)

#define STG_E_UNKNOWN \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfd)

#define STG_E_UNIMPLEMENTEDFUNCTION\
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfe)

#define STG_E_INVALIDFLAG \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xff)

/* Standard storage error codes */
#define STG_E_INUSE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x100)

#define STG_E_NOTCURRENT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x101)

#define STG_E_REVERTED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x102)

#define STG_E_CANTSAVE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x103)

#define STG_E_OLDFORMAT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x104)
    
#define STG_E_OLDDLL \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x105)

#define STG_E_SHAREREQUIRED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x106)
/* 20a */

#define STG_E_NOTFILEBASEDSTORAGE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x107)

#define STG_E_EXTANTMARSHALLINGS \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x108)

#define STG_E_DOCFILECORRUPT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x109)

/* 20a */

/* Information returns */
#define STG_S_CONVERTED \
    MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_STORAGE, 0x200)

#endif // _WINERROR_

/****** Storage types *******************************************************/


#define CWCSTORAGENAME 32

/* Storage instantiation modes */
#define STGM_DIRECT     0x00000000L
#define STGM_TRANSACTED     0x00010000L

#define STGM_READ       0x00000000L
#define STGM_WRITE      0x00000001L
#define STGM_READWRITE      0x00000002L

#define STGM_SHARE_DENY_NONE    0x00000040L
#define STGM_SHARE_DENY_READ    0x00000030L
#define STGM_SHARE_DENY_WRITE   0x00000020L
#define STGM_SHARE_EXCLUSIVE    0x00000010L

#define STGM_PRIORITY       0x00040000L
#define STGM_DELETEONRELEASE    0x04000000L

#define STGM_CREATE     0x00001000L
#define STGM_CONVERT        0x00020000L
#define STGM_FAILIFTHERE    0x00000000L

/* Storage commit types */
typedef enum tagSTGC
{
    STGC_DEFAULT = 0,
    STGC_OVERWRITE  = 1,
    STGC_ONLYIFCURRENT  = 2,
    STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE = 4
	,STGC_FORCELONG	=	2147483647
} STGC;

/* Stream name block definitions */
typedef char  *  *SNB;


#ifndef _WINDOWS_
#ifndef _FILETIME_
#define _FILETIME_
typedef struct FARSTRUCT tagFILETIME
{
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} FILETIME;
#endif
#endif


/* Storage stat buffer */

typedef struct FARSTRUCT tagSTATSTG
{
    char *	pwcsName;
	FSSpec		*pspec;
    unsigned long type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    unsigned long grfMode;
    unsigned long grfLocksSupported;
    CLSID clsid;
    unsigned long grfStateBits;
    unsigned long reserved;
} STATSTG;


/* Storage element types */
typedef enum tagSTGTY
{
    STGTY_STORAGE   = 1,
    STGTY_STREAM    = 2,
    STGTY_LOCKBYTES = 3,
    STGTY_PROPERTY  = 4
	,STGTY_FORCELONG	=	2147483647
} STGTY;

typedef enum tagSTREAM_SEEK
{
    STREAM_SEEK_SET = 0,
    STREAM_SEEK_CUR = 1,
    STREAM_SEEK_END = 2
	,STREAM_SEEK_FORCELONG	=	2147483647
} STREAM_SEEK;

typedef enum tagLOCKTYPE
{
    LOCK_WRITE      = 1,
    LOCK_EXCLUSIVE  = 2,
    LOCK_ONLYONCE   = 4
	,LOCKTYPE_FORCELONG	=	2147483647
} LOCKTYPE;

typedef enum tagSTGMOVE
{
    STGMOVE_MOVE    = 0,
    STGMOVE_COPY    = 1
	,STGMOVE_FORCELONG	=	2147483647
} STGMOVE;

typedef enum tagSTATFLAG
{
    STATFLAG_DEFAULT = 0,
    STATFLAG_NONAME = 1
	,STATFLAG_FORCELONG	=	2147483647
} STATFLAG;

/****** Storage Enumerators *************************************************/

#undef  INTERFACE
#define INTERFACE   IEnumSTATSTG

DECLARE_INTERFACE_(IEnumSTATSTG, IUnknown)
{
    BEGIN_INTERFACE
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    /* *** IENUMSTATSTG methods *** */
    STDMETHOD(Next) (THIS_ unsigned long celt, STATSTG  * rgelt, unsigned long  *pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumSTATSTG  * *ppenm) PURE;
};

typedef IEnumSTATSTG* LPENUMSTATSTG, ** LPLPENUMSTATSTG;



/****** ILockBytes Interface ************************************************/

#undef  INTERFACE
#define INTERFACE   ILockBytes

DECLARE_INTERFACE_(ILockBytes, IUnknown)
{
    BEGIN_INTERFACE

    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    /* *** ILockBytes methods *** */
    STDMETHOD(ReadAt) (THIS_ ULARGE_INTEGER ulOffset,
             void *pv,
             unsigned long cb,
             unsigned long  *pcbRead) PURE;
    STDMETHOD(WriteAt) (THIS_ ULARGE_INTEGER ulOffset,
#if !defined(__cplusplus)
              void *pv,
#else
              void const *pv,
#endif
              unsigned long cb,
              unsigned long  *pcbWritten) PURE;
    STDMETHOD(Flush) (THIS) PURE;
    STDMETHOD(SetSize) (THIS_ ULARGE_INTEGER cb) PURE;
    STDMETHOD(LockRegion) (THIS_ ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 unsigned long dwLockType) PURE;
    STDMETHOD(UnlockRegion) (THIS_ ULARGE_INTEGER libOffset,
                   ULARGE_INTEGER cb,
                 unsigned long dwLockType) PURE;
    STDMETHOD(Stat) (THIS_ STATSTG  *pstatstg, unsigned long grfStatFlag) PURE;
};

typedef ILockBytes * LPLOCKBYTES, ** LPLPLOCKBYTES;



/****** IStream Interface ***************************************************/


#undef  INTERFACE
#define INTERFACE   IStream

DECLARE_INTERFACE_(IStream, IUnknown)
{
    BEGIN_INTERFACE
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    /* *** IStream methods *** */
    STDMETHOD(Read) (THIS_ void *pv,
		     unsigned long cb, unsigned long  *pcbRead) PURE;
    STDMETHOD(Write) (THIS_ 
#if !defined(__cplusplus)
			void *pv,
#else
			void const *pv,
#endif
            unsigned long cb,
            unsigned long  *pcbWritten) PURE;
    STDMETHOD(Seek) (THIS_ LARGE_INTEGER dlibMove,
               unsigned long dwOrigin,
               ULARGE_INTEGER  *plibNewPosition) PURE;
    STDMETHOD(SetSize) (THIS_ ULARGE_INTEGER libNewSize) PURE;
    STDMETHOD(CopyTo) (THIS_ IStream  *pstm,
             ULARGE_INTEGER cb,
             ULARGE_INTEGER  *pcbRead,
             ULARGE_INTEGER  *pcbWritten) PURE;
    STDMETHOD(Commit) (THIS_ unsigned long grfCommitFlags) PURE;
    STDMETHOD(Revert) (THIS) PURE;
    STDMETHOD(LockRegion) (THIS_ ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 unsigned long dwLockType) PURE;
    STDMETHOD(UnlockRegion) (THIS_ ULARGE_INTEGER libOffset,
                 ULARGE_INTEGER cb,
                 unsigned long dwLockType) PURE;
    STDMETHOD(Stat) (THIS_ STATSTG  *pstatstg, unsigned long grfStatFlag) PURE;
    STDMETHOD(Clone)(THIS_ IStream  *  *ppstm) PURE;
};

//typedef IStream * LPSTREAM, ** LPLPSTREAM;

/****** IStorage Interface **************************************************/


#undef  INTERFACE
#define INTERFACE   IStorage

DECLARE_INTERFACE_(IStorage, IUnknown)
{
    BEGIN_INTERFACE
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    /* *** IStorage methods *** */
    STDMETHOD(CreateStream) (THIS_ const char * pwcsName,
                   unsigned long grfMode,
                   unsigned long reserved1,
                   unsigned long reserved2,
                   IStream  * *ppstm) PURE;
    STDMETHOD(OpenStream) (THIS_ const char * pwcsName,
		 void  *reserved1,
                 unsigned long grfMode,
                 unsigned long reserved2,
                 IStream  * *ppstm) PURE;
    STDMETHOD(CreateStorage) (THIS_ const char * pwcsName,
                unsigned long grfMode,
                unsigned long reserved1,
                unsigned long reserved2,
                IStorage  * *ppstg) PURE;
    STDMETHOD(OpenStorage) (THIS_ const char * pwcsName,
                  IStorage  *pstgPriority,
                  unsigned long grfMode,
                  SNB snbExclude,
                  unsigned long reserved,
                  IStorage  * *ppstg) PURE;
    STDMETHOD(CopyTo) (THIS_ unsigned long ciidExclude,
#if !defined(__cplusplus)
 		       IID *rgiidExclude,
#else
 		       IID const  *rgiidExclude,
#endif
 		       SNB snbExclude,
 		       IStorage  *pstgDest) PURE;
    STDMETHOD(MoveElementTo) (THIS_ 
						const char * lpszName,
    			    	IStorage  *pstgDest,
                       	const char  * lpszNewName,
                              unsigned long grfFlags) PURE;
    STDMETHOD(Commit) (THIS_ unsigned long grfCommitFlags) PURE;
    STDMETHOD(Revert) (THIS) PURE;
    STDMETHOD(EnumElements) (THIS_ unsigned long reserved1,
                 void  *reserved2,
                 unsigned long reserved3,
                 IEnumSTATSTG  * *ppenm) PURE;
    STDMETHOD(DestroyElement) (THIS_ const char * pwcsName) PURE;
    STDMETHOD(RenameElement) (THIS_ const char * pwcsOldName,
                const char * pwcsNewName) PURE;
    STDMETHOD(SetElementTimes) (THIS_ const char  *lpszName,
    			       			const FILETIME  *pctime,
                                const FILETIME  *patime,
                                const FILETIME  *pmtime) PURE;
    STDMETHOD(SetClass) (THIS_ REFCLSID clsid) PURE;
    STDMETHOD(SetStateBits) (THIS_ unsigned long grfStateBits, unsigned long grfMask) PURE;
    STDMETHOD(Stat) (THIS_ STATSTG  *pstatstg, unsigned long grfStatFlag) PURE;
};

//typedef IStorage * LPSTORAGE, ** LPLPSTORAGE;



/****** IRootStorage Interface **********************************************/

#undef  INTERFACE
#define INTERFACE   IRootStorage

DECLARE_INTERFACE_(IRootStorage, IUnknown)
{
    BEGIN_INTERFACE
    /* *** IUnknown methods *** */
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    /* *** IRootStorage methods *** */
    STDMETHOD(SwitchToFile) (THIS_ char * lpstrFile) PURE;
};

typedef IRootStorage * LPROOTSTORAGE, ** LPLPROOTSTORAGE;



/****** Storage API Prototypes ********************************************/

STDAPI StgCreateDocfile(const char * pwcsName,
            unsigned long grfMode,
            unsigned long reserved,
            IStorage  *  *ppstgOpen);
STDAPI StgCreateDocfileOnILockBytes(ILockBytes  *plkbyt,
                    unsigned long grfMode,
                    unsigned long reserved,
                    IStorage  *  *ppstgOpen);
STDAPI StgOpenStorage(const char * pwcsName,
              IStorage  *pstgPriority,
              unsigned long grfMode,
              SNB snbExclude,
              unsigned long reserved,
              IStorage  *  *ppstgOpen);
STDAPI StgOpenStorageOnILockBytes(ILockBytes  *plkbyt,
                  IStorage  *pstgPriority,
                  unsigned long grfMode,
                  SNB snbExclude,
                  unsigned long reserved,
                  IStorage  *  *ppstgOpen);
STDAPI StgIsStorageFile(const char * pwcsName);
STDAPI StgIsStorageILockBytes(ILockBytes * plkbyt);

STDAPI StgSetTimes( const char * lpszName,
          			const FILETIME * pctime,
                  	const FILETIME * patime,
                  	const FILETIME * pmtime);

// Use UTF8 Format to preserve UNICODE names through Macintosh's ANSI APIs

STDAPI UTF8ToUnicode(   const char* lpSrcStr,
						int cchSrc,
						wchar_t* lpDestStr,
						int cchDest);

STDAPI UnicodeToUTF8(   const wchar_t* lpSrcStr,
						int cchSrc,
						char* lpDestStr,
						int cchDest);

#endif /* __STORAGE__ */

