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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMXMLPStoredStream
#include "OMXMLPStoredStream.h"

#include "OMAssertions.h"
#include "OMRawStorage.h"
#include "OMUtilities.h"

OMXMLPStoredStream::OMXMLPStoredStream(OMRawStorage* store)
: _store(store),
  _position(0)
{
  TRACE("OMXMLPStoredStream::OMXMLPStoredStream");
  PRECONDITION("Valid store", _store != 0);
}

OMXMLPStoredStream::~OMXMLPStoredStream(void)
{
  TRACE("OMXMLPStoredStream::~OMXMLPStoredStream");
  PRECONDITION("Stream not open", _store == 0);
}

void OMXMLPStoredStream::read(void* data, size_t size) const
{
  TRACE("OMXMLPStoredStream::read");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  OMUInt32 bytesRead;
  read(reinterpret_cast<OMByte*>(data), size, bytesRead);

  ASSERT("Successful read", bytesRead == size);
}

void OMXMLPStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMXMLPStoredStream::read");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  _store->readAt(_position, data, bytes, bytesRead);

  OMXMLPStoredStream* nonConstThis = const_cast<OMXMLPStoredStream*>(this);
  nonConstThis->_position = _position + bytesRead;
}

void OMXMLPStoredStream::write(void* data, size_t size)
{
  TRACE("OMXMLPStoredStream::write");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  OMUInt32 bytesWritten;
  write(reinterpret_cast<OMByte*>(data), size, bytesWritten);

  ASSERT("Successful write", bytesWritten == size);
}

void OMXMLPStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMXMLPStoredStream::write");
  PRECONDITION("Valid store", _store != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  _store->writeAt(_position, data, bytes, bytesWritten);
  _position = _position + bytesWritten;
}

OMUInt64 OMXMLPStoredStream::size(void) const
{
  TRACE("OMXMLPStoredStream::size");
  PRECONDITION("Valid store", _store != 0);

  OMUInt64 result = _store->extent();
  return result;
}

void OMXMLPStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMXMLPStoredStream::setSize");
  PRECONDITION("Valid store", _store != 0);

  _store->extend(newSize);
}

OMUInt64 OMXMLPStoredStream::position(void) const
{
  TRACE("OMXMLPStoredStream::position");
  PRECONDITION("Valid store", _store != 0);

  return _position;
}

void OMXMLPStoredStream::setPosition(const OMUInt64 offset)
{
  TRACE("OMXMLPStoredStream::setPosition");
  PRECONDITION("Valid store", _store != 0);

  _position = offset;
}

void OMXMLPStoredStream::close(void)
{
  TRACE("OMXMLPStoredStream::close");
  PRECONDITION("Valid store", _store != 0);

  delete _store;
  _store = 0;
  _position = 0;
}
