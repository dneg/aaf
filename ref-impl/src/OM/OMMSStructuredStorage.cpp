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

#include "OMSingleton.h"

#if defined(OM_OS_WINDOWS)
#define LINKAGE __stdcall
wchar_t* ssLibraryName = L"ole32.dll";
wchar_t* coLibraryName = L"ole32.dll";
#elif defined(OM_OS_MACOS)
#define LINKAGE
wchar_t* ssLibraryName = L"Microsoft Structured Storage";
wchar_t* coLibraryName = L"Microsoft Component Library";
#elif defined(OM_OS_MACOSX)
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

class MSSSLibrary;

class MSSSLibrary : public OMSingleton<MSSSLibrary> {
public:

  MSSSLibrary(void);

  OMInt32 StgCreateDocfile(const SSCHAR* pwcsName,
                           OMUInt32 grfMode,
                           OMUInt32 reserved,
                           IStorage** ppstgOpen);

  OMInt32 StgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                       OMUInt32 grfMode,
                                       OMUInt32 reserved,
                                       IStorage** ppstgOpen);

  OMInt32 StgOpenStorage(SSCHAR* pwcsName,
                         IStorage* pstgPriority,
                         OMUInt32 grfMode,
                         SSCHAR** snbExclude,
                         OMUInt32 reserved,
                         IStorage** ppstgOpen);

  OMInt32 StgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen);

  OMInt32 StgIsStorageFile(const SSCHAR* pwcsName);

private:

  OMDynamicLibrary* library(void);

  OMDynamicLibrary* _ssLibrary;
  pStgCreateDocfile_t _pStgCreateDocfile;
  pStgCreateDocfileOnILockBytes_t _pStgCreateDocfileOnILockBytes ;
  pStgOpenStorage_t _pStgOpenStorage;
  pStgOpenStorageOnILockBytes_t _pStgOpenStorageOnILockBytes;
  pStgIsStorageFile_t _pStgIsStorageFile;
};

MSSSLibrary::MSSSLibrary(void)
: _ssLibrary(0),
  _pStgCreateDocfile(0),
  _pStgCreateDocfileOnILockBytes(0),
  _pStgOpenStorage(0),
  _pStgOpenStorageOnILockBytes(0),
  _pStgIsStorageFile(0)
{
}

