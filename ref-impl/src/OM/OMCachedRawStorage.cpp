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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMCachedRawStorage
#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMExceptions.h"
#include "OMRawStorage.h"

#include "OMCachedRawStorage.h"

  // @mfunc Constructor.
OMCachedRawStorage::OMCachedRawStorage(OMUInt32 pageSize,
                                       OMUInt32 pageCount,
                                       OMUInt64 size)
: OMPageCache(pageSize, pageCount/*, allocator*/),
  _size(size),
  _extent(size),
  _position(0)
{
  TRACE("OMCachedRawStorage::OMCachedRawStorage");
}

  // @mfunc Constructor.
OMCachedRawStorage::OMCachedRawStorage(OMUInt32 pageSize,
                                       OMUInt32 pageCount,
                                       OMUInt64 size,
                                       OMCachePageAllocator* allocator)
: OMPageCache(pageSize, pageCount, allocator),
  _size(size),
  _extent(size),
  _position(0)
{
  TRACE("OMCachedRawStorage::OMCachedRawStorage");
}

  // @mfunc Destructor.
OMCachedRawStorage::~OMCachedRawStorage(void)
{
  TRACE("OMCachedRawStorage::~OMCachedRawStorage");
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMCachedRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMCachedRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMCachedRawStorage::read(OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesRead) const
{
  TRACE("OMCachedRawStorage::read");

  readAt(_position, bytes, byteCount, bytesRead);
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMCachedRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMCachedRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMCachedRawStorage::readAt(OMUInt64 position,
                                OMByte* bytes,
                                OMUInt32 byteCount,
                                OMUInt32& bytesRead) const
{
  TRACE("OMCachedRawStorage::readAt");
  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Valid byte count", byteCount > 0);
  PRECONDITION("Readable", isReadable());
  PRECONDITION("Positionable", isPositionable());

  if (position < _size) {
    OMUInt32 readCount;
    OMUInt64 remaining = _size - position;
    if (byteCount > remaining) {
      readCount = static_cast<OMUInt32>(remaining);
    } else {
      readCount = byteCount;
    }
    const_cast<OMCachedRawStorage*>(this)->
    readCachedAt(position, bytes, readCount, bytesRead);
    ASSERT("All bytes read", bytesRead == readCount);
    const_cast<OMCachedRawStorage*>(this)->
    _position = position + bytesRead;
  } else {
    bytesRead = 0;
  }
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMCachedRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMCachedRawStorage::size> causes this
  //        <c OMCachedRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMCachedRawStorage::write(const OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesWritten)
{
  TRACE("OMCachedRawStorage::write");

  writeAt(_position, bytes, byteCount, bytesWritten);
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMCachedRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMCachedRawStorage::size> causes this
  //        <c OMCachedRawStorage> to be extended, however such
  //        extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMCachedRawStorage::writeAt(OMUInt64 position,
                                 const OMByte* bytes,
                                 OMUInt32 byteCount,
                                 OMUInt32& bytesWritten)
{
  TRACE("OMCachedRawStorage::writeAt");

  PRECONDITION("Writable", isWritable());
  PRECONDITION("Positionable", isPositionable());

  OMUInt64 newExtent = position + byteCount;
  if (newExtent > _extent) {
    _extent = newExtent;
  }
  writeCachedAt(position, bytes, byteCount, bytesWritten);
  _position = position + bytesWritten;
  if (_position > _size) {
    _size = _position;
  }
}

  // @mfunc The current extent of this <c OMCachedRawStorage> in bytes.
  //        The <f extent()> is the allocated size, while the <f size()>
  //        is the valid size.
  //        precondition - isPositionable()
OMUInt64 OMCachedRawStorage::extent(void) const
{
  TRACE("OMCachedRawStorage::extent");

  return _extent;
}

  // @mfunc Set the size of this <c OMCachedRawStorage> to <p newSize>
  //        bytes. If <p newSize> is greater than
  //        <mf OMCachedRawStorage::size> then this
  //        <c OMCachedRawStorage> is extended. If <p newSize> is less
  //        than <mf OMCachedRawStorage::size> then this
  //        <c OMCachedRawStorage> is truncated. Truncation may also
  //        result in the current position for <f read()> and <f write()>
  //        being set to <mf OMCachedRawStorage::size>.
  //        precondition - isExtendible()
void OMCachedRawStorage::extend(OMUInt64 newSize)
{
  TRACE("OMCachedRawStorage::extend");

  _extent = newSize;
}

  // @mfunc The current size of this <c OMCachedRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMCachedRawStorage> in bytes.
  //   @this const
OMUInt64 OMCachedRawStorage::size(void) const
{
  TRACE("OMCachedRawStorage::size");

  PRECONDITION("Positionable", isPositionable());
  return _size;
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMCachedRawStorage>.
OMUInt64 OMCachedRawStorage::position(void) const
{
  TRACE("OMCachedRawStorage::position");

  return _position;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMCachedRawStorage>.
  //        precondition - isPositionable()
void OMCachedRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMCachedRawStorage::setPosition");

  const_cast<OMCachedRawStorage*>(this)->
  _position = newPosition;
}

