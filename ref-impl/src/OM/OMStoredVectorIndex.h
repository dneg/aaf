#ifndef OMSTOREDVECTORINDEX_H
#define OMSTOREDVECTORINDEX_H

#include "OMPortability.h"
#include "OMTypes.h"

class OMStoredVectorIndex {
public:
  OMStoredVectorIndex(size_t size);
  ~OMStoredVectorIndex(void);
  OMUInt32 highWaterMark(void) const;
  void insert(size_t position, OMUInt32 name);
  size_t entries(void) const;
  void iterate(size_t& context, OMUInt32& name) const;
private:
  OMUInt32 _highWaterMark;
  size_t _size;
  size_t _entries;
  OMUInt32* _names;
};

#endif
