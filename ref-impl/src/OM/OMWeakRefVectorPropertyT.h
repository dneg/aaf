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
//         OMWeakReferenceVectorProperty
#ifndef OMWEAKREFVECTORPROPERTYT_H
#define OMWEAKREFVECTORPROPERTYT_H

#include "OMAssertions.h"
#include "OMWeakReferenceVectorIter.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"
#include "OMStoredObject.h"
#include "OMFile.h"
#include "OMStrongReferenceSet.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceVectorProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the objects referenced by the
  //         elements of this <c OMWeakReferenceVectorProperty> reside.
  //   @parm The id of the property by which the <p ReferencedObject>s
  //         are uniquely identified (the key).
template <typename ReferencedObject>
OMWeakReferenceVectorProperty<ReferencedObject>::
                OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                              const wchar_t* name,
                                              const wchar_t* targetName,
                                              const OMPropertyId keyPropertyId)
: OMWeakReferenceVector(propertyId, name),
  _targetTag(nullOMPropertyTag),
  _targetName(targetName),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId),
  _targetSet(0)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::"
                                              "OMWeakReferenceVectorProperty");
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceVectorProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the objects referenced by the
  //         elements of this <c OMWeakReferenceVectorProperty> reside.
  //   @parm The id of the property by which the <p ReferencedObject>s
  //         are uniquely identified (the key).
template <typename ReferencedObject>
OMWeakReferenceVectorProperty<ReferencedObject>::OMWeakReferenceVectorProperty(
                                        const OMPropertyId propertyId,
                                        const wchar_t* name,
                                        const OMPropertyId keyPropertyId,
                                        const OMPropertyId* targetPropertyPath)
: OMWeakReferenceVector(propertyId, name),
  _targetTag(nullOMPropertyTag),
  _targetName(0),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId),
  _targetSet(0)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::"
                                              "OMWeakReferenceVectorProperty");

  _targetPropertyPath = savePropertyPath(targetPropertyPath);
}

  // @mfunc Destructor.
template <typename ReferencedObject>
OMWeakReferenceVectorProperty<ReferencedObject>::
                                           ~OMWeakReferenceVectorProperty(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::"
                                             "~OMWeakReferenceVectorProperty");

  delete [] _targetPropertyPath;
}

  // @mfunc Save this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::save");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  store()->save(*this);
}

  // @mfunc Close this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::close(void)
{
  size_t count = _vector.count();
  for (size_t i = 0; i < count; i++) {
    VectorElement& element = _vector.getAt(i);
    element.close();
  }
}

  // @mfunc Detach this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::detach(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::detach");
  size_t count = _vector.count();
  for (size_t i = 0; i < count; i++) {
    VectorElement& element = _vector.getAt(i);
    element.detach();
  }
}

  // @mfunc Restore this <c OMWeakReferenceVectorProperty>, the external
  //        (persisted) size of the <c OMWeakReferenceVectorProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The external (persisted) size of the
  //         <c OMWeakReferenceVectorProperty>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::restore(
                                                           size_t externalSize)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::restore");

  store()->restore(*this, externalSize);
  setPresent();
}

  // @mfunc The number of objects contained within this
  //        <c OMWeakReferenceVectorProperty> if any.
  //   @rdesc The number of objects.
