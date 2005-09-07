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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMCachedRawStorage
#ifndef OMCACHEDRAWSTORAGE_H
#define OMCACHEDRAWSTORAGE_H

#include "OMFile.h"
#include "OMRawStorage.h"
#include "OMPageCache.h"

class OMCachePageAllocator;

  // @class Cached <c OMRawStorage>s.
  //
  //   @base public | <c OMDiskRawStorage>
  //   @base public | <c OMPageCache>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMCachedRawStorage : public OMRawStorage,
                           public OMPageCache {
public:

  enum {defaultPageSize  = 4096};
  enum {defaultPageCount =   64};

  // @access Public members.

    // @cmember Constructor.
  OMCachedRawStorage(OMUInt32 pageSize, OMUInt32 pageCount, OMUInt64 size);

    // @cmember Constructor.
  OMCachedRawStorage(OMUInt32 pageSize,
                     OMUInt32 pageCount,
                     OMUInt64 size,
                     OMCachePageAllocator* allocator);

    // @cmember Destructor.
  virtual ~OMCachedRawStorage(void);

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMCachedRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMCachedRawStorage::size> causes <p bytesRead>
    //          to be less than <p byteCount>. Reading bytes that have never
    //          been written returns undefined data in <p bytes>.
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMCachedRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMCachedRawStorage::size> causes <p bytesRead>
    //          to be less than <p byteCount>. Reading bytes that have never
    //          been written returns undefined data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMCachedRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMCachedRawStorage::size> causes this
    //          <c OMCachedRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMCachedRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMCachedRawStorage::size> causes this
    //          <c OMCachedRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
    //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember The current extent of this <c OMCachedRawStorage> in bytes.
    //          The <f extent()> is the allocated size, while the <f size()>
    //          is the valid size.
    //          precondition - isPositionable()
  virtual OMUInt64 extent(void) const;

    // @cmember Set the size of this <c OMCachedRawStorage> to <p newSize>
    //          bytes. If <p newSize> is greater than
    //          <mf OMCachedRawStorage::size> then this
    //          <c OMCachedRawStorage> is extended. If <p newSize> is less
    //          than <mf OMCachedRawStorage::size> then this
    //          <c OMCachedRawStorage> is truncated. Truncation may also
    //          result in the current position for <f read()> and <f write()>
    //          being set to <mf OMCachedRawStorage::size>.
    //          precondition - isExtendible()
  virtual void extend(OMUInt64 newSize);

    // @cmember The current size of this <c OMCachedRawStorage> in bytes.
    //          The <f size()> is the valid size, while the <f extent()>
    //          is the allocated size.
    //          precondition - isPositionable()
  virtual OMUInt64 size(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMCachedRawStorage>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMCachedRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition) const;

private:
  // @access Private members.

  OMUInt64 _size;       // Size as known to clients
  OMUInt64 _extent;
  OMUInt64 _position;   // Position as known to clients

};

#endif
