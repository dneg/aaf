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
#ifndef OMSETITERATOR_H
#define OMSETITERATOR_H

#include "OMContainerIterator.h"
#include "OMRedBlackTreeIterator.h"

template <typename Key, typename Element>
class OMSet;

// @class Iterators over <c OMSet>s.
//   @tcarg class | Key | The type of the unique key that identifies
//          the contained elements.
//   @tcarg class | Element | The type of the contained elements.
//   @base public | <c OMContainerIterator>
template <typename Key, typename Element>
class OMSetIterator : public OMContainerIterator<Element> {
public:
  // @access Public members.

    // @cmember Create an <c OMSetIterator> over the given
    //          <c OMSet> <p set> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the forward direction
    //          (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the reverse direction
    //          (decreasing <p Key>s).
  OMSetIterator(const OMSet<Key, Element>& set,
                OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMSetIterator>.
  virtual ~OMSetIterator(void);

    // @cmember Reset this <c OMSetIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the forward direction
    //          (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMSetIterator> is made ready to traverse the
    //          associated <c OMSet> in the reverse direction
    //          (decreasing <p Key>s).
   virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMSetIterator> positioned before the first
    //          <p Element> ?
   virtual bool before(void) const;

    // @cmember Is this <c OMSetIterator> positioned after the last
    //          <p Element> ?
   virtual bool after(void) const;

    // @cmember Advance this <c OMSetIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMSet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMSetIterator::valid> becomes <e bool.true> and
    //          <mf OMSetIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMSet> is reached
    //          then the result is <e bool.false>,
    //          <mf OMSetIterator::valid> becomes <e bool.false> and
    //          <mf OMSetIterator::after> becomes <e bool.true>. 
   virtual bool operator++();

    // @cmember Retreat this <c OMSetIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMSet> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMSetIterator::valid> becomes <e bool.true> and
    //          <mf OMSetIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMSet> is
    //          reached then the result is <e bool.false>,
    //          <mf OMSetIterator::valid> becomes <e bool.false> and
    //          <mf OMSetIterator::before> becomes <e bool.true>. 
   virtual bool operator--();

    // @cmember Return the <p Element> in the associated <c OMSet>
    //          at the position currently designated by this
    //          <c OMSetIterator>.
   virtual Element& value(void) const;

    // @cmember Set the <p Element> in the associated <c OMSet> at the
    //          position currently designated by this
    //          <c OMSetIterator> to <p newElement>. The previous
    //          <p Element> is returned. To preserve the ordering of
    //          <p Key>s, the <p Key> of <p newElement> must be the same as
    //          that of the existing <p Element>.
   virtual Element setValue(Element newElement);

    // @cmember Return the <p Key> of the <p Element> in the associated
    //          <c OMSet> at the position currently designated by this
    //          <c OMSetIterator>.
   const Key key(void) const;

private:

  OMRedBlackTreeIterator<Key, Element> _iterator;

};

#include "OMSetIteratorT.h"

#endif

