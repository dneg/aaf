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

// @doc OMINTERNAL
#ifndef OMSETITERATOR_H
#define OMSETITERATOR_H

#include "OMContainerIterator.h"
#include "OMRedBlackTreeIterator.h"

template <typename Key, typename Element>
class OMSet;

  // @class Iterators over <c OMSet>s.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @base public | <c OMContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Key, typename Element>
class OMSetIterator : public OMContainerIterator<Element> {
public:
  // @access Public members.

    // @cmember Create an <c OMSetIterator> over the given
    //          <c OMSet> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the forward direction
    //          (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the reverse direction
    //          (decreasing <p Key>s).
  OMSetIterator(const OMSet<Key, Element>& set,
                OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMSetIterator>.
  virtual ~OMSetIterator(void);

    // @cmember Reset this <c OMSetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the forward direction
    //          (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the reverse direction
    //          (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMSetIterator> positioned before the first
    //          <p Element> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMSetIterator> positioned after the last
    //          <p Element> ?
  virtual bool after(void) const;

    // @cmember The number of <p Elements>s in the associated
    //          <c OMSet>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMSetIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMSet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMSetIterator::valid> becomes <e bool.true> and
    //          <mf OMSetIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMSet> is reached
    //          then the result is <e bool.false>,
    //          <mf OMSetIterator::valid> becomes <e bool.false> and
    //          <mf OMSetIterator::after> becomes <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMSetIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMSet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMSetIterator::valid> becomes <e bool.true> and
    //          <mf OMSetIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMSet> is
    //          reached then the result is <e bool.false>,
    //          <mf OMSetIterator::valid> becomes <e bool.false> and
    //          <mf OMSetIterator::before> becomes <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p Element> in the associated <c OMSet>
    //          at the position currently designated by this
    //          <c OMSetIterator>.
  virtual Element& value(void) const;

    // @cmember Set the <p Element> in the associated <c OMSet> at the
    //          position currently designated by this
    //          <c OMSetIterator> to <p newElement>. The previous
    //          <p Element> is returned. To preserve the ordering of
    //          <p Key>s, the <p Key> of <p newElement> must be the same as
    //          that of the existing <p Element>.
  virtual Element setValue(const Key k, Element newElement);

    // @cmember Return the <p Key> of the <p Element> in the associated
    //          <c OMSet> at the position currently designated by this
    //          <c OMSetIterator>.
  Key key(void) const;

private:

  OMRedBlackTreeIterator<Key, Element> _iterator;

};

#include "OMSetIteratorT.h"

#endif

