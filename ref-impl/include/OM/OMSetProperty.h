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

#ifndef OMSETPROPERTY_H
#define OMSETPROPERTY_H

#include "OMDataSet.h"

#include "OMIdentitySet.h"

class OMDataContainerIterator;
template <typename Element>
class OMSetPropertyIterator;

  // @class Persistent sets of data (i.e. non-object) items supported
  //        by the Object Manager.
  //        The items are unique. The items are not ordered. Duplicate
  //        items  are not allowed.
  //   @tcarg class | Element | The element type..
  //   @base public | <c OMDataSet>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMSetProperty : public OMDataSet {
public:

  OMSetProperty(const OMPropertyId propertyId, const wchar_t* name);

  virtual ~OMSetProperty(void);

  virtual size_t count(void) const;

  virtual void insert(const Element& element);

  virtual bool ensurePresent(const Element& element);

  virtual void remove(const Element& element);

  virtual bool ensureAbsent(const Element& element);

  virtual bool contains(const Element& element) const;

  virtual void clear(void);

  virtual OMDataContainerIterator* createIterator(void) const;

    // @cmember The number of objects contained within this
    //          <c OMSetProperty> if any.
  virtual OMUInt64 objectCount(void) const;

  // Direct property access interface

    // @cmember The size of the raw bits of this <c OMSetProperty>. The
    //          size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMSetProperty>. The raw bits
    //          are copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMSetProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

  virtual void insert(void* value);

  virtual size_t elementSize(void) const;

private:

  OMIdentitySet<Element> _set;

  friend class OMSetPropertyIterator<Element>;
};

#include "OMSetPropertyT.h"

#endif
