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
#ifndef OMSTRONGREFERENCEVECTORITER_H
#define OMSTRONGREFERENCEVECTORITER_H

#include "OMVectorIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMStrongReferenceVectorProperty;

// @class Iterators over <c OMStrongReferenceVectorProperty>s.
//   @tcarg class | ReferencedObject | The type of the contained objects.
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

protected:

  typedef OMStrongReferenceVectorElement<ReferencedObject> VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // @cmember Create an <c OMStrongReferenceVectorIterator> given
    //          an underlying <c OMVectorIterator>.
  OMStrongReferenceVectorIterator(const VectorIterator& iter);

private:

  VectorIterator _iterator;

};

#include "OMStrongReferenceVectorIterT.h"

#endif
