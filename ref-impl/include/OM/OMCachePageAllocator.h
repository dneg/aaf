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

// @doc OMEXTERNAL
#ifndef OMCACHEPAGEALLOCATOR_H
#define OMCACHEPAGEALLOCATOR_H

#include "OMDataTypes.h"

class OMCachePageAllocator {
public:
  // @access Public members.

    // @cmember Constructor.
  OMCachePageAllocator(OMUInt32 pageSize, OMUInt32 pageCount);

    // @cmember Destructor.
  virtual ~OMCachePageAllocator(void);

    // @cmember Allocate a page.
  virtual OMByte* allocate(void) = 0;

    // @cmember Deallocate a page.
  virtual void deallocate(OMByte* page) = 0;

protected:

  OMUInt32 _pageSize;
  OMUInt32 _pageCount;

};

class OMBuiltinCachePageAllocator : public OMCachePageAllocator {
public:
  // @access Public members.

    // @cmember Constructor.
  OMBuiltinCachePageAllocator(OMUInt32 pageSize, OMUInt32 pageCount);

    // @cmember Destructor.
  virtual ~OMBuiltinCachePageAllocator(void);

    // @cmember Allocate a page.
  virtual OMByte* allocate(void);

    // @cmember Deallocate a page.
  virtual void deallocate(OMByte* page);

};

#endif