template <typename ReferencedObject>
OMUInt64
OMWeakReferenceVectorProperty<ReferencedObject>::objectCount(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::objectCount");
  // OMWeakReferenceVectorProperty doesn't contain objects.
  return 0;
}

  // @mfunc The number of <p ReferencedObject>s in this
  //        <c OMWeakReferenceVectorProperty>.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::count(void) const
{
  return _vector.count();
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> to <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceVectorProperty<ReferencedObject>::setValueAt(
                                                const ReferencedObject* object,
                                                const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::setValueAt");

  PRECONDITION("Valid index", index < count());
  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
  PRECONDITION("Valid target object", targetSet()->containsObject(object));
#endif

  // Set the vector to contain the new object
  //
  VectorElement& element = _vector.getAt(index);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  element.reference().setTargetTag(targetTag());
#endif
  ReferencedObject* result = 0;
  OMStorable* p = element.setValue(object->identification(), object);
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  setPresent();

  POSTCONDITION("Object properly inserted",
     _vector.getAt(index).getValue() == const_cast<ReferencedObject*>(object));
  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> to 0.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The position to clear.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<ReferencedObject>::clearValueAt(
                                                            const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::clearValueAt");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);
  OMStorable* p = element.setValue(nullOMUniqueObjectIdentification, 0);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  POSTCONDITION("Object properly cleared",
                                         _vector.getAt(index).getValue() == 0);
  return result;
}

  // @mfunc The value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceVectorProperty<ReferencedObject>::valueAt(
                                                     const size_t index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::valueAt");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  OMStorable* p = element.getValue();
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  }
  return result;
}

  // @mfunc Get the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> into <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::getValueAt(
                                                     ReferencedObject*& object,
                                                     const size_t index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::getValueAt");
  OBSOLETE("OMWeakReferenceVectorProperty<ReferencedObject>::valueAt");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  OMStorable* p = element.getValue();
  if (p != 0) {
    object = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", object != 0);
  }
}

  // @mfunc If <p index> is valid, get the value of this
  //        <c OMWeakReferenceVectorProperty> at position <p index>
  //        into <p object> and return true, otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @rdesc True if <p index> is valid, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorProperty<ReferencedObject>::find(
                                               const size_t index,
                                               ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::find");

  bool result;
  if (index < count()) {
    object = valueAt(index);
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Append the given <p ReferencedObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::appendValue");

  PRECONDITION("Valid object", object != 0);

  insertAt(object, count());

}

  // @mfunc Prepend the given <p ReferencedObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::prependValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::prependValue");

  PRECONDITION("Valid object", object != 0);

  insertAt(object, 0);
}

  // @mfunc Insert <p object> into this
  //        <c OMWeakReferenceVectorProperty>. This function is
  //        redefined from <c OMContainerProperty> as
  //        <mf OMWeakReferenceVectorProperty::appendValue>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::insert");

  PRECONDITION("Valid object", object != 0);

  appendValue(object);
}

  // @mfunc Insert <p value> into this <c OMWeakReferenceVectorProperty>
  //        at position <p index>. Existing values at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::insertAt(
                                                const ReferencedObject* object,
                                                const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::insertAt");

  PRECONDITION("Valid index", index <= count());
  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
  PRECONDITION("Valid target object", targetSet()->containsObject(object));
#endif

  OMUniqueObjectIdentification key = object->identification();
  VectorElement newElement(this, key, _targetTag);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  newElement.reference().setTargetTag(targetTag());
