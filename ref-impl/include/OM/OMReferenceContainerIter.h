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
#ifndef OMREFERENCECONTAINERITER_H
#define OMREFERENCECONTAINERITER_H

#include "OMContainerIterator.h"

#include <stddef.h>

class OMObject;

  // @class Abstract base class for iterators over Object Manager reference
  //        containers.
  //        The references may be Strong or Weak.
  //        The collections may be Vectors or Sets.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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

    // @cmember Clear (set to 0) the <p OMObject> in the associated
    //          reference container at the position currently
    //          designated by this <c OMReferenceContainerIterator>.
    //          The existing object, if any, is returned. The associated
    //          reference container is not modified in that no entry is
    //          removed, the existing entry remains but no longer refers
    //          to a valid object.
  virtual OMObject* clearObject(void) = 0;

};

#endif
