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
#ifndef OMREFERENCECONTAINERITER_H
#define OMREFERENCECONTAINERITER_H

#include "OMContainerIterator.h"

#include <stddef.h>

class OMObject;

// @class Abstract base class for iterators over Object Manager reference
//        containers.
//        The references may be Strong or Weak.
//        The collections may be Vectors or Sets.
class OMReferenceContainerIterator {
public:
  // @access Public members.

    // @cmember Destroy this <c OMReferenceContainerIterator>.
  virtual ~OMReferenceContainerIterator(void) {}

    // @cmember Create a copy of this <c OMReferenceContainerIterator>.
  virtual OMReferenceContainerIterator* copy(void) const = 0;

    // @cmember Reset this <c OMReferenceContainerIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMReferenceContainerIterator> is made ready to traverse the
    //          associated reference container in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMReferenceContainerIterator> is made ready to traverse the
    //          associated reference container in the reverse
    //          direction (decreasing <p Key>s).
   virtual void reset(OMIteratorPosition initialPosition = OMBefore) = 0;

    // @cmember Is this <c OMReferenceContainerIterator> positioned before
    //          the first <p OMObject> ?
   virtual bool before(void) const = 0;

    // @cmember Is this <c OMReferenceContainerIterator> positioned after
    //          the last <p OMObject> ?
   virtual bool after(void) const = 0;

    // @cmember Is this <c OMReferenceContainerIterator> validly
    //          positioned on a <p OMObject> ?
  virtual bool valid(void) const = 0;

    // @cmember The number of <p OMObject>s in the associated
    //          reference container.
  virtual size_t count(void) const = 0;

    // @cmember Advance this <c OMReferenceContainerIterator> to the next
    //          <p OMObject>, if any.
    //          If the end of the associated
    //          reference container is not reached then the
    //          result is <e bool.true>,
    //          <mf OMReferenceContainerIterator::valid> becomes <e bool.true>
    //          and <mf OMReferenceContainerIterator::after> becomes
    //          <e bool.false>.
    //          If the end of the associated
    //          reference container is reached then the result
    //          is <e bool.false>, <mf OMReferenceContainerIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMReferenceContainerIterator::after> becomes
    //          <e bool.true>. 
   virtual bool operator++() = 0;

    // @cmember Retreat this <c OMReferenceContainerIterator> to the previous
    //          <p OMObject>, if any.
    //          If the beginning of the associated
    //          reference container is not reached then the
    //          result is <e bool.true>,
    //          <mf OMReferenceContainerIterator::valid> becomes <e bool.true>
    //          and <mf OMReferenceContainerIterator::before> becomes
    //          <e bool.false>.
    //          If the beginning of the associated
    //          reference container is reached then the result
    //          is <e bool.false>, <mf OMReferenceContainerIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMReferenceContainerIterator::before> becomes
    //          <e bool.true>. 
   virtual bool operator--() = 0;

    // @cmember Return the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMReferenceContainerIterator>.
   virtual OMObject* currentObject(void) const = 0;

};

#endif
