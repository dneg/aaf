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
  virtual Value setValue(Value newValue);

    // @cmember Return the <p Key> of the <p Value> in the associated
    //          <c OMRedBlackTree> at the position currently designated by this
    //          <c OMRedBlackTreeIterator>.
  Key key(void) const;

private:

  const OMRedBlackTree<Key, Value>* _tree;
  OMRedBlackTree<Key, Value>::Node* _current;

};

#include "OMRedBlackTreeIteratorT.h"

#endif

