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
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMWeakReferenceVectorIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMWeakReferenceVectorIterator> over the given
  //        <c OMWeakReferenceVectorProperty> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The <c OMWeakReferenceVector> over which to iterate.
  //   @parm The initial position for this <c OMWeakReferenceVectorIterator>.
template <typename ReferencedObject>
OMWeakReferenceVectorIterator<ReferencedObject>::OMWeakReferenceVectorIterator(
                 const OMWeakReferenceVectorProperty<ReferencedObject>& vector,
                 OMIteratorPosition initialPosition)
  : _iterator(vector._vector, initialPosition)
{
}

  // @mfunc Destroy this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
template <typename ReferencedObject>
OMWeakReferenceVectorIterator<ReferencedObject>::
                                                ~OMWeakReferenceVectorIterator(
                                                                          void)
{
}

  // @mfunc Create a copy of this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The new <c OMWeakReferenceVectorIterator>.
  //   @this const
template <typename ReferencedObject>
OMReferenceContainerIterator*
              OMWeakReferenceVectorIterator<ReferencedObject>::copy(void) const
{
  TRACE("OMWeakReferenceVectorIterator<ReferencedObject>::copy");

  OMWeakReferenceVectorIterator<ReferencedObject>*
       result = new OMWeakReferenceVectorIterator<ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMWeakReferenceVectorIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMWeakReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceVectorProperty> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The position to which this <c OMWeakReferenceVectorIterator>
  //         should be reset.
template <typename ReferencedObject>
void OMWeakReferenceVectorIterator<ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMWeakReferenceVectorIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceVectorIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorIterator<ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMWeakReferenceVectorIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceVectorIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorIterator<ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMWeakReferenceVectorIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceVectorIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorIterator<ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorIterator<ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMWeakReferenceVectorIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMWeakReferenceVectorProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMWeakReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMWeakReferenceVectorIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMWeakReferenceVectorProperty> is reached then the result
  //        is <e bool.false>, <mf OMWeakReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMWeakReferenceVectorIterator::after> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMWeakReferenceVectorIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename ReferencedObject>
bool OMWeakReferenceVectorIterator<ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMWeakReferenceVectorIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMWeakReferenceVectorProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMWeakReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMWeakReferenceVectorIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMWeakReferenceVectorProperty> is reached then the result
  //        is <e bool.false>, <mf OMWeakReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMWeakReferenceVectorIterator::before> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMWeakReferenceVectorIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename ReferencedObject>
bool OMWeakReferenceVectorIterator<ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorIterator<ReferencedObject>::value(void) const
{
  TRACE("OMWeakReferenceVectorIterator<ReferencedObject>::value");

  const VectorElement& element = _iterator.value();

  ReferencedObject* result = element.getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorIterator<ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMWeakReferenceVectorIterator<ReferencedObject>::setValue");

  VectorElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(newObject);

  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator> to 0.
  //        The previous <p ReferencedObject>, if any, is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorIterator<ReferencedObject>::clearValue(void)
{
  TRACE("OMWeakReferenceVectorIterator<ReferencedObject>::clearValue");

  VectorElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(0);

  return result;
}

  // @mfunc Return the index of the <p ReferencedObject> in the
  //        associated <c OMWeakReferenceVectorProperty> at the position
  //        currently designated by this
  //        <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The index of the current position.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorIterator<ReferencedObject>::index(void) const
{
  return _iterator.index();
}

  // @mfunc Return the <p Key> of the <p ReferencedObject> in the
  //        associated <c OMWeakReferenceVectorProperty> at the position
  //        currently designated by this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename ReferencedObject>
OMUniqueObjectIdentification
OMWeakReferenceVectorIterator<ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceVectorIterator<ReferencedObject>::identification");

  const VectorElement& element = _iterator.value();

  return element.identification();
}

  // @mfunc Return the <c OMObject> in the associated
  //        <c OMWeakReferenceVectorProperty> at the position currently
  //        designated by this <c OMWeakReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <c OMObject> at the current position.
  //   @this const
template <typename ReferencedObject>
OMObject*
OMWeakReferenceVectorIterator<ReferencedObject>::currentObject(void) const
{
  TRACE("OMWeakReferenceVectorIterator<ReferencedObject>::currentObject");

  OMObject* result = value();

  return result;
}

  // @mfunc Create an <c OMWeakReferenceVectorIterator> given
  //        an underlying <c OMVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The underlying <c OMVectorIterator>.
template <typename ReferencedObject>
OMWeakReferenceVectorIterator<ReferencedObject>::OMWeakReferenceVectorIterator(
                                                    const VectorIterator& iter)
  : _iterator(iter) // probably bitwise
{
}
