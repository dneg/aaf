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
#ifndef OMSTOREDSETINDEX_H
#define OMSTOREDSETINDEX_H

#include "OMPortability.h"
#include "OMDataTypes.h"

#include <stddef.h>

  // @class The in-memory representation of the on-disk index for a
  //        stored object set.
class OMStoredSetIndex {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStoredSetIndex(size_t capacity, OMUInt32 keyPid, OMUInt32 keySize);

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
  OMUInt32 _keySize; // The size of a key
  OMUInt32* _localKeys;
  OMUInt32* _referenceCounts;
  OMByte* _keys;
};

#endif
