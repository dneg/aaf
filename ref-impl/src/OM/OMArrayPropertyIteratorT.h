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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMArrayPropertyIterator
#include "OMAssertions.h"

#include "OMArrayProperty.h"

  // @mfunc Create an <c OMArrayPropertyIterator> over the given
  //        <c OMArrayProperty> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMArrayPropertyIterator> is made ready to traverse the
  //        associated <c OMArrayProperty> in the forward direction
  //        (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMArrayPropertyIterator> is made ready to traverse the
  //        associated <c OMArrayProperty> in the reverse direction
  //        (decreasing indexes).
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The <c OMArrayProperty> over which to iterate.
  //   @parm The initial position for this <c OMArrayPropertyIterator>.
template <typename Element>
OMArrayPropertyIterator<Element>::OMArrayPropertyIterator(
                                        const OMArrayProperty<Element>& vector,
                                        OMIteratorPosition initialPosition)
  : _iterator(vector._vector, initialPosition)
{
  TRACE("OMArrayPropertyIterator<Element>::OMArrayPropertyIterator");
}

  // @mfunc Destroy this <c OMArrayPropertyIterator>.
  //   @tcarg class | Element | The type of the contained elements.
template <typename Element>
OMArrayPropertyIterator<Element>::~OMArrayPropertyIterator(void)
{
  TRACE("OMArrayPropertyIterator<Element>::~OMArrayPropertyIterator");
}
template <typename Element>
OMDataContainerIterator* OMArrayPropertyIterator<Element>::copy(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::copy");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Reset this <c OMArrayPropertyIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMArrayPropertyIterator> is made ready to traverse the
  //        associated <c OMArrayProperty> in the forward direction
  //        (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMArrayPropertyIterator> is made ready to traverse the
  //        associated <c OMArrayProperty> in the reverse direction
  //        (decreasing indexes).
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The position to which this <c OMArrayPropertyIterator>
  //         should be reset.
template <typename Element>
void
OMArrayPropertyIterator<Element>::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMArrayPropertyIterator<Element>::reset");
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMArrayPropertyIterator> positioned before the first
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMArrayPropertyIterator> is
  //          positioned before
  //          the first <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMArrayPropertyIterator<Element>::before(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::before");
  return _iterator.before();
}

  // @mfunc Is this <c OMArrayPropertyIterator> positioned after the last
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMArrayPropertyIterator> is
  //          positioned after
  //          the last <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMArrayPropertyIterator<Element>::after(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::after");
  return _iterator.after();
}

  // @mfunc The number of <p Elements>s in the associated
  //        <c OMArrayProperty>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The number of <p Elements>s
  //   @this const
template <typename Element>
size_t OMArrayPropertyIterator<Element>::count(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::count");
  return _iterator.count();
}

template <typename Element>
bool OMArrayPropertyIterator<Element>::valid(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::valid");
  return _iterator.valid();
}

  // @mfunc Advance this <c OMArrayPropertyIterator> to the next <p Element>,
  //        if any.
  //        If the end of the associated <c OMArrayProperty> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMArrayPropertyIterator::valid> becomes <e bool.true> and
  //        <mf OMArrayPropertyIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMArrayProperty> is reached
  //        then the result is <e bool.false>,
  //        <mf OMArrayPropertyIterator::valid> becomes <e bool.false> and
  //        <mf OMArrayPropertyIterator::after> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMArrayPropertyIterator> has passed
  //          the last <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMArrayPropertyIterator<Element>::operator++()
{
  TRACE("OMArrayPropertyIterator<Element>::operator++()");
  return ++_iterator;
}

  // @mfunc Retreat this <c OMArrayPropertyIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated <c OMArrayProperty> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMArrayPropertyIterator::valid> becomes <e bool.true> and
  //        <mf OMArrayPropertyIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMArrayProperty> is
  //        reached then the result is <e bool.false>,
  //        <mf OMArrayPropertyIterator::valid> becomes <e bool.false> and
  //        <mf OMArrayPropertyIterator::before> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMArrayPropertyIterator> has passed
  //          the first <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMArrayPropertyIterator<Element>::operator--()
{
  TRACE("OMArrayPropertyIterator<Element>::operator--()");
  return --_iterator;
}

template <typename Element>
const OMByte* OMArrayPropertyIterator<Element>::currentElement(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::currentElement");

  const OMByte* result = reinterpret_cast<const OMByte*>(&_iterator.value());

  return result;
}

  // @mfunc Return the <p Element> in the associated <c OMArrayProperty>
  //        at the position currently designated by this
  //        <c OMArrayPropertyIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Element>
Element& OMArrayPropertyIterator<Element>::value(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::value");

  return _iterator.value();
}

  // @mfunc Set the <p Element> in the associated <c OMArrayProperty> at the
  //        position currently designated by this
  //        <c OMArrayPropertyIterator> to <p newElement>. The previous
  //        <p Element> is returned.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The new <p Element>.
  //   @rdesc The previous <p Element>.
template <typename Element>
Element OMArrayPropertyIterator<Element>::setValue(Element newElement)
{
  TRACE("OMArrayPropertyIterator<Element>::setValue");
  return _iterator.setValue(newElement);
}

  // @mfunc Return the index of the <p Element> in the
  //        associated <c OMArrayProperty> at the position
  //        currently designated by this <c OMArrayPropertyIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The index of the current position.
  //   @this const
template <typename Element>
size_t OMArrayPropertyIterator<Element>::index(void) const
{
  TRACE("OMArrayPropertyIterator<Element>::index");
  return _iterator.index();
}

