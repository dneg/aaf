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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMRedBlackTreeIterator
#include "OMAssertions.h"

  // @mfunc Create an <c OMRedBlackTreeIterator> over the given
  //        <c OMRedBlackTree> <p redBlackTree> and initialize it
  //        to the given <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMRedBlackTreeIterator> is made ready to traverse the
  //        associated <c OMRedBlackTree> in the forward direction
  //        (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMRedBlackTreeIterator> is made ready to traverse the
  //        associated <c OMRedBlackTree> in the reverse direction
  //        (decreasing <p Key>s).
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @parm The <c OMRedBlackTree> over which to iterate.
  //   @parm The initial position for this <c OMRedBlackTreeIterator>.
template <typename Key, typename Value>
OMRedBlackTreeIterator<Key, Value>::OMRedBlackTreeIterator(
                                const OMRedBlackTree<Key, Value>& redBlackTree,
                                OMIteratorPosition initialPosition)
  : _tree(&redBlackTree)
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::OMRedBlackTreeIterator");
  reset(initialPosition);
}

  // @mfunc Destroy this <c OMRedBlackTreeIterator>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
template <typename Key, typename Value>
OMRedBlackTreeIterator<Key, Value>::~OMRedBlackTreeIterator(void)
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::~OMRedBlackTreeIterator");
}

  // @mfunc Reset this <c OMRedBlackTreeIterator> to the given
  //        <p initialPosition>.
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMBefore> then this
  //        <c OMRedBlackTreeIterator> is made ready to traverse the
  //        associated <c OMRedBlackTree> in the forward direction
  //        (increasing <p Key>s).
  //        If <p initialPosition> is specified as
  //        <e OMIteratorPosition.OMAfter> then this
  //        <c OMRedBlackTreeIterator> is made ready to traverse the
  //        associated <c OMRedBlackTree> in the reverse direction
  //        (decreasing <p Key>s).
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @parm The position to which this <c OMRedBlackTreeIterator> should
  //          be reset.
template <typename Key, typename Value>
void OMRedBlackTreeIterator<Key, Value>::reset(
                                            OMIteratorPosition initialPosition)
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::reset");

  PRECONDITION("Valid initial position",
                (initialPosition == OMBefore) || (initialPosition == OMAfter));
  if (initialPosition == OMBefore) {
    _current = 0;
  } else {
    _current = _tree->_nil;
  }
  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMBefore, before()));
  POSTCONDITION("Valid position",
                               IMPLIES(initialPosition == OMAfter,  after()));
}

  // @mfunc Is this <c OMRedBlackTreeIterator> positioned before the first
  //        <p Value> ?
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc <e bool.true> if this <c OMRedBlackTreeIterator> is positioned
  //          before the first <p Value>, <e bool.false> otherwise.
  //   @this const
template <typename Key, typename Value>
bool OMRedBlackTreeIterator<Key, Value>::before(void) const
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::before");

  bool result;
  if (_current == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMRedBlackTreeIterator> positioned after the last
  //        <p Value> ?
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc <e bool.true> if this <c OMRedBlackTreeIterator> is positioned
  //          after the last <p Value>, <e bool.false> otherwise.
  //   @this const
template <typename Key, typename Value>
bool OMRedBlackTreeIterator<Key, Value>::after(void) const
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::after");

  bool result;
  if (_current == _tree->_nil) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The number of <p Value>s in the associated
  //        <c OMRedBlackTree>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc The number of <p Value>s.
  //   @this const
template <typename Key, typename Value>
size_t OMRedBlackTreeIterator<Key, Value>::count(void) const
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::count");

  return _tree->count();
}

  // @mfunc Advance this <c OMRedBlackTreeIterator> to the next <p Value>,
  //        if any.
  //        If the end of the associated <c OMRedBlackTree> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMRedBlackTreeIterator::valid> becomes <e bool.true> and
  //        <mf OMRedBlackTreeIterator::after> becomes <e bool.false>.
  //        If the end of the associated <c OMRedBlackTree> is reached
  //        then the result is <e bool.false>,
  //        <mf OMRedBlackTreeIterator::valid> becomes <e bool.false> and
  //        <mf OMRedBlackTreeIterator::after> becomes <e bool.true>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc <e bool.false> if this <c OMRedBlackTreeIterator> has passed
  //          the last <p Value>, <e bool.true> otherwise.
