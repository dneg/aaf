//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMIDENTITYSET_H
#define OMIDENTITYSET_H

#include "OMDataTypes.h"
#include "OMContainer.h"
#include "OMRedBlackTree.h"

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
  OMUInt32 count(void) const;

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
