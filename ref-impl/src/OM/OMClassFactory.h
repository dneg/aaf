#ifndef OMCLASSFACTORY_H
#define OMCLASSFACTORY_H

#include "OMPortability.h"

class OMStorable;

class OMClassFactory {
public:

  // Create with space for `capacity' entries.
  //
  OMClassFactory(int capacity);

  // Register a class id and its associated creation function.
  //
  void add(int classId, OMStorable* (*create)(void));

  // Deregister a class id.
  //
  void remove(int classId);

  // Create an instance of the appropriate derived class, given the class id.
  //
  OMStorable* create(int classId) const;

protected:

  struct FactoryEntry;

  // FactoryEntry for `classId' or null if not found.
  //
  FactoryEntry* find(int classId) const;

  // First free entry or null if full.
  //
  FactoryEntry* find(void) const;

private:

  struct FactoryEntry {
    int _classId;
    OMStorable* (*_creationFunction)(void);
    int _valid;
  };

  int _capacity;        // Number of potential entries.
  FactoryEntry* _table; // Dynamically allocated array.

};

#endif
