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

// @doc OMEXTERNAL
#ifndef OMMSSTRUCTUREDSTORAGE_H
#define OMMSSTRUCTUREDSTORAGE_H

#include "OMDataTypes.h"

// @module OMMSStructuredStorage | Interface to various implementations
//         of Microsoft Structured Storage.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

// Figure out which Microsoft supplied Structured Storage implementation
// to use. Here there are three different implementations to consider.
//
// 1) The standard implementation on Windows platforms
//    for which OM_WINDOWS_SS is defined
// 2) The implementation on Macintosh
//    for which OM_MACINTOSH_SS is defined
// 3) The reference implementation
//    for which OM_REFERENCE_SS is defined
//
#if defined(OM_OS_WINDOWS)
#define OM_WINDOWS_SS
#elif defined(OM_OS_MACOS)
#define OM_MACINTOSH_SS
#elif defined(OM_OS_MACOSX)
#define OM_MACINTOSH_SS
#elif defined(OM_OS_UNIX)
#define OM_REFERENCE_SS
#endif

// Each Microsoft supplied Structured Storage implementation requires
// us to include different header files.
// 
#if defined(OM_WINDOWS_SS)
#include <objbase.h>
#elif defined(OM_MACINTOSH_SS)
#include "wintypes.h"
#define NO_INCLUDES
#include "macdef.h"
#include "macpub.h"
#include "compobj.h"
#include "storage.h"
#elif defined (OM_REFERENCE_SS)
#include "h/storage.h"
#else
#error "Don't know which structured storage implementation to use."
#endif

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

#if defined(OM_MACINTOSH_SS) || defined(OM_REFERENCE_SS)

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
  result.HighPart = (unsigned long)(((OMUInt64)x) >> 32);
  result.LowPart  = (unsigned long) (0x00000000FFFFFFFF & (OMUInt64)x);
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

#endif
