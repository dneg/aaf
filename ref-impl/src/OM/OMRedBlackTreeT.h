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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMRedBlackTree
#ifndef OMREDBLACKTREET_H
#define OMREDBLACKTREET_H
#include "OMAssertions.h"

// Red-Black Trees

// This implementation was taken from [1] which is infinitely better
// than the treatment in [2].
//
// References ...
//
// [1] "Introduction to Algorithms", Thomas H. Cormen, Charles
// E. Leiserson, Ronald L. Rivest, 1997 MIT Press (McGraw-Hill) ISBN
// 0-262-03141-8, page 263
//
// [2] "Algorithms, 2nd Ed.", Robert Sedgewick, 1988 Addison-Wesley,
// ISBN 0-201-06673-4, page 219
//

template <typename Key, typename Value>
OMRedBlackTree<Key, Value>::OMRedBlackTree(void)
  : _root(0), _nil(0), _count(0)
{
  // Cormen et. al. page 206 and 272
  //
  TRACE("OMRedBlackTree<Key, Value>::OMRedBlackTree<Key, Value>");

  // Create and initialize the sentinel.
  //
  _nil = new Node();
  ASSERT("Valid heap pointer", _nil != 0);

  // It is difficult to have default values of generic type.
  //
  //_nil->_key = 0;
  //_nil->_value = 0;

  // Arbitrary values.
  //
  _nil->_parent = 0;
  _nil->_left = 0;
  _nil->_right = 0;

  // Important initalization to preserve Red-Black tree properties.
  //
  _nil->_color = Node::Black;

  // Initialize the root.
  //
  _root = _nil;

  INVARIANT();
}


template <typename Key, typename Value>
OMRedBlackTree<Key, Value>::~OMRedBlackTree(void)
{
  TRACE("OMRedBlackTree<Key, Value>::~OMRedBlackTree<Key, Value>");
  INVARIANT();

  // Destroy the tree
  //
  destroy(_root);
  _root = 0;

  // Destroy the sentinel
  //
  delete _nil;
  _nil = 0;

  _count = 0;
}

  // @mfunc Insert the <p Value> <p v> into this
  //         <c OMRedBlackTree> and associate it with <p Key> <p k>.
  //         If this the first instance of an item identified
  //         by <p Key> <p k> in this <c OMRedBlackTree>, the result
  //         is true, otherwise the result is false.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The key.
  //   @parm The value.
  //   @rdesc True if this is the first instance of an item identified
  //          by <p Key> <p k> in this <c OMRedBlackTree>, false otherwise.
template <typename Key, typename Value>
bool OMRedBlackTree<Key, Value>::insert(const Key k, Value v)
{
  TRACE("OMRedBlackTree<Key, Value>::insert");
  INVARIANT();

  bool result = insert(_root, k, v);

  INVARIANT();
  POSTCONDITION("Value inserted", contains(k));

  return result;
}

  // @mfunc Find the item in this <c OMRedBlackTree> identified
  //        by <p k>.  If the item is found it is returned in
  //        <p v> and the result is true. If the element is not
  //        found the result is false.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The <p Key> for which to search.
  //   @parm The <p Value> associated with <p k>, if any, by reference.
  //   @rdesc True if <p k> was found in this <c OMRedBlackTree>, false
  //          otherwise.
  //   @this const
template <typename Key, typename Value>
bool OMRedBlackTree<Key, Value>::find(const Key k, Value& v) const
{
  TRACE("OMRedBlackTree<Key, Value>::find");
  INVARIANT();

  bool result;

  Node* n = find(k, _root);

  if (n != _nil) {
    // found
    result = true;
    v = n->_value;
  } else {
    // not found
    result = false;
  }

  INVARIANT();
  POSTCONDITION("Consistent result", IMPLIES( result,  contains(k)));
  POSTCONDITION("Consistent result", IMPLIES(!result, !contains(k)));
  return result;
}

  // @mfunc Find the item in this <c OMRedBlackTree> identified
  //        by <p k>.  If the item is found it is returned in
  //        <p v> and the result is true. If the element is not
  //        found the result is false.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The <p Key> for which to search.
  //   @parm The <p Value> associated with <p k>, if any, by pointer.
  //   @rdesc True if <p k> was found in this <c OMRedBlackTree>, false
  //          otherwise.
  //   @this const
