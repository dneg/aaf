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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMKLVStoredStream 
#include "OMKLVStoredStream.h"

#include "OMAssertions.h"
#include "OMRawStorage.h"

OMKLVStoredStream::OMKLVStoredStream(OMRawStorage* store)
: _store(store)
{
  TRACE("OMKLVStoredStream::OMKLVStoredStream");
  PRECONDITION("Valid store", _store != 0);
}

OMKLVStoredStream::~OMKLVStoredStream(void)
{
  TRACE("OMKLVStoredStream::~OMKLVStoredStream");
  PRECONDITION("Stream not open", _store == 0);
}

void OMKLVStoredStream::read(void* data, size_t size) const
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

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStream::write(void* data, size_t size)
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

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

OMUInt64 OMKLVStoredStream::size(void) const
{
  TRACE("OMKLVStoredStream::size");
  PRECONDITION("Valid store", _store != 0);

  OMUInt64 result = 0;
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return result;
}

void OMKLVStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMKLVStoredStream::setSize");
  PRECONDITION("Valid store", _store != 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

OMUInt64 OMKLVStoredStream::position(void) const
{
  TRACE("OMKLVStoredStream::position");
  PRECONDITION("Valid store", _store != 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

void OMKLVStoredStream::setPosition(const OMUInt64 offset)
{
  TRACE("OMKLVStoredStream::setPosition");
  PRECONDITION("Valid store", _store != 0);

  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredStream::close(void)
{
  TRACE("OMKLVStoredStream::close");
  PRECONDITION("Valid store", _store != 0);

  delete _store;
  _store = 0;
}
