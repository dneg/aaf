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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMPageCache
#ifndef OMPAGECACHE_H
#define OMPAGECACHE_H

#include "OMDataTypes.h"
#include "OMSet.h"
#include "OMSetIterator.h"
#include "OMList.h"
#include "OMListIterator.h"

  // @class Page caches.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMPageCache {
public:
  // @access Public members.

    // @cmember Constructor.
  OMPageCache(OMUInt32 pageSize, OMUInt32 pageCount);

    // @cmember Destructor.
  virtual ~OMPageCache(void);

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> into the buffer at address <p bytes>
    //          from this <c OMPageCache>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void readCachedAt(OMUInt64 position,
                            OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> from the buffer at address <p bytes>
    //          to this <c OMPageCache>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
  virtual void writeCachedAt(OMUInt64 position,
                             const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten);

    // @cmember Flush this <c OMPageCache>.
  virtual void flush(void);

private:
  // @access Private members.

    // @cmember Read a page or partial page using the cache.
  virtual void readCachedPage(OMUInt64 page,
                              OMUInt32 offset,
                              OMUInt32 byteCount,
                              OMByte* destination);

    // @cmember Write a page or partial page using the cache.
  virtual void writeCachedPage(OMUInt64 page,
                               OMUInt32 offset,
                               OMUInt32 byteCount,
                               const OMByte* source);

    // @cmember Read a page or partial page without using the cache.
  virtual void readPage(OMUInt64 position,
                        OMUInt32 byteCount,
                        OMByte* destination) = 0;

    // @cmember Write a page or partial page without using the cache.
  virtual void writePage(OMUInt64 position,
                         OMUInt32 byteCount,
                         const OMByte* source) = 0;

  struct CacheEntry;

    // O(lg N) map from page number to CacheEntry
  typedef OMSet<OMUInt64, CacheEntry*> Cache;
  typedef OMSetIterator<OMUInt64, CacheEntry*> CacheIterator;

  typedef OMList<CacheEntry*> CacheList;
  typedef OMListIterator<CacheEntry*> CacheListIterator;

  struct CacheEntry {
    OMByte* _page;
    OMUInt64 _pageNumber;
    CacheListIterator _position;
    bool _isDirty;
  };

    // @cmember The <t CacheEntry> for page number <p page>. The entry
    //          returned is already in use for <p page>, or newly created,
    //          or previously occupied by the least recently used page.
  virtual CacheEntry* cacheEntry(OMUInt64 page);

    // @cmember The <t CacheEntry> for page number <p page>, if
    //          present, 0 otherwise.
  virtual CacheEntry* findEntry(OMUInt64 page);

    // @cmember Allocate a <t CacheEntry> for page number <p page> either
    //          by creating a new entry or by replacing an existing entry.
  virtual CacheEntry* allocateEntry(OMUInt64 page);

    // @cmember Create a new <t CacheEntry> for page number <p page>.
  virtual CacheEntry* newEntry(OMUInt64 page);

    // @cmember Allocate a <t CacheEntry> for page number <p page>
    //          by replacing an existing entry.
  virtual CacheEntry* replaceEntry(OMUInt64 page);

  OMUInt32 _pageSize;
  OMUInt32 _pageCount;
  OMUInt32 _validPageCount;
  CacheEntry* _mruEntry;
  Cache _cache;
  CacheList _mruList;

};

#endif
