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

// @doc OMEXTERNAL
#include <stddef.h>

  // @class AVL Trees
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Key, typename Value>
class OMAVLTree {
public:

  OMAVLTree(void);

  ~OMAVLTree(void);

  bool insert(const Key k, Value v);

  bool find(const Key k, Value& v) const;

  bool contains(const Key k) const;

  bool remove(const Key k);

  void traverseInOrder(
         void (*f)(size_t height, Key k, const Value& v)) const;

  void traverseInPreOrder(
         void (*f)(size_t height, Key k, const Value& v)) const;

  void traverseInPostOrder(
         void (*f)(size_t height, Key k, const Value& v)) const;

  size_t height(void) const;

private:

  struct Node;

  bool insert(Node*& subTree, Key k, Value v, bool& heightChanged);

  void leftRotation(Node*& subTree);

  void rightRotation(Node*& subTree);

  void leftRightRotation(Node*& subTree);

  void rightLeftRotation(Node*& subTree);

  bool remove(Node*& subTree, Key k, bool& heightChanged);

  void balanceLeft(Node*& subTree, bool& heightChanged);

  void balanceRight(Node*& subTree, bool& heightChanged);

  void swapDelete(Node*& oldTree, Node*& replace, bool& heightChanged);

  void traverseInOrder(Node* subTree,
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

  void destroy(Node* subTree);

  Node* _root;

  struct Node {
    Key _key;
    Value _value;
    int _balance; // -1 0 +1
    Node* _left;
    Node* _right;
  };

};

#include "OMAVLTreeT.h"
