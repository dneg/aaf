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
  //        from this <c OMMemoryRawStorage> at <p offset> into the buffer
  //        at address <p bytes>. The actual number of bytes read is
  //        returned in <p bytesRead>. Reading from offsets greater than
  //        <mf OMRawStorage::size> causes <p bytesRead> to be less than
  //        <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The offset from which to read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMemoryRawStorage::readAt(OMUInt64 offset,
                                OMByte* bytes,
                                OMUInt32 byteCount,
                                OMUInt32& bytesRead) const
{
  TRACE("OMMemoryRawStorage::readAt");

  // TBS
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to this <c OMMemoryRawStorage> at <p offset> from the buffer
  //        at address <p bytes>. The actual number of bytes written is
  //        returned in <p bytesWritten>. Writing to offsets greater than
  //        <mf OMRawStorage::size> causes this <c OMMemoryRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The offset at which to write.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMemoryRawStorage::writeAt(OMUInt64 offset,
                                 const OMByte* bytes,
                                 OMUInt32 byteCount,
                                 OMUInt32& bytesWritten)
{
  TRACE("OMMemoryRawStorage::writeAt");

  // TBS
}

  // @mfunc The current size of this <c OMMemoryRawStorage> in bytes.
  //   @rdesc The current size of this <c OMDiskRawSrorage> in bytes.
  //   @this const
OMUInt64 OMMemoryRawStorage::size(void) const
{
  TRACE("OMMemoryRawStorage::setSize");

  // TBS
  return 0;
}

  // @mfunc Set the size of this <c OMMemoryRawStorage> to <p newSize>
  //        bytes. If <p newSize> is greater than <mf OMRawStorage::size>
  //        then this <c OMMemoryRawStorage> is extended. If <p newSize>
  //        is less than <mf OMRawStorage::size> then this
  //        <c OMMemoryRawStorage> is truncated.
  //   @parm The new size of this <c OMDiskRawSrorage> in bytes.
  //   @devnote There is no ANSI way of truncating a file in place.
void OMMemoryRawStorage::setSize(OMUInt64 newSize)
{
  TRACE("OMMemoryRawStorage::setSize");

  // TBS
}
