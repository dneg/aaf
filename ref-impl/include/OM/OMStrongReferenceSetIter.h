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
#ifndef OMSTRONGREFERENCESETITER_H
#define OMSTRONGREFERENCESETITER_H

#include "OMSetIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetProperty;

  // @class Iterators over <c OMStrongReferenceSetProperty>s.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @base public | <c OMReferenceContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetIterator : public OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMStrongReferenceSetIterator> over the given
    //          <c OMStrongReferenceSetProperty> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMStrongReferenceSetIterator> is made ready to traverse the
    //          associated <c OMStrongReferenceSetProperty> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMStrongReferenceSetIterator> is made ready to traverse the
    //          associated <c OMStrongReferenceSetProperty> in the reverse
    //          direction (decreasing <p Key>s).
  OMStrongReferenceSetIterator(
                     const OMStrongReferenceSetProperty<UniqueIdentification,
                                                        ReferencedObject>& set,
                     OMIteratorPosition initialPosition = OMBefore);

    // @cmember Destroy this <c OMStrongReferenceSetIterator>.
  virtual ~OMStrongReferenceSetIterator(void);

    // @cmember Create a copy of this <c OMStrongReferenceSetIterator>.
  virtual OMReferenceContainerIterator* copy(void) const;

    // @cmember Reset this <c OMStrongReferenceSetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMStrongReferenceSetIterator> is made ready to traverse the
    //          associated <c OMStrongReferenceSetProperty> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMStrongReferenceSetIterator> is made ready to traverse the
    //          associated <c OMStrongReferenceSetProperty> in the reverse
    //          direction (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMStrongReferenceSetIterator> positioned before
    //          the first <p ReferencedObject> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMStrongReferenceSetIterator> positioned after
    //          the last <p ReferencedObject> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMStrongReferenceSetIterator> validly
    //          positioned on a <p ReferencedObject> ?
  virtual bool valid(void) const;

    // @cmember The number of <p ReferencedObject>s in the associated
    //          <c OMStrongReferenceSetProperty>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMStrongReferenceSetIterator> to the next
    //          <p ReferencedObject>, if any.
    //          If the end of the associated
    //          <c OMStrongReferenceSetProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMStrongReferenceSetIterator::valid> becomes <e bool.true>
    //          and <mf OMStrongReferenceSetIterator::after> becomes
    //          <e bool.false>.
    //          If the end of the associated
    //          <c OMStrongReferenceSetProperty> is reached then the result
    //          is <e bool.false>, <mf OMStrongReferenceSetIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMStrongReferenceSetIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMStrongReferenceSetIterator> to the previous
    //          <p ReferencedObject>, if any.
    //          If the beginning of the associated
    //          <c OMStrongReferenceSetProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMStrongReferenceSetIterator::valid> becomes <e bool.true>
    //          and <mf OMStrongReferenceSetIterator::before> becomes
    //          <e bool.false>.
    //          If the beginning of the associated
    //          <c OMStrongReferenceSetProperty> is reached then the result
    //          is <e bool.false>, <mf OMStrongReferenceSetIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMStrongReferenceSetIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p ReferencedObject> in the associated
    //          <c OMStrongReferenceSetProperty> at the position currently
    //          designated by this <c OMStrongReferenceSetIterator>.
  virtual ReferencedObject* value(void) const;

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMStrongReferenceSetProperty> at the position currently
    //          designated by this <c OMStrongReferenceSetIterator> to
    //          <p newObject>. The previous <p ReferencedObject>, if any,
    //          is returned. To preserve the ordering of <p Key>s, the
    //          <p Key> of <p newObject> must be the same as that of the
    //          existing <p ReferencedObject>.
  virtual ReferencedObject* setValue(const ReferencedObject* newObject);

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMStrongReferenceSetProperty> at the position currently
    //          designated by this <c OMStrongReferenceSetIterator> to 0.
    //          The previous <p ReferencedObject>, if any, is returned.
  virtual ReferencedObject* clearValue(void);

    // @cmember Return the <p Key> of the <p ReferencedObject> in the
    //          associated <c OMStrongReferenceSetProperty> at the position
    //          currently designated by this <c OMStrongReferenceSetIterator>.
  UniqueIdentification identification(void) const;

    // @cmember Return the <p OMObject> in the associated
    //          reference container property at the position currently
    //          designated by this <c OMStrongReferenceSetIterator>.
  virtual OMObject* currentObject(void) const;

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMStrongReferenceSetIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void);

protected:

  typedef OMStrongReferenceSetElement SetElement;

  typedef OMSetIterator<UniqueIdentification, SetElement> SetIterator;

    // @cmember Create an <c OMStrongReferenceSetIterator> given
    //          an underlying <c OMSetIterator>.
  OMStrongReferenceSetIterator(const SetIterator& iter);

private:

  SetIterator _iterator;

};

#include "OMStrongReferenceSetIterT.h"

#endif