OMInt32 MSSSLibrary::StgCreateDocfile(const SSCHAR* pwcsName,
                                      OMUInt32 grfMode,
                                      OMUInt32 reserved,
                                      IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgCreateDocfile");

  if (_pStgCreateDocfile == 0) {
    _pStgCreateDocfile = (pStgCreateDocfile_t)library()->findSymbol(
                                                           "StgCreateDocfile");
  }
  ASSERT("Symbol found", _pStgCreateDocfile != 0);
  OMInt32 result = (*_pStgCreateDocfile)(pwcsName,
                                        grfMode,
                                        reserved,
                                        ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                                  OMUInt32 grfMode,
                                                  OMUInt32 reserved,
                                                  IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgCreateDocfileOnILockBytes");

  if (_pStgCreateDocfileOnILockBytes == 0) {
    _pStgCreateDocfileOnILockBytes =
                       (pStgCreateDocfileOnILockBytes_t)library()->findSymbol(
                                               "StgCreateDocfileOnILockBytes");
  }
  ASSERT("Symbol found", _pStgCreateDocfileOnILockBytes != 0);
  OMInt32 result = (*_pStgCreateDocfileOnILockBytes)(plkbyt,
                                                    grfMode,
                                                    reserved,
                                                    ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgOpenStorage(SSCHAR* pwcsName,
                                    IStorage* pstgPriority,
                                    OMUInt32 grfMode,
                                    SSCHAR** snbExclude,
                                    OMUInt32 reserved,
                                    IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgOpenStorage");

  if (_pStgOpenStorage == 0) {
    _pStgOpenStorage = (pStgOpenStorage_t)library()->findSymbol(
                                                             "StgOpenStorage");
  }
  ASSERT("Symbol found", _pStgOpenStorage!= 0);
  OMInt32 result = (*_pStgOpenStorage)(pwcsName,
                                      pstgPriority,
                                      grfMode,
                                      snbExclude,
                                      reserved,
                                      ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen)
{
  TRACE("MSSSLibrary::StgOpenStorageOnILockBytes");

  if (_pStgOpenStorageOnILockBytes == 0) {
    _pStgOpenStorageOnILockBytes =
                         (pStgOpenStorageOnILockBytes_t)library()->findSymbol(
                                                 "StgOpenStorageOnILockBytes");
  }
  ASSERT("Symbol found", _pStgOpenStorageOnILockBytes != 0);
  OMInt32 result = (*_pStgOpenStorageOnILockBytes)(plkbyt,
                                                  pstgPriority,
                                                  grfMode,
                                                  snbExclude,
                                                  reserved,
                                                  ppstgOpen);
  return result;
}

OMInt32 MSSSLibrary::StgIsStorageFile(const SSCHAR* pwcsName)
{
  TRACE("MSSSLibrary::StgIsStorageFile");

  if (_pStgIsStorageFile == 0) {
    _pStgIsStorageFile = (pStgIsStorageFile_t)library()->findSymbol(
                                                           "StgIsStorageFile");
  }
  ASSERT("Symbol found", _pStgIsStorageFile!= 0);
  OMInt32 result = (*_pStgIsStorageFile)(pwcsName);
  return result;
}

OMDynamicLibrary* MSSSLibrary::library(void)
{
  TRACE("MSSSLibrary::library");
  if (_ssLibrary == 0) {
    _ssLibrary = OMDynamicLibrary::loadLibrary(ssLibraryName);
  }
  ASSERT("Library loaded", _ssLibrary != 0);
  return _ssLibrary;
}

OMInt32 OMStgCreateDocfile(const SSCHAR* pwcsName,
                           OMUInt32 grfMode,
                           OMUInt32 reserved,
                           IStorage** ppstgOpen)
{
  TRACE("OMStgCreateDocfile");
  return MSSSLibrary::instance()->StgCreateDocfile(pwcsName,
                                                   grfMode,
                                                   reserved,
                                                   ppstgOpen);
}

OMInt32 OMStgCreateDocfileOnILockBytes(ILockBytes* plkbyt,
                                       OMUInt32 grfMode,
                                       OMUInt32 reserved,
                                       IStorage** ppstgOpen)
{
  TRACE("OMStgCreateDocfileOnILockBytes");
  return MSSSLibrary::instance()->StgCreateDocfileOnILockBytes(plkbyt,
                                                               grfMode,
                                                               reserved,
                                                               ppstgOpen);
}

OMInt32 OMStgOpenStorage(SSCHAR* pwcsName,
                         IStorage* pstgPriority,
                         OMUInt32 grfMode,
                         SSCHAR** snbExclude,
                         OMUInt32 reserved,
                         IStorage** ppstgOpen)
{
  return MSSSLibrary::instance()->StgOpenStorage(pwcsName,
                                                 pstgPriority,
                                                 grfMode,
                                                 snbExclude,
                                                 reserved,
                                                 ppstgOpen);
}

OMInt32 OMStgOpenStorageOnILockBytes(ILockBytes* plkbyt,
                                     IStorage* pstgPriority,
                                     OMUInt32 grfMode,
                                     SSCHAR** snbExclude,
                                     OMUInt32 reserved,
                                     IStorage** ppstgOpen)
{
  TRACE("OMStgOpenStorageOnILockBytes");
  return MSSSLibrary::instance()->StgOpenStorageOnILockBytes(plkbyt,
                                                             pstgPriority,
                                                             grfMode,
                                                             snbExclude,
                                                             reserved,
                                                             ppstgOpen);
}

OMInt32 OMStgIsStorageFile(const SSCHAR* pwcsName)
{
  TRACE("OMStgIsStorageFile");
  return MSSSLibrary::instance()->StgIsStorageFile(pwcsName);
}

typedef OMInt32 (LINKAGE *pCoInitialize_t)(void*);
typedef void (LINKAGE *pCoUninitialize_t)(void);

class MSCOMLibrary;

class MSCOMLibrary : public OMSingleton<MSCOMLibrary> {
public:

  MSCOMLibrary(void);

  OMInt32 CoInitialize(void* pvReserved);

  void CoUninitialize(void);

private:

  OMDynamicLibrary* library(void);

  OMDynamicLibrary* _coLibrary;
  pCoInitialize_t _pCoInitialize;
  pCoUninitialize_t _pCoUninitialize;
};

MSCOMLibrary::MSCOMLibrary(void)
: _coLibrary(0),
  _pCoInitialize(0),
  _pCoUninitialize(0)
{
}

OMInt32 MSCOMLibrary::CoInitialize(void* pvReserved)
{
  TRACE("MSCOMLibrary::CoInitialize");

  if (_pCoInitialize == 0) {
    _pCoInitialize = (pCoInitialize_t)library()->findSymbol("CoInitialize");
  }
  ASSERT("Symbol found", _pCoInitialize!= 0);
  OMInt32 result = (*_pCoInitialize)(pvReserved);
  return result;
}

void MSCOMLibrary::CoUninitialize(void)
{
  TRACE("MSCOMLibrary::OMCoUninitialize");

  if (_pCoUninitialize == 0) {
    _pCoUninitialize = (pCoUninitialize_t)library()->findSymbol(
                                                             "CoUninitialize");
  }
  ASSERT("Symbol found", _pCoUninitialize!= 0);
  (*_pCoUninitialize)();
}

OMDynamicLibrary* MSCOMLibrary::library(void)
{
  TRACE("MSCOMLibrary::library");
  if (_coLibrary == 0) {
    _coLibrary = OMDynamicLibrary::loadLibrary(coLibraryName);
  }
  ASSERT("Library loaded", _coLibrary != 0);
  return _coLibrary;
}

OMInt32 OMCoInitialize(void* pvReserved)
{
  TRACE("OMCoInitialize");
  return MSCOMLibrary::instance()->CoInitialize(pvReserved);
}

void OMCoUninitialize(void)
{
  TRACE("OMCoUninitialize");
  MSCOMLibrary::instance()->CoUninitialize();
}


