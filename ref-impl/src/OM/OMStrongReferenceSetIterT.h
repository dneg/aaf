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

  // @mfunc Create an <c OMStrongReferenceSetIterator> over the given
  //        <c OMStrongReferenceSetProperty> <p set> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMStrongReferenceSetIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceSetProperty> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMStrongReferenceSetIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceSetProperty> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The <c OMStrongReferenceSet> over which to iterate.
  //   @parm The initial position for this <c OMStrongReferenceSetIterator>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::OMStrongReferenceSetIterator(
                     const OMStrongReferenceSetProperty<UniqueIdentification,
                                                        ReferencedObject>& set,
                     OMIteratorPosition initialPosition)
  : _iterator(set._set, initialPosition)
{
}

  // @mfunc Destroy this <c OMStrongReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::~OMStrongReferenceSetIterator(
                                                                          void)
{
}

  // @mfunc Create a copy of this <c OMStrongReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The new <c OMStrongReferenceSetIterator>.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
OMReferenceContainerIterator<ReferencedObject>*
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::copy(void) const
{
  TRACE("OMStrongReferenceSetIterator<UniqueIdentification, "
                                     "ReferencedObject>::copy");

  OMStrongReferenceSetIterator<UniqueIdentification, ReferencedObject>*
        result = new OMStrongReferenceSetIterator<UniqueIdentification,
                                                  ReferencedObject>(_iterator);

  return result;
}

  // @mfunc Reset this <c OMStrongReferenceSetIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMStrongReferenceSetIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceSetProperty> in the forward
  //        direction (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMStrongReferenceSetIterator> is made ready to traverse the
  //        associated <c OMStrongReferenceSetProperty> in the reverse
  //        direction (decreasing <p Key>s).
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The position to which this <c OMStrongReferenceSetIterator>
  //         should be reset.
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::reset(
                                            OMIteratorPosition initialPosition)
{
  _iterator.reset(initialPosition);
}

  // @mfunc Is this <c OMStrongReferenceSetIterator> positioned before
  //        the first <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMStrongReferenceSetIterator> is
  //          positioned before the first <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::before(void) const
{
  return _iterator.before();
}

  // @mfunc Is this <c OMStrongReferenceSetIterator> positioned after
  //        the last <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMStrongReferenceSetIterator> is
  //          positioned after the last <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::after(void) const
{
  return _iterator.after();
}

  // @mfunc Is this <c OMStrongReferenceSetIterator> validly
  //        positioned on a <p ReferencedObject> ?
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.true> if this <c OMStrongReferenceSetIterator> is
  //          positioned on a <p ReferencedObject>, <e bool.false>
  //          otherwise.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::valid(void) const
{
  return _iterator.valid();
}

  // @mfunc The number of <p ReferencedObject>s in the associated
  //        <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The number of <p ReferencedObject>s
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
size_t
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::count(void) const
{
  return _iterator.count();
}

  // @mfunc Advance this <c OMStrongReferenceSetIterator> to the next
  //        <p ReferencedObject>, if any.
  //        If the end of the associated
  //        <c OMStrongReferenceSetProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMStrongReferenceSetIterator::valid> becomes <e bool.true>
  //        and <mf OMStrongReferenceSetIterator::after> becomes
  //        <e bool.false>.
  //        If the end of the associated
  //        <c OMStrongReferenceSetProperty> is reached then the result
  //        is <e bool.false>, <mf OMStrongReferenceSetIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMStrongReferenceSetIterator::after> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMStrongReferenceSetIterator> has
  //          passed the last <p ReferencedObject>, <e bool.true> otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::operator++()
{
  return ++_iterator;
}

  // @mfunc Retreat this <c OMStrongReferenceSetIterator> to the previous
  //        <p ReferencedObject>, if any.
  //        If the beginning of the associated
  //        <c OMStrongReferenceSetProperty> is not reached then the
  //        result is <e bool.true>,
  //        <mf OMStrongReferenceSetIterator::valid> becomes <e bool.true>
  //        and <mf OMStrongReferenceSetIterator::before> becomes
  //        <e bool.false>.
  //        If the beginning of the associated
  //        <c OMStrongReferenceSetProperty> is reached then the result
  //        is <e bool.false>, <mf OMStrongReferenceSetIterator::valid>
  //        becomes <e bool.false> and
  //        <mf OMStrongReferenceSetIterator::before> becomes
  //        <e bool.true>. 
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc <e bool.false> if this <c OMStrongReferenceSetIterator> has
  //          passed the first <p ReferencedObject>, <e bool.true> otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::operator--()
{
  return --_iterator;
}

  // @mfunc Return the <p ReferencedObject> in the associated
  //        <c OMStrongReferenceSetProperty> at the position currently
  //        designated by this <c OMStrongReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p ReferencedObject> at the current position.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::value(void) const
{
  TRACE("OMStrongReferenceSetIterator<UniqueIdentification, "
                                     "ReferencedObject>::value");

  const SetElement& element = _iterator.value();

  ReferencedObject* result = element.getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Set the <p ReferencedObject> in the associated
  //        <c OMStrongReferenceSetProperty> at the position currently
  //        designated by this <c OMStrongReferenceSetIterator> to
  //        <p newObject>. The previous <p ReferencedObject>, if any,
  //        is returned. To preserve the ordering of <p Key>s, the
  //        <p Key> of <p newObject> must be the same as that of the
  //        existing <p ReferencedObject>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The new <p ReferencedObject>.
  //   @rdesc The previous <p ReferencedObject> if any, otherwise 0.
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::setValue(
                                             const ReferencedObject* newObject)
{
  TRACE("OMStrongReferenceSetIterator<UniqueIdentification, "
                                     "ReferencedObject>::setValue");

  PRECONDITION("Matching keys",
    IMPLIES(newObject != 0 , newObject->identification() == identification()));

  SetElement& element = _iterator.value();

  ReferencedObject* result = element.setValue(newObject);

  return result;
}

  // @mfunc Return the <p Key> of the <p ReferencedObject> in the
  //        associated <c OMStrongReferenceSetProperty> at the position
  //        currently designated by this <c OMStrongReferenceSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
UniqueIdentification
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::identification(void) const
{
  TRACE("OMStrongReferenceSetIterator<UniqueIdentification, "
                                     "ReferencedObject>::identification");

  return _iterator.key();
}

  // @mfunc Create an <c OMStrongReferenceSetIterator> given
  //        an underlying <c OMSetIterator>.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @parm The underlying <c OMSetIterator>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetIterator<UniqueIdentification,
                             ReferencedObject>::OMStrongReferenceSetIterator(
                                                       const SetIterator& iter)
  : _iterator(iter) // probably bitwise
{
}
