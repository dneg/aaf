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
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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
  OMStrongReferenceVectorElement(const OMStrongReferenceVectorElement& rhs);

    // @cmember Destructor.
  ~OMStrongReferenceVectorElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide assignment of object references.
  OMStrongReferenceVectorElement& operator=(
                                    const OMStrongReferenceVectorElement& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMStrongReferenceVectorElement& rhs) const;

    // @cmember Set the value of this <c OMStrongReferenceVectorElement>.
  OMStorable* setValue(const OMStorable* value);

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
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStrongReferenceSetElement : public OMStrongReferenceVectorElement {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceSetElement(void);

    // @cmember Constructor.
  OMStrongReferenceSetElement(OMProperty* property,
                              const wchar_t* name,
                              OMUInt32 localKey,
                              void* identification,
                              size_t identificationSize);

    // @cmember Constructor.
  OMStrongReferenceSetElement(OMProperty* property,
                              const wchar_t* name,
                              OMUInt32 localKey,
                              OMUInt32 referenceCount,
                              void* identification,
                              size_t identificationSize);

    // @cmember Copy constructor.
  OMStrongReferenceSetElement(const OMStrongReferenceSetElement& rhs);

    // @cmember Destructor.
  ~OMStrongReferenceSetElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide assignment of object references.
  OMStrongReferenceSetElement& operator=(
                                       const OMStrongReferenceSetElement& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide equality of object references.
  bool operator==(const OMStrongReferenceSetElement& rhs) const;

    // @cmember Set the value of this <c OMStrongReferenceSetElement>.
  OMStorable* setValue(void* identification, const OMStorable* value);

    // @cmember The unique key of this <c OMStrongReferenceSetElement>.
  void* identification(void) const;

  static void enableReferenceCounting(void);

    // @cmember The count of weak references to this
    //          <c OMStrongReferenceSetElement>.
  OMUInt32 referenceCount(void) const;

    // @cmember Increase the count of weak references to this
    //          <c OMStrongReferenceSetElement> by one. Return
    //          the new value of the reference count.
  OMUInt32 increaseReferenceCount(void);

    // @cmember Decrease the count of weak references to this
    //          <c OMStrongReferenceSetElement> by one. Return
    //          the new value of the reference count.
  OMUInt32 decreaseReferenceCount(void);

    // @cmember Is this a sticky <c OMStrongReferenceSetElement>?
    //          If so, this <c OMStrongReferenceSetElement> is always
    //          persistent. If not, this <c OMStrongReferenceSetElement> is
    //          only persistent if it has a non-zero <f referenceCount()>.
  bool isSticky(void) const;

    // @cmember Unstick this <c OMStrongReferenceSetElement>.
  void clearSticky(void);

    // @cmember Make this <c OMStrongReferenceSetElement> sticky.
  void setSticky(void);

private:
  // @access Private members.

  void initialize(OMUInt32 referenceCount,
                  void* identification,
                  size_t identificationSize);

  // Special value for the reference count indicating that this
  // <c OMStrongReferenceSetElement> is not reference counted.
  //
  enum {sticky = 0xffffffff};

  // Either 0      - to enable reference counting by default or
  //        sticky - to disable reference counting by default
  static OMUInt32 _initialReferenceCount;

    // @cmember The unique key of this <c OMStrongReferenceSetElement>.
    //          The element's unique key is in memory even when the
    //          referenced object is not. That way we can tell if an
    //          object is present in a container without loading the object.
  void* _identification;
  size_t _identificationSize;

    // @cmember The count of weak references to this
    //          <c OMStrongReferenceSetElement>.
  OMUInt32 _referenceCount;

};

  // @class Elements of Object Manager reference vectors.
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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
  OMWeakReferenceVectorElement(const OMWeakReferenceVectorElement& rhs);

    // @cmember Destructor.
  ~OMWeakReferenceVectorElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide assignment of object references.
  OMWeakReferenceVectorElement& operator=(
                                      const OMWeakReferenceVectorElement& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMVector>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMWeakReferenceVectorElement& rhs) const;

    // @cmember Set the value of this <c OMWeakReferenceVectorElement>.
  OMStorable* setValue(const OMUniqueObjectIdentification& identification,
                       const OMStorable* value);

    // @cmember The unique key of this <c OMWeakReferenceVectorElement>.
  OMUniqueObjectIdentification identification(void) const;

};

  // @class Elements of Object Manager reference sets.
  //   @base public | <c OMContainerElement>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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
  OMWeakReferenceSetElement(const OMWeakReferenceSetElement& rhs);

    // @cmember Destructor.
  ~OMWeakReferenceSetElement(void);

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide assignment of object references.
  OMWeakReferenceSetElement& operator=(const OMWeakReferenceSetElement& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMSet>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMWeakReferenceSetElement& rhs) const;

    // @cmember Set the value of this <c OMWeakReferenceSetElement>.
  OMStorable* setValue(const OMUniqueObjectIdentification& identification,
                       const OMStorable* value);

    // @cmember The unique key of this <c OMWeakReferenceSetElement>.
  OMUniqueObjectIdentification identification(void) const;

};

#include "OMContainerElementT.h"

#endif
