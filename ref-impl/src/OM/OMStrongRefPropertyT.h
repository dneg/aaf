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
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMStrongReferenceProperty
#ifndef OMSTRONGREFPROPERTYT_H
#define OMSTRONGREFPROPERTYT_H

#include "OMAssertions.h"
#include "OMStoredObject.h"

template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::OMStrongReferenceProperty(
                                                 const OMPropertyId propertyId,
                                                 const wchar_t* name)
: OMStrongReference(propertyId, name),
  _reference()
{
  TRACE(
     "OMStrongReferenceProperty<ReferencedObject>::OMStrongReferenceProperty");

  _reference = OMStrongObjectReference(this, storedName());
}

template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::~OMStrongReferenceProperty(void)
{
  TRACE(
    "OMStrongReferenceProperty<ReferencedObject>::~OMStrongReferenceProperty");
  _reference.setValue(0);
}

  // @mfunc Get the value of this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::getValue(
                                               ReferencedObject*& object) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::getValue");
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

  // @mfunc Set the value of this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject* OMStrongReferenceProperty<ReferencedObject>::setValue(
                                               const ReferencedObject* object)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::setValue");

  // tjb - PRECONDITION("Valid object", object != 0);

  OMStorable* p = _reference.setValue(object);
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

  // @mfunc Clear the value of this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject* OMStrongReferenceProperty<ReferencedObject>::clearValue(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::clearValue");

  OMStorable* p = _reference.setValue(0);
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
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>
  //   @rdesc The result of the assignment.
  //   @parm A pointer to a <p ReferencedObject> by value.
template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>&
OMStrongReferenceProperty<ReferencedObject>::operator = (
                                                 const ReferencedObject* value)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::operator =");

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
OMStrongReferenceProperty<ReferencedObject>::operator -> (void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::operator ->");

  OMStorable* p = _reference.getValue();
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  } else {
    result = 0;
  }
  return result;
}

  // @mfunc Dereference operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>
  //   @rdesc A pointer to a <p ReferencedObject> by value.
  //   @this const
template <typename ReferencedObject>
const ReferencedObject*
OMStrongReferenceProperty<ReferencedObject>::operator -> (void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::operator ->");

  OMStorable*p = _reference.getValue();
  const ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<const ReferencedObject*>(p);
    ASSERT("Object is correct type", p != 0);
  } else {
    result = 0;
  }
  return result;
}

  // @mfunc Type conversion. Convert an
  //        <c OMStrongReferenceProperty> into a pointer to the
  //        referenced (contained) <p ReferencedObject>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::operator ReferencedObject* ()
                                                                          const
{
  TRACE(
    "OMStrongReferenceProperty<ReferencedObject>::operator ReferencedObject*");

  ReferencedObject* result;
  getValue(result);
  return result;
}

  // @mfunc Save this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::save(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::save");

  store()->save(*this);
}

  // @mfunc Close this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::close(void)
{
  if (!isOptional() || isPresent()) {
    _reference.close();
  }
}

  // @mfunc Detach this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::detach(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::detach");
  if (!isOptional() || isPresent()) {
    _reference.detach();
  }
}

  // @mfunc Restore this <c OMStrongReferenceProperty>, the external
  //        (persisted) size of the <c OMStrongReferenceProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The external (persisted) size of the
  //         <c OMStrongReferenceProperty>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::restore(size_t externalSize)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::restore");

  store()->restore(*this, externalSize);
  setPresent();
}

  // @mfunc The number of objects contained within this
  //        <c OMStrongReferenceProperty> if any.
  //   @rdesc The number of objects.
template <typename ReferencedObject>
OMUInt64 OMStrongReferenceProperty<ReferencedObject>::objectCount(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::objectCount");
  OMUInt64 result = 0;
  OMStorable* p = _reference.getValue();
  if (p != 0) {
    result = p->objectCount();
  }
  return result;
}

  // @mfunc  Is this <c OMStrongReferenceProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc True if this <c OMStrongReferenceProperty> is void, false
  //          otherwise
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::isVoid");

  bool result;

  if (_reference.getValue() == 0) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

  // @mfunc Remove this optional <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::removeProperty(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::removeProperty");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc Get the raw bits of this <c OMStrongReferenceProperty>.
  //        The raw bits are copied to the buffer at address <p bits>
  //        which is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceProperty");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  ReferencedObject* pointer;
  getValue(pointer);

  memcpy(bits, &pointer, bitsSize());
}

  // @mfunc Set the raw bits of this <c OMStrongReferenceProperty>. The raw
  //        bits are copied from the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::setBits(const OMByte* bits,
                                                          size_t ANAME(size))
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceProperty");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject* p = *(const ReferencedObject**)bits;
  setValue(p);
}

  // @mfunc Get the value of this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to an <c OMObject>.
  //   @this const
template <typename ReferencedObject>
OMObject*
OMStrongReferenceProperty<ReferencedObject>::getObject(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::getObject");

  ReferencedObject* p = 0;  
  getValue(p);
  OMObject* result = p;

  return result;
}

  // @mfunc Set the value of this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <c OMObject>.
  //   @rdesc A pointer to the old <c OMObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
OMObject* OMStrongReferenceProperty<ReferencedObject>::setObject(
                                                        const OMObject* object)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::setObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return setValue(p);
}

  // @mfunc Clear the value of this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the old <c OMObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
OMObject* OMStrongReferenceProperty<ReferencedObject>::clearObject(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::clearObject");

  return clearValue();
}

  // @mfunc The value of this <c OMStrongReferenceProperty>
  //        as an <c OMStorable>.
  //   @rdesc The <c OMStorable> represented by this
  //          <c OMStrongReferenceProperty>
  //   @this const
template <typename ReferencedObject>
OMStorable* OMStrongReferenceProperty<ReferencedObject>::storable(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::storable");

  ReferencedObject* pointer;
  getValue(pointer);
  OMStorable* result = 0;
  if (pointer != 0) {
    result = dynamic_cast<OMStorable*>(pointer);
  }
  return result;
}

template <typename ReferencedObject>
OMStrongObjectReference&
OMStrongReferenceProperty<ReferencedObject>::reference(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::storable");

  return const_cast<OMStrongObjectReference&>(_reference);
}

template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::shallowCopyTo(
                                           OMProperty* /* destination */) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::shallowCopyTo");
  // Nothing to do - this is a shallow copy
}

template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::deepCopyTo(
                                                     OMProperty* destination,
                                                     void* clientContext) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::deepCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMStrongReferenceProperty<ReferencedObject> Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  ASSERT("Destination reference is void", dest->isVoid());
  OMStorable* source = _reference.getValue();
  ASSERT("Valid source", source != 0);
  OMStorable* d = source->shallowCopy();
  dest->setObject(d);
  d->onCopy(clientContext);
  source->deepCopyTo(d, clientContext);
}

#endif
