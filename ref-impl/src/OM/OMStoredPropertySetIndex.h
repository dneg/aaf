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

  void insert(OMPropertyId propertyId, int type, size_t offset, size_t length);
  
  IndexEntry* find(OMPropertyId propertyId) const;

  size_t entries(void) const;

  void iterate(size_t& context,
               OMPropertyId& propertyId,
               int& type,
               size_t& offset,
               size_t& length) const;

  bool isSorted(void);

protected:
    
  OMStoredPropertySetIndex::IndexEntry* find(void) const;
    
private:
    
  struct IndexEntry {
    OMPropertyId _propertyId;
    int _type;
    size_t _offset;
    size_t _length;
    bool _valid;
  };
    
  size_t _capacity;
  IndexEntry* _table;
  size_t _entries;
  bool _dirty;
};

#endif
