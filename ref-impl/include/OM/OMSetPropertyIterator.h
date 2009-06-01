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

#ifndef OMSETPROPERTYITERATOR_H
#define OMSETPROPERTYITERATOR_H

#include "OMDataContainerIterator.h"

#include "OMIdentitySetIter.h"
#include "OMSetProperty.h"

  // @class Iterators over <c OMSetProperty>s.
  //   @tcarg class | Element | The type of the contained elements.
  //   @base public | <c OMDataContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMSetPropertyIterator : public OMDataContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMSetPropertyIterator> over the given
    //          <c OMSetProperty> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMSetPropertyIterator> is made ready to traverse the
    //          associated <c OMSetProperty> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMSetPropertyIterator> is made ready to traverse the
    //          associated <c OMSetProperty> in the reverse
    //          direction (decreasing <p Key>s).
  OMSetPropertyIterator(const OMSetProperty<Element>& set,
                        OMIteratorPosition initialPosition = OMBefore);

    // @cmember Destroy this <c OMSetPropertyIterator>.
  virtual ~OMSetPropertyIterator(void);

    // @cmember Create a copy of this <c OMSetPropertyIterator>.
  virtual OMDataContainerIterator* copy(void) const;

    // @cmember Reset this <c OMSetPropertyIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMSetPropertyIterator> is made ready to traverse the
    //          associated <c OMSetProperty> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMSetPropertyIterator> is made ready to traverse the
    //          associated <c OMSetProperty> in the reverse
    //          direction (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMSetPropertyIterator> positioned before
    //          the first <p Element> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMSetPropertyIterator> positioned after
    //          the last <p Element> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMSetPropertyIterator> validly
    //          positioned on a <p Element> ?
  virtual bool valid(void) const;

    // @cmember The number of <p Element>s in the associated
    //          <c OMSetProperty>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMSetPropertyIterator> to the next
    //          <p Element>, if any.
    //          If the end of the associated
    //          <c OMSetProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMSetPropertyIterator::valid> becomes <e bool.true>
    //          and <mf OMSetPropertyIterator::after> becomes
    //          <e bool.false>.
    //          If the end of the associated
    //          <c OMSetProperty> is reached then the result
    //          is <e bool.false>, <mf OMSetPropertyIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMSetPropertyIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMSetPropertyIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated
    //          <c OMSetProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMSetPropertyIterator::valid> becomes <e bool.true>
    //          and <mf OMSetPropertyIterator::before> becomes
    //          <e bool.false>.
    //          If the beginning of the associated
    //          <c OMSetProperty> is reached then the result
    //          is <e bool.false>, <mf OMSetPropertyIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMSetPropertyIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p Element> in the associated
    //          <c OMSetProperty> at the position currently
    //          designated by this <c OMSetPropertyIterator>.
  virtual Element value(void) const;

  virtual const OMByte* currentElement(void) const;

protected:

  typedef OMIdentitySetIterator<Element> SetIterator;

    // @cmember Create an <c OMSetPropertyIterator> given
    //          an underlying <c OMSetIterator>.
  OMSetPropertyIterator(const SetIterator& iter);

private:

  SetIterator _iterator;

};

#include "OMSetPropertyIteratorT.h"

#endif
