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
#ifndef OMREFERENCEVECTORITER_H
#define OMREFERENCEVECTORITER_H

#include "OMVectorIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMReferenceVector;

  // @class Iterators over <c OMReferenceVector>s.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMReferenceVectorIterator : public OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMReferenceVectorIterator> over the given
    //          <c OMReferenceVector> <p vector> and
    //          initialize it to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMReferenceVector> in the
    //          forward direction (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMReferenceVector> in the
    //          reverse direction (decreasing indexes).
  OMReferenceVectorIterator(
                             const OMReferenceVector<ReferencedObject>& vector,
                             OMIteratorPosition initialPosition = OMBefore);

    // @cmember Create a copy of this <c OMReferenceVectorIterator>.
  virtual OMReferenceContainerIterator* copy(void) const;

    // @cmember Destroy this <c OMReferenceVectorIterator>.
  virtual ~OMReferenceVectorIterator(void);

    // @cmember Reset this <c OMReferenceVectorIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMReferenceVector> in the
    //          forward direction (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMReferenceVector> in the
    //          reverse direction (decreasing indexes).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMReferenceVectorIterator> positioned before
    //          the first <p ReferencedObject> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMReferenceVectorIterator> positioned after
    //          the last <p ReferencedObject> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMReferenceVectorIterator> validly
    //          positioned on a <p ReferencedObject> ?
  virtual bool valid(void) const;

    // @cmember The number of <p ReferencedObject>s in the associated
    //          <c OMReferenceVector>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMReferenceVectorIterator> to the
    //          next <p ReferencedObject>, if any.
    //          If the end of the associated
    //          <c OMReferenceVector> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMReferenceVectorIterator::valid> becomes
    //          <e bool.true> and <mf OMReferenceVectorIterator::after>
    //          becomes <e bool.false>.
    //          If the end of the associated
    //          <c OMReferenceVector> is reached then the result
    //          is <e bool.false>, <mf OMReferenceVectorIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMReferenceVectorIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMReferenceVectorIterator> to the
    //          previous <p ReferencedObject>, if any.
    //          If the beginning of the associated
    //          <c OMReferenceVector> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMReferenceVectorIterator::valid> becomes
    //          <e bool.true> and <mf OMReferenceVectorIterator::before>
    //          becomes <e bool.false>.
    //          If the beginning of the associated
    //          <c OMReferenceVector> is reached then the result
    //          is <e bool.false>, <mf OMReferenceVectorIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMReferenceVectorIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p ReferencedObject> in the associated
    //          <c OMReferenceVector> at the position currently
    //          designated by this <c OMReferenceVectorIterator>.
  virtual ReferencedObject* value(void) const;

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMReferenceVector> at the position currently
    //          designated by this <c OMReferenceVectorIterator> to
    //          <p newObject>. The previous <p ReferencedObject>, if any,
    //          is returned.
  virtual ReferencedObject* setValue(const ReferencedObject* newObject);

    // @cmember Return the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMReferenceVectorIterator>.
  virtual OMObject* currentObject(void) const;

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMReferenceVectorIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void);

    // @cmember Return the index of the <p ReferencedObject> in the
    //          associated <c OMReferenceVector> at the position
    //          currently designated by this
    //          <c OMReferenceVectorIterator>.
  virtual size_t index(void) const;

protected:

  typedef OMVectorElement<ReferencedObject> VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // @cmember Create an <c OMReferenceVectorIterator> given
    //          an underlying <c OMVectorIterator>.
  OMReferenceVectorIterator(const VectorIterator& iter);

private:

  VectorIterator _iterator;

};

#include "OMReferenceVectorIterT.h"

#endif
