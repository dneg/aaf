//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMSTOREDPROPERTYSETINDEX_H
#define OMSTOREDPROPERTYSETINDEX_H

#include "OMDataTypes.h"

#include <stddef.h>

  // @class The in-memory representation of the on-disk index for a
  //        stored property set.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStoredPropertySetIndex {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStoredPropertySetIndex(size_t capacity);

    // @cmember Destructor.
  ~OMStoredPropertySetIndex(void);

    // @cmember Insert a new property into this <c OMStoredPropertySetIndex>.
    //          The new property has id <p propertyId>. The stored property
    //          representation is <p storedForm>. The property value
    //          occupies <p length> bytes starting at offset <p offset>.
  void insert(OMPropertyId propertyId,
              OMStoredForm storedForm,
              OMPropertyOffset offset,
              OMPropertySize length);

    // @cmember The number of properties in this <c OMStoredPropertySetIndex>.
  size_t entries(void) const;

    // @cmember Iterate over the properties in this
    //          <c OMStoredPropertySetIndex>.
  void iterate(size_t& context,
               OMPropertyId& propertyId,
               OMStoredForm& storedForm,
               OMPropertyOffset& offset,
               OMPropertySize& length) const;

    // @cmember Find the property with property id <p propertyId> in this
    //          <c OMStoredPropertySetIndex>. If found the <p storedForm>,
    //          <p offset> and <p length> of the property are returned.
  bool find(const OMPropertyId& propertyId,
            OMStoredForm& storedForm,
            OMPropertyOffset& offset,
            OMPropertySize& length) const;

    // @cmember Is this <c OMStoredPropertySetIndex> valid ?
  bool isValid(OMPropertyOffset baseOffset) const;

private:

  struct IndexEntry;

  OMStoredPropertySetIndex::IndexEntry* find(OMPropertyId propertyId) const;

  struct IndexEntry {
    OMPropertyId _propertyId;
    OMStoredForm _storedForm;
    OMPropertyOffset _offset;
    OMPropertySize _length;
  };

  size_t _capacity;
  IndexEntry* _table;
  size_t _entries;
};

#endif
