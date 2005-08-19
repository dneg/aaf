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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMKLVStoredStream
#include "OMKLVStoredStream.h"

#include "OMAssertions.h"
#include "OMRawStorage.h"

OMKLVStoredStream::OMKLVStoredStream(OMRawStorage* store)
: _label(nullOMKLVKey),
  _blockSize(0),
  _fileOffset(0),
  _store(store),
  _position(0)
{
  TRACE("OMKLVStoredStream::OMKLVStoredStream");
  PRECONDITION("Valid store", _store != 0);
}

OMKLVStoredStream::~OMKLVStoredStream(void)
{
  TRACE("OMKLVStoredStream::~OMKLVStoredStream");
  PRECONDITION("Stream not open", _store == 0);
}

void OMKLVStoredStream::read(void* ANAME(data), size_t ANAME(size)) const
{
  TRACE("OMKLVStoredStream::read");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMKLVStoredStream::read");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  _store->readAt(_position, data, bytes, bytesRead);
  OMKLVStoredStream* nonConstThis = const_cast<OMKLVStoredStream*>(this);
  nonConstThis->_position = _position + bytesRead;
}

void OMKLVStoredStream::write(void* ANAME(data), size_t ANAME(size))
{
  TRACE("OMKLVStoredStream::write");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMKLVStoredStream::write");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  _store->writeAt(_position,
                  reinterpret_cast<const OMByte*>(data),
                  bytes,
                  bytesWritten);
  _position = _position + bytesWritten;
}

OMUInt64 OMKLVStoredStream::size(void) const
{
  TRACE("OMKLVStoredStream::size");
  PRECONDITION("Valid store", _store != 0);

  OMUInt64 result = _store->size();
  return result;
}

void OMKLVStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMKLVStoredStream::setSize");
  PRECONDITION("Valid store", _store != 0);

  _store->extend(newSize);
}

OMUInt64 OMKLVStoredStream::position(void) const
{
  TRACE("OMKLVStoredStream::position");
  PRECONDITION("Valid store", _store != 0);

  return _position;
}

void OMKLVStoredStream::setPosition(const OMUInt64 offset)
{
  TRACE("OMKLVStoredStream::setPosition");
  PRECONDITION("Valid store", _store != 0);

  _position = offset;
}

void OMKLVStoredStream::close(void)
{
  TRACE("OMKLVStoredStream::close");
  PRECONDITION("Valid store", _store != 0);

  delete _store;
  _store = 0;
}

  // @mfunc Set the label to <p label>.
  //   @parm The new label.
void OMKLVStoredStream::setLabel(const OMKLVKey& label)
{
  TRACE("OMKLVStoredStream::setLabel");
  _label = label;
}

  // @mfunc Get the label.
  //   @rdesc The label.
OMKLVKey OMKLVStoredStream::label(void) const
{
  TRACE("OMKLVStoredStream::label");
  return _label;
}

  // @mfunc Set the essence element label to <p label>.
  //   @parm The new label.
void OMKLVStoredStream::setEssenceElementLabel(const OMKLVKey& label)
{
  TRACE("OMKLVStoredStream::setEssenceElementLabel");
  setLabel(label);
}

  // @mfunc Get the essence element label.
  //   @rdesc The label.
OMKLVKey OMKLVStoredStream::essenceElementLabel(void) const
{
  TRACE("OMKLVStoredStream::essenceElementLabel");
  return label();
}

  // @mfunc Set the block size (alignment) of this essence element.
  //   @parm The block size.
void OMKLVStoredStream::setBlockSize(OMUInt32 blockSize)
{
  TRACE("OMKLVStoredStream::setBlockSize");
  _blockSize = blockSize;
}

  // @mfunc The block size (alignment) of this essence element.
  //   @rdecs The block size.
OMUInt32 OMKLVStoredStream::blockSize(void) const
{
  TRACE("OMKLVStoredStream::blockSize");
  return _blockSize;
}

  // @mfunc Set the file offset of this essence element.
  //   @parm The file offset.
void OMKLVStoredStream::setFileOffset(OMUInt64 fileOffset)
{
  TRACE("OMKLVStoredStream::setFileOffset");
  _fileOffset = fileOffset;
}

  // @mfunc The file offset of this essence element.
  //   @rdesc The file offset.
OMUInt64 OMKLVStoredStream::fileOffset(void) const
{
  TRACE("OMKLVStoredStream::fileOffset");
  return _fileOffset;
}
