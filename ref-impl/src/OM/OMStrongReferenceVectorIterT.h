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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMStrongReferenceVectorIterator
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

  OMStorable* p = element.getValue();
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }

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

  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(newObject);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
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

  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(0);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
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

  // @mfunc Clear (set to 0) the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMStrongReferenceVectorIterator>.
  //        The existing object, if any, is returned. The associated
  //        reference container is not modified in that no entry is
  //        removed, the existing entry remains but no longer refers
  //        to a valid object.
  //   @rdesc The previous <c OMObject> at the current position.
template <typename ReferencedObject>
OMObject*
OMStrongReferenceVectorIterator<ReferencedObject>::clearObject(void)
{
  TRACE("OMStrongReferenceVectorIterator<ReferencedObject>::clearObject");

  OMObject* result = setValue(0);
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
