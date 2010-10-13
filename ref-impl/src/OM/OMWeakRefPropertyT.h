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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMWeakReferenceProperty
#ifndef OMWEAKREFPROPERTYT_H
#define OMWEAKREFPROPERTYT_H

#include "OMAssertions.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"
#include "OMStoredObject.h"
#include "OMStrongReferenceSet.h"

#if 1 // HACK4MEIP2
#include "OMUniqueObjectIdentType.h"
#endif

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the object referenced by this
  //         <c OMWeakReferenceProperty> resides.
  //   @parm The id of the property by which the <p ReferencedObject>
  //         is uniquely identified (the key).
template <typename Key, typename ReferencedObject>
OMWeakReferenceProperty<Key, ReferencedObject>::OMWeakReferenceProperty(
                                              const OMPropertyId propertyId,
                                              const wchar_t* name,
                                              const wchar_t* targetName,
                                              const OMPropertyId keyPropertyId)
: OMWeakReference(propertyId, name),
  _reference(),
  _targetTag(nullOMPropertyTag),
  _targetName(targetName),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId),
  _targetSet(0)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::"
                                                    "OMWeakReferenceProperty");

  _reference = OMWeakObjectReference(this,
                                     &(OMConstant<Key>::null),
                                     sizeof(Key));
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceProperty>.
  //   @parm The id of the property by which the <p ReferencedObject>
  //         is uniquely identified (the key).
  //   @parm The name (as a list of pids) of the the <c OMProperty> instance
  //         (a set property) in which the object referenced by this
  //         <c OMWeakReferenceProperty> resides.
template <typename Key, typename ReferencedObject>
OMWeakReferenceProperty<Key, ReferencedObject>::OMWeakReferenceProperty(
                                        const OMPropertyId propertyId,
                                        const wchar_t* name,
                                        const OMPropertyId keyPropertyId,
                                        const OMPropertyId* targetPropertyPath)
: OMWeakReference(propertyId, name),
  _reference(),
  _targetTag(nullOMPropertyTag),
  _targetName(0),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId),
  _targetSet(0)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::"
                                                    "OMWeakReferenceProperty");

  _reference = OMWeakObjectReference(this,
                                     &(OMConstant<Key>::null),
                                     sizeof(Key));
  _targetPropertyPath = savePropertyPath(targetPropertyPath);
}

template <typename Key, typename ReferencedObject>
OMWeakReferenceProperty<Key, ReferencedObject>::~OMWeakReferenceProperty(void)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::"
                                                   "~OMWeakReferenceProperty");

  delete [] _targetPropertyPath;
}

  // @mfunc Get the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @this const
template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::getValue(
                                               ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::getValue");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  OMStorable* p = getReferencedValue();
  if (p != 0) {
    ReferencedObject* result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
    object = result;
  } else {
    object = 0;
  }
}

  // @mfunc Set the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject* OMWeakReferenceProperty<Key, ReferencedObject>::setValue(
                                               const ReferencedObject* object)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::setValue");

  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
  PRECONDITION("Valid target object", targetSet()->containsObject(object));

  _reference.setTargetTag(targetTag());
#endif
  const Key id = object->identification();
  OMStorable* p = _reference.setValue(&id, object);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  } else {
    result = 0;
  }
  setPresent();
  return result;
}

  // @mfunc Clear the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceProperty<Key, ReferencedObject>::clearValue(void)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::clearValue");

  OMStorable* p = _reference.setValue(&(OMConstant<Key>::null), 0);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  } else {
    result = 0;
  }
  return result;
}

  // @mfunc Assignment operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The result of the assignment.
  //   @parm A pointer to a <p ReferencedObject> by value.
template <typename Key, typename ReferencedObject>
  OMWeakReferenceProperty<Key, ReferencedObject>&
OMWeakReferenceProperty<Key, ReferencedObject>::operator = (
                                                 const ReferencedObject* value)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::operator =");

  setValue(value);
  return *this;
}

  // @mfunc Dereference operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>
  //   @rdesc A pointer to a <p ReferencedObject> by value.
template <typename Key, typename ReferencedObject>
ReferencedObject*
OMWeakReferenceProperty<Key, ReferencedObject>::operator -> (void)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::operator ->");

  return getReferencedValue();
}

