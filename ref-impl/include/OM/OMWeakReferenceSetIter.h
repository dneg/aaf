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
#ifndef OMWEAKREFERENCESETITER_H
#define OMWEAKREFERENCESETITER_H

#include "OMSetIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMWeakReferenceSetProperty;

  // @class Iterators over <c OMWeakReferenceSetProperty>s.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @base public | <c OMReferenceContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakReferenceSetIterator : public OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMWeakReferenceSetIterator> over the given
    //          <c OMWeakReferenceSetProperty> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMWeakReferenceSetIterator> is made ready to traverse the
    //          associated <c OMWeakReferenceSetProperty> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMWeakReferenceSetIterator> is made ready to traverse the
    //          associated <c OMWeakReferenceSetProperty> in the reverse
    //          direction (decreasing <p Key>s).
  OMWeakReferenceSetIterator(
                       const OMWeakReferenceSetProperty<ReferencedObject>& set,
                       OMIteratorPosition initialPosition = OMBefore);

    // @cmember Destroy this <c OMWeakReferenceSetIterator>.
  virtual ~OMWeakReferenceSetIterator(void);

    // @cmember Create a copy of this <c OMWeakReferenceSetIterator>.
  virtual OMReferenceContainerIterator* copy(void) const;

    // @cmember Reset this <c OMWeakReferenceSetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMWeakReferenceSetIterator> is made ready to traverse the
    //          associated <c OMWeakReferenceSetProperty> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMWeakReferenceSetIterator> is made ready to traverse the
    //          associated <c OMWeakReferenceSetProperty> in the reverse
    //          direction (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMWeakReferenceSetIterator> positioned before
    //          the first <p ReferencedObject> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMWeakReferenceSetIterator> positioned after
    //          the last <p ReferencedObject> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMWeakReferenceSetIterator> validly
    //          positioned on a <p ReferencedObject> ?
  virtual bool valid(void) const;

    // @cmember The number of <p ReferencedObject>s in the associated
    //          <c OMWeakReferenceSetProperty>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMWeakReferenceSetIterator> to the next
    //          <p ReferencedObject>, if any.
    //          If the end of the associated
    //          <c OMWeakReferenceSetProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMWeakReferenceSetIterator::valid> becomes <e bool.true>
    //          and <mf OMWeakReferenceSetIterator::after> becomes
    //          <e bool.false>.
    //          If the end of the associated
    //          <c OMWeakReferenceSetProperty> is reached then the result
    //          is <e bool.false>, <mf OMWeakReferenceSetIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMWeakReferenceSetIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMWeakReferenceSetIterator> to the previous
    //          <p ReferencedObject>, if any.
    //          If the beginning of the associated
    //          <c OMWeakReferenceSetProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMWeakReferenceSetIterator::valid> becomes <e bool.true>
    //          and <mf OMWeakReferenceSetIterator::before> becomes
    //          <e bool.false>.
    //          If the beginning of the associated
    //          <c OMWeakReferenceSetProperty> is reached then the result
    //          is <e bool.false>, <mf OMWeakReferenceSetIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMWeakReferenceSetIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p ReferencedObject> in the associated
    //          <c OMWeakReferenceSetProperty> at the position currently
    //          designated by this <c OMWeakReferenceSetIterator>.
  virtual ReferencedObject* value(void) const;

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMWeakReferenceSetProperty> at the position currently
    //          designated by this <c OMWeakReferenceSetIterator> to
    //          <p newObject>. The previous <p ReferencedObject>, if any,
    //          is returned. To preserve the ordering of <p Key>s, the
    //          <p Key> of <p newObject> must be the same as that of the
    //          existing <p ReferencedObject>.
  virtual ReferencedObject* setValue(const ReferencedObject* newObject);

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMWeakReferenceSetProperty> at the position currently
    //          designated by this <c OMWeakReferenceSetIterator> to 0.
    //          The previous <p ReferencedObject>, if any, is returned.
  virtual ReferencedObject* clearValue(void);

    // @cmember Return the <p Key> of the <p ReferencedObject> in the
    //          associated <c OMWeakReferenceSetProperty> at the position
    //          currently designated by this <c OMWeakReferenceSetIterator>.
  OMUniqueObjectIdentification identification(void) const;

    // @cmember Return the <p OMObject> in the associated
    //          reference container property at the position currently
    //          designated by this <c OMWeakReferenceSetIterator>.
  virtual OMObject* currentObject(void) const;

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMWeakReferenceSetIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void);

protected:

  typedef OMWeakReferenceSetElement SetElement;

  typedef OMSetIterator<OMUniqueObjectIdentification, SetElement> SetIterator;

    // @cmember Create an <c OMWeakReferenceSetIterator> given
    //          an underlying <c OMSetIterator>.
  OMWeakReferenceSetIterator(const SetIterator& iter);

private:

  SetIterator _iterator;

};

#include "OMWeakReferenceSetIterT.h"

#endif