template <typename Key, typename Value>
bool OMRedBlackTree<Key, Value>::find(const Key k, Value** v) const
{
  TRACE("OMRedBlackTree<Key, Value>::find");
  INVARIANT();

  bool result;

  Node* n = find(k, _root);

  if (n != _nil) {
    // found
    result = true;
    *v = &n->_value;
  } else {
    // not found
    result = false;
  }

  INVARIANT();
  POSTCONDITION("Consistent result", IMPLIES( result,  contains(k)));
  POSTCONDITION("Consistent result", IMPLIES(!result, !contains(k)));
  return result;
}

  // @mfunc Does this <c OMRdBlackTree> contain an item
  //        identified by <p k>?
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The <p Key> for which to search.
  //   @rdesc True if <p k> was found in this <c OMRedBlackTree>, false
  //          otherwise.
  //   @this const
template <typename Key, typename Value>
bool OMRedBlackTree<Key, Value>::contains(const Key k) const
{
  TRACE("OMRedBlackTree<Key, Value>::contains");
  INVARIANT();

  bool result;

  Node* n = find(k, _root);

  if (n != _nil) {
    // found
    result = true;
  } else {
    // not found
    result = false;
  }

  INVARIANT();

  return result;
}

  // @mfunc Remove the item assciated with <p Key> <p k> from this
  //        <c OMRedBlackTree>.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The <p Key> of the item to remove.
  //   @rdesc True if an item identified by <p Key> <p k> was found,
  //          false otherwise.
template <typename Key, typename Value>
bool OMRedBlackTree<Key, Value>::remove(const Key k)
{
  TRACE("OMRedBlackTree<Key, Value>::remove");
  INVARIANT();

  Node* z = find(k, _root);
  bool result;
  if (z != _nil) {
    // Cormen et. al. page 273, RB-DELETE
    //
    Node* y;
    if ((z->_left == _nil) || (z->_right == _nil)) {
      y = z;
    } else {
      y = successor(z);
    }
    Node* x;
    if (y->_left != _nil) {
      x = y->_left;
    } else {
      x = y->_right;
    }
    x->_parent = y->_parent;
    if (y->_parent == _nil) {
      _root = x;
    } else {
      if (y == y->_parent->_left) {
        y->_parent->_left = x;
      } else {
        y->_parent->_right = x;
      }
    }
    if (y != z) {
      z->_key = y->_key;
      z->_value = y->_value; // this copies a value
    }
    if (y->_color == Node::Black) {
      rebalance(x);
    }
    delete y;
    result = true;
    _count = _count - 1;
  } else {
    result = false;
  }

  INVARIANT();
  POSTCONDITION("Value not present", !contains(k));

  return result;
}

  // @mfunc Remove all items from this <c OMRedBlackTree>.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::clear(void)
{
  TRACE("OMRedBlackTree<Key, Value>::clear");

  destroy(_root);
  _root = _nil;

  POSTCONDITION("Empty tree", _root == _nil);
  POSTCONDITION("All elements removed", count() == 0);
  INVARIANT();
}

  // @mfunc The number of items in this <c OMRedBlackTree>.
  //        <mf OMRedBlackTree::count> returns the actual number
  //        of items in the <c OMRedBlackTree>.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @rdesc The number of items in the <c OMRedBlackTree>.
  //   @this const
template <typename Key, typename Value>
size_t OMRedBlackTree<Key, Value>::count(void) const
{
  TRACE("OMRedBlackTree<Key, Value>::count");
  return _count;
}

  // @mfunc Traverse this <c OMRedBlackTree> in order, the
  //        function <p f> is called for each item.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The function to apply to each item.
template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::traverseInOrder(
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMRedBlackTree<Key, Value>::traverseInOrder");
  INVARIANT();
  PRECONDITION("Valid apply function", f != 0);

  traverseInOrder(_root, 0, f);

  INVARIANT();
}

  // @mfunc Traverse this <c OMRedBlackTree> in pre-order, the
  //        function <p f> is called for each item.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The function to apply to each item.
  //   @this const
template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::traverseInPreOrder(
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMRedBlackTree<Key, Value>::traverseInPreOrder");
  INVARIANT();
  PRECONDITION("Valid apply function", f != 0);

  traverseInPreOrder(_root, 0, f);

  INVARIANT();
}

  // @mfunc Traverse this <c OMRedBlackTree> in post-order, the
  //        function <p f> is called for each item.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The function to apply to each item.
  //   @this const
