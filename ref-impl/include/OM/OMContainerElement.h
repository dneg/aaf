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

// @doc OMINTERNAL
#ifndef OMCONTAINERELEMENT_H
#define OMCONTAINERELEMENT_H

#include "OMDataTypes.h"
#include "OMObjectReference.h"

class OMProperty;
class OMStorable;

  // @class Pointer elements of non-persistent Object Manager vectors.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMVectorElement {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVectorElement(void);

    // @cmember Constructor.
  OMVectorElement(const ReferencedObject* pointer);

    // @cmember Copy constructor.
  OMVectorElement(const OMVectorElement<ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMVectorElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMContainer>.
  OMVectorElement<ReferencedObject>& operator=(
                                 const OMVectorElement<ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
  bool operator== (const OMVectorElement<ReferencedObject>& rhs) const;

    // @cmember Get the value of this <c OMVectorElement>.
  ReferencedObject* getValue(void) const;

    // @cmember Set the value of this <c OMVectorElement>.
  ReferencedObject* setValue(const ReferencedObject* value);

    // @cmember The value of this <c OMVectorElement> as a pointer.
  ReferencedObject* pointer(void) const;

private:

  ReferencedObject* _pointer;

};

  // @class Pointer elements of non-persistent Object Manager sets.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename UniqueIdentification, typename ReferencedObject>
class OMSetElement : public OMVectorElement<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMSetElement(void);

    // @cmember Constructor.
  OMSetElement(const ReferencedObject* pointer);

    // @cmember Copy constructor.
  OMSetElement(const OMSetElement<UniqueIdentification,
                                  ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMSetElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMSet>.
  OMSetElement<UniqueIdentification, ReferencedObject>& operator=(
                                    const OMSetElement<UniqueIdentification,
                                                       ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMSet>.
  bool operator== (const OMSetElement<UniqueIdentification,
                                      ReferencedObject>& rhs) const;

    // @cmember The unique key of this <c OMSetElement>.
  UniqueIdentification identification(void) const;

};

  // @class Elements of Object Manager reference containers.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference 
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ObjectReference>
class OMContainerElement {
public:
  // @access Public members.

    // @cmember Constructor.
  OMContainerElement(void);

    // @cmember Constructor.
  OMContainerElement(const ObjectReference& reference);

    // @cmember Copy constructor.
  OMContainerElement(const OMContainerElement<ObjectReference>& rhs);

    // @cmember Destructor.
  ~OMContainerElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide assignment of object references.
  OMContainerElement<ObjectReference>& operator=
                              (const OMContainerElement<ObjectReference>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMContainerElement<ObjectReference>& rhs) const;

    // @cmember The contained ObjectReference.
  ObjectReference& reference(void);

    // @cmember Set the contained ObjectReference.
  void setReference(const ObjectReference& reference);

  // Shared interface with OMObjectReference.

    // @cmember Save this <c OMContainerElement>.
  void save(void);

    // @cmember Close this <c OMContainerElement>.
  void close(void);

    // @cmember Detach this <c OMContainerElement>.
  void detach(void);

    // @cmember Restore this <c OMContainerElement>.
  void restore(void);

    // @cmember Get the value of this <c OMContainerElement>.
  OMStorable* getValue(void) const;

    // @cmember The value of this <c OMContainerElement> as a pointer.
    //          This function provides low-level access. If the object exits
    //          but has not yet been loaded then the value returned is 0.
  OMStorable* pointer(void) const;

protected:
  // @access Protected members.

    // @cmember The actual object reference.
  ObjectReference _reference;

};

  // @class Elements of Object Manager reference vectors.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMStrongReferenceVectorElement : public
                                  OMContainerElement<OMStrongObjectReference> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceVectorElement(void);

    // @cmember Constructor.
  OMStrongReferenceVectorElement(OMProperty* property,
                                 const wchar_t* name,
                                 OMUInt32 localKey);

    // @cmember Copy constructor.
  OMStrongReferenceVectorElement(
                  const OMStrongReferenceVectorElement<ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMStrongReferenceVectorElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide assignment of object references.
  OMStrongReferenceVectorElement<ReferencedObject>& operator=(
                  const OMStrongReferenceVectorElement<ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide equality of object references.
  bool operator== (
            const OMStrongReferenceVectorElement<ReferencedObject>& rhs) const;

    // @cmember Set the value of this <c OMStrongReferenceVectorElement>.
  ReferencedObject* setValue(const ReferencedObject* value);

    // @cmember The local key of this <c OMStrongReferenceVectorElement>.
  OMUInt32 localKey(void) const;

private:

    // @cmember The local key of this <c OMStrongReferenceVectorElement>.
    //          The key is unique only within a given container instance
    //          and is assigned to each element of the container in such
    //          way as to be independent of the element's position within
    //          the container.
  OMUInt32 _localKey;

};

  // @class Elements of Object Manager reference sets.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetElement : public
                             OMStrongReferenceVectorElement<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceSetElement(void);

    // @cmember Constructor.
  OMStrongReferenceSetElement(OMProperty* property,
                              const wchar_t* name,
                              OMUInt32 localKey,
                              OMUInt32 referenceCount,
                              UniqueIdentification identification);

    // @cmember Copy constructor.
  OMStrongReferenceSetElement(
                 const OMStrongReferenceSetElement<UniqueIdentification,
                                                   ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMStrongReferenceSetElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide assignment of object references.
  OMStrongReferenceSetElement<UniqueIdentification,
                              ReferencedObject>& operator=(
                 const OMStrongReferenceSetElement<UniqueIdentification,
                                                   ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide equality of object references.
  bool operator== (
           const OMStrongReferenceSetElement<UniqueIdentification,
                                             ReferencedObject>& rhs) const;

    // @cmember Set the value of this <c OMStrongReferenceSetElement>.
  ReferencedObject* setValue(const ReferencedObject* value);

    // @cmember The unique key of this <c OMStrongReferenceSetElement>.
  UniqueIdentification identification(void) const;

    // @cmember The count of weak references to this
    //          <c OMStrongReferenceSetElement>.
  OMUInt32 referenceCount(void) const;

private:
  // @access Private members.

    // @cmember The unique key of this <c OMStrongReferenceSetElement>.
    //          The element's unique key is in memory even when the
    //          referenced object is not. That way we can tell if an
    //          object is present in a container without loading the object.
  UniqueIdentification _identification;

    // @cmember The count of weak references to this
    //          <c OMStrongReferenceSetElement>.
  OMUInt32 _referenceCount;

};

  // @class Elements of Object Manager reference vectors.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakReferenceVectorElement : public
                                    OMContainerElement<OMWeakObjectReference> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceVectorElement(void);

    // @cmember Constructor.
  OMWeakReferenceVectorElement(OMProperty* property,
                               OMUniqueObjectIdentification identification,
                               OMPropertyTag targetTag);

    // @cmember Copy constructor.
  OMWeakReferenceVectorElement(
                    const OMWeakReferenceVectorElement<ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMWeakReferenceVectorElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide assignment of object references.
  OMWeakReferenceVectorElement<ReferencedObject>& operator=(
                    const OMWeakReferenceVectorElement<ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide equality of object references.
  bool operator== (
              const OMWeakReferenceVectorElement<ReferencedObject>& rhs) const;

    // @cmember Set the value of this <c OMWeakReferenceVectorElement>.
  ReferencedObject* setValue(const ReferencedObject* value);

    // @cmember The unique key of this <c OMWeakReferenceVectorElement>.
  OMUniqueObjectIdentification identification(void) const;

};

  // @class Elements of Object Manager reference sets.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakReferenceSetElement : public
                                    OMContainerElement<OMWeakObjectReference> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceSetElement(void);

    // @cmember Constructor.
  OMWeakReferenceSetElement(OMProperty* property,
                            OMUniqueObjectIdentification identification,
                            OMPropertyTag targetTag);

    // @cmember Copy constructor.
  OMWeakReferenceSetElement(
                       const OMWeakReferenceSetElement<ReferencedObject>& rhs);

    // @cmember Destructor.
  ~OMWeakReferenceSetElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide assignment of object references.
  OMWeakReferenceSetElement<ReferencedObject>& operator=(
                       const OMWeakReferenceSetElement<ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide equality of object references.
  bool operator== (
                 const OMWeakReferenceSetElement<ReferencedObject>& rhs) const;

    // @cmember Set the value of this <c OMWeakReferenceSetElement>.
  ReferencedObject* setValue(const ReferencedObject* value);

    // @cmember The unique key of this <c OMWeakReferenceSetElement>.
  OMUniqueObjectIdentification identification(void) const;

};

#include "OMContainerElementT.h"

#endif
