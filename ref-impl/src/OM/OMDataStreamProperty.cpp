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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDataStreamProperty

#include "OMDataStreamProperty.h"

#include "OMAssertions.h"

#include "OMStoredStream.h"
#include "OMPropertySet.h"
#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMType.h"
#include "OMUtilities.h"

OMDataStreamProperty::OMDataStreamProperty(const OMPropertyId propertyId,
                                           const wchar_t* name)
: OMDataStream(propertyId, name),
  _stream(0),
  _byteOrder(unspecified)
{
}

OMDataStreamProperty::~OMDataStreamProperty(void)
{
  TRACE("OMDataStreamProperty::~OMDataStreamProperty");
  if (_stream != 0) {
    close();
  }
  POSTCONDITION("Stream closed", _stream == 0);
}

  // @mfunc Save this <c OMDataStreamProperty>.
  //   @this const
void OMDataStreamProperty::save(void) const
{
  TRACE("OMDataStreamProperty::save");

  store()->save(*this);

  // The stream has never been written to but we want the stream to
  // exist in the file, create it.
  //
  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
}

  // @mfunc The number of objects contained within this
  //        <c OMDataStreamProperty> if any.
  //   @rdesc The number of objects.
OMUInt64 OMDataStreamProperty::objectCount(void) const
{
  TRACE("OMDataStreamProperty::objectCount");
  // OMDataStreamProperty doesn't contain objects.
  return 0;
}

  // @mfunc Restore this <c OMDataStreamProperty>, the size of the
  //        <c OMDataStreamProperty> is <p externalSize>.
  //   @parm The size of the <c OMDataStreamProperty>.
void OMDataStreamProperty::restore(size_t externalSize)
{
  TRACE("OMDataStreamProperty::restore");

  store()->restore(*this, externalSize);
  open();
  setPresent();

  POSTCONDITION("Properly opened stream", _stream != 0);
}

  // @mfunc The size, in bytes, of the data in this
  //        <c OMDataStreamProperty>.
  //   @rdesc  The size, in bytes, of the data in this
  //           <c OMDataStreamProperty>.
  //   @this const
OMUInt64 OMDataStreamProperty::size(void) const
{
  TRACE("OMDataStreamProperty::size");

  OMUInt64 result = stream()->size();

  return result;
}

  // @mfunc Set the size, in bytes, of the data in this
  //        <c OMDataStreamProperty>.
  //   @rdesc  The new size, in bytes, of the data in this
  //           <c OMDataStreamProperty>.
