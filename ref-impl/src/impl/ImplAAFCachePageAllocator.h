#ifndef __ImplAAFCachePageAllocator_h__
#define __ImplAAFCachePageAllocator_h__
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

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

class OMCachePageAllocator;

class ImplAAFCachePageAllocator : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCachePageAllocator ();

protected:
  virtual ~ImplAAFCachePageAllocator ();

public:
  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE Initialize (aafUInt32  pageCount,
                                                  aafUInt32  pageSize);

  //****************
  // Allocate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE Allocate(aafMemPtr_t *  pPtr);

  //****************
  // Deallocate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE Deallocate(aafMemPtr_t ptr);

private:
  OMCachePageAllocator* _pAllocator;
};

#endif // ! __ImplAAFCachePageAllocator_h__
