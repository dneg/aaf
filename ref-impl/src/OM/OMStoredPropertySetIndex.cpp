#include "OMStoredPropertySetIndex.h"

OMStoredPropertySetIndex::OMStoredPropertySetIndex(size_t capacity)
: _capacity(capacity), _table(0), _entries(0), _dirty(false)
{
  _table = new IndexEntry[_capacity];
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
  delete [] _table;
  _table = 0;
}

void OMStoredPropertySetIndex::insert(OMPropertyId propertyId,
                                      OMUInt32 type,
                                      OMUInt32 offset,
                                      OMUInt32 length)
{
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
  return _entries;
}

void OMStoredPropertySetIndex::iterate(size_t& context,
                                       OMPropertyId& propertyId,
                                       OMUInt32& type,
                                       OMUInt32& offset,
                                       OMUInt32& length) const
{
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

bool OMStoredPropertySetIndex::isSorted(void)
{
  bool result = true;
  bool haveLastOffset = false;
  size_t lastOffset;
  
  for (size_t i = 0; i < _capacity; i++) {
    if (_table[i]._valid) {
      if (haveLastOffset) {
        if (_table[i]._offset <= lastOffset) {
          result = false;
          break;
        }
      } else {
        lastOffset = _table[i]._offset;
        haveLastOffset = true;
      }
    }
  }
  
  return result;
}

OMStoredPropertySetIndex::IndexEntry* OMStoredPropertySetIndex::find(
                                                                    void) const
{
  OMStoredPropertySetIndex::IndexEntry* result = 0;

  for (size_t i = 0; i < _capacity; i++) {
    if (!_table[i]._valid) {
      result = &_table[i];
      break;
    }
  }
  return result;
}
