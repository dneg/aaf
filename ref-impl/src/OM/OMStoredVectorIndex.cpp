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
#include "OMStoredVectorIndex.h"
#include "OMAssertions.h"

OMStoredVectorIndex::OMStoredVectorIndex(size_t size)
: _highWaterMark(0), _size(size), _entries(0), _keys(0)
{
  TRACE("OMStoredVectorIndex::OMStoredVectorIndex");

  _keys = new OMUInt32[_size];
  ASSERT("Valid heap pointer", _keys != 0);

  for (size_t i = 0; i < _size; i++) {
    _keys[i] = 0;
  }
}

OMStoredVectorIndex::~OMStoredVectorIndex(void)
{
  TRACE("OMStoredVectorIndex::~OMStoredVectorIndex");

  delete [] _keys;
  _keys = 0;
}

  // @mfunc The high water mark in the set of keys assigned to
  //        this <c OMStoredVectorIndex>.
  //   @rdesc The highest previously allocated key.
  //   @this const
OMUInt32 OMStoredVectorIndex::highWaterMark(void) const
{
  TRACE("OMStoredVectorIndex::highWaterMark");

  return _highWaterMark;
}

  // @mfunc Insert a new element in this <c OMStoredVectorIndex>
  //        at position <p position> with key <p key>.
  //   @parm The position at which the new element should be inserted.
  //   @parm The key assigned to the element.
void OMStoredVectorIndex::insert(size_t position, OMUInt32 key)
{
  TRACE("OMStoredVectorIndex::insert");
  PRECONDITION("Valid position", position < _size);

  // Assumes sequential insertion.

  _keys[position] = key;
  _highWaterMark = _highWaterMark + 1;
  _entries = _entries + 1;
}

  // @mfunc The number of elements in this <c OMStoredVectorIndex>.
  //   @rdesc The number of elements.
  //   @this const
size_t OMStoredVectorIndex::entries(void) const
{
  TRACE("OMStoredVectorIndex::entries");

  return _entries;
}

  // @mfunc Iterate over the elements in this <c OMStoredVectorIndex>.
  //   @parm Iteration context. Set this to 0 to start with the
  //         "first" element.
  //   @parm The key of the "current" element.
  //   @this const
void OMStoredVectorIndex::iterate(size_t& context, OMUInt32& key) const
{
  TRACE("OMStoredVectorIndex::iterate");
  PRECONDITION("Valid context", context < _size);

  key = _keys[context];
  context = context + 1;
}

  // @mfunc Is this <c OMStoredVectorIndex> valid ?
  //   @rdesc True if this is a valid <c OMStoredVectorIndex>, false otherwise.
  //   @this const
bool OMStoredVectorIndex::isValid(void) const
{
  // No checks yet.
  // Possible checks include checking that all of the names are unique
  return true;
}
