/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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
#ifndef OMVECTORITERATOR_H
#define OMVECTORITERATOR_H

#include "OMContainerIterator.h"

template <typename Element>
class OMVector;

// @class Iterators over <c OMVector>s.
//   @tcarg class | Element | The type of the contained elements.
//   @base public | <c OMContainerIterator>
template <typename Element>
class OMVectorIterator : public OMContainerIterator<Element> {
public:
  // @access Public members.

    // @cmember Create an <c OMVectorIterator> over the given
    //          <c OMVector> <p vector> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the forward direction
    //          (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the reverse direction
    //          (decreasing indexes).
  OMVectorIterator(const OMVector<Element>& vector,
                   OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMVectorIterator>.
  ~OMVectorIterator(void);

    // @cmember Reset this <c OMVectorIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the forward direction
    //          (increasing indexes).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMVectorIterator> is made ready to traverse the
    //          associated <c OMVector> in the reverse direction
    //          (decreasing indexes).
   virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMVectorIterator> positioned before the first
    //          <p Element> ?
   virtual bool before(void) const;

    // @cmember Is this <c OMVectorIterator> positioned after the last
    //          <p Element> ?
   virtual bool after(void) const;

    // @cmember Advance this <c OMVectorIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMVector> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMVectorIterator::valid> becomes <e bool.true> and
    //          <mf OMVectorIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMVector> is reached
    //          then the result is <e bool.false>,
    //          <mf OMVectorIterator::valid> becomes <e bool.false> and
    //          <mf OMVectorIterator::after> becomes <e bool.true>. 
   virtual bool operator++();

    // @cmember Retreat this <c OMVectorIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMVector> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMVectorIterator::valid> becomes <e bool.true> and
    //          <mf OMVectorIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMVector> is
    //          reached then the result is <e bool.false>,
    //          <mf OMVectorIterator::valid> becomes <e bool.false> and
    //          <mf OMVectorIterator::before> becomes <e bool.true>. 
   virtual bool operator--();

    // @cmember Return the <p Element> in the associated <c OMVector>
    //          at the position currently designated by this
    //          <c OMVectorIterator>.
   virtual Element& value(void) const;

    // @cmember Set the <p Element> in the associated <c OMVector> at the
    //          position currently designated by this
    //          <c OMVectorIterator> to <p newElement>. The previous
    //          <p Element> is returned.
   virtual Element setValue(Element newElement);

    // @cmember Return the index of the <p Element> in the
    //          associated <c OMVector> at the position
    //          currently designated by this <c OMVectorIterator>.
   virtual size_t index(void) const;

private:

  const OMVector<Element>* _vector;
  size_t _index;
  enum OMVectorIteratorState {OMVectorIteratorBefore,
                              OMVectorIteratorValid,
                              OMVectorIteratorAfter};
  OMVectorIteratorState _state;

};

#include "OMVectorIteratorT.h"

#endif

