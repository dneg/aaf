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

// @doc OMEXTERNAL
#include "OMAssertions.h"

  // @mfunc Create an <c OMStrongReferenceVectorIterator> over the given
  //        <c OMStrongReferenceVectorProperty> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMStrongReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceVectorProperty> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMStrongReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceVectorProperty> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The <c OMStrongReferenceVector> over which to iterate.
  //   @parm The initial position for this <c OMStrongReferenceVectorIterator>.
template <typename ReferencedObject>
OMStrongReferenceVectorIterator<ReferencedObject>::
                                               OMStrongReferenceVectorIterator(
               const OMStrongReferenceVectorProperty<ReferencedObject>& vector,
               OMIteratorPosition initialPosition)
  : _iterator(vector._vector, initialPosition)
{
}

  // @mfunc Destroy this <c OMStrongReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
template <typename ReferencedObject>
OMStrongReferenceVectorIterator<ReferencedObject>::
                                              ~OMStrongReferenceVectorIterator(
                                                                          void)
{
}

  // @mfunc Create a copy of this <c OMStrongReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The new <c OMStrongReferenceVectorIterator>.
  //   @this const
template <typename ReferencedObject>
OMReferenceContainerIterator*
            OMStrongReferenceVectorIterator<ReferencedObject>::copy(void) const
{
  TRACE("OMStrongReferenceVectorIterator<ReferencedObject>::copy");

  OMStrongReferenceVectorIterator<ReferencedObject>*
     result = new OMStrongReferenceVectorIterator<ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMStrongReferenceVectorIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMStrongReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceVectorProperty> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMStrongReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceVectorProperty> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The position to which this <c OMStrongReferenceVectorIterator>
  //         should be reset.
template <typename ReferencedObject>
void OMStrongReferenceVectorIterator<ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMStrongReferenceVectorIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMStrongReferenceVectorIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceVectorIterator<ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMStrongReferenceVectorIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMStrongReferenceVectorIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceVectorIterator<ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMStrongReferenceVectorIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMStrongReferenceVectorIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceVectorIterator<ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename ReferencedObject>
size_t OMStrongReferenceVectorIterator<ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMStrongReferenceVectorIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMStrongReferenceVectorProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMStrongReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMStrongReferenceVectorIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMStrongReferenceVectorProperty> is reached then the result
  //        is <e bool.false>, <mf OMStrongReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMStrongReferenceVectorIterator::after> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMStrongReferenceVectorIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename ReferencedObject>
bool OMStrongReferenceVectorIterator<ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMStrongReferenceVectorIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMStrongReferenceVectorProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMStrongReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMStrongReferenceVectorIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMStrongReferenceVectorProperty> is reached then the result
  //        is <e bool.false>, <mf OMStrongReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMStrongReferenceVectorIterator::before> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMStrongReferenceVectorIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename ReferencedObject>
bool OMStrongReferenceVectorIterator<ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMStrongReferenceVectorProperty> at the position currently
  //        designated by this <c OMStrongReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename ReferencedObject>
ReferencedObject*
OMStrongReferenceVectorIterator<ReferencedObject>::value(void) const
{
  TRACE("OMStrongReferenceVectorIterator<ReferencedObject>::value");

  const VectorElement& element = _iterator.value();

  ReferencedObject* result = element.getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMStrongReferenceVectorProperty> at the position currently
  //        designated by this <c OMStrongReferenceVectorIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename ReferencedObject>
ReferencedObject*
OMStrongReferenceVectorIterator<ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMStrongReferenceVectorIterator<ReferencedObject>::setValue");

  VectorElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(newObject);

  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMStrongReferenceVectorProperty> at the position currently
  //        designated by this <c OMStrongReferenceVectorIterator> to 0.
  //        The previous <p ReferencedObject>, if any, is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename ReferencedObject>
ReferencedObject*
OMStrongReferenceVectorIterator<ReferencedObject>::clearValue(void)
{
  TRACE("OMStrongReferenceVectorIterator<ReferencedObject>::clearValue");

  VectorElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(0);

  return result;
}

  // @mfunc Return the index of the <p ReferencedObject> in the
  //        associated <c OMStrongReferenceVectorProperty> at the position
  //        currently designated by this
  //        <c OMStrongReferenceVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The index of the current position.
  //   @this const
template <typename ReferencedObject>
size_t OMStrongReferenceVectorIterator<ReferencedObject>::index(void) const
{
  return _iterator.index();
}

  // @mfunc Return the <c OMObject> in the associated
  //        <c OMStrongReferenceVectorProperty> at the position currently
  //        designated by this <c OMStrongReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <c OMObject> at the current position.
  //   @this const
template <typename ReferencedObject>
OMObject*
OMStrongReferenceVectorIterator<ReferencedObject>::currentObject(void) const
{
  TRACE("OMStrongReferenceVectorIterator<ReferencedObject>::currentObject");

  OMObject* result = value();

  return result;
}

  // @mfunc Create an <c OMStrongReferenceVectorIterator> given
  //        an underlying <c OMVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The underlying <c OMVectorIterator>.
template <typename ReferencedObject>
OMStrongReferenceVectorIterator<ReferencedObject>::
                                               OMStrongReferenceVectorIterator(
                                                   const VectorIterator & iter)
  : _iterator(iter) // probably bitwise
{
}
