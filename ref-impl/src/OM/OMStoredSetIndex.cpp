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
OMStoredSetIndex::OMStoredSetIndex(size_t capacity)
: _highWaterMark(0), _capacity(capacity), _entries(0),
  _names(0), _referenceCounts(0), _keys(0)
{
  TRACE("OMStoredSetIndex::OMStoredSetIndex");

  _names = new OMUInt32[_capacity];
  ASSERT("Valid heap pointer", _names != 0);
  _referenceCounts = new OMUInt32[_capacity];
  ASSERT("Valid heap pointer", _referenceCounts != 0);
  _keys = new OMUniqueObjectIdentification[_capacity];
  ASSERT("Valid heap pointer", _keys != 0);

  for (size_t i = 0; i < _capacity; i++) {
    _names[i] = 0;
    _referenceCounts[i] = 0;
    memset(&_keys[i], 0, sizeof(_keys[i]));
  }
}

  // @mfunc Destructor.
OMStoredSetIndex::~OMStoredSetIndex(void)
{
  TRACE("OMStoredSetIndex::~OMStoredSetIndex");

  delete [] _names;
  _names = 0;
  delete [] _referenceCounts;
  _referenceCounts = 0;
  delete [] _keys;
  _keys = 0;
}

  // @mfunc The high water mark in the set of names assigned to
  //        this <c OMStoredSetIndex>.
  //   @rdesc The highest previously allocated key.
  //   @this const
OMUInt32 OMStoredSetIndex::highWaterMark(void) const
{
  TRACE("OMStoredSetIndex::highWaterMark");

  return _highWaterMark;
}

  // @mfunc Insert a new element in this <c OMStoredSetIndex>.
  //        The name of an element is an integer. Names are assigned
  //        such that the names of existing elements do not have to
  //        change when other elements are added to or removed from
  //        the associated <c OMStrongReferenceSet>. The name is
  //        independent of the element's logical or physical position
  //        within the associated <c OMStrongReferenceSet>. The name is
  //        also independent of the element's key.
  //   @parm The position at which the new element should be inserted.
  //   @parm The name assigned to the element.
  //   @parm The count of references to the element.
  //   @parm The unique key of the element.
void OMStoredSetIndex::insert(
                            size_t position,
                            OMUInt32 name,
                            OMUInt32 referenceCount,
                            const OMUniqueObjectIdentification& key)
{
  TRACE("OMStoredSetIndex::insert");
  PRECONDITION("Valid position", position < _capacity);

  // Assumes sequential insertion.

  _names[position] = name;
  _keys[position] = key;
  _referenceCounts[position] = referenceCount;
  _highWaterMark = _highWaterMark + 1;
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
  //   @parm The name of the "current" element.
  //   @parm The count of references to the "current" element.
  //   @parm The unique key of the "current" element.
  //   @this const
void OMStoredSetIndex::iterate(size_t& context,
                               OMUInt32& name,
                               OMUInt32& referenceCount,
                               OMUniqueObjectIdentification& key) const
{
  TRACE("OMStoredSetIndex::iterate");
  PRECONDITION("Valid context", context < _capacity);

  name = _names[context];
  referenceCount = _referenceCounts[context];
  key = _keys[context];
  context = context + 1;
}

  // @mfunc Is this <c OMStoredSetIndex> valid ?
  //   @rdesc True if this is a valid <c OMStoredSetIndex>, false otherwise.
  //   @this const
bool OMStoredSetIndex::isValid(void) const
{
  // No checks yet.
  // Possible checks include checking that all of the names are unique
  return true;
}
