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

// @doc OMINTERNAL
#ifndef OMCONTAINERITERATOR_H
#define OMCONTAINERITERATOR_H

template <typename Element>
class OMContainer;

// @enum Iterator initial position
enum OMIteratorPosition {
  OMBefore, // @emem Position the iterator before the first element
  OMAfter   // @emem Position the iterator after the last element
};

  // @class Abstract base class for iterators over Object Manager containers.
  //        The elements of an Object Manager container have a well defined
  //        order. An Object Manager container may be traversed in either the
  //        forward or reverse direction.
  //   @tcarg class | Element | The type of the contained elements.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMContainerIterator {
public:
  // @access Public members.

    // @cmember Reset this <c OMContainerIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMContainerIterator> is made ready to traverse the
    //          associated <c OMContainer> in the forward direction.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMContainerIterator> is made ready to traverse the
    //          associated <c OMContainer> in the reverse direction.
  virtual void reset(OMIteratorPosition initialPosition) = 0;

    // @cmember Is this <c OMContainerIterator> positioned before the first
    //          <p Element> ?
  virtual bool before(void) const = 0;

    // @cmember Is this <c OMContainerIterator> positioned after the last
    //          <p Element> ?
  virtual bool after(void) const = 0;

    // @cmember Is this <c OMContainerIterator> validly positioned on
    //          an <p Element> ?
  virtual bool valid(void) const {return (!after()) && (!before());}

    // @cmember Advance this <c OMContainerIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMContainer> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMContainerIterator::valid> becomes <e bool.true> and
    //          <mf OMContainerIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMContainer> is reached
    //          then the result is <e bool.false>,
    //          <mf OMContainerIterator::valid> becomes <e bool.false> and
    //          <mf OMContainerIterator::after> becomes <e bool.true>.
  virtual bool operator++() = 0;

    // @cmember Retreat this <c OMContainerIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMContainer> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMContainerIterator::valid> becomes <e bool.true> and
    //          <mf OMContainerIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMContainer> is
    //          reached then the result is <e bool.false>,
    //          <mf OMContainerIterator::valid> becomes <e bool.false> and
    //          <mf OMContainerIterator::before> becomes <e bool.true>.
  virtual bool operator--() = 0;

    // @cmember Return the <p Element> in the associated <c OMContainer>
    //          at the position currently designated by this
    //          <c OMContainerIterator>.
  virtual Element& value(void) const = 0;

};

#endif

