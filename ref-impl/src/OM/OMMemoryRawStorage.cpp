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
#include "OMRawStorage.h"
#include "OMAssertions.h"

#include "OMMemoryRawStorage.h"

  // @mfunc Create an <c OMMemoryRawStorage> object for modify access.
  //   @rdesc The newly created <c OMMemoryRawStorage> object.
OMMemoryRawStorage*
OMMemoryRawStorage::openNewModify(void)
{
  TRACE("OMMemoryRawStorage::openNewModify");

  // TBS
  return 0;
}

  // @mfunc Constructor.
OMMemoryRawStorage::OMMemoryRawStorage(void)
{
  TRACE("OMMemoryRawStorage::OMMemoryRawStorage");

  // TBS
}

  // @mfunc Destructor.
OMMemoryRawStorage::~OMMemoryRawStorage(void)
{
  TRACE("OMMemoryRawStorage::~OMMemoryRawStorage");

  // TBS
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from the current position in this <c OMMemoryRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMemoryRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMemoryRawStorage::read(OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesRead) const
{
  TRACE("OMMemoryRawStorage::read");

  // TBS
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to the current position in this <c OMMemoryRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMemoryRawStorage::size> causes this
  //        <c OMMemoryRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMemoryRawStorage::write(const OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesWritten)
{
  TRACE("OMMemoryRawStorage::write");

  // TBS
}

  // @mfunc May this <c OMMemoryRawStorage> be changed in size ?
  //        An implementation of <c OMMemoryRawStorage> for disk files
  //        would most probably return true. An implemetation
  //        for network streams would return false. An implementation
  //        for fixed size contiguous memory files (avoiding copying)
  //        would return false.
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMemoryRawStorage::isSizeable(void) const
{
  TRACE("OMMemoryRawStorage::isSizeable");

  return true;
}

  // @mfunc The current size of this <c OMMemoryRawStorage> in bytes.
  //        precondition - isSizeable()
  //   @rdesc The current size of this <c OMDiskRawSrorage> in bytes.
  //   @this const
OMUInt64 OMMemoryRawStorage::size(void) const
{
  TRACE("OMMemoryRawStorage::size");

  PRECONDITION("Sizeable", isSizeable());

  // TBS
  return 0;
}

  // @mfunc Set the size of this <c OMMemoryRawStorage> to <p newSize>
  //        bytes.
  //        If <p newSize> is greater than <mf OMMemoryRawStorage::size>
  //        then this <c OMMemoryRawStorage> is extended. If <p newSize>
  //        is less than <mf OMMemoryRawStorage::size> then this
  //        <c OMMemoryRawStorage> is truncated. Truncation may also result
  //        in the current position for <f read()> and <f write()>
  //        being set to <mf OMMemoryRawStorage::size>.
  //        precondition - isSizeable()
  //   @parm The new size of this <c OMDiskRawSrorage> in bytes.
  //   @devnote There is no ANSI way of truncating a file in place.
void OMMemoryRawStorage::setSize(OMUInt64 newSize)
{
  TRACE("OMMemoryRawStorage::setSize");

  PRECONDITION("Sizeable", isSizeable());

  // TBS
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMMemoryRawStorage> be changed ?
  //        An implementation of <c OMMemoryRawStorage> for disk files
  //        would most probably return true. An implemetation
  //        for network streams would return false. An implementation
  //        for memory files would return true.
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMemoryRawStorage::isPositionable(void) const
{
  TRACE("OMMemoryRawStorage::isPositionable");

  return true;
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of this
  //        <c OMMemoryRawStorage>.
  //        precondition - isPositionable()
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMemoryRawStorage::position(void) const
{
  TRACE("OMMemoryRawStorage::position");

  PRECONDITION("Positionable", isPositionable());

  // TBS
  return 0;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of this
  //        <c OMMemoryRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMemoryRawStorage::setPosition(OMUInt64 newPosition)
{
  TRACE("OMMemoryRawStorage::setPosition");

  PRECONDITION("Positionable", isPositionable());

  // TBS
}
