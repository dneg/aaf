#ifndef OMSTOREDPROPERTYSETINDEX_H
#define OMSTOREDPROPERTYSETINDEX_H

#include "OMPortability.h"
#include "OMTypes.h"

class OMStoredPropertySetIndex {
private:
  struct IndexEntry;  
public:
  
  OMStoredPropertySetIndex(size_t capacity);

  ~OMStoredPropertySetIndex(void);

  void insert(OMPropertyId propertyId,
              OMUInt32 type,
              OMUInt32 offset,
              OMUInt32 length);
  
  IndexEntry* find(OMPropertyId propertyId) const;

  size_t entries(void) const;

  void iterate(size_t& context,
               OMPropertyId& propertyId,
               OMUInt32& type,
               OMUInt32& offset,
               OMUInt32& length) const;

  bool isSorted(void);

protected:
    
  OMStoredPropertySetIndex::IndexEntry* find(void) const;
    
private:
    
  struct IndexEntry {
    OMPropertyId _propertyId;
    OMUInt32 _type;
    OMUInt32 _offset;
    OMUInt32 _length;
    bool _valid;
  };
    
  size_t _capacity;
  IndexEntry* _table;
  size_t _entries;
  bool _dirty;
};

#endif
