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
#ifndef OMSTRONGREFSETPROPERTY_H
#define OMSTRONGREFSETPROPERTY_H

#include "OMSet.h"
#include "OMContainerElement.h"
#include "OMStrongReferenceSet.h"

template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetIterator;

class OMStrongReferenceSetElement;

  // @class Persistent sets of uniquely identified strongly referenced
  //        (contained) objects supported by the Object Manager.
  //        Objects are accessible by unique identifier (the key).
  //        The objects are not ordered. Duplicates objects are not allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @base public | <c OMStrongReferenceSet>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetProperty : public OMStrongReferenceSet {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceSetProperty(const OMPropertyId propertyId,
                               const wchar_t* name,
                               const OMPropertyId keyPropertyId);

    // @cmember Destructor.
  virtual ~OMStrongReferenceSetProperty(void);

    // @cmember Save this <c OMStrongReferenceSetProperty>.
  virtual void save(void) const;

    // @cmember Close this <c OMStrongReferenceSetProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMStrongReferenceSetProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMStrongReferenceSetProperty>, the
    //          external (persisted) size of the
    //          <c OMStrongReferenceSetProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of objects contained within this
    //          <c OMStrongReferenceSetProperty> if any.
  virtual OMUInt64 objectCount(void) const;

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMStrongReferenceSetProperty>.
  size_t count(void) const;

    // @cmember Insert <p object> into this
    //          <c OMStrongReferenceSetProperty>.
  void insert(const ReferencedObject* object);

    // @cmember If it is not already present, insert <p object> into this
    //          <c OMStrongReferenceSetProperty> and return true,
    //          otherwise return false.
  bool ensurePresent(const ReferencedObject* object);

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMStrongReferenceSetProperty>.
  void appendValue(const ReferencedObject* object);

    // @cmember Remove the <p ReferencedObject> identified by
    //          <p identification> from this <c OMStrongReferenceSetProperty>.
  ReferencedObject* remove(const UniqueIdentification& identification);

    // @cmember If it is present, remove the <p ReferencedObject> identified by
    //          <p identification> from this <c OMStrongReferenceSetProperty>
    //          and return true, otherwise return false.
  bool ensureAbsent(const UniqueIdentification& identification);

    // @cmember Remove <p object> from this
    //          <c OMStrongReferenceSetProperty>.
  void removeValue(const ReferencedObject* object);

    // @cmember If it is present, remove <p object> from this
    //          <c OMStrongReferenceSetProperty> and return true,
    //          otherwise return false.
  bool ensureAbsent(const ReferencedObject* object);

    // @cmember Does this <c OMStrongReferenceSetProperty> contain
    //          <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMStrongReferenceSetProperty> contain a
    //          <p ReferencedObject> identified by <p identification>?
  virtual bool contains(
                     const UniqueIdentification& identification) const;

    // @cmember The <p ReferencedObject> in this
    //          <c OMStrongReferenceSetProperty> identified by
    //          <p identification>.
  ReferencedObject* value(
                     const UniqueIdentification& identification) const;

    // @cmember Find the <p ReferencedObject> in this
    //          <c OMStrongReferenceSetProperty> identified by
    //          <p identification>.  If the object is found it is returned
    //          in <p object> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const UniqueIdentification& identification,
                    ReferencedObject*& object) const;

  // Optional property interface

    // @cmember Is this <c OMStrongReferenceSetProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMStrongReferenceSetProperty>.
  virtual void removeProperty(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMStrongReferenceSetProperty>. The size is given
    //          in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMStrongReferenceSetProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this
    //          <c OMStrongReferenceSetProperty>. The raw bits are
    //          copied from the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Insert <p object> into this
    //          <c OMStrongReferenceSetProperty>.
  virtual void insertObject(const OMObject* object);

    // @cmember Does this <c OMStrongReferenceSetProperty> contain
    //          <p object> ?
  virtual bool containsObject(const OMObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMStrongReferenceSetProperty>.
  virtual void removeObject(const OMObject* object);

    // @cmember Remove all objects from this
    //          <c OMStrongReferenceSetProperty>.
  virtual void removeAllObjects(void);

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMStrongReferenceSetProperty>.
  virtual OMReferenceContainerIterator* createIterator(void) const;

    // @cmember Remove the <c OMObject> identified by <p identification>
    //          from this <c OMStrongReferenceSetProperty>.
  virtual OMObject* remove(void* identification);

    // @cmember Does this <c OMStrongReferenceSetProperty> contain an
    //          <c OMObject> identified by <p identification> ?
  virtual bool contains(void* identification) const;

    // @cmember Find the <c OMObject> in this <c OMStrongReferenceSetProperty>
    //          identified by <p identification>.  If the object is found
    //          it is returned in <p object> and the result is < e bool.true>.
    //          If the object is not found the result is <e bool.false>.
  virtual bool findObject(void* identification, OMObject*& object) const;

  bool isValidIdentification(UniqueIdentification& id) const;

  virtual OMContainerIterator<OMStrongReferenceSetElement>*
                                                          iterator(void) const;

  virtual void insert(void* key,
                      const OMStrongReferenceSetElement& element);

  virtual ReferencedObject* replace(const ReferencedObject* object);

  virtual OMKeySize keySize(void) const;

  virtual OMPropertyId keyPropertyId(void) const;

  virtual bool find(void* key, OMStrongReferenceSetElement** element) const;

  virtual void find(void* key, OMStorable*& object) const;

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

private:

  typedef OMStrongReferenceSetElement SetElement;

  typedef OMSetIterator<UniqueIdentification, SetElement> SetIterator;

  // The set of references.
  OMSet<UniqueIdentification, SetElement> _set;

  friend class OMStrongReferenceSetIterator<UniqueIdentification,
                                            ReferencedObject>;

    // @cmember The id of the property whose value is the unique
    //          identifier of objects in this set.
  OMPropertyId _keyPropertyId;

    // OMStrongReferenceSetProperty can't be assigned - declare but
    // don't define
  OMStrongReferenceSetProperty& operator = (
                                      const OMStrongReferenceSetProperty& rhs);

    // OMStrongReferenceSetProperty can't be copied - declare but don't define
  OMStrongReferenceSetProperty(const OMStrongReferenceSetProperty& rhs);

};

#include "OMStrongRefSetPropertyT.h"

#endif
