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
#ifndef OMDATASTREAMPROPERTY_H
#define OMDATASTREAMPROPERTY_H

#include "OMDataTypes.h"

#include "OMDataStream.h"

class OMStoredStream;

  // @class Persistent data stream properties supported by the Object
  //        Manager.
  //   @base | public <c OMDataStream>.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDataStreamProperty : public OMDataStream {
public:
  // @access Public members.

    // @cmember Constructor.
  OMDataStreamProperty(const OMPropertyId propertyId, const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMDataStreamProperty(void);

    // @cmember Save this <c OMDataStreamProperty>.
  virtual void save(void) const;

    // @cmember The number of objects contained within this
    //          <c OMDataStreamProperty> if any.
  virtual OMUInt64 objectCount(void) const;

    // @cmember Restore this <c OMDataStreamProperty>, the size of the
    //          <c OMDataStreamProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember Close this  <c OMDataStreamProperty>.
  virtual void close(void);

    // @cmember The size, in bytes, of the data in this
    //          <c OMDataStreamProperty>.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size, in bytes, of the data in this
    //          <c OMDataStreamProperty>.
  virtual void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
  OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
  virtual void setPosition(const OMUInt64 offset) const;

  // Raw access interface - these functions do not use the OMType interface

    // @cmember Attempt to read the number of bytes given by <p bytes>
    //          from the data stream into the buffer at address
    //          <p buffer>. The actual number of bytes read is returned
    //          in <p bytesRead>.
  virtual void read(OMByte* buffer,
                    const OMUInt32 bytes,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to write the number of bytes given by <p bytes>
    //          to the data stream from the buffer at address
    //          <p buffer>. The actual number of bytes written is returned
    //          in <p bytesWritten>.
  virtual void write(const OMByte* buffer,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten);

  // Typed access interface - these functions use the OMType interface

    // @cmember Attempt to read the number of elements given by
    //          <p elementCount> and described by <p elementType> and
    //          <p externalElementSize> from the data stream into the buffer
    //          at address <p elements>. The actual number of elements read
    //          is returned in <p elementsRead>.
  void readTypedElements(const OMType* elementType,
                         size_t externalElementSize,
                         OMByte* elements,
                         OMUInt32 elementCount,
                         OMUInt32& elementsRead) const;


    // @cmember Attempt to write the number of elements given by
    //          <p elementCount> and described by <p elementType> and
    //          <p internalElementSize> to the data stream from the buffer
    //          at address <p elements>. The actual number of elements written
    //          is returned in <p elementsWritten>.
  void writeTypedElements(const OMType* elementType,
                          size_t internalElementSize,
                          const OMByte* elements,
                          OMUInt32 elementCount,
                          OMUInt32& elementsWritten);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMDataStreamProperty>. The size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMDataStreamProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMDataStreamProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Is a byte order specifed for this stream ?
  virtual bool hasByteOrder(void) const;

    // @cmember Specify a byte order for this stream.
  virtual void setByteOrder(OMByteOrder byteOrder);

    // @cmember The byte order of this stream.
  virtual OMByteOrder byteOrder(void) const;

    // @cmember Clear the byte order of this stream
  virtual void clearByteOrder(void);

  virtual OMByteOrder storedByteOrder(void) const;

  virtual void setStoredByteOrder(OMByteOrder byteOrder);

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

protected:

  virtual const wchar_t* storedName(void) const;

private:

  OMStoredStream* stream(void) const;

  void open(void);
  void create(void);

  OMStoredStream* _stream;
  bool _exists; // true means an accessible persisted representation exists
  OMByteOrder _byteOrder;

    // OMDataStreamProperty can't be assigned - declare but don't define
  OMDataStreamProperty& operator = (const OMDataStreamProperty& rhs);

    // OMDataStreamProperty can't be copied - declare but don't define
  OMDataStreamProperty(const OMDataStreamProperty& rhs);

};

#endif
