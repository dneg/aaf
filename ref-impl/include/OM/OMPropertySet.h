// @doc

#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMPortability.h"

class OMProperty;
class OMStorable;

  // @class Container class for <c OMProperty> objects.
class OMPropertySet {
public:
  // @access Public members

    // @cmember Constructor.
  OMPropertySet(void);

    // @cmember Destructor.
  ~OMPropertySet(void);

    // @cmember Get the <c OMProperty> associated with the property id
    //          <p pid>.
  OMProperty* get(const int pid) const;

    // @cmember Insert the <c OMProperty> <p property> into this
    //          <c OMPropertySet>.
  void put(OMProperty* property);

    // @cmember Iterate over the <c OMProperty> objects in this
    //          <c OMPropertySet>.
  void iterate(size_t& context, OMProperty*& property) const;

    // @cmember Does this <c OMPropertySet> contain an <c OMProperty>
    //          with property id <p pid> ?
  bool contains(const int pid) const;

    // @cmember The number of <c OMProperty> objects in this
    //          <c OMPropertySet>.
  size_t count(void) const;

    // @cmember Inform this <c OMPropertySet> that it contains the
    //          <c OMProperty> objects of the <c OMStorable>
    //          <p containingObject>.
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
