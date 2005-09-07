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

#include "ImplAAFCachePageAllocator.h"

#include "AAFResult.h"

#include "OMCachePageAllocator.h"

ImplAAFCachePageAllocator::ImplAAFCachePageAllocator ()
: _pAllocator(0)
{
}

ImplAAFCachePageAllocator::~ImplAAFCachePageAllocator ()
{
  delete _pAllocator;
}

AAFRESULT STDMETHODCALLTYPE
  ImplAAFCachePageAllocator::Initialize
    (aafUInt32  pageCount,
     aafUInt32  pageSize)
{
  if (_pAllocator != 0)
    return AAFRESULT_ALREADY_INITIALIZED;

  _pAllocator = new OMBuiltinCachePageAllocator(pageSize, pageCount);
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
  ImplAAFCachePageAllocator::Allocate(aafMemPtr_t *  pPtr)
{
  if (_pAllocator == 0)
    return AAFRESULT_NOT_INITIALIZED;

  try {
    aafMemPtr_t ptr = _pAllocator->allocate();
    *pPtr = ptr;
  } catch (...) {
    return AAFRESULT_NOMEMORY;
  }

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
  ImplAAFCachePageAllocator::Deallocate(aafMemPtr_t ptr)
{
  if (_pAllocator == 0)
    return AAFRESULT_NOT_INITIALIZED;

  _pAllocator->deallocate(ptr);
  return AAFRESULT_SUCCESS;
}
