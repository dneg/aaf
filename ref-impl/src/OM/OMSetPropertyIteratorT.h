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


// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMSetPropertyIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMSetPropertyIterator> over the given
  //        <c OMSetProperty> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMSetPropertyIterator> is made ready to traverse the
  //        associated <c OMSetProperty> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMSetPropertyIterator> is made ready to traverse the
  //        associated <c OMSetProperty> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | Element | The type of the contained items.
  //   @parm The <c OMStrongReferenceSet> over which to iterate.
  //   @parm The initial position for this <c OMSetPropertyIterator>.
template <typename Element>
OMSetPropertyIterator<Element>::OMSetPropertyIterator(
                                            const OMSetProperty<Element>& set,
                                            OMIteratorPosition initialPosition)
  : _iterator(set._set, initialPosition)
{
}

  // @mfunc Destroy this <c OMSetPropertyIterator>.
  //   @tcarg class | Element | The type of the contained items.
template <typename Element>
OMSetPropertyIterator<Element>::~OMSetPropertyIterator(void)
{
}

template <typename Element>
OMDataContainerIterator* OMSetPropertyIterator<Element>::copy(void) const
{
  TRACE("OMSetPropertyIterator<Element>::copy");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Reset this <c OMSetPropertyIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMSetPropertyIterator> is made ready to traverse the
  //        associated <c OMSetProperty> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMSetPropertyIterator> is made ready to traverse the
  //        associated <c OMSetProperty> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | Element | The type of the contained items.
  //   @parm The position to which this <c OMSetPropertyIterator>
  //         should be reset.
template <typename Element>
void
OMSetPropertyIterator<Element>::reset(OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMSetPropertyIterator> positioned before
  //        the first <p Element> ?
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc <e bool.true> if this <c OMSetPropertyIterator> is
  //          positioned before the first <p Element>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Element>
bool OMSetPropertyIterator<Element>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMSetPropertyIterator> positioned after
  //        the last <p Element> ?
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc <e bool.true> if this <c OMSetPropertyIterator> is
  //          positioned after the last <p Element>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Element>
bool OMSetPropertyIterator<Element>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMSetPropertyIterator> validly
  //        positioned on a <p Element> ?
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc <e bool.true> if this <c OMSetPropertyIterator> is
  //          positioned on a <p Element>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Element>
bool OMSetPropertyIterator<Element>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p Element>s in the associated
  //        <c OMSetProperty>.
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc The number of <p Element>s
  //   @this const
template <typename Element>
size_t OMSetPropertyIterator<Element>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMSetPropertyIterator> to the next
  //        <p Element>, if any.
  //        If the end of the associated
  //        <c OMSetProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMSetPropertyIterator::valid> becomes <e bool.true>
  //        and <mf OMSetPropertyIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMSetProperty> is reached then the result
  //        is <e bool.false>, <mf OMSetPropertyIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMSetPropertyIterator::after> becomes
  //        <e bool.true>.
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc <e bool.false> if this <c OMSetPropertyIterator> has
  //          passed the last <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMSetPropertyIterator<Element>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMSetPropertyIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated
  //        <c OMSetProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMSetPropertyIterator::valid> becomes <e bool.true>
  //        and <mf OMSetPropertyIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMSetProperty> is reached then the result
  //        is <e bool.false>, <mf OMSetPropertyIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMSetPropertyIterator::before> becomes
  //        <e bool.true>.
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc <e bool.false> if this <c OMSetPropertyIterator> has
  //          passed the first <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMSetPropertyIterator<Element>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p Element> in the associated
  //        <c OMSetProperty> at the position currently
  //        designated by this <c OMSetPropertyIterator>.
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Element>
Element OMSetPropertyIterator<Element>::value(void) const
{
  TRACE("OMSetPropertyIterator<Element>::value");

  return _iterator.value();
}

  // @mfunc Return the element in the associated
  //        <c OMSetProperty> at the position currently
  //        designated by this <c OMSetPropertyIterator>.
  //   @tcarg class | Element | The type of the contained items.
  //   @rdesc The element at the current position.
  //   @this const
template <typename Element>
const OMByte*
OMSetPropertyIterator<Element>::currentElement(void) const
{
  TRACE("OMSetPropertyIterator<Element>::currentElement");

  const OMByte* result = reinterpret_cast<const OMByte*>(&_iterator.value());

  return result;
}

  // @mfunc Create an <c OMSetPropertyIterator> given
  //        an underlying <c OMSetIterator>.
  //   @tcarg class | Element | The type of the contained items.
  //   @parm The underlying <c OMSetIterator>.
template <typename Element>
OMSetPropertyIterator<Element>::OMSetPropertyIterator(const SetIterator& iter)
  : _iterator(iter) // probably bitwise
{
}
