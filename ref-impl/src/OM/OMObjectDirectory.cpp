#include "OMObjectDirectory.h"
#include "OMAssertions.h"
#include "OMPortability.h"

#include <string.h>
#include <iostream.h>

OMObjectDirectory::OMObjectDirectory(int capacity)
: _capacity(capacity), _current(0)
{
  _table = new TableEntry[_capacity];
  for (int i = 0; i < _capacity; i++) {
    _table[i]._object = 0;
    _table[i]._name = 0;
  }
}

OMObjectDirectory::~OMObjectDirectory(void)
{
  delete [] _table;
  _table = 0;
}

bool OMObjectDirectory::lookup(const char* name, const OMStorable*& p) const
{
  TRACE("OMObjectDirectory::lookup");
  PRECONDITION("Valid name to look up", validString(name));
  bool result = false;
  
  for (int i = 0; i < _current; i++) {
    int status = strcmp(name, _table[i]._name);
    if (status == 0) {
      result = true;
      p = _table[i]._object;
      break;
    }
  }
  return result;
}

void OMObjectDirectory::insert(const char* name, const OMStorable* p)
{
  if (_current < _capacity) {
    char* n = new char[strlen(name) + 1];
    strcpy(n , name);
    _table[_current]._object = const_cast<OMStorable *>(p);
    _table[_current]._name = n;
    _current++;
  }
}

int OMObjectDirectory::count(void) const
{
  return _current;
}

void OMObjectDirectory::dump(void) const
{
  for (int i = 0; i < _current; i++) {
    cout << i << " [" << _table[i]._object << "] \"" << _table[i]._name << "\"" << endl;
  }
}
