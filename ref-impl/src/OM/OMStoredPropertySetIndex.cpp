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
#include "OMStoredPropertySetIndex.h"

#include "OMAssertions.h"

OMStoredPropertySetIndex::OMStoredPropertySetIndex(size_t capacity)
: _capacity(capacity), _table(0), _entries(0)
{
  TRACE("OMStoredPropertySetIndex::OMStoredPropertySetIndex");

  _table = new IndexEntry[_capacity];
  ASSERT("Valid heap pointer", _table != 0);

  for (size_t i = 0; i < _capacity; i++) {
    _table[i]._valid = false;
    _table[i]._propertyId = 0;
    _table[i]._storedForm = 0;
    _table[i]._length = 0;
    _table[i]._offset = 0;
  }
}

OMStoredPropertySetIndex::~OMStoredPropertySetIndex(void)
{
  TRACE("OMStoredPropertySetIndex::~OMStoredPropertySetIndex");

  delete [] _table;
  _table = 0;
}

  // @mfunc Insert a new property into this <c OMStoredPropertySetIndex>.
  //        The new property has id <p propertyId>. The stored property
  //        representation is <p storedForm>. The property value
  //        occupies <p length> bytes starting at offset <p offset>.
  //   @parm The id of the property to insert.
  //   @parm The stored form to use for the property.
  //   @parm The offset of the property value in bytes.
  //   @parm The size of the property value in bytes.
void OMStoredPropertySetIndex::insert(OMPropertyId propertyId,
                                      OMUInt32 storedForm,
                                      OMUInt32 offset,
                                      OMUInt32 length)
{
  TRACE("OMStoredPropertySetIndex::insert");

  IndexEntry* entry = find(propertyId);

  ASSERT("New index entry", entry == 0);
  if (entry == 0 ) {
    entry = find();
    ASSERT("Found space for new entry", entry != 0);
    _entries++;
  }
  ASSERT("Valid index entry", entry != 0);

  entry->_propertyId = propertyId;
  entry->_storedForm = storedForm;
  entry->_offset = offset;
  entry->_length = length;
  entry->_valid = true;
}

  // @mfunc The number of properties in this <c OMStoredPropertySetIndex>.
  //   @rdesc The number of properties.
  //   @this const
size_t OMStoredPropertySetIndex::entries(void) const
{
  TRACE("OMStoredPropertySetIndex::entries");

  return _entries;
}

  // @mfunc Iterate over the properties in this <c OMStoredPropertySetIndex>.
  //   @parm Iteration  context. Set this to 0 to start with the
  //         "first" property.
  //   @parm The id of the "current" property.
  //   @parm The stored form used for the "current" property.
  //   @parm The offset of the "current" property value in bytes.
  //   @parm The size of the "current" property value in bytes.
  //   @this const
void OMStoredPropertySetIndex::iterate(size_t& context,
                                       OMPropertyId& propertyId,
                                       OMUInt32& storedForm,
                                       OMUInt32& offset,
                                       OMUInt32& length) const
{
  TRACE("OMStoredPropertySetIndex::iterate");

  OMStoredPropertySetIndex::IndexEntry* entry = 0;
  size_t start = context;
  size_t found = 0;

  for (size_t i = start; i < _capacity; i++) {
    if (_table[i]._valid) {
      entry = &_table[i];
      found = i;
      break;
    }
  }
  if (entry != 0) {
    propertyId = entry->_propertyId;
    storedForm = entry->_storedForm;
    offset = entry->_offset;
    length = entry->_length;
    context = ++found;
  } else {
    context = 0;
  }
}

  // @mfunc Find the property with property id <p propertyId> in this
  //        <c OMStoredPropertySetIndex>. If found the <p storedForm>,
  //        <p offset> and <p length> of the property are returned.
  //   @parm The id of the property to find.
  //   @parm The stored form used for the property.
  //   @parm The offset of the property value in bytes.
  //   @parm The size of the property value in bytes.
  //   @rdesc True if a property with the given id was found, false otherwise.
  //   @this const  
bool OMStoredPropertySetIndex::find(const OMPropertyId& propertyId,
                                    OMUInt32& storedForm,
                                    OMUInt32& offset,
                                    OMUInt32& length) const
{
  bool result;

  OMStoredPropertySetIndex::IndexEntry* e = find(propertyId);
  if (e != 0) {
    storedForm = e->_storedForm;
    offset = e->_offset;
    length = e->_length;
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMStoredPropertySetIndex> valid ?
  //   @rdesc True if this <c OMStoredPropertySetIndex> is valid,
  //          false otherwise.
  //   @this const
bool OMStoredPropertySetIndex::isValid(OMUInt32 baseOffset) const
{
  TRACE("OMStoredPropertySetIndex::isValid");

  // The validity constraints are ...
  // 1) Each entry must have a non-zero length
  // 2) Entries must not overlap
  // 3) Entries must be in order of offset
  // 4) There must be no gaps between entries
  // We may choose to relax 3 and 4 in the future
  bool result = true;
  size_t entries = 0;
  size_t currentOffset;
  size_t currentLength;
  size_t position = baseOffset;

  for (size_t i = 0; i < _capacity; i++) {
    if (_table[i]._valid) {
      entries++; // count valid entries
      currentOffset = _table[i]._offset;
      currentLength = _table[i]._length;
      if (currentLength == 0) {
        result = false; // entry has invalid length
        break;
      }
      if (currentOffset != position) {
        result = false;  // gap or overlap
        break;
	  }
      // this entry is valid, calculate the expected next position
      position = position + currentLength;
    }
  }

  if (entries != _entries) {
    result = false;
  }
  
  return result;
}

OMStoredPropertySetIndex::IndexEntry* OMStoredPropertySetIndex::find(
                                                                    void) const
{
  TRACE("OMStoredPropertySetIndex::find");

  OMStoredPropertySetIndex::IndexEntry* result = 0;

  for (size_t i = 0; i < _capacity; i++) {
    if (!_table[i]._valid) {
      result = &_table[i];
      break;
    }
  }
  return result;
}

OMStoredPropertySetIndex::IndexEntry* OMStoredPropertySetIndex::find(
                                                 OMPropertyId propertyId) const
{
  TRACE("OMStoredPropertySetIndex::find");

  OMStoredPropertySetIndex::IndexEntry* result = 0;

  for (size_t i = 0; i < _capacity; i++) {
    if (_table[i]._valid) {
      if (_table[i]._propertyId == propertyId) {
        result = &_table[i];
        break;
      }
    }
  }
  return result;

}
