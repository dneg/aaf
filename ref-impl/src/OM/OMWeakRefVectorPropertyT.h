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
#ifndef OMWEAKREFVECTORPROPERTYT_H
#define OMWEAKREFVECTORPROPERTYT_H

#include "OMAssertions.h"
#include "OMStoredVectorIndex.h"
#include "OMWeakReferenceVectorIter.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceVectorProperty>.
  //   @parm The name of the the <c OMProperty> instance (a set property)
  //         in which the objects referenced by the elements of this
  //         <c OMWeakReferenceVectorProperty> reside.
template <typename ReferencedObject>
OMWeakReferenceVectorProperty<ReferencedObject>::
                OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                              const wchar_t* name,
                                              const wchar_t* targetName,
                                              const OMPropertyId keyPropertyId)
: OMContainerProperty<ReferencedObject>(propertyId,
                                        SF_WEAK_OBJECT_REFERENCE_VECTOR,
                                        name),
  _targetTag(nullOMPropertyTag),
  _targetName(targetName),
  _keyPropertyId(keyPropertyId)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::"
                                              "OMWeakReferenceVectorProperty");
}

  // @mfunc Destructor.
template <typename ReferencedObject>
OMWeakReferenceVectorProperty<ReferencedObject>::
                                           ~OMWeakReferenceVectorProperty(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::"
                                             "~OMWeakReferenceVectorProperty");
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

  OMPropertyTag tag = targetTag();

  // create a vector index
  //
  size_t count = _vector.count();
  OMUniqueObjectIdentification* index = 0;
  if (count > 0) {
    index = new OMUniqueObjectIdentification[count];
    ASSERT("Valid heap pointer", index != 0);
  }
  size_t position = 0;

  // Iterate over the vector saving each element. The index entries
  // are written in order of their unique keys.
  //
  VectorIterator iterator(_vector, OMBefore);
  while (++iterator) {

    VectorElement& element = iterator.value();

    // enter into the index
    //
    index[position] = element.identification();

    // save the object
    //
    element.save();

    position = position + 1;

  }

  // save the vector index
  //
  store()->save(storedName(),
                index,
                count,
                tag,
                _keyPropertyId);
  delete [] index;

  // make an entry in the property index
  //
  saveName();

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

  // get the name of the vector index stream
  //
  restoreName(externalSize);

  // restore the index
  //
  OMUniqueObjectIdentification* vectorIndex = 0;
  size_t entries;
  OMPropertyTag tag;
  OMPropertyId keyPropertyId;
  store()->restore(storedName(),
                   vectorIndex,
                   entries,
                   tag,
                   keyPropertyId);

  ASSERT("Valid vector index", IMPLIES(entries != 0, vectorIndex != 0));
  ASSERT("Valid vector index", IMPLIES(entries == 0, vectorIndex == 0));
  ASSERT("Consistent key property ids", keyPropertyId == _keyPropertyId);
  _targetTag = tag;

  // Iterate over the index restoring the elements of the vector.
  //
  if (entries > 0) {
    grow(entries); // Set the vector size
    for (size_t i = 0; i < entries; i++) {
      OMUniqueObjectIdentification key = vectorIndex[i];
      VectorElement newElement(this, key, _targetTag);
      newElement.restore();
      _vector.setAt(newElement, i);
    }
  }
  delete [] vectorIndex;
  setPresent();
}

  // @mfunc The number of <p ReferencedObject>s in this
  //        <c OMWeakReferenceVectorProperty>.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::count(void) const
{
  return _vector.count();
}

  // @mfunc Get the size of this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @parm The size of this <c OMWeakReferenceVectorProperty>.
  //     @this const
template <typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::getSize(
                                                            size_t& size) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::getSize");
  OBSOLETE("OMWeakReferenceVectorProperty<ReferencedObject>::count");

  size = count();
}

  // @mfunc Get the size of this <c OMWeakReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @rdesc The size of this <c OMWeakReferenceVectorProperty>.
  //     @this const
template <typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::getSize(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::getSize");
  OBSOLETE("OMWeakReferenceVectorProperty<ReferencedObject>::count");

  return count();
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

  PRECONDITION("Valid index", (index >= 0) && (index <= count()));
  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
#endif
  if (index == count()) {
    // This is an append, make sure the new element is defined.
    OMUniqueObjectIdentification key = object->identification();
    VectorElement newElement(this, key, _targetTag);
    _vector.append(newElement);
  }

  // Set the vector to contain the new object
  //
  VectorElement& element = _vector.getAt(index);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  element.reference().setTargetTag(targetTag());
#endif
  ReferencedObject* oldObject = element.setValue(object);
  setPresent();

  POSTCONDITION("Object properly inserted",
                                    _vector.getAt(index).getValue() == object);
  return oldObject;
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
  PRECONDITION("Valid index", ((index >= 0) && (index < count())));

  VectorElement& element = _vector.getAt(index);

  ReferencedObject* result = element.getValue();
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
  PRECONDITION("Valid index", ((index >= 0) && (index < count())));

  VectorElement& element = _vector.getAt(index);

  object = element.getValue();

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

  setValueAt(object, count());

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

  PRECONDITION("Valid index", (index >= 0) && (index <= count()));
  PRECONDITION("Valid object", object != 0);

  OMUniqueObjectIdentification key = object->identification();
  VectorElement newElement(this, key, _targetTag);
  newElement.setValue(object);
  _vector.insertAt(newElement, index);
  setPresent();

  POSTCONDITION("Object properly inserted",
                                    _vector.getAt(index).getValue() == object);
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
    if (element.pointer() == object) {
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
  PRECONDITION("Valid index", (index >= 0) && (index <= count()));

  ReferencedObject* result = setValueAt(0, index);
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
    if (element.pointer() == object) {
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
    if (element.pointer() == object) {
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
  //        <c OMWeakReferemceVectorProperty> so that it
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
    ReferencedObject* object = element.getValue();
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
void OMWeakReferenceVectorProperty<ReferencedObject>::remove(void)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::remove");
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
template<typename ReferencedObject>
size_t OMWeakReferenceVectorProperty<ReferencedObject>::bitsSize(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::bitsSize");

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
template<typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::getBits");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject** p = (const ReferencedObject**)bits;

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
template<typename ReferencedObject>
void OMWeakReferenceVectorProperty<ReferencedObject>::setBits(
                                                            const OMByte* bits,
                                                            size_t size)
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::setBits");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  size_t count = size / sizeof(ReferencedObject*);
  ReferencedObject** p = (ReferencedObject**)bits;

  for (size_t i = 0; i < count; i++) {
    ReferencedObject* object = p[i];
    setValueAt(object, i);
  }

}

template<typename ReferencedObject>
OMPropertyTag
OMWeakReferenceVectorProperty<ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceVectorProperty<ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceVectorProperty<ReferencedObject>* nonConstThis =
            const_cast<OMWeakReferenceVectorProperty<ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                           file()->referencedProperties()->insert(_targetName);
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

#endif
