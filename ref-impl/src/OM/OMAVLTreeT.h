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

// AVLTrees - Adelson-Velskii and Landis

// This implementation was taken from [1] which is based on [2] and
// [3]. For in depth discussion see [4]. For good a good general
// algorithm reference see [5].
//
// References ...
//
// [1] "The Modula-2 Software Component Library, Vol. 3", Charles Lins,
// 1989 Springer-Verlag, ISBN 0-387-97074-6, page 111
//
// [2] "Algorithms + Data Structures = Programs", Niklaus Wirth, 1976
// Prentice Hall, ISBN 0-13-022418-9, page 215
//
// [3] "Algorithms and Data Structures", Nlklaus Wirth, 1986 Prentice
// Hall, ISBN 0-13-022005-1, page 218
//
// [4] "The Design and Analysis of Computer Algorithms", Alfred V. Aho,
// John E. Hopcroft, Jeffrey D. Ullman, 1974 Addison Wesley,
// ISBN 0-201000029-6, page 145
//
// [5] "Introduction to Algorithms", Thomas H. Cormen, Charles
// E. Leiserson, Ronald L. Rivest, 1997 MIT Press (McGraw-Hill) ISBN
// 0-262-03141-8
//

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMAVLTree

template <typename Key, typename Value>
OMAVLTree<Key, Value>::OMAVLTree(void)
: _root(0)
{
  TRACE("OMAVLTree<Key, Value>::OMAVLTree");
  INVARIANT();
}

template <typename Key, typename Value>
OMAVLTree<Key, Value>::~OMAVLTree(void)
{
  TRACE("OMAVLTree<Key, Value>::~OMAVLTree");
  INVARIANT();
  destroy(_root);
  _root = 0;
}

template <typename Key, typename Value>
bool OMAVLTree<Key, Value>::insert(const Key k, Value v)
{
  TRACE("OMAVLTree<Key, Value>::insert");
  INVARIANT();

  bool height = false;
  bool result;
  result = insert(_root, k, v, height);

  INVARIANT();
  POSTCONDITION("Value inserted", contains(k));

  return result;
}

template <typename Key, typename Value>
bool OMAVLTree<Key, Value>::find(const Key k, Value& v) const
{
  TRACE("OMAVLTree<Key, Value>::find");
  INVARIANT();

  Node* n = _root;
  bool result = false;

  while (n != 0) {
    if (k > n->_key) {
      n = n->_right;
    } else if (k < n->_key) {
      n = n->_left;
    } else {
      v = n->_value;
      result = true;
      break;
    }
  }

  INVARIANT();
  POSTCONDITION("Key present",     IMPLIES( result,  contains(k)));
  POSTCONDITION("Key not present", IMPLIES(!result, !contains(k)));

  return result;

}

template <typename Key, typename Value>
bool OMAVLTree<Key, Value>::contains(const Key k) const
{
  TRACE("OMAVLTree<Key, Value>::contains");
  INVARIANT();

  Node* n = _root;
  bool result = false;

  while (n != 0) {
    if (k > n->_key) {
      n = n->_right;
    } else if (k < n->_key) {
      n = n->_left;
    } else {
      result = true;
      break;
    }
  }

  INVARIANT();

  return result;

}

template <typename Key, typename Value>
bool OMAVLTree<Key, Value>::remove(const Key k)
{
  TRACE("OMAVLTree<Key, Value>::remove");
  INVARIANT();

  bool result;
  bool height = false;
  result = remove(_root, k, height);

  INVARIANT();
  POSTCONDITION("Key not present", !contains(k));

  return result;
}