template <typename Key, typename ReferencedObject>
const ReferencedObject*
OMWeakReferenceProperty<Key, ReferencedObject>::operator -> (void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::operator ->");

  return getReferencedValue();
}

  // @mfunc Type conversion. Convert an
  //        <c OMWeakReferenceProperty> into a pointer to the
  //        referenced (pointed to) <p ReferencedObject>.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p ReferencedObject>.
  //   @this const
template <typename Key, typename ReferencedObject>
OMWeakReferenceProperty<Key, ReferencedObject>::operator
ReferencedObject* () const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::"
                                                 "operator ReferencedObject*");

  ReferencedObject* result;
  getValue(result);
  return result;
}

  // @mfunc Save this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @this const
template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::save");

  PRECONDITION("Non-void weak reference",
                                isResolved() || (!isVoid() && isResolvable()));

  store()->save(*this);
}

  // @mfunc Close this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::close(void)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::close");
  // tjb
}

  // @mfunc The number of objects contained within this
  //        <c OMWeakReferenceProperty> if any.
  //   @rdesc The number of objects.
template <typename Key, typename ReferencedObject>
OMUInt64
OMWeakReferenceProperty<Key, ReferencedObject>::objectCount(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::objectCount");
  // OMWeakReferenceProperty doesn't contain objects.
  return 0;
}

  // @mfunc Restore this <c OMWeakReferenceProperty>, the external
  //        (persisted) size of the <c OMWeakReferenceProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The external (persisted) size of the <c OMWeakReferenceProperty>.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceProperty<Key, ReferencedObject>::restore(
                                                   OMPropertySize externalSize)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::restore");

  store()->restore(*this, externalSize);
  setPresent();
}

  // @mfunc  Is this <c OMWeakReferenceProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc True if this <c OMWeakReferenceProperty> is void, false
  //          otherwise
  //   @this const
template <typename Key, typename ReferencedObject>
bool OMWeakReferenceProperty<Key, ReferencedObject>::isVoid(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::isVoid");

  bool result;
  if (identification() == OMConstant<Key>::null) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

  // @mfunc Get the raw bits of this <c OMWeakReferenceProperty>. The raw bits
  //        are copied to the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
  //   @this const
template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::getBits(
                                                    OMByte* bits,
                                                    OMUInt32 ANAME(size)) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceProperty");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  ReferencedObject* pointer;
  getValue(pointer);

  memcpy(bits, &pointer, bitsSize());
}

  // @mfunc Set the raw bits of this <c OMWeakReferenceProperty>. The raw
  //        bits are copied from the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceProperty<Key, ReferencedObject>::setBits(const OMByte* bits,
                                                        OMUInt32 ANAME(size))
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceProperty");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject* p = *(const ReferencedObject**)bits;
  setValue(p);
}

  // @mfunc Get the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to an <c OMObject>.
  //   @this const
template <typename Key, typename ReferencedObject>
OMObject*
OMWeakReferenceProperty<Key, ReferencedObject>::getObject(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::getObject");

  ReferencedObject* p = 0;
  getValue(p);
  OMObject* result = p;

  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <c OMObject>.
  //   @rdesc A pointer to the old <c OMObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
OMObject* OMWeakReferenceProperty<Key, ReferencedObject>::setObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::setObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return setValue(p);
}

  // @mfunc Clear the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the old <c OMObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename Key, typename ReferencedObject>
OMObject* OMWeakReferenceProperty<Key, ReferencedObject>::clearObject(void)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::clearObject");

  return clearValue();
}

template <typename Key, typename ReferencedObject>
const Key&
OMWeakReferenceProperty<Key, ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::identification");

  POSTCONDITION("Valid identification", _reference.identification() != 0);
  return *reinterpret_cast<const Key*>(_reference.identification());
}

  // @mfunc Get the raw bits of the identification of this
  //        <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the buffer containing the identification.
template <typename Key, typename ReferencedObject>
const void*
OMWeakReferenceProperty<Key, ReferencedObject>::identificationBits(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::identificationBits");

  return _reference.identification();
}

  // @mfunc Set the raw bits of the identification of this
  //        <c OMWeakReferenceProperty>. The raw bits are
  //        copied from the buffer at address <p id> which
  //        is <p keySize> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
