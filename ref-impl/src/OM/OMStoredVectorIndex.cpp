#include "OMStoredVectorIndex.h"
#include "OMAssertions.h"

OMStoredVectorIndex::OMStoredVectorIndex(size_t size)
: _highWaterMark(0), _size(size), _entries(0), _names(0)
{
  TRACE("OMStoredVectorIndex::OMStoredVectorIndex");

  _names = new size_t[_size];
  for (size_t i = 0; i < _size; i++) {
    _names[i] = 0;
  }
}

OMStoredVectorIndex::~OMStoredVectorIndex(void)
{
  TRACE("OMStoredVectorIndex::~OMStoredVectorIndex");

  delete [] _names;
  _names = 0;
}

size_t OMStoredVectorIndex::highWaterMark(void) const
{
  TRACE("OMStoredVectorIndex::highWaterMark");
  return _highWaterMark;
}

void OMStoredVectorIndex::insert(size_t position, size_t name)
{
  TRACE("OMStoredVectorIndex::insert");
  PRECONDITION("Valid position", position < _size);
  _names[position] = name;
  _highWaterMark = _highWaterMark + 1;
  _entries = _entries + 1;
}

size_t OMStoredVectorIndex::entries(void) const
{
  TRACE("OMStoredVectorIndex::entries");
  return _entries;
}

void OMStoredVectorIndex::iterate(size_t& context, size_t& name) const
{
  TRACE("OMStoredVectorIndex::iterate");
  PRECONDITION("Valid context", context < _size);
  name = _names[context];
  context = context + 1;
}
