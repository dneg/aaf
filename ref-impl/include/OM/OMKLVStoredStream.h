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
#ifndef OMKLVSTOREDSTREAM_H
#define OMKLVSTOREDSTREAM_H

#include "OMStoredStream.h"

class OMRawStorage;

  // @class Implementation of <c OMStoredStream> for
  //        SMPTE (Society of Motion Picture and Television Engineers)
  //        Key Length Value (KLV) binary files.
  //   @base public | <c OMStoredStream>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredStream : public OMStoredStream {
public:
  // @access Public members.

    // @cmember Constructor.
  OMKLVStoredStream(OMRawStorage* store);

    // @cmember Destructor.
  ~OMKLVStoredStream(void);

    // @cmember Read <p size> bytes from this <c OMXMLStoredStream>
    //          into the buffer at address <p data>.
  virtual void read(void* data, size_t size) const;

    // @cmember Attempt to read <p bytes> bytes from this
    //          <c OMXMLStoredStream> into the buffer at address <p data>.
    //          The actual number of bytes read is returned in <p bytesRead>.
  virtual void read(OMByte* data,
                    const OMUInt32 bytes,
                    OMUInt32& bytesRead) const;

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to this <c OMXMLStoredStream>.
  virtual void write(void* data, size_t size);

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to this <c OMXMLStoredStream>. The actual
    //          number of bytes written is returned in <p bytesWritten>.
  virtual void write(const OMByte* data,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten);

    // @cmember The size of this <c OMXMLStoredStream> in bytes.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMXMLStoredStream> to <p bytes>.
  virtual void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMXMLStoredStream>.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and
    //          <f write()>, as an offset in bytes from the
    //          begining of this <c OMXMLStoredStream>.
  virtual void setPosition(const OMUInt64 offset);

    // @cmember Close this <c OMXMLStoredStream>.
  virtual void close(void);

private:
  // @access Private members.

  OMRawStorage* _store;

};

#endif
