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
#include "OMStoredSetIndex.h"
#include "OMAssertions.h"

  // @mfunc Constructor.
  //   @parm The capacity of this <c OMStoredSetIndex>.
OMStoredSetIndex::OMStoredSetIndex(size_t capacity,
                                   OMUInt32 keyPid,
                                   OMUInt32 keySize)
: _firstFreeKey(0), _capacity(capacity), _entries(0),
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
