#ifndef OMCLASSFACTORY_H
#define OMCLASSFACTORY_H

#include "OMPortability.h"
#include "OMTypes.h"

class OMStorable;

class OMClassFactory {
public:

  // Create with space for `capacity' entries.
  //
  OMClassFactory(int capacity);

  // Register a class id and its associated creation function.
  //
  void add(const OMClassId& classId, OMStorable* (*create)(const OMClassId&));

  // Deregister a class id.
  //
  void remove(const OMClassId& classId);

  // Create an instance of the appropriate derived class, given the class id.
  //
  OMStorable* create(const OMClassId& classId) const;

protected:

  struct FactoryEntry;

  // FactoryEntry for `classId' or null if not found.
  //
  FactoryEntry* find(const OMClassId& classId) const;

  // First free entry or null if full.
  //
  FactoryEntry* find(void) const;

  static bool equal(const OMClassId& ida, const OMClassId& idb);

private:

  struct FactoryEntry {
    OMClassId _classId;
    OMStorable* (*_creationFunction)(const OMClassId&);
    int _valid;
  };

  int _capacity;        // Number of potential entries.
  FactoryEntry* _table; // Dynamically allocated array.

};

#endif
