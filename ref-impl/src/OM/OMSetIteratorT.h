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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSetIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMSetIterator> over the given
  //        <c OMSet> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMSetIterator> is made ready to traverse the
  //        associated <c OMSet> in the forward direction
  //        (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMSetIterator> is made ready to traverse the
  //        associated <c OMSet> in the reverse direction
  //        (decreasing <p Key>s).
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The <c OMSet> over which to iterate.
  //   @parm The initial position for this <c OMSetIterator>.
template <typename Key, typename Element>
OMSetIterator<Key, Element>::OMSetIterator(const OMSet<Key, Element>& set,
                                           OMIteratorPosition initialPosition)
  : _iterator(set._tree, initialPosition)
{
  TRACE("OMSetIterator<Key, Element>::OMSetIterator");
}

  // @mfunc Destroy this <c OMSetIterator>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
template <typename Key, typename Element>
OMSetIterator<Key, Element>::~OMSetIterator(void)
{
  TRACE("OMSetIterator<Key, Element>::~OMSetIterator");
}

  // @mfunc Reset this <c OMSetIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMSetIterator> is made ready to traverse the
  //        associated <c OMSet> in the forward direction
  //        (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMSetIterator> is made ready to traverse the
  //        associated <c OMSet> in the reverse direction
  //        (decreasing <p Key>s).
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The position to which this <c OMSetIterator> should be reset.
template <typename Key, typename Element>
void OMSetIterator<Key, Element>::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMSetIterator<Key, Element>::reset");

  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMSetIterator> positioned before the first
  //        <p Element> ?
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMSetIterator> is positioned before
  //          the first <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSetIterator<Key, Element>::before(void) const
{
  TRACE("OMSetIterator<Key, Element>::before");

  return _iterator.before();
}

  // @mfunc Is this <c OMSetIterator> positioned after the last
  //        <p Element> ?
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMSetIterator> is positioned after
  //          the last <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSetIterator<Key, Element>::after(void) const
{
  TRACE("OMSetIterator<Key, Element>::after");

  return _iterator.after();
}

  // @mfunc The number of <p Elements>s in the associated
  //        <c OMSet>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The number of <p Elements>s
  //   @this const
template <typename Key, typename Element>
size_t OMSetIterator<Key, Element>::count(void) const
{
  TRACE("OMSetIterator<Key, Element>::count");

  return _iterator.count();
}

  // @mfunc Advance this <c OMSetIterator> to the next <p Element>,
  //        if any.
  //        If the end of the associated <c OMSet> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMSetIterator::valid> becomes <e bool.true> and
  //        <mf OMSetIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMSet> is reached
  //        then the result is <e bool.false>,
  //        <mf OMSetIterator::valid> becomes <e bool.false> and
  //        <mf OMSetIterator::after> becomes <e bool.true>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMSetIterator> has passed
  //          the last <p Element>, <e bool.true> otherwise.
template <typename Key, typename Element>
bool OMSetIterator<Key, Element>::operator++()
{
  TRACE("OMSetIterator<Key, Element>::operator++()");

  PRECONDITION("Valid iterator", _iterator.before() || _iterator.valid());

  return ++_iterator;
}

  // @mfunc Retreat this <c OMSetIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated <c OMSet> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMSetIterator::valid> becomes <e bool.true> and
  //        <mf OMSetIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMSet> is
  //        reached then the result is <e bool.false>,
  //        <mf OMSetIterator::valid> becomes <e bool.false> and
  //        <mf OMSetIterator::before> becomes <e bool.true>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMSetIterator> has passed
  //          the first <p Element>, <e bool.true> otherwise.
template <typename Key, typename Element>
bool OMSetIterator<Key, Element>::operator--()
{
  TRACE("OMSetIterator<Key, Element>::operator--()");

  PRECONDITION("Valid iterator", _iterator.after() || _iterator.valid());

  return --_iterator;
}

  // @mfunc Return the <p Element> in the associated <c OMSet>
  //        at the position currently designated by this
  //        <c OMSetIterator>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Key, typename Element>
Element& OMSetIterator<Key, Element>::value(void) const
{
  TRACE("OMSetIterator<Key, Element>::value");

  PRECONDITION("Valid iterator", _iterator.valid());

  return _iterator.value();
}

  // @mfunc Set the <p Element> in the associated <c OMSet> at the
  //        position currently designated by this
  //        <c OMSetIterator> to <p newElement>. The previous
  //        <p Element> is returned. To preserve the ordering of
  //        <p Key>s, the <p Key> of <p newElement> must be the same as
  //        that of the existing <p Element>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The <p Key>.
  //   @parm The new <p Element>.
  //   @rdesc The previous <p Element>.
template <typename Key, typename Element>
Element OMSetIterator<Key, Element>::setValue(const Key k,
                                              Element newElement)
{
  TRACE("OMSetIterator<Key, Element>::setValue");

  PRECONDITION("Valid iterator", _iterator.valid());
  PRECONDITION("Matching keys", k == key());

  return _iterator.setValue(k, newElement);
}

  // @mfunc Return the <p Key> of the <p Element> in the associated
  //        <c OMSet> at the position currently designated by this
  //        <c OMSetIterator>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained elements.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename Key, typename Element>
Key OMSetIterator<Key, Element>::key(void) const
{
  TRACE("OMSetIterator<Key, Element>::key");

  PRECONDITION("Valid iterator", _iterator.valid());

  return _iterator.key();
}

