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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMReferenceSetIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMStrongReferenceSetIterator> over the given
  //        <c OMReferenceSet> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMStrongReferenceSetIterator> is made ready to traverse the
  //        associated <c OMReferenceSet> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMStrongReferenceSetIterator> is made ready to traverse the
  //        associated <c OMReferenceSet> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The <c OMStrongReferenceSet> over which to iterate.
  //   @parm The initial position for this <c OMReferenceSetIterator>.
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceSetIterator<UniqueIdentification,
                       ReferencedObject>::OMReferenceSetIterator(
                                   const OMReferenceSet<UniqueIdentification,
                                                        ReferencedObject>& set,
                                   OMIteratorPosition initialPosition)
  : _iterator(set._set, initialPosition)
{
}

  // @mfunc Destroy this <c OMReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceSetIterator<UniqueIdentification,
                       ReferencedObject>::~OMReferenceSetIterator(void)
{
}

  // @mfunc Create a copy of this <c OMReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The new <c OMReferenceSetIterator>.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceContainerIterator*
OMReferenceSetIterator<UniqueIdentification,
                       ReferencedObject>::copy(void) const
{
  TRACE("OMReferenceSetIterator<UniqueIdentification, "
                                                    "ReferencedObject>::copy");

  OMReferenceSetIterator<UniqueIdentification, ReferencedObject>*
              result = new OMReferenceSetIterator<UniqueIdentification,
                                                  ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMReferenceSetIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMReferenceSetIterator> is made ready to traverse the
  //        associated <c OMReferenceSet> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMReferenceSetIterator> is made ready to traverse the
  //        associated <c OMReferenceSet> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The position to which this <c OMReferenceSetIterator>
  //         should be reset.
template <typename UniqueIdentification, typename ReferencedObject>
void OMReferenceSetIterator<UniqueIdentification, ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMReferenceSetIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMReferenceSetIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSetIterator<UniqueIdentification,
                            ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMReferenceSetIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMReferenceSetIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSetIterator<UniqueIdentification,
                            ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMReferenceSetIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMReferenceSetIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSetIterator<UniqueIdentification,
                            ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMReferenceSet>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
size_t OMReferenceSetIterator<UniqueIdentification,
                              ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMReferenceSetIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMReferenceSet> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMReferenceSetIterator::valid> becomes <e bool.true>
  //        and <mf OMReferenceSetIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMReferenceSet> is reached then the result
  //        is <e bool.false>, <mf OMReferenceSetIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMReferenceSetIterator::after> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMReferenceSetIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSetIterator<UniqueIdentification,
                            ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMReferenceSetIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMReferenceSet> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMReferenceSetIterator::valid> becomes <e bool.true>
  //        and <mf OMReferenceSetIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMReferenceSet> is reached then the result
  //        is <e bool.false>, <mf OMReferenceSetIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMReferenceSetIterator::before> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMReferenceSetIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMReferenceSetIterator<UniqueIdentification,
                            ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMReferenceSet> at the position currently
  //        designated by this <c OMReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject* OMReferenceSetIterator<UniqueIdentification,
                                         ReferencedObject>::value(void) const
{
  TRACE("OMReferenceSetIterator<UniqueIdentification, "
                                                   "ReferencedObject>::value");

  const SetElement& element = _iterator.value();

  ReferencedObject* result = element.getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMReferenceSet> at the position currently
  //        designated by this <c OMReferenceSetIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned. To preserve the ordering of <p Key>s, the
  //        <p Key> of <p newObject> must be the same as that of the
  //        existing <p ReferencedObject>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject* OMReferenceSetIterator<UniqueIdentification,
                                         ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMReferenceSetIterator<UniqueIdentification, "
                                                "ReferencedObject>::setValue");

  PRECONDITION("Matching keys",
    IMPLIES(newObject != 0 , newObject->identification() == identification()));

  SetElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(newObject);

  return result;
}

  // @mfunc Return the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <c OMObject> at the current position.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
OMObject* OMReferenceSetIterator<UniqueIdentification,
                                 ReferencedObject>::currentObject(void) const
{
  TRACE("OMReferenceSetIterator<UniqueIdentification, "
                               "ReferencedObject>::currentObject");

  OMObject* result = value();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Clear (set to 0) the <p OMObject> in the associated
  //        reference container at the position currently
  //        designated by this <c OMReferenceSetIterator>.
  //        The existing object, if any, is returned. The associated
  //        reference container is not modified in that no entry is
  //        removed, the existing entry remains but no longer refers
  //        to a valid object.
  //   @rdesc The previous <c OMObject> at the current position.
template <typename UniqueIdentification, typename ReferencedObject>
OMObject* OMReferenceSetIterator<UniqueIdentification,
                                 ReferencedObject>::clearObject(void)
{
  TRACE("OMReferenceSetIterator<UniqueIdentification, "
                               "ReferencedObject>::clearObject");

  OMObject* result = 0;
  ASSERT("Unimplemented code not reached", false); // tjb
  return result;
}

  // @mfunc Return the <p Key> of the <p ReferencedObject> in the
  //        associated <c OMReferenceSet> at the position
  //        currently designated by this <c OMReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
UniqueIdentification
OMReferenceSetIterator<UniqueIdentification,
                       ReferencedObject>::identification(void) const
{
  TRACE("OMReferenceSetIterator<UniqueIdentification, "
                                          "ReferencedObject>::identification");

  return _iterator.key();
}

  // @mfunc Create an <c OMReferenceSetIterator> given
  //        an underlying <c OMSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The underlying <c OMSetIterator>.
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceSetIterator<UniqueIdentification,
                       ReferencedObject>::OMReferenceSetIterator(
                                                       const SetIterator& iter)
  : _iterator(iter) // probably bitwise
{
}
