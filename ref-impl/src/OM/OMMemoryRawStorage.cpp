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

  OMMemoryRawStorage* result = new OMMemoryRawStorage();
  ASSERT("Valid heap pointer", result != 0);

  return result;
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

  // @mfunc Is it possible to write to this <c OMMemoryRawStorage> ?
  //  @rdesc True if this <c OMMemoryRawStorage> is writable, false otherwise.
  //  @this const
bool OMMemoryRawStorage::isWritable(void) const
{
  TRACE("OMMemoryRawStorage::isWritable");

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
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
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
  // count of whole pages
  size_t pageCount = 0;
  if (lastPage > firstPage) {
    // One or more page boundaries
    firstCount = _pageSize - firstOffset;
    OMUInt64 lastCount64 = (position() + count) - (lastPage * _pageSize);
    ASSERT("Supported count", lastCount64 < ~(size_t)0);
    lastCount = static_cast<size_t>(lastCount64);
    pageCount = lastPage - firstPage - 1;
  } else {
    // No page boundaries
    firstCount = count;
    lastCount = 0;
    pageCount = 0;
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

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Valid byte count", byteCount > 0);

  // TBS - writes that extend

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
  // count of whole pages
  size_t pageCount = 0;
  if (lastPage > firstPage) {
    // One or more page boundaries
    firstCount = _pageSize - firstOffset;
    OMUInt64 lastCount64 = (position() + byteCount) - (lastPage * _pageSize);
    ASSERT("Supported count", lastCount64 < ~(size_t)0);
    lastCount = static_cast<size_t>(lastCount64);
    pageCount = lastPage - firstPage - 1;
  } else {
    // No page boundaries
    firstCount = byteCount;
    lastCount = 0;
    pageCount = 0;
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
  //   @rdesc The current size of this <c OMMemoryRawStorage> in bytes.
  //   @this const
OMUInt64 OMMemoryRawStorage::size(void) const
{
  TRACE("OMMemoryRawStorage::size");

  PRECONDITION("Sizeable", isSizeable());

  OMUInt64 result = _size;
  POSTCONDITION("Consistent size and page count",
                                   _size <= (_pageVector.count() * _pageSize));
  return result;
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
  //   @parm The new size of this <c OMMemoryRawStorage> in bytes.
  //   @devnote There is no ANSI way of truncating a file in place.
void OMMemoryRawStorage::setSize(OMUInt64 newSize)
{
  TRACE("OMMemoryRawStorage::setSize");

  PRECONDITION("Sizeable", isSizeable());

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
  //        offset in bytes from the beginning of this
  //        <c OMMemoryRawStorage>.
  //        precondition - isPositionable()
  //   @rdesc The current position for <f read()> and <f write()>.
  //   @this const
OMUInt64 OMMemoryRawStorage::position(void) const
{
  TRACE("OMMemoryRawStorage::position");

  PRECONDITION("Positionable", isPositionable());

  return _position;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the beginning of this
  //        <c OMMemoryRawStorage>.
  //        precondition - isPositionable()
  //   @parm The new position.
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMMemoryRawStorage::setPosition(OMUInt64 newPosition)
{
  TRACE("OMMemoryRawStorage::setPosition");

  PRECONDITION("Positionable", isPositionable());
  PRECONDITION("Valid position", newPosition < _size);

  _position = newPosition;
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
