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
  //   @parm The new <p Element>.
  //   @rdesc The previous <p Element>.
template <typename Key, typename Element>
Element OMSetIterator<Key, Element>::setValue(Element newElement)
{
  TRACE("OMSetIterator<Key, Element>::setValue");

  PRECONDITION("Valid iterator", _iterator.valid());
  PRECONDITION("Matching keys", newElement.identification() == key());

  return _iterator.setValue(newElement);
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

