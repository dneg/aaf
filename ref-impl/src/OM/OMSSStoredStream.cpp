//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSSStoredStream

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMSSStoredStream.h"

#include "OMMSStructuredStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMTransparentStoredStreamFilter.h"

static void checkStatus(HRESULT status);

OMSSStoredStream::OMSSStoredStream(IStream* stream)
: _stream(stream)
{
  TRACE("OMSSStoredStream::OMSSStoredStream");
  PRECONDITION("Valid stream", _stream != 0);
}

OMSSStoredStream::~OMSSStoredStream(void)
{
  TRACE("OMSSStoredStream::~OMSSStoredStream");
  PRECONDITION("Stream not open", _stream == 0);
}

void OMSSStoredStream::read(void* data, OMUInt32 size) const
{
  TRACE("OMSSStoredStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  OMUInt32 bytesRead;
  HRESULT result = _stream->Read(data, size, &bytesRead);
  checkStatus(result);
  ASSERT("Succeeded", SUCCEEDED(result));

  ASSERT("Successful read", bytesRead == size);
}

void OMSSStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMSSStoredStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT result = _stream->Read(data, bytes, &bytesRead);
  checkStatus(result);
  ASSERT("Succeeded", SUCCEEDED(result));
}

void OMSSStoredStream::write(void* data, OMUInt32 size)
{
  TRACE("OMSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  OMUInt32 bytesWritten;
  HRESULT resultCode = _stream->Write(data, size, &bytesWritten);
  checkStatus(resultCode);
  ASSERT("Succeeded", SUCCEEDED(resultCode));

  ASSERT("Successful write", bytesWritten == size);
}

void OMSSStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT resultCode = _stream->Write(data, bytes, &bytesWritten);
  checkStatus(resultCode);
  ASSERT("Succeeded", SUCCEEDED(resultCode));
}

OMUInt64 OMSSStoredStream::size(void) const
{
  TRACE("OMSSStoredStream::size");
  PRECONDITION("Valid stream", _stream != 0);

  STATSTG statstg;
  HRESULT status = _stream->Stat(&statstg, STATFLAG_NONAME);
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  OMUInt64 result = toOMUInt64(statstg.cbSize);
  return result;
}

void OMSSStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMSSStoredStream::setSize");

  ULARGE_INTEGER newStreamSize = fromOMUInt64(newSize);
  HRESULT status = _stream->SetSize(newStreamSize);
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));
}

OMUInt64 OMSSStoredStream::position(void) const
{
  TRACE("OMSSStoredStream::position");
  PRECONDITION("Valid stream", _stream != 0);

  OMUInt64 result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = _stream->Seek(zero, STREAM_SEEK_CUR, &position);
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  result = toOMUInt64(position);
  return result;
}

void OMSSStoredStream::setPosition(const OMUInt64 offset) const
{
  TRACE("OMSSStoredStream::setPosition");
  PRECONDITION("Valid stream", _stream != 0);

  ULARGE_INTEGER newPosition = fromOMUInt64(offset);
  ULARGE_INTEGER oldPosition;
  LARGE_INTEGER position;
  memcpy(&position, &newPosition, sizeof(LARGE_INTEGER));
  HRESULT status = _stream->Seek(position, STREAM_SEEK_SET, &oldPosition);
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));
}

void OMSSStoredStream::close(void)
{
  TRACE("OMSSStoredStream::close");
  PRECONDITION("Valid stream", _stream != 0);

#if defined(OM_DEBUG)
  HRESULT status = _stream->Release();
  ASSERT("Reference count is 0.", status == 0);
#else
  _stream->Release();
#endif
  _stream = 0;
}

  // @mfunc Does this <c OMStoredStream> know about essence element keys?
  //   @rdesc True if this supports essence element keys, false otherwise.
  //   @this const
bool OMSSStoredStream::hasEssenceElementKey(void) const
{
  TRACE("OMSSStoredStream::hasEssenceElementKey");

  return false;
}

  // @mfunc The essence element key associated with this <c OMStoredStream>.
  //   @rdesc The essence element key.
  //   @this const
OMKLVKey OMSSStoredStream::essenceElementKey(void) const
{
  TRACE("OMSSStoredStream::essenceElementKey");
  PRECONDITION("Stream supports essence element keys", false);

  // Shouldn't be here
  return nullOMKLVKey;
}

  // @mfunc Specify the essence element key for this <c OMStoredStream>.
  //   @parm The essence element key.
void OMSSStoredStream::setEssenceElementKey(const OMKLVKey& /*key*/)
{
  TRACE("OMSSStoredStream::setEssenceElementKey");
  PRECONDITION("Stream supports essence element keys", false);
  // Shouldn't be here
}

OMStoredStreamFilter* OMSSStoredStream::createFilter(void)
{
  TRACE("OMKLVStoredStream::createFilter");

  OMStoredStreamFilter* result = new OMTransparentStoredStreamFilter(this);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

static void checkStatus(HRESULT status)
{
  TRACE("checkStatus");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

#endif // !OM_NO_STRUCTURED_STORAGE
