#ifndef OMCLASSFACTORY_H
#define OMCLASSFACTORY_H

#include "OMPortability.h"
#include "OMTypes.h"

class OMStorable;

class OMClassFactory {
public:

  // Create an instance of the appropriate derived class, given the class id.
  //
  virtual OMStorable* create(const OMClassId& classId) const = 0;

};

#endif
