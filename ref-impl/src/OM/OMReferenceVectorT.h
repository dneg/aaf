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

#endif
