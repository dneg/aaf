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
// Licensor of the AAF Association is Schema Software, Inc.
// All rights reserved.
//
//=---------------------------------------------------------------------=




/************************************************************************
* StructuredStorage.h
* Public Header file for Structured Storage library
*
* (c) Schema Software Inc., 2001-2004
* Authors: Bob Sabiston, Yuri Khramov, Mark Ambachtsheer
************************************************************************

$Revision$
$Date$
*/
#ifndef _StructuredStorage_h
#define _StructuredStorage_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef SSTG_PALMOS
#include <VFSMgr.h>      /* For definition of FileRef */
#endif /* SSTG_PALMOS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    /** Status codes -- return codes for open and create */
#define SSTG_OK 0
#define SSTG_ERROR_FILE_NOT_FOUND 1
#define SSTG_ERROR_ILLEGAL_ACCESS 2
#define SSTG_ERROR_FILE_NOT_SSTG  3
#define SSTG_ERROR_FILEIO 4
#define SSTG_ERROR_MEMORY 5
#define SSTG_ERROR_ILLEGAL_CALL 6
#define SSTG_ERROR_STG_LOCKED 7
#define SSTG_ERROR_CORRUPTED_FAT 8
#define SSTG_ERROR_NOT_STORAGE 9
#define SSTG_ERROR_NOT_STREAM 10
#define SSTG_ERROR_CHILD_NOT_FOUND 11
#define SSTG_ERROR_NOT_SUPPORTED 12
#define SSTG_ERROR_PROPERTY_NOT_FOUND 13
#define SSTG_ERROR_PROPERTY_FORMAT 14
#define SSTG_ERROR_END_OF_STREAM 15

    /* Access mode enumeration */
    typedef enum {STG_READ, STG_WRITE, STG_RW} StgMode;

#define STG_CANREAD( mode )  (( mode == STG_READ  ) || ( mode == STG_RW ))
#define STG_CANWRITE( mode ) (( mode == STG_WRITE ) || ( mode == STG_RW ))

    /* Define a standard 64-bit integer type.  Use #defines rather than 
     * typedefs so that the modifiers "unsigned" and "signed" can be used in 
     * conjunction with the resulting type. 
     * If this define does not work with your compiler, just change the 
     * definition here.
     */
#ifdef _MSC_VER /* Visual C++ */
    #define SSRW_INT64 __int64
#else /* !_MSC_VER, every other compiler */
    #define SSRW_INT64 long long
#endif /* !_MSC_VER */

#include "SsrwInputSource.h"

    /* Relative Seek position options enumeration */
    typedef enum {
        STG_START = 0,
        STG_CUR = 1,
        STG_END = 2 } SeekPos;
    /* Strucutred Storage Directory entry type enumeration */
    typedef enum { STORAGE, STREAM } StgDirType;
    /* Property IDs for for Summary Info
    we do not typedef them on purpose */
    enum
    {  PIDSI_TITLE = 0x02,           /* Title */
       PIDSI_SUBJECT =  0x00000003,  /* Subject */
       PIDSI_AUTHOR = 0x00000004,    /* Author */
       PIDSI_KEYWORDS = 0x00000005,  /* Keywords */
       PIDSI_COMMENTS = 0x00000006,  /* Comments */
       PIDSI_TEMPLATE = 0x00000007,  /* Template */
       PIDSI_LASTAUTHOR = 0x00000008,   /* Last Saved By */
       PIDSI_REVNUMBER = 0x00000009, /* Revision Number*/
       PIDSI_EDITTIME = 0x0000000A,  /* Total Editing Time */
       PIDSI_LASTPRINTED = 0x0000000B,  /* Last Printed */
       PIDSI_CREATE_DTM = 0x0000000C,   /* Create Time/Date */
       PIDSI_LASTSAVE_DTM = 0x0000000D,/* Last saved Time/Date */
       PIDSI_PAGECOUNT = 0x0000000E,
       PIDSI_WORDCOUNT = 0x0000000F,
       PIDSI_CHARCOUNT = 0x00000010,
       PIDSI_THUMBNAIL = 0x00000011,	/* used for previews by many applications */
       PIDSI_APPNAME = 0x00000012,      /* Name of Creating Application */
       PIDSI_SECURITY = 0x00000013
    };

    /* Property IDs for for Summary Info
    we do not typedef them on purpose */
    enum
    {
        PIDDSI_CATEGORY = 0x00000002,
        PIDDSI_PRESFORMAT = 0x00000003,
        PIDDSI_BYTECOUNT = 0x00000004,
        PIDDSI_LINECOUNT = 0x00000005,
        PIDDSI_PARCOUNT = 0x00000006,
        PIDDSI_SLIDECOUNT = 0x00000007,
        PIDDSI_NOTECOUNT = 0x00000008,
        PIDDSI_HIDDENCOUNT = 0x00000009,
        PIDDSI_MMCLIPCOUNT = 0x0000000A,
        PIDDSI_SCALE = 0x0000000B,
        /* not supported in this version
           PIDDSI_HEADINGPAIR = 0x0000000C,
           PIDDSI_DOCPARTS = 0x0000000D,
        */
        PIDDSI_MANAGER = 0x0000000E,
        PIDDSI_COMPANY = 0x0000000F,
        PIDDSI_LINKSDIRTY = 0x00000010
    };

    /* Enumeration of supported property streams */
    typedef enum
    {
        SUMMARY_INFO = 0x001,
        DOC_SUMMARY_INFO = 0x002
    } PropertyStreamType;

    /* Enumeration for type tags */
    typedef enum
    {
        INT2_TYPE,
        INT4_TYPE,
        STRING_TYPE,
        TIME_TYPE,
        CF_TYPE,
        BOOL_TYPE
    } TypeTag;

    /* just in case */
