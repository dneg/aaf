/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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
#ifndef OMSTRONGREFVECTORPROPERTYT_H
#define OMSTRONGREFVECTORPROPERTYT_H

#include "OMAssertions.h"
#include "OMStoredVectorIndex.h"
#include "OMStrongReferenceVectorIter.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMStrongReferenceVectorProperty>.
template <typename ReferencedObject>
OMStrongReferenceVectorProperty<ReferencedObject>::
                 OMStrongReferenceVectorProperty(const OMPropertyId propertyId,
                                                 const char* name)
: OMContainerProperty<ReferencedObject>(propertyId,
                                        SF_STRONG_OBJECT_REFERENCE_VECTOR,
                                        name)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::"
                                            "OMStrongReferenceVectorProperty");
}

  // @mfunc Destructor.
template <typename ReferencedObject>
OMStrongReferenceVectorProperty<ReferencedObject>::
                                         ~OMStrongReferenceVectorProperty(void)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::"
                                           "~OMStrongReferenceVectorProperty");
  size_t count = _vector.count();
  for (size_t i = 0; i < count; i++) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = _vector.getAt(i);
    element.setValue(0);
  }
}

  // @mfunc Save this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm Client context for callbacks.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::save(
                                                     void* clientContext) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::save");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());
  OMStoredObject* s = container->store();

  const char* propertyName = name();

  // create a vector index
  //
  size_t count = _vector.count();
  OMStoredVectorIndex* index = new OMStoredVectorIndex(count);
  ASSERT("Valid heap pointer", index != 0);
  index->setHighWaterMark(localKey());
  size_t position = 0;

  // Iterate over the vector saving each element
  //
  OMVectorIterator<
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> > iterator(_vector, OMBefore);
  while (++iterator) {

    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = iterator.value();

    // enter into the index
    //
    index->insert(position, element.localKey());

    // save the object
    //
    element.save(clientContext);

    position = position + 1;

  }

  // save the vector index
  //
  ASSERT("Valid vector index", index->isValid());
  s->save(index, name());
  delete index;

  // make an entry in the property index
  //
  s->write(_propertyId,
           _storedForm,
           (void *)propertyName,
           strlen(propertyName) + 1);

}

  // @mfunc Close this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::close(void)
{
  size_t count = _vector.count();
  for (size_t i = 0; i < count; i++) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = _vector.getAt(i);
    element.close();
  }
}

  // @mfunc Detach this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::detach(void)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::detach");
  size_t count = _vector.count();
  for (size_t i = 0; i < count; i++) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = _vector.getAt(i);
    element.detach();
  }
}

  // @mfunc Restore this <c OMStrongReferenceVectorProperty>, the external
  //        (persisted) size of the <c OMStrongReferenceVectorProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The external (persisted) size of the
  //         <c OMStrongReferenceVectorProperty>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::restore(
                                                           size_t externalSize)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::restore");

  // get the name of the vector index stream
  //
  char* propertyName = new char[externalSize];
  ASSERT("Valid heap pointer", propertyName != 0);
  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  store->read(_propertyId, _storedForm, propertyName, externalSize);
  ASSERT("Consistent property size", externalSize == strlen(propertyName) + 1);
  ASSERT("Consistent property name", strcmp(propertyName, name()) == 0);
  delete [] propertyName;

  // restore the index
  //
  OMStoredVectorIndex* vectorIndex = 0;
  store->restore(vectorIndex, name());
  ASSERT("Valid vector index", vectorIndex->isValid());
  setLocalKey(vectorIndex->highWaterMark());

  // Iterate over the index restoring the elements of the vector
  //
  size_t entries = vectorIndex->entries();
  if (entries > 0) {
    grow(entries); // Set the vector size
    size_t context = 0;
    OMUInt32 localKey;
    for (size_t i = 0; i < entries; i++) {
      vectorIndex->iterate(context, localKey);
      char* name = elementName(localKey);
      OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                      ReferencedObject> newElement(this, name, localKey);
      newElement.restore();
      _vector.setAt(newElement, i);
      delete [] name;
      name = 0; // for BoundsChecker
    }
  }
  delete vectorIndex;
  setPresent();
}

  // @mfunc The number of <p ReferencedObject>s in this
  //        <c OMStrongReferenceVectorProperty>.
  //   @this const
