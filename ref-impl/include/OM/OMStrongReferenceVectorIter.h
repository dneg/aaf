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
#ifndef OMSTRONGREFERENCEVECTORITER_H
#define OMSTRONGREFERENCEVECTORITER_H

#include "OMVectorIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMStrongReferenceVectorProperty;

  // @class Iterators over <c OMStrongReferenceVectorProperty>s.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @base public | <c OMReferenceContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMStrongReferenceVectorIterator : public OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMStrongReferenceVectorIterator> over the given
    //          <c OMStrongReferenceVectorProperty> <p vector> and
    //          initialize it to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMStrongReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMStrongReferenceVectorProperty> in the
    //          forward direction (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMStrongReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMStrongReferenceVectorProperty> in the
    //          reverse direction (decreasing indexes).
  OMStrongReferenceVectorIterator(
               const OMStrongReferenceVectorProperty<ReferencedObject>& vector,
               OMIteratorPosition initialPosition = OMBefore);

    // @cmember Create a copy of this <c OMStrongReferenceVectorIterator>.
  virtual OMReferenceContainerIterator* copy(void) const;

    // @cmember Destroy this <c OMStrongReferenceVectorIterator>.
  virtual ~OMStrongReferenceVectorIterator(void);

    // @cmember Reset this <c OMStrongReferenceVectorIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMStrongReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMStrongReferenceVectorProperty> in the
    //          forward direction (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMStrongReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMStrongReferenceVectorProperty> in the
    //          reverse direction (decreasing indexes).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMStrongReferenceVectorIterator> positioned before
    //          the first <p ReferencedObject> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMStrongReferenceVectorIterator> positioned after
    //          the last <p ReferencedObject> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMStrongReferenceVectorIterator> validly
    //          positioned on a <p ReferencedObject> ?
  virtual bool valid(void) const;

    // @cmember The number of <p ReferencedObject>s in the associated
    //          <c OMStrongReferenceVectorProperty>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMStrongReferenceVectorIterator> to the
    //          next <p ReferencedObject>, if any.
    //          If the end of the associated
    //          <c OMStrongReferenceVectorProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMStrongReferenceVectorIterator::valid> becomes
    //          <e bool.true> and <mf OMStrongReferenceVectorIterator::after>
    //          becomes <e bool.false>.
    //          If the end of the associated
    //          <c OMStrongReferenceVectorProperty> is reached then the result
    //          is <e bool.false>, <mf OMStrongReferenceVectorIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMStrongReferenceVectorIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMStrongReferenceVectorIterator> to the
    //          previous <p ReferencedObject>, if any.
    //          If the beginning of the associated
    //          <c OMStrongReferenceVectorProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMStrongReferenceVectorIterator::valid> becomes
    //          <e bool.true> and <mf OMStrongReferenceVectorIterator::before>
    //          becomes <e bool.false>.
    //          If the beginning of the associated
    //          <c OMStrongReferenceVectorProperty> is reached then the result
    //          is <e bool.false>, <mf OMStrongReferenceVectorIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMStrongReferenceVectorIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p ReferencedObject> in the associated
    //          <c OMStrongReferenceVectorProperty> at the position currently
    //          designated by this <c OMStrongReferenceVectorIterator>.
  virtual ReferencedObject* value(void) const;

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMStrongReferenceVectorProperty> at the position currently
    //          designated by this <c OMStrongReferenceVectorIterator> to
    //          <p newObject>. The previous <p ReferencedObject>, if any,
    //          is returned.
  virtual ReferencedObject* setValue(const ReferencedObject* newObject);

    // @cmember Set the <p Element> in the associated <c OMContainer> at the
    //          position currently designated by this
    //          <c OMStrongReferenceVectorIterator> to 0.
    //          The previous <p ReferencedObject>, if any is returned.
  virtual ReferencedObject* clearValue(void);

    // @cmember Return the index of the <p ReferencedObject> in the
    //          associated <c OMStrongReferenceVectorProperty> at the position
    //          currently designated by this
    //          <c OMStrongReferenceVectorIterator>.
  virtual size_t index(void) const;

    // @cmember Return the <p OMObject> in the associated
    //          reference container property at the position currently
    //          designated by this <c OMStrongReferenceVectorIterator>.
  virtual OMObject* currentObject(void) const;

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMStrongReferenceVectorIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void);

protected:

  typedef OMStrongReferenceVectorElement VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // @cmember Create an <c OMStrongReferenceVectorIterator> given
    //          an underlying <c OMVectorIterator>.
  OMStrongReferenceVectorIterator(const VectorIterator& iter);

private:

  VectorIterator _iterator;

};

#include "OMStrongReferenceVectorIterT.h"

#endif
