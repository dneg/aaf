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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMReferenceVectorIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMReferenceVectorIterator> over the given
  //        <c OMReferenceVector> <p vector> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMReferenceVector> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMReferenceVector> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The <c OMStrongReferenceVector> over which to iterate.
  //   @parm The initial position for this <c OMReferenceVectorIterator>.
template <typename ReferencedObject>
OMReferenceVectorIterator<ReferencedObject>::OMReferenceVectorIterator(
                             const OMReferenceVector<ReferencedObject>& vector,
                             OMIteratorPosition initialPosition)
  : _iterator(vector._vector, initialPosition)
{
}

  // @mfunc Destroy this <c OMReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
template <typename ReferencedObject>
OMReferenceVectorIterator<ReferencedObject>::~OMReferenceVectorIterator(void)
{
}

  // @mfunc Create a copy of this <c OMReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The new <c OMReferenceVectorIterator>.
  //   @this const
template <typename ReferencedObject>
OMReferenceContainerIterator*
                  OMReferenceVectorIterator<ReferencedObject>::copy(void) const
{
  TRACE("OMReferenceVectorIterator<ReferencedObject>::copy");

  OMReferenceVectorIterator<ReferencedObject>*
     result = new OMReferenceVectorIterator<ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMReferenceVectorIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMReferenceVector> in the forward
  //        direction (increasing indexes).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMReferenceVectorIterator> is made ready to traverse the
  //        associated <c OMReferenceVector> in the reverse
  //        direction (decreasing indexes).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The position to which this <c OMReferenceVectorIterator>
  //         should be reset.
template <typename ReferencedObject>
void OMReferenceVectorIterator<ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMReferenceVectorIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMReferenceVectorIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVectorIterator<ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMReferenceVectorIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMReferenceVectorIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVectorIterator<ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMReferenceVectorIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMReferenceVectorIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVectorIterator<ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename ReferencedObject>
size_t OMReferenceVectorIterator<ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMReferenceVectorIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMReferenceVector> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMReferenceVectorIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMReferenceVector> is reached then the result
  //        is <e bool.false>, <mf OMReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMReferenceVectorIterator::after> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMReferenceVectorIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename ReferencedObject>
bool OMReferenceVectorIterator<ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMReferenceVectorIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMReferenceVector> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMReferenceVectorIterator::valid> becomes <e bool.true>
  //        and <mf OMReferenceVectorIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMReferenceVector> is reached then the result
  //        is <e bool.false>, <mf OMReferenceVectorIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMReferenceVectorIterator::before> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMReferenceVectorIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename ReferencedObject>
bool OMReferenceVectorIterator<ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMReferenceVector> at the position currently
  //        designated by this <c OMReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename ReferencedObject>
ReferencedObject*
OMReferenceVectorIterator<ReferencedObject>::value(void) const
{
  TRACE("OMReferenceVectorIterator<ReferencedObject>::value");

  const VectorElement& element = _iterator.value();

  ReferencedObject* result = element.getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMReferenceVector> at the position currently
  //        designated by this <c OMReferenceVectorIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename ReferencedObject>
ReferencedObject*
OMReferenceVectorIterator<ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMReferenceVectorIterator<ReferencedObject>::setValue");

  VectorElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(newObject);

  return result;
}

  // @mfunc Return the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMReferenceVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <c OMObject> at the current position.
  //   @this const
template <typename ReferencedObject>
OMObject*
OMReferenceVectorIterator<ReferencedObject>::currentObject(void) const
{
  TRACE("OMReferenceVectorIterator<ReferencedObject>::currentObject");

  OMObject* result = value();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Clear (set to 0) the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMReferenceVectorIterator>.
  //        The existing object, if any, is returned. The associated
  //        reference container is not modified in that no entry is
  //        removed, the existing entry remains but no longer refers
  //        to a valid object.
  //   @rdesc The previous <c OMObject> at the current position.
template <typename ReferencedObject>
OMObject*
OMReferenceVectorIterator<ReferencedObject>::clearObject(void)
{
  TRACE("OMReferenceVectorIterator<ReferencedObject>::clearObject");

  OMObject* result = 0;
  ASSERT("Unimplemented code not reached", false); // tjb
  return result;
}

  // @mfunc Return the index of the <p ReferencedObject> in the
  //        associated <c OMReferenceVector> at the position
  //        currently designated by this
  //        <c OMReferenceVectorIterator>.
  //   @tcarg class | Element | The type of the contained elements.
  //   @rdesc The index of the current position.
  //   @this const
template <typename ReferencedObject>
size_t OMReferenceVectorIterator<ReferencedObject>::index(void) const
{
  return _iterator.index();
}

  // @mfunc Create an <c OMReferenceVectorIterator> given
  //        an underlying <c OMVectorIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The underlying <c OMVectorIterator>.
template <typename ReferencedObject>
OMReferenceVectorIterator<ReferencedObject>::OMReferenceVectorIterator(
                                                   const VectorIterator & iter)
  : _iterator(iter) // probably bitwise
{
}
