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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMIdentitySetIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMIdentitySetIterator> over the given
  //        <c OMIdentitySet> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMIdentitySetIterator> is made ready to traverse the
  //        associated <c OMIdentitySet> in the forward direction
  //        (increasing <p Value>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMIdentitySetIterator> is made ready to traverse the
  //        associated <c OMIdentitySet> in the reverse direction
  //        (decreasing <p Value>s).
  //   @tcarg class | Element | The type of the unique elements.
  //   @parm The <c OMIdentitySet> over which to iterate.
  //   @parm The initial position for this <c OMIdentitySetIterator>.
template <typename Element>
OMIdentitySetIterator<Element>::OMIdentitySetIterator(
                                            const OMIdentitySet<Element>& set,
                                            OMIteratorPosition initialPosition)
  : _iterator(set._tree, initialPosition)
{
  TRACE("OMIdentitySetIterator<Element>::OMIdentitySetIterator");
}

  // @mfunc Destroy this <c OMIdentitySetIterator>.
  //   @tcarg class | Element | The type of the unique elements.
template <typename Element>
OMIdentitySetIterator<Element>::~OMIdentitySetIterator(void)
{
  TRACE("OMIdentitySetIterator<Element>::~OMIdentitySetIterator");
}

  // @mfunc Reset this <c OMIdentitySetIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMIdentitySetIterator> is made ready to traverse the
  //        associated <c OMIdentitySet> in the forward direction
  //        (increasing <p Value>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMIdentitySetIterator> is made ready to traverse the
  //        associated <c OMIdentitySet> in the reverse direction
  //        (decreasing <p Value>s).
  //   @tcarg class | Element | The type of the unique elements.
  //   @parm The position to which this <c OMIdentitySetIterator>
  //         should be reset.
template <typename Element>
void OMIdentitySetIterator<Element>::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMIdentitySetIterator<Element>::reset");

  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMIdentitySetIterator> positioned before the first
  //        <p Element> ?
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc <e bool.true> if this <c OMIdentitySetIterator>
  //          is positioned before
  //          the first <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMIdentitySetIterator<Element>::before(void) const
{
  TRACE("OMIdentitySetIterator<Element>::before");

  return _iterator.before();
}

  // @mfunc Is this <c OMIdentitySetIterator> positioned after the last
  //        <p Element> ?
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc <e bool.true> if this <c OMIdentitySetIterator>
  //          is positioned after
  //          the last <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMIdentitySetIterator<Element>::after(void) const
{
  TRACE("OMIdentitySetIterator<Element>::after");

  return _iterator.after();
}

  // @mfunc Is this <c OMIdentitySetIterator> validly positioned on
  //        an <p Element> ?
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc <e bool.true> if this <c OMIdentitySetIterator>
  //          is validly positioned on an <p Element>,
  //          <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMIdentitySetIterator<Element>::valid(void) const
{
  TRACE("OMIdentitySetIterator<Element>::valid");

  return _iterator.valid();
}

  // @mfunc The number of <p Elements>s in the associated
  //        <c OMIdentitySet>.
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc The number of <p Elements>s
  //   @this const
template <typename Element>
OMUInt32 OMIdentitySetIterator<Element>::count(void) const
{
  TRACE("OMIdentitySetIterator<Element>::count");

  return _iterator.count();
}

  // @mfunc Advance this <c OMIdentitySetIterator> to the next <p Element>,
  //        if any.
  //        If the end of the associated <c OMIdentitySet> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMIdentitySetIterator::valid> becomes <e bool.true> and
  //        <mf OMIdentitySetIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMIdentitySet> is reached
  //        then the result is <e bool.false>,
  //        <mf OMIdentitySetIterator::valid> becomes <e bool.false> and
  //        <mf OMIdentitySetIterator::after> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc <e bool.false> if this <c OMIdentitySetIterator> has passed
  //          the last <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMIdentitySetIterator<Element>::operator++()
{
  TRACE("OMIdentitySetIterator<Element>::operator++()");

  PRECONDITION("Valid iterator", _iterator.before() || _iterator.valid());

  return ++_iterator;
}

  // @mfunc Retreat this <c OMIdentitySetIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated <c OMIdentitySet> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMIdentitySetIterator::valid> becomes <e bool.true> and
  //        <mf OMIdentitySetIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMIdentitySet> is
  //        reached then the result is <e bool.false>,
  //        <mf OMIdentitySetIterator::valid> becomes <e bool.false> and
  //        <mf OMIdentitySetIterator::before> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc <e bool.false> if this <c OMIdentitySetIterator> has passed
  //          the first <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMIdentitySetIterator<Element>::operator--()
{
  TRACE("OMIdentitySetIterator<Element>::operator--()");

  PRECONDITION("Valid iterator", _iterator.after() || _iterator.valid());

  return --_iterator;
}

  // @mfunc Return the <p Element> in the associated <c OMIdentitySet>
  //        at the position currently designated by this
  //        <c OMIdentitySetIterator>.
  //   @tcarg class | Element | The type of the unique elements.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Element>
const Element& OMIdentitySetIterator<Element>::value(void) const
{
  TRACE("OMIdentitySetIterator<Element>::value");

  PRECONDITION("Valid iterator", _iterator.valid());

  return _iterator.key();
}
