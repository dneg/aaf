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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMArrayProperty
#ifndef OMARRAYPROPERTYT_H
#define OMARRAYPROPERTYT_H

#include "OMArrayPropertyIterator.h"

template <typename Element>
OMArrayProperty<Element>::OMArrayProperty(const OMPropertyId propertyId,
                                          const wchar_t* name)
: OMDataVector(propertyId, SF_DATA_VECTOR, name)
{
}

template <typename Element>
OMArrayProperty<Element>::~OMArrayProperty(void)
{
}

  // @mfunc Get the value of this <c OMArrayProperty>.
  //   @tcarg class | Element | The type of the property. This
  //          can be any type.
  //   @parm A pointer to a <p Element>
  //   @parm The size of the <p value>
  //   @this const
template <typename Element>
void OMArrayProperty<Element>::getValue(Element* value,
                                        size_t valueSize) const
{
  TRACE("OMArrayProperty<Element>::getValue");
  PRECONDITION("Buffer large enough", valueSize >= size());

  size_t elementCount = valueSize / elementSize();
  for (size_t i = 0; i < elementCount; i++) {
    value[i] = getAt(i);
  }
}

  // @mfunc Set the value of this <c OMArrayProperty>. The
  //        value is set by copying <p valueSize> bytes from the
  //        address <p value> into the <c OMArrayProperty>.
  //   @tcarg class | Element | The type of the property. This
  //          can be any type.
  //   @parm A pointer to an array of <p Element>s
  //   @parm The size of the array <p value> in bytes
template <typename Element>
void OMArrayProperty<Element>::setValue(const Element* value,
                                        size_t valueSize)
{
  TRACE("OMArrayProperty<Element>::setValue");

  size_t elementCount = valueSize / elementSize();
  if (elementCount > 0) {
    setElementValues(value, elementCount);
  }
}

  // @mfunc Set the value of this <c OMArrayProperty>. The
  //        value is set by copying <p elementCount> elements from the
  //        address <p value> into the <c OMArrayProperty>.
  //   @tcarg class | Element | The array element type.
template <typename Element>
void OMArrayProperty<Element>::setElementValues(const Element* value,
                                                size_t elementCount)
{
  TRACE("OMArrayProperty<Element>::setElementValues");
  PRECONDITION("Valid data buffer", value != 0);
  PRECONDITION("Valid count", elementCount > 0);

  _vector.clear();
  _vector.grow(elementCount);

  for (size_t i = 0; i < elementCount; i++) {
    appendValue(&value[i]);
  }
}

  // @mfunc Get the value of the item at position <p index> in this
  //        <c OMArrayProperty>. The value is obtained by copying
  //        a single item of type Element from this
  //        <c OMArrayProperty> at position <p index>.
  //   @tcarg class | Element | The array element type.
  //   @parm The location that is to receive the value copied out of
  //         this <c OMArrayProperty>.
  //   @parm The index of the value to get.
  //   @this const
template <typename Element>
void OMArrayProperty<Element>::getValueAt(Element* value,
                                          const size_t index) const
{
  TRACE("OMArrayProperty<Element>::getValueAt");
  PRECONDITION("Valid value", value != 0);

  *value = _vector.getAt(index);;
}

template <typename Element>
Element& OMArrayProperty<Element>::getAt(const size_t index) const
{
  TRACE("OMArrayProperty<Element>::getAt");

  return _vector.getAt(index);
}

  // @mfunc Set the value of the item at position <p index> in this
  //        <c OMArrayProperty>. The value is set by copying
  //        a single item of type Element into this
  //        <c OMArrayProperty> at position <p index>.
  //   @tcarg class | Element | The array element type.
  //   @parm A pointer to the new value that is to be copied into
  //         this <c OMArrayProperty>.
  //   @parm The index of the value to set.
template <typename Element>
void OMArrayProperty<Element>::setValueAt(const Element* value,
                                          const size_t index)
{
  TRACE("OMArrayProperty<Element>::setValueAt");
  PRECONDITION("Valid value", value != 0);

  setAt(*value, index);
}

template <typename Element>
void OMArrayProperty<Element>::setAt(const Element value,
                                     const size_t index)
{
  TRACE("OMArrayProperty<Element>::setAt");

  _vector.setAt(value, index);
  setPresent();
}

  // @mfunc Set the value of the item at the last position in this
  //        <c OMArrayProperty>. The <c OMArrayProperty>
  //        is first increased in size by one item. The value is then
  //        set by copying a single item of type Element into this
  //        <c OMArrayProperty> at the last position.
  //   @tcarg class | Element | The array element type.
  //   @parm A pointer to the new value that is to be copied into
  //         this <c OMArrayProperty>.
template <typename Element>
void OMArrayProperty<Element>::appendValue(const Element* value)
{
  TRACE("OMArrayProperty<Element>::appendValue");
  PRECONDITION("Valid value", value != 0);

  _vector.append(*value);
  setPresent();
}

template <typename Element>
void OMArrayProperty<Element>::append(const Element& value)
{
  TRACE("OMArrayProperty<Element>::append");

  _vector.append(value);
  setPresent();
}

  // @mfunc Set the value of the item at the first position in this
  //        <c OMArrayProperty>. The <c OMArrayProperty>
  //        is first increased in size by one item and all existing items
  //        are moved up by on position. The value is then
  //        set by copying a single item of type Element into this
  //        <c OMArrayProperty> at the first position.
  //   @tcarg class | Element | The array element type.
  //   @parm A pointer to the new value that is to be copied into
  //         this <c OMArrayProperty>.