template <typename ReferencedObject>
size_t OMStrongReferenceVectorProperty<ReferencedObject>::count(void) const
{
  return _vector.count();
}

  // @mfunc Get the size of this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @parm The size of this <c OMStrongReferenceVectorProperty>.
  //     @this const
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::getSize(
                                                            size_t& size) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::getSize");
  OBSOLETE("OMStrongReferenceVectorProperty<ReferencedObject>::count");

  size = count();
}

  // @mfunc Get the size of this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @rdesc The size of this <c OMStrongReferenceVectorProperty>.
  //     @this const
template <typename ReferencedObject>
size_t OMStrongReferenceVectorProperty<ReferencedObject>::getSize(void) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::getSize");
  OBSOLETE("OMStrongReferenceVectorProperty<ReferencedObject>::count");

  return count();
}

  // @mfunc Set the value of this <c OMStrongReferenceVectorProperty>
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
ReferencedObject* 
                 OMStrongReferenceVectorProperty<ReferencedObject>::setValueAt(
                                                const ReferencedObject* object,
                                                const size_t index)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::setValueAt");
  PRECONDITION("Valid index", (index >= 0) && (index <= count()));
  
  if (index == count()) {
    // This is an append, make sure the new element is defined.
    OMUInt32 localKey = nextLocalKey();
    char* name = elementName(localKey);
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> newElement(this, name, localKey);
    _vector.append(newElement);
    delete [] name;
  }

  // Set the vector to contain the new object
  //
  OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                  ReferencedObject>& element = _vector.getAt(index);
  ReferencedObject* oldObject = element.setValue(object);
  setPresent();

  POSTCONDITION("Object properly inserted",
                                    _vector.getAt(index).getValue() == object);
  return oldObject;
}

  // @mfunc Get the value of this <c OMStrongReferenceVectorProperty>
  //        at position <p index> into <p object>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::getValueAt(
                                                     ReferencedObject*& object,
                                                     const size_t index) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::getValueAt");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid index", ((index >= 0) && (index < count())));

  OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                  ReferencedObject>& element = _vector.getAt(index);

  object = element.getValue();

}

  // @mfunc Append the given <p ReferencedObject> <p object> to
  //        this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::appendValue");

  setValueAt(object, count());

}

  // @mfunc Prepend the given <p ReferencedObject> <p object> to
  //        this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::prependValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::prependValue");

  insertAt(object, 0);
}

  // @mfunc Insert <p object> into this
  //        <c OMStrongReferenceVectorProperty>. This function is
  //        redefined from <c OMContainerProperty> as
  //        <mf OMStrongReferenceVectorProperty::appendValue>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::insert");

  appendValue(object);
}

  // @mfunc Insert <p value> into this <c OMStrongReferenceVectorProperty>
  //        at position <p index>. Existing values at <p index> and
  //        higher are shifted up one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @parm The position at which to insert the <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::insertAt(
                            const ReferencedObject* object, const size_t index)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::insertAt");

  PRECONDITION("Valid index", (index >= 0) && (index <= count()));
  
  OMUInt32 localKey = nextLocalKey();
  char* name = elementName(localKey);
  OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                  ReferencedObject> newElement(this, name, localKey);
  newElement.setValue(object);
  _vector.insertAt(newElement, index);
  delete [] name;
  setPresent();

  POSTCONDITION("Object properly inserted",
                                    _vector.getAt(index).getValue() == object);
}

  // @mfunc Does this <c OMStrongReferenceVectorProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceVectorProperty<ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::containsValue");

  bool result = false;
  OMVectorIterator<
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> > iterator(_vector, OMBefore);
  // This loop causes objects to be loaded but the one we are
  // looking for must already be loaded.
  while (++iterator) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = iterator.value();
    if (element.getValue() == object) {
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc Remove <p object> from this
  //        <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::removeValue");

  PRECONDITION("Object is present", containsValue(object));

  size_t index = indexOfValue(object);
  removeAt(index);
}

  // @mfunc Remove the object from this
  //        <c OMStrongReferenceVectorProperty> at position <p index>.
  //        Existing objects in this <c OMStrongReferenceVectorProperty>
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
OMStrongReferenceVectorProperty<ReferencedObject>::removeAt(const size_t index)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::removeLast");

  ReferencedObject* result = setValueAt(0, index);
  _vector.removeAt(index);
  return result;
}

  // @mfunc Remove the last (index == count() - 1) object
  //        from this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMStrongReferenceVectorProperty<ReferencedObject>::removeLast(void)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::removeLast");

  return removeAt(count() - 1);
}

  // @mfunc Remove the first (index == 0) object
  //        from this <c OMStrongReferenceVectorProperty>. Existing
  //        objects in this <c OMStrongReferenceVectorProperty> are
  //        shifted down one index position.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMStrongReferenceVectorProperty<ReferencedObject>::removeFirst(void)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::removeFirst");

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
size_t OMStrongReferenceVectorProperty<ReferencedObject>::indexOfValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::indexOfValue");

  PRECONDITION("Object is present", containsValue(object));

  size_t result;

  OMVectorIterator<
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> > iterator(_vector, OMBefore);
  // This loop causes objects to be loaded but the one we are
  // looking for must already be loaded.
  while (++iterator) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = iterator.value();
    if (element.getValue() == object) {
      result = iterator.index();
      break;
    }
  }
  return result;
}

  // @mfunc Increase the capacity of this
  //        <c OMStrongReferemceVectorProperty> so that it
  //        can contain at least <p capacity> <p ReferencedObject>s
  //        without having to be resized.
  //   @parm The desired capacity.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::grow(
                                                         const size_t capacity)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::grow");
  PRECONDITION("Valid capacity", capacity > count());

  // Increase the capacity of the vector.
  size_t oldCount = _vector.count();
  _vector.grow(capacity);

  // Make sure the new elements are defined.
  for (size_t i = oldCount; i < capacity; i++) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> voidElement;
    _vector.insert(voidElement);
  }
}

  // @mfunc Is this <c OMStrongReferenceVectorProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc True if this <c OMStrongReferenceVectorProperty> is void,
  //          false otherwise. 
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceVectorProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::isVoid");

  bool result = true;

  OMVectorIterator<
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> > iterator(_vector, OMBefore);
  while (++iterator) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = iterator.value();
    ReferencedObject* object = element.getValue();
    if (object != 0) {
      result = false;
      break;
    }
  }
  return result;
}

  // @mfunc Remove this optional <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::remove(void)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::remove");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc The size of the raw bits of this
  //        <c OMStrongReferenceVectorProperty>. The size is given in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The size of the raw bits of this
  //          <c OMStrongReferenceVectorProperty> in bytes.
  //   @this const
template<typename ReferencedObject>
size_t OMStrongReferenceVectorProperty<ReferencedObject>::bitsSize(void) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::bitsSize");

  return sizeof(ReferencedObject*) * count();
}

  // @mfunc Get the raw bits of this <c OMStrongReferenceVectorProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
  //   @this const
template<typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::getBits");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject** p = (const ReferencedObject**)bits;

  OMVectorIterator<
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject> > iterator(_vector, OMBefore);
  while (++iterator) {
    OMVectorElement<OMStrongObjectReference<ReferencedObject>,
                    ReferencedObject>& element = iterator.value();
    *p++ = element.getValue();
  }
}

  // @mfunc Set the raw bits of this
  //        <c OMStrongReferenceVectorProperty>. The raw bits are
  //        copied from the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
template<typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::setBits(
                                                            const OMByte* bits,
                                                            size_t size)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::setBits");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  size_t count = size / sizeof(ReferencedObject*);
  ReferencedObject** p = (ReferencedObject**)bits;

  for (size_t i = 0; i < count; i++) {
    ReferencedObject* object = p[i];
    setValueAt(object, i);
  }

}

#endif
