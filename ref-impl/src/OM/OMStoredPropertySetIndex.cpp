#include "OMStoredPropertySetIndex.h"

#include "OMAssertions.h"

OMStoredPropertySetIndex::OMStoredPropertySetIndex(size_t capacity)
: _capacity(capacity), _table(0), _entries(0), _dirty(false)
{
  TRACE("OMStoredPropertySetIndex::OMStoredPropertySetIndex");

  _table = new IndexEntry[_capacity];
  ASSERT("Valid heap pointer", _table != 0);

  for (size_t i = 0; i < _capacity; i++) {
    _table[i]._valid = false;
    _table[i]._propertyId = 0;
    _table[i]._type = 0;
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

void OMStoredPropertySetIndex::insert(OMPropertyId propertyId,
                                      OMUInt32 type,
                                      OMUInt32 offset,
                                      OMUInt32 length)
{
  TRACE("OMStoredPropertySetIndex::insert");

  IndexEntry* entry = find(propertyId);

  if (entry == 0 ) {
    entry = find();
    if (entry != 0) {
      entry->_propertyId = propertyId;
      entry->_type = type;
      entry->_offset = offset;
      entry->_length = length;
      entry->_valid = true;
      _entries++;
    } else {
      // error - no free slots
    }
  } else {
    // already in index
  }

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

size_t OMStoredPropertySetIndex::entries(void) const
{
  TRACE("OMStoredPropertySetIndex::entries");

  return _entries;
}

void OMStoredPropertySetIndex::iterate(size_t& context,
                                       OMPropertyId& propertyId,
                                       OMUInt32& type,
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
    type = entry->_type;
    offset = entry->_offset;
    length = entry->_length;
    context = ++found;
  } else {
    context = 0;
  }
}

bool OMStoredPropertySetIndex::isValid(void) const
{
  TRACE("OMStoredPropertySetIndex::isValid");

  bool result = true;
  size_t entries = 0;
  size_t position;
  bool firstEntry = true;
  size_t previousOffset;
  size_t currentOffset;
  size_t currentLength;

  for (size_t i = 0; i < _capacity; i++) {
    if (_table[i]._valid) {
      entries++; // count valid entries
      currentOffset = _table[i]._offset;
      currentLength = _table[i]._length;
      if (currentLength <= 0) {
        result = false; // entry has invalid length
        break;
      }
      if (firstEntry) {
        previousOffset = currentOffset;
        position = currentOffset + currentLength;
        firstEntry = false;
      } else {
        if (currentOffset <= previousOffset) {
          result = false; // entries out of order
          break;
        } else if (position > currentOffset) {
          result = false; // entries overlap
          break; 
        } else {
          // this entry is valid
          previousOffset = currentOffset;
          position = position + currentLength;
        }
      }
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
