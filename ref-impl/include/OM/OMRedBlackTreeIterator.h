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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMREDBLACKTREEITERATOR_H
#define OMREDBLACKTREEITERATOR_H

#if defined (_MSC_VER)
  // - 'identifier' : identifier was truncated to 'number' characters in
  //   the debug information
#pragma warning(disable:4786) // Gak !
#endif

#include "OMContainerIterator.h"
#include "OMRedBlackTree.h"

  // @class Iterators over <c OMRedBlackTree>s.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @base public | <c OMContainerIterator>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Key, typename Value>
class OMRedBlackTreeIterator : public OMContainerIterator<Value> {
public:
  // @access Public members.

    // @cmember Create an <c OMRedBlackTreeIterator> over the given
    //          <c OMRedBlackTree> <p redBlackTree> and initialize it
    //          to the given <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMRedBlackTreeIterator> is made ready to traverse the
    //          associated <c OMRedBlackTree> in the forward direction
    //          (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMRedBlackTreeIterator> is made ready to traverse the
    //          associated <c OMRedBlackTree> in the reverse direction
    //          (decreasing <p Key>s).
  OMRedBlackTreeIterator(const OMRedBlackTree<Key, Value>& redBlackTree,
                         OMIteratorPosition initialPosition);

    // @cmember Destroy this <c OMRedBlackTreeIterator>.
  virtual ~OMRedBlackTreeIterator(void);

    // @cmember Reset this <c OMRedBlackTreeIterator> to the given
    //          <p initialPosition>.
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMBefore> then this
    //          <c OMRedBlackTreeIterator> is made ready to traverse the
    //          associated <c OMRedBlackTree> in the forward direction
    //          (increasing <p Key>s).
    //          If <p initialPosition> is specified as
    //          <e OMIteratorPosition.OMAfter> then this
    //          <c OMRedBlackTreeIterator> is made ready to traverse the
    //          associated <c OMRedBlackTree> in the reverse direction
    //          (decreasing <p Key>s).
  virtual void reset(OMIteratorPosition initialPosition);

    // @cmember Is this <c OMRedBlackTreeIterator> positioned before the first
    //          <p Value> ?
  virtual bool before(void) const;

    // @cmember Is this <c OMRedBlackTreeIterator> positioned after the last
    //          <p Value> ?
  virtual bool after(void) const;

    // @cmember The number of <p Value>s in the associated
    //          <c OMRedBlackTree>.
  virtual size_t count(void) const;

    // @cmember Advance this <c OMRedBlackTreeIterator> to the next <p Value>,
    //          if any.
    //          If the end of the associated <c OMRedBlackTree> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMRedBlackTreeIterator::valid> becomes <e bool.true> and
    //          <mf OMRedBlackTreeIterator::after> becomes <e bool.false>.
    //          If the end of the associated <c OMRedBlackTree> is reached
    //          then the result is <e bool.false>,
    //          <mf OMRedBlackTreeIterator::valid> becomes <e bool.false> and
    //          <mf OMRedBlackTreeIterator::after> becomes <e bool.true>.
  virtual bool operator++();

    // @cmember Retreat this <c OMRedBlackTreeIterator> to the previous
    //          <p Value>, if any.
    //          If the beginning of the associated <c OMRedBlackTree> is not
    //          reached then the result is <e bool.true>,
    //          <mf OMRedBlackTreeIterator::valid> becomes <e bool.true> and
    //          <mf OMRedBlackTreeIterator::before> becomes <e bool.false>.
    //          If the beginning of the associated <c OMRedBlackTree> is
    //          reached then the result is <e bool.false>,
    //          <mf OMRedBlackTreeIterator::valid> becomes <e bool.false> and
    //          <mf OMRedBlackTreeIterator::before> becomes <e bool.true>.
  virtual bool operator--();

    // @cmember Return the <p Value> in the associated <c OMRedBlackTree>
    //          at the position currently designated by this
    //          <c OMRedBlackTreeIterator>.
  virtual Value& value(void) const;

    // @cmember Set the <p Value> in the associated <c OMRedBlackTree> at the
    //          position currently designated by this
    //          <c OMRedBlackTreeIterator> to <p newValue>. The previous
    //          <p Value> is returned. To preserve the ordering of
    //          <p Key>s, the <p Key> of <p newValue> must be the same as
    //          that of the existing <p Value>.
  virtual Value setValue(const Key k, Value newValue);

    // @cmember Return the <p Key> of the <p Value> in the associated
    //          <c OMRedBlackTree> at the position currently designated by this
    //          <c OMRedBlackTreeIterator>.
  Key key(void) const;

private:

  typedef typename OMRedBlackTree<Key, Value>::Node TreeNode;

  const OMRedBlackTree<Key, Value>* _tree;
  TreeNode* _current;

};

#include "OMRedBlackTreeIteratorT.h"

#endif