template <typename Key, typename ReferencedObject>
void
OMWeakReferenceProperty<Key, ReferencedObject>::setIdentificationBits(
  const void* id,
  OMKeySize idSize)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::setIdentificationBits");
  PRECONDITION("Valid key size", idSize == keySize());

  _reference.setIdentification(id);
}

  // @mfunc The size of the raw bits of the identification
  //        of this <c OMWeakReferenceProperty>. The size is
  //        given in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The size of the raw bits of the identification.
template <typename Key, typename ReferencedObject>
OMKeySize
OMWeakReferenceProperty<Key, ReferencedObject>::keySize(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::keySize");

  return sizeof(Key);
}

template <typename Key, typename ReferencedObject>
OMStrongReferenceSet*
OMWeakReferenceProperty<Key, ReferencedObject>::targetSet(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::targetSet");
  OMWeakReferenceProperty<Key, ReferencedObject>* nonConstThis =
             const_cast<OMWeakReferenceProperty<Key, ReferencedObject>*>(this);
  if (_targetSet == 0) {
    nonConstThis->_targetSet = OMWeakObjectReference::targetSet(this,
                                                                targetTag());
  }
  POSTCONDITION("Valid result", _targetSet != 0);
  return _targetSet;
}

template <typename Key, typename ReferencedObject>
OMPropertyId
OMWeakReferenceProperty<Key, ReferencedObject>::keyPropertyId(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::keyPropertyId");

  return _keyPropertyId;
}

template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::setTargetTag(
                                                       OMPropertyTag targetTag)
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::setTargetTag");

  _targetTag = targetTag;
}

template <typename Key, typename ReferencedObject>
OMPropertyTag
OMWeakReferenceProperty<Key, ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceProperty<Key, ReferencedObject>* nonConstThis =
             const_cast<OMWeakReferenceProperty<Key, ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                  file()->referencedProperties()->insert(targetPropertyPath());
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

template <typename Key, typename ReferencedObject>
OMPropertyId*
OMWeakReferenceProperty<Key, ReferencedObject>::targetPropertyPath(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::targetPropertyPath");

  if (_targetPropertyPath == 0) {

    ASSERT("Valid target name", validWideString(_targetName));

    OMWeakReferenceProperty<Key, ReferencedObject>* nonConstThis =
             const_cast<OMWeakReferenceProperty<Key, ReferencedObject>*>(this);
    nonConstThis->_targetPropertyPath = file()->path(_targetName);
  }
  POSTCONDITION("Valid result", _targetPropertyPath != 0);
  return _targetPropertyPath;
}

template <typename Key, typename ReferencedObject>
void
OMWeakReferenceProperty<Key, ReferencedObject>::clearTargetTag(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::clearTargetTag");

  OMWeakReferenceProperty<Key, ReferencedObject>* nonConstThis =
             const_cast<OMWeakReferenceProperty<Key, ReferencedObject>*>(this);

  nonConstThis->_targetTag = nullOMPropertyTag;
  delete [] nonConstThis->_targetPropertyPath;
  nonConstThis->_targetPropertyPath = 0;
}

template <typename Key, typename ReferencedObject>
const wchar_t*
OMWeakReferenceProperty<Key, ReferencedObject>::targetName(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::targetName");

  return _targetName;
}

template <typename Key, typename ReferencedObject>
bool OMWeakReferenceProperty<Key, ReferencedObject>::isResolved(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::isResolved");

  bool result;
  if (_reference.pointer() != 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

template <typename Key, typename ReferencedObject>
bool OMWeakReferenceProperty<Key, ReferencedObject>::isResolvable(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::isResolvable");

  bool result;
  OMFile* file = propertySet()->container()->file();
  ASSERT("Valid file", file != 0);
  if (file->propertyTableExists()) {
    OMPropertyTable* table = file->referencedProperties();
    ASSERT("Valid table", table != 0);
    if (table->isValid(_targetTag)) {
      result = true;
    } else {
      result = false;
    }
  } else {
    result = false;
  }
  return result;
}

template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::shallowCopyTo(
                                                 OMProperty* destination) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMWeakReferenceProperty Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);
  ASSERT("Valid source", (_targetName != 0) || (_targetPropertyPath != 0));

  Key id = identification();

  dest->_reference = _reference;
  dest->_reference.setValue(&id, 0); // set reference unresolved
  dest->_targetTag = nullOMPropertyTag;
  dest->_targetName = _targetName;
  delete [] dest->_targetPropertyPath;
  if (_targetPropertyPath != 0) {
    // In the case of extension properties, pids from one file are not
    // valid in another. However, the target of a weak reference cannot
    // be contained in an extension property so here we copy the pids.
    FORALL(i, lengthOfPropertyPath(_targetPropertyPath),
      ASSERT("Predefined property", _targetPropertyPath[i] < 0x8000));
    dest->_targetPropertyPath = savePropertyPath(_targetPropertyPath);
  } else {
    dest->_targetPropertyPath = 0;
  }
  dest->_keyPropertyId = _keyPropertyId;
  dest->setPresent();
}

