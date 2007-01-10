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

// @doc OMINTERNAL
#ifndef OMDATASTREAMPROPERTYFILTER_H
#define OMDATASTREAMPROPERTYFILTER_H

#include "OMDataTypes.h"

class OMDataStreamProperty;
class OMStoredStreamFilter;
class OMType;

  // @class Filtered data stream property.
class OMDataStreamPropertyFilter {
public:
  // @access Public members.

    // @cmember Constructor.
  OMDataStreamPropertyFilter(OMDataStreamProperty* property);

    // @cmember Destructor.
  virtual ~OMDataStreamPropertyFilter(void);

    // @cmember The size, in bytes, of the data in this
    //          <c OMDataStreamPropertyFilterProperty>.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size, in bytes, of the data in this
    //          <c OMDataStreamPropertyFilterProperty>.
  virtual void setSize(const OMUInt64 newSize);

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the begining of the data stream.
  virtual void setPosition(const OMUInt64 offset) const;

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
  virtual void readTypedElements(const OMType* elementType,
                                 OMUInt32 externalElementSize,
                                 OMByte* elements,
                                 OMUInt32 elementCount,
                                 OMUInt32& elementsRead) const;


    // @cmember Attempt to write the number of elements given by
    //          <p elementCount> and described by <p elementType> and
    //          <p internalElementSize> to the data stream from the buffer
    //          at address <p elements>. The actual number of elements written
    //          is returned in <p elementsWritten>.
  virtual void writeTypedElements(const OMType* elementType,
                                  OMUInt32 internalElementSize,
                                  const OMByte* elements,
                                  OMUInt32 elementCount,
                                  OMUInt32& elementsWritten);

private:
  // @access Private members.

  OMDataStreamProperty* _property;
  OMStoredStreamFilter* _streamFilter;
};

#endif
