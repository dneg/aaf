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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMReferenceVector
#ifndef OMREFERENCEVECTORT_H
#define OMREFERENCEVECTORT_H

#include "OMAssertions.h"
#include "OMReferenceVectorIter.h"

  // @mfunc Constructor.
template <typename ReferencedObject>
OMReferenceVector<ReferencedObject>::
                 OMReferenceVector(void)
{
  TRACE("OMReferenceVector<ReferencedObject>::OMReferenceVector");
}

  // @mfunc Destructor.
template <typename ReferencedObject>
OMReferenceVector<ReferencedObject>::~OMReferenceVector(void)
{
  TRACE("OMReferenceVector<ReferencedObject>::~OMReferenceVector");
}

  // @mfunc The number of <p ReferencedObject>s in this <c OMReferenceVector>.
  //   @this const
template <typename ReferencedObject>
size_t OMReferenceVector<ReferencedObject>::count(void) const
{
  return _vector.count();
}

  // @mfunc Set the value of this <c OMReferenceVector>
  //        at position <p index> to <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
  //   @rdesc A pointer to the old <p ReferencedObject>.
template <typename ReferencedObject>
ReferencedObject* OMReferenceVector<ReferencedObject>::setValueAt(
                                                const ReferencedObject* object,
                                                const size_t index)
{
  TRACE("OReferenceVector<ReferencedObject>::setValueAt");
  PRECONDITION("Valid index", index <= count());

  if (index == count()) {
    // This is an append, make sure the new element is defined.
    VectorElement newElement(object);
    _vector.append(newElement);
  }

  // Set the vector to contain the new object
  //
  VectorElement& element = _vector.getAt(index);
  ReferencedObject* oldObject = element.setValue(object);

  POSTCONDITION("Object properly inserted",
                                    _vector.getAt(index).getValue() == object);
  return oldObject;
}

  // @mfunc Set the value of this <c OMReferenceVector>
  //        at position <p index> to 0.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The position to clear.
  //   @rdesc A pointer to the old <p ReferencedObject>.
template <typename ReferencedObject>
ReferencedObject*
OMReferenceVector<ReferencedObject>::clearValueAt(const size_t index)
{
  TRACE("OReferenceVector<ReferencedObject>::clearValueAt");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);
  ReferencedObject* oldObject = element.setValue(0);

  POSTCONDITION("Object properly cleared",
                                         _vector.getAt(index).getValue() == 0);
  return oldObject;
}

  // @mfunc The value of this <c OMReferenceVector> at position <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMReferenceVector<ReferencedObject>::valueAt(
                                                      const size_t index) const
{
  TRACE("OMReferenceVector<ReferencedObject>::valueAt");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  ReferencedObject* result = element.getValue();
  return result;
}

  // @mfunc Get the value of this <c OMReferenceVector>
  //        at position <p index> into <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::getValueAt(
                                                     ReferencedObject*& object,
                                                     const size_t index) const
{
  TRACE("OMReferenceVector<ReferencedObject>::getValueAt");
  OBSOLETE("OMReferenceVector<ReferencedObject>::valueAt");
  PRECONDITION("Valid index", index < count());

  VectorElement& element = _vector.getAt(index);

  object = element.getValue();

}

  // @mfunc If <p index> is valid, get the value of this
  //        <c OMReferenceVector> at position <p index>
  //        into <p object> and return true, otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @rdesc True if <p index> is valid, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVector<ReferencedObject>::find(
                                               const size_t index,
                                               ReferencedObject*& object) const
{
  TRACE("OMReferenceVector<ReferencedObject>::find");

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
  //        this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::appendValue");

  setValueAt(object, count());

}

  // @mfunc Prepend the given <p ReferencedObject> <p object> to
  //        this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::prependValue(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::prependValue");

  insertAt(object, 0);
}

  // @mfunc Insert <p object> into this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::insert");

  appendValue(object);
}

  // @mfunc Insert <p value> into this <c OMReferenceVector>
  //        at position <p index>. Existing values at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::insertAt(
                            const ReferencedObject* object, const size_t index)
{
  TRACE("OMReferenceVector<ReferencedObject>::insertAt");

  PRECONDITION("Valid index", index <= count());

  VectorElement newElement(object);
  _vector.insertAt(newElement, index);

  POSTCONDITION("Object properly inserted",
                                    _vector.getAt(index).getValue() == object);
}

  // @mfunc Does this <c OMReferenceVector> contain <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVector<ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMReferenceVector<ReferencedObject>::containsValue");

  PRECONDITION("Valid object", object != 0);

  bool result = false;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == object) {
      result = true;
      break;
    }
  }

  return result;
}

  // @mfunc Remove <p object> from this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  size_t index = indexOfValue(object);
  removeAt(index);
}

  // @mfunc Remove the object from this
  //        <c OMReferenceVector> at position <p index>.
  //        Existing objects in this <c OMReferenceVector>
  //        at <p index> + 1 and higher are shifted down one index
  //        position.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The position from which to remove the <p ReferencedObject>.
  //   @rdesc A pointer to the removed <p ReferencedObject>.
template <typename ReferencedObject>
ReferencedObject*
OMReferenceVector<ReferencedObject>::removeAt(const size_t index)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeAt");
  PRECONDITION("Valid index", index < count());

  ReferencedObject* result = clearValueAt(index);
  _vector.removeAt(index);
  return result;
}

  // @mfunc Remove the last (index == count() - 1) object
  //        from this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @rdesc A pointer to the removed <p ReferencedObject>.
template <typename ReferencedObject>
ReferencedObject*
OMReferenceVector<ReferencedObject>::removeLast(void)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeLast");

  return removeAt(count() - 1);
}

  // @mfunc Remove the first (index == 0) object
  //        from this <c OMReferenceVector>. Existing
  //        objects in this <c OMReferenceVector> are
  //        shifted down one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @rdesc A pointer to the removed <p ReferencedObject>.
