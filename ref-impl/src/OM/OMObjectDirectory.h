#ifndef OMOBJECTDIRECTORY_H
#define OMOBJECTDIRECTORY_H

#include "OMPortability.h"

class OMStorable;

class OMObjectDirectory {
public:

  // Create with space for `capacity' entries.
  //
  OMObjectDirectory(int capacity);

  ~OMObjectDirectory(void);

  //
  //
  bool lookup(const char* name, const OMStorable*& p) const;

  //
  //
  void insert(const char* name, const OMStorable* p);

  // Count of valid entries.
  //
  int count(void) const;

  void destroyAll(void (*destroy)(OMStorable*&));

  void dump(void) const;

private:

  struct TableEntry {
    OMStorable* _object;
  char* _name;
  };
  int _capacity;        // Number of potential entries.
  int _current;         // Index of first unoccupied entry.
  TableEntry* _table;   // Dynamically allocated array.

};

#endif
