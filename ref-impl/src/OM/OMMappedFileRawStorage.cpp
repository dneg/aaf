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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMappedFileRawStorage
#include "OMMappedFileRawStorage.h"

  // @mfunc Create an <c OMMappedFileRawStorage> object by opening
  //        an existing file for read-only access, the file is named
  //        <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMMappedFileRawStorage> object.
OMMappedFileRawStorage*
OMMappedFileRawStorage::openExistingRead(const wchar_t* /* fileName */)
{
  TRACE("OMMappedFileRawStorage::openExistingRead");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create an <c OMMappedFileRawStorage> object by opening
  //        an existing file for modify access, the file is named
  //        <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMMappedFileRawStorage> object.
OMMappedFileRawStorage*
OMMappedFileRawStorage::openExistingModify(const wchar_t* /* fileName */)
{
  TRACE("OMMappedFileRawStorage::openExistingModify");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create an <c OMMappedFileRawStorage> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must not already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMMappedFileRawStorage> object.
OMMappedFileRawStorage*
OMMappedFileRawStorage::openNewModify(const wchar_t* /* fileName */)
{
  TRACE("OMMappedFileRawStorage::openNewModify");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Constructor.
  //   @parm The access mode.
OMMappedFileRawStorage::OMMappedFileRawStorage(
                                         /* TBS */
                                         OMFile::OMAccessMode /* accessMode */)
{
  TRACE("OMMappedFileRawStorage::OMMappedFileRawStorage");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Destructor.
OMMappedFileRawStorage::~OMMappedFileRawStorage(void)
{
  TRACE("OMMappedFileRawStorage::~OMMappedFileRawStorage");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}


  // @mfunc Is it possible to read from this <c OMMappedFileRawStorage> ?
  //  @rdesc True if this <c OMMappedFileRawStorage> is readable,
  //         false otherwise.
  //  @this const
bool OMMappedFileRawStorage::isReadable(void) const
{
  TRACE("OMMappedFileRawStorage::isReadable");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return false;
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMMappedFileRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMappedFileRawStorage::size> causes <p bytesRead>
  //        to be less than <p byteCount>.
  //        Reading bytes that have never been written returns undefined
  //        data in <p bytes>.
  //   @parm OMByte* | bytes | The buffer into which the bytes are to be read.
  //   @parm OMUInt32 | byteCount | The number of bytes to read.
  //   @parm OMUInt32& | bytesRead | The number of bytes actually read.
  //   @this const
void OMMappedFileRawStorage::read(OMByte* /* bytes */,
                                  OMUInt32 /* byteCount */,
                                  OMUInt32& /* bytesRead */) const
{
  TRACE("OMMappedFileRawStorage::read");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMMappedFileRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMappedFileRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm OMUInt64 | position | The position from which the bytes are
  //         to be read.
  //   @parm OMByte* | bytes | The buffer into which the bytes are to be read.
  //   @parm OMUInt32 | byteCount | The number of bytes to read.
  //   @parm OMUInt32& | bytesRead | The number of bytes actually read.
  //   @this const
void OMMappedFileRawStorage::readAt(OMUInt64 /* position */,
                                    OMByte* /* bytes */,
                                    OMUInt32 /* byteCount */,
                                    OMUInt32& /* bytesRead */) const
{
  TRACE("OMMappedFileRawStorage::readAt");
  PRECONDITION("Readable", isReadable());
  PRECONDITION("Readable", isPositionable());

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Is it possible to write to this <c OMMappedFileRawStorage> ?
  //  @rdesc True if this <c OMMappedFileRawStorage> is writable,
  //         false otherwise.
  //  @this const
bool OMMappedFileRawStorage::isWritable(void) const
{
  TRACE("OMMappedFileRawStorage::isWritable");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return false;
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMMappedFileRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMappedFileRawStorage::size> causes this
  //        <c OMMappedFileRawStorage> to be extended, however such
  //        extension can fail, causing <p bytesWritten> to be less than
  //        <p byteCount>.
  //   @parm const OMByte* | bytes | The buffer from which the bytes are
  //         to be written.
  //   @parm OMUInt32 | byteCount | The number of bytes to write.
  //   @parm OMUInt32& | bytesWritten | The actual number of bytes written.
void OMMappedFileRawStorage::write(const OMByte* /* bytes */,
                                   OMUInt32 /* byteCount */,
                                   OMUInt32& /* bytesWritten */)
{
  TRACE("OMMappedFileRawStorage::write");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMMappedFileRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMappedFileRawStorage::size> causes this
  //        <c OMMappedFileRawStorage> to be extended, however such
  //        extension can fail, causing <p bytesWritten> to be less
  //        than <p byteCount>.
  //   @parm OMUInt64 | position | The position to which the bytes are
  //         to be written.
  //   @parm OMByte* | bytes | The buffer from which the bytes are to be
  //         written.
  //   @parm OMUInt32 | byteCount | The number of bytes to write.
  //   @parm OMUInt32& | bytesWritten | The actual number of bytes written.
void OMMappedFileRawStorage::writeAt(OMUInt64 /* position */,
                                     const OMByte* /* bytes */,
                                     OMUInt32 /* byteCount */,
                                     OMUInt32& /* bytesWritten */)
{
  TRACE("OMMappedFileRawStorage::writeAt");

  PRECONDITION("Writable", isWritable());
  PRECONDITION("Readable", isPositionable());

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc May this <c OMMappedFileRawStorage> be changed in size ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMappedFileRawStorage::isExtendible(void) const
{
  TRACE("OMMappedFileRawStorage::isExtendible");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return false;
}

  // @mfunc The current extent of this <c OMMappedFileRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current extent of this <c OMMappedFileRawStorage> in bytes.
  //   @this const
OMUInt64 OMMappedFileRawStorage::extent(void) const
{
  TRACE("OMMappedFileRawStorage::extent");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Set the size of this <c OMMappedFileRawStorage> to <p newSize>
  //        bytes. If <p newSize> is greater than
  //        <mf OMMappedFileRawStorage::size>
  //        then this <c OMMappedFileRawStorage> is extended.
  //        If <p newSize> is less than <mf OMMappedFileRawStorage::size>
  //        then this <c OMMappedFileRawStorage> is truncated.
  //        Truncation may also result in the current position for
  //        <f read()> and <f write()> being set to
  //        <mf OMMappedFileRawStorage::size>.
  //        precondition - isExtendible()
  //   @parm The new size of this <c OMMappedFileRawStorage> in bytes.
void OMMappedFileRawStorage::extend(OMUInt64 /* newSize */)
{
  TRACE("OMMappedFileRawStorage::extend");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc The current size of this <c OMMappedFileRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMMappedFileRawStorage> in bytes.
  //   @this const
OMUInt64 OMMappedFileRawStorage::size(void) const
{
  TRACE("OMMappedFileRawStorage::size");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMMappedFileRawStorage> be changed ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMappedFileRawStorage::isPositionable(void) const
{
  TRACE("OMMappedFileRawStorage::isPositionable");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return false;
}

  // @mfunc Synchronize this <c OMMappedFileRawStorage> with its external
  //        representation.
void OMMappedFileRawStorage::synchronize(void)
{
  TRACE("OMMappedFileRawStorage::synchronize");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMappedFileRawStorage>.
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMappedFileRawStorage::position(void) const
{
  TRACE("OMMappedFileRawStorage::position");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMappedFileRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @this const
void OMMappedFileRawStorage::setPosition(OMUInt64 /* newPosition */) const
{
  TRACE("OMMappedFileRawStorage::setPosition");

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}
