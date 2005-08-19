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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMXFRawStorage

#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"

#include "OMMXFRawStorage.h"

  // @mfunc Constructor.
OMMXFRawStorage::OMMXFRawStorage(OMRawStorage* store)
: _store(store)
{
  TRACE("OMMXFRawStorage::OMMXFRawStorage");
  PRECONDITION("Valid store", _store != 0);
}

  // @mfunc Destructor.
OMMXFRawStorage::~OMMXFRawStorage(void)
{
  TRACE("OMMXFRawStorage::~OMMXFRawStorage");

  PRECONDITION("Valid store", _store != 0);

  delete _store;
  _store = 0;
}

  // @mfunc Is it possible to read from this <c OMMXFRawStorage> ?
  //  @rdesc True if this <c OMMXFRawStorage> is readable, false otherwise.
  //  @this const
bool OMMXFRawStorage::isReadable(void) const
{
  TRACE("OMMXFRawStorage::isReadable");

  return _store->isReadable();
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMMXFRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMXFRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMMXFRawStorage::read(OMByte* bytes,
                           OMUInt32 byteCount,
                           OMUInt32& bytesRead) const
{
  TRACE("OMMXFRawStorage::read");

  _store->read(bytes, byteCount, bytesRead);
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMMXFRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMXFRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMMXFRawStorage::readAt(OMUInt64 position,
                             OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesRead) const
{
  TRACE("OMMXFRawStorage::readAt");

  _store->readAt(position, bytes, byteCount, bytesRead);
}

  // @mfunc Is it possible to write to this <c OMMXFRawStorage> ?
  //  @rdesc True if this <c OMMXFRawStorage> is writable, false otherwise.
  //  @this const
bool OMMXFRawStorage::isWritable(void) const
{
  TRACE("OMMXFRawStorage::isWritable");

  return _store->isWritable();
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMMXFRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMXFRawStorage::size> causes this <c OMMXFRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMMXFRawStorage::write(const OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesWritten)
{
  TRACE("OMMXFRawStorage::write");

  _store->write(bytes, byteCount, bytesWritten);
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMMXFRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMXFRawStorage::size> causes this <c OMMXFRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMMXFRawStorage::writeAt(OMUInt64 position,
                              const OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesWritten)
{
  TRACE("OMMXFRawStorage::writeAt");

  _store->writeAt(position, bytes, byteCount, bytesWritten);
}

  // @mfunc May this <c OMMXFRawStorage> be changed in size ?
  //  @rdesc True if this <c OMMXFRawStorage> is extendible, false otherwise.
  //   @this const
bool OMMXFRawStorage::isExtendible(void) const
{
  TRACE("OMMXFRawStorage::isExtendible");

  return _store->isExtendible();
}

  // @mfunc The current extent of this <c OMMXFRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current extent of this <c OMMXFRawStorage> in bytes.
  //   @this const
OMUInt64 OMMXFRawStorage::extent(void) const
{
  TRACE("OMMXFRawStorage::extent");

  return _store->extent();
}

  // @mfunc Set the size of this <c OMMXFRawStorage> to <p newSize> bytes.
  //        If <p newSize> is greater than <mf OMMXFRawStorage::size>
  //        then this <c OMMXFRawStorage> is extended. If <p newSize>
  //        is less than <mf OMMXFRawStorage::size> then this
  //        <c OMMXFRawStorage> is truncated. Truncation may also result
  //        in the current position for <f read()> and <f write()>
  //        being set to <mf OMMXFRawStorage::size>.
  //        precondition - isExtendible()
  //   @parm The new size of this <c OMMXFRawStorage> in bytes.
void OMMXFRawStorage::extend(OMUInt64 newSize)
{
  TRACE("OMMXFRawStorage::extend");

  _store->extend(newSize);
}

  // @mfunc The current size of this <c OMMXFRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMMXFRawStorage> in bytes.
  //   @this const
OMUInt64 OMMXFRawStorage::size(void) const
{
  TRACE("OMMXFRawStorage::size");

  return _store->size();
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMMXFRawStorage> be changed ?
  //  @rdesc True if this <c OMMXFRawStorage> is positionable, false otherwise.
  //   @this const
bool OMMXFRawStorage::isPositionable(void) const
{
  TRACE("OMMXFRawStorage::isPositionable");

  return _store->isPositionable();
}

  // @mfunc Synchronize this <c OMMXFRawStorage> with its external
  //        representation.
void OMMXFRawStorage::synchronize(void)
{
  TRACE("OMMXFRawStorage::synchronize");

  _store->synchronize();
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMXFRawStorage>.
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMXFRawStorage::position(void) const
{
  TRACE("OMMXFRawStorage::position");

  return _store->position();
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMXFRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @this const
void OMMXFRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMMXFRawStorage::setPosition");

  _store->setPosition(newPosition);
}
