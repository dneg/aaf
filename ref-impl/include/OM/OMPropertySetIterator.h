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

// @doc OMINTERNAL
#ifndef OMPROPERTYSETITERATOR_H
#define OMPROPERTYSETITERATOR_H

#include "OMContainerIterator.h"
#include "OMRedBlackTreeIterator.h"
#include "OMDataTypes.h"

class OMProperty;
class OMPropertySet;

  // @class Iterators over <c OMPropertySet>s.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMPropertySetIterator {
public:
  // @access Public members.

    // @cmember Create an <c OMPropertySetIterator> over the given
    //          <c OMPropertySet> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the forward direction
    //          (increasing <p OMPropertyId>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the reverse direction
    //          (decreasing <p OMPropertyId>s).
  OMPropertySetIterator(const OMPropertySet& set,
                        OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMPropertySetIterator>.
  virtual ~OMPropertySetIterator(void);

    // @cmember Reset this <c OMPropertySetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the forward direction
    //          (increasing <p OMPropertyId>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMPropertySetIterator> is made ready to traverse the
    //          associated <c OMPropertySet> in the reverse direction
    //          (decreasing <p OMPropertyId>s).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMPropertySetIterator> positioned before the first
    //          <c OMProperty> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMPropertySetIterator> positioned after the last
    //          <c OMProperty> ?
  virtual bool after(void) const;

    // @cmember Is this <c OMPropertySetIterator> validly positioned on
    //          an <c OMProperty> ?
  virtual bool valid(void) const;

    // @cmember The number of <c OMProperty>s in the associated
    //          <c OMPropertySet>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMPropertySetIterator> to the next
    //          <c OMProperty>, if any.
    //          If the end of the associated <c OMPropertySet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.true> and
    //          <mf OMPropertySetIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMPropertySet> is reached
    //          then the result is <e bool.false>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.false> and
    //          <mf OMPropertySetIterator::after> becomes <e bool.true>. 
  virtual bool operator++();

    // @cmember Retreat this <c OMPropertySetIterator> to the previous
    //          <c OMProperty>, if any.
    //          If the beginning of the associated <c OMPropertySet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.true> and
    //          <mf OMPropertySetIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMPropertySet> is
    //          reached then the result is <e bool.false>,
    //          <mf OMPropertySetIterator::valid> becomes <e bool.false> and
    //          <mf OMPropertySetIterator::before> becomes <e bool.true>. 
  virtual bool operator--();

    // @cmember Return the <c OMProperty> in the associated <c OMPropertySet>
    //          at the position currently designated by this
    //          <c OMPropertySetIterator>.
  virtual OMProperty* property(void) const;

    // @cmember Return the <p OMPropertyId> of the <c OMProperty> in the
    //          associated <c OMPropertySet> at the position currently
    //          designated by this <c OMPropertySetIterator>.
  OMPropertyId propertyId(void) const;

private:

  OMRedBlackTreeIterator<OMPropertyId, OMProperty*> _iterator;

};


#endif

