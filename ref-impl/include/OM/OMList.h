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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMList
#ifndef OMLIST_H
#define OMLIST_H

#include "OMContainer.h"

#include <stddef.h>

template <typename Element>
class OMListIterator;

  // @class Elastic sequential collections of elements.
  //        The order of elements is determined externally.
  //        Duplicate elements are allowed.
  //   @tcarg class | Element | The type of an <c OMList> element.
  //          This type must support operator = and operator ==.
  //   @base public | <c OMContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMList : public OMContainer<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMList();

    // @cmember Destructor.
  virtual ~OMList(void);

    // @cmember Insert <p value> into this <c OMList>.
  virtual void insert(const Element value);

  virtual void insertAfter(const Element value,
                           const OMListIterator<Element>& position);

  virtual void insertBefore(const Element value,
                            const OMListIterator<Element>& position);

    // @cmember Does this <c OMList> contain <p value> ?
  virtual bool containsValue(const Element value) const;

    // @cmember The number of elements in this <c OMList>.
    //          <mf OMList::count> returns the actual number
    //          of elements in the <c OMList>.
  size_t count(void) const;

  OMListIterator<Element> findValue(const Element& value);

    // @cmember Remove <p value> from this <c OMList>.
    //          In the case of duplicate values, the one closest to
    //          the head of the list is removed.
  virtual void removeValue(const Element value);

  virtual void remove(OMListIterator<Element>& position);

    // @cmember Is this <c OMList> empty ?
  virtual bool empty(void) const;

    // @cmember Append the given <p Element> <p value> to
    //          this <c OMList>. The new element is added after
    //          the last element currently in this <c OMList>.
  void append(const Element value);

    // @cmember Prepend the given <p Element> <p value> to
    //          this <c OMList>. The new element is added before
    //          the first element currently in this <c OMList>.
  void prepend(const Element value);

    // @cmember Remove the last element from this <c OMList>.
  void removeLast(void);

    // @cmember Remove the first element from this <c OMList>.
  void removeFirst(void);

    // @cmember Remove all elements from this <c OMList>.
  virtual void clear(void);

    // @cmember The number of elements with value <p value>.
  size_t countValue(const Element value) const;

  OMListIterator<Element> first(void) const;

  OMListIterator<Element> last(void) const;

private:
  // @access Private members.

  struct Node;
  Node* _nil;
  size_t _count;

  struct Node {
    Node* _next;
    Node* _previous;
    Element _value;
  };

  virtual void insertAfter(const Element value, Node* node);

  virtual void insertBefore(const Element value, Node* node);

  virtual void remove(Node* node);

  friend class OMListIterator<Element>;

};

#include "OMListT.h"

#endif
