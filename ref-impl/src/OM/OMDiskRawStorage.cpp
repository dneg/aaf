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
#include "OMUtilities.h"

#include <limits.h>
#include <errno.h>

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

  FILE* file = wfopen(fileName, L"rb");
  ASSERT("File successfully opened", file != 0); // tjb - error

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

  FILE* file = wfopen(fileName, L"r+b");
  ASSERT("File successfully opened", file != 0); // tjb - error

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

  FILE* file = wfopen(fileName, L"w+b");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::modifyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Destructor.
OMDiskRawStorage::~OMDiskRawStorage(void)
{
  TRACE("OMDiskRawStorage::~OMDiskRawStorage");

  PRECONDITION("Valid file", _file != 0);

  fclose(_file);
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

  read(_file, bytes, byteCount, bytesRead);
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

  write(_file, bytes, byteCount, bytesWritten);
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

  setSize(_file, newSize);
}

  // @mfunc The current size of this <c OMDiskRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMDiskRawStorage> in bytes.
  //   @this const
OMUInt64 OMDiskRawStorage::size(void) const
{
  TRACE("OMDiskRawStorage::size");

  PRECONDITION("Positionable", isPositionable());

  OMUInt64 result = size(_file);
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

  int status = fflush(_file);
  ASSERT("Successful flush", status == 0); // tjb - error
}

  // @mfunc Constructor.
  //   @parm The file.
  //   @parm The access mode.
OMDiskRawStorage::OMDiskRawStorage(FILE* file,
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

  OMUInt64 result = position(_file);
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

  setPosition(_file, newPosition);
}

void OMDiskRawStorage::read(FILE* file,
                            OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead) const
{
  TRACE("OMDiskRawStorage::read");

  size_t actualByteCount = fread(bytes, 1, byteCount, file);

  bytesRead = actualByteCount;
}

void OMDiskRawStorage::write(FILE* file,
                             const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten)
{
  TRACE("OMDiskRawStorage::write");

  size_t actualByteCount = fwrite(bytes, 1, byteCount, file);

  bytesWritten = actualByteCount;
}

OMUInt64 OMDiskRawStorage::size(FILE* file) const
{
  TRACE("OMDiskRawStorage::size");

  OMUInt64 oldPosition = position();

  long int seekStatus = fseek(file, 0, SEEK_END);
  ASSERT("Successful seek", seekStatus == 0); // tjb - error

  errno = 0;
  long int position = ftell(file);
  ASSERT("Successful tell", IMPLIES(position == -1L, errno == 0));

  const_cast<OMDiskRawStorage*>(this)->setPosition(oldPosition);

  OMUInt64 result = position;
  return result;
}

void OMDiskRawStorage::setSize(FILE* file, OMUInt64 newSize)
{
  TRACE("OMDiskRawStorage::setSize");

  OMUInt64 currentSize = size(file);

  if (newSize > currentSize) {

    // Extend by writing a single byte.
    //
    OMUInt64 oldPosition = position(file); // Save position
    OMByte nullByte = 0;
    OMUInt32 bytesWritten = 0;

    setPosition(file, newSize - 1);
    write(file, &nullByte, 1, bytesWritten);
    ASSERT("Successful write", bytesWritten == 1);
    ASSERT("Size properly changed", size(file) == newSize);
    setPosition(file, oldPosition); // Restore position
  }
  // else no ISO/ANSI way to truncate the file in place
}

OMUInt64 OMDiskRawStorage::position(FILE* file) const
{
  TRACE("OMDiskRawStorage::position");

  errno = 0;
  long int position = ftell(file);
  ASSERT("Successful tell", IMPLIES(position == -1L, errno == 0));

  OMUInt64 result = position;
  return result;
}

void OMDiskRawStorage::setPosition(FILE* file, OMUInt64 newPosition) const
{
  TRACE("OMDiskRawStorage::setPosition");

  ASSERT("Supported position", newPosition <= LONG_MAX); // tjb - limit
  long int liNewPosition = static_cast<long int>(newPosition);

  int seekStatus = fseek(file, liNewPosition, SEEK_SET);
  ASSERT("Successful seek", seekStatus == 0); // tjb - error
}
