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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMARRAYPROPERTY_H
#define OMARRAYPROPERTY_H

#include "OMProperty.h"
#include "OMDataVector.h"
#include "OMVector.h"

template <typename Element>
class OMArrayPropertyIterator;

  // @class Persistent array properties supported by
  //        the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMArrayProperty : public OMDataVector {
public:

  OMArrayProperty(const OMPropertyId propertyId, const wchar_t* name);

  virtual ~OMArrayProperty(void);

    // @cmember Get the value of this <c OMArrayProperty>.
  void getValue(Element* value, OMPropertySize valueSize) const;

    // @cmember Set the value of this <c OMArrayProperty>. The
    //          value is set by copying <p valueSize> bytes from the
    //          address <p value> into the <c OMArrayProperty>.
  void setValue(const Element* value, OMPropertySize valueSize);

    // @cmember Set the value of this <c OMArrayProperty>. The
    //          value is set by copying <p elementCount> elements from the
    //          address <p value> into the <c OMArrayProperty>.
  void setElementValues(const Element* value, OMUInt32 elementCount);

    // @cmember Get the value of the item at position <p index> in this
    //          <c OMArrayProperty>. The value is obtained by copying
    //          a single item of type Element from this
    //          <c OMArrayProperty> at position <p index>.
  void getValueAt(Element* value, const OMUInt32 index) const;

    // @cmember Get the value of the <p Element> at
    //          position <p index> in this <c OMArrayProperty>.
  Element& getAt(const OMUInt32 index) const;

    // @cmember Set the value of the item at position <p index> in this
    //          <c OMArrayProperty>. The value is set by copying
    //          a single item of type Element into this
    //          <c OMArrayProperty> at position <p index>.
  void setValueAt(const Element* value, const OMUInt32 index);

   // @cmember Set the value of the <p Element> at
    //          position <p index> in this <c OMArrayProperty>.
    //          The existing <p Element> at <p index> is replaced.
  void setAt(const Element value, const OMUInt32 index);

    // @cmember Set the value of the item at the last position in this
    //          <c OMArrayProperty>. The <c OMArrayProperty>
    //          is first increased in size by one item. The value is then
    //          set by copying a single item of type Element into this
    //          <c OMArrayProperty> at the last position.
  void appendValue(const Element* value);

  void append(const Element& value);

    // @cmember Set the value of the item at the first position in this
    //          <c OMArrayProperty>. The <c OMArrayProperty>
    //          is first increased in size by one item and all existing items
    //          are moved up by on position. The value is then
    //          set by copying a single item of type Element into this
    //          <c OMArrayProperty> at the first position.
  void prependValue(const Element* value);

    // @cmember Get the value of this <c OMArrayProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMArrayProperty>. The buffer is at address
    //          <p buffer> and is <p bufferSize> bytes in size.
    //          Copying only takes place if the buffer is large enough.
  bool copyToBuffer(Element* buffer, OMUInt32 bufferSize) const;

    // @cmember Get the value of this <c OMArrayProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMArrayProperty>. The buffer is at address
    //          <p buffer> and is <p elementCount> elements in size.
    //          Copying only takes place if the buffer is large enough.
  bool copyElementsToBuffer(Element* buffer, OMUInt32 elementCount) const;

    // @cmember The number of items in this this <c OMArrayProperty>.
  virtual OMUInt32 count(void) const;

  virtual void appendValue(void* value);

  virtual void clear(void);

  virtual OMUInt32 elementSize(void) const;

  virtual OMDataContainerIterator* createIterator(void) const;

  virtual OMUInt32 size(void) const;

    // @cmember The number of objects contained within this
    //          <c OMSetProperty> if any.
  virtual OMUInt64 objectCount(void) const;

  // Direct property access interface

    // @cmember The size of the raw bits of this <c OMSetProperty>. The
    //          size is given in bytes.
  virtual OMUInt32 bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMSetProperty>. The raw bits
    //          are copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, OMUInt32 size) const;

    // @cmember Set the raw bits of this <c OMSetProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, OMUInt32 size);

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

private:

  OMVector<Element> _vector;

  friend class OMArrayPropertyIterator<Element>;

};

#include "OMArrayPropertyT.h"

#endif
