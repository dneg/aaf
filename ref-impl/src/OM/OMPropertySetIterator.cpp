/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMINTERNAL

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

