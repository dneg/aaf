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

// @doc OMEXTERNAL
#ifndef OMMSSTRUCTUREDSTORAGE_H
#define OMMSSTRUCTUREDSTORAGE_H

#include <time.h>
#include "OMDataTypes.h"

// @module OMMSStructuredStorage | Interface to various implementations
//         of Microsoft Structured Storage.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

// Use the standard implementation on MS Windows platforms (ole32.dll).
//
// The Microsoft reference implementation is broken and will not be
// fixed, so no Microsoft implementation is available for other platforms.
// However, the definitions in the reference implementation are still
// needed by the OM.

#if defined(OM_OS_WINDOWS)
#define OM_USE_WINDOWS_SS
#elif defined(OM_OS_UNIX)
#define OM_USE_REFERENCE_SS
#endif

// Each Microsoft supplied Structured Storage implementation requires
// us to include different header files.
//
#if defined(OM_USE_WINDOWS_SS)
#include <objbase.h>
#elif defined (OM_USE_REFERENCE_SS)
#include "h/storage.h"
#else
#error "Don't know which structured storage implementation to use."
#endif

// redefine STGOPTIONS so that it is available on all platforms
typedef struct tagOM_STGOPTIONS
{
  USHORT      usVersion;
  USHORT      reserved;
  ULONG       ulSectorSize;
  const WCHAR *pwcsTemplateFile;
} OM_STGOPTIONS;



// Determine whether or not UNICODE versions of the APIs are in use.
//
#if defined(OM_OS_WINDOWS) && defined(UNICODE)
#define OM_UNICODE_APIS
#endif

// SSCHAR is used in client code for all character (string) arguments
// to functions whose prototype changes when UNICODE is defined.
//
#if defined(OM_UNICODE_APIS)
typedef wchar_t SSCHAR;
#else
typedef char SSCHAR;
#endif

#if defined(OM_USE_REFERENCE_SS)

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

/*
* Formula from From Microsoft FAQ Q167296
* Under UNIX platforms, file times are maintained in the form of a ANSI C
* runtime arithmetic type named 'time_t', which represents seconds since
* midnight January 1, 1970 UTC (coordinated universal time).
*
* Under Win32 platforms, file times are maintained primarily in the form of a
* 64-bit FILETIME structure, which represents the number of 100-nanosecond
* intervals since January 1, 1601 UTC (coordinate universal time).
*
*/

// Portable way to define 64 bit constants
#ifdef _MSC_VER
#define UINT64_C(c)	c
#else
#define UINT64_C(c) c##LL
// Also, provide a replacement Int32x32To64() for non-MSVC compilers
static inline OMInt64 Int32x32To64 (DWORD multiplier, DWORD multiplicand)
{
	return (OMInt64)multiplier * (OMInt64)multiplicand;
}
#endif

const OMInt64 SECS_TO_100NS = 10000000;
const OMInt64 NS100_BETWEEN_EPOCHS = UINT64_C(116444736000000000);

static inline void convertUnixTimeToFileTime (time_t ut, FILETIME *ft)
{
	OMInt64 ll = Int32x32To64(ut, SECS_TO_100NS) + NS100_BETWEEN_EPOCHS;
	ft->dwLowDateTime = (DWORD)ll;
	ft->dwHighDateTime = (DWORD)(ll >> 32);
}

static inline void convertFileTimeToUnixTime (const FILETIME *ft, time_t *ut)
{
	// first convert filetime to long long in 100ns
	OMInt64 ll = ((OMInt64)ft->dwHighDateTime << 32) + ft->dwLowDateTime;


	// then convert to seconds in units of 100ns since unix epocs
	ll -= NS100_BETWEEN_EPOCHS;

	// convert it to seconds from Win Epoch
	ll /= SECS_TO_100NS;
	*ut = (time_t)ll;
}

// Initialization and finalization.
//
void OMMSSInitialize(void);

void OMMSSFinalize(void);

#if defined(OM_DYNAMIC_SS)
// For dynamic linking to the Structured Storage library redirect
// the following calls to wrapper functions.
//
#define StgCreateDocfile OMStgCreateDocfile
#define StgCreateDocfileOnILockBytes OMStgCreateDocfileOnILockBytes
#define StgOpenStorage OMStgOpenStorage
#define StgOpenStorageOnILockBytes OMStgOpenStorageOnILockBytes
#define StgIsStorageFile OMStgIsStorageFile

#define CoInitialize OMCoInitialize
#define CoUninitialize OMCoUninitialize
#endif

OMInt32 OMStgCreateDocfile(const SSCHAR* pwcsName,
                           OMUInt32 grfMode,
                           OMUInt32 reserved,
                           IStorage** ppstgOpen);

OMInt32 OMStgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                       OMUInt32 grfMode,
                                       OMUInt32 reserved,
                                       IStorage** ppstgOpen);

OMInt32 OMStgOpenStorage(SSCHAR* pwcsName,
                         IStorage* pstgPriority,
                         OMUInt32 grfMode,
                         SSCHAR** snbExclude,
                         OMUInt32 reserved,
                         IStorage** ppstgOpen);

OMInt32 OMStgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen);

OMInt32 OMStgIsStorageFile(const SSCHAR* pwcsName);


OMInt32 OMCoInitialize(void* pvReserved);

void OMCoUninitialize(void);



#ifdef OM_USE_STORAGE_EX

// This function does not exist in the current MS Structured Storage library
// but is required to create 4K files through the raw interface.
// Therefore the function is simulated in OMMSStructuredStorage.cpp
// 040109 Ian Baker Metaglue Corp.
OMInt32 StgCreateDocfileOnILockBytesEx (
										ILockBytes* plkbyt,
										DWORD grfMode,
										DWORD stgfmt,
										DWORD grfAttrs,
										OM_STGOPTIONS* pStgOptions,
										void* reserved2,
										REFIID riid,
										void** ppObjectOpen   );


#endif // OM_USE_STORAGE_EX

#endif
