// @doc OMEXTERNAL
#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMPortability.h"
#include "OMTypes.h"

class OMProperty;
class OMStorable;

  // @class Container class for <c OMProperty> objects.
class OMPropertySet {
public:
  // @access Public members.

    // @cmember Constructor.
  OMPropertySet(void);

    // @cmember Destructor.
  ~OMPropertySet(void);

    // @cmember Get the <c OMProperty> associated with the property id
    //          <p propertyId>.
  OMProperty* get(const OMPropertyId propertyId) const;

    // @cmember Insert the <c OMProperty> <p property> into this
    //          <c OMPropertySet>.
  void put(OMProperty* property);

    // @cmember Iterate over the <c OMProperty> objects in this
    //          <c OMPropertySet>.
  void iterate(size_t& context, OMProperty*& property) const;

    // @cmember Does this <c OMPropertySet> contain an <c OMProperty>
    //          with property id <p propertyId> ?
  bool contains(const OMPropertyId propertyId) const;

    // @cmember The number of <c OMProperty> objects in this
    //          <c OMPropertySet>.
  size_t count(void) const;

    // @cmember This <c OMPropertySet> is contained by the given
    //          <c OMStorable> object <p container>. The <c OMProperty>
    //          objects in this <c OMPropertySet> are the properties of
    //          the given <c OMStorable> object <p container>.
  void setContainer(const OMStorable* container);

    // @cmember The <c OMStorable> object that contains this
    //          <c OMPropertySet>.
  OMStorable* container(void) const;

protected:
  static bool equal(const OMPropertyId& propertyIda,
                    const OMPropertyId& propertyIdb);

private:

  struct OMPropertySetElement;

  // OMPropertySetElement for 'propertyId' or null if not found.
  //
  OMPropertySetElement* find(const OMPropertyId propertyId) const;

  // First free entry or null if full.
  //
  OMPropertySetElement* find(void) const;

  void grow(const size_t additionalElements);

  struct OMPropertySetElement {
    OMPropertyId _propertyId;
    OMProperty* _property;
    bool _valid;
  };
  OMPropertySetElement* _propertySet;  // Representation
  size_t _capacity;                    // Number of potential elements
  size_t _count;                       // Number of usable elements

  const OMStorable* _container;
};

#endif