template <typename Key, typename ReferencedObject>
void OMWeakReferenceProperty<Key, ReferencedObject>::deepCopyTo(
                                                     OMProperty* destination,
                                                     void* clientContext) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::deepCopyTo");
  PRECONDITION( "Valid destination", destination != 0 );

  OMStorable* source = getReferencedValue();
  if (source != 0) {
    // There's a referenced object, copy it
    Key id = identification();

    typedef OMWeakReferenceProperty<Key, ReferencedObject> Property;
    Property* wp = dynamic_cast<Property*>(destination);
    ASSERT("Correct property type", wp != 0);

    // Update the target tag on object reference
    wp->setTargetTag(wp->targetTag());

    OMStrongReferenceSet* dest = wp->targetSet();
    ASSERT("Destination is correct type", dest != 0);

    if (!dest->contains(&id)) {
      // Referenced object not already present, copy it
      // and insert it in the target set
      OMStorable* container = dest->container();
      ASSERT("Valid container", container != 0);
      OMClassFactory* factory = container->classFactory();
      ASSERT("Valid class factory", factory != 0);
      OMStorable* d = source->shallowCopy(factory);
      dest->insertObject(d);
      d->onCopy(clientContext);
      source->deepCopyTo(d, clientContext);
    }
  }
}

template <typename Key, typename ReferencedObject>
OMStorable* OMWeakReferenceProperty<Key, ReferencedObject>::getReferencedValue(void) const
{
  TRACE("OMWeakReferenceProperty<Key, ReferencedObject>::getReferencedValue");

  OMWeakReferenceProperty<Key, ReferencedObject>* nonConstThis =
             const_cast<OMWeakReferenceProperty<Key, ReferencedObject>*>(this);

  if ((_reference.pointer() == 0) &&
      (identification() != OMConstant<Key>::null)) {
    OMStorable* object = 0;
    const void* id = _reference.identification();
    // find() should take 'const void*'
    targetSet()->find(const_cast<void*>(id), object);
    if (object) {  // HACK4MEIP2
      nonConstThis->_reference.setValue(id, object);
    }
  }
#if 1 // HACK4MEIP2
  if ((_reference.pointer() == 0) &&
      (identification() != OMConstant<Key>::null)) {
    // We failed to resolve the reference as an object id, try again as a label
    // We should only come here for KLV encoded files.
    ASSERT("Referenced object ID can be a label",
                        keySize() == sizeof(OMUniqueObjectIdentification));
    OMUniqueObjectIdentification bid;
	const Key& key = identification();
	memcpy( reinterpret_cast<void*>(&bid), reinterpret_cast<const void*>(&key), sizeof(bid) );
    if (hostByteOrder() != bigEndian) {
	  OMUniqueObjectIdentificationType::instance()->reorder(
                                               reinterpret_cast<OMByte*>(&bid),
                                               sizeof(bid));
    }
    OMKLVKey k;
    memcpy(&k, &bid, sizeof(OMKLVKey));
    OMUniqueObjectIdentification id;
    convert(id, k);
    nonConstThis->_reference.setIdentification(&id);
    OMStorable* object = 0;
    targetSet()->find(&id, object);
    if (object) {
      nonConstThis->_reference.setValue(&id, object);
    }
  }
#endif
  // If the following assertion is violated we have a dangling weak
  // reference.  The reference illegally designates an object that is
  // not present in the target set.  Code elsewhere prevents the
  // removal of objects that are weakly referenced hence a dangling
  // reference is an assertion violation rather than a run-time error.
  //
  POSTCONDITION("Object found",
                  IMPLIES(identification() != OMConstant<Key>::null,
                          _reference.pointer() != 0));
  return _reference.pointer();
}
#endif
