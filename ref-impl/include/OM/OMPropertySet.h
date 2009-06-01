//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMDataTypes.h"
#include "OMRedBlackTree.h"
#include "OMVector.h"
#include "OMVectorIterator.h"


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
  OMUInt16 count(void) const;

  OMUInt16 countPresent(void) const;

    // @cmember This <c OMPropertySet> is contained by the given
    //          <c OMStorable> object <p container>. The <c OMProperty>
    //          objects in this <c OMPropertySet> are the properties of
    //          the given <c OMStorable> object <p container>.
  void setContainer(const OMStorable* container);

    // @cmember The <c OMStorable> object that contains this
    //          <c OMPropertySet>.
  OMStorable* container(void) const;

    // @cmember Get the dynamic built-in <c OMProperty> associated with the 
    //          property uid <p propertyUId>.
  OMProperty* getDynamicBuiltin(const OMUniqueObjectIdentification& propertyUId) const;

    // @cmember Insert the dynamic built-in <c OMProperty> <p property> with
    //          property uid <p propertyUId> into this <c OMPropertySet>.
  void putDynamicBuiltin(const OMUniqueObjectIdentification& propertyUId, OMProperty* property);

    // @cmember Is an dynamic built-in <c OMProperty> with property uid 
    //          <p propertyUId> present in this <c OMPropertySet> ?
  bool dynamicBuiltinIsPresent(const OMUniqueObjectIdentification& propertyUId) const;

    // @cmember Finalise the dynamic built-in <c OMProperty> with property uid 
    //          <p propertyUId> in this <c OMPropertySet> with the <p propertyId>.
  void finaliseDynamicBuiltin(const OMUniqueObjectIdentification& propertyUId, const OMPropertyId propertyId);

private:

  bool find(const OMPropertyId propertyId, OMProperty *&result) const;

  friend class OMPropertySetIterator;

  typedef OMVectorIterator<OMProperty*> SetIterator;
  typedef OMVector<OMProperty*> Set;

  typedef OMRedBlackTreeIterator<OMUniqueObjectIdentification, OMProperty*> DynamicSetIterator;
  typedef OMRedBlackTree<OMUniqueObjectIdentification, OMProperty*> DynamicSet;

  // OMProperty with 'propertyName' or null if not found.
  //
  OMProperty* find(const wchar_t* propertyName) const;

  Set _set;
  const OMStorable* _container;
  
  // temporary set of built-in properties that are yet to have a property id 
  // assigned to them
  DynamicSet _dynamicSet;

};

#endif
