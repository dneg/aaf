//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMWeakReferenceSetIterator
#ifndef OMWEAKREFERENCESETITERT_H
#define OMWEAKREFERENCESETITERT_H

#include "OMAssertions.h"

  // @mfunc Create an <c OMWeakReferenceSetIterator> over the given
  //        <c OMWeakReferenceSetProperty> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMWeakReferenceSetIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceSetProperty> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMWeakReferenceSetIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceSetProperty> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The <c OMWeakReferenceSet> over which to iterate.
  //   @parm The initial position for this <c OMWeakReferenceSetIterator>.
template <typename Key, typename ReferencedObject>
OMWeakReferenceSetIterator<Key, ReferencedObject>::OMWeakReferenceSetIterator(
                  const OMWeakReferenceSetProperty<Key, ReferencedObject>& set,
                  OMIteratorPosition initialPosition)
  : _iterator(set._set, initialPosition)
{
}

  // @mfunc Destroy this <c OMWeakReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
template <typename Key, typename ReferencedObject>
OMWeakReferenceSetIterator<Key, ReferencedObject>::~OMWeakReferenceSetIterator(void)
{
}

  // @mfunc Create a copy of this <c OMWeakReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The new <c OMWeakReferenceSetIterator>.
  //   @this const
template <typename Key, typename ReferencedObject>
OMReferenceContainerIterator*
            OMWeakReferenceSetIterator<Key, ReferencedObject>::copy(void) const
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::copy");

  OMWeakReferenceSetIterator<Key, ReferencedObject>*
     result = new OMWeakReferenceSetIterator<Key, ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMWeakReferenceSetIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMWeakReferenceSetIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceSetProperty> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMWeakReferenceSetIterator> is made ready to traverse the
  //        associated <c OMWeakReferenceSetProperty> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The position to which this <c OMWeakReferenceSetIterator>
  //         should be reset.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceSetIterator<Key, ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMWeakReferenceSetIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceSetIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceSetIterator<Key, ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMWeakReferenceSetIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceSetIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceSetIterator<Key, ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMWeakReferenceSetIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.true> if this <c OMWeakReferenceSetIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceSetIterator<Key, ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename Key, typename ReferencedObject>
size_t OMWeakReferenceSetIterator<Key, ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMWeakReferenceSetIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMWeakReferenceSetProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMWeakReferenceSetIterator::valid> becomes <e bool.true>
  //        and <mf OMWeakReferenceSetIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMWeakReferenceSetProperty> is reached then the result
  //        is <e bool.false>, <mf OMWeakReferenceSetIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMWeakReferenceSetIterator::after> becomes
  //        <e bool.true>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.false> if this <c OMWeakReferenceSetIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceSetIterator<Key, ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMWeakReferenceSetIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMWeakReferenceSetProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMWeakReferenceSetIterator::valid> becomes <e bool.true>
  //        and <mf OMWeakReferenceSetIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMWeakReferenceSetProperty> is reached then the result
  //        is <e bool.false>, <mf OMWeakReferenceSetIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMWeakReferenceSetIterator::before> becomes
  //        <e bool.true>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc <e bool.false> if this <c OMWeakReferenceSetIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceSetIterator<Key, ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceSetProperty> at the position currently
  //        designated by this <c OMWeakReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceSetIterator<Key, ReferencedObject>::value(void) const
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::value");

  const SetElement& element = _iterator.value();

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
  //        <c OMWeakReferenceSetProperty> at the position currently
  //        designated by this <c OMWeakReferenceSetIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned. To preserve the ordering of <p Key>s, the
  //        <p Key> of <p newObject> must be the same as that of the
  //        existing <p ReferencedObject>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceSetIterator<Key, ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::setValue");

  PRECONDITION("Matching keys",
    IMPLIES(newObject != 0 , newObject->identification() == identification()));

  Key id = OMConstant<Key>::null;
  if (newObject != 0) {
    id = newObject->identification();
  }

  SetElement& element = _iterator.value();

  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(&id, newObject);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMWeakReferenceSetProperty> at the position currently
  //        designated by this <c OMWeakReferenceSetIterator> to 0.
  //        The previous <p ReferencedObject>, if any, is returned.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceSetIterator<Key, ReferencedObject>::clearValue(void)
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::clearValue");

  SetElement& element = _iterator.value();

  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(&(OMConstant<Key>::null), 0);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  return result;
}

  // @mfunc Return the <p Key> of the <p ReferencedObject> in the
  //        associated <c OMWeakReferenceSetProperty> at the position
  //        currently designated by this <c OMWeakReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
Key
OMWeakReferenceSetIterator<Key, ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::identification");

  return _iterator.key();
}

  // @mfunc Return the <c OMObject> in the associated
  //        <c OMWeakReferenceSetProperty> at the position currently
  //        designated by this <c OMWeakReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @rdesc The <c OMObject> at the current position.
  //   @this const
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceSetIterator<Key, ReferencedObject>::currentObject(void) const
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::currentObject");

  OMObject* result = value();

  return result;
}

  // @mfunc Clear (set to 0) the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMWeakReferenceSetIterator>.
  //        The existing object, if any, is returned. The associated
  //        reference container is not modified in that no entry is
  //        removed, the existing entry remains but no longer refers
  //        to a valid object.
  //   @rdesc The previous <c OMObject> at the current position.
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceSetIterator<Key, ReferencedObject>::clearObject(void)
{
  TRACE("OMWeakReferenceSetIterator<Key, ReferencedObject>::clearObject");

  OMObject* result = 0;
  ASSERT("Unimplemented code not reached", false); // tjb
  return result;
}

  // @mfunc Create an <c OMWeakReferenceSetIterator> given
  //        an underlying <c OMSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @tcarg class | Key | The type of the identifier of the contained object.
  //   @parm The underlying <c OMSetIterator>.
template <typename Key, typename ReferencedObject>
OMWeakReferenceSetIterator<Key, ReferencedObject>::OMWeakReferenceSetIterator(
                                                       const SetIterator& iter)
  : _iterator(iter) // probably bitwise
{
}

#endif