#define boolean char
#define SSRW_FALSE 0
#define SSRW_TRUE (!SSRW_FALSE)

    /* Structures keeping the information about SStorage objects
      - their members should not be used in the caller programs */
    struct _RootStorage;
    struct _Stream;
    struct _Storage;
    typedef struct _Stream Stream;
    typedef struct _Storage Storage;
    typedef struct _RootStorage RootStorage;

    /* Corresponding structures to Windows GUID, CLSID */
    typedef struct _SSRW_GUID
    {
        unsigned long data1;
        unsigned short data2;
        unsigned short data3;
        unsigned char data4[8];
    }
    SSRW_GUID;
    typedef SSRW_GUID SSRW_CLSID;

    /** Statistic data structure - contains information about storages and streams */
    typedef struct _StgInfo
    {
        wchar_t* sName;
        StgDirType type;
        unsigned long userFlags;
        unsigned SSRW_INT64 size;
        time_t creationTime;
        time_t modificationTime;
        StgMode accessMode;
        SSRW_CLSID clsid;
    }
    StgInfo;

    /* Prototype for a function pointer matching the standard C library's realloc
     * function definition, needed for creating structured storage files in buffers 
     */
    typedef void* (*SsrwReallocFunc)(
        void* in_ptr,
        size_t in_size);

    /* RootStorage object functions available for caller programs */

    extern int openStructuredStorage (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int openStructuredStorageEx (
            const wchar_t* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int openStructuredStorageInBuf (
            const void** in_ppBuf,
            unsigned long in_ulBufSize,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int openStructuredStorageInSource (
            SSRWIS* in_pSSSRWIS,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int getStorageFromRoot(
            RootStorage* in_pRoot,
            Storage** out_ppStorage);

    extern int closeStructuredStorage (
            RootStorage** in_pRoot);

    extern int structuredStorageGetSectorSize(
            const RootStorage* in_pRoot,
            unsigned long* out_pSectorSize);

#ifdef SSTG_PALMOS

    extern int openStructuredStorageInPalmStream (
            FileHand in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int openStructuredStorageInPalmVFSStream (
            FileRef in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int openStructuredStorageInVFS (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned short in_volRef);

#else /* !SSTG_PALMOS */

    extern int openStructuredStorageInStream (
            FILE* in_pFile,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

#endif /* !SSTG_PALMOS */

    /* Storage object funcions available for caller programs*/
    extern int openStorage (
            Storage* in_this,
            const wchar_t* in_name,
            Storage** out_ppChild);

    extern int openStream (
            Storage* in_this,
            const wchar_t* in_name,
            Stream** out_ppChild);

    extern int closeStorage (
            Storage** io_pThis);

    extern int getStorageInfo(
            Storage* in_this,
            StgInfo** out_ppInfo );

    extern int getChildrenInfo(
            Storage* in_this,
            StgInfo** out_paInfo,
            int* out_pCount );

    extern int destroy(
            Storage* io_pParent,
            const wchar_t* in_pName);

    extern int streamSetClass(
            Stream* io_this,
            const SSRW_CLSID* in_pCLSID);

    extern int storageSetClass(
            Storage* io_this,
            const SSRW_CLSID* in_pCLSID);

    /* Stream  object functions available for caller programs*/
    extern int streamSeek (
            Stream* in_this,
            long in_offset,
            SeekPos in_pos);

    extern int streamGetPos (
            Stream* in_this,
            unsigned long* out_offset );

    extern int streamGetBytesAvailableToRead(
            Stream* in_pStream,
            unsigned long* out_pOffsetFromEnd);

    extern int streamSeek64 (
            Stream* in_this,
            SSRW_INT64 in_offset,
            SeekPos in_pos);

    extern int streamGetPos64 (
            Stream* in_this,
            unsigned SSRW_INT64* out_offset);

    extern int streamGetBytesAvailableToRead64(
            Stream* in_pStream,
            unsigned SSRW_INT64* out_pOffsetFromEnd);

    extern int streamRead (
            Stream* in_this,
            void* out_buffer,
            unsigned long* in_pCount);

    extern int readLEshort(
            Stream* in_pStream,
            short* out_psVal);

    extern int readLEword(
            Stream* in_pStream,
            unsigned short* out_pwVal);

    extern int readLElong(
            Stream* in_pStream,
            long* out_plVal );

    extern int readLEdword(
            Stream* in_pStream,
            unsigned long* out_pdwVal);

    extern int readLEfloat(
            Stream* in_pStream,
            float* out_pfVal);

    extern int readLEdouble(
            Stream* in_pStream,
            double* out_pdVal);

    extern int readLEwchar(
            Stream* in_pStream,
            wchar_t* out_pwcVal);

    extern int readLEwstring(
            Stream* in_pStream,
            int* io_piCount,
            wchar_t* out_strString);

    extern int closeStream(
            Stream** io_ppStream);

    extern int getStreamInfo(
            Stream* in_pStream,
            StgInfo** out_ppInfo);

    /* StgInfo object functions */
    extern int freeInfo(
            StgInfo** io_paInfo,
            int in_count);

    /* SummaryInfo and DocumentSummaryInfo properties management */
    extern int getDocumentProperty(
            RootStorage* pRoot,
            PropertyStreamType streamId,
            int propertyId,
            TypeTag* pType,
            void** ppValue );

    extern int freeDocumentProperty(
            void** ppValue);

#ifndef SSRW_READ_ONLY

    extern int createStructuredStorage (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int createStructuredStorageEx (
            const wchar_t* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern int createStructuredStorageInBuf (
            void** in_ppBuf,
            unsigned long in_ulBufSize,
            SsrwReallocFunc in_pRealloc,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int createStructuredStorageInBufEx (
            void** in_ppBuf,
            unsigned long in_ulBufSize,
            SsrwReallocFunc in_pRealloc,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern int createStructuredStorageInSource (
            SSRWIS* in_pSSRWIS,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

#ifdef SSTG_PALMOS

    extern int createStructuredStorageInPalmStream (
            FileHand in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int createStructuredStorageInPalmStreamEx (
            FileHand in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern int createStructuredStorageInPalmVFSStream (
            FileRef in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int createStructuredStorageInPalmVFSStreamEx (
            FileRef in_file,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

    extern int createStructuredStorageInVFS (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned short in_volRef);

    extern int createStructuredStorageInVFSEx (
            const char* in_fileName,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned short in_volRef,
            unsigned long in_sectorSize);

#else /* !SSTG_PALMOS */

    extern int createStructuredStorageInStream (
            FILE* in_pFile,
            StgMode in_accessMode,
            RootStorage** out_ppRoot);

    extern int createStructuredStorageInStreamEx (
            FILE* in_pFile,
            StgMode in_accessMode,
            RootStorage** out_ppRoot,
            unsigned long in_sectorSize);

#endif /* !SSTG_PALMOS */

    extern int flushStream(Storage* in_pStorage,
                           const wchar_t* in_wszStreamName,
                           const void* in_pvBuf,
                           unsigned long* in_pcbBufSize);

    extern int createStorage (
            Storage* in_this,
            const wchar_t* in_name,
            Storage** out_ppChild);

    extern int createStream (
            Storage* in_this,
            const wchar_t* in_name,
            Stream** out_ppChild);

    extern int streamResize(
            Stream* in_pStream,
            SSRW_INT64 in_llNewSize); 

    extern int streamWrite(
            Stream* in_pStream,
            const void* in_pBuffer,
            unsigned long* io_pCount);

    extern int writeLEshort(
            Stream* in_pStream,
            short out_val);

    extern int writeLEword(
            Stream* in_pStream,
            unsigned short out_val);

    extern int writeLElong(
            Stream* in_pStream,
            long in_val);

    extern int writeLEdword(
            Stream* in_pStream,
            unsigned long in_val);

    extern int writeLEfloat(
            Stream* in_pStream,
            float in_val);

    extern int writeLEdouble(
            Stream* in_pStream,
            double in_val);

    extern int writeLEwchar(
            Stream* in_pStream,
            wchar_t in_val);

    extern int writeLEwstring(
            Stream* in_pStream,
            const wchar_t* in_wszString,
            int* io_piCount);

    extern int setDocumentProperty(
            RootStorage* pRoot,
            PropertyStreamType streamId,
            int propertyId,
            TypeTag srcType,
            const void* pValue);

    extern int setDefaultDocumentProperties(
            RootStorage* pRoot);

#endif /* !SSRW_READ_ONLY */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