#endif
  newElement.setValue(key, object);
  _vector.insertAt(newElement, index);
  setPresent();

  POSTCONDITION("Object properly inserted",
     _vector.getAt(index).getValue() == const_cast<ReferencedObject*>(object));
}

  // @mfunc Does this <c OMWeakReferenceVectorProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorProperty<ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::containsValue");

  PRECONDITION("Valid object", object != 0);

  bool result = false;
  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == const_cast<ReferencedObject*>(object)) {
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc Remove <p object> from this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  size_t index = indexOfValue(object);
  removeAt(index);
}

  // @mfunc Remove the object from this
  //        <c OMWeakReferenceVectorProperty> at position <p index>.
  //        Existing objects in this <c OMWeakReferenceVectorProperty>
  //        at <p index> + 1 and higher are shifted down one index
  //        position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The position from which to remove the <p ReferencedObject>.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<ReferencedObject>::removeAt(const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeAt");
  PRECONDITION("Valid index", index < count());

  ReferencedObject* result = clearValueAt(index);
  _vector.removeAt(index);
  return result;
}

  // @mfunc Remove the last (index == count() - 1) object
  //        from this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<ReferencedObject>::removeLast(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeLast");

  return removeAt(count() - 1);
}

  // @mfunc Remove the first (index == 0) object
  //        from this <c OMWeakReferenceVectorProperty>. Existing
  //        objects in this <c OMWeakReferenceVectorProperty> are
  //        shifted down one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceVectorProperty<ReferencedObject>::removeFirst(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeFirst");

  return removeAt(0);
}

  // @mfunc The index of the <p ReferencedObject*> <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the <p ReferencedObject> to find.
  //   @rdesc The index.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::indexOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::indexOfValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  size_t result;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == const_cast<ReferencedObject*>(object)) {
      result = iterator.index();
      break;
    }
  }
  return result;
}

  // @mfunc The number of occurrences of <p object> in this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to count.
  //   @rdesc The number of occurrences.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::countOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::countOfValue");

  PRECONDITION("Valid object", object != 0);

  size_t result = 0;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == const_cast<ReferencedObject*>(object)) {
      result = result + 1;
    }
  }
  return result;
}

  // @mfunc Does this <c OMWeakReferenceVectorProperty> contain
  //        <p index> ? Is <p index> valid ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The index.
  //   @rdesc True if the index is valid, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorProperty<ReferencedObject>::containsIndex(
                                                      const size_t index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::containsIndex");

  bool result;
  if (index < count()) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc If this <c OMWeakReferenceProperty> contains <p object>
  //        then place its index in <p index> and return true, otherwise
  //        return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object for which to search.
  //   @parm The index of the object.
  //   @rdesc True if the object was found, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorProperty<ReferencedObject>::findIndex(
                           const ReferencedObject* object, size_t& index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::findIndex");

  PRECONDITION("Valid object", object != 0);

  bool result = false;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == object) {
      index = iterator.index();
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc Increase the capacity of this
  //        <c OMWeakReferenceVectorProperty> so that it
  //        can contain at least <p capacity> <p ReferencedObject>s
  //        without having to be resized.
  //   @parm The desired capacity.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::grow(
                                                         const size_t capacity)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::grow");
  PRECONDITION("Valid capacity", capacity > count());

  // Increase the capacity of the vector.
  size_t oldCount = _vector.count();
  _vector.grow(capacity);

  // Make sure the new elements are defined.
  for (size_t i = oldCount; i < capacity; i++) {
    VectorElement voidElement;
    _vector.insert(voidElement);
  }
}

  // @mfunc Is this <c OMWeakReferenceVectorProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc True if this <c OMWeakReferenceVectorProperty> is void,
  //          false otherwise. 
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceVectorProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::isVoid");

  bool result = true;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    OMStorable* object = element.getValue();
    if (object != 0) {
      result = false;
      break;
    }
  }
  return result;
}

  // @mfunc Remove this optional <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::removeProperty(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeProperty");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc The size of the raw bits of this
  //        <c OMWeakReferenceVectorProperty>. The size is given in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The size of the raw bits of this
  //          <c OMWeakReferenceVectorProperty> in bytes.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::bitsSize(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::bitsSize");
  OBSOLETE("methods on class OMReferenceVectorProperty");

  return sizeof(ReferencedObject*) * count();
}

  // @mfunc Get the raw bits of this <c OMWeakReferenceVectorProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceVectorProperty");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const OMStorable** p = (const OMStorable**)bits;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    *p++ = element.getValue();
  }
}

  // @mfunc Set the raw bits of this
  //        <c OMWeakReferenceVectorProperty>. The raw bits are
  //        copied from the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::setBits(
                                                            const OMByte* bits,
                                                            size_t size)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::setBits");
  OBSOLETE("methods on class OMReferenceVectorProperty");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  size_t elementCount = size / sizeof(ReferencedObject*);
  ReferencedObject** p = (ReferencedObject**)bits;

  for (size_t i = 0; i < elementCount; i++) {
    ReferencedObject* object = p[i];
    if (i < count()) {
      setValueAt(object, i);
    } else {
      appendValue(object);
    }
  }

}

  // @mfunc Insert <p object> into this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The <c OMObject> to insert.
