#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMPortability.h"

class OMProperty;
class OMStorable;

class OMPropertySet {
public:

  OMPropertySet(void);

  ~OMPropertySet(void);

  OMProperty* get(const int pid) const;
  void put(OMProperty* property);
  void iterate(size_t& context, OMProperty*& property) const;
  bool contains(const int pid) const;
  size_t count(void) const;
  void setContainingObject(const OMStorable* containingObject);
protected:
  static bool equal(const int& pida, const int& pidb);

private:

  struct OMPropertySetElement;

  // OMPropertySetElement for 'pid' or null if not found.
  //
  OMPropertySetElement* find(const int pid) const;

  // First free entry or null if full.
  //
  OMPropertySetElement* find(void) const;

  void grow(const size_t additionalElements);

  struct OMPropertySetElement {
    int _pid;
    OMProperty* _property;
    bool _valid;
  };
  OMPropertySetElement* _propertySet;  // Representation
  size_t _capacity;                    // Number of potential elements
  size_t _count;                       // Number of usable elements

  const OMStorable* _containingObject;
};

#endif
