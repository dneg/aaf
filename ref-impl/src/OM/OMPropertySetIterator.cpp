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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMPropertySetIterator

#include "OMPropertySetIterator.h"
#include "OMPropertySet.h"

#include "OMAssertions.h"


  // @mfunc Create an <c OMPropertySetIterator> over the given
  //        <c OMPropertySet> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMPropertySetIterator> is made ready to traverse the
  //        associated <c OMPropertySet> in the forward direction
  //        (increasing <p OMPropertyId>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMPropertySetIterator> is made ready to traverse the
  //        associated <c OMPropertySet> in the reverse direction
  //        (decreasing <p OMPropertyId>s).
  //   @parm The <c OMPropertySet> over which to iterate.
  //   @parm The initial position for this <c OMPropertySetIterator>.
OMPropertySetIterator::OMPropertySetIterator(
                                            const OMPropertySet& set,
                                            OMIteratorPosition initialPosition)
  : _iterator(set._set, initialPosition)
{
  TRACE("OMPropertySetIterator::OMPropertySetIterator");
}

  // @mfunc Destroy this <c OMPropertySetIterator>.
OMPropertySetIterator::~OMPropertySetIterator(void)
{
  TRACE("OMPropertySetIterator::~OMPropertySetIterator");
}

  // @mfunc Reset this <c OMPropertySetIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMPropertySetIterator> is made ready to traverse the
  //        associated <c OMPropertySet> in the forward direction
  //        (increasing <p OMPropertyId>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMPropertySetIterator> is made ready to traverse the
  //        associated <c OMPropertySet> in the reverse direction
  //        (decreasing <p OMPropertyId>s).
  //   @parm The position to which this <c OMPropertySetIterator>
  //         should be reset.
void OMPropertySetIterator::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMPropertySetIterator::reset");

  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMPropertySetIterator> positioned before the first
  //        <c OMProperty> ?
  //   @rdesc <e bool.true> if this <c OMPropertySetIterator> is positioned
  //          before the first <c OMProperty>, <e bool.false> otherwise.
  //   @this const
bool OMPropertySetIterator::before(void) const
{
  TRACE("OMPropertySetIterator::before");

  return _iterator.before();
}

  // @mfunc Is this <c OMPropertySetIterator> positioned after the last
  //        <c OMProperty> ?
  //   @rdesc <e bool.true> if this <c OMPropertySetIterator> is positioned
  //          after the last <c OMProperty>, <e bool.false> otherwise.
  //   @this const
bool OMPropertySetIterator::after(void) const
{
  TRACE("OMPropertySetIterator::after");

  return _iterator.after();
}

  // @mfunc Is this <c OMPropertySetIterator> validly positioned on
  //        an <c OMProperty> ?
  //   @rdesc <e bool.true> if this <c OMPropertySetIterator> is validly
  //          positioned on an <c OMProperty>, <e bool.false> otherwise.
  //   @this const
bool OMPropertySetIterator::valid(void) const
{
  TRACE("OMPropertySetIterator::valid");

  return _iterator.valid();
}

  // @mfunc The number of <c OMProperty>s in the associated
  //        <c OMPropertySet>.
  //   @rdesc The number of <c OMProperty>s
  //   @this const
size_t OMPropertySetIterator::count(void) const
{
  TRACE("OMPropertySetIterator::count");

  return _iterator.count();
}

  // @mfunc Advance this <c OMPropertySetIterator> to the next
  //        <c OMProperty>, if any.
  //        If the end of the associated <c OMPropertySet> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMPropertySetIterator::valid> becomes <e bool.true> and
  //        <mf OMPropertySetIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMPropertySet> is reached
  //        then the result is <e bool.false>,
  //        <mf OMPropertySetIterator::valid> becomes <e bool.false> and
  //        <mf OMPropertySetIterator::after> becomes <e bool.true>.
  //   @rdesc <e bool.false> if this <c OMPropertySetIterator> has passed
  //          the last <c OMProperty>, <e bool.true> otherwise.
bool OMPropertySetIterator::operator++()
{
  TRACE("OMPropertySetIterator::operator++()");

  PRECONDITION("Valid iterator", _iterator.before() || _iterator.valid());

  return ++_iterator;
}

  // @mfunc Retreat this <c OMPropertySetIterator> to the previous
  //        <c OMProperty>, if any.
  //        If the beginning of the associated <c OMPropertySet> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMPropertySetIterator::valid> becomes <e bool.true> and
  //        <mf OMPropertySetIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMPropertySet> is
  //        reached then the result is <e bool.false>,
  //        <mf OMPropertySetIterator::valid> becomes <e bool.false> and
  //        <mf OMPropertySetIterator::before> becomes <e bool.true>.
  //   @rdesc <e bool.false> if this <c OMPropertySetIterator> has passed
  //          the first <c OMProperty>, <e bool.true> otherwise.
bool OMPropertySetIterator::operator--()
{
  TRACE("OMPropertySetIterator::operator--()");

  PRECONDITION("Valid iterator", _iterator.after() || _iterator.valid());

  return --_iterator;
}

  // @mfunc Return the <c OMProperty> in the associated <c OMPropertySet>
  //        at the position currently designated by this
  //        <c OMPropertySetIterator>.
  //   @rdesc The <c OMProperty> at the current position.
  //   @this const
OMProperty* OMPropertySetIterator::property(void) const
{
  TRACE("OMPropertySetIterator::property");

  PRECONDITION("Valid iterator", _iterator.valid());

  return _iterator.value();
}

  // @mfunc Return the <p OMPropertyId> of the <c OMProperty> in the
  //        associated <c OMPropertySet> at the position currently
  //        designated by this <c OMPropertySetIterator>.
  //   @rdesc The <p OMPropertyId> at the current position.
  //   @this const
OMPropertyId OMPropertySetIterator::propertyId(void) const
{
  TRACE("OMPropertySetIterator::propertyId");

  PRECONDITION("Valid iterator", _iterator.valid());

  return _iterator.key();
}

