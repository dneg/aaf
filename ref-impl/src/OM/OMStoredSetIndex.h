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
#ifndef OMSTOREDSETINDEX_H
#define OMSTOREDSETINDEX_H

#include "OMDataTypes.h"

#include <stddef.h>

  // @class The in-memory representation of the on-disk index for a
  //        stored object set.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStoredSetIndex {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStoredSetIndex(size_t capacity, OMPropertyId keyPid, OMKeySize keySize);

    // @cmember Destructor.
  ~OMStoredSetIndex(void);

    // @cmember The first free key in the set of local keys assigned to
    //          this <c OMStoredSetIndex>.
  OMUInt32 firstFreeKey(void) const;

    // @cmember Set the first free key in the set of local keys assigned to
    //          this <c OMStoredSetIndex>.
  void setFirstFreeKey(OMUInt32 firstFreeKey);

    // @cmember The last free key in the set of local keys assigned to
    //          this <c OMStoredSetIndex>.
  OMUInt32 lastFreeKey(void) const;

    // @cmember Set the last free key in the set of local keys assigned to
    //          this <c OMStoredSetIndex>.
  void setLastFreeKey(OMUInt32 lastFreeKey);

  size_t keySize(void) const;

  OMPropertyId keyPropertyId(void) const;

    // @cmember Insert a new element in this <c OMStoredSetIndex>.
    //          The local key of an element is an integer.
    //          The local key is used to derive the name of the storage
    //          on which an element is saved. Local keys are assigned
    //          such that the names of existing elements do not have to
    //          change when other elements are added to or removed from
    //          the associated <c OMStrongReferenceSet>. The local key is
    //          independent of the element's logical or physical position
    //          within the associated <c OMStrongReferenceSet>. The local
    //          key is also independent of the element's unique key.
  void insert(size_t position,
              OMUInt32 localKey,
              OMUInt32 referenceCount,
              void* key);

    // @cmember The number of elements in this <c OMStoredSetIndex>.
  size_t entries(void) const;

    // @cmember Iterate over the elements in this <c OMStoredSetIndex>.
  void iterate(size_t& context,
               OMUInt32& localKey,
               OMUInt32& referenceCount,
               void* key) const;

    // @cmember Is this <c OMStoredSetIndex> valid ?
  bool isValid(void) const;

private:

  OMUInt32 _firstFreeKey;
  OMUInt32 _lastFreeKey;
  size_t _capacity;
  size_t _entries;
  OMPropertyId _keyPropertyId; // Id of property that is the key
  OMKeySize _keySize; // The size of a key
  OMUInt32* _localKeys;
  OMUInt32* _referenceCounts;
  OMByte* _keys;
};

#endif