template <typename Element>
void OMArrayProperty<Element>::prependValue(const Element* value)
{
  TRACE("OMArrayProperty<Element>::prependValue");
  PRECONDITION("Valid value", value != 0);

  _vector.prepend(*value);
  setPresent();
}

  // @mfunc Get the value of this <c OMArrayProperty>.  The
  //        value is obtained by copying the value from the
  //        <c OMArrayProperty>. The buffer is at address
  //        <p buffer> and is <p bufferSize> bytes in size.
  //        Copying only takes place if the buffer is large enough.
  //   @tcarg class | Element | The type of the property. This
  //          can be any type.
  //   @parm A pointer to the buffer.
  //   @parm The size of the buffer.
  //   @rdesc <e bool.true> if the value was successfully copied <e
  //          bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMArrayProperty<Element>::copyToBuffer(Element* buffer,
                                            size_t bufferSize) const
{
  TRACE("OMArrayProperty<Element>::copyToBuffer");
  PRECONDITION("Valid buffer", buffer != 0);

  bool result = false;
  if (bufferSize >= bitsSize()) {
    OMVectorIterator<Element> iterator(_vector, OMBefore);
    while (++iterator) {
      *buffer++ = iterator.value();
    }
    result = true;
  }
  return result;
}

  // @mfunc Get the value of this <c OMArrayProperty>.  The
  //        value is obtained by copying the value from the
  //        <c OMArrayProperty>. The buffer is at address
  //        <p buffer> and is <p elementCount> elements in size.
  //        Copying only takes place if the buffer is large enough.
  //   @tcarg class | Element | The array element type.
  //   @parm A pointer to the buffer.
  //   @parm The number of elements in the buffer.
  //   @rdesc <e bool.true> if the value was successfully copied <e
  //          bool.false> otherwise.
  //   @this const
template <typename Element>
bool
OMArrayProperty<Element>::copyElementsToBuffer(Element* buffer,
                                               size_t elementCount) const
{
  TRACE("OMArrayProperty<Element>::copyElementsToBuffer");

  PRECONDITION("Valid buffer", buffer != 0);
  PRECONDITION("Valid count", elementCount > 0);

  return copyToBuffer(buffer, elementCount * sizeof(Element));
}

  // @mfunc The number of items in this this <c OMArrayProperty>.
  //   @tcarg class | Element | The array element type.
  //   @rdesc The number of items in this <c OMArrayProperty>.
  //   @this const
template <typename Element>
size_t OMArrayProperty<Element>::count(void) const
{
  TRACE("OMArrayProperty<Element>::count");

  return _vector.count();
}

template <typename Element>
void OMArrayProperty<Element>::appendValue(void* value)
{
  TRACE("OMArrayProperty<Element>::appendValue");
  PRECONDITION("Valid value", value != 0);

  appendValue(static_cast<Element*>(value));
}

template <typename Element>
void  OMArrayProperty<Element>::clear(void)
{
  TRACE("OMArrayProperty<Element>::clear");

  _vector.clear();
}

template <typename Element>
size_t OMArrayProperty<Element>::elementSize(void) const
{
  TRACE("OMArrayProperty<Element>::elementSize");
  return sizeof(Element);
}

template <typename Element>
OMDataContainerIterator*
OMArrayProperty<Element>::createIterator(void) const
{
  TRACE("OMArrayProperty<Element>::createIterator");

  OMDataContainerIterator* result = new OMArrayPropertyIterator<Element>(
                                                                     *this,
                                                                     OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

template <typename Element>
size_t OMArrayProperty<Element>::size(void) const
{
  TRACE("OMArrayProperty<Element>::size");

  size_t result = count() * elementSize();
  return result;
}

template <typename Element>
OMUInt64 OMArrayProperty<Element>::objectCount(void) const
{
  TRACE("OMArrayProperty<Element>::objectCount");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

template <typename Element>
size_t OMArrayProperty<Element>::bitsSize(void) const
{
  TRACE("OMArrayProperty<Element>::bitsSize");

  size_t result = count() * elementSize();
  return result;
}

template <typename Element>
void OMArrayProperty<Element>::getBits(OMByte* bits, size_t ANAME(size)) const
{
  TRACE("OMArrayProperty<Element>::getBits");

  PRECONDITION("Valid buffer", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  Element* buffer = reinterpret_cast<Element*>(bits);

  OMArrayPropertyIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    *buffer++ = iterator.value();
  }
}

template <typename Element>
void OMArrayProperty<Element>::setBits(const OMByte* bits,
                                       size_t size)
{
  TRACE("OMArrayProperty<Element>::setBits");
  PRECONDITION("Valid bits", bits != 0);

  size_t elementCount = size / elementSize();
  if (elementCount > 0) {
    setElementValues(reinterpret_cast<const Element*>(bits), elementCount);
  }
}

template <typename Element>
void
OMArrayProperty<Element>::shallowCopyTo(OMProperty* destination) const
{
  TRACE("OMArrayProperty<Element>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMArrayProperty<Element> Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  dest->clear();
  OMArrayPropertyIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    dest->appendValue(&iterator.value());
  }
}

template <typename Element>
void OMArrayProperty<Element>::deepCopyTo(OMProperty* /* destination */,
                                          void* /* clientContext */) const
{
  TRACE("OMArrayProperty<Element>::deepCopyTo");
  // Nothing to do - this is a deep copy
}

#endif
