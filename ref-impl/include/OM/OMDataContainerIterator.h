//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMDATACONTAINERITERATOR_H
#define OMDATACONTAINERITERATOR_H

#include "OMContainerIterator.h"
#include "OMDataTypes.h"

  // @class Iterators over containers of data (i.e. non-object) items.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDataContainerIterator {
public:
  // @access Public members.

    // @cmember Destroy this <c OMDataContainerIterator>.
  virtual ~OMDataContainerIterator(void) {}

    // @cmember Create a copy of this <c OMDataContainerIterator>.
  virtual OMDataContainerIterator* copy(void) const = 0;

    // @cmember Reset this <c OMDataContainerIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMDataContainerIterator> is made ready to traverse the
    //          associated reference container in the forward
    //          direction (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMDataContainerIterator> is made ready to traverse the
    //          associated reference container in the reverse
    //          direction (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition = OMBefore) = 0;

    // @cmember Is this <c OMDataContainerIterator> positioned before
    //          the first element ?
  virtual bool before(void) const = 0;

    // @cmember Is this <c OMDataContainerIterator> positioned after
    //          the last element ?
  virtual bool after(void) const = 0;

    // @cmember Is this <c OMDataContainerIterator> validly
    //          positioned on a element ?
  virtual bool valid(void) const = 0;

    // @cmember The number of elements in the associated
    //          reference container.
  virtual size_t count(void) const = 0;

    // @cmember Advance this <c OMDataContainerIterator> to the next
    //          element, if any.
    //          If the end of the associated
    //          reference container is not reached then the
    //          result is <e bool.true>,
    //          <mf OMDataContainerIterator::valid> becomes <e bool.true>
    //          and <mf OMDataContainerIterator::after> becomes
    //          <e bool.false>.
    //          If the end of the associated
    //          reference container is reached then the result
    //          is <e bool.false>, <mf OMDataContainerIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMDataContainerIterator::after> becomes
    //          <e bool.true>.
  virtual bool operator++() = 0;

    // @cmember Retreat this <c OMDataContainerIterator> to the previous
    //          element, if any.
    //          If the beginning of the associated
    //          reference container is not reached then the
    //          result is <e bool.true>,
    //          <mf OMDataContainerIterator::valid> becomes <e bool.true>
    //          and <mf OMDataContainerIterator::before> becomes
    //          <e bool.false>.
    //          If the beginning of the associated
    //          reference container is reached then the result
    //          is <e bool.false>, <mf OMDataContainerIterator::valid>
    //          becomes <e bool.false> and
    //          <mf OMDataContainerIterator::before> becomes
    //          <e bool.true>.
  virtual bool operator--() = 0;

  virtual const OMByte* currentElement(void) const = 0;

};

#endif
