// @doc OMINTERNAL
#ifndef OMSTOREDVECTORINDEX_H
#define OMSTOREDVECTORINDEX_H

#include "OMPortability.h"
#include "OMTypes.h"

#include <stddef.h>

  // @class The in-memory representation of the on-disk index for a
  //        stored object vector.
class OMStoredVectorIndex {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStoredVectorIndex(size_t size);

    // @cmember Destructor.
  ~OMStoredVectorIndex(void);

    // @cmember The high water mark in the set of keys assigned to
    //          this <c OMStoredVectorIndex>.
    // @this const
  OMUInt32 highWaterMark(void) const;

    // @cmember Insert a new element in this <c OMStoredVectorIndex>
    //          at position <p position> with key <p key>.
  void insert(size_t position, OMUInt32 key);

    // @cmember The number of elements in this <c OMStoredVectorIndex>.
    // @this const
  size_t entries(void) const;

    // @cmember Iterate over the elements in this <c OMStoredVectorIndex>.
    // @this const
  void iterate(size_t& context, OMUInt32& key) const;

    // @cmember Is this <c OMStoredVectorIndex> valid ?
    // @this const
  bool isValid(void) const;

private:

  OMUInt32 _highWaterMark;
  size_t _size;
  size_t _entries;
  OMUInt32* _keys;
};

#endif
