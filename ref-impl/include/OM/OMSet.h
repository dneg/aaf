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
#ifndef OMSET_H
#define OMSET_H

#include "OMContainer.h"
#include "OMRedBlackTree.h"

#include <stddef.h>

template <typename Key, typename Element>
class OMSetIterator;

  // @class Sets of uniquely identified elements. Duplicate elements
  //        are not allowed.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @base public | <c OMContainer>
template <typename Key, typename Element>
class OMSet : public OMContainer<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMSet();

    // @cmember Destructor.
  virtual ~OMSet(void);

    // @cmember Insert <p value> into this <c OMSet>.
  virtual void insert(const Element value);

    // @cmember Does this <c OMSet> contain <p value> ?
  virtual bool containsValue(const Element value) const;

    // @cmember Does this <c OMSet> contain an <p Element>
    //          identified by <p key>?
  virtual bool contains(const Key key) const;

    // @cmember Find the <p Element> in this <c OMSet> identified by
    //          <p key>.  If the element is found it is returned in
    //          <p value> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const Key key, Element& value) const;

    // @cmember Find the <p Element> in this <c OMSet> identified by
    //          <p key>.  If the element is found it is returned in
    //          <p value> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const Key key, Element** value) const;

    // @cmember The number of elements in this <c OMSet>.
    //          <mf OMSet::count> returns the actual number
    //          of elements in the <c OMSet>.
  size_t count(void) const;

    // @cmember Append the given <p Element> <p value> to
    //          this <c OMSet>.
  void append(const Element value);

    // @cmember Remove the <p Element> with <p Key> <p key>
    //          from this <c OMSet>.
  virtual void remove(const Key key);

    // @cmember Remove <p value> from this <c OMSet>.
  virtual void removeValue(const Element value);

private:
  // @access Private members.

    // The set is implemented as a binary search tree.
    //
  OMRedBlackTree<Key, Element> _tree;

  friend class OMSetIterator<Key, Element>;

};

#include "OMSetT.h"

#endif
