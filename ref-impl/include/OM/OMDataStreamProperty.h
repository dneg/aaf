// @doc OMINTERNAL
#ifndef OMDATASTREAMPROPERTY_H
#define OMDATASTREAMPROPERTY_H

#include "OMTypes.h"

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

    // @cmember Save this <c OMDataStreamProperty> to the
    //          <c OMStoredObject> <p s>.
    // @this const
  virtual void saveTo(OMStoredObject& s) const;

    // @cmember Restore this <c OMDataStreamProperty> from the
    //          <c OMStoredObject> <p s>, the size of the
    //          <c OMDataStreamProperty> is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size);

    // @cmember The size, in bytes, of the data in this
    //          <c OMDataStreamProperty>.
    //   @this const
  OMUInt64 size(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
    //          The valid range for <p offset> is 0 .. <f size()>.
    // @this const
  OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
    //          The valid range for <p offset> is 0 .. <f size()>.
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

private:

  void open(void);
  void create(void);
  void close(void);

  IStream* _stream;

};

#endif
