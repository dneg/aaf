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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSSSStoredStream 
#include "OMSSSStoredStream.h"

#include "OMSSSStructuredStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"

typedef int sresult;
#define SUCCEEDED(result) (SSTG_OK==result)
static void checkStatus(sresult status);

OMSSSStoredStream::OMSSSStoredStream(Stream* stream)
: _stream(stream)
{
  TRACE("OMSSSStoredStream::OMSSSStoredStream");
  PRECONDITION("Valid stream", _stream != 0);
}

OMSSSStoredStream::~OMSSSStoredStream(void)
{
  TRACE("OMSSSStoredStream::~OMSSSStoredStream");
  PRECONDITION("Stream not open", _stream == 0);
}

void OMSSSStoredStream::read(void* data, size_t size) const
{
  TRACE("OMSSSStoredStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesRead = size;
  sresult result = streamRead( _stream, data, &bytesRead);
  checkStatus(result);
  ASSERT("Succeeded", SUCCEEDED(result));

  ASSERT("Successful read", bytesRead == size);
}

void OMSSSStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMSSSStoredStream::read");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

	bytesRead = bytes;

  sresult result = streamRead( _stream, data, &bytesRead);
  checkStatus(result);
  ASSERT("Succeeded", SUCCEEDED(result));
}

void OMSSSStoredStream::write(void* data, size_t size)
{
  TRACE("OMSSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesWritten = (unsigned long)size;
  sresult resultCode = streamWrite( _stream, data, &bytesWritten);
  checkStatus(resultCode);
  ASSERT("Succeeded", SUCCEEDED(resultCode));

  ASSERT("Successful write", bytesWritten == size);
}

void OMSSSStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMSSSStoredStream::write");
  PRECONDITION("Valid stream", _stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

	bytesWritten = bytes;
  sresult resultCode = streamWrite( _stream, data, &bytesWritten);
  checkStatus(resultCode);
  ASSERT("Succeeded", SUCCEEDED(resultCode));
}

OMUInt64 OMSSSStoredStream::size(void) const
{
  TRACE("OMSSSStoredStream::size");
  PRECONDITION("Valid stream", _stream != 0);

  StgInfo* info;
  sresult status = getStreamInfo( _stream, &info );
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  OMUInt64 result = info->size;

	status = freeInfo( &info, 1 );
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

	// return result;

	// apparently, info->size isn't updated after write()
	// these four lines get the actual end of stream in sz
	OMUInt64 p = position();
	sresult ss = streamSeek64( _stream, 0, STG_END );
	OMUInt64 sz = position();
	ss = streamSeek64( _stream, p, STG_START );
  return sz;
}

void OMSSSStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMSSSStoredStream::setSize");
  PRECONDITION("Valid stream", _stream != 0);

	sresult status = streamResize( _stream, newSize );
	/*pre64 
	// PRECONDITION("Valid Size", newsize<MAXINT32);
	OMUInt32 pos = (OMUInt32)newSize;
	pos -= 1;
	sresult status = streamSeek( stream,  pos, STG_START );

	OMUInt8 newTerm = 0;
	OMUInt32 count = 1;
  status = streamWrite( stream, &newTerm, &count );
	*/
}

OMUInt64 OMSSSStoredStream::position(void) const
{
  TRACE("OMSSSStoredStream::position");
  PRECONDITION("Valid stream", _stream != 0);

	OMUInt32 pos;
  sresult s = streamGetPos( _stream, &pos);

  OMUInt64 result;
	sresult status = streamGetPos64( _stream, &result );

	/*pre64
	OMUInt32 pos;
  sresult status = streamGetPos( _stream, &pos);

	StgInfo * pSI;
  sresult s = 0;
	s=::getStreamInfo(_stream,&pSI);

  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  result = pos;
	*/

  return result;
}

void OMSSSStoredStream::setPosition(const OMUInt64 offset)
{
  TRACE("OMSSSStoredStream::setPosition");
  PRECONDITION("Valid stream", _stream != 0);

	//pre64 OMUInt32 position = (OMUInt32)offset;
	//pre64 sresult status = streamSeek( _stream,  position, STG_START );
	sresult status = streamSeek64( _stream, offset, STG_START );

  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));
}

void OMSSSStoredStream::close(void)
{
  TRACE("OMSSSStoredStream::close");
  PRECONDITION("Valid stream", _stream != 0);

#if defined(OM_DEBUG)
  sresult status = closeStream( &_stream );
  ASSERT("Reference count is 0.", status == 0);
#else
  closeStream( &_stream );
#endif
}

static void checkStatus(sresult status)
{
  TRACE("checkStatus");

  if (SSTG_OK!=status) {
    throw OMException(status | 0x80000000L);
  }
}
