#ifndef OMSTOREDPROPERTYSETINDEX_H
#define OMSTOREDPROPERTYSETINDEX_H

#include "OMPortability.h"

class OMStoredPropertySetIndex {
public:
  
  OMStoredPropertySetIndex(size_t capacity);

  ~OMStoredPropertySetIndex(void);

  void insert(int pid, int type, size_t offset, size_t length);
  
  struct IndexEntry;
  
  IndexEntry* find(int pid) const;

  size_t entries(void) const;

  void iterate(size_t& context,
               int& pid,
               int& type,
               size_t& offset,
               size_t& length) const;

  bool isSorted(void);

protected:
    
  OMStoredPropertySetIndex::IndexEntry* find(void) const;
    
private:
    
  struct IndexEntry {
    int _pid;
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
