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
#include "OMMSStructuredStorage.h"

#include "OMAssertions.h"
#include "OMDynamicLibrary.h"

static OMDynamicLibrary* _ssLibrary = 0;

#if defined(OM_OS_WINDOWS)
#define LINKAGE __stdcall
wchar_t* ssLibraryName = L"ole32.dll";
wchar_t* coLibraryName = L"ole32.dll";
#elif defined(OM_OS_MACOS)
#define LINKAGE
wchar_t* ssLibraryName = L"Microsoft Structured Storage";
wchar_t* coLibraryName = L"Microsoft Component Library";
#elif defined(OM_OS_UNIX)
#define LINKAGE
wchar_t* ssLibraryName = L"StructuredStorage.so";
wchar_t* coLibraryName = L"StructuredStorage.so";
#endif

typedef OMInt32 (LINKAGE *pStgCreateDocfile_t)(const SSCHAR*,
                                               OMUInt32,
                                               OMUInt32,
                                               IStorage**);
typedef OMInt32 (LINKAGE *pStgCreateDocfileOnILockBytes_t)(ILockBytes*,
                                                           OMUInt32,
                                                           OMUInt32,
                                                           IStorage**);
typedef OMInt32 (LINKAGE *pStgOpenStorage_t)(SSCHAR*,
                                             IStorage*,
                                             OMUInt32,
                                             SSCHAR**,
                                             OMUInt32,
                                             IStorage**);
typedef OMInt32 (LINKAGE *pStgOpenStorageOnILockBytes_t)(ILockBytes*,
                                                         IStorage*,
                                                         OMUInt32,
                                                         SSCHAR**,
                                                         OMUInt32,
                                                         IStorage**);
typedef OMInt32 (LINKAGE *pStgIsStorageFile_t)(const SSCHAR*);

pStgCreateDocfile_t pStgCreateDocfile = 0;
pStgCreateDocfileOnILockBytes_t pStgCreateDocfileOnILockBytes = 0;
pStgOpenStorage_t pStgOpenStorage = 0;
pStgOpenStorageOnILockBytes_t pStgOpenStorageOnILockBytes = 0;
pStgIsStorageFile_t pStgIsStorageFile = 0;

OMInt32 OMStgCreateDocfile(const SSCHAR* pwcsName,
                           OMUInt32 grfMode,
                           OMUInt32 reserved,
                           IStorage** ppstgOpen)
{
  TRACE("OMStgCreateDocfile");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);

  if (pStgCreateDocfile == 0) {
    pStgCreateDocfile = (pStgCreateDocfile_t)_ssLibrary->findSymbol(
                                                           "StgCreateDocfile");
  }
  ASSERT("Symbol found", pStgCreateDocfile != 0);
  OMInt32 result = (*pStgCreateDocfile)(pwcsName,
                                        grfMode,
                                        reserved,
                                        ppstgOpen);
  return result;
}

OMInt32 OMStgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                       OMUInt32 grfMode,
                                       OMUInt32 reserved,
                                       IStorage** ppstgOpen)
{
  TRACE("OMStgCreateDocfileOnILockBytes");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);

  if (pStgCreateDocfileOnILockBytes == 0) {
    pStgCreateDocfileOnILockBytes =
                       (pStgCreateDocfileOnILockBytes_t)_ssLibrary->findSymbol(
                                               "StgCreateDocfileOnILockBytes");
  }
  ASSERT("Symbol found", pStgCreateDocfileOnILockBytes != 0);
  OMInt32 result = (*pStgCreateDocfileOnILockBytes)(plkbyt,
                                                    grfMode,
                                                    reserved,
                                                    ppstgOpen);
  return result;
}

OMInt32 OMStgOpenStorage(SSCHAR* pwcsName,
                         IStorage* pstgPriority,
                         OMUInt32 grfMode,
                         SSCHAR** snbExclude,
                         OMUInt32 reserved,
                         IStorage** ppstgOpen)
{
  TRACE("OMStgOpenStorage");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);

  if (pStgOpenStorage == 0) {
    pStgOpenStorage = (pStgOpenStorage_t)_ssLibrary->findSymbol(
                                                             "StgOpenStorage");
  }
  ASSERT("Symbol found", pStgOpenStorage!= 0);
  OMInt32 result = (*pStgOpenStorage)(pwcsName,
                                      pstgPriority,
                                      grfMode,
                                      snbExclude,
                                      reserved,
                                      ppstgOpen);
  return result;
}

OMInt32 OMStgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen)
{
  TRACE("OMStgOpenStorageOnILockBytes");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);

  if (pStgOpenStorageOnILockBytes == 0) {
    pStgOpenStorageOnILockBytes =
                         (pStgOpenStorageOnILockBytes_t)_ssLibrary->findSymbol(
                                                 "StgOpenStorageOnILockBytes");
  }
  ASSERT("Symbol found", pStgOpenStorageOnILockBytes != 0);
  OMInt32 result = (*pStgOpenStorageOnILockBytes)(plkbyt,
                                                  pstgPriority,
                                                  grfMode,
                                                  snbExclude,
                                                  reserved,
                                                  ppstgOpen);
  return result;
}

OMInt32 OMStgIsStorageFile(const SSCHAR* pwcsName)
{
  TRACE("OMStgIsStorageFile");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);

  if (pStgIsStorageFile == 0) {
    pStgIsStorageFile = (pStgIsStorageFile_t)_ssLibrary->findSymbol(
                                                           "StgIsStorageFile");
  }
  ASSERT("Symbol found", pStgIsStorageFile!= 0);
  OMInt32 result = (*pStgIsStorageFile)(pwcsName);
  return result;
}

static OMDynamicLibrary* _coLibrary = 0;

typedef OMInt32 (LINKAGE *pCoInitialize_t)(void*);
typedef void (LINKAGE *pCoUninitialize_t)(void);

pCoInitialize_t pCoInitialize = 0;
pCoUninitialize_t pCoUninitialize = 0;

OMInt32 OMCoInitialize(void* pvReserved)
{
  TRACE("OMCoInitialize");
  if (_coLibrary == 0) {
    _coLibrary = OMDynamicLibrary::loadLibrary(coLibraryName);
  }
  ASSERT("Library loaded", _coLibrary != 0);

  if (pCoInitialize == 0) {
    pCoInitialize = (pCoInitialize_t)_coLibrary->findSymbol("CoInitialize");
  }
  ASSERT("Symbol found", pCoInitialize!= 0);
  OMInt32 result = (*pCoInitialize)(pvReserved);
  return result;
}

void OMCoUninitialize(void)
{
  TRACE("OMCoUninitialize");
  if (_coLibrary == 0) {
    _coLibrary = OMDynamicLibrary::loadLibrary(coLibraryName);
  }
  ASSERT("Library loaded", _coLibrary != 0);

  if (pCoUninitialize == 0) {
    pCoUninitialize = (pCoUninitialize_t)_coLibrary->findSymbol(
                                                             "CoUninitialize");
  }
  ASSERT("Symbol found", pCoUninitialize!= 0);
  (*pCoUninitialize)();
}


