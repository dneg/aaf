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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMPageCache 
#include "OMPageCache.h"

#include "OMDataTypes.h"
#include "OMAssertions.h"
#include "OMSetIterator.h"

#include <string.h>

  // @mfunc Constructor.
  //   @parm The size of each page in bytes.
  //   @parm The number of pages.
OMPageCache::OMPageCache(OMUInt32 pageSize,
                         OMUInt32 pageCount)
: _pageSize(pageSize),
  _pageCount(pageCount),
  _validPageCount(0),
  _mruEntry(0)
{
  TRACE("OMPageCache::OMPageCache");

  PRECONDITION("Valid page size", _pageSize > 0);
  PRECONDITION("Valid page count", _pageCount > 0);
}

  // @mfunc Destructor.
OMPageCache::~OMPageCache(void)
{
  TRACE("OMPageCache::~OMPageCache");

  CacheIterator iterator(_cache, OMBefore);
  while (++iterator) {
    CacheEntry* entry = iterator.value();
    ASSERT("Page is clean", !entry->_isDirty);
    delete [] entry->_page;
    entry->_page = 0;
    delete entry;
  }
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> into the buffer at address <p bytes>
  //        from this <c OMPageCache>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //   @parm The position at which to start reading.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
void OMPageCache::readCachedAt(OMUInt64 position,
                               OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesRead)
{
  TRACE("OMPageCache::readCachedAt");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Valid byte count", byteCount > 0);

  OMByte* destination = bytes;
  OMUInt32 remaining = byteCount;
  OMUInt64 page = position / _pageSize;
  OMUInt32 offset = static_cast<OMUInt32>(position - (page * _pageSize));
  OMUInt32 free = _pageSize - offset;
  OMUInt32 readSize;
  if (remaining < free) {
    readSize = remaining;
  } else {
    readSize = free;
  }
  while (remaining > 0) {
    readCachedPage(page, offset, readSize, destination);
    remaining = remaining - readSize;
    destination = destination + readSize;
    page = page + 1;
    offset = 0;
    if (remaining < _pageSize) {
      readSize = remaining;
    } else {
      readSize = _pageSize;
    }
  }
  bytesRead = byteCount;
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> from the buffer at address <p bytes>
  //        to this <c OMPageCache>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //   @parm The position at which to start writing.
  //   @parm The buffer from which the bytes are written.
  //   @parm The number of bytes to write.
  //   @parm The number of bytes actually written.
void OMPageCache::writeCachedAt(OMUInt64 position,
                                const OMByte* bytes,
                                OMUInt32 byteCount,
                                OMUInt32& bytesWritten)
{
  TRACE("OMPageCache::writeCachedAt");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Valid byte count", byteCount > 0);

  const OMByte* source = bytes;
  OMUInt32 remaining = byteCount;
  OMUInt64 page = position / _pageSize;
  OMUInt32 offset = static_cast<OMUInt32>(position - (page * _pageSize));
  OMUInt32 free = _pageSize - offset;
  OMUInt32 writeSize;
  if (remaining < free) {
    writeSize = remaining;
  } else {
    writeSize = free;
  }
  while (remaining > 0) {
    writeCachedPage(page, offset, writeSize, source);
    remaining = remaining - writeSize;
    source = source + writeSize;
    page = page + 1;
    offset = 0;
    if (remaining < _pageSize) {
      writeSize = remaining;
    } else {
      writeSize = _pageSize;
    }
  }
  bytesWritten = byteCount;
}

  // @mfunc Flush this <c OMPageCache>.
void OMPageCache::flush(void)
{
  TRACE("OMPageCache::flush");

  CacheIterator iterator(_cache, OMBefore);
  while (++iterator) {
    CacheEntry* entry = iterator.value();
    OMUInt64 pageNumber = iterator.key();
    ASSERT("Consistent page numbers", pageNumber == entry->_pageNumber);
    if (entry->_isDirty) {
      writePage(pageNumber * _pageSize, _pageSize, entry->_page);
      entry->_isDirty = false;
    }
  }
}

  // @mfunc Read a page or partial page using the cache.
  //   @parm The page to be read from.
  //   @parm The starting offset within the page.
  //   @parm The number of bytes to read.
  //   @parm The buffer into which the bytes are to be read.
void OMPageCache::readCachedPage(OMUInt64 page,
                                 OMUInt32 offset,
                                 OMUInt32 byteCount,
                                 OMByte* destination)
{
  TRACE("OMPageCache::readCachedPage");

  PRECONDITION("Valid offset", offset < _pageSize);
  PRECONDITION("Valid count", ((byteCount > 0) && (byteCount <= _pageSize)));
  PRECONDITION("Valid buffer", destination != 0);

  CacheEntry* entry = cacheEntry(page);
  ASSERT("Page found", entry != 0);
  ASSERT("Consistent page numbers", page == entry->_pageNumber);
  OMByte* p = entry->_page;
  ASSERT("Valid page", p != 0);
  memcpy(destination, p + offset, byteCount);
}

  // @mfunc Write a page or partial page using the cache.
  //   @parm The page to be written to.
  //   @parm The starting offset within the page.
  //   @parm the number of bytes to write.
  //   @parm The buffer from which the bytes are to be written.
void OMPageCache::writeCachedPage(OMUInt64 page,
                                  OMUInt32 offset,
                                  OMUInt32 byteCount,
                                  const OMByte* source)
{
  TRACE("OMPageCache::writeCachedPage");

  PRECONDITION("Valid offset", offset < _pageSize);
  PRECONDITION("Valid count", ((byteCount > 0) && (byteCount <= _pageSize)));
  PRECONDITION("Valid buffer", source != 0);

  CacheEntry* entry = cacheEntry(page);
  ASSERT("Page found", entry != 0);
  ASSERT("Consistent page numbers", page == entry->_pageNumber);
  OMByte* p = entry->_page;
  ASSERT("Valid page", p != 0);
  memcpy(p + offset, source, byteCount);
  entry->_isDirty = true;
}

  // @mfunc The <t CacheEntry> for page number <p page>. The entry
  //        returned is already in use for <p page>, or newly created,
  //        or previously occupied by the least recently used page.
  //   @parm The page number.
  //   @rdesc The <t CacheEntry> for page number <p page>.
OMPageCache::CacheEntry* OMPageCache::cacheEntry(OMUInt64 page)
{
  TRACE("OMPageCache::cacheEntry");

  CacheEntry* entry = 0;
  if ((_mruEntry != 0) && (_mruEntry->_pageNumber == page)) {
    entry = _mruEntry;
  } else {
    entry = findEntry(page);
    if (entry == 0) {
      // Page not in cache
      entry = allocateEntry(page);
    }
    _mruEntry = entry;
  }
  ASSERT("Page found", entry != 0);
  ASSERT("Consistent page numbers", page == entry->_pageNumber);

  return entry;
}

  // @mfunc The <t CacheEntry> for page number <p page>, if
  //        present, 0 otherwise.
  //   @parm The page number.
  //   @rdesc The <t CacheEntry> for page number <p page>, if
  //          present, 0 otherwise.
OMPageCache::CacheEntry* OMPageCache::findEntry(OMUInt64 page)
{
  TRACE("OMPageCache::findEntry");

  CacheEntry* result;
  CacheEntry* entry;
  if (_cache.find(page, entry)) {
    result = entry;
    ASSERT("Consistent page numbers", page == entry->_pageNumber);
    // Promote to most recently used
    _mruList.remove(entry->_position);
    _mruList.prepend(entry);
     entry->_position = _mruList.first();
  } else {
    result = 0;
  }
  return result;
}

  // @mfunc Allocate a <t CacheEntry> for page number <p page> either
  //        by creating a new entry or by replacing an existing entry.
  //   @parm The page number.
  //   @rdesc The allocated <t CacheEntry>.
OMPageCache::CacheEntry* OMPageCache::allocateEntry(OMUInt64 page)
{
  TRACE("OMPageCache::allocateEntry");

  CacheEntry* result;
  if (_validPageCount < _pageCount) {
    result = newEntry(page);
    ASSERT("Consistent page numbers", page == result->_pageNumber);
    _validPageCount++;
  } else {
    result = replaceEntry(page);
  }
  ASSERT("Page found", result != 0);
  OMByte* p = result->_page;
  ASSERT("Valid page", p != 0);
#if defined(OM_CLEAR_PAGES)
  for (OMUInt32 i = 0; i < _pageSize; i++) {
    p[i] = "deadbeef"[i % 8];
  }
#endif
  readPage(page * _pageSize, _pageSize, p);
  return result;
}

  // @mfunc Create a new <t CacheEntry> for page number <p page>.
  //   @parm The page number.
  //   @rdesc The newly created <t CacheEntry>.
OMPageCache::CacheEntry* OMPageCache::newEntry(OMUInt64 page)
{
  TRACE("OMPageCache::newEntry");

  OMByte* p = new OMByte[_pageSize];
  ASSERT("Valid heap pointer", p != 0);

  CacheEntry* entry = new CacheEntry;
  ASSERT("Valid heap pointer", entry != 0);

  // Add new entry to head of most recently used list
  _mruList.prepend(entry);
  entry->_pageNumber = page;
  entry->_page = p;
  entry->_isDirty = false;
  entry->_position = _mruList.first();
  _cache.insert(page, entry);

  return entry;
}

  // @mfunc Allocate a <t CacheEntry> for page number <p page>
  //        by replacing an existing entry.
  //   @parm The page number.
  //   @rdesc The allocated <t CacheEntry>.
OMPageCache::CacheEntry* OMPageCache::replaceEntry(OMUInt64 page)
{
  TRACE("OMPageCache::replaceEntry");

  // Victim is at the tail of the most recently used list
  // Add new entry to head of most recently used list
  CacheListIterator iterator = _mruList.last();
  CacheEntry* victim = iterator.value();
  OMUInt64 victimPage = victim->_pageNumber;
  _mruList.remove(iterator);
  if (victim->_isDirty) {
    writePage(victimPage * _pageSize, _pageSize, victim->_page);
    victim->_isDirty = false;
  }
  _cache.remove(victimPage);
  CacheEntry* newEntry = victim;
  _mruList.prepend(newEntry);

  newEntry->_pageNumber = page;
  newEntry->_position = _mruList.first();
  _cache.insert(page, newEntry);
  return newEntry;
}
