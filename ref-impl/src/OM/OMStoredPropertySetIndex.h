// @doc OMINTERNAL
#ifndef OMSTOREDPROPERTYSETINDEX_H
#define OMSTOREDPROPERTYSETINDEX_H

#include "OMPortability.h"
#include "OMTypes.h"

#include <stddef.h>

  // @class The in-memory representation of the on-disk index for a
  //        stored property set.
class OMStoredPropertySetIndex {
private:
  struct IndexEntry;  
public:
  // @access Public members.

    // @cmember Constructor.  
  OMStoredPropertySetIndex(size_t capacity);

    // @cmember Destructor.
  ~OMStoredPropertySetIndex(void);

    // @cmember Insert a new property into this <c OMStoredPropertySetIndex>.
    //          The new property has id <p propertyId>. The property
    //          representation is of type <p type>. The property value
    //          occupies <p length> bytes starting at offset <p offset>.
  void insert(OMPropertyId propertyId,
              OMUInt32 type,
              OMUInt32 offset,
              OMUInt32 length);
  
    // @cmember The number of properties in this <c OMStoredPropertySetIndex>.
    // @this const
  size_t entries(void) const;

    // @cmember Iterate over the properties in this
    //          <c OMStoredPropertySetIndex>.
    // @this const
  void iterate(size_t& context,
               OMPropertyId& propertyId,
               OMUInt32& type,
               OMUInt32& offset,
               OMUInt32& length) const;

    // @cmember Find the property with property id <p propertyId> in this
    //          <c OMStoredPropertySetIndex>. If found the <p type>,
    //          <p offset> and <p length> of the property are returned.
    // @this const  
  bool find(const OMPropertyId& propertyId,
            OMUInt32& type,
            OMUInt32& offset,
            OMUInt32& length) const;

    // @cmember Is this <c OMStoredPropertySetIndex> valid ?
    // @this const
  bool isValid(void) const;

protected:

  OMStoredPropertySetIndex::IndexEntry* find(void) const;

  OMStoredPropertySetIndex::IndexEntry* find(OMPropertyId propertyId) const;
    
private:
    
  struct IndexEntry {
    OMPropertyId _propertyId;
    OMUInt32 _type;
    OMUInt32 _offset;
    OMUInt32 _length;
    bool _valid;
  };
    
  size_t _capacity;
  IndexEntry* _table;
  size_t _entries;
};

#endif
