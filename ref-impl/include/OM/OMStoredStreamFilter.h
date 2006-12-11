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
#ifndef OMSTOREDSTREAMFILTER_H
#define OMSTOREDSTREAMFILTER_H

#include "OMDataTypes.h"

class OMStoredStream;

  // @class Filtered OMStoredStream
class OMStoredStreamFilter {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMStoredStreamFilter(void) {} = 0;

    // @cmember Read <p size> bytes from this <c OMStoredStreamFilter>
    //          into the buffer at address <p data>.
  virtual void read(void* data, OMUInt32 size) const = 0;

    // @cmember Attempt to read <p bytes> bytes from this
    //          <c OMStoredStreamFilter> into the buffer at address <p data>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void read(OMByte* data,
                    const OMUInt32 bytes,
                    OMUInt32& bytesRead) const = 0;

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to this <c OMStoredStreamFilter>.
  virtual void write(void* data, OMUInt32 size) = 0;

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to this <c OMStoredStreamFilter>. The actual
    //          number of bytes written is returned in <p bytesWritten>.
  virtual void write(const OMByte* data,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten) = 0;

    // @cmember The size of this <c OMStoredStreamFilter> in bytes.
  virtual OMUInt64 size(void) const = 0;

    // @cmember Set the size of this <c OMStoredStreamFilter> to <p bytes>.
  virtual void setSize(const OMUInt64 newSize) = 0;

    // @cmember The current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMStoredStreamFilter>.
  virtual OMUInt64 position(void) const = 0;

    // @cmember Set the current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMStoredStreamFilter>.
  virtual void setPosition(const OMUInt64 offset) const = 0;

    // @cmember Close this <c OMStoredStreamFilter>.
//  virtual void close(void) = 0;

    // @cmember Get unfiltered <c OMStoredStream>.
  virtual const OMStoredStream* stream() const = 0;

    // @cmember Get unfiltered <c OMStoredStream>.
  virtual OMStoredStream* stream() = 0;
};

#endif
