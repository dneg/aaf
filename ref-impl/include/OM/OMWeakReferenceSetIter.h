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
#ifndef OMWEAKREFERENCESETITER_H
#define OMWEAKREFERENCESETITER_H

#include "OMSetIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename ReferencedObject>
class OMWeakReferenceSetProperty;

// @class Iterators over <c OMWeakReferenceSetProperty>s.
//   @tcarg class | ReferencedObject | The type of the contained objects.
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

    // @cmember Return the <p Key> of the <p ReferencedObject> in the
    //          associated <c OMWeakReferenceSetProperty> at the position
    //          currently designated by this <c OMWeakReferenceSetIterator>.
   OMUniqueObjectIdentification identification(void) const;

    // @cmember Return the <p OMObject> in the associated
    //          reference container property at the position currently
    //          designated by this <c OMWeakReferenceSetIterator>.
   virtual OMObject* currentObject(void) const;

protected:

  typedef OMWeakReferenceSetElement<ReferencedObject> SetElement;

  typedef OMSetIterator<OMUniqueObjectIdentification, SetElement> SetIterator;

    // @cmember Create an <c OMWeakReferenceSetIterator> given
    //          an underlying <c OMSetIterator>.
  OMWeakReferenceSetIterator(const SetIterator& iter);

private:

  SetIterator _iterator;

};

#include "OMWeakReferenceSetIterT.h"

#endif
