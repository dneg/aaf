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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMCachedDiskRawStorage 
#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMExceptions.h"
#include "OMDiskRawStorage.h"

#include "OMCachedDiskRawStorage.h"

  // @mfunc Create an <c OMCachedDiskRawStorage> object by opening an existing
  //        file for read-only access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openExistingRead(const wchar_t* fileName,
                                         OMUInt32 pageSize,
                                         OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openExistingRead(fileName);

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                          file,
                                                          OMFile::readOnlyMode,
                                                          pageSize,
                                                          pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMCachedDiskRawStorage> object by opening an existing
  //        file for modify access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openExistingModify(const wchar_t* fileName,
                                           OMUInt32 pageSize,
                                           OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openExistingModify(fileName);

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                            file,
                                                            OMFile::modifyMode,
                                                            pageSize,
                                                            pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMCachedDiskRawStorage> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must not already exist.
  //   @parm The file name.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openNewModify(const wchar_t* fileName,
                                      OMUInt32 pageSize,
                                      OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openNewModify(fileName);

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                            file,
                                                            OMFile::modifyMode,
                                                            pageSize,
                                                            pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Destructor.
OMCachedDiskRawStorage::~OMCachedDiskRawStorage(void)
{
  TRACE("OMCachedDiskRawStorage::~OMCachedDiskRawStorage");

  flush();
  synchronize();
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMCachedDiskRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMCachedDiskRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMCachedDiskRawStorage::read(OMByte* bytes,
                                  OMUInt32 byteCount,
                                  OMUInt32& bytesRead) const
{
  TRACE("OMCachedDiskRawStorage::read");

  readAt(_position, bytes, byteCount, bytesRead);
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMCachedDiskRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMCachedDiskRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMCachedDiskRawStorage::readAt(OMUInt64 position,
                                    OMByte* bytes,
                                    OMUInt32 byteCount,
                                    OMUInt32& bytesRead) const
{
  TRACE("OMCachedDiskRawStorage::readAt");
  PRECONDITION("Readable", isReadable());
  PRECONDITION("Positionable", isPositionable());

  const_cast<OMCachedDiskRawStorage*>(this)->
  readCachedAt(position, bytes, byteCount, bytesRead);
  const_cast<OMCachedDiskRawStorage*>(this)->
  _position = position + bytesRead;
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMCachedDiskRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMCachedDiskRawStorage::size> causes this
  //        <c OMCachedDiskRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMCachedDiskRawStorage::write(const OMByte* bytes,
                                   OMUInt32 byteCount,
                                   OMUInt32& bytesWritten)
{
  TRACE("OMCachedDiskRawStorage::write");

  writeAt(_position, bytes, byteCount, bytesWritten);
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMCachedDiskRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMCachedDiskRawStorage::size> causes this
  //        <c OMCachedDiskRawStorage> to be extended, however such
  //        extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMCachedDiskRawStorage::writeAt(OMUInt64 position,
                                     const OMByte* bytes,
                                     OMUInt32 byteCount,
                                     OMUInt32& bytesWritten)
{
  TRACE("OMCachedDiskRawStorage::writeAt");

  PRECONDITION("Writable", isWritable());
  PRECONDITION("Positionable", isPositionable());

  writeCachedAt(position, bytes, byteCount, bytesWritten);
  _position = position + bytesWritten;
  if (_position > _size) {
    _size = _position;
  }
}

  // @mfunc The current extent of this <c OMCachedDiskRawStorage> in bytes.
  //        The <f extent()> is the allocated size, while the <f size()>
  //        is the valid size.
  //        precondition - isPositionable()
OMUInt64 OMCachedDiskRawStorage::extent(void) const
{
  TRACE("OMCachedDiskRawStorage::extent");

  // Size and Extent are equivalent
  return size();
}

  // @mfunc Set the size of this <c OMCachedDiskRawStorage> to <p newSize>
  //        bytes. If <p newSize> is greater than
  //        <mf OMCachedDiskRawStorage::size> then this
  //        <c OMCachedDiskRawStorage> is extended. If <p newSize> is less
  //        than <mf OMCachedDiskRawStorage::size> then this
  //        <c OMCachedDiskRawStorage> is truncated. Truncation may also
  //        result in the current position for <f read()> and <f write()>
  //        being set to <mf OMCachedDiskRawStorage::size>.
  //        precondition - isExtendible()
void OMCachedDiskRawStorage::extend(OMUInt64 newSize)
{
  TRACE("OMCachedDiskRawStorage::extend");

  _size = newSize;
}

  // @mfunc The current size of this <c OMCachedDiskRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMCachedDiskRawStorage> in bytes.
  //   @this const
OMUInt64 OMCachedDiskRawStorage::size(void) const
{
  TRACE("OMCachedDiskRawStorage::size");

  PRECONDITION("Positionable", isPositionable());
  return _size;
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMCachedDiskRawStorage>.
  //        precondition - isPositionable()
OMUInt64 OMCachedDiskRawStorage::position(void) const
{
  TRACE("OMCachedDiskRawStorage::position");

  return _position;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMCachedDiskRawStorage>.
  //        precondition - isPositionable()
void OMCachedDiskRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMCachedDiskRawStorage::setPosition");

  const_cast<OMCachedDiskRawStorage*>(this)->
  _position = newPosition;
}

  // @mfunc Synchronize this <c OMCachedDiskRawStorage> with its external
  //        representation.
void OMCachedDiskRawStorage::synchronize(void)
{
  TRACE("OMCachedDiskRawStorage::synchronize");
}

  // @mfunc Read a page or partial page without using the cache.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
void OMCachedDiskRawStorage::readPage(OMUInt64 position,
                                      OMUInt32 byteCount,
                                      OMByte* destination)
{
  TRACE("OMCachedDiskRawStorage::readPage");
  PRECONDITION("Valid destination", destination != 0);

  OMUInt64 streamSize = _actualSize;
  if (position < streamSize) {
    OMUInt32 remaining = static_cast<OMUInt32>(streamSize - position);
    OMUInt32 readSize;
    if (remaining < byteCount) {
      readSize = remaining;
    } else {
      readSize = byteCount;
    }
    ASSERT("Valid read size", readSize != 0);
    OMUInt32 br;
    OMDiskRawStorage::setPosition(position);
    OMDiskRawStorage::read(destination, readSize, br);
    ASSERT("All bytes read", br == readSize);
  }
}

  // @mfunc Write a page or partial page without using the cache.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
void OMCachedDiskRawStorage::writePage(OMUInt64 position,
                                       OMUInt32 byteCount,
                                       const OMByte* source)
{
  TRACE("OMCachedDiskRawStorage::writePage");
  PRECONDITION("Valid source", source != 0);
  PRECONDITION("Stream is writable", isWritable());

  OMUInt64 streamSize = _size;
  OMUInt32 remaining = static_cast<OMUInt32>(streamSize - position);
  OMUInt32 writeSize;
  if (remaining < byteCount) {
    writeSize = remaining;
  } else {
    writeSize = byteCount;
  }
  ASSERT("Valid write size", writeSize != 0);
  OMUInt32 bw;
  OMDiskRawStorage::setPosition(position);
  OMDiskRawStorage::write(source, writeSize, bw);
  ASSERT("All bytes written", bw == writeSize);
  OMUInt64 lastPosition = position + writeSize;
  if (lastPosition > _actualSize) {
    _actualSize = lastPosition;
  }
}

  // @mfunc Constructor.
OMCachedDiskRawStorage::OMCachedDiskRawStorage(OMStream* file,
                                               OMFile::OMAccessMode accessMode,
                                               OMUInt32 pageSize,
                                               OMUInt32 pageCount)
: OMDiskRawStorage(file, accessMode),
  OMPageCache(pageSize, pageCount),
  _size(0),
  _actualSize(0),
  _position(0)
{
  TRACE("OMCachedDiskRawStorage::OMCachedDiskRawStorage");
  _actualSize = OMDiskRawStorage::size();
  _size = _actualSize;
}
