/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

// @doc OMINTERNAL
#ifndef OMCONTAINERELEMENT_H
#define OMCONTAINERELEMENT_H

#include "OMDataTypes.h"
#include "OMObjectReference.h"

class OMProperty;

  // @class Elements of Object Manager reference containers.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
class OMContainerElement {
public:
  // @access Public members.

    // @cmember Constructor.
  OMContainerElement(void);

    // @cmember Constructor.
  OMContainerElement(OMProperty* property,
                     const char* name,
                     OMUInt32 localKey);

    // @cmember Copy constructor.
  OMContainerElement(
             const OMContainerElement<ObjectReference, ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMContainerElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide assignment of object references.
  OMContainerElement<ObjectReference, ReferencedObject>& operator=
            (const OMContainerElement<ObjectReference, ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (
       const OMContainerElement<ObjectReference, ReferencedObject>& rhs) const;

  // Shared interface with OMObjectReference.

    // @cmember Save this <c OMContainerElement>.
  void save(void* clientContext);

    // @cmember Close this <c OMContainerElement>.
  void close(void);

    // @cmember Restore this <c OMContainerElement>.
  void restore(void);

    // @cmember Get the value of this <c OMContainerElement>.
  ReferencedObject* getValue(void) const;

    // @cmember Set the value of this <c OMContainerElement>.
  ReferencedObject* setValue(const ReferencedObject* value);

    // @cmember The local key of this <c OMContainerElement>.
  OMUInt32 localKey(void) const;

private:
  // @access Private members.

    // @cmember The local key of this <c OMContainerElement>.
    //          The key is unique only within a given container instance
    //          and is assigned to each element of the container in such
    //          way as to be independent of the element's position within
    //          the container.
  OMUInt32 _localKey;

    // @cmember The actual object reference.
  ObjectReference _reference;

};

  // @class Elements of Object Manager reference vectors.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMContainerElement>
template <typename ObjectReference, typename ReferencedObject>
class OMVectorElement : public OMContainerElement<ObjectReference,
                                                  ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVectorElement(void);

    // @cmember Constructor.
  OMVectorElement(OMProperty* property,
                  const char* name,
                  OMUInt32 localKey);

    // @cmember Copy constructor.
  OMVectorElement(
                const OMVectorElement<ObjectReference, ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMVectorElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide assignment of object references.
  OMVectorElement<ObjectReference, ReferencedObject>& operator=
               (const OMVectorElement<ObjectReference, ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide equality of object references.
  bool operator== (
          const OMVectorElement<ObjectReference, ReferencedObject>& rhs) const;

};

  // @class Elements of Object Manager reference sets.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMContainerElement>
template <typename ObjectReference, typename ReferencedObject>
class OMSetElement : public OMContainerElement<ObjectReference,
                                               ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMSetElement(void);

    // @cmember Constructor.
  OMSetElement(OMProperty* property,
               const char* name,
               OMUInt32 key,
               OMUniqueObjectIdentification identification);

    // @cmember Copy constructor.
  OMSetElement(const OMSetElement<ObjectReference, ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMSetElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide assignment of object references.
  OMSetElement<ObjectReference, ReferencedObject>& operator=
                  (const OMSetElement<ObjectReference, ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide equality of object references.
  bool operator== (
             const OMSetElement<ObjectReference, ReferencedObject>& rhs) const;

    // @cmember The unique key of this <c OMSetElement>.
  const OMUniqueObjectIdentification identification(void) const;

    // @cmember The count of weak references to this <c OMSetElement>.
  OMUInt32 referenceCount(void) const;

private:
  // @access Private members.

    // @cmember The unique key of this <c OMSetElement>.
    //          The element's unique key is in memory even when the
    //          referenced object is not. That way we can tell if an
    //          object is present in a container without loading the object.
  OMUniqueObjectIdentification _identification;

    // @cmember The count of weak references to this <c OMSetElement>.
  OMUInt32 _referenceCount;

};


#include "OMContainerElementT.h"

#endif
