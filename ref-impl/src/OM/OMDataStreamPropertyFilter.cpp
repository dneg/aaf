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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMDataStreamPropertyFilter.h"

#include "OMAssertions.h"
#include "OMDataStreamProperty.h"
#include "OMStoredStream.h"
#include "OMStoredStreamFilter.h"
#include "OMType.h"
#include "OMUtilities.h"

OMDataStreamPropertyFilter::OMDataStreamPropertyFilter(
  OMDataStreamProperty* property)
: _property(property),
  _streamFilter(0)
{
  TRACE("OMDataStreamPropertyFilter::OMDataStreamPropertyFilter");
  PRECONDITION("Valid property", property != 0);

  _streamFilter = property->stream()->createFilter();

  POSTCONDITION("Valid stream filter", _streamFilter != 0);
}

OMDataStreamPropertyFilter::~OMDataStreamPropertyFilter(void)
{
  TRACE("OMDataStreamPropertyFilter::~OMDataStreamPropertyFilter");

  delete _streamFilter;
  _streamFilter = 0;
}

  // @mfunc The size, in bytes, of the data in this
  //        <c OMDataStreamPropertyFilter>.
  //   @rdesc  The size, in bytes, of the data in this
  //           <c OMDataStreamPropertyFilter>.
  //   @this const
OMUInt64 OMDataStreamPropertyFilter::size(void) const
{
  TRACE("OMDataStreamPropertyFilter::size");

  OMUInt64 result = _streamFilter->size();

  return result;
}

  // @mfunc Set the size, in bytes, of the data in this
  //        <c OMDataStreamPropertyFilter>.
  //   @rdesc  The new size, in bytes, of the data in this
  //           <c OMDataStreamPropertyFilter>.
