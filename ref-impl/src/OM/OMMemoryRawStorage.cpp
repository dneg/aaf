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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMemoryRawStorage
#include "OMRawStorage.h"
#include "OMAssertions.h"

#include "OMMemoryRawStorage.h"

  // @mfunc Create an <c OMMemoryRawStorage> object for modify access.
  //   @rdesc The newly created <c OMMemoryRawStorage> object.
OMMemoryRawStorage*
OMMemoryRawStorage::openNewModify(void)
{
  TRACE("OMMemoryRawStorage::openNewModify");

  OMMemoryRawStorage* result = new OMMemoryRawStorage();
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Destructor.
OMMemoryRawStorage::~OMMemoryRawStorage(void)
{
  TRACE("OMMemoryRawStorage::~OMMemoryRawStorage");

  size_t count = _pageVector.count();
  for (size_t i = 0; i < count; i++) {
    OMByte* page = _pageVector.valueAt(i);
    delete [] page;
  }
}

  // @mfunc Is it possible to read from this <c OMMemoryRawStorage> ?
  //  @rdesc True if this <c OMMemoryRawStorage> is readable, false otherwise.
  //  @this const
bool OMMemoryRawStorage::isReadable(void) const
{
  TRACE("OMMemoryRawStorage::isReadable");

  return true;
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
void OMMemoryRawStorage::read(OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesRead) const
{
  TRACE("OMMemoryRawStorage::read");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Valid byte count", byteCount > 0);

  OMUInt64 remainingBytes = size() - position();
  OMUInt32 count;
  if (byteCount < remainingBytes) {
    // Sufficient bytes to satify read request
    count = byteCount;
  } else {
    // Insufficient bytes to satify read request
    ASSERT("Supported byte count", remainingBytes < ~(size_t)0);
    count = static_cast<size_t>(remainingBytes);
  }

  OMUInt64 firstPage64 = (position() / _pageSize);
  ASSERT("Supported first page", firstPage64 < ~(size_t)0);
  size_t firstPage = static_cast<size_t>(firstPage64);

  OMUInt64 lastPage64 = (position() + count) / _pageSize;
  ASSERT("Supported last page", lastPage64 < ~(size_t)0);
  size_t lastPage = static_cast<size_t>(lastPage64);

  ASSERT("Proper page ordering", lastPage >= firstPage);

  // offset of first partial page
  OMUInt64 firstOffset64 = position() - (firstPage * _pageSize);
  ASSERT("Supported offset", firstOffset64 < ~(size_t)0);
  size_t firstOffset = static_cast<size_t>(firstOffset64);
  // byte count of first partial page
  size_t firstCount = 0;
  // byte count of last partial page
  size_t lastCount = 0;
  if (lastPage > firstPage) {
    // One or more page boundaries
    firstCount = _pageSize - firstOffset;
    OMUInt64 lastCount64 = (position() + count) - (lastPage * _pageSize);
    ASSERT("Supported count", lastCount64 < ~(size_t)0);
    lastCount = static_cast<size_t>(lastCount64);
  } else {
    // No page boundaries
    firstCount = count;
    lastCount = 0;
  }

  OMByte* dest = bytes;
  // Read first, or only, partial page
  if (firstCount > 0) {
    read(firstPage, firstOffset, firstCount, dest);
    dest = dest + firstCount;
  }
  // Read whole pages, if any
  for (size_t i = firstPage + 1; i < lastPage; i++) {
    read(i, 0, _pageSize, dest);
    dest = dest + _pageSize;
  }
  // Read last partial page, if any
  if (lastCount > 0) {
    read(lastPage, 0, lastCount, dest);
    dest = dest + lastCount;
  }
  const_cast<OMMemoryRawStorage*>(this)->_position = _position + count;
  bytesRead = count;
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from offset <p position> in this <c OMMemoryRawStorage>
  //        into the buffer at address <p bytes>.
  //        The actual number of bytes read is returned in <p bytesRead>.
  //        Reading from positions greater than
  //        <mf OMMemoryRawStorage::size> causes <p bytesRead> to be less
  //        than <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The position from which the bytes are to be read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMMemoryRawStorage::readAt(OMUInt64 position,
                                OMByte* bytes,
                                OMUInt32 byteCount,
                                OMUInt32& bytesRead) const
{
  TRACE("OMMemoryRawStorage::readAt");
  PRECONDITION("Readable", isReadable());
  PRECONDITION("Positionable", isPositionable());

  setPosition(position);
  read(bytes, byteCount, bytesRead);
}

  // @mfunc Is it possible to write to this <c OMMemoryRawStorage> ?
  //  @rdesc True if this <c OMMemoryRawStorage> is writable, false otherwise.
  //  @this const
bool OMMemoryRawStorage::isWritable(void) const
{
  TRACE("OMMemoryRawStorage::isWritable");

  return true;
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
void OMMemoryRawStorage::write(const OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesWritten)
{
  TRACE("OMMemoryRawStorage::write");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Valid byte count", byteCount > 0);

  // Make sure there is space
  OMUInt64 lastPosition = _position + byteCount;
  if (lastPosition > _size) {
    extend(lastPosition);
  }

  OMUInt64 firstPage64 = (position() / _pageSize);
  ASSERT("Supported first page", firstPage64 < ~(size_t)0);
  size_t firstPage = static_cast<size_t>(firstPage64);

  OMUInt64 lastPage64 = (position() + byteCount) / _pageSize;
  ASSERT("Supported last page", lastPage64 < ~(size_t)0);
  size_t lastPage = static_cast<size_t>(lastPage64);

  ASSERT("Proper page ordering", lastPage >= firstPage);

  // offset of first partial page
  OMUInt64 firstOffset64 = position() - (firstPage * _pageSize);
  ASSERT("Supported offset", firstOffset64 < ~(size_t)0);
  size_t firstOffset = static_cast<size_t>(firstOffset64);
  // byte count of first partial page
  size_t firstCount = 0;
  // byte count of last partial page
  size_t lastCount = 0;
  if (lastPage > firstPage) {
    // One or more page boundaries
    firstCount = _pageSize - firstOffset;
    OMUInt64 lastCount64 = (position() + byteCount) - (lastPage * _pageSize);
    ASSERT("Supported count", lastCount64 < ~(size_t)0);
    lastCount = static_cast<size_t>(lastCount64);
  } else {
    // No page boundaries
    firstCount = byteCount;
    lastCount = 0;
  }

  const OMByte* source = bytes;
  // Write first, or only, partial page
  if (firstCount > 0) {
    write(firstPage, firstOffset, firstCount, source);
    source = source + firstCount;
  }
  // Write whole pages, if any
  for (size_t i = firstPage + 1; i < lastPage; i++) {
    write(i, 0, _pageSize, source);
    source = source + _pageSize;
  }
  // Write last partial page, if any
  if (lastCount > 0) {
    write(lastPage, 0, lastCount, source);
    source = source + lastCount;
  }
  _position = _position + byteCount;
  bytesWritten = byteCount;
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to offset <p position> in this <c OMMemoryRawStorage>
  //        from the buffer at address <p bytes>.
  //        The actual number of bytes written is returned in
  //        <p bytesWritten>.
  //        Writing to positions greater than
  //        <mf OMMemoryRawStorage::size> causes this <c OMMemoryRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The position to which the bytes are to be written.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMMemoryRawStorage::writeAt(OMUInt64 position,
                                 const OMByte* bytes,
                                 OMUInt32 byteCount,
                                 OMUInt32& bytesWritten)
{
  TRACE("OMMemoryRawStorage::writeAt");

  PRECONDITION("Writable", isWritable());
  PRECONDITION("Positionable", isPositionable());

  setPosition(position);
  write(bytes, byteCount, bytesWritten);
}

  // @mfunc May this <c OMMemoryRawStorage> be changed in size ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMemoryRawStorage::isExtendible(void) const
{
  TRACE("OMMemoryRawStorage::isExtendible");

  return true;
}

  // @mfunc The current extent of this <c OMMemoryRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current extent of this <c OMMemoryRawStorage> in bytes.
  //   @this const
OMUInt64 OMMemoryRawStorage::extent(void) const
{
  TRACE("OMMemoryRawStorage::extent");

  PRECONDITION("Positionable", isPositionable());

  // TBS tjb - For now extent and size are the same
  return size();
}

  // @mfunc Set the size of this <c OMMemoryRawStorage> to <p newSize>
  //        bytes.
  //        If <p newSize> is greater than <mf OMMemoryRawStorage::size>
  //        then this <c OMMemoryRawStorage> is extended. If <p newSize>
  //        is less than <mf OMMemoryRawStorage::size> then this
  //        <c OMMemoryRawStorage> is truncated. Truncation may also result
  //        in the current position for <f read()> and <f write()>
  //        being set to <mf OMMemoryRawStorage::size>.
  //        precondition - isExtendible()
  //   @parm The new size of this <c OMMemoryRawStorage> in bytes.
void OMMemoryRawStorage::extend(OMUInt64 newSize)
{
  TRACE("OMMemoryRawStorage::extend");

  PRECONDITION("Extendible", isExtendible());

  if (newSize != _size) {

    // Calculate new page count
    size_t oldPageCount = _pageVector.count();
    size_t pageCount;
    if (newSize != 0) {
      OMUInt64 pageCount64 = ((newSize - 1) / _pageSize) + 1;
      ASSERT("Supported page count", pageCount64 < ~(size_t)0);
      pageCount = static_cast<size_t>(pageCount64);
    } else {
      pageCount = 0;
    }

    if (pageCount > oldPageCount) {

      // grow vector
      _pageVector.grow(pageCount);

      // allocate new pages
      for (size_t i = oldPageCount; i < pageCount; i++) {
        OMByte* page = new OMByte[_pageSize];
        ASSERT("Valid heap pointer", page != 0);
        _pageVector.insertAt(page, i);
      }
    } else if (pageCount < oldPageCount) {

      // deallocate old pages
      for (size_t i = oldPageCount - 1; i >= pageCount; --i) {
        OMByte* page = _pageVector.valueAt(i);
        delete [] page;
        _pageVector.removeAt(i);
      }

      // shrink vector
      _pageVector.shrink(pageCount);

    } // else vector contains correct number of pages

    _size = newSize;
    POSTCONDITION("Consistent size and page count",
                                   _size <= (_pageVector.count() * _pageSize));
  }
}

  // @mfunc The current size of this <c OMMemoryRawStorage> in bytes.
  //        precondition - isPositionable()
  //   @rdesc The current size of this <c OMMemoryRawStorage> in bytes.
  //   @this const
OMUInt64 OMMemoryRawStorage::size(void) const
{
  TRACE("OMMemoryRawStorage::size");

  PRECONDITION("Positionable", isPositionable());

  OMUInt64 result = _size;
  POSTCONDITION("Consistent size and page count",
                                   _size <= (_pageVector.count() * _pageSize));
  return result;
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMMemoryRawStorage> be changed ?
  //   @rdesc Always <e bool.true>.
  //   @this const
bool OMMemoryRawStorage::isPositionable(void) const
{
  TRACE("OMMemoryRawStorage::isPositionable");

  return true;
}

  // @mfunc Synchronize this <c OMMemoryRawStorage> with its external
  //        representation.
void OMMemoryRawStorage::synchronize(void)
{
  TRACE("OMMemoryRawStorage::synchronize");
  // nothing to do
}

  // @mfunc Constructor.
OMMemoryRawStorage::OMMemoryRawStorage(void)
: _pageVector(),
  _pageSize(4 * 1024),
  _size(0),
  _position(0)
{
  TRACE("OMMemoryRawStorage::OMMemoryRawStorage");
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMemoryRawStorage>.
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMemoryRawStorage::position(void) const
{
  TRACE("OMMemoryRawStorage::position");

  return _position;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMemoryRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @this const
void OMMemoryRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMMemoryRawStorage::setPosition");

  PRECONDITION("Positionable", isPositionable());
  PRECONDITION("Valid position", newPosition <= _size);

  OMMemoryRawStorage* nonConstThis = const_cast<OMMemoryRawStorage*>(this);
  nonConstThis->_position = newPosition;
}

  // @mfunc Write a page or partial page.
  //   @parm The page to be written to.
  //   @parm The starting offset within the page.
  //   @parm the number of bytes to write.
  //   @parm The buffer from which the bytes are to be written.
void OMMemoryRawStorage::write(size_t page,
                               size_t offset,
                               size_t byteCount,
                               const OMByte* source)
{
  TRACE("OMMemoryRawStorage::write");

  PRECONDITION("Valid page", page < _pageVector.count());
  PRECONDITION("Valid offset", offset < _pageSize);
  PRECONDITION("Valid count", ((byteCount > 0) && (byteCount <= _pageSize)));
  PRECONDITION("Valid buffer", source != 0);

  OMByte* pageStart = _pageVector.valueAt(page);

  memcpy(pageStart + offset, source, byteCount);
}

  // @mfunc Read a page or partial page.
  //   @parm The page to be read from.
  //   @parm The starting offset within the page.
  //   @parm the number of bytes to read.
  //   @parm The buffer into which the bytes are to be read.
void OMMemoryRawStorage::read(size_t page,
                              size_t offset,
                              size_t byteCount,
                              OMByte* destination) const
{
  TRACE("OMMemoryRawStorage::read");

  PRECONDITION("Valid page", page < _pageVector.count());
  PRECONDITION("Valid offset", offset < _pageSize);
  PRECONDITION("Valid count", ((byteCount > 0) && (byteCount <= _pageSize)));
  PRECONDITION("Valid buffer", destination != 0);

  OMByte* pageStart = _pageVector.valueAt(page);

  memcpy(destination, pageStart + offset, byteCount);
}
