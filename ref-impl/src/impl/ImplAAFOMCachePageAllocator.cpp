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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFOMCachePageAllocator.h"

#include "OMCachePageAllocator.h"

#include "AAF.h"
#include "AAFResult.h"
#include "OMAssertions.h"

#include <new>

ImplAAFOMCachePageAllocator::ImplAAFOMCachePageAllocator(
                                            IAAFCachePageAllocator* pAllocator,
                                            aafUInt32 pageSize,
                                            aafUInt32 pageCount)
: OMCachePageAllocator(pageCount, pageSize),
  _pAllocator(pAllocator)
{
  ASSERTU(_pAllocator != 0);
  _pAllocator->AddRef();
}

ImplAAFOMCachePageAllocator::~ImplAAFOMCachePageAllocator()
{
  ASSERTU(_pAllocator != 0);
  _pAllocator->Release();
  _pAllocator = 0;
}

OMByte* ImplAAFOMCachePageAllocator::allocate(void)
{
  aafMemPtr_t ptr = 0;
  HRESULT hr = _pAllocator->Allocate(&ptr);
  if (!AAFRESULT_SUCCEEDED(hr)) {
    throw std::bad_alloc();
  }
  return ptr;
}

void ImplAAFOMCachePageAllocator::deallocate(OMByte* page)
{
  (void)_pAllocator->Deallocate(page);
}