void OMDataStreamProperty::setSize(const OMUInt64 newSize)
{
  TRACE("OMDataStreamProperty::setSize");

  stream()->setSize(newSize);

  POSTCONDITION("Size properly set", size() == newSize);
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //   @rdesc The current position for <f read()> and <f write()>, as an
  //          offset in bytes from the begining of the data stream.
  //   @this const
OMUInt64 OMDataStreamProperty::position(void) const
{
  TRACE("OMDataStreamProperty::position");

  OMUInt64 result = stream()->position();

  POSTCONDITION("Valid position", result <= size());
  return result;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //   @parm The position to use for subsequent calls to read() and
  //         write() on this stream. The position is specified as an
  //         offset in bytes from the begining of the data stream.
  //   @this const
void OMDataStreamProperty::setPosition(const OMUInt64 offset) const
{
  TRACE("OMDataStreamProperty::setPosition");

  stream()->setPosition(offset);

  POSTCONDITION("Position properly set", position() == offset);
}

OMStoredStream* OMDataStreamProperty::stream(void) const
{
  TRACE("OMDataStreamProperty::stream");
  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
  ASSERT("Valid stream", _stream != 0);
  return _stream;
}

void OMDataStreamProperty::open(void)
{
  TRACE("OMDataStreamProperty::open");
  PRECONDITION("Stream not already opened", _stream == 0);

  _stream = store()->openStoredStream(*this);

  POSTCONDITION("Stream opened", _stream != 0);
}

void OMDataStreamProperty::create(void)
{
  TRACE("OMDataStreamProperty::create");
  PRECONDITION("Stream not already created", _stream == 0);

  _stream = store()->createStoredStream(*this);

  POSTCONDITION("Stream opened", _stream != 0);
}

void OMDataStreamProperty::close(void)
{
  TRACE("OMDataStreamProperty::close");
  PRECONDITION("Stream not already closed", _stream != 0);

  _stream->close();
  delete _stream;
  _stream = 0;

  POSTCONDITION("Stream closed", _stream == 0);
}

  // @mfunc Attempt to read the number of bytes given by <p bytes>
  //          from the data stream into the buffer at address
  //          <p buffer>. The actual number of bytes read is returned
  //          in <p bytesRead>.
  //   @parm The address of the buffer into which the bytes should be read.
  //   @parm The number of bytes to read.
  //   @parm The actual number of bytes that were read.
  //   @this const
void OMDataStreamProperty::read(OMByte* buffer,
                                const OMUInt32 bytes,
                                OMUInt32& bytesRead) const
{
  TRACE("OMDataStreamProperty::read");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  stream()->read(buffer, bytes, bytesRead);
}

  // @mfunc  Attempt to write the number of bytes given by <p bytes>
  //         to the data stream from the buffer at address
  //          <p buffer>. The actual number of bytes written is returned
  //          in <p bytesWritten>.
  //   @parm  The address of the buffer from which the bytes should be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes that were written.
void OMDataStreamProperty::write(const OMByte* buffer,
                                 const OMUInt32 bytes,
                                 OMUInt32& bytesWritten)
{
  TRACE("OMDataStreamProperty::write");

  stream()->write(buffer, bytes, bytesWritten);
  setPresent();
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
void OMDataStreamProperty::readTypedElements(const OMType* elementType,
                                             size_t externalElementSize,
                                             OMByte* elements,
                                             OMUInt32 elementCount,
                                             OMUInt32& elementsRead) const
{
  TRACE("OMDataStreamProperty::readTypedElements");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid element type", elementType != 0);
  PRECONDITION("Valid element size", externalElementSize!= 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Stream byte order is known", hasByteOrder());

  bool reorder = false;
  if (byteOrder() != hostByteOrder()) {
    reorder = true;
  }

  // Allocate buffer for one element
  OMByte* buffer = new OMByte[externalElementSize];

  for (size_t i = 0; i < elementCount; i++) {

    // Read an element of the property value
    OMUInt32 actualByteCount;
    read(buffer, externalElementSize, actualByteCount);
    ASSERT("All bytes read", actualByteCount == externalElementSize);

    // Reorder an element of the property value
    if (reorder) {
      elementType->reorder(buffer, externalElementSize);
    }

    // Internalize an element of the property value
    size_t requiredBytesSize = elementType->internalSize(buffer,
                                                         externalElementSize);

    elementType->internalize(buffer,
                             externalElementSize,
                             &elements[i * requiredBytesSize],
                             requiredBytesSize,
                             hostByteOrder());
  }
  delete [] buffer;
  elementsRead = elementCount;
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
void OMDataStreamProperty::writeTypedElements(const OMType* elementType,
                                              size_t internalElementSize,
                                              const OMByte* elements,
                                              OMUInt32 elementCount,
                                              OMUInt32& elementsWritten)
{
  TRACE("OMDataStreamProperty::writeTypedElements");

  PRECONDITION("Valid element type", elementType != 0);
  PRECONDITION("Valid element size", internalElementSize!= 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Stream byte order is known", hasByteOrder());

  bool reorder = false;
  if (byteOrder() != hostByteOrder()) {
    reorder = true;
  }

  // Allocate buffer for one element
  size_t externalBytesSize = elementType->externalSize(
                                                 const_cast<OMByte*>(elements),
                                                 internalElementSize);

  OMByte* buffer = new OMByte[externalBytesSize];

  for (size_t i = 0; i < elementCount; i++) {

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
  setPresent();
}

  // @mfunc The size of the raw bits of this
  //        <c OMDataStreamProperty>. The size is given in bytes.
  //   @rdesc The size of the raw bits of this
  //          <c OMDataStreamProperty> in bytes.
  //   @this const
size_t OMDataStreamProperty::bitsSize(void) const
{
  TRACE("OMDataStreamProperty::bitsSize");

  return sizeof(OMStoredStream*); // tjb !
}

  // @mfunc Get the raw bits of this <c OMDataStreamProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
  //   @this const
void OMDataStreamProperty::getBits(OMByte* bits, size_t ANAME(size)) const
{
  TRACE("OMDataStreamProperty::getBits");
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  memcpy(bits, &_stream, bitsSize());
}

  // @mfunc Set the raw bits of this <c OMDataStreamProperty>. The raw
  //         bits are copied from the buffer at address <p bits> which
  //         is <p size> bytes in size.
  //   @parm const OMByte* | bits | The address of the buffer from which the
  //         raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
void OMDataStreamProperty::setBits(const OMByte* ANAME(bits),
                                   size_t ANAME(size))
{
  TRACE("OMDataStreamProperty::setBits");
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Is a byte order specifed for this stream ?
bool OMDataStreamProperty::hasByteOrder(void) const
{
  TRACE("OMDataStreamProperty::hasByteOrder");

  bool result;
  if ((_byteOrder == littleEndian) || (_byteOrder == bigEndian)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Specify a byte order for this stream.
void OMDataStreamProperty::setByteOrder(OMByteOrder byteOrder)
{
  TRACE("OMDataStreamProperty::setByteOrder");

  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("No existing byte order", !hasByteOrder());
  PRECONDITION("Stream is empty", size() == 0);

  _byteOrder = byteOrder;

  POSTCONDITION("Byte order properly set", hasByteOrder());
}

  // @mfunc The byte order of this stream.
OMByteOrder OMDataStreamProperty::byteOrder(void) const
{
  TRACE("OMDataStreamProperty::byteOrder");

  PRECONDITION("Byte order set", hasByteOrder());

  return _byteOrder;
}

  // @mfunc Clear the byte order of this stream
void OMDataStreamProperty::clearByteOrder(void)
{
  TRACE("OMDataStreamProperty::clearByteOrder");

  PRECONDITION("Byte order set", hasByteOrder());

  _byteOrder = unspecified;

  POSTCONDITION("Byte order properly cleared", !hasByteOrder());
}

OMByteOrder OMDataStreamProperty::storedByteOrder(void) const
{
  TRACE("OMDataStreamProperty::storedByteOrder");

  return _byteOrder;
}

void OMDataStreamProperty::setStoredByteOrder(OMByteOrder byteOrder)
{
  TRACE("OMDataStreamProperty::setStoredByteOrder");

  _byteOrder = byteOrder;
}

void OMDataStreamProperty::shallowCopyTo(OMProperty* /* destination */) const
{
  TRACE("OMDataStreamProperty::shallowCopyTo");
  // Nothing to do - this is a shallow copy
}

void OMDataStreamProperty::deepCopyTo(OMProperty* destination,
                                      void* /* clientContext */) const
{
  TRACE("OMDataStreamProperty::deepCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMDataStreamProperty Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  ASSERT("Destination stream is empty", dest->size() == 0);
  if (hasByteOrder()) {
    dest->setByteOrder(byteOrder());
  }

  // Save current position of source
  OMUInt64 savedPosition = position();

  // Copy the stream contents
  setPosition(0);
  dest->setPosition(0);
  OMUInt32 bytesRead;
  OMUInt32 bytesWritten;
  OMUInt64 bytesToCopy = size();
  OMByte buffer[1024];
  while (bytesToCopy != 0) {
    read(buffer, sizeof(buffer), bytesRead);
    dest->write(buffer, bytesRead, bytesWritten);
    bytesToCopy = bytesToCopy - bytesWritten;
  }

  // Restore current position of source
  setPosition(savedPosition);
}

const wchar_t* OMDataStreamProperty::storedName(void) const
{
  TRACE("OMDataStreamProperty::storedName");

  if (_storedName == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->_storedName = OMStoredObject::streamName(_name, propertyId());
  }
  return _storedName;
}
