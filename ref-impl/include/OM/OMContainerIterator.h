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

    // @cmember Set the <p Element> in the associated <c OMContainer> at the
    //          position currently designated by this <c OMContainerIterator>
    //          to <p newElement>. The previous <p Element> is returned.
  virtual Element setValue(Element newElement) = 0;

};

#endif

