/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMINTERNAL

#include "OMMappedFileRawStorage.h"

  // @mfunc Create an <c OMMappedFileRawStorage> object by opening
  //        an existing file for read-only access, the file is named
  //        <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMMappedFileRawStorage> object.
OMMappedFileRawStorage*
OMMappedFileRawStorage::openExistingRead(const wchar_t* fileName)
{
  TRACE("OMMappedFileRawStorage::openExistingRead");

  // TBS
  return 0;
}

  // @mfunc Create an <c OMMappedFileRawStorage> object by opening
  //        an existing file for modify access, the file is named
  //        <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMMappedFileRawStorage> object.
OMMappedFileRawStorage*
OMMappedFileRawStorage::openExistingModify(const wchar_t* fileName)
{
  TRACE("OMMappedFileRawStorage::openExistingModify");

  // TBS
  return 0;
}

  // @mfunc Create an <c OMMappedFileRawStorage> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must not already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMMappedFileRawStorage> object.
OMMappedFileRawStorage*
OMMappedFileRawStorage::openNewModify(const wchar_t* fileName)
{
  TRACE("OMMappedFileRawStorage::openNewModify");

  // TBS
  return 0;
}

  // @mfunc Constructor.
  //   @parm The file.
  //   @parm The access mode.
OMMappedFileRawStorage::OMMappedFileRawStorage(/* TBS */
                                               OMFile::OMAccessMode accessMode)
{
  TRACE("OMMappedFileRawStorage::OMMappedFileRawStorage");

  // TBS
}

  // @mfunc Destructor.
OMMappedFileRawStorage::~OMMappedFileRawStorage(void)
{
  TRACE("OMMappedFileRawStorage::~OMMappedFileRawStorage");

  // TBS
}


  // @mfunc Is it possible to read from this <c OMMappedFileRawStorage> ?
  //  @rdesc True if this <c OMMappedFileRawStorage> is readable,
  //         false otherwise.
  //  @this const
bool OMMappedFileRawStorage::isReadable(void) const
{
  TRACE("OMMappedFileRawStorage::isReadable");

  // TBS
  return false;
}

  // @mfunc Is it possible to write to this <c OMMappedFileRawStorage> ?
  //  @rdesc True if this <c OMMappedFileRawStorage> is writable,
  //         false otherwise.
  //  @this const
bool OMMappedFileRawStorage::isWritable(void) const
{
  TRACE("OMMappedFileRawStorage::isWritable");

  // TBS
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
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMMappedFileRawStorage::read(OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead) const
{
  TRACE("OMMappedFileRawStorage::read");

  // TBS
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
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMMappedFileRawStorage::write(const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten)
{
  TRACE("OMMappedFileRawStorage::write");

  // TBS
}

  // @mfunc May this <c OMMappedFileRawStorage> be changed in size ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMappedFileRawStorage::isSizeable(void) const
{
  TRACE("OMMappedFileRawStorage::isSizeable");

  // TBS
  return false;
}

  // @mfunc The current size of this <c OMMappedFileRawStorage> in bytes.
  //        precondition - isSizeable()
  //   @rdesc The current size of this <c OMMappedFileRawStorage> in bytes.
  //   @this const
OMUInt64 OMMappedFileRawStorage::size(void) const
{
  TRACE("OMMappedFileRawStorage::size");

  // TBS
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
  //        precondition - isSizeable()
  //   @parm The new size of this <c OMMappedFileRawStorage> in bytes.
  //   @devnote There is no ISO/ANSI way of truncating a file in place.
void OMMappedFileRawStorage::setSize(OMUInt64 newSize)
{
  TRACE("OMMappedFileRawStorage::setSize");

  // TBS
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMMappedFileRawStorage> be changed ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMappedFileRawStorage::isPositionable(void) const
{
  TRACE("OMMappedFileRawStorage::isPositionable");

  // TBS
  return false;
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMappedFileRawStorage>.
  //        precondition - isPositionable()
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMappedFileRawStorage::position(void) const
{
  TRACE("OMMappedFileRawStorage::position");

  // TBS
  return 0;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMappedFileRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMappedFileRawStorage::setPosition(OMUInt64 newPosition)
{
  TRACE("OMMappedFileRawStorage::setPosition");

  // TBS
}

  // @mfunc Synchronize this <c OMMappedFileRawStorage> with its external
  //        representation.
void OMMappedFileRawStorage::synchronize(void)
{
  TRACE("OMMappedFileRawStorage::synchronize");

  // TBS
}
