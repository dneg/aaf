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
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

OMMSSStoredStream::~OMMSSStoredStream(void)
{
  TRACE("OMMSSStoredStream::~OMMSSStoredStream");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

void OMMSSStoredStream::read(void* data, size_t size) const
{
  TRACE("OMMSSStoredStream::read");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

void OMMSSStoredStream::read(OMByte* data,
                             const OMUInt32 bytes,
                             OMUInt32& bytesRead) const
{
  TRACE("OMMSSStoredStream::read");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

void OMMSSStoredStream::write(void* data, size_t size)
{
  TRACE("OMMSSStoredStream::write");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

void OMMSSStoredStream::write(const OMByte* data,
                              const OMUInt32 bytes,
                              OMUInt32& bytesWritten)
{
  TRACE("OMMSSStoredStream::write");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

OMUInt64 OMMSSStoredStream::size(void) const
{
  TRACE("OMMSSStoredStream::size");
  ASSERT("Unimplemented code not reached", false);
  return 0; // tjb TBS
}

void OMMSSStoredStream::setSize(const OMUInt64 newSize)
{
  TRACE("OMMSSStoredStream::setSize");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

OMUInt64 OMMSSStoredStream::position(void) const
{
  TRACE("OMMSSStoredStream::position");
  ASSERT("Unimplemented code not reached", false);
  return 0; // tjb TBS
}

void OMMSSStoredStream::setPosition(const OMUInt64 offset)
{
  TRACE("OMMSSStoredStream::setPosition");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}

void OMMSSStoredStream::close(void)
{
  TRACE("OMMSSStoredStream::close");
  // tjb TBS
  ASSERT("Unimplemented code not reached", false);
}
