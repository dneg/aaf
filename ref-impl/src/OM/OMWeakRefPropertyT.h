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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
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

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the object referenced by this
  //         <c OMWeakReferenceProperty> resides.
  //   @parm The id of the property by which the <p ReferencedObject>
  //         is uniquely identified (the key).
template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty(
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
  TRACE("OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty");

  _reference = OMWeakObjectReference(this);
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceProperty>.
  //   @parm The id of the property by which the <p ReferencedObject>
  //         is uniquely identified (the key).
  //   @parm The name (as a list of pids) of the the <c OMProperty> instance
  //         (a set property) in which the object referenced by this
  //         <c OMWeakReferenceProperty> resides.
template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty(
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
  TRACE("OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty");

  _reference = OMWeakObjectReference(this);
  _targetPropertyPath = savePropertyPath(targetPropertyPath);
}

template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::~OMWeakReferenceProperty(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::~OMWeakReferenceProperty");

  delete [] _targetPropertyPath;
}

  // @mfunc Get the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::getValue(
                                               ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getValue");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  OMStorable* p = _reference.getValue();
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
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceProperty<ReferencedObject>::setValue(
                                               const ReferencedObject* object)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::setValue");

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
  OMStorable* p = _reference.setValue(object->identification(), object);
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
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceProperty<ReferencedObject>::clearValue(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::clearValue");

  OMStorable* p = _reference.setValue(nullOMUniqueObjectIdentification, 0);
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
template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>&
OMWeakReferenceProperty<ReferencedObject>::operator = (
                                                 const ReferencedObject* value)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::operator =");

  setValue(value);
  return *this;
}

  // @mfunc Dereference operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>
  //   @rdesc A pointer to a <p ReferencedObject> by value.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceProperty<ReferencedObject>::operator -> (void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::operator ->");

  return _reference.getValue();
}

template <typename ReferencedObject>
const ReferencedObject*
OMWeakReferenceProperty<ReferencedObject>::operator -> (void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::operator ->");

  return _reference.getValue();
}

  // @mfunc Type conversion. Convert an
  //        <c OMWeakReferenceProperty> into a pointer to the
  //        referenced (pointed to) <p ReferencedObject>.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::operator ReferencedObject* () const
{
  TRACE(
      "OMWeakReferenceProperty<ReferencedObject>::operator ReferencedObject*");

  ReferencedObject* result;
  getValue(result);
  return result;
}

  // @mfunc Save this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::save");

  PRECONDITION("Non-void weak reference", !_reference.isVoid());

  store()->save(*this);
}

  // @mfunc Close this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::close(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::close");
  // tjb
}

  // @mfunc The number of objects contained within this
  //        <c OMWeakReferenceProperty> if any.
  //   @rdesc The number of objects.
template <typename ReferencedObject>
OMUInt64 OMWeakReferenceProperty<ReferencedObject>::objectCount(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::objectCount");
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
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::restore(size_t externalSize)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::restore");

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
template <typename ReferencedObject>
bool OMWeakReferenceProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::isVoid");

  bool result;
  const OMUniqueObjectIdentification& key = _reference.identification();
  if (key == nullOMUniqueObjectIdentification) {
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
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getBits");
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
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::setBits(const OMByte* bits,
                                                        size_t ANAME(size))
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getBits");
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
template <typename ReferencedObject>
OMObject*
OMWeakReferenceProperty<ReferencedObject>::getObject(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getObject");

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
template <typename ReferencedObject>
OMObject* OMWeakReferenceProperty<ReferencedObject>::setObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::setObject");

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
template <typename ReferencedObject>
OMObject* OMWeakReferenceProperty<ReferencedObject>::clearObject(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::clearObject");

  return clearValue();
}

template <typename ReferencedObject>
const OMUniqueObjectIdentification&
OMWeakReferenceProperty<ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::identification");

  return reference().identification();
}

template <typename ReferencedObject>
OMWeakObjectReference&
OMWeakReferenceProperty<ReferencedObject>::reference(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::reference");

  return const_cast<OMWeakObjectReference&>(_reference);
}


template <typename ReferencedObject>
OMStrongReferenceSet*
OMWeakReferenceProperty<ReferencedObject>::targetSet(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::targetSet");
  OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);
  if (_targetSet == 0) {
    nonConstThis->_targetSet = OMWeakObjectReference::targetSet(this,
                                                                targetTag());
  }
  POSTCONDITION("Valid result", _targetSet != 0);
  return _targetSet;
}

template <typename ReferencedObject>
OMPropertyId
OMWeakReferenceProperty<ReferencedObject>::keyPropertyId(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::keyPropertyId");

  return _keyPropertyId;
}

template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::setTargetTag(
                                                       OMPropertyTag targetTag)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::setTargetTag");

  _targetTag = targetTag;
}

template <typename ReferencedObject>
OMPropertyTag OMWeakReferenceProperty<ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                  file()->referencedProperties()->insert(targetPropertyPath());
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

template <typename ReferencedObject>
OMPropertyId*
OMWeakReferenceProperty<ReferencedObject>::targetPropertyPath(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::targetPropertyPath");

  if (_targetPropertyPath == 0) {

    ASSERT("Valid target name", validWideString(_targetName));

    OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);
    nonConstThis->_targetPropertyPath = file()->path(_targetName);
  }
  POSTCONDITION("Valid result", _targetPropertyPath != 0);
  return _targetPropertyPath;
}

template <typename ReferencedObject>
void
OMWeakReferenceProperty<ReferencedObject>::clearTargetTag(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::clearTargetTag");

  OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);

  nonConstThis->_targetTag = nullOMPropertyTag;
  delete [] nonConstThis->_targetPropertyPath;
  nonConstThis->_targetPropertyPath = 0;
}

template <typename ReferencedObject>
const wchar_t*
OMWeakReferenceProperty<ReferencedObject>::targetName(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::targetName");

  return _targetName;
}

template <typename ReferencedObject>
bool OMWeakReferenceProperty<ReferencedObject>::isResolved(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::isResolved");

  bool result;
  if (_reference.pointer() != 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

template <typename ReferencedObject>
bool OMWeakReferenceProperty<ReferencedObject>::isResolvable(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::isResolvable");

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

template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::shallowCopyTo(
                                                 OMProperty* destination) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMWeakReferenceProperty Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  dest->_reference = _reference;
  dest->_targetTag = nullOMPropertyTag;
  dest->_targetName = _targetName;
  delete [] dest->_targetPropertyPath;
  dest->_targetPropertyPath = 0;
  dest->_keyPropertyId = _keyPropertyId;
}

template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::deepCopyTo(
                                               OMProperty* /* destination */,
                                               void* /* clientContext */) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::deepCopyTo");
  // Nothing to do - this is a deep copy
}

#endif
