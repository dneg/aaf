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
#ifndef OMRAWSTORAGE_H
#define OMRAWSTORAGE_H

#include "OMDataTypes.h"

  // @class Abstract base class supporting access to the raw bytes of
  //        files supported by the Object Manager.
  //
  //        Object Manager clients use this interface, after a file has been
  //        saved and closed, to access the raw bytes of a file, or, before
  //        a file has been opened, to supply the raw bytes to be used for
  //        that file.
  //
  //        Object Manager clients implement this interface to allow files
  //        to be stored in locations not known to the Object Manager.
  //
  //        For example, Object Manager clients may wish to store files in
  //        a proprietary file system, this could be done by providing an
  //        implementation of this interface for that file system.
  //
  //        Additionally a number of built-in implementations of this
  //        interface are provided -
  //
  //        <c OMDiskRawStorage>       - an implementation of <c OMRawStorage>
  //                                     for disk files. Uses ANSI file
  //                                     functions only.
  //
  //        <c OMMemoryRawStorage>     - an implementation of <c OMRawStorage>
  //                                     that stores the file in memory.
  //
  //        <c OMMappedFileRawStorage> - an implementation of <c OMRawStorage>
  //                                     for files mapped into memory.
  //
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMRawStorage {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMRawStorage(void) {}

    // @cmember Is it possible to read from this <c OMRawStorage> ?
  virtual bool isReadable(void) const = 0;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()>
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const = 0;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const = 0;

    // @cmember Is it possible to write to this <c OMRawStorage> ?
  virtual bool isWritable(void) const = 0;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMRawStorage::size> causes this <c OMRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()>
    //   @devnote How is failure to extend indicated ?
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten) = 0;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMRawStorage::size> causes this <c OMRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
    //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten) = 0;

    // @cmember May this <c OMRawStorage> be changed in size ?
    //          An implementation of <c OMRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for fixed size contiguous memory files (avoiding copying)
    //          would return false.
  virtual bool isExtendible(void) const = 0;

    // @cmember The current extent of this <c OMRawStorage> in bytes.
    //          The <f extent()> is the allocated size, while the <f size()>
    //          is the valid size.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 extent(void) const = 0;

    // @cmember Set the size of this <c OMRawStorage> to <p newSize> bytes.
    //          If <p newSize> is greater than <mf OMRawStorage::size>
    //          then this <c OMRawStorage> is extended. If <p newSize>
    //          is less than <mf OMRawStorage::size> then this
    //          <c OMRawStorage> is truncated. Truncation may also result
    //          in the current position for <f read()> and <f write()>
    //          being set to <mf OMRawStorage::size>.
    //          @precondition <f isExtendible()>
    //   @devnote How is failure to extend indicated ?
  virtual void extend(OMUInt64 newSize) = 0;

    // @cmember The current size of this <c OMRawStorage> in bytes.
    //          The <f size()> is the valid size, while the <f extent()>
    //          is the allocated size.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 size(void) const = 0;

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMRawStorage> be changed ?
    //          An implementation of <c OMRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for memory files would return true.
  virtual bool isPositionable(void) const = 0;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMRawStorage>.
  virtual OMUInt64 position(void) const = 0;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition) const = 0;

    // @cmember Synchronize this <c OMRawStorage> with its external
    //          representation.
    //          An implementation of <c OMRawStorage> for disk files would
    //          most probably implement this virtual function as a flush.
    //          This virtual function would probably be implemented as a
    //          noop in implemetations for network streams and for memory
    //          files.
  virtual void synchronize(void) = 0;

};

#endif