template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::insertObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::insertObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  insert(p);
}

  // @mfunc Does this <c OMWeakReferenceVectorProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The <c OMObject> for which to search.
  //   @rdesc True if <p object> is present, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool
OMWeakReferenceVectorProperty<ReferencedObject>::containsObject(
                                                  const OMObject* object) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::containsObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return containsValue(p);
}

  // @mfunc Remove <p object> from this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The <c OMObject> to remove.
template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::removeObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  removeValue(p);
}

  // @mfunc Remove all objects from this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::removeAllObjects(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeAllObjects");

  _vector.clear();
  POSTCONDITION("All objects removed", count() == 0);
}

  // @mfunc Create an <c OMReferenceContainerIterator> over this
  //        <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @rdesc An <c OMReferenceContainerIterator> over this
  //          <c OMWeakReferenceVectorProperty>.
  //   @this const
template <typename ReferencedObject>
OMReferenceContainerIterator*
OMWeakReferenceVectorProperty<ReferencedObject>::createIterator(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::createIterator");

  OMWeakReferenceVectorIterator<ReferencedObject>* result =
          new OMWeakReferenceVectorIterator<ReferencedObject>(*this, OMBefore);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index> to <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The new object.
  //   @parm The index.
  //   @rdesc The old object.
template <typename ReferencedObject>
OMObject*
OMWeakReferenceVectorProperty<ReferencedObject>::setObjectAt(
                                                        const OMObject* object,
                                                        const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::setObjectAt");

  PRECONDITION("Valid index", index < count());
  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return setValueAt(p, index);
}

  // @mfunc The value of this <c OMWeakReferenceVectorProperty>
  //        at position <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The index.
  //   @rdesc The object.
  //   @this const
template <typename ReferencedObject>
OMObject*
OMWeakReferenceVectorProperty<ReferencedObject>::getObjectAt(
                                                      const size_t index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::getObjectAt");

  PRECONDITION("Valid index", index < count());

  return valueAt(index);
}

  // @mfunc Append the given <p OMObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to append.
template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::appendObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::appendObject");

  PRECONDITION("Valid object", object != 0);

  insertObjectAt(object, count());
}

  // @mfunc Prepend the given <p OMObject> <p object> to
  //        this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to prepend.
template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::prependObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::prependObject");

  PRECONDITION("Valid object", object != 0);

  insertObjectAt(object, 0);
}

  // @mfunc Remove the object from this
  //        <c OMWeakReferenceVectorProperty> at position <p index>.
  //        Existing objects in this <c OMWeakReferenceVectorProperty>
  //        at <p index> + 1 and higher are shifted down one index
  //        position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The index of the object to remove.
template <typename ReferencedObject>
OMObject*
OMWeakReferenceVectorProperty<ReferencedObject>::removeObjectAt(
                                                            const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::removeObjectAt");

  PRECONDITION("Valid index", index < count());

  return removeAt(index);
}

  // @mfunc Insert <p object> into this <c OMWeakReferenceVectorProperty>
  //        at position <p index>. Existing objects at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to insert.
  //   @parm The index at which to insert the object.
template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::insertObjectAt(
                                                        const OMObject* object,
                                                        const size_t index)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::insertObjectAt");

  PRECONDITION("Valid index", index <= count());
  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  insertAt(p, index);
}

