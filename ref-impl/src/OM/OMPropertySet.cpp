#include "OMPropertySet.h"
#include "OMProperty.h"

#include "OMAssertions.h"

OMPropertySet::OMPropertySet(void)
: _count(0), _containingObject(0)
{
  for (int i = 0; i < MAXIMUMPROPERTIES; i++) {
    _propertySet[i] = 0;
  }
}

OMProperty* OMPropertySet::get(const int pid) const
{
  TRACE("OMPropertySet::get");
  OMProperty* result;
  PRECONDITION("Valid property id", ((pid >=0) && (pid < MAXIMUMPROPERTIES)));
  result = _propertySet[pid];
  POSTCONDITION("Valid result", result != 0);
  return result;
}

void OMPropertySet::put(OMProperty* p)
{
  TRACE("OMPropertySet::put");
  // SAVE(_count);
  int pid = p->pid();
  PRECONDITION("Valid property id", ((pid >=0) && (pid < MAXIMUMPROPERTIES)));
  PRECONDITION("Property not already installed", _propertySet[pid] == 0);
  _propertySet[pid] = p;
  p->setContainingObject(_containingObject);
  _count++;
  POSTCONDITION("Propery installed", _propertySet[pid] == p);
  // POSTCONDITION("Count increased by one", _count == OLD(_count) + 1);
  // Following sould be in a "invariant" routine
  // The following checks that property ids are assigned contiguously and are "put" in
  // numeric order of pid. What is it in the prototype that requires this ?
  for (size_t i = 0; i < MAXIMUMPROPERTIES; i++) {
    if (i < _count) {
      ASSERT("Valid property", _propertySet[i] != 0);
    } else {
      ASSERT("Invalid property", _propertySet[i] == 0);
    }
  }
}

size_t OMPropertySet::count(void) const
{
  return _count;
}

void OMPropertySet::setContainingObject(const OMStorable* containingObject)
{
  _containingObject = containingObject;
}
