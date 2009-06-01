//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include<stdio.h>
#include <assert.h>
#include <string.h>

class TestCachePageAllocator : public IAAFCachePageAllocator {
public:
  // Defeat gcc warning about private ctor/dtor and no friends
  // Note that this dummy function cannot itself be called because
  // it requires a constructed TestCachePageAllocator object.
  friend void dummyFriend(TestCachePageAllocator);

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppIfc);

  aafUInt32 STDMETHODCALLTYPE AddRef();

  aafUInt32 STDMETHODCALLTYPE Release();

  HRESULT STDMETHODCALLTYPE Initialize(aafUInt32  pageCount,
                                       aafUInt32  pageSize);

  HRESULT STDMETHODCALLTYPE Allocate(aafMemPtr_t *  pPtr);

  HRESULT STDMETHODCALLTYPE Deallocate(aafMemPtr_t  ptr);

  static HRESULT Create(IAAFCachePageAllocator** ppAllocator);

private:

  TestCachePageAllocator();

  virtual ~TestCachePageAllocator();

  aafUInt32 _referenceCount;

  aafUInt32 _pageSize;
};

HRESULT TestCachePageAllocator::QueryInterface(REFIID iid, void ** ppIfc)
{
  if (ppIfc == 0)
    return AAFRESULT_NULL_PARAM;

  if (memcmp(&iid, &IID_IUnknown, sizeof(IID)) == 0) {
    IUnknown* unk = (IUnknown*) this;
    *ppIfc = (void*) unk;
    AddRef ();
    return AAFRESULT_SUCCESS; 
  } else if (memcmp(&iid, &IID_IAAFCachePageAllocator, sizeof(IID)) == 0) {
    IAAFCachePageAllocator* cpa = this;
    *ppIfc = (void*) cpa;
    AddRef ();
    return AAFRESULT_SUCCESS;
  } else {
    return E_NOINTERFACE;
  }
}

aafUInt32 TestCachePageAllocator::AddRef()
{
  return ++_referenceCount;
}

aafUInt32 TestCachePageAllocator::Release()
{
  aafUInt32 r = --_referenceCount;
  if (r == 0) {
    delete this;
  }
  return r;
}

HRESULT TestCachePageAllocator::Initialize(aafUInt32 /* pageCount */,
                                           aafUInt32 pageSize)
{
  _pageSize = pageSize;
  return AAFRESULT_SUCCESS;
}

HRESULT TestCachePageAllocator::Allocate(aafMemPtr_t *  pPtr)
{
  HRESULT hr = AAFRESULT_SUCCESS;
  aafMemPtr_t result = 0;
  try {
    result = new aafUInt8[_pageSize];
  } catch (...) {
    return AAFRESULT_NOMEMORY;
  }
  if (result != 0) {
    *pPtr = result;
  } else {
    hr = AAFRESULT_NOMEMORY;
  }
  return hr;
}

HRESULT TestCachePageAllocator::Deallocate(aafMemPtr_t  ptr)
{
  delete [] ptr;
  return AAFRESULT_SUCCESS;
}

TestCachePageAllocator::TestCachePageAllocator()
: _referenceCount(0),
  _pageSize(0)
{
}

HRESULT TestCachePageAllocator::Create(IAAFCachePageAllocator** ppAllocator)
{
  if (ppAllocator == 0)
    return AAFRESULT_NULL_PARAM;

  IAAFCachePageAllocator* result = new TestCachePageAllocator();
  if (result == 0)
    return AAFRESULT_NOMEMORY;

  result->AddRef();
  *ppAllocator = result;
  return AAFRESULT_SUCCESS;
}

TestCachePageAllocator::~TestCachePageAllocator()
{
  assert(_referenceCount == 0);
}

extern "C" HRESULT CAAFCachePageAllocator_test(testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

HRESULT CAAFCachePageAllocator_test(testMode_t /* mode */,
    aafUID_t fileKind,
    testRawStorageType_t /* rawStorageType */,
    aafProductIdentification_t productID)
{
  const size_t fileNameBufLen = 128;
  aafWChar pFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

  (void)RemoveTestFile(pFileName );

  IAAFRawStorage* pRawStorage = 0;
  HRESULT hr = AAFCreateRawStorageDisk(pFileName,
                                       kAAFFileExistence_new,
                                       kAAFFileAccess_modify,
                                       &pRawStorage);
  if (!AAFRESULT_SUCCEEDED(hr)) return hr;

  IAAFCachePageAllocator* pAllocator = 0;
  hr = TestCachePageAllocator::Create(&pAllocator);
  if (!AAFRESULT_SUCCEEDED(hr)) {
    pRawStorage->Release();
    pRawStorage = 0;
    return hr;
  }

  IAAFRawStorage* pCachedRawStorage = 0;
  hr = AAFCreateRawStorageCached2(pRawStorage,
                                  16,
                                  4096,
                                  pAllocator,
                                  &pCachedRawStorage);
  if (!AAFRESULT_SUCCEEDED(hr)) {
    pRawStorage->Release();
    pRawStorage = 0;
    pAllocator->Release();
    pAllocator = 0;
    return hr;
  }
  aafUID_t encoding = EffectiveTestFileEncoding(fileKind);
  IAAFFile* pFile = 0;
  hr = AAFCreateAAFFileOnRawStorage(pCachedRawStorage,
                                    kAAFFileExistence_new,
                                    kAAFFileAccess_modify,
                                    &encoding,
                                    0,
                                    &productID,
                                    &pFile);
  if (!AAFRESULT_SUCCEEDED(hr)) {
    pRawStorage->Release();
    pRawStorage = 0;
    pAllocator->Release();
    pAllocator = 0;
    pCachedRawStorage->Release();
    pCachedRawStorage = 0;
    return hr;
  }
  hr = pFile->Open();
  if (!AAFRESULT_SUCCEEDED(hr)) {
    pRawStorage->Release();
    pRawStorage = 0;
    pAllocator->Release();
    pAllocator = 0;
    pCachedRawStorage->Release();
    pCachedRawStorage = 0;
    pFile->Release();
    pFile = 0;
    return hr;
  }

  hr = pFile->Save();
  if (!AAFRESULT_SUCCEEDED(hr)) return hr;
  hr = pFile->Close();
  if (!AAFRESULT_SUCCEEDED(hr)) return hr;
  pRawStorage->Release();
  pRawStorage = 0;
  pAllocator->Release();
  pAllocator = 0;
  pCachedRawStorage->Release();
  pCachedRawStorage = 0;
  pFile->Release();
  pFile = 0;
  return hr;
}
