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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStoredSetIndex

#include "OMStoredSetIndex.h"
#include "OMAssertions.h"

  // @mfunc Constructor.
  //   @parm The capacity of this <c OMStoredSetIndex>.
OMStoredSetIndex::OMStoredSetIndex(size_t capacity,
                                   OMPropertyId keyPid,
                                   OMKeySize keySize)
: _firstFreeKey(0), _lastFreeKey(~(OMUInt32)0),
  _capacity(capacity), _entries(0),
  _keyPropertyId(keyPid), _keySize(keySize),
  _localKeys(0), _referenceCounts(0), _keys(0)
{
  TRACE("OMStoredSetIndex::OMStoredSetIndex");

  _localKeys = new OMUInt32[_capacity];
  ASSERT("Valid heap pointer", _localKeys != 0);
  _referenceCounts = new OMUInt32[_capacity];
  ASSERT("Valid heap pointer", _referenceCounts != 0);
  _keys = new OMByte[_capacity * _keySize];
  ASSERT("Valid heap pointer", _keys != 0);

  for (size_t i = 0; i < _capacity; i++) {
    _localKeys[i] = 0;
    _referenceCounts[i] = 0;
    memset(&_keys[i * _keySize], 0, _keySize);
  }
}

  // @mfunc Destructor.
OMStoredSetIndex::~OMStoredSetIndex(void)
{
  TRACE("OMStoredSetIndex::~OMStoredSetIndex");

  delete [] _localKeys;
  _localKeys = 0;
  delete [] _referenceCounts;
  _referenceCounts = 0;
  delete [] _keys;
  _keys = 0;
}

  // @mfunc The first free key in the set of local keys assigned to
  //        this <c OMStoredSetIndex>.
  //   @rdesc The highest previously allocated local key.
  //   @this const
OMUInt32 OMStoredSetIndex::firstFreeKey(void) const
{
  TRACE("OMStoredSetIndex::firstFreeKey");

  return _firstFreeKey;
}

  // @mfunc Set the first free key in the set of local keys assigned to
  //        this <c OMStoredSetIndex>.
  //   @parm The highest allocated local key.
void OMStoredSetIndex::setFirstFreeKey(OMUInt32 firstFreeKey)
{
  TRACE("OMStoredSetIndex::setFirstFreeKey");

  _firstFreeKey = firstFreeKey;
}

  // @mfunc The last free key in the set of local keys assigned to
  //        this <c OMStoredSetIndex>.
  //   @rdesc The highest previously allocated local key.
  //   @this const
OMUInt32 OMStoredSetIndex::lastFreeKey(void) const
{
  TRACE("OMStoredSetIndex::lastFreeKey");

  return _lastFreeKey;
}

  // @mfunc Set the last free key in the set of local keys assigned to
  //        this <c OMStoredSetIndex>.
  //   @parm The highest allocated local key.
void OMStoredSetIndex::setLastFreeKey(OMUInt32 lastFreeKey)
{
  TRACE("OMStoredSetIndex::setLastFreeKey");

  _lastFreeKey = lastFreeKey;
}

size_t OMStoredSetIndex::keySize(void) const
{
  return _keySize;
}

OMPropertyId OMStoredSetIndex::keyPropertyId(void) const
{
  return _keyPropertyId;
}

  // @mfunc Insert a new element in this <c OMStoredSetIndex>.
  //        The local key of an element is an integer.
  //        The local key is used to derive the name of the storage
  //        on which an element is saved. Local keys are assigned
  //        such that the names of existing elements do not have to
  //        change when other elements are added to or removed from
  //        the associated <c OMStrongReferenceSet>. The local key is
  //        independent of the element's logical or physical position
  //        within the associated <c OMStrongReferenceSet>. The local
  //        key is also independent of the element's unique key.
  //   @parm The position at which the new element should be inserted.
  //   @parm The local key assigned to the element.
  //   @parm The count of references to the element.
  //   @parm The unique key of the element.
void OMStoredSetIndex::insert(
                            size_t position,
                            OMUInt32 localKey,
                            OMUInt32 referenceCount,
                            void* key)
{
  TRACE("OMStoredSetIndex::insert");
  PRECONDITION("Valid position", position < _capacity);

  _localKeys[position] = localKey;
  memcpy(&_keys[position * _keySize], key, _keySize);
  _referenceCounts[position] = referenceCount;
  _entries = _entries + 1;
}

  // @mfunc The number of elements in this <c OMStoredSetIndex>.
  //   @rdesc The number of elements.
  //   @this const
size_t OMStoredSetIndex::entries(void) const
{
  TRACE("OMStoredSetIndex::entries");

  return _entries;
}

  // @mfunc Iterate over the elements in this <c OMStoredSetIndex>.
  //   @parm Iteration context. Set this to 0 to start with the
  //         "first" element.
  //   @parm The local key of the "current" element.
  //   @parm The count of references to the "current" element.
  //   @parm The unique key of the "current" element.
  //   @this const
void OMStoredSetIndex::iterate(size_t& context,
                               OMUInt32& localKey,
                               OMUInt32& referenceCount,
                               void* key) const
{
  TRACE("OMStoredSetIndex::iterate");
  PRECONDITION("Valid context", context < _capacity);

  localKey = _localKeys[context];
  referenceCount = _referenceCounts[context];
  memcpy(key, &_keys[context * _keySize], _keySize);
  context = context + 1;
}

  // @mfunc Is this <c OMStoredSetIndex> valid ?
  //   @rdesc True if this is a valid <c OMStoredSetIndex>, false otherwise.
  //   @this const
bool OMStoredSetIndex::isValid(void) const
{
  // No checks yet.
  // Possible checks include checking that all of the local keys are unique
  // and that all of the unique keys are in fact unique.
  return true;
}
