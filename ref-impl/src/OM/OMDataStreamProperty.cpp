// @doc OMEXTERNAL
#include "OMDataStreamProperty.h"

#include "OMPropertySet.h"
#include "OMStorable.h"
#include "OMStoredObject.h"

OMDataStreamProperty::OMDataStreamProperty(const OMPropertyId propertyId,
                                           const char* name)
  : OMProperty(propertyId, TID_DATA_STREAM, name),_stream(0)
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

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  // Use the property name as the stream name
  //
  const char* streamName = name();
  store->write(_propertyId,
               _type,
               (void*)streamName,
               strlen(streamName) + 1);

  // The stream has never been written to but we want the stream to
  // exist in the file, create it.
  //
  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
}

  // @mfunc Restore this <c OMDataStreamProperty>, the size of the
  //        <c OMDataStreamProperty> is <p size>.
  //   @parm The size of the <c OMDataStreamProperty>.
void OMDataStreamProperty::restore(size_t size)
{
  TRACE("OMDataStreamProperty::restore");

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  char* streamName = new char[size];
  ASSERT("Valid heap pointer", streamName != 0);
  store->read(_propertyId,
              _type,
              streamName,
              size);
  ASSERT("Consistent property size", size == strlen(streamName) + 1);
  ASSERT("Consistent property name", strcmp(streamName, name()) == 0);
  delete [] streamName;

  open();

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
  OMStoredObject* s = _propertySet->container()->store();
  result = s->streamSize(_stream);
 
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

  OMStoredObject* s = _propertySet->container()->store();
 
  s->streamSetSize(_stream, newSize);

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

  OMStoredObject* s = _propertySet->container()->store();
 
  OMUInt64 result = s->streamPosition(_stream);

  POSTCONDITION("Valid position", (result >= 0) && (result <= size()));
  return result;
}

  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //   @parm The position to use for subsequent calls to read() and
  //         write() on this stream. The position is specified as an
  //         offset in bytes from the begining of the data stream.
  //   @this const
void OMDataStreamProperty::setPosition(const OMUInt64 offset)
{
  TRACE("OMDataStreamProperty::setPosition");

  if (_stream == 0) {
    OMDataStreamProperty* p = const_cast<OMDataStreamProperty*>(this);
    p->create();
  }
  ASSERT("Valid stream", _stream != 0);

  OMStoredObject* s = _propertySet->container()->store();
 
  s->streamSetPosition(_stream, offset);

  POSTCONDITION("Position properly set", position() == offset);
}

void OMDataStreamProperty::open(void)
{
  TRACE("OMDataStreamProperty::open");
  PRECONDITION("Stream not already opened", _stream == 0);

  OMStoredObject* s = _propertySet->container()->store();
  _stream = s->openStream(name());

  POSTCONDITION("Stream opened", _stream != 0);
}

void OMDataStreamProperty::create(void)
{
  TRACE("OMDataStreamProperty::create");
  PRECONDITION("Stream not already created", _stream == 0);

  OMStoredObject* s = _propertySet->container()->store();
  _stream = s->createStream(name());

  POSTCONDITION("Stream opened", _stream != 0);
}

void OMDataStreamProperty::close(void)
{
  TRACE("OMDataStreamProperty::close");
  PRECONDITION("Stream not already closed", _stream != 0);

  OMStoredObject* s = _propertySet->container()->store();
  s->closeStream(_stream);

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
  OMStoredObject* s = _propertySet->container()->store();
  s->readFromStream(_stream, buffer, bytes, bytesRead);
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

  OMStoredObject* s = _propertySet->container()->store();
  s->writeToStream(_stream, buffer, bytes, bytesWritten);
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
  //   @parm The size of the buffer.
  //   @this const
void OMDataStreamProperty::getBits(OMByte* bits, size_t size) const
{
  TRACE("OMDataStreamProperty::getBits");
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  memcpy(bits, &_stream, bitsSize());
}



