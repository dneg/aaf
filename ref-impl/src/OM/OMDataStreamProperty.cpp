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

// @doc OMEXTERNAL
#include "OMDataStreamProperty.h"

#include "OMAssertions.h"

#include "OMPropertySet.h"
#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMType.h"
#include "OMUtilities.h"

OMDataStreamProperty::OMDataStreamProperty(const OMPropertyId propertyId,
                                           const wchar_t* name)
  : OMProperty(propertyId, SF_DATA_STREAM, name),_stream(0),
    _byteOrder(unspecified)
{
}

OMDataStreamProperty::~OMDataStreamProperty(void)
{
}

  // @mfunc Save this <c OMDataStreamProperty>.
  //   @this const
void OMDataStreamProperty::save(void) const
{
  TRACE("OMDataStreamProperty::save");

  // Use the property name as the stream name
  //
  const wchar_t* name = storedName();
  store()->saveStream(_propertyId, _storedForm, name, _byteOrder);

  // The stream has never been written to but we want the stream to
  // exist in the file, create it.
  //
  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
}

  // @mfunc Restore this <c OMDataStreamProperty>, the size of the
  //        <c OMDataStreamProperty> is <p externalSize>.
  //   @parm The size of the <c OMDataStreamProperty>.
void OMDataStreamProperty::restore(size_t externalSize)
{
  TRACE("OMDataStreamProperty::restore");

  size_t characterCount = lengthOfWideString(storedName()) + 1;
  size_t size = (characterCount * sizeof(OMCharacter)) + 1;
  ASSERT("Consistent property size", size == externalSize);

  wchar_t* name = 0;
  OMByteOrder bo;
  store()->restoreStream(_propertyId, _storedForm, size, &name, &bo);
  ASSERT("Consistent property name",
                                   compareWideString(name, storedName()) == 0);
  ASSERT("Valid stored byte order", ((bo == littleEndian) ||
                                     (bo == bigEndian) ||
                                     (bo == unspecified)));
  _byteOrder = bo;
  delete [] name;

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

  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
  ASSERT("Valid stream", _stream != 0);

  OMUInt64 result;
  result = store()->streamSize(_stream);
 
  return result;
}

  // @mfunc Set the size, in bytes, of the data in this
  //        <c OMDataStreamProperty>.
  //   @rdesc  The new size, in bytes, of the data in this
  //           <c OMDataStreamProperty>.
void OMDataStreamProperty::setSize(const OMUInt64 newSize)
{
  TRACE("OMDataStreamProperty::setSize");

  if (_stream == 0) {
    create();
  }
  ASSERT("Valid stream", _stream != 0);

  store()->streamSetSize(_stream, newSize);

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

  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
  ASSERT("Valid stream", _stream != 0);

  OMUInt64 result = store()->streamPosition(_stream);

  POSTCONDITION("Valid position", (result >= 0) && (result <= size()));
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

  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
  ASSERT("Valid stream", _stream != 0);

  store()->streamSetPosition(_stream, offset);

  POSTCONDITION("Position properly set", position() == offset);
}

void OMDataStreamProperty::open(void)
{
  TRACE("OMDataStreamProperty::open");
  PRECONDITION("Stream not already opened", _stream == 0);

  _stream = store()->openStream(storedName());

  POSTCONDITION("Stream opened", _stream != 0);
}

void OMDataStreamProperty::create(void)
{
  TRACE("OMDataStreamProperty::create");
  PRECONDITION("Stream not already created", _stream == 0);

  _stream = store()->createStream(storedName());

  POSTCONDITION("Stream opened", _stream != 0);
}

void OMDataStreamProperty::close(void)
{
  TRACE("OMDataStreamProperty::close");
  PRECONDITION("Stream not already closed", _stream != 0);

  store()->closeStream(_stream);

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

  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }

  ASSERT("Valid stream", _stream != 0);

  store()->readFromStream(_stream, buffer, bytes, bytesRead);
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

  if (_stream == 0) {
    create();
  }
  ASSERT("Valid stream", _stream != 0);

  store()->writeToStream(_stream, buffer, bytes, bytesWritten);
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
  TRACE("OMDataStreamProperty::readElements");

  PRECONDITION("Valid element type", elementType != 0);
  PRECONDITION("Valid element size", externalElementSize!= 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid element count", elementCount > 0);

  // Allocate buffer for one element
  OMByte* buffer = new OMByte[externalElementSize];

  for (size_t i = 0; i < elementCount; i++) {

    // Read an element of the property value
    OMUInt32 actualByteCount;
    read(buffer, externalElementSize, actualByteCount);
    ASSERT("All bytes read", actualByteCount == externalElementSize);

    // Reorder an element of the property value
    if (store()->byteOrder() != hostByteOrder()) {
      elementType->reorder(buffer, externalElementSize);
    }

    // Internalize an element of the property value
    size_t requiredBytesSize = elementType->internalSize(buffer,
                                                         externalElementSize);
    ASSERT("Internal element size equals external element size",
                                     requiredBytesSize == externalElementSize);

    elementType->internalize(buffer,
                             externalElementSize,
                             &elements[i * externalElementSize],
                             requiredBytesSize,
                             hostByteOrder());
  }
  delete [] buffer;
  elementsRead = elementCount;
}


  // @mfunc Attempt to write the number of elements given by
  //        <p elementCount> and described by <p elementType> and
  //        <p externalElementSize> to the data stream from the buffer
  //        at address <p elements>. The actual number of elements written
  //        is returned in <p elementsWritten>.
  //   @parm The element type
  //   @parm The external element size 
  //   @parm The address of the buffer from which the elements should
  //         be written.
  //   @parm The number of elements to write.
  //   @parm The actual number of elements that were written.
void OMDataStreamProperty::writeTypedElements(const OMType* elementType,
                                              size_t externalElementSize,
                                              const OMByte* elements,
                                              OMUInt32 elementCount,
                                              OMUInt32& elementsWritten)
{
  TRACE("OMDataStreamProperty::writeElements");

  PRECONDITION("Valid element type", elementType != 0);
  PRECONDITION("Valid element size", externalElementSize!= 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid element count", elementCount > 0);

  // Allocate buffer for one element
  size_t externalBytesSize = elementType->externalSize(
                                                 const_cast<OMByte*>(elements),
                                                 externalElementSize);
  ASSERT("Internal element size equals external element size",
                                     externalBytesSize == externalElementSize);
  OMByte* buffer = new OMByte[externalBytesSize];

  for (size_t i = 0; i < elementCount; i++) {
 
    // Externalize an element of the property value
    elementType->externalize(
                       const_cast<OMByte*>(&elements[i * externalElementSize]),
                       externalElementSize,
                       buffer,
                       externalBytesSize,
                       store()->byteOrder());

    // Reorder an element of the property value
    if (store()->byteOrder() != hostByteOrder()) {
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

  // @mfunc The size of the raw bits of this
  //        <c OMDataStreamProperty>. The size is given in bytes.
  //   @rdesc The size of the raw bits of this
  //          <c OMDataStreamProperty> in bytes.
  //   @this const
size_t OMDataStreamProperty::bitsSize(void) const
{
  TRACE("OMDataStreamProperty::bitsSize");

  return sizeof(IStream*);
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

const wchar_t* OMDataStreamProperty::storedName(void) const
{
  TRACE("OMDataStreamProperty::storedName");

  if (_storedName == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->_storedName = OMStoredObject::streamName(_name, propertyId());
  }
  return _storedName;
}
