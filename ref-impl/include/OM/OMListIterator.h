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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMListIterator
#ifndef OMLISTITERATOR_H
#define OMLISTITERATOR_H

#include "OMContainerIterator.h"

#include <stddef.h>

template <typename Element>
class OMList;

  // @class Iterators over <c OMList>s.
  //   @tcarg class | Element | The type of the contained elements.
  //   @base public | <c OMContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMListIterator : public OMContainerIterator<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMListIterator(void);

    // @cmember Create an <c OMListIterator> over the given
    //          <c OMList> <p list> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMListIterator> is made ready to traverse the
    //          associated <c OMList> in the forward direction.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMListIterator> is made ready to traverse the
    //          associated <c OMList> in the reverse direction.
  OMListIterator(const OMList<Element>& list,
                 OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMListIterator>.
  virtual ~OMListIterator(void);

    // @cmember Reset this <c OMListIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMListIterator> is made ready to traverse the
    //          associated <c OMList> in the forward direction.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMListIterator> is made ready to traverse the
    //          associated <c OMList> in the reverse direction.
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMListIterator> positioned before the first
    //          <p Element> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMListIterator> positioned after the last
    //          <p Element> ?
  virtual bool after(void) const;

    // @cmember The number of <p Elements>s in the associated
    //          <c OMList>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMListIterator> to the next <p Element>,
    //          if any.
    //          If the end of the associated <c OMList> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMListIterator::valid> becomes <e bool.true> and
    //          <mf OMListIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMList> is reached
    //          then the result is <e bool.false>,
    //          <mf OMListIterator::valid> becomes <e bool.false> and
    //          <mf OMListIterator::after> becomes <e bool.true>. 
  virtual bool operator++();

    // @cmember Retreat this <c OMListIterator> to the previous
    //          <p Element>, if any.
    //          If the beginning of the associated <c OMList> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMListIterator::valid> becomes <e bool.true> and
    //          <mf OMListIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMList> is
    //          reached then the result is <e bool.false>,
    //          <mf OMListIterator::valid> becomes <e bool.false> and
    //          <mf OMListIterator::before> becomes <e bool.true>. 
  virtual bool operator--();

    // @cmember Return the <p Element> in the associated <c OMList>
    //          at the position currently designated by this
    //          <c OMListIterator>.
  virtual Element& value(void) const;

    // @cmember Set the <p Element> in the associated <c OMList> at the
    //          position currently designated by this
    //          <c OMListIterator> to <p newElement>. The previous
    //          <p Element> is returned.
  virtual Element setValue(Element newElement);

private:

  typedef typename OMList<Element>::Node ListNode;

  const OMList<Element>* _list;
  ListNode* _current;

  enum OMListIteratorState {OMListIteratorBefore,
                            OMListIteratorValid,
                            OMListIteratorAfter};
  OMListIteratorState _state;

  friend class OMList<Element>;

};

#include "OMListIteratorT.h"

#endif

