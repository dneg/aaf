/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMEXTERNAL
#ifndef OMPROPERTYSET_H
#define OMPROPERTYSET_H

#include "OMPortability.h"
#include "OMDataTypes.h"
#include "OMRedBlackTree.h"

#include <stddef.h>

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
