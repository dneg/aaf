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
#ifndef OMREFERENCESETITER_H
#define OMREFERENCESETITER_H

#include "OMSetIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename UniqueIdentification, typename ReferencedObject>
class OMReferenceSet;

  // @class Iterators over <c OMReferenceSet>s.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename UniqueIdentification, typename ReferencedObject>
class OMReferenceSetIterator : public OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMReferenceSetIterator> over the given
    //          <c OMReferenceSet> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMReferenceSetIterator> is made ready to traverse the
    //          associated <c OMReferenceSet> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMReferenceSetIterator> is made ready to traverse the
    //          associated <c OMReferenceSet> in the reverse
    //          direction (decreasing <p Key>s).
  OMReferenceSetIterator(const OMReferenceSet<UniqueIdentification,
                                              ReferencedObject>& set,
                         OMIteratorPosition initialPosition = OMBefore);

    // @cmember Destroy this <c OMReferenceSetIterator>.
  virtual ~OMReferenceSetIterator(void);

    // @cmember Create a copy of this <c OMReferenceSetIterator>.
  virtual OMReferenceContainerIterator* copy(void) const;

    // @cmember Reset this <c OMReferenceSetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMReferenceSetIterator> is made ready to traverse the
    //          associated <c OMReferenceSet> in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMReferenceSetIterator> is made ready to traverse the
    //          associated <c OMReferenceSet> in the reverse
    //          direction (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMReferenceSetIterator> positioned before
    //          the first <p ReferencedObject> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMReferenceSetIterator> positioned after
    //          the last <p ReferencedObject> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMReferenceSetIterator> validly
    //          positioned on a <p ReferencedObject> ?
  virtual bool valid(void) const;

    // @cmember The number of <p ReferencedObject>s in the associated
    //          <c OMReferenceSet>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMReferenceSetIterator> to the next
    //          <p ReferencedObject>, if any.
    //          If the end of the associated
    //          <c OMReferenceSet> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMReferenceSetIterator::valid> becomes <e bool.true>
    //          and <mf OMReferenceSetIterator::after> becomes
    //          <e bool.false>.
    //          If the end of the associated
    //          <c OMReferenceSet> is reached then the result
    //          is <e bool.false>, <mf OMReferenceSetIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMReferenceSetIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMReferenceSetIterator> to the previous
    //          <p ReferencedObject>, if any.
    //          If the beginning of the associated
    //          <c OMReferenceSet> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMReferenceSetIterator::valid> becomes <e bool.true>
    //          and <mf OMReferenceSetIterator::before> becomes
    //          <e bool.false>.
    //          If the beginning of the associated
    //          <c OMReferenceSet> is reached then the result
    //          is <e bool.false>, <mf OMReferenceSetIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMReferenceSetIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p ReferencedObject> in the associated
    //          <c OMReferenceSet> at the position currently
    //          designated by this <c OMReferenceSetIterator>.
  virtual ReferencedObject* value(void) const;

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMReferenceSet> at the position currently
    //          designated by this <c OMReferenceSetIterator> to
    //          <p newObject>. The previous <p ReferencedObject>, if any,
    //          is returned. To preserve the ordering of <p Key>s, the
    //          <p Key> of <p newObject> must be the same as that of the
    //          existing <p ReferencedObject>.
  virtual ReferencedObject* setValue(const ReferencedObject* newObject);

    // @cmember Return the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMReferenceSetIterator>.
  virtual OMObject* currentObject(void) const;

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMReferenceSetIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void);

    // @cmember Return the <p Key> of the <p ReferencedObject> in the
    //          associated <c OMReferenceSet> at the position
    //          currently designated by this <c OMReferenceSetIterator>.
  UniqueIdentification identification(void) const;

protected:

  typedef OMSetElement<UniqueIdentification, ReferencedObject> SetElement;

  typedef OMSetIterator<UniqueIdentification, SetElement> SetIterator;

    // @cmember Create an <c OMReferenceSetIterator> given
    //          an underlying <c OMSetIterator>.
  OMReferenceSetIterator(const SetIterator& iter);

private:

  SetIterator _iterator;

};

#include "OMReferenceSetIterT.h"

#endif
