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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMSSStoredStream 
#include "OMMSSStoredStream.h"

#include "OMMSStructuredStorage.h"
#include "OMAssertions.h"

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
  ASSERT("Succeeded", SUCCEEDED(result)); // tjb - error

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
  ASSERT("Succeeded", SUCCEEDED(result)); // tjb - error
}

void OMMSSStoredStream::write(void* data, size_t size)
{
  TRACE("OMMSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesWritten;
  HRESULT resultCode = _stream->Write(data, size, &bytesWritten);
  ASSERT("Succeeded", SUCCEEDED(resultCode)); // tjb - error

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
  ASSERT("Succeeded", SUCCEEDED(resultCode)); // tjb - error
}

OMUInt64 OMMSSStoredStream::size(void) const
{
  TRACE("OMMSSStoredStream::size");
  PRECONDITION("Valid stream", _stream != 0);

  STATSTG statstg;
  HRESULT status = _stream->Stat(&statstg, STATFLAG_NONAME);
  ASSERT("Succeeded", SUCCEEDED(status)); // tjb - error

  OMUInt64 result = toOMUInt64(statstg.cbSize);
  return result;
}

void OMMSSStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMMSSStoredStream::setSize");

  ULARGE_INTEGER newStreamSize = fromOMUInt64(newSize);
  HRESULT status = _stream->SetSize(newStreamSize);
  ASSERT("Succeeded", SUCCEEDED(status)); // tjb - error
}

OMUInt64 OMMSSStoredStream::position(void) const
{
  TRACE("OMMSSStoredStream::position");
  PRECONDITION("Valid stream", _stream != 0);

  OMUInt64 result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = _stream->Seek(zero, STREAM_SEEK_CUR, &position);
  ASSERT("Succeeded", SUCCEEDED(status)); // tjb - error

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
  ASSERT("Succeeded", SUCCEEDED(status)); // tjb - error
}

void OMMSSStoredStream::close(void)
{
  TRACE("OMMSSStoredStream::close");
  PRECONDITION("Valid stream", _stream != 0);

  _stream->Release();
  _stream = 0;
}
