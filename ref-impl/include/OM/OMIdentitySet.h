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
#ifndef OMIDENTITYSET_H
#define OMIDENTITYSET_H

#include "OMContainer.h"
#include "OMRedBlackTree.h"

#include <stddef.h>

template <typename Element>
class OMIdentitySetIterator;

  // @class Sets of unique elements. Duplicate elements
  //        are not allowed.
  //   @tcarg class | Element | The type of the unique elements.
  //          This type must support operator =, operator !=,
  //          operator == and operator <lt>.
  //   @base public | <c OMContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMIdentitySet : public OMContainer<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMIdentitySet();

    // @cmember Destructor.
  virtual ~OMIdentitySet(void);

    // @cmember Insert <p element> into this <c OMIdentitySet>.
    //          @precondition <f !contains(element)>
  virtual void insert(const Element& element);

    // @cmember If it is not already present, insert <p element> into this
    //          <c OMIdentitySet> and return true, otherwise return false.
  bool ensurePresent(const Element& element);

    // @cmember Does this <c OMIdentitySet> contain <p element> ?
  virtual bool contains(const Element& element) const;

    // @cmember The number of elements in this <c OMIdentitySet>.
  size_t count(void) const;

    // @cmember Remove the <p element> from this <c OMIdentitySet>.
    //          @precondition <f contains(element)>
  virtual void remove(const Element& element);

    // @cmember Remove all elements from this <c OMIdentitySet>.
  virtual void clear(void);

    // @cmember If it is present, remove <p element> from this
    //          <c OMIdentitySet> and return true, otherwise return false.
  bool ensureAbsent(const Element& element);

private:
  // @access Private members.

    // The set is implemented as a binary search tree.
    //
  OMRedBlackTree<Element, int> _tree;

  friend class OMIdentitySetIterator<Element>;

};

#include "OMIdentitySetT.h"

#endif
