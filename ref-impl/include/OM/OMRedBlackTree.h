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
#ifndef OMREDBLACKTREE_H
#define OMREDBLACKTREE_H

#if defined (_MSC_VER)
  // - 'identifier' : identifier was truncated to 'number' characters in
  //   the debug information
#pragma warning(disable:4786) // Gak !
#endif

#include "OMAssertions.h"
#include <stddef.h>

template <typename Key, typename Value>
class OMRedBlackTreeIterator;

  // @class Red-black trees. A red-black tree is an approximately
  //        balanced binary search tree providing O(lg N) performance
  //        for the dynamic set operations. Items in the tree are
  //        uniquely identified by key and carry an associated value.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @tcarg class | Value | The type of the value carried in an
  //          <c OMRedBlackTree> item. This type must support operator =.
template <typename Key, typename Value>
class OMRedBlackTree {
public:
  // @access Public members.

    // @cmember Constructor.
  OMRedBlackTree(void);

    // @cmember Destructor.
  ~OMRedBlackTree(void);

    // @cmember Insert the <p Value> <p v> into this
    //          <c OMRedBlackTree> and associate it with <p Key> <p k>.
  bool insert(const Key k, Value v);

    // @cmember Find the item in this <c OMRedBlackTree> identified
    //          <p k>.  If the item is found it is returned in
    //          <p v> and the result is true. If the element is not
    //          found the result is false.
  bool find(const Key k, Value& v) const;

    // @cmember Find the item in this <c OMRedBlackTree> identified
    //          <p k>.  If the item is found it is returned in
    //          <p v> and the result is true. If the element is not
    //          found the result is false.
  bool find(const Key k, Value** v) const;

    // @cmember Does this <c OMRdBlackTree> contain an item
    //          identified by <p k>?
  bool contains(const Key k) const;

    // @cmember Remove the item assciated with <p Key> k from this
    //          <c OMRedBlackTree>.
  bool remove(const Key k);

    // @cmember The number of items in this <c OMRedBlackTree>.
    //          <mf OMRedBlackTree::count> returns the actual number
    //          of items in the <c OMRedBlackTree>.
  size_t count(void) const;

    // @cmember Traverse this <c OMRedBlackTree> in order, the
    //          function <p f> is called for each item in the tree.
  void traverseInOrder(void (*f)(size_t height,
                                 Key k,
                                 const Value& v)) const;

    // @cmember Traverse this <c OMRedBlackTree> in pre-order, the
    //          function <p f> is called for each item in the tree.
  void traverseInPreOrder(void (*f)(size_t height,
                                    Key k,
                                    const Value& v)) const;

    // @cmember Traverse this <c OMRedBlackTree> in post-order, the
    //          function <p f> is called for each item in the tree.
  void traverseInPostOrder(void (*f)(size_t height,
                                     Key k,
                                     const Value& v)) const;

    // @cmember The height of this <c OMRedBlackTree>.
  size_t height(void) const;

private:

  struct Node;

  bool insert(Node* n, Key k, Value v);

  Node* find(Key k, Node* x) const;

  Node* predecessor(Node* x) const;

  Node* successor(Node* x) const;

  Node* minimum(Node* x) const;

  Node* maximum(Node* x) const;

  void leftRotate(Node* n);

  void rightRotate(Node* n);
 
  void rebalance(Node* x);

  void traverseInOrder(
         Node* subTree,
         size_t height,
         void (*f)(size_t height, Key k, const Value& v)) const;

  void traverseInPreOrder(
         Node* subTree,
         size_t height,
         void (*f)(size_t height, Key k, const Value& v)) const;

  void traverseInPostOrder(
         Node* subTree,
         size_t height,
         void (*f)(size_t height, Key k, const Value& v)) const;

  size_t height(Node* subTree) const;

  void checkInvariant(void) const;

  void checkInvariant(Node* subTree) const;

  size_t blackHeight(Node* subTree) const;

  void destroy(Node* subTree);

  Node* _root;
  Node* _nil; // sentinel
  size_t _count;

  struct Node {

    enum Color {Red, Black};

    Key _key;
    Value _value;
    Node* _parent;
    Node* _left;
    Node* _right;
    Color _color;
  };

  friend class OMRedBlackTreeIterator<Key, Value>;

};

#include "OMRedBlackTreeT.h"

#endif
