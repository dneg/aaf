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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMWrappedRawStorage

#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"

#include "OMWrappedRawStorage.h"

  // @mfunc Constructor.
OMWrappedRawStorage::OMWrappedRawStorage(OMRawStorage* store)
: _store(store)
{
  TRACE("OMWrappedRawStorage::OMWrappedRawStorage");
  PRECONDITION("Valid store", _store != 0);
}

  // @mfunc Destructor.
OMWrappedRawStorage::~OMWrappedRawStorage(void)
{
  TRACE("OMWrappedRawStorage::~OMWrappedRawStorage");

  PRECONDITION("Valid store", _store != 0);

  _store = 0; // We don't own _store
}

  // @mfunc Is it possible to read from this <c OMWrappedRawStorage> ?
  //  @rdesc True if this <c OMWrappedRawStorage> is readable, false otherwise.
  //  @this const
bool OMWrappedRawStorage::isReadable(void) const
{
  TRACE("OMWrappedRawStorage::isReadable");

  return _store->isReadable();
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMWrappedRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMWrappedRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMWrappedRawStorage::read(OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesRead) const
{
  TRACE("OMWrappedRawStorage::read");

  _store->read(bytes, byteCount, bytesRead);
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMWrappedRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMWrappedRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMWrappedRawStorage::readAt(OMUInt64 position,
                                 OMByte* bytes,
                                 OMUInt32 byteCount,
                                 OMUInt32& bytesRead) const
{
  TRACE("OMWrappedRawStorage::readAt");

  _store->readAt(position, bytes, byteCount, bytesRead);
}

  // @mfunc Is it possible to write to this <c OMWrappedRawStorage> ?
  //  @rdesc True if this <c OMWrappedRawStorage> is writable, false otherwise.
  //  @this const
bool OMWrappedRawStorage::isWritable(void) const
{
  TRACE("OMWrappedRawStorage::isWritable");

  return _store->isWritable();
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMWrappedRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMWrappedRawStorage::size> causes this <c OMWrappedRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMWrappedRawStorage::write(const OMByte* bytes,
                                OMUInt32 byteCount,
                                OMUInt32& bytesWritten)
{
  TRACE("OMWrappedRawStorage::write");

  _store->write(bytes, byteCount, bytesWritten);
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMWrappedRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMWrappedRawStorage::size> causes this <c OMWrappedRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMWrappedRawStorage::writeAt(OMUInt64 position,
                                  const OMByte* bytes,
                                  OMUInt32 byteCount,
                                  OMUInt32& bytesWritten)
{
  TRACE("OMWrappedRawStorage::writeAt");

  _store->writeAt(position, bytes, byteCount, bytesWritten);
}

  // @mfunc May this <c OMWrappedRawStorage> be changed in size ?
  //  @rdesc True if this <c OMWrappedRawStorage> is extendible,
  //         false otherwise.
  //   @this const
bool OMWrappedRawStorage::isExtendible(void) const
{
  TRACE("OMWrappedRawStorage::isExtendible");

  return _store->isExtendible();
}

  // @mfunc The current extent of this <c OMWrappedRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current extent of this <c OMWrappedRawStorage> in bytes.
  //   @this const
OMUInt64 OMWrappedRawStorage::extent(void) const
{
  TRACE("OMWrappedRawStorage::extent");

  return _store->extent();
}

  // @mfunc Set the size of this <c OMWrappedRawStorage> to <p newSize> bytes.
  //        If <p newSize> is greater than <mf OMWrappedRawStorage::size>
  //        then this <c OMWrappedRawStorage> is extended. If <p newSize>
  //        is less than <mf OMWrappedRawStorage::size> then this
  //        <c OMWrappedRawStorage> is truncated. Truncation may also result
  //        in the current position for <f read()> and <f write()>
  //        being set to <mf OMWrappedRawStorage::size>.
  //        precondition - isExtendible()
  //   @parm The new size of this <c OMWrappedRawStorage> in bytes.
void OMWrappedRawStorage::extend(OMUInt64 newSize)
{
  TRACE("OMWrappedRawStorage::extend");

  _store->extend(newSize);
}

  // @mfunc The current size of this <c OMWrappedRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMWrappedRawStorage> in bytes.
  //   @this const
OMUInt64 OMWrappedRawStorage::size(void) const
{
  TRACE("OMWrappedRawStorage::size");

  return _store->size();
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMWrappedRawStorage> be changed ?
  //  @rdesc True if this <c OMWrappedRawStorage> is positionable,
  //         false otherwise.
  //   @this const
bool OMWrappedRawStorage::isPositionable(void) const
{
  TRACE("OMWrappedRawStorage::isPositionable");

  return _store->isPositionable();
}

  // @mfunc Synchronize this <c OMWrappedRawStorage> with its external
  //        representation.
void OMWrappedRawStorage::synchronize(void)
{
  TRACE("OMWrappedRawStorage::synchronize");

  _store->synchronize();
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMWrappedRawStorage>.
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMWrappedRawStorage::position(void) const
{
  TRACE("OMWrappedRawStorage::position");

  return _store->position();
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMWrappedRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @this const
void OMWrappedRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMWrappedRawStorage::setPosition");

  _store->setPosition(newPosition);
}

void OMWrappedRawStorage::streamReadAt(OMUInt64 position,
                                       OMByte* bytes,
                                       OMUInt32 byteCount,
                                       OMUInt32& bytesRead) const
{
  TRACE("OMWrappedRawStorage::");

  _store->streamReadAt(position, bytes, byteCount, bytesRead);
}

void OMWrappedRawStorage::streamReadAt(OMUInt64 position,
                                       OMIOVector buffers,
                                       OMUInt32 bufferCount,
                                       OMUInt32& bytesRead) const
{
  TRACE("OMWrappedRawStorage::streamReadAt");

  _store->streamReadAt(position, buffers, bufferCount, bytesRead);
}

void OMWrappedRawStorage::streamWriteAt(OMUInt64 position,
                                        const OMByte* bytes,
                                        OMUInt32 byteCount,
                                        OMUInt32& bytesWritten)
{
  TRACE("OMWrappedRawStorage::streamWriteAt");

  _store->streamWriteAt(position, bytes, byteCount, bytesWritten);
}

void OMWrappedRawStorage::streamWriteAt(OMUInt64 position,
                                        OMIOVector buffers,
                                        OMUInt32 bufferCount,
                                        OMUInt32& bytesWritten)
{
  TRACE("OMWrappedRawStorage::streamWriteAt");

  _store->streamWriteAt(position, buffers, bufferCount, bytesWritten);
}
