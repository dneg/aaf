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
#include "OMAssertions.h"

  // @mfunc Create an <c OMVectorIterator> over the given
  //        <c OMVector> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the forward direction
  //        (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the reverse direction
  //        (decreasing indexes).
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The <c OMVector> over which to iterate.
  //   @parm The initial position for this <c OMVectorIterator>.
template <typename Element>
OMVectorIterator<Element>::OMVectorIterator(const OMVector<Element>& vector,
                                            OMIteratorPosition initialPosition)
  : _vector(&vector)
{
  TRACE("OMVectorIterator<Element>::OMVectorIterator");
  reset(initialPosition);
}

  // @mfunc Destroy this <c OMVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
template <typename Element>
OMVectorIterator<Element>::~OMVectorIterator(void)
{
  TRACE("OMVectorIterator<Element>::~OMVectorIterator");
}

  // @mfunc Reset this <c OMVectorIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the forward direction
  //        (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMVectorIterator> is made ready to traverse the
  //        associated <c OMVector> in the reverse direction
  //        (decreasing indexes).
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The position to which this <c OMVectorIterator> should be reset.
template <typename Element>
void OMVectorIterator<Element>::reset(OMIteratorPosition initialPosition)
{
  TRACE("OMVectorIterator<Element>::reset");

  PRECONDITION("Valid initial position",
                (initialPosition == OMBefore) || (initialPosition == OMAfter));

  if (initialPosition == OMBefore) {
    _state = OMVectorIteratorBefore;
  } else {
    _state = OMVectorIteratorAfter;
  }

  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMBefore, before()));
  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMAfter,  after()));
}

  // @mfunc Is this <c OMVectorIterator> positioned before the first
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMVectorIterator> is positioned before
  //          the first <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMVectorIterator<Element>::before(void) const
{
  TRACE("OMVectorIterator<Element>::before");

  bool result;
  if (_state == OMVectorIteratorBefore) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMVectorIterator> positioned after the last
  //        <p Element> ?
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.true> if this <c OMVectorIterator> is positioned after
  //          the last <p Element>, <e bool.false> otherwise.
  //   @this const
template <typename Element>
bool OMVectorIterator<Element>::after(void) const
{
  TRACE("OMVectorIterator<Element>::after");

  bool result;
  if (_state == OMVectorIteratorAfter) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The number of <p Elements>s in the associated
  //        <c OMVector>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The number of <p Elements>s
  //   @this const
template <typename Element>
size_t OMVectorIterator<Element>::count(void) const
{
  TRACE("OMVectorIterator<Element>::count");

  return _vector->count();
}

  // @mfunc Advance this <c OMVectorIterator> to the next <p Element>,
  //        if any.
  //        If the end of the associated <c OMVector> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMVectorIterator::valid> becomes <e bool.true> and
  //        <mf OMVectorIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMVector> is reached
  //        then the result is <e bool.false>,
  //        <mf OMVectorIterator::valid> becomes <e bool.false> and
  //        <mf OMVectorIterator::after> becomes <e bool.true>. 
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMVectorIterator> has passed
  //          the last <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMVectorIterator<Element>::operator++()
{
  TRACE("OMVectorIterator<Element>::operator++()");

  PRECONDITION("Valid iterator", before() || valid());

  bool result;
  if (_state == OMVectorIteratorBefore) {
    // we are in state "before", so point to first value, if any
    if (_vector->count() != 0) {
      // vector contains some elements
      _index = 0;
      _state = OMVectorIteratorValid;
    } else {
      // vector is empty, go to state "after"
      _state = OMVectorIteratorAfter;
    }
  } else if (_index < _vector->count() - 1) {
    // advance
    _index = _index + 1;
    _state = OMVectorIteratorValid;
  } else {
    // no more elements, go to state "after"
    _state = OMVectorIteratorAfter;
  }
  if (_state == OMVectorIteratorValid) {
    result = true;
  } else {
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !after()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  after()));
  POSTCONDITION("Consistent result", IMPLIES( result,  valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !valid()));
  POSTCONDITION("Valid index",
               IMPLIES(valid(), (_index >= 0) && (_index < _vector->count())));
  return result;
}

  // @mfunc Retreat this <c OMVectorIterator> to the previous
  //        <p Element>, if any.
  //        If the beginning of the associated <c OMVector> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMVectorIterator::valid> becomes <e bool.true> and
  //        <mf OMVectorIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMVector> is
  //        reached then the result is <e bool.false>,
  //        <mf OMVectorIterator::valid> becomes <e bool.false> and
  //        <mf OMVectorIterator::before> becomes <e bool.true>. 
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc <e bool.false> if this <c OMVectorIterator> has passed
  //          the first <p Element>, <e bool.true> otherwise.
template <typename Element>
bool OMVectorIterator<Element>::operator--()
{
  TRACE("OMVectorIterator<Element>::operator--()");

  PRECONDITION("Valid iterator", after() || valid());

  bool result;
  if (_state == OMVectorIteratorAfter) {
    // we are in state "after", so point to last value, if any
    if (_vector->count() != 0) {
      // vector contains some elements
      _index = _vector->count() - 1;
      _state = OMVectorIteratorValid;
    } else {
      // vector is empty, go to state "before"
      _state = OMVectorIteratorBefore;
    }
  } else if (_index > 0) {
    // retreat
    _index = _index - 1;
    _state = OMVectorIteratorValid;
  } else {
    // no more elements, go to state "before"
    _state = OMVectorIteratorBefore;
  }
  if (_state == OMVectorIteratorValid) {
    result = true;
  } else {
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !before()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  before()));
  POSTCONDITION("Consistent result", IMPLIES( result,  valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !valid()));
  POSTCONDITION("Valid index",
               IMPLIES(valid(), (_index >= 0) && (_index < _vector->count())));
  return result;
}

  // @mfunc Return the <p Element> in the associated <c OMVector>
  //        at the position currently designated by this
  //        <c OMVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The <p Element> at the current position.
  //   @this const
template <typename Element>
Element& OMVectorIterator<Element>::value(void) const
{
  TRACE("OMVectorIterator<Element>::value");

  PRECONDITION("Valid iterator", valid());

  return _vector->getAt(_index);
}

  // @mfunc Set the <p Element> in the associated <c OMVector> at the
  //        position currently designated by this
  //        <c OMVectorIterator> to <p newElement>. The previous
  //        <p Element> is returned.
  //   @tcarg class | Element | The type of the contained elements.
  //   @parm The new <p Element>.
  //   @rdesc The previous <p Element>.
template <typename Element>
Element OMVectorIterator<Element>::setValue(Element newElement)
{
  TRACE("OMVectorIterator<Element>::setValue");

  PRECONDITION("Valid iterator", valid());

  OMVector<Element>* vector = const_cast<OMVector<Element>*>(_vector);

  Element result = vector->getAt(_index);
  vector->setAt(newElement, _index);

  return result;
}

  // @mfunc Return the index of the <p Element> in the
  //        associated <c OMVector> at the position
  //        currently designated by this <c OMVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The index of the current position.
  //   @this const
template <typename Element>
size_t OMVectorIterator<Element>::index(void) const
{
  TRACE("OMVectorIterator<Element>::index");

  PRECONDITION("Valid iterator", valid());

  size_t result = _index;

  POSTCONDITION("Valid index", (_index >= 0) && (_index < _vector->count()));

  return result;
}

