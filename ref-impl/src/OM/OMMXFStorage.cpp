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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMXFStorage

#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"

#include "OMMXFStorage.h"

  // @mfunc Constructor.
OMMXFStorage::OMMXFStorage(OMRawStorage* store)
: _store(store)
{
  TRACE("OMMXFStorage::OMMXFStorage");
  PRECONDITION("Valid store", _store != 0);
}

  // @mfunc Destructor.
OMMXFStorage::~OMMXFStorage(void)
{
  TRACE("OMMXFStorage::~OMMXFStorage");

  PRECONDITION("Valid store", _store != 0);

  delete _store;
  _store = 0;
}

  // @mfunc Is it possible to read from this <c OMMXFStorage> ?
  //  @rdesc True if this <c OMMXFStorage> is readable, false otherwise.
  //  @this const
bool OMMXFStorage::isReadable(void) const
{
  TRACE("OMMXFStorage::isReadable");

  return _store->isReadable();
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMMXFStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMXFStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMMXFStorage::read(OMByte* bytes,
                        OMUInt32 byteCount,
                        OMUInt32& bytesRead) const
{
  TRACE("OMMXFStorage::read");

  _store->read(bytes, byteCount, bytesRead);
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMMXFStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMXFStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMMXFStorage::readAt(OMUInt64 position,
                          OMByte* bytes,
                          OMUInt32 byteCount,
                          OMUInt32& bytesRead) const
{
  TRACE("OMMXFStorage::readAt");

  _store->readAt(position, bytes, byteCount, bytesRead);
}

  // @mfunc Is it possible to write to this <c OMMXFStorage> ?
  //  @rdesc True if this <c OMMXFStorage> is writable, false otherwise.
  //  @this const
bool OMMXFStorage::isWritable(void) const
{
  TRACE("OMMXFStorage::isWritable");

  return _store->isWritable();
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMMXFStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMXFStorage::size> causes this <c OMMXFStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMMXFStorage::write(const OMByte* bytes,
                         OMUInt32 byteCount,
                         OMUInt32& bytesWritten)
{
  TRACE("OMMXFStorage::write");

  _store->write(bytes, byteCount, bytesWritten);
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMMXFStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMXFStorage::size> causes this <c OMMXFStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMMXFStorage::writeAt(OMUInt64 position,
                           const OMByte* bytes,
                           OMUInt32 byteCount,
                           OMUInt32& bytesWritten)
{
  TRACE("OMMXFStorage::writeAt");

  _store->writeAt(position, bytes, byteCount, bytesWritten);
}

  // @mfunc May this <c OMMXFStorage> be changed in size ?
  //  @rdesc True if this <c OMMXFStorage> is extendible, false otherwise.
  //   @this const
bool OMMXFStorage::isExtendible(void) const
{
  TRACE("OMMXFStorage::isExtendible");

  return _store->isExtendible();
}

  // @mfunc The current extent of this <c OMMXFStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current extent of this <c OMMXFStorage> in bytes.
  //   @this const
OMUInt64 OMMXFStorage::extent(void) const
{
  TRACE("OMMXFStorage::extent");

  return _store->extent();
}

  // @mfunc Set the size of this <c OMMXFStorage> to <p newSize> bytes.
  //        If <p newSize> is greater than <mf OMMXFStorage::size>
  //        then this <c OMMXFStorage> is extended. If <p newSize>
  //        is less than <mf OMMXFStorage::size> then this
  //        <c OMMXFStorage> is truncated. Truncation may also result
  //        in the current position for <f read()> and <f write()>
  //        being set to <mf OMMXFStorage::size>.
  //        precondition - isExtendible()
  //   @parm The new size of this <c OMMXFStorage> in bytes.
void OMMXFStorage::extend(OMUInt64 newSize)
{
  TRACE("OMMXFStorage::extend");

  _store->extend(newSize);
}

  // @mfunc The current size of this <c OMMXFStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMMXFStorage> in bytes.
  //   @this const
OMUInt64 OMMXFStorage::size(void) const
{
  TRACE("OMMXFStorage::size");

  return _store->size();
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMMXFStorage> be changed ?
  //  @rdesc True if this <c OMMXFStorage> is positionable, false otherwise.
  //   @this const
bool OMMXFStorage::isPositionable(void) const
{
  TRACE("OMMXFStorage::isPositionable");

  return _store->isPositionable();
}

  // @mfunc Synchronize this <c OMMXFStorage> with its external
  //        representation.
void OMMXFStorage::synchronize(void)
{
  TRACE("OMMXFStorage::synchronize");

  _store->synchronize();
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMXFStorage>.
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMXFStorage::position(void) const
{
  TRACE("OMMXFStorage::position");

  return _store->position();
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMXFStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @this const
void OMMXFStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMMXFStorage::setPosition");

  _store->setPosition(newPosition);
}