template <typename ReferencedObject>
ReferencedObject*
OMReferenceVector<ReferencedObject>::removeFirst(void)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeFirst");

  return removeAt(0);
}

  // @mfunc The index of the <p ReferencedObject*> <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm A pointer to the <p ReferencedObject> to find.
  //   @rdesc The index.
  //   @this const
template <typename ReferencedObject>
size_t OMReferenceVector<ReferencedObject>::indexOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMReferenceVector<ReferencedObject>::indexOfValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  size_t result;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == object) {
      result = iterator.index();
      break;
    }
  }
  return result;
}

  // @mfunc The number of occurrences of <p object> in this
  //        <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The object to count.
  //   @rdesc The number of occurrences.
  //   @this const
template <typename ReferencedObject>
size_t OMReferenceVector<ReferencedObject>::countOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMReferenceVector<ReferencedObject>::countOfValue");

  PRECONDITION("Valid object", object != 0);

  size_t result = 0;

  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {
    VectorElement& element = iterator.value();
    if (element.pointer() == object) {
      result = result + 1;
    }
  }
  return result;
}

  // @mfunc Does this <c OMReferenceVector> contain
  //        <p index> ? Is <p index> valid ?
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The index.
  //   @rdesc True if the index is valid, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVector<ReferencedObject>::containsIndex(
                                                      const size_t index) const
{
  TRACE("OMReferenceVector<ReferencedObject>::containsIndex");

  bool result;
  if (index < count()) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc If this <c OMReferenceVector> contains <p object>
  //        then place its index in <p index> and return true, otherwise
  //        return false.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The object for which to search.
  //   @parm The index of the object.
  //   @rdesc True if the object was found, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMReferenceVector<ReferencedObject>::findIndex(
                           const ReferencedObject* object, size_t& index) const
{
  TRACE("OMReferenceVector<ReferencedObject>::findIndex");
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

  // @mfunc Increase the capacity of this <c OMReferenceVector> so that it
  //        can contain at least <p capacity> <p ReferencedObject>s
  //        without having to be resized.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm The desired capacity.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::grow(const size_t capacity)
{
  TRACE("OMReferenceVector<ReferencedObject>::grow");
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

  // @mfunc Insert <p object> into this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::insertObject(const OMObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::insertObject");
  appendObject(object);
}

  // @mfunc Does this <c OMReferenceVector> contain <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename ReferencedObject>
bool
OMReferenceVector<ReferencedObject>::containsObject(
                                                  const OMObject* object) const
{
  TRACE("OMReferenceVector<ReferencedObject>::containsObject");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  return containsValue(obj);
}

  // @mfunc Remove <p object> from this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::removeObject(const OMObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeObject");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  removeValue(obj);
}

  // @mfunc Remove all objects from this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::removeAllObjects(void)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeAllObjects");

  _vector.clear();
  POSTCONDITION("All objects removed", count() == 0);
}

  // @mfunc Create an <c OMReferenceContainerIterator> over this
  //        <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @rdesc TBS
template <typename ReferencedObject>
OMReferenceContainerIterator*
OMReferenceVector<ReferencedObject>::createIterator(void) const
{
  TRACE("OMReferenceVector<ReferencedObject>::createIterator");

  OMReferenceVectorIterator<ReferencedObject>* result =
              new OMReferenceVectorIterator<ReferencedObject>(*this, OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Set the value of this <c OMReferenceVector>
  //        at position <p index> to <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename ReferencedObject>
OMObject*
OMReferenceVector<ReferencedObject>::setObjectAt(const OMObject* object,
                                                 const size_t index)
{
  TRACE("OMReferenceVector<ReferencedObject>::setObjectAt");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  return setValueAt(obj, index);
}

  // @mfunc The value of this <c OMReferenceVector>
  //        at position <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename ReferencedObject>
OMObject*
OMReferenceVector<ReferencedObject>::getObjectAt(const size_t index) const
{
  TRACE("OMReferenceVector<ReferencedObject>::getObjectAt");

  return valueAt(index);
}

  // @mfunc Append the given <p OMObject> <p object> to
  //        this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::appendObject(const OMObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::appendObject");

  insertObjectAt(object, count());
}

  // @mfunc Prepend the given <p OMObject> <p object> to
  //        this <c OMReferenceVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
template <typename ReferencedObject>
void OMReferenceVector<ReferencedObject>::prependObject(const OMObject* object)
{
  TRACE("OMReferenceVector<ReferencedObject>::prependObject");

  insertObjectAt(object, 0);
}

  // @mfunc Remove the object from this
  //        <c OMReferenceVector> at position <p index>.
  //        Existing objects in this <c OMReferenceVector>
  //        at <p index> + 1 and higher are shifted down one index
  //        position.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @rdesc TBS
template <typename ReferencedObject>
OMObject*
OMReferenceVector<ReferencedObject>::removeObjectAt(const size_t index)
{
  TRACE("OMReferenceVector<ReferencedObject>::removeObjectAt");

  return removeAt(index);
}

  // @mfunc Insert <p object> into this <c OMReferenceVector>
  //        at position <p index>. Existing objects at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @parm TBS
  //   @parm TBS
template <typename ReferencedObject>
void
OMReferenceVector<ReferencedObject>::insertObjectAt(const OMObject* object,
                                                    const size_t index)
{
  TRACE("OMReferenceVector<ReferencedObject>::insertObjectAt");

  const ReferencedObject* obj = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", obj != 0);
  ReferencedObject* o = const_cast<ReferencedObject*>(obj);
  insertAt(o, index);
}

#endif
