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

  // @mfunc Save this <c OMDataStreamProperty> to the
  //        <c OMStoredObject> <p s>.
  //   @parm The <c OMStoredObject> on which to save this
  //         <c OMDataStreamProperty>.
  //   @this const
void OMDataStreamProperty::saveTo(OMStoredObject& s) const
{
  // stub
}

  // @mfunc Restore this <c OMDataStreamProperty> from the
  //        <c OMStoredObject> <p s>, the size of the
  //        <c OMDataStreamProperty> is <p size>.
  //   @parm The <c OMStoredObject> from which to restore this
  //         <c OMDataStreamProperty>.
  //   @parm The size of the <c OMDataStreamProperty>.
void OMDataStreamProperty::restoreFrom(OMStoredObject& s, size_t size)
{
  // stub
}

  // @mfunc The size, in bytes, of the data in this
  //        <c OMDataStreamProperty>.
  //   @rdesc  The size, in bytes, of the data in this
  //           <c OMDataStreamProperty>.
  //   @this const
OMUInt64 OMDataStreamProperty::size(void) const
{
  // stub
  return 0;
}

  // @mfunc Set the size, in bytes, of the data in this
  //        <c OMDataStreamProperty>.
  //   @rdesc  The new size, in bytes, of the data in this
  //           <c OMDataStreamProperty>.
void OMDataStreamProperty::setSize(const OMUInt64 newSize)
{
  // stub
}

  // @mfunc The current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //        The valid range for <p offset> is 0 .. <f size()>.
  //   @rdesc The current position for <f read()> and <f write()>, as an
  //          offset in bytes from the begining of the data stream.
  //   @this const
OMUInt64 OMDataStreamProperty::position(void) const
{
  // stub
  return 0;
}
  // @mfunc Set the current position for <f read()> and <f write()>, as an
  //        offset in bytes from the begining of the data stream.
  //        The valid range for <p offset> is 0 .. <f size()>.
  //   @parm The position to use for subsequent calls to read() and
  //         write() on this stream. The position is specified as an
  //          offset in bytes from the begining of the data stream.
  //   @this const
void OMDataStreamProperty::setPosition(const OMUInt64 offset)
{
  // stub
}

void OMDataStreamProperty::open(void)
{
  // stub
}

void OMDataStreamProperty::create(void)
{
  // stub
}

void OMDataStreamProperty::close(void)
{
  // stub
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
  // stub
  bytesRead = 0;
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
  // stub
  bytesWritten = 0;
}



