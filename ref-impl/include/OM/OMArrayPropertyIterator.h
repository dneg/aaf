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

// @doc OMINTERNAL
#ifndef OMARRAYPROPERTYITERATOR_H
#define OMARRAYPROPERTYITERATOR_H

#include "OMDataContainerIterator.h"
#include "OMVectorIterator.h"

#include <stddef.h>

template <typename Element>
class OMArrayProperty;

  // @class Iterators over <c OMArrayProperty>s.
  //   @tcarg class | Element | The type of the contained elements.
  //   @base public | <c OMDataContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMArrayPropertyIterator : public OMDataContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMArrayPropertyIterator> over the given
    //          <c OMArrayProperty> <p vector> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMArrayPropertyIterator> is made ready to traverse the
    //          associated <c OMArrayProperty> in the forward direction
    //          (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMArrayPropertyIterator> is made ready to traverse the
    //          associated <c OMArrayProperty> in the reverse direction
    //          (decreasing indexes).
  OMArrayPropertyIterator(const OMArrayProperty<Element>& vector,
                   OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMArrayPropertyIterator>.
  virtual ~OMArrayPropertyIterator(void);

    // @cmember Create a copy of this <c OMArrayPropertyIterator>.
  virtual OMDataContainerIterator* copy(void) const;

    // @cmember Reset this <c OMArrayPropertyIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMArrayPropertyIterator> is made ready to traverse the
    //          associated <c OMArrayProperty> in the forward direction
    //          (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMArrayPropertyIterator> is made ready to traverse the
    //          associated <c OMArrayProperty> in the reverse direction
    //          (decreasing indexes).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMArrayPropertyIterator> positioned before the first
    //          <p Element> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMArrayPropertyIterator> positioned after the last
    //          <p Element> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMArrayPropertyIterator> validly
    //          positioned on a <p Element> ?
  virtual bool valid(void) const;

    // @cmember The number of <p Elements>s in the associated
    //          <c OMArrayProperty>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMArrayPropertyIterator> to the next
    //          <p Element>, if any.
    //          If the end of the associated <c OMArrayProperty> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMArrayPropertyIterator::valid> becomes <e bool.true> and
    //          <mf OMArrayPropertyIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMArrayProperty> is reached
    //          then the result is <e bool.false>,
    //          <mf OMArrayPropertyIterator::valid> becomes <e bool.false> and
    //          <mf OMArrayPropertyIterator::after> becomes <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMArrayPropertyIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMArrayProperty> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMArrayPropertyIterator::valid> becomes <e bool.true> and
    //          <mf OMArrayPropertyIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMArrayProperty> is
    //          reached then the result is <e bool.false>,
    //          <mf OMArrayPropertyIterator::valid> becomes <e bool.false> and
    //          <mf OMArrayPropertyIterator::before> becomes <e bool.true>.
  virtual bool operator--();

  virtual const OMByte* currentElement(void) const;

    // @cmember Return the <p Element> in the associated <c OMArrayProperty>
    //          at the position currently designated by this
    //          <c OMArrayPropertyIterator>.
  virtual Element& value(void) const;

    // @cmember Set the <p Element> in the associated <c OMArrayProperty>
    //          at the position currently designated by this
    //          <c OMArrayPropertyIterator> to <p newElement>. The previous
    //          <p Element> is returned.
  virtual Element setValue(Element newElement);

    // @cmember Return the index of the <p Element> in the
    //          associated <c OMArrayProperty> at the position
    //          currently designated by this <c OMArrayPropertyIterator>.
  virtual size_t index(void) const;

protected:

  typedef OMVectorIterator<Element> VectorIterator;

    // @cmember Create an <c OMArrayPropertyIterator> given
    //          an underlying <c OMVectorIterator>.
  OMArrayPropertyIterator(const VectorIterator& iter);

private:

  VectorIterator _iterator;

};

#include "OMArrayPropertyIteratorT.h"

#endif

