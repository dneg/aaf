// @doc OMINTERNAL
#ifndef OMDATASTREAMPROPERTY_H
#define OMDATASTREAMPROPERTY_H

#include "OMDataTypes.h"

#include "OMProperty.h"

  // @class Persistent data stream properties supported by the Object
  //        Manager.
  //   @base | public <c OMProperty>.
class OMDataStreamProperty : public OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMDataStreamProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMDataStreamProperty(void);

    // @cmember Save this <c OMDataStreamProperty>.
    // @this const
  virtual void save(void) const;

    // @cmember Restore this <c OMDataStreamProperty>, the size of the
    //          <c OMDataStreamProperty> is <p size>.
  virtual void restore(size_t size);

    // @cmember Close this  <c OMDataStreamProperty>.
  virtual void close(void);

    // @cmember The size, in bytes, of the data in this
    //          <c OMDataStreamProperty>.
    //   @this const
  OMUInt64 size(void) const;

    // @cmember Set the size, in bytes, of the data in this
    //          <c OMDataStreamProperty>.
  void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
    // @this const
  OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
  void setPosition(const OMUInt64 offset);

    // @cmember Attempt to read the number of bytes given by <p bytes>
    //          from the data stream into the buffer at address
    //          <p buffer>. The actual number of bytes read is returned
    //          in <p bytesRead>.
    //   @this const
  void read(OMByte* buffer,
            const OMUInt32 bytes,
            OMUInt32& bytesRead) const;

    // @cmember Attempt to write the number of bytes given by <p bytes>
    //          to the data stream from the buffer at address
    //          <p buffer>. The actual number of bytes written is returned
    //          in <p bytesWritten>.
  void write(const OMByte* buffer,
             const OMUInt32 bytes,
             OMUInt32& bytesWritten);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMDataStreamProperty>. The size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMDataStreamProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

private:

  void open(void);
  void create(void);

  IStream* _stream;

};

#endif
