//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMWRAPPEDRAWSTORAGE_H
#define OMWRAPPEDRAWSTORAGE_H

#include "OMRawStorage.h"
#include "OMDataTypes.h"


  // @class Wrapped <c OMRawStorage>
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMWrappedRawStorage : public OMRawStorage {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWrappedRawStorage(OMRawStorage* store);

    // @cmember Destructor.
  virtual ~OMWrappedRawStorage(void);

    // @cmember Is it possible to read from this <c OMWrappedRawStorage> ?
  virtual bool isReadable(void) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMWrappedRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMWrappedRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()>
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMWrappedRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMWrappedRawStorage::size> causes <p bytesRead> to be less
    //          than <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Is it possible to write to this <c OMWrappedRawStorage> ?
  virtual bool isWritable(void) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMWrappedRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMWrappedRawStorage::size> causes this
    //          <c OMWrappedRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()>
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMWrappedRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMWrappedRawStorage::size> causes this
    //          <c OMWrappedRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember May this <c OMWrappedRawStorage> be changed in size ?
    //          An implementation of <c OMWrappedRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for fixed size contiguous memory files (avoiding copying)
    //          would return false.
  virtual bool isExtendible(void) const;

    // @cmember The current extent of this <c OMWrappedRawStorage> in bytes.
    //          The <f extent()> is the allocated size, while the <f size()>
    //          is the valid size.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 extent(void) const;

    // @cmember Set the size of this <c OMWrappedRawStorage> to <p newSize>
    //          bytes.
    //          If <p newSize> is greater than <mf OMWrappedRawStorage::size>
    //          then this <c OMWrappedRawStorage> is extended. If <p newSize>
    //          is less than <mf OMWrappedRawStorage::size> then this
    //          <c OMWrappedRawStorage> is truncated. Truncation may also
    //          result
    //          in the current position for <f read()> and <f write()>
    //          being set to <mf OMWrappedRawStorage::size>.
    //          @precondition <f isExtendible()>
  virtual void extend(OMUInt64 newSize);

    // @cmember The current size of this <c OMWrappedRawStorage> in bytes.
    //          The <f size()> is the valid size, while the <f extent()>
    //          is the allocated size.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 size(void) const;

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMWrappedRawStorage> be changed ?
    //          An implementation of <c OMWrappedRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for memory files would return true.
  virtual bool isPositionable(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMWrappedRawStorage>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMWrappedRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition) const;

    // @cmember Synchronize this <c OMWrappedRawStorage> with its external
    //          representation.
  virtual void synchronize(void);

  // Functions for accessing stream data within this <c OMWrappedRawStorage>

    // @cmember Attempt to read the number of stream data bytes given by
    //          <p byteCount> from offset <p position> in this
    //          <c OMWrappedRawStorage> into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void streamReadAt(OMUInt64 position,
                            OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead) const;

    // @cmember Attempt to fill the vector of buffers given by <p buffers>
    //          with stream data read from offset <p position> in
    //          this <c OMWrappedRawStorage>. This is "read scatter".
    //          The <p bufferCount> buffers are read in order until all have
    //          been successfully read or an error is encountered. Once
    //          an error has been encountered on one buffer no additional
    //          buffers are read.
    //          The number of bytes read is returned in <p bytesRead>.
  virtual void streamReadAt(OMUInt64 position,
                            OMIOBufferDescriptor* buffers,
                            OMUInt32 bufferCount,
                            OMUInt32& bytesRead) const;

    // Asynchronous read - single buffer
  virtual void streamReadAt(OMUInt64 position,
                            OMByte* buffer,
                            const OMUInt32 bytes,
                            void* /* */ completion,
                            const void* clientArgument) const;

    // Asynchronous read - multiple buffers
  virtual void streamReadAt(OMUInt64 position,
                            OMIOBufferDescriptor* buffers,
                            OMUInt32 bufferCount,
                            void* /* */ completion,
                            const void* clientArgument) const;

    // @cmember Attempt to write the number of stream data bytes given by
    //          <p byteCount> to offset  <p position> in this
    //          <c OMWrappedRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
  virtual void streamWriteAt(OMUInt64 position,
                             const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten);

    // @cmember Attempt to write the vector of buffers given by <p buffers>
    //          to this <c OMWrappedRawStorage>. Stream data bytes are written
    //          starting at offset <p position> in this
    //          <c OMWrappedRawStorage>.
    //          This is "write gather".
    //          The <p bufferCount> buffers are written in order until all have
    //          been successfully written or an error is encountered. Once
    //          an error has been encountered on one buffer no additional
    //          buffers are written.
    //          The number of bytes written is returned in <p bytesWritten>.
  virtual void streamWriteAt(OMUInt64 position,
                             OMIOBufferDescriptor* buffers,
                             OMUInt32 bufferCount,
                             OMUInt32& bytesWritten);

    // Asynchronous write - single buffer
  virtual void streamWriteAt(OMUInt64 position,
                             const OMByte* buffer,
                             const OMUInt32 bytes,
                             void* /* */ completion,
                             const void* clientArgument);

    // Asynchronous write - multiple buffers
  virtual void streamWriteAt(OMUInt64 position,
                             const OMIOBufferDescriptor* buffers,
                             OMUInt32 bufferCount,
                             void* /* */ completion,
                             const void* clientArgument);

private:
  OMRawStorage* _store;
};

#endif
