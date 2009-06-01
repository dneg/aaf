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
#ifndef OMTRANSPARENTSTOREDSTREAMFILTER_H
#define OMTRANSPARENTSTOREDSTREAMFILTER_H

#include "OMStoredStreamFilter.h"

  // @class Implementation of <c OMStoredStreamFilter>.
  //   @base public | <c OMStoredStreamFilter>
class OMTransparentStoredStreamFilter : public OMStoredStreamFilter {
public:
  // @access Public members.

    // @cmember Constructor.
  OMTransparentStoredStreamFilter(OMStoredStream* stream);

    // @cmember Destructor.
  virtual ~OMTransparentStoredStreamFilter(void);

    // @cmember Read <p size> bytes from this <c OMTransparentStoredStreamFilter>
    //          into the buffer at address <p data>.
  virtual void read(void* data, OMUInt32 size) const;

    // @cmember Attempt to read <p bytes> bytes from this
    //          <c OMTransparentStoredStreamFilter> into the buffer at address <p data>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void read(OMByte* data,
                    const OMUInt32 bytes,
                    OMUInt32& bytesRead) const;

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to this <c OMTransparentStoredStreamFilter>.
  virtual void write(void* data, OMUInt32 size);

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to this <c OMTransparentStoredStreamFilter>. The actual
    //          number of bytes written is returned in <p bytesWritten>.
  virtual void write(const OMByte* data,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten);

    // @cmember The size of this <c OMTransparentStoredStreamFilter> in bytes.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMTransparentStoredStreamFilter> to <p bytes>.
  virtual void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMTransparentStoredStreamFilter>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMTransparentStoredStreamFilter>.
  virtual void setPosition(const OMUInt64 offset) const;

    // @cmember Close this <c OMTransparentStoredStreamFilter>.
//  virtual void close(void);

    // @cmember Get unfiltered <c OMStoredStream>.
  virtual const OMStoredStream* stream() const;

    // @cmember Get unfiltered <c OMStoredStream>.
  virtual OMStoredStream* stream();

private:

  OMStoredStream* _stream;
};

inline OMTransparentStoredStreamFilter::OMTransparentStoredStreamFilter(
                                                        OMStoredStream* stream)
: _stream(stream)
{
  TRACE("OMTransparentStoredStreamFilter::OMTransparentStoredStreamFilter");
  PRECONDITION("Valid stream", stream != 0);
}

inline OMTransparentStoredStreamFilter::~OMTransparentStoredStreamFilter(void)
{
  TRACE("OMTransparentStoredStreamFilter::~OMTransparentStoredStreamFilter");

  // We don't own the stream
  _stream = 0;
}

inline void OMTransparentStoredStreamFilter::read(void* data,
                                                  OMUInt32 size) const
{
  _stream->read(data, size);
}

  // @cmember Attempt to read <p bytes> bytes from this
  //          <c OMTransparentStoredStreamFilter> into the buffer at address <p data>.
  //          The actual number of bytes read is returned in <p bytesRead>.
inline void OMTransparentStoredStreamFilter::read(OMByte* data,
                                                  const OMUInt32 bytes,
                                                  OMUInt32& bytesRead) const
{
  _stream->read(data, bytes, bytesRead);
}

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to this <c OMTransparentStoredStreamFilter>.
inline void OMTransparentStoredStreamFilter::write(void* data,
                                                   OMUInt32 size)
{
  _stream->write(data, size);
}

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to this <c OMTransparentStoredStreamFilter>. The actual
    //          number of bytes written is returned in <p bytesWritten>.
inline void OMTransparentStoredStreamFilter::write(const OMByte* data,
                                                   const OMUInt32 bytes,
                                                   OMUInt32& bytesWritten)
{
  _stream->write(data, bytes, bytesWritten);
}

    // @cmember The size of this <c OMTransparentStoredStreamFilter> in bytes.
inline OMUInt64 OMTransparentStoredStreamFilter::size(void) const
{
  return _stream->size();
}

    // @cmember Set the size of this <c OMTransparentStoredStreamFilter> to <p bytes>.
inline void OMTransparentStoredStreamFilter::setSize(const OMUInt64 newSize)
{
  _stream->setSize(newSize);
}

    // @cmember The current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMTransparentStoredStreamFilter>.
inline OMUInt64 OMTransparentStoredStreamFilter::position(void) const
{
  return _stream->position();
}

    // @cmember Set the current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMTransparentStoredStreamFilter>.
inline void OMTransparentStoredStreamFilter::setPosition(
                                                   const OMUInt64 offset) const
{
  _stream->setPosition(offset);
}

    // @cmember Get unfiltered <c OMStoredStream>.
inline const OMStoredStream* OMTransparentStoredStreamFilter::stream() const
{
  return _stream;
}

    // @cmember Get unfiltered <c OMStoredStream>.
inline OMStoredStream* OMTransparentStoredStreamFilter::stream()
{
  return _stream;
}

#endif
