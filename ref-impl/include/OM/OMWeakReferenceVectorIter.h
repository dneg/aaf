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
#ifndef OMWEAKREFERENCEVECTORITER_H
#define OMWEAKREFERENCEVECTORITER_H

#include "OMVectorIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMWeakReferenceVectorProperty;

// @class Iterators over <c OMWeakReferenceVectorProperty>s.
//   @tcarg class | ReferencedObject | The type of the contained objects.
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

protected:

  typedef OMWeakReferenceVectorElement<ReferencedObject> VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // @cmember Create an <c OMWeakReferenceVectorIterator> given
    //          an underlying <c OMVectorIterator>.
  OMWeakReferenceVectorIterator(const VectorIterator& iter);

private:

  VectorIterator _iterator;

};

#include "OMWeakReferenceVectorIterT.h"

#endif
