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
#ifndef OMSTRONGREFERENCESETITER_H
#define OMSTRONGREFERENCESETITER_H

#include "OMSetIterator.h"
#include "OMReferenceContainerIter.h"
#include "OMContainerElement.h"

template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetProperty;

// @class Iterators over <c OMStrongReferenceSetProperty>s.
//   @tcarg class | ReferencedObject | The type of the contained objects.
template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetIterator :
                        public OMReferenceContainerIterator<ReferencedObject> {
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
  virtual OMReferenceContainerIterator<ReferencedObject>* copy(void) const;

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

    // @cmember Return the <p Key> of the <p ReferencedObject> in the
    //          associated <c OMStrongReferenceSetProperty> at the position
    //          currently designated by this <c OMStrongReferenceSetIterator>.
   UniqueIdentification identification(void) const;

protected:

  typedef OMStrongReferenceSetElement<UniqueIdentification,
                                      ReferencedObject> SetElement;

  typedef OMSetIterator<UniqueIdentification, SetElement> SetIterator;

    // @cmember Create an <c OMStrongReferenceSetIterator> given
    //          an underlying <c OMSetIterator>.
  OMStrongReferenceSetIterator(const SetIterator& iter);

private:

  SetIterator _iterator;

};

#include "OMStrongReferenceSetIterT.h"

#endif
