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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDiskRawStorage

#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"

#include "OMDiskRawStorage.h"

  // @mfunc Create an <c OMDiskRawStorage> object by opening an existing
  //        file for read-only access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openExistingRead(const wchar_t* fileName)
{
  TRACE("OMDiskRawStorage::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openExistingRead(fileName);

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::readOnlyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMDiskRawStorage> object by opening an existing
  //        file for modify access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openExistingModify(const wchar_t* fileName)
{
  TRACE("OMDiskRawStorage::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openExistingModify(fileName);

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::modifyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMDiskRawStorage> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must not already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openNewModify(const wchar_t* fileName)
{
  TRACE("OMDiskRawStorage::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openNewModify(fileName);

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::modifyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMDiskRawStorage> object by creating a new
  //        temporary file for modify access.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openNewModify(void)
{
  TRACE("OMDiskRawStorage::openNewModify");

  OMStream* file = OMStream::openNewModify();

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::modifyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Destructor.
OMDiskRawStorage::~OMDiskRawStorage(void)
{
  TRACE("OMDiskRawStorage::~OMDiskRawStorage");

  PRECONDITION("Valid file", _file != 0);

  synchronize();
  delete _file;
  _file = 0;
}

  // @mfunc Is it possible to read from this <c OMDiskRawStorage> ?
  //  @rdesc True if this <c OMDiskRawStorage> is readable, false otherwise.
  //  @this const
bool OMDiskRawStorage::isReadable(void) const
{
  TRACE("OMDiskRawStorage::isReadable");

  bool result;
  if ((_mode == OMFile::modifyMode) || (_mode == OMFile::readOnlyMode)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMDiskRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMDiskRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMDiskRawStorage::read(OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead) const
{
  TRACE("OMDiskRawStorage::read");

  PRECONDITION("Readable", isReadable());

  _file->read(bytes, byteCount, bytesRead);
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMDiskRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMDiskRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMDiskRawStorage::readAt(OMUInt64 position,
                              OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesRead) const
{
  TRACE("OMDiskRawStorage::readAt");
  PRECONDITION("Readable", isReadable());
  PRECONDITION("Positionable", isPositionable());

  setPosition(position);
  read(bytes, byteCount, bytesRead);
}

  // @mfunc Is it possible to write to this <c OMDiskRawStorage> ?
  //  @rdesc True if this <c OMDiskRawStorage> is writable, false otherwise.
  //  @this const
bool OMDiskRawStorage::isWritable(void) const
{
  TRACE("OMDiskRawStorage::isWritable");

  bool result;
  if ((_mode == OMFile::modifyMode) || (_mode == OMFile::writeOnlyMode)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMDiskRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMDiskRawStorage::size> causes this <c OMDiskRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMDiskRawStorage::write(const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten)
{
  TRACE("OMDiskRawStorage::write");

  PRECONDITION("Writable", isWritable());

  _file->write(bytes, byteCount, bytesWritten);
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMDiskRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMDiskRawStorage::size> causes this <c OMDiskRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMDiskRawStorage::writeAt(OMUInt64 position,
                               const OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesWritten)
{
  TRACE("OMDiskRawStorage::writeAt");

  PRECONDITION("Writable", isWritable());
  PRECONDITION("Positionable", isPositionable());

  setPosition(position);
  write(bytes, byteCount, bytesWritten);
}

  // @mfunc May this <c OMDiskRawStorage> be changed in size ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMDiskRawStorage::isExtendible(void) const
{
  TRACE("OMDiskRawStorage::isExtendible");

  return true;
}

  // @mfunc The current extent of this <c OMDiskRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current extent of this <c OMDiskRawStorage> in bytes.
  //   @this const
OMUInt64 OMDiskRawStorage::extent(void) const
{
  TRACE("OMDiskRawStorage::extent");

  PRECONDITION("Positionable", isPositionable());

  // TBS tjb - For now extent and size are the same
  return size();
}

  // @mfunc Set the size of this <c OMDiskRawStorage> to <p newSize> bytes.
  //        If <p newSize> is greater than <mf OMDiskRawStorage::size>
  //        then this <c OMDiskRawStorage> is extended. If <p newSize>
  //        is less than <mf OMDiskRawStorage::size> then this
  //        <c OMDiskRawStorage> is truncated. Truncation may also result
  //        in the current position for <f read()> and <f write()>
  //        being set to <mf OMDiskRawStorage::size>.
  //        precondition - isExtendible()
  //   @parm The new size of this <c OMDiskRawStorage> in bytes.
  //   @devnote There is no ISO/ANSI way of truncating a file in place.
void OMDiskRawStorage::extend(OMUInt64 newSize)
{
  TRACE("OMDiskRawStorage::extend");

  PRECONDITION("Extendible", isExtendible());
  PRECONDITION("Writable", isWritable());

  _file->setSize(newSize);
}

  // @mfunc The current size of this <c OMDiskRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMDiskRawStorage> in bytes.
  //   @this const
OMUInt64 OMDiskRawStorage::size(void) const
{
  TRACE("OMDiskRawStorage::size");

  PRECONDITION("Positionable", isPositionable());

  OMUInt64 result = _file->size();
  return result;
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMDiskRawStorage> be changed ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMDiskRawStorage::isPositionable(void) const
{
  TRACE("OMDiskRawStorage::isPositionable");

  return true;
}

  // @mfunc Synchronize this <c OMDiskRawStorage> with its external
  //        representation.
void OMDiskRawStorage::synchronize(void)
{
  TRACE("OMDiskRawStorage::synchronize");

  if (_file->isWritable()) {
    _file->synchronize();
  }
}

  // @mfunc Constructor.
  //   @parm The file.
  //   @parm The access mode.
OMDiskRawStorage::OMDiskRawStorage(OMStream* file,
                                   OMFile::OMAccessMode accessMode)
: _file(file),
  _mode(accessMode)
{
  TRACE("OMDiskRawStorage::OMDiskRawStorage");

  PRECONDITION("Valid file", _file != 0);
  PRECONDITION("Valid mode", (_mode == OMFile::readOnlyMode)  ||
                             (_mode == OMFile::writeOnlyMode) ||
                             (_mode == OMFile::modifyMode));
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMDiskRawStorage>.
  //        precondition - isPositionable()
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMDiskRawStorage::position(void) const
{
  TRACE("OMDiskRawStorage::position");

  PRECONDITION("Positionable", isPositionable());

  OMUInt64 result = _file->position();
  return result;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMDiskRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @this const
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMDiskRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMDiskRawStorage::setPosition");

  PRECONDITION("Positionable", isPositionable());

  _file->setPosition(newPosition);
}
