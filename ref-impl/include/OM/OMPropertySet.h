#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMPortability.h"

#define MAXIMUMPROPERTIES (10)

class OMProperty;
class OMStorable;

class OMPropertySet {
public:
  OMPropertySet(void);

  OMProperty* get(const int pid) const;
  void put(OMProperty* p);
  size_t count(void) const;
  void setContainingObject(const OMStorable* containingObject);
private:
  OMProperty* _propertySet[MAXIMUMPROPERTIES];
  size_t _count;
  const OMStorable* _containingObject;
};

#endif
