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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMListIterator
#include "OMAssertions.h"

#include "OMList.h"

  // @mfunc Default <c OMListIterator> constructor.
template <typename Element>
OMListIterator<Element>::OMListIterator(void)
: _list(0),
  _current(0),
  _state(OMListIteratorBefore) // tjb arbitrary
{
  TRACE("OMListIterator<Element>::OMListIterator");
}

  // @mfunc Create an <c OMListIterator> over the given
  //        <c OMList> <p list> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMListIterator> is made ready to traverse the
  //        associated <c OMList> in the forward direction.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMListIterator> is made ready to traverse the
  //        associated <c OMList> in the reverse direction.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The <c OMList> over which to iterate.
  //   @parm The initial position for this <c OMListIterator>.
template <typename Element>
OMListIterator<Element>::OMListIterator(const OMList<Element>& list,
                                        OMIteratorPosition initialPosition)
  : _list(&list)
{
  TRACE("OMListIterator<Element>::OMListIterator");
  reset(initialPosition);
}

  // @mfunc Destroy this <c OMListIterator>.
  //   @tcarg class | Element | The type of the contained elements.
template <typename Element>
OMListIterator<Element>::~OMListIterator(void)
{
  TRACE("OMListIterator<Element>::~OMListIterator");
}

  // @mfunc Reset this <c OMListIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMListIterator> is made ready to traverse the
  //        associated <c OMList> in the forward direction.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMListIterator> is made ready to traverse the
  //        associated <c OMList> in the reverse direction.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The position to which this <c OMListIterator> should be reset.
template <typename Element>
void OMListIterator<Element>::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMListIterator<Element>::reset");

  PRECONDITION("Valid initial position",
                (initialPosition == OMBefore) || (initialPosition == OMAfter));

  _current = _list->_nil;
  if (initialPosition == OMBefore) {
    _state = OMListIteratorBefore;
  } else {
    _state = OMListIteratorAfter;
  }

  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMBefore, before()));
  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMAfter,  after()));
}

  // @mfunc Is this <c OMListIterator> positioned before the first
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMListIterator> is positioned before
  //          the first <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMListIterator<Element>::before(void) const
{
  TRACE("OMListIterator<Element>::before");

  bool result;
  if (_state == OMListIteratorBefore) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMListIterator> positioned after the last
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMListIterator> is positioned after
  //          the last <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMListIterator<Element>::after(void) const
{
  TRACE("OMListIterator<Element>::after");

  bool result;
  if (_state == OMListIteratorAfter) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The number of <p Elements>s in the associated
  //        <c OMList>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The number of <p Elements>s
  //   @this const
template <typename Element>
size_t OMListIterator<Element>::count(void) const
{
  TRACE("OMListIterator<Element>::count");

  return _list->count();
}

  // @mfunc Advance this <c OMListIterator> to the next <p Element>,
  //        if any.
  //        If the end of the associated <c OMList> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMListIterator::valid> becomes <e bool.true> and
  //        <mf OMListIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMList> is reached
  //        then the result is <e bool.false>,
  //        <mf OMListIterator::valid> becomes <e bool.false> and
  //        <mf OMListIterator::after> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMListIterator> has passed
  //          the last <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMListIterator<Element>::operator++()
{
  TRACE("OMListIterator<Element>::operator++()");

  PRECONDITION("Valid iterator", before() || valid());

  bool result;
  _current = _current->_next;
  if (_current != _list->_nil) {
    _state = OMListIteratorValid;
    result = true;
  } else {
    _state = OMListIteratorAfter;
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !after()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  after()));
  POSTCONDITION("Consistent result", IMPLIES( result,  valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !valid()));
  return result;
}

  // @mfunc Retreat this <c OMListIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated <c OMList> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMListIterator::valid> becomes <e bool.true> and
  //        <mf OMListIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMList> is
  //        reached then the result is <e bool.false>,
  //        <mf OMListIterator::valid> becomes <e bool.false> and
  //        <mf OMListIterator::before> becomes <e bool.true>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMListIterator> has passed
  //          the first <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMListIterator<Element>::operator--()
{
  TRACE("OMListIterator<Element>::operator--()");

  PRECONDITION("Valid iterator", after() || valid());

  bool result;
  _current = _current->_previous;
  if (_current != _list->_nil) {
    _state = OMListIteratorValid;
    result = true;
  } else {
    _state = OMListIteratorBefore;
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !before()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  before()));
  POSTCONDITION("Consistent result", IMPLIES( result,  valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !valid()));

  return result;
}

  // @mfunc Return the <p Element> in the associated <c OMList>
  //        at the position currently designated by this
  //        <c OMListIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Element>
Element& OMListIterator<Element>::value(void) const
{
  TRACE("OMListIterator<Element>::value");

  PRECONDITION("Valid iterator", valid());

  return _current->_value;
}

  // @mfunc Set the <p Element> in the associated <c OMList> at the
  //        position currently designated by this
  //        <c OMListIterator> to <p newElement>. The previous
  //        <p Element> is returned.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The new <p Element>.
  //   @rdesc The previous <p Element>.
template <typename Element>
Element OMListIterator<Element>::setValue(Element newElement)
{
  TRACE("OMListIterator<Element>::setValue");

  PRECONDITION("Valid iterator", valid());

  Element result = value();

  _current->_value = newElement;

  return result;
}
