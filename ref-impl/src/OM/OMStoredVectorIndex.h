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
#ifndef OMSTOREDVECTORINDEX_H
#define OMSTOREDVECTORINDEX_H

#include "OMDataTypes.h"

#include <stddef.h>

  // @class The in-memory representation of the on-disk index for a
  //        stored object vector.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStoredVectorIndex {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStoredVectorIndex(size_t capacity);

    // @cmember Destructor.
  ~OMStoredVectorIndex(void);

    // @cmember The first free key in the set of local keys assigned to
    //          this <c OMStoredVectorIndex>.
  OMUInt32 firstFreeKey(void) const;

    // @cmember Set the first free key in the set of local keys assigned to
    //          this <c OMStoredVectorIndex>.
  void setFirstFreeKey(OMUInt32 firstFreeKey);

    // @cmember The last free key in the set of local keys assigned to
    //          this <c OMStoredVectorIndex>.
  OMUInt32 lastFreeKey(void) const;

    // @cmember Set the last free key in the set of local keys assigned to
    //          this <c OMStoredVectorIndex>.
  void setLastFreeKey(OMUInt32 lastFreeKey);

    // @cmember Insert a new element in this <c OMStoredVectorIndex>
    //          at position <p position> with local key <p localKey>.
    //          The local key of an element is an integer.
    //          The local key is used to derive the name of the storage
    //          on which an element is saved. Local keys are assigned
    //          such that the names of existing elements do not have to
    //          change when other elements are added to or removed from
    //          the associated <c OMStrongReferenceVector>. The local key is
    //          independent of the element's logical or physical position
    //          within the associated <c OMStrongReferenceVector>.
  void insert(size_t position, OMUInt32 localKey);

    // @cmember The number of elements in this <c OMStoredVectorIndex>.
  size_t entries(void) const;

    // @cmember Iterate over the elements in this <c OMStoredVectorIndex>.
  void iterate(size_t& context, OMUInt32& localKey) const;

    // @cmember Is this <c OMStoredVectorIndex> valid ?
  bool isValid(void) const;

private:

  OMUInt32 _firstFreeKey;
  OMUInt32 _lastFreeKey;
  size_t _capacity;
  size_t _entries;
  OMUInt32* _localKeys;
};

#endif
