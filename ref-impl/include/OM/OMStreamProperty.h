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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMSTREAMPROPERTY_H
#define OMSTREAMPROPERTY_H

#include "OMDataTypes.h"

#include "OMDataStreamProperty.h"

  // @class Persistent typed data stream properties supported by the
  //        Object Manager.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @base | public <c OMDataStreamProperty>.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMStreamProperty : public OMDataStreamProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStreamProperty(const OMPropertyId propertyId, const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStreamProperty(void);

    // All reads/and writes advance the current element

  // Random access in chunks of Elements

    // @cmember Read <p elementCount> <p Element>s, starting at <p index>,
    //          from this <c OMStreamProperty> into <p elements>.
  void readElements(OMUInt64 index,
                    OMUInt32 elementCount,
                    Element* elements) const;

    // @cmember Write <p elementCount> <p Element>s, starting at <p index>,
    //          to this <c OMStreamProperty> from <p elements>.
  void writeElements(OMUInt64 index,
                     OMUInt32 elementCount,
                     const Element* elements);

  // Random access one Element at a time.

    // @cmember Read a single <p Element>, at <p index>,
    //          from this <c OMStreamProperty> into <p element>.
  void readElement(OMUInt64 index,
                   Element* element) const;

    // @cmember Write a single <p Element>s, at <p index>,
    //          to this <c OMStreamProperty> from <p element>.
  void writeElement(OMUInt64 index,
                    const Element* element);

  // Sequential access in chunks of Elements

    // @cmember Read <p elementCount> <p Element>s from the current position in
    //          this <c OMStreamProperty> into <p elements>.
  void readElements(OMUInt32 elementCount,
                    Element* elements) const;

    // @cmember Write <p elementCount> <p Element>s to the current position in
    //          this <c OMStreamProperty> from <p elements>.
  void writeElements(OMUInt32 elementCount,
                     const Element* elements);

  // Sequential access one Element at a time.

    // @cmember Read a single <p Element> from the current position in
    //          this <c OMStreamProperty> into <p element>.
  void readElement(Element* element) const;

    // @cmember Write a single <p Element> to the current position in
    //          this <c OMStreamProperty> from <p element>.
  void writeElement(const Element* element);

  // Extend in chunks of Elements

    // @cmember Write <p elementCount> <p Element>s to the end of this
    //          <c OMStreamProperty> from <p elements>.
  void appendElements(OMUInt32 elementCount,
                      const Element* elements);

  // Extend one Element at a time

    // @cmember Write a single <p Element> to the end of
    //          this <c OMStreamProperty>.
  void appendElement(const Element* element);

  // Stream positioning and count of elements

    // @cmember The index of the current <p Element>.
  OMUInt64 index(void) const;

    // @cmember Make the <p Element> at <p newIndex> the current one.
  void setIndex(const OMUInt64 newIndex) const;

    // @cmember The count of <p Element>s in this <c OMStreamProperty>.
  OMUInt64 elementCount(void) const;

    // @cmember Set the count of <p Element>s in this <c OMStreamProperty>
    //          to <p newElementCount>.
  void setElementCount(OMUInt64 newElementCount);

};

#include "OMStreamPropertyT.h"

#endif
