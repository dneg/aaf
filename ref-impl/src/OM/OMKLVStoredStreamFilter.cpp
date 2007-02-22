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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#include "OMKLVStoredStreamFilter.h"

#include "OMAssertions.h"
#include "OMKLVStoredStream.h"
#include "OMMXFStorage.h"

OMKLVStoredStreamFilter::OMKLVStoredStreamFilter(OMKLVStoredStream* stream)
: _stream(stream),
  _initialized(false),
  _keyWritten(false),
  _position(0),
  _klvLength(0),
  _klvLengthOffset(0),
  _klvValueOffset(0)
{
  TRACE("OMKLVStoredStreamFilter::OMKLVStoredStreamFilter");

  POSTCONDITION("Valid stream", _stream != 0);
}

OMKLVStoredStreamFilter::~OMKLVStoredStreamFilter(void)
{
  TRACE("OMKLVStoredStreamFilter::~OMKLVStoredStreamFilter");

  // We don't own the stream
  _stream = 0;
}

void OMKLVStoredStreamFilter::read(void* ANAME(data), OMUInt32 ANAME(size)) const
{
  TRACE("OMKLVStoredStreamFilter::read");
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStreamFilter::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMKLVStoredStreamFilter::read");
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  if (!_initialized) {
    initialize();
  }

  const OMUInt64 remainingBytes = _klvLength - _position;
  OMUInt32 bytesToRead;
  if (bytes <= remainingBytes) {
    bytesToRead = bytes;
  } else {
    ASSERT("Supported byte count", remainingBytes < ~(OMUInt32)0);
    bytesToRead = static_cast<OMUInt32>(remainingBytes);
  }
  if (bytesToRead > 0) {
    _stream->setPosition(_klvValueOffset + _position);
    _stream->read(data, bytesToRead, bytesRead);

    OMKLVStoredStreamFilter* non_const_this =
                                    const_cast<OMKLVStoredStreamFilter*>(this);
    non_const_this->_position += bytesRead;
  } else {
    bytesRead = 0;
  }
}

