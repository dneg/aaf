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
#ifndef OMMEMORYRAWSTORAGE_H
#define OMMEMORYRAWSTORAGE_H

#include "OMRawStorage.h"
#include "OMFile.h"
#include "OMVector.h"

#include <stdio.h>

  // @class Class supporting access to the raw bytes of memory
  //        files supported by the Object Manager.
  //
  //        This is an Object Manager built-in implementation of the
  //        <c OMRawStorage> interface.
  //
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMMemoryRawStorage : public OMRawStorage {
public:
  // @access Static members.

    // @cmember Create an <c OMMemoryRawStorage> object for modify access.
  static OMMemoryRawStorage* openNewModify(void);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMMemoryRawStorage(void);

    // @cmember Is it possible to read from this <c OMMemoryRawStorage> ?
  virtual bool isReadable(void) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMMemoryRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMemoryRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMMemoryRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMemoryRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 possition,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Is it possible to write to this <c OMMemoryRawStorage> ?
  virtual bool isWritable(void) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMMemoryRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMemoryRawStorage::size> causes this
    //          <c OMMemoryRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMMemoryRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMemoryRawStorage::size> causes this
    //          <c OMMemoryRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
    //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember May this <c OMMemoryRawStorage> be changed in size ?
  virtual bool isExtendible(void) const;

    // @cmember The current size of this <c OMMemoryRawStorage> in bytes.
    //          precondition - isExtendible()
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMMemoryRawStorage> to <p newSize>
    //          bytes.
    //          If <p newSize> is greater than <mf OMMemoryRawStorage::size>
    //          then this <c OMMemoryRawStorage> is extended. If <p newSize>
    //          is less than <mf OMMemoryRawStorage::size> then this
    //          <c OMMemoryRawStorage> is truncated. Truncation may also result
    //          in the current position for <f read()> and <f write()>
    //          being set to <mf OMMemoryRawStorage::size>.
    //          precondition - isExtendible()
  virtual void extend(OMUInt64 newSize);

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMMemoryRawStorage> be changed ?
  virtual bool isPositionable(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMemoryRawStorage>.
    //          precondition - isPositionable()
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMemoryRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition);

    // @cmember Synchronize this <c OMMemoryRawStorage> with its external
    //          representation.
  virtual void synchronize(void);

private:
  // @access Private members.

    // @cmember Constructor.
  OMMemoryRawStorage(void);

    // @cmember Write a page or partial page.
  virtual void write(size_t page,
                     size_t offset,
                     size_t byteCount,
                     const OMByte* source);

    // @cmember Read a page or partial page.
  virtual void read(size_t page,
                    size_t offset,
                    size_t byteCount,
                    OMByte* destination) const;

  OMVector<OMByte*> _pageVector;
  size_t _pageSize;
  OMUInt64 _size;
  OMUInt64 _position;
};

#endif
