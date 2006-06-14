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

// @doc OMINTERNAL
#ifndef OMIDENTITYSETITERATOR_H
#define OMIDENTITYSETITERATOR_H

#include "OMContainerIterator.h"
#include "OMRedBlackTreeIterator.h"

template <typename Element>
class OMIdentitySet;

  // @class Iterators over <c OMIdentitySet>s.
  //   @tcarg class | Element | The type of the unique elements.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMIdentitySetIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMIdentitySetIterator> over the given
    //          <c OMIdentitySet> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMIdentitySetIterator> is made ready to traverse the
    //          associated <c OMIdentitySet> in the forward direction
    //          (increasing <p Element>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMIdentitySetIterator> is made ready to traverse the
    //          associated <c OMIdentitySet> in the reverse direction
    //          (decreasing <p Element>s).
  OMIdentitySetIterator(const OMIdentitySet<Element>& set,
                        OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMIdentitySetIterator>.
  virtual ~OMIdentitySetIterator(void);

    // @cmember Reset this <c OMIdentitySetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMIdentitySetIterator> is made ready to traverse the
    //          associated <c OMIdentitySet> in the forward direction
    //          (increasing <p Element>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMIdentitySetIterator> is made ready to traverse the
    //          associated <c OMIdentitySet> in the reverse direction
    //          (decreasing <p Element>s).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMIdentitySetIterator> positioned before the first
    //          <p Element> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMIdentitySetIterator> positioned after the last
    //          <p Element> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMIdentitySetIterator> validly positioned on
    //          an <p Element> ?
  virtual bool valid(void) const;

    // @cmember The number of <p Elements>s in the associated
    //          <c OMIdentitySet>.
  virtual OMUInt32 count(void) const;

    // @cmember Advance this <c OMIdentitySetIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMIdentitySet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMIdentitySetIterator::valid> becomes <e bool.true> and
    //          <mf OMIdentitySetIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMIdentitySet> is reached
    //          then the result is <e bool.false>,
    //          <mf OMIdentitySetIterator::valid> becomes <e bool.false> and
    //          <mf OMIdentitySetIterator::after> becomes <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMIdentitySetIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMIdentitySet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMIdentitySetIterator::valid> becomes <e bool.true> and
    //          <mf OMIdentitySetIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMIdentitySet> is
    //          reached then the result is <e bool.false>,
    //          <mf OMIdentitySetIterator::valid> becomes <e bool.false> and
    //          <mf OMIdentitySetIterator::before> becomes <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p Element> in the associated <c OMIdentitySet>
    //          at the position currently designated by this
    //          <c OMIdentitySetIterator>.
  virtual const Element& value(void) const;

private:

  OMRedBlackTreeIterator<Element, int> _iterator;

};

#include "OMIdentitySetIterT.h"

#endif

