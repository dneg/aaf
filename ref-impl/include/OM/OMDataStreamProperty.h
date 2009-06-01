//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMDATASTREAMPROPERTY_H
#define OMDATASTREAMPROPERTY_H

#include "OMDataTypes.h"

#include "OMDataStream.h"

class OMStoredStream;
class OMDataStreamAccess;
class OMDataStreamPropertyFilter;

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
  virtual void detach(void);

    // @cmember Save this <c OMDataStreamProperty>.
  virtual void save(void) const;

    // @cmember The number of objects contained within this
    //          <c OMDataStreamProperty> if any.
  virtual OMUInt64 objectCount(void) const;

    // @cmember Restore this <c OMDataStreamProperty>, the size of the
    //          <c OMDataStreamProperty> is <p externalSize>.
  virtual void restore(OMPropertySize externalSize);

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
  virtual OMUInt64 position(void) const;

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
                         OMUInt32 externalElementSize,
                         OMByte* elements,
                         OMUInt32 elementCount,
                         OMUInt32& elementsRead) const;


    // @cmember Attempt to write the number of elements given by
    //          <p elementCount> and described by <p elementType> and
    //          <p internalElementSize> to the data stream from the buffer
    //          at address <p elements>. The actual number of elements written
    //          is returned in <p elementsWritten>.
  void writeTypedElements(const OMType* elementType,
                          OMUInt32 internalElementSize,
                          const OMByte* elements,
                          OMUInt32 elementCount,
                          OMUInt32& elementsWritten);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMDataStreamProperty>. The size is given in bytes.
  virtual OMUInt32 bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMDataStreamProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, OMUInt32 size) const;

    // @cmember Set the raw bits of this <c OMDataStreamProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, OMUInt32 size);

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

  // Deferred access.

  void setStreamAccess(OMDataStreamAccess* streamAccess);

  void clearStreamAccess(void);

  bool hasStreamAccess(void) const;

  OMDataStreamAccess* streamAccess(void) const;

  OMStoredStream* stream(void) const; // tjb should be private


  // Stream essence element key

    // @cmember Does this stream know about essence element keys?
  virtual bool hasEssenceElementKey(void) const;

    // @cmember The essence element key associated with this stream.
    //          @precondition <f hasEssenceElementKey()>
  virtual OMKLVKey essenceElementKey(void) const;

    // @cmember Specify the essence element key for this stream.
    //          @precondition <f hasEssenceElementKey()>
  virtual void setEssenceElementKey(const OMKLVKey& key);


  // Stream filtering

  virtual OMDataStreamPropertyFilter* createFilter();


protected:

  virtual const wchar_t* storedName(void) const;

private:

  void open(void);
  void create(void);

  OMStoredStream* _stream;
  bool _exists; // true means an accessible persisted representation exists
  OMByteOrder _byteOrder;

    // OMDataStreamProperty can't be assigned - declare but don't define
  OMDataStreamProperty& operator = (const OMDataStreamProperty& rhs);

    // OMDataStreamProperty can't be copied - declare but don't define
  OMDataStreamProperty(const OMDataStreamProperty& rhs);

  OMDataStreamAccess* _streamAccess;

};

#endif