template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::traverseInPostOrder(
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMRedBlackTree<Key, Value>::traverseInPostOrder");
  INVARIANT();
  PRECONDITION("Valid apply function", f != 0);

  traverseInPostOrder(_root, 0, f);

  INVARIANT();
}

  // @mfunc The height of this <c OMRedBlackTree>.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
  //   @parm The hiight of this <c OMRedBlackTree>.
  //   @this const
template <typename Key, typename Value>
size_t OMRedBlackTree<Key, Value>::height(void) const
{
  TRACE("OMRedBlackTree<Key, Value>::height");
  INVARIANT();

  size_t result = height(_root);

  INVARIANT();

  return result;
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::checkInvariant(void) const
{
  TRACE("OMRedBlackTree<Key, Value>::checkInvariant");
#if defined(OM_INVARIANT)
  checkInvariant(_root);
#endif
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::checkInvariant(Node* subTree) const
{
  TRACE("OMRedBlackTree<Key, Value>::checkInvariant");

  if (subTree != _nil) {
    // Every node is either red or black.
    //
    ASSERT("Valid color",
           (subTree->_color == Node::Red) || (subTree->_color == Node::Black));

    // Every leaf is black.
    // TBS

    // If a node is red then both its children are black.
    //
    ASSERT("Red node has black children",
                            IMPLIES(subTree->_color == Node::Red,
                                    (subTree->_left->_color == Node::Black) &&
                                    (subTree->_right->_color == Node::Black)));

    // Every simple path from a node to a descendant leaf contains the
    // same number of black nodes.
    //
    ASSERT("Tree is black balanced",
                  blackHeight(subTree->_left) == blackHeight(subTree->_right));

    checkInvariant(subTree->_left);
    checkInvariant(subTree->_right);
  }
}

template <typename Key, typename Value>
size_t OMRedBlackTree<Key, Value>::blackHeight(Node* subTree) const
{
  TRACE("OMRedBlackTree<Key, Value>::blackHeight");
  //INVARIANT();

  size_t result = 0;
  if (subTree != _nil) {
    size_t leftHeight = blackHeight(subTree->_left); // arbitrary
    if (subTree->_color == Node::Black) {
      result = leftHeight + 1;
    } else {
      result = leftHeight;
    }
  } else {
    result = 1;
  }

  //INVARIANT();

  return result;
}

template <typename Key, typename Value>
typename OMRedBlackTree<Key, Value>::Node*
OMRedBlackTree<Key, Value>::find(Key k, Node* x) const
{
  TRACE("OMRedBlackTree<Key, Value>::find");
  INVARIANT();

  while ((x != _nil) && (k != x->_key)) {
    if (k < x->_key) {
      x = x->_left;
    } else {
      x = x->_right;
    }
  }

  INVARIANT();
  //POSTCONDITION("Consistent result", IMPLIES(x != _nil,  contains(k)));
  //POSTCONDITION("Consistent result", IMPLIES(x == _nil, !contains(k)));

  return x;
}

template <typename Key, typename Value>
bool OMRedBlackTree<Key, Value>::insert(Node* n, Key k, Value v)
{
  TRACE("OMRedBlackTree<Key, Value>::insert");
  // Cormen et. al. page 251 TREE-INSERT
  // Cormen et. al. page 268 RB-INSERT
  //
  bool result = false;
  Node* parent = _nil;

  // Find the place to insert
  while ((n != _nil) && (n->_key != k)) {
    parent = n;
    if (k < n->_key) {
      n = n->_left;
    } else {
      n = n->_right;
    }
  }

  if (n == _nil) {
    // There is no existing node with this key, create one
    result = true;
    n = new Node();
    ASSERT("Valid heap pointer", n != 0);
    n->_key = k;
    n->_value = v;
    n->_parent = parent;
    n->_left = _nil;
    n->_right = _nil;
    // Set parent links
    if (parent != _nil) {
      if (k < parent->_key) {
        parent->_left = n;
      } else {
        parent->_right = n;
      }
    } else {
      _root = n;
    }
    _count = _count + 1;
    // RedBlack specific insertion
    n->_color = Node::Red;
    while ((n != _root) && (n->_parent->_color == Node::Red)) {
      Node* y = _nil;
      if (n->_parent == n->_parent->_parent->_left) {
        // Same as "else" with "right" and "left" exchanged.
        y = n->_parent->_parent->_right;
        if (y->_color == Node::Red) {
          n->_parent->_color = Node::Black;
          y->_color = Node::Black;
          n->_parent->_parent->_color = Node::Red;
          n = n->_parent->_parent;
        } else {
          if (n == n->_parent->_right) {
            n = n->_parent;
            leftRotate(n);
          }
          n->_parent->_color = Node::Black;
          n->_parent->_parent->_color = Node::Red;
          rightRotate(n->_parent->_parent);
        }
      } else {
        // Same as "if" with "right" and "left" exchanged.
        y = n->_parent->_parent->_left;
        if (y->_color == Node::Red) {
          n->_parent->_color = Node::Black;
          y->_color = Node::Black;
          n->_parent->_parent->_color = Node::Red;
          n = n->_parent->_parent;
        } else {
          if (n == n->_parent->_left) {
            n = n->_parent;
            rightRotate(n);
          }
          n->_parent->_color = Node::Black;
          n->_parent->_parent->_color = Node::Red;
          leftRotate(n->_parent->_parent);
        }
      }
    }
    _root->_color = Node::Black;
  }
  return result;
}

template <typename Key, typename Value>
typename OMRedBlackTree<Key, Value>::Node*
OMRedBlackTree<Key, Value>::predecessor(Node* x) const
{
  TRACE("OMRedBlackTree<Key, Value>::predecessor");
  // Cormen et. al. page 249 TREE-PREDECESSOR [by symmetry with TREE-SUCCESSOR]
  //
  Node* result;
  if (x->_left != _nil) {
    result = maximum(x->_left);
  } else {
    Node* y = x->_parent;
    while ((y != _nil) && (x == y->_left)) {
      x = y;
      y = y->_parent;
    }
    result = y;
  }
  return result;
}

template <typename Key, typename Value>
typename OMRedBlackTree<Key, Value>::Node*
OMRedBlackTree<Key, Value>::successor(Node* x) const
{
  TRACE("OMRedBlackTree<Key, Value>::successor");
  // Cormen et. al. page 249 TREE-SUCCESSOR
  //
  Node* result;
  if (x->_right != _nil) {
    result = minimum(x->_right);
  } else {
    Node* y = x->_parent;
    while ((y != _nil) && (x == y->_right)) {
      x = y;
      y = y->_parent;
    }
    result = y;
  }
  return result;
}

template <typename Key, typename Value>
typename OMRedBlackTree<Key, Value>::Node*
OMRedBlackTree<Key, Value>::minimum(Node* x) const
{
  TRACE("OMRedBlackTree<Key, Value>::minimum");
  // Cormen et. al. page 248 TREE-MINIMUM
  //
  Node* result = x;
  while (result->_left != _nil) {
    result = result->_left;
  }
  return result;
}

template <typename Key, typename Value>
typename OMRedBlackTree<Key, Value>::Node*
OMRedBlackTree<Key, Value>::maximum(Node* x) const
{
  TRACE("OMRedBlackTree<Key, Value>::maximum");
  // Cormen et. al. page 248 TREE-MAXIMUM
  //
  Node* result = x;
  while (result->_right != _nil) {
    result = result->_right;
  }
  return result;
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::leftRotate(Node* n)
{
  TRACE("OMRedBlackTree<Key, Value>::leftRotate");

  // Cormen et. al. page 266 LEFT-ROTATE
  //
  ASSERT("Non-nil right child", n->_right != _nil);
  Node* y = n->_right;
  n->_right = y->_left;
  if (y->_left != _nil) {
    y->_left->_parent = n;
  }
  y->_parent = n->_parent;
  if (n->_parent == _nil) {
    _root = y;
  } else {
    if (n == n->_parent->_left) {
      n->_parent->_left = y;
    } else {
      n->_parent->_right = y;
    }
  }
  y->_left = n;
  n->_parent = y;
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::rightRotate(Node* n)
{
  TRACE("OMRedBlackTree<Key, Value>::rightRotate");

  // Cormen et. al. page 267 RIGHT-ROTATE [Exercise]
  //
  ASSERT("Non-nil left child", n->_left != _nil);
  Node* y = n->_left;
  n->_left = y->_right;
  if (y->_right != _nil) {
    y->_right->_parent = n;
  }
  y->_parent = n->_parent;
  if (n->_parent == _nil) {
    _root = y;
  } else {
    if (n == n->_parent->_right) {
      n->_parent->_right = y;
    } else {
      n->_parent->_left = y;
    }
  }
  y->_right = n;
  n->_parent = y;
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::rebalance(Node* x)
{
  TRACE("OMRedBlackTree<Key, Value>::rebalance");
  // Cormen et. al. page 274 RB-DELETE-FIXUP
  //
  while ((x != _root) && (x->_color == Node::Black)) {
    Node* w;
    if (x == x->_parent->_left) {
      // Same as "else" with "right" and "left" exchanged.
      w = x->_parent->_right;
      if (w->_color == Node::Red) {
        w->_color = Node::Black;
        x->_parent->_color = Node::Red;
        leftRotate(x->_parent);
        w = x->_parent->_right;
      }
      if ((w->_left->_color == Node::Black) &&
          (w->_right->_color == Node::Black)) {
        w->_color = Node::Red;
        x = x->_parent;
      } else {
        if (w->_right->_color == Node::Black) {
          w->_left->_color = Node::Black;
          w->_color = Node::Red;
          rightRotate(w);
          w = x->_parent->_right;
        }
        w->_color = x->_parent->_color;
        x->_parent->_color = Node::Black;
        w->_right->_color = Node::Black;
        leftRotate(x->_parent);
        x = _root;
      }
    } else {
      // Same as "if" with "right" and "left" exchanged.
      w = x->_parent->_left;
      if (w->_color == Node::Red) {
        w->_color = Node::Black;
        x->_parent->_color = Node::Red;
        rightRotate(x->_parent);
        w = x->_parent->_left;
      }
      if ((w->_right->_color == Node::Black) &&
          (w->_left->_color == Node::Black)) {
        w->_color = Node::Red;
        x = x->_parent;
      } else {
        if (w->_left->_color == Node::Black) {
          w->_right->_color = Node::Black;
          w->_color = Node::Red;
          leftRotate(w);
          w = x->_parent->_left;
        }
        w->_color = x->_parent->_color;
        x->_parent->_color = Node::Black;
        w->_left->_color = Node::Black;
        rightRotate(x->_parent);
        x = _root;
      }
    }
  }
  x->_color = Node::Black;
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::traverseInOrder(
                         Node* subTree,
                         size_t height,
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMRedBlackTree<Key, Value>::traverseInOrder(");

  if (subTree != _nil) {
    traverseInOrder(subTree->_left, height + 1, f);
    (*f)(height, subTree->_key, subTree->_value);
    traverseInOrder(subTree->_right, height + 1, f);
  }
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::traverseInPreOrder(
                         Node* subTree,
                         size_t height,
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMRedBlackTree<Key, Value>::traverseInPreOrder(");

  if (subTree != _nil) {
    (*f)(height, subTree->_key, subTree->_value);
    traverseInPreOrder(subTree->_left, height + 1, f);
    traverseInPreOrder(subTree->_right, height + 1, f);
  }
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::traverseInPostOrder(
                         Node* subTree,
                         size_t height,
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMRedBlackTree<Key, Value>::traverseInPostOrder(");

  if (subTree != _nil) {
    traverseInPostOrder(subTree->_left, height + 1, f);
    traverseInPostOrder(subTree->_right, height + 1, f);
    (*f)(height, subTree->_key, subTree->_value);
  }
}

template <typename Key, typename Value>
size_t OMRedBlackTree<Key, Value>::height(Node* subTree) const
{
  TRACE("OMRedBlackTree<Key, Value>::height");
  size_t result;
  size_t leftHeight;
  size_t rightHeight;
  size_t tallestHeight;

  if (subTree == _nil) {
    result = 0;
  } else {
    leftHeight = height(subTree->_left);
    rightHeight = height(subTree->_right);
    if (rightHeight > leftHeight) {
      tallestHeight = rightHeight;
    } else {
      tallestHeight = leftHeight;
    }
    result = 1 + tallestHeight;
  }
  return result;
}

template <typename Key, typename Value>
void OMRedBlackTree<Key, Value>::destroy(Node* subTree)
{
  TRACE("OMRedBlackTree<Key, Value>::destroy");

  // Traverse the tree in post-order deleting the nodes
  //
  if (subTree != _nil) {
    destroy(subTree->_left);
    destroy(subTree->_right);
    delete subTree;
    _count = _count - 1;
  }
}

#endif
