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
#ifndef OMWEAKREFERENCEVECTORITER_H
#define OMWEAKREFERENCEVECTORITER_H

#include "OMVectorIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMWeakReferenceVectorProperty;

  // @class Iterators over <c OMWeakReferenceVectorProperty>s.
  //   @tcarg class | ReferencedObject | The type of the contained objects.
  //   @base public | <c OMReferenceContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakReferenceVectorIterator : public OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMWeakReferenceVectorIterator> over the given
    //          <c OMWeakReferenceVectorProperty> <p vector> and
    //          initialize it to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMWeakReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMWeakReferenceVectorProperty> in the
    //          forward direction (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMWeakReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMWeakReferenceVectorProperty> in the
    //          reverse direction (decreasing indexes).
  OMWeakReferenceVectorIterator(
                 const OMWeakReferenceVectorProperty<ReferencedObject>& vector,
                 OMIteratorPosition initialPosition = OMBefore);

    // @cmember Create a copy of this <c OMWeakReferenceVectorIterator>.
  virtual OMReferenceContainerIterator* copy(void) const;

    // @cmember Destroy this <c OMWeakReferenceVectorIterator>.
  virtual ~OMWeakReferenceVectorIterator(void);

    // @cmember Reset this <c OMWeakReferenceVectorIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMWeakReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMWeakReferenceVectorProperty> in the
    //          forward direction (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMWeakReferenceVectorIterator> is made ready to traverse
    //          the associated <c OMWeakReferenceVectorProperty> in the
    //          reverse direction (decreasing indexes).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore);

    // @cmember Is this <c OMWeakReferenceVectorIterator> positioned before
    //          the first <p ReferencedObject> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMWeakReferenceVectorIterator> positioned after
    //          the last <p ReferencedObject> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMWeakReferenceVectorIterator> validly
    //          positioned on a <p ReferencedObject> ?
  virtual bool valid(void) const;

    // @cmember The number of <p ReferencedObject>s in the associated
    //          <c OMWeakReferenceVectorProperty>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMWeakReferenceVectorIterator> to the
    //          next <p ReferencedObject>, if any.
    //          If the end of the associated
    //          <c OMWeakReferenceVectorProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMWeakReferenceVectorIterator::valid> becomes
    //          <e bool.true> and <mf OMWeakReferenceVectorIterator::after>
    //          becomes <e bool.false>.
    //          If the end of the associated
    //          <c OMWeakReferenceVectorProperty> is reached then the result
    //          is <e bool.false>, <mf OMWeakReferenceVectorIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMWeakReferenceVectorIterator::after> becomes
    //          <e bool.true>. 
  virtual bool operator++();

    // @cmember Retreat this <c OMWeakReferenceVectorIterator> to the
    //          previous <p ReferencedObject>, if any.
    //          If the beginning of the associated
    //          <c OMWeakReferenceVectorProperty> is not reached then the
    //          result is <e bool.true>,
    //          <mf OMWeakReferenceVectorIterator::valid> becomes
    //          <e bool.true> and <mf OMWeakReferenceVectorIterator::before>
    //          becomes <e bool.false>.
    //          If the beginning of the associated
    //          <c OMWeakReferenceVectorProperty> is reached then the result
    //          is <e bool.false>, <mf OMWeakReferenceVectorIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMWeakReferenceVectorIterator::before> becomes
    //          <e bool.true>. 
  virtual bool operator--();

    // @cmember Return the <p ReferencedObject> in the associated
    //          <c OMWeakReferenceVectorProperty> at the position currently
    //          designated by this <c OMWeakReferenceVectorIterator>.
  virtual ReferencedObject* value(void) const;

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMWeakReferenceVectorProperty> at the position currently
    //          designated by this <c OMWeakReferenceVectorIterator> to
    //          <p newObject>. The previous <p ReferencedObject>, if any,
    //          is returned.
  virtual ReferencedObject* setValue(const ReferencedObject* newObject);

    // @cmember Set the <p ReferencedObject> in the associated
    //          <c OMWeakReferenceVectorProperty> at the position currently
    //          designated by this <c OMWeakReferenceVectorIterator> to 0.
    //          The previous <p ReferencedObject>, if any, is returned.
  virtual ReferencedObject* clearValue(void);

    // @cmember Return the index of the <p ReferencedObject> in the
    //          associated <c OMWeakReferenceVectorProperty> at the position
    //          currently designated by this
    //          <c OMWeakReferenceVectorIterator>.
  virtual size_t index(void) const;

    // @cmember Return the <p Key> of the <p ReferencedObject> in the
    //          associated <c OMWeakReferenceVectorProperty> at the position
    //          currently designated by this <c OMWeakReferenceVectorIterator>.
  OMUniqueObjectIdentification identification(void) const;

    // @cmember Return the <p OMObject> in the associated
    //          reference container property at the position currently
    //          designated by this <c OMWeakReferenceVectorIterator>.
  virtual OMObject* currentObject(void) const;

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMWeakReferenceVectorIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void);

protected:

  typedef OMWeakReferenceVectorElement VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // @cmember Create an <c OMWeakReferenceVectorIterator> given
    //          an underlying <c OMVectorIterator>.
  OMWeakReferenceVectorIterator(const VectorIterator& iter);

private:

  VectorIterator _iterator;

};

#include "OMWeakReferenceVectorIterT.h"

#endif