void OMKLVStoredStreamFilter::write(void* ANAME(data), OMUInt32 ANAME(size))
{
  TRACE("OMKLVStoredStreamFilter::write");
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStreamFilter::write(const OMByte* data,
                                      const OMUInt32 bytes,
                                      OMUInt32& bytesWritten)
{
  TRACE("OMKLVStoredStreamFilter::write");
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  if (!_initialized) {
    initialize();
  }

  if (!_keyWritten)
  {
    OMKLVKey key = _stream->label();
    if (key == nullOMKLVKey) {
      OMKLVKey defalutKey = {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01,
                             0x0e, 0x04, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff};
      key = defalutKey;
      _stream->setLabel(key);
    }
    _stream->setPosition(0);
    OMKLVStoredStream::writeKLVKey(*_stream, key);
    _keyWritten = true;

    const OMUInt64 lengthPosition =
                                 OMKLVStoredStream::reserveKLVLength(*_stream);
    ASSERT("Valid KLV length offset", _klvLengthOffset == lengthPosition);
    ASSERT("Valid KLV value offset", _klvValueOffset == _stream->position());
  }

  _stream->setPosition(_klvValueOffset + _position);
  _stream->write(data, bytes, bytesWritten);
  _position += bytesWritten;
  if (_position > _klvLength) {
    OMKLVStoredStream::fixupKLVLength(*_stream, _klvLengthOffset);
    _klvLength = _position;
  }
}

OMUInt64 OMKLVStoredStreamFilter::size(void) const
{
  TRACE("OMKLVStoredStreamFilter::size");

  if (!_initialized) {
    initialize();
  }

  return _klvLength;
}

void OMKLVStoredStreamFilter::setSize(const OMUInt64 newSize)
{
  TRACE("OMKLVStoredStreamFilter::setSize");

  if (!_initialized) {
    initialize();
  }

  OMUInt64 currentPosition = _stream->position();

  _stream->setSize(_klvValueOffset + newSize);
  _klvLength = newSize;

  // Update KLV length
  _stream->setPosition(_klvValueOffset + newSize);
  OMKLVStoredStream::fixupKLVLength(*_stream, _klvLengthOffset);
  _stream->setPosition(currentPosition);
}

OMUInt64 OMKLVStoredStreamFilter::position(void) const
{
  TRACE("OMKLVStoredStreamFilter::position");

  if (!_initialized) {
    initialize();
  }

  return _position;
}

void OMKLVStoredStreamFilter::setPosition(const OMUInt64 newPosition) const
{
  TRACE("OMKLVStoredStreamFilter::setPosition");

  if (!_initialized) {
    initialize();
  }

  _stream->setPosition(_klvValueOffset + newPosition);
  OMKLVStoredStreamFilter* non_const_this =
                                    const_cast<OMKLVStoredStreamFilter*>(this);
  non_const_this->_position = newPosition;
}

void OMKLVStoredStreamFilter::close(void)
{
  TRACE("OMKLVStoredStreamFilter::close");

  _stream->close();
}

const OMStoredStream* OMKLVStoredStreamFilter::stream(void) const
{
  TRACE("OMKLVStoredStreamFilter::stream");
  return _stream;
}

OMStoredStream* OMKLVStoredStreamFilter::stream(void)
{
  TRACE("OMKLVStoredStreamFilter::stream");
  return _stream;
}

OMUInt32 OMKLVStoredStreamFilter::streamIdentification(void) const
{
  TRACE("OMKLVStoredStreamFilter::streamIdentification");
  return _stream->streamIdentification();
}

  // @mfunc Set the label to <p label>.
  //   @parm The new label.
void OMKLVStoredStreamFilter::setLabel(const OMKLVKey& label)
{
  TRACE("OMKLVStoredStreamFilter::setLabel");
  _stream->setLabel(label);
}

  // @mfunc Get the label.
  //   @rdesc The label.
OMKLVKey OMKLVStoredStreamFilter::label(void) const
{
  TRACE("OMKLVStoredStreamFilter::label");
  return _stream->label();
}

  // @mfunc Set the block size (alignment) of this essence element.
  //   @parm The block size.
void OMKLVStoredStreamFilter::setBlockSize(OMUInt32 blockSize)
{
  TRACE("OMKLVStoredStreamFilter::setBlockSize");
  _stream->setBlockSize(blockSize);
}

  // @mfunc The block size (alignment) of this essence element.
  //   @rdesc The block size.
OMUInt32 OMKLVStoredStreamFilter::blockSize(void) const
{
  TRACE("OMKLVStoredStreamFilter::blockSize");
  return _stream->blockSize();
}

void OMKLVStoredStreamFilter::initialize() const
{
  TRACE("OMKLVStoredStreamFilter::initialize");
  PRECONDITION("Not already initialized", _initialized == false);

  // Remember the current stream position
  const OMUInt64 currentStreamPosition = _stream->position();

  OMUInt64 lengthPosition = 0;
  OMUInt64 length = 0;
  OMUInt64 valuePosition = 0;

  // Attempt to read the key at the beginning of the stream
  bool readKL = false;
  _stream->setPosition(0);
  OMKLVKey key;
  if (OMKLVStoredStream::readKLVKey(*_stream, key)) {
    lengthPosition = _stream->position();

    // Attempt to read the length following the key
    if (OMKLVStoredStream::readKLVLength(*_stream, length)) {
      valuePosition = _stream->position();
      readKL = true;
    }
  }

  // If failed to read key an length assume the defaults
  if (!readKL) {
    lengthPosition = 16;
    length = 0;
    valuePosition = lengthPosition + 8 + 1;
  }

  OMKLVStoredStreamFilter* non_const_this =
                                    const_cast<OMKLVStoredStreamFilter*>(this);
  non_const_this->_klvLengthOffset = lengthPosition;
  non_const_this->_klvLength = length;
  non_const_this->_klvValueOffset = valuePosition;
  if (currentStreamPosition > _klvValueOffset) {
    non_const_this->_position = currentStreamPosition - _klvValueOffset;
  } else {
    non_const_this->_position = 0;
  }
  non_const_this->_initialized = true;

  // Restore the current stream position
  _stream->setPosition( currentStreamPosition );
}
