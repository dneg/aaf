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
#ifndef OMKLVSTOREDSTREAM_H
#define OMKLVSTOREDSTREAM_H

#include "OMStoredStream.h"
#include "OMVector.h"

class OMMXFStorage;

  // @class Implementation of <c OMStoredStream> for
  //        SMPTE (Society of Motion Picture and Television Engineers)
  //        Key Length Value (KLV) binary files.
  //   @base public | <c OMStoredStream>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredStream : public OMStoredStream {
public:
  // @access Public members.

    // @cmember Constructor.
  OMKLVStoredStream(OMMXFStorage* store, OMUInt32 sid);

    // @cmember Destructor.
  ~OMKLVStoredStream(void);

    // @cmember Read <p size> bytes from this <c OMKLVStoredStream>
    //          into the buffer at address <p data>.
  virtual void read(void* data, size_t size) const;

    // @cmember Attempt to read <p bytes> bytes from this
    //          <c OMKLVStoredStream> into the buffer at address <p data>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void read(OMByte* data,
                    const OMUInt32 bytes,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the vector of buffers given by <p buffers>
    //          from this <c OMKLVStoredStream>. This is "read scatter". The
    //          <p bufferCount> buffers are read in order until all have
    //          been successfully read or an error is encountered. Once
    //          an error has been encountered on one buffer no additional
    //          buffers are read.
    //          The number of bytes read is returned in <p bytesRead>.
  virtual void read(OMIOBufferDescriptor* buffers,
                    OMUInt32 bufferCount,
                    OMUInt32& bytesRead) const;

    // Asynchronous read - single buffer
  virtual void read(OMUInt64 position,
                    OMByte* buffer,
                    const OMUInt32 bytes,
                    void* /* */ completion,
                    const void* clientArgument);

    // Asynchronous read - multiple buffers
  virtual void read(OMUInt64 position,
                    OMIOBufferDescriptor* buffers,
                    OMUInt32 bufferCount,
                    void* /* */ completion,
                    const void* clientArgument) const;

    // @cmember Find out if <p bytesRequired> contiguous bytes, starting at
    //          <p position>, in this <c OMKLVStoredStream> are available for
    //          writing. The actual number of bytes available is returned
    //          in <p bytesAvailable>.
  virtual void probe(OMUInt64 position,
                     OMUInt32 bytesRequired,
                     OMUInt32& bytesAvailable) const;

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to this <c OMKLVStoredStream>.
  virtual void write(void* data, size_t size);

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to this <c OMKLVStoredStream>. The actual
    //          number of bytes written is returned in <p bytesWritten>.
  virtual void write(const OMByte* data,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten);

    // @cmember Attempt to write the vector of buffers given by <p buffers>
    //          to this <c OMKLVStoredStream>. This is "write gather". The
    //          <p bufferCount> buffers are written in order until all have
    //          been successfully written or an error is encountered. Once
    //          an error has been encountered on one buffer no additional
    //          buffers are written.
    //          The number of bytes written is returned in <p bytesWritten>.
  virtual void write(OMIOBufferDescriptor* buffers,
                     OMUInt32 bufferCount,
                     OMUInt32& bytesWritten);

    // Asynchronous write - single buffer
  virtual void write(OMUInt64 position,
                     const OMByte* buffer,
                     const OMUInt32 bytes,
                     void* /* */ completion,
                     const void* clientArgument);

    // Asynchronous write - multiple buffers
  virtual void write(OMUInt64 position,
                     const OMIOBufferDescriptor* buffers,
                     OMUInt32 bufferCount,
                     void* /* */ completion,
                     const void* clientArgument);

    // @cmember The size of this <c OMKLVStoredStream> in bytes.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMKLVStoredStream> to <p bytes>.
  virtual void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMKLVStoredStream>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMKLVStoredStream>.
  virtual void setPosition(const OMUInt64 offset) const;

    // @cmember Close this <c OMKLVStoredStream>.
  virtual void close(void);

  virtual OMUInt32 streamIdentification(void) const;

    // @cmember Set the label to <p label>.
  virtual void setLabel(const OMKLVKey& label);

    // @cmember Get the label.
  virtual OMKLVKey label(void) const;

    // @cmember Set the block size (alignment) of this essence element.
  virtual void setBlockSize(OMUInt32 blockSize);

    // @cmember The block size (alignment) of this essence element.
  virtual OMUInt32 blockSize(void) const;

    // @cmember Set the file offset of this essence element.
  virtual void setFileOffset(OMUInt64 fileOffset);

    // @cmember The file offset of this essence element.
  virtual OMUInt64 fileOffset(void) const;

  static bool readKLVKey(const OMStoredStream& stream, OMKLVKey& key);

  static bool readKLVLength(const OMStoredStream& stream, OMUInt64& length);

private:
  // @access Private members.
  OMKLVKey _label;
  OMUInt32 _blockSize;
  OMUInt64 _fileOffset;
  OMMXFStorage* _store;
  OMUInt32 _sid;
  OMUInt64 _position;

};

#endif