template <typename Key, typename Value>
bool OMRedBlackTreeIterator<Key, Value>::operator++()
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::operator++()");

  PRECONDITION("Valid iterator", before() || valid());

  bool result;
  if (_current == 0) {
    // we are in state "before", so point to first value, if any
    if (_tree->_root != _tree->_nil) {
      // tree contains some nodes
      _current = _tree->minimum(_tree->_root);
    } else {
      // tree is empty, go to state "after"
      _current = _tree->_nil;
    }
  } else if (_current != _tree->_nil) {
    // advance
    _current = _tree->successor(_current);
  }
  if (_current != _tree->_nil) {
    // we are pointing at the next value
    result = true;
  } else {
    // there is no next value
    result = false;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !after()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  after()));
  POSTCONDITION("Consistent result", IMPLIES( result,  valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !valid()));
  POSTCONDITION("Valid result",
               IMPLIES(valid(), (_current != 0) && (_current != _tree->_nil)));
  return result;
}

  // @mfunc Retreat this <c OMRedBlackTreeIterator> to the previous
  //        <p Value>, if any.
  //        If the beginning of the associated <c OMRedBlackTree> is not
  //        reached then the result is <e bool.true>,
  //        <mf OMRedBlackTreeIterator::valid> becomes <e bool.true> and
  //        <mf OMRedBlackTreeIterator::before> becomes <e bool.false>.
  //        If the beginning of the associated <c OMRedBlackTree> is
  //        reached then the result is <e bool.false>,
  //        <mf OMRedBlackTreeIterator::valid> becomes <e bool.false> and
  //        <mf OMRedBlackTreeIterator::before> becomes <e bool.true>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc <e bool.false> if this <c OMRedBlackTreeIterator> has passed
  //          the first <p Value>, <e bool.true> otherwise.
template <typename Key, typename Value>
bool OMRedBlackTreeIterator<Key, Value>::operator--()
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::operator--()");

  PRECONDITION("Valid iterator", after() || valid());

  bool result;
  if (_current == _tree->_nil) {
    // we are in state "after", so point to last value, if any
    if (_tree->_root != _tree->_nil) {
      // tree contains some nodes
      _current = _tree->maximum(_tree->_root);
    } else {
      // tree is empty, go to state "before"
      _current = 0;
    }
  } else if (_current != _tree->_nil) {
    // retreat
    _current = _tree->predecessor(_current);
  }
  if (_current != _tree->_nil) {
    // we are pointing at the previous value
    result = true;
  } else {
    // there is no previous value
    result = false;
    // go to state "before"
    _current = 0;
  }
  POSTCONDITION("Consistent result", IMPLIES( result, !before()));
  POSTCONDITION("Consistent result", IMPLIES(!result,  before()));
  POSTCONDITION("Consistent result", IMPLIES( result,  valid()));
  POSTCONDITION("Consistent result", IMPLIES(!result, !valid()));
  POSTCONDITION("Valid result",
               IMPLIES(valid(), (_current != 0) && (_current != _tree->_nil)));
  return result;
}

  // @mfunc Return the <p Value> in the associated <c OMRedBlackTree>
  //        at the position currently designated by this
  //        <c OMRedBlackTreeIterator>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc The <p Value> at the current position.
  //   @this const
template <typename Key, typename Value>
Value& OMRedBlackTreeIterator<Key, Value>::value(void) const
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::value");

  PRECONDITION("Valid iterator", valid());

  Value& result = _current->_value;
  return result;
}

  // @mfunc Set the <p Value> in the associated <c OMRedBlackTree> at the
  //        position currently designated by this
  //        <c OMRedBlackTreeIterator> to <p newValue>. The previous
  //        <p Value> is returned. To preserve the ordering of
  //        <p Key>s, the <p Key> of <p newValue> must be the same as
  //        that of the existing <p Value>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @parm The key <p Key>.
  //   @parm The new <p Value>.
  //   @rdesc The previous <p Value>.
template <typename Key, typename Value>
Value OMRedBlackTreeIterator<Key, Value>::setValue(const Key ANAME(k),
                                                   Value newValue)
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::setValue");

  PRECONDITION("Valid iterator", valid());
  PRECONDITION("Matching keys", k == key());

  Value result = _current->_value;
  _current->_value = newValue;

  return result;
}

  // @mfunc Return the <p Key> of the <p Value> in the associated
  //        <c OMRedBlackTree> at the position currently designated by this
  //        <c OMRedBlackTreeIterator>.
  //   @tcarg class | Key | The type of the unique key that identifies
  //          the contained values.
  //   @tcarg class | Value | The type of the contained values.
  //   @rdesc The <p Key> at the current position.
  //   @this const
template <typename Key, typename Value>
Key OMRedBlackTreeIterator<Key, Value>::key(void) const
{
  TRACE("OMRedBlackTreeIterator<Key, Value>::key");

  PRECONDITION("Valid iterator", valid());

  Key result = _current->_key;
  return result;
}
