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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMSSStoredStream 
#include "OMMSSStoredStream.h"

#include "OMMSStructuredStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"

static void check(HRESULT status);

OMMSSStoredStream::OMMSSStoredStream(IStream* stream)
: _stream(stream)
{
  TRACE("OMMSSStoredStream::OMMSSStoredStream");
  PRECONDITION("Valid stream", _stream != 0);
}

OMMSSStoredStream::~OMMSSStoredStream(void)
{
  TRACE("OMMSSStoredStream::~OMMSSStoredStream");
  PRECONDITION("Stream not open", _stream == 0);
}

void OMMSSStoredStream::read(void* data, size_t size) const
{
  TRACE("OMMSSStoredStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesRead;
  HRESULT result = _stream->Read(data, size, &bytesRead);
  check(result);
  ASSERT("Succeeded", SUCCEEDED(result));

  ASSERT("Successful read", bytesRead == size);
}

void OMMSSStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMMSSStoredStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT result = _stream->Read(data, bytes, &bytesRead);
  check(result);
  ASSERT("Succeeded", SUCCEEDED(result));
}

void OMMSSStoredStream::write(void* data, size_t size)
{
  TRACE("OMMSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesWritten;
  HRESULT resultCode = _stream->Write(data, size, &bytesWritten);
  check(resultCode);
  ASSERT("Succeeded", SUCCEEDED(resultCode));

  ASSERT("Successful write", bytesWritten == size);
}

void OMMSSStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMMSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT resultCode = _stream->Write(data, bytes, &bytesWritten);
  check(resultCode);
  ASSERT("Succeeded", SUCCEEDED(resultCode));
}

OMUInt64 OMMSSStoredStream::size(void) const
{
  TRACE("OMMSSStoredStream::size");
  PRECONDITION("Valid stream", _stream != 0);

  STATSTG statstg;
  HRESULT status = _stream->Stat(&statstg, STATFLAG_NONAME);
  check(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  OMUInt64 result = toOMUInt64(statstg.cbSize);
  return result;
}

void OMMSSStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMMSSStoredStream::setSize");

  ULARGE_INTEGER newStreamSize = fromOMUInt64(newSize);
  HRESULT status = _stream->SetSize(newStreamSize);
  check(status);
  ASSERT("Succeeded", SUCCEEDED(status));
}

OMUInt64 OMMSSStoredStream::position(void) const
{
  TRACE("OMMSSStoredStream::position");
  PRECONDITION("Valid stream", _stream != 0);

  OMUInt64 result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = _stream->Seek(zero, STREAM_SEEK_CUR, &position);
  check(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  result = toOMUInt64(position);
  return result;
}

void OMMSSStoredStream::setPosition(const OMUInt64 offset)
{
  TRACE("OMMSSStoredStream::setPosition");
  PRECONDITION("Valid stream", _stream != 0);

  ULARGE_INTEGER newPosition = fromOMUInt64(offset);
  ULARGE_INTEGER oldPosition;
  LARGE_INTEGER position;
  memcpy(&position, &newPosition, sizeof(LARGE_INTEGER));
  HRESULT status = _stream->Seek(position, STREAM_SEEK_SET, &oldPosition);
  check(status);
  ASSERT("Succeeded", SUCCEEDED(status));
}

void OMMSSStoredStream::close(void)
{
  TRACE("OMMSSStoredStream::close");
  PRECONDITION("Valid stream", _stream != 0);

#if defined(OM_ENABLE_DEBUG)
  HRESULT status = _stream->Release();
  ASSERT("Reference count is 0.", status == 0);
#else
  _stream->Release();
#endif
  _stream = 0;
}

static void check(HRESULT status)
{
  TRACE("check");

  if (FAILED(status)) {
    throw OMException(status);
  }
}