template <typename Key, typename Value>
bool OMAVLTree<Key, Value>::remove(Node*& subTree, Key k, bool& heightChanged)
{
  TRACE("OMAVLTree<Key, Value>::remove");

  bool result = true;
  if (subTree == 0) {
    result = false;
  } else {
    if (k < subTree->_key) {
      result = remove(subTree->_left, k, heightChanged);
      if (heightChanged) {
        balanceLeft(subTree, heightChanged);
      }
    } else if (k > subTree->_key) {
      result = remove(subTree->_right, k, heightChanged);
      if (heightChanged) {
        balanceRight(subTree, heightChanged);
      }
    } else {
#if 1
      Node* oldTree = subTree;
      if (subTree->_right == 0) {
        subTree = subTree->_left;
        heightChanged = true;
      } else if (subTree->_left == 0) {
        subTree = subTree->_right;
        heightChanged = true;
      } else {
        swapDelete(oldTree, oldTree->_left, heightChanged);
        if (heightChanged) {
          balanceLeft(subTree, heightChanged);
        }
      }
      delete oldTree;
#else
      if ((subTree->_left == 0) && (subTree->_right == 0)) {
        // A leaf - remove it
        delete subTree;
        subTree = 0;
        heightChanged = true;
        result = true;
      } else if ((subTree->_left == 0) || (subTree->_right == 0)) {
        // One child - the child becomes the new sub tree
        Node* toDelete = subTree;
        if (subTree->_left == 0) {
          subTree = subTree->_right;
        } else {
          subTree = subTree->_left;
        }

        delete toDelete;
        heightChanged = true;
        result = true;

      } else {
        // Two children
        Node* toDelete = subTree;
        subTree = subTree->_right->_right;
        subTree->_left = toDelete->_left;
        subTree->_right = toDelete->_right;
        delete toDelete;
        heightChanged = true;
        result = true;
      }
#endif
    }
  }
  return result;
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::balanceLeft(Node*& subTree, bool& heightChanged)
{
  TRACE("OMAVLTree<Key, Value>::balanceLeft");

  int newBalance;

  switch (subTree->_balance) {
  case -1:
    subTree->_balance = 0;
    break;
  case 0:
    subTree->_balance = 1;
    heightChanged = false;
    break;
  case +1:
    newBalance = subTree->_right->_balance;
    if (newBalance >= 0) {
      leftRotation(subTree);
      if (newBalance == 0) {
        subTree->_balance = -1;
        subTree->_left->_balance = +1;
        heightChanged = false;
      } else {
        subTree->_balance = 0;
        subTree->_left->_balance = 0;
      }
    } else {
      rightLeftRotation(subTree);
    }
    break;
  }
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::balanceRight(Node*& subTree, bool& heightChanged)
{
  TRACE("OMAVLTree<Key, Value>::balanceRight");

  int newBalance;

  switch (subTree->_balance) {
  case +1:
    subTree->_balance = 0;
    break;
  case 0:
    subTree->_balance = -1;
    heightChanged = false;
    break;
  case -1:
    newBalance = subTree->_left->_balance;
    if (newBalance <= 0) {
      rightRotation(subTree);
      if (newBalance == 0) {
        subTree->_balance = +1;
        subTree->_right->_balance = -1;
        heightChanged = false;
      } else {
        subTree->_balance = 0;
        subTree->_right->_balance = 0;
      }
    } else {
      leftRightRotation(subTree);
    }
    break;
  }
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::swapDelete(Node*& oldTree,
                                       Node*& replace,
                                       bool& heightChanged)
{
  TRACE("OMAVLTree<Key, Value>::swapDelete");

  if (replace->_right != 0) {
    swapDelete(oldTree, replace->_right, heightChanged);
    if (heightChanged) {
      balanceRight(replace, heightChanged);
    }
  } else {
    oldTree->_key = replace->_key;
    oldTree->_value = replace->_value; // !!! This copies a value
    oldTree = replace;
    replace = replace->_left;
    heightChanged = true;
  }
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::traverseInOrder(
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMAVLTree<Key, Value>::traverseInOrder");

  traverseInOrder(_root, 0, f);
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::traverseInPreOrder(
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMAVLTree<Key, Value>::traverseInPreOrder");

  traverseInPreOrder(_root, 0, f);
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::traverseInPostOrder(
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMAVLTree<Key, Value>::traverseInPostOrder");

  traverseInPostOrder(_root, 0, f);
}

template <typename Key, typename Value>
size_t OMAVLTree<Key, Value>::height(void) const
{
  TRACE("OMAVLTree<Key, Value>::height");

  return height(_root);
}

template <typename Key, typename Value>
size_t OMAVLTree<Key, Value>::height(Node* subTree) const
{
  TRACE("OMAVLTree<Key, Value>::height");

  size_t result;
  size_t leftHeight;
  size_t rightHeight;
  size_t tallestHeight;

  if (subTree == 0) {
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
void OMAVLTree<Key, Value>::checkInvariant(void) const
{
  TRACE("OMAVLTree<Key, Value>::checkInvariant");

  checkInvariant(_root);
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::checkInvariant(Node* subTree) const
{
  TRACE("OMAVLTree<Key, Value>::checkInvariant");

  size_t leftHeight;
  size_t rightHeight;
  size_t differenceInHeight;

  if (subTree != 0) {
    ASSERT("Node has valid balance",
                     ((subTree->_balance >= -1) && (subTree->_balance <= +1)));
    leftHeight = height(subTree->_left);
    rightHeight = height(subTree->_right);
    if (rightHeight > leftHeight) {
      differenceInHeight = rightHeight - leftHeight;
    } else {
      differenceInHeight = leftHeight - rightHeight;
    }
    ASSERT("Subtree is AVL balanced", differenceInHeight <= 1);
    checkInvariant(subTree->_left);
    checkInvariant(subTree->_right);
  }
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::destroy(Node* subTree)
{
  if (subTree != 0) {
    destroy(subTree->_left);
    destroy(subTree->_right);
    delete subTree;
  }
}

template <typename Key, typename Value>
bool OMAVLTree<Key, Value>::insert(Node*& subTree,
                                   Key k,
                                   Value v,
                                   bool& heightChanged)
{
  TRACE("OMAVLTree<Key, Value>::insert");

  bool result = true;

  if (subTree == 0) {
    subTree = new Node();
    subTree->_key = k;
    subTree->_value = v;
    subTree->_balance = 0;
    subTree->_left = 0;
    subTree->_right = 0;
    heightChanged = true;
  } else {
    if (k > subTree->_key) {
      result = insert(subTree->_right, k, v, heightChanged);
      if (heightChanged) {
        switch(subTree->_balance) {
        case -1:
          subTree->_balance = 0;
          heightChanged = false; // rebalanced
          break;
        case 0:
          subTree->_balance = 1;
          break;
        case +1:
          if (subTree->_right->_balance == 1) {
            leftRotation(subTree);
            subTree->_left->_balance = 0;
          } else {
            rightLeftRotation(subTree);
          }
          subTree->_balance = 0;
          heightChanged = false;
          break;
        }
      }
    } else if (k < subTree->_key) {
      result = insert(subTree->_left, k, v, heightChanged);
      if (heightChanged) {
        switch(subTree->_balance) {
        case +1:
          subTree->_balance = 0;
          heightChanged = false; // rebalanced
          break;
        case 0:
          subTree->_balance = -1;
          break;
        case -1:
          if (subTree->_left->_balance == -1) {
            rightRotation(subTree);
            subTree->_right->_balance = 0;
          } else {
            leftRightRotation(subTree);
          }
          subTree->_balance = 0;
          heightChanged = false;
          break;
        }
      }
    } else {
      // this key is already present
      result = false;
    }
  }
  return result;
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::leftRotation(Node*& subTree)
{
  TRACE("OMAVLTree<Key, Value>::leftRotation");

  Node* temp = subTree;
  subTree = subTree->_right;
  temp->_right = subTree->_left;
  subTree->_left = temp;
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::rightRotation(Node*& subTree)
{
  TRACE("OMAVLTree<Key, Value>::rightRotation");

  Node* temp = subTree;
  subTree = subTree->_left;
  temp->_left = subTree->_right;
  subTree->_right = temp;
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::leftRightRotation(Node*& subTree)
{
  TRACE("OMAVLTree<Key, Value>::leftRightRotation");

  Node* tempLeft = subTree->_left;
  Node* temp = tempLeft->_right;
  tempLeft->_right = temp->_left;
  subTree->_left = temp->_right;
  temp->_left = tempLeft;
  temp->_right = subTree;

  if (temp->_balance == -1) {
    subTree->_balance = +1;
  } else {
    subTree->_balance = 0;
  }
  if (temp->_balance == +1) {
    tempLeft->_balance = -1;
  } else {
    tempLeft->_balance = 0;
  }
  temp->_balance = 0;
  subTree = temp;

}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::rightLeftRotation(Node*& subTree)
{
  TRACE("OMAVLTree<Key, Value>::rightLeftRotation");

  Node* tempRight = subTree->_right;
  Node* temp = tempRight->_left;
  tempRight->_left = temp->_right;
  subTree->_right = temp->_left;
  temp->_right = tempRight;
  temp->_left = subTree;

  if (temp->_balance == +1) {
    subTree->_balance = -1;
  } else {
    subTree->_balance = 0;
  }
  if (temp->_balance == -1) {
    tempRight->_balance = +1;
  } else {
    tempRight->_balance = 0;
  }
  temp->_balance = 0;
  subTree = temp;
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::traverseInOrder(
                         Node* subTree,
                         size_t height,
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMAVLTree<Key, Value>::traverseInOrder");

  if (subTree != 0) {
    traverseInOrder(subTree->_left, height + 1, f);
    (*f)(height, subTree->_key, subTree->_value);
    traverseInOrder(subTree->_right, height + 1, f);
  }
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::traverseInPreOrder(
                         Node* subTree,
                         size_t height,
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMAVLTree<Key, Value>::traverseInPreOrder");

  if (subTree != 0) {
    (*f)(height, subTree->_key, subTree->_value);
    traverseInOrder(subTree->_left, height + 1, f);
    traverseInOrder(subTree->_right, height + 1, f);
  }
}

template <typename Key, typename Value>
void OMAVLTree<Key, Value>::traverseInPostOrder(
                         Node* subTree,
                         size_t height,
                         void (*f)(size_t height, Key k, const Value& v)) const
{
  TRACE("OMAVLTree<Key, Value>::traverseInPostOrder");

  if (subTree != 0) {
    traverseInOrder(subTree->_left, height + 1, f);
    traverseInOrder(subTree->_right, height + 1, f);
    (*f)(height, subTree->_key, subTree->_value);
  }
}

