// @doc OMEXTERNAL
#include "OMDataStreamProperty.h"

#include "OMPropertySet.h"
#include "OMStorable.h"

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

  // @mfunc Restore this <c OMDataStreamProperty> from the
  //        <c OMStoredObject> <p s>, the size of the
  //        <c OMDataStreamProperty> is <p size>.
  //   @parm The <c OMStoredObject> from which to restore this
  //         <c OMDataStreamProperty>.
  //   @parm The size of the <c OMDataStreamProperty>.
void OMDataStreamProperty::restoreFrom(OMStoredObject& s, size_t size)
{
  TRACE("OMDataStreamProperty::restoreFrom");

  OMStoredObject* store = _propertySet->container()->store();
  char* streamName = new char[size];
  store->read(_propertyId,
              _type,
              streamName,
              size);
  ASSERT("Consistent stream and property names",
         strcmp(streamName, name()) == 0);
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
  
  OMUInt64 result;
  if (_stream != 0) {
    OMStoredObject* s = _propertySet->container()->store();
    result = s->streamSize(_stream);
  } else {
    result = 0;
  }
 
  return result;
}

  // @mfunc Set the size, in bytes, of the data in this
  //        <c OMDataStreamProperty>.
  //   @rdesc  The new size, in bytes, of the data in this
  //           <c OMDataStreamProperty>.
void OMDataStreamProperty::setSize(const OMUInt64 newSize)
{
  TRACE("OMDataStreamProperty::setSize");
  PRECONDITION("Stream already opened", _stream != 0);

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
  PRECONDITION("Stream already opened", _stream != 0);

  OMStoredObject* s = _propertySet->container()->store();
 
  OMUInt64 result = s->streamPosition(_stream);

  POSTCONDITION("Valid position", (result >= 0) && (result < size()));
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
  PRECONDITION("Stream already opened", _stream != 0);

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
  TRACE("OMDataStreamProperty::readInt8Array");

  OMDataStreamProperty* nonConstThis = const_cast<OMDataStreamProperty*>(this);
  if (_stream == 0) {
    nonConstThis->open();
  }
  ASSERT("Valid stream", _stream != 0);
  OMStoredObject* s = _propertySet->container()->store();
  s->readFromStream(_stream, buffer, bytes);
  bytesRead = bytes; // tjb
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
  TRACE("OMDataStreamProperty::writeInt8Array");

  if (_stream == 0) {
    create();
  }
  ASSERT("Valid stream", _stream != 0);
  OMStoredObject* s = _propertySet->container()->store();
  s->writeToStream(_stream, (void*)buffer, bytes);
  bytesWritten = bytes; // tjb
}