template <typename ReferencedObject>
OMContainerIterator<OMWeakReferenceVectorElement>*
OMWeakReferenceVectorProperty<ReferencedObject>::iterator(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::iterator");

  OMVectorIterator<VectorElement>* result =
                        new OMVectorIterator<VectorElement>(_vector, OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::insert(
                                   const size_t index,
                                   const OMWeakReferenceVectorElement& element)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::insert");

  _vector.setAt(element, index);
}

template <typename ReferencedObject>
OMPropertyTag
OMWeakReferenceVectorProperty<ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceVectorProperty<ReferencedObject>* nonConstThis =
            const_cast<OMWeakReferenceVectorProperty<ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                  file()->referencedProperties()->insert(targetPropertyPath());
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

template <typename ReferencedObject>
const OMUniqueObjectIdentification&
OMWeakReferenceVectorProperty<ReferencedObject>::identification(
                                                            size_t index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::identification");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  return element.identification();
}

template <typename ReferencedObject>
OMStrongReferenceSet*
OMWeakReferenceVectorProperty<ReferencedObject>::targetSet(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::targetSet");
  OMWeakReferenceVectorProperty<ReferencedObject>* nonConstThis =
            const_cast<OMWeakReferenceVectorProperty<ReferencedObject>*>(this);
  if (_targetSet == 0) {
    nonConstThis->_targetSet = OMWeakObjectReference::targetSet(this,
                                                                targetTag());
  }
  POSTCONDITION("Valid result", _targetSet != 0);
  return _targetSet;
}

template <typename ReferencedObject>
OMPropertyId
OMWeakReferenceVectorProperty<ReferencedObject>::keyPropertyId(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::keyPropertyId");

  return _keyPropertyId;
}

template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::setTargetTag(
                                                       OMPropertyTag targetTag)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::setTargetTag");

  _targetTag = targetTag;
}

template <typename ReferencedObject>
OMPropertyId*
OMWeakReferenceVectorProperty<ReferencedObject>::targetPropertyPath(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::targetPropertyPath");

  if (_targetPropertyPath == 0) {
    ASSERT("Valid target name", validWideString(_targetName));
    OMWeakReferenceVectorProperty<ReferencedObject>* nonConstThis =
            const_cast<OMWeakReferenceVectorProperty<ReferencedObject>*>(this);
    nonConstThis->_targetPropertyPath = file()->path(_targetName);
  }
  POSTCONDITION("Valid result", _targetPropertyPath != 0);
  return _targetPropertyPath;
}

template <typename ReferencedObject>
void
OMWeakReferenceVectorProperty<ReferencedObject>::clearTargetTag(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::clearTargetTag");

  OMWeakReferenceVectorProperty<ReferencedObject>* nonConstThis =
            const_cast<OMWeakReferenceVectorProperty<ReferencedObject>*>(this);

  nonConstThis->_targetTag = nullOMPropertyTag;
  delete [] nonConstThis->_targetPropertyPath;
  nonConstThis->_targetPropertyPath = 0;
}

template <typename ReferencedObject>
const wchar_t*
OMWeakReferenceVectorProperty<ReferencedObject>::targetName(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::targetName");

  return _targetName;
}

template <typename ReferencedObject>
bool
OMWeakReferenceVectorProperty<ReferencedObject>::isResolved(size_t index) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::isResolved");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  bool result;
  if (element.pointer() != 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

template <typename ReferencedObject>
bool
OMWeakReferenceVectorProperty<ReferencedObject>::isResolvable(
                                                     size_t ANAME(index)) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::isResolvable");
  PRECONDITION("Valid index", index < count());

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
void OMWeakReferenceVectorProperty<ReferencedObject>::shallowCopyTo(
                                                 OMProperty* destination) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::shallowCopyTo");
  PRECONDITION("Valid destination", destination != 0);

  typedef OMWeakReferenceVectorProperty Property;
  Property* dest = dynamic_cast<Property*>(destination);
  ASSERT("Destination is correct type", dest != 0);
  ASSERT("Valid destination", dest != this);

  ASSERT("Destination vector is void", dest->isVoid());
  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    dest->_vector.insert(element);
  }

  dest->_targetTag = nullOMPropertyTag;
  dest->_targetName = _targetName;
  delete [] dest->_targetPropertyPath;
  dest->_targetPropertyPath = 0;
  dest->_keyPropertyId = _keyPropertyId;
}

template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::deepCopyTo(
                                               OMProperty* /* destination */,
                                               void* /* clientContext */) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::deepCopyTo");
  // Nothing to do - this is a deep copy
}

#endif
