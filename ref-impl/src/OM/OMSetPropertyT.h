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
//         OMSetProperty
#ifndef OMSETPROPERTYT_H
#define OMSETPROPERTYT_H

#include "OMSetPropertyIterator.h"

template <typename Element>
OMSetProperty<Element>::OMSetProperty(const OMPropertyId propertyId,
                                      const wchar_t* name)
: OMDataSet(propertyId, SF_DATA_SET, name)
{
}

template <typename Element>
OMSetProperty<Element>::~OMSetProperty(void)
{
  TRACE("OMSetProperty<Element>::~OMSetProperty");
}

template <typename Element>
size_t OMSetProperty<Element>::count(void) const
{
  TRACE("OMSetProperty<Element>::count");
  return _set.count();
}

template <typename Element>
void OMSetProperty<Element>::insert(const Element& element)
{
  TRACE("OMSetProperty<Element>::insert");

  PRECONDITION("Element is not present", !contains(element));
  _set.insert(element);
  POSTCONDITION("Element is present", contains(element));
  setPresent(); // Mark property present
}

template <typename Element>
bool OMSetProperty<Element>::ensurePresent(const Element& element)
{
  TRACE("OMSetProperty<Element>::ensurePresent");

  bool present = contains(element);
  if (!present) {
    insert(element);
  }
  POSTCONDITION("Element is present", contains(element));
  return present;
}

template <typename Element>
void OMSetProperty<Element>::remove(const Element& element)
{
  TRACE("OMSetProperty<Element>::remove");
  PRECONDITION("Element is present", contains(element));
  _set.remove(element);
  POSTCONDITION("Element is not present", !contains(element));
}

template <typename Element>
bool OMSetProperty<Element>::ensureAbsent(const Element& element)
{
  TRACE("OMSetProperty<Element>::ensureAbsent");

  bool result = contains(element);
  if (result) {
    _set.remove(element);
  }
  POSTCONDITION("Element is not present", !contains(element));
  return result;
}

template <typename Element>
bool OMSetProperty<Element>::contains(const Element& element) const
{
  TRACE("OMSetProperty<Element>::contains");

  return _set.contains(element);
}

template <typename Element>
void OMSetProperty<Element>::clear(void)
{
  TRACE("OMSetProperty<Element>::clear");

  _set.clear();
  POSTCONDITION("Set is empty", count() == 0);
}

template <typename Element>
OMDataContainerIterator* OMSetProperty<Element>::createIterator(void) const
{
  TRACE("OMSetProperty<Element>::createIterator");
  OMDataContainerIterator* result =
                           new OMSetPropertyIterator<Element>(*this, OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

template <typename Element>
OMUInt64 OMSetProperty<Element>::objectCount(void) const
{
  TRACE("OMSetProperty<Element>::objectCount");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

template <typename Element>
size_t OMSetProperty<Element>::bitsSize(void) const
{
  TRACE("OMSetProperty<Element>::bitsSize");

  size_t result = count() * elementSize();
  return result;
}

template <typename Element>
void OMSetProperty<Element>::getBits(OMByte* bits, size_t ANAME(size)) const
{
  TRACE("OMSetProperty<Element>::getBits");

  PRECONDITION("Valid buffer", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  Element* buffer = reinterpret_cast<Element*>(bits);

  OMSetPropertyIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    *buffer++ = iterator.value();
  }
}

template <typename Element>
void OMSetProperty<Element>::setBits(const OMByte* /* bits */,
                                     size_t /* size */)
{
  TRACE("OMSetProperty<Element>::setBits");
  ASSERT("Unimplemented code not reached", false);
}

template <typename Element>
void OMSetProperty<Element>::shallowCopyTo(OMProperty* destination) const
{
  TRACE("OMSetProperty<Element>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMSetProperty<Element> Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  dest->clear();
  OMSetPropertyIterator<Element> iterator(*this, OMBefore);
  while (++iterator) {
    dest->insert(iterator.value());
  }
}

template <typename Element>
void OMSetProperty<Element>::deepCopyTo(OMProperty* /* destination */,
                                        void* /* clientContext */) const
{
  TRACE("OMSetProperty<Element>::deepCopyTo");
  // Nothing to do - this is a deep copy
}

template <typename Element>
void OMSetProperty<Element>::insert(void* value)
{
  TRACE("OMSetProperty<Element>::insert");
  insert(*static_cast<Element*>(value));
}

template <typename Element>
size_t OMSetProperty<Element>::elementSize(void) const
{
  TRACE("OMSetProperty<Element>::elementSize");

  return sizeof(Element);
}

#endif
