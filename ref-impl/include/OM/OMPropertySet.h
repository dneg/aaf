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

// @doc OMEXTERNAL
#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMDataTypes.h"
#include "OMRedBlackTree.h"

#include <stddef.h>

class OMProperty;
class OMStorable;

  // @class Container class for <c OMProperty> objects.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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

    // @cmember Get the <c OMProperty> named <p propertyName>.
  OMProperty* get(const wchar_t* propertyName) const;

    // @cmember Insert the <c OMProperty> <p property> into this
    //          <c OMPropertySet>.
  void put(OMProperty* property);

    // @cmember Is an <c OMProperty> with property id <p propertyId>
    //          present in this <c OMPropertySet> ?
  bool isPresent(const OMPropertyId propertyId) const;

    // @cmember Is an <c OMProperty> with name <p propertyName>
    //          present in this <c OMPropertySet> ?
  bool isPresent(const wchar_t* propertyName) const;

    // @cmember Is an <c OMProperty> with property id <p propertyId>
    //          allowed in this <c OMPropertySet> ?
  bool isAllowed(const OMPropertyId propertyId) const;

    // @cmember Is an <c OMProperty> with property id <p propertyId>
    //          a required member of this <c OMPropertySet> ?
  bool isRequired(const OMPropertyId propertyId) const;

    // @cmember The number of <c OMProperty> objects in this
    //          <c OMPropertySet>.
  size_t count(void) const;

  size_t countPresent(void) const;

    // @cmember This <c OMPropertySet> is contained by the given
    //          <c OMStorable> object <p container>. The <c OMProperty>
    //          objects in this <c OMPropertySet> are the properties of
    //          the given <c OMStorable> object <p container>.
  void setContainer(const OMStorable* container);

    // @cmember The <c OMStorable> object that contains this
    //          <c OMPropertySet>.
  OMStorable* container(void) const;

private:

  friend class OMPropertySetIterator;

  typedef OMRedBlackTreeIterator<OMPropertyId, OMProperty*> SetIterator;
  typedef OMRedBlackTree<OMPropertyId, OMProperty*> Set;

  // OMProperty with 'propertyName' or null if not found.
  //
  OMProperty* find(const wchar_t* propertyName) const;

  Set _set;
  const OMStorable* _container;
};

#endif
