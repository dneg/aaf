#ifndef OMSTOREDVECTORINDEX_H
#define OMSTOREDVECTORINDEX_H

#include "OMPortability.h"

class OMStoredVectorIndex {
public:
  OMStoredVectorIndex(size_t size);
  size_t highWaterMark(void) const;
  void insert(size_t position, size_t name);
  size_t entries(void) const;
  void iterate(size_t& context, size_t& name) const;
private:
  size_t _highWaterMark;
  size_t _size;
  size_t _entries;
  size_t* _names;
};

#endif