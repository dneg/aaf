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
OMReferenceContainerIterator<ReferencedObject>*
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