void OMDataStreamPropertyFilter::setSize(const OMUInt64 newSize)
{
  TRACE("OMDataStreamPropertyFilter::setSize");

  _streamFilter->setSize(newSize);

  POSTCONDITION("Size properly set", size() == newSize);
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //   @rdesc The current position for <f read()> and <f write()>, as an
  //          offset in bytes from the begining of the data stream.
  //   @this const
OMUInt64 OMDataStreamPropertyFilter::position(void) const
{
  TRACE("OMDataStreamPropertyFilter::position");

  OMUInt64 result = _streamFilter->position();
  return result;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //   @parm The position to use for subsequent calls to read() and
  //         write() on this stream. The position is specified as an
  //         offset in bytes from the begining of the data stream.
  //   @this const
void OMDataStreamPropertyFilter::setPosition(const OMUInt64 offset) const
{
  TRACE("OMDataStreamPropertyFilter::setPosition");

  _streamFilter->setPosition(offset);

  POSTCONDITION("Position properly set", position() == offset);
}

  // @mfunc Attempt to read the number of bytes given by <p bytes>
  //          from the data stream into the buffer at address
  //          <p buffer>. The actual number of bytes read is returned
  //          in <p bytesRead>.
  //   @parm The address of the buffer into which the bytes should be read.
  //   @parm The number of bytes to read.
  //   @parm The actual number of bytes that were read.
  //   @this const
void OMDataStreamPropertyFilter::read(OMByte* buffer,
                                const OMUInt32 bytes,
                                OMUInt32& bytesRead) const
{
  TRACE("OMDataStreamPropertyFilter::read");

  PRECONDITION("Optional property is present",
                     IMPLIES(_property->isOptional(), _property->isPresent()));

  _streamFilter->read(buffer, bytes, bytesRead);
}

  // @mfunc  Attempt to write the number of bytes given by <p bytes>
  //         to the data stream from the buffer at address
  //          <p buffer>. The actual number of bytes written is returned
  //          in <p bytesWritten>.
  //   @parm  The address of the buffer from which the bytes should be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes that were written.
void OMDataStreamPropertyFilter::write(const OMByte* buffer,
                                 const OMUInt32 bytes,
                                 OMUInt32& bytesWritten)
{
  TRACE("OMDataStreamPropertyFilter::write");

  _streamFilter->write(buffer, bytes, bytesWritten);
}

  // @mfunc Attempt to read the number of elements given by
  //        <p elementCount> and described by <p elementType> and
  //        <p externalElementSize> from the data stream into the buffer
  //        at address <p elements>. The actual number of elements read
  //        is returned in <p elementsRead>.
  //   @parm The element type
  //   @parm The external element size
  //   @parm The address of the buffer into which the elements should be read.
  //   @parm The number of elements to read.
  //   @parm The actual number of elements that were read.
  //   @this const
void OMDataStreamPropertyFilter::readTypedElements(const OMType* elementType,
                                           OMUInt32 externalElementSize,
                                           OMByte* elements,
                                           OMUInt32 elementCount,
                                           OMUInt32& elementsRead) const
{
  TRACE("OMDataStreamPropertyFilter::readTypedElements");

  PRECONDITION("Optional property is present",
                     IMPLIES(_property->isOptional(), _property->isPresent()));
  PRECONDITION("Valid element type", elementType != 0);
  PRECONDITION("Valid element size", externalElementSize!= 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Stream byte order is known", _property->hasByteOrder());

  OMUInt64 currentPosition = position();
  OMUInt64 streamSize = size();

  OMUInt32 readCount = 0;
  if (currentPosition < streamSize) {
    OMUInt64 remaining = (streamSize - currentPosition) / externalElementSize;
    if (remaining < elementCount) {
      readCount = static_cast<OMUInt32>(remaining);
    } else {
      readCount = elementCount;
    }
  }
  if (readCount > 0) {

    bool reorder = false;
    if (_property->byteOrder() != hostByteOrder()) {
      reorder = true;
    }

    // Allocate buffer for one element
    OMByte* buffer = new OMByte[externalElementSize];

    for (OMUInt32 i = 0; i < readCount; i++) {

      // Read an element of the property value
      OMUInt32 actualByteCount;
      read(buffer, externalElementSize, actualByteCount);
      ASSERT("All bytes read", actualByteCount == externalElementSize);

      // Reorder an element of the property value
      if (reorder) {
        elementType->reorder(buffer, externalElementSize);
      }

      // Internalize an element of the property value
      OMUInt32 requiredBytesSize = elementType->internalSize(
                                                          buffer,
                                                          externalElementSize);

      elementType->internalize(buffer,
                             externalElementSize,
                             &elements[i * requiredBytesSize],
                             requiredBytesSize,
                             hostByteOrder());
    }
    delete [] buffer;
  }
  elementsRead = readCount;
}


  // @mfunc Attempt to write the number of elements given by
  //        <p elementCount> and described by <p elementType> and
  //        <p internalElementSize> to the data stream from the buffer
  //        at address <p elements>. The actual number of elements written
  //        is returned in <p elementsWritten>.
  //   @parm The element type
  //   @parm The internal element size
  //   @parm The address of the buffer from which the elements should
  //         be written.
  //   @parm The number of elements to write.
  //   @parm The actual number of elements that were written.
void OMDataStreamPropertyFilter::writeTypedElements(const OMType* elementType,
                                            OMUInt32 internalElementSize,
                                            const OMByte* elements,
                                            OMUInt32 elementCount,
                                            OMUInt32& elementsWritten)
{
  TRACE("OMDataStreamPropertyFilter::writeTypedElements");

  PRECONDITION("Valid element type", elementType != 0);
  PRECONDITION("Valid element size", internalElementSize!= 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Stream byte order is known", _property->hasByteOrder());

  bool reorder = false;
  if (_property->byteOrder() != hostByteOrder()) {
    reorder = true;
  }

  // Allocate buffer for one element
  OMUInt32 externalBytesSize = elementType->externalSize(
                                                 const_cast<OMByte*>(elements),
                                                 internalElementSize);

  OMByte* buffer = new OMByte[externalBytesSize];

  for (OMUInt32 i = 0; i < elementCount; i++) {

    // Externalize an element of the property value
    elementType->externalize(
                       const_cast<OMByte*>(&elements[i * internalElementSize]),
                       internalElementSize,
                       buffer,
                       externalBytesSize,
                       hostByteOrder());

    // Reorder an element of the property value
    if (reorder) {
      elementType->reorder(buffer, externalBytesSize);
    }

    // Write an element of the property value
    OMUInt32 actualByteCount;
    write(buffer, externalBytesSize, actualByteCount);
    ASSERT("All bytes written", actualByteCount == externalBytesSize);
  }
  delete [] buffer;
  elementsWritten = elementCount;
}

