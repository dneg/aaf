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
#ifndef OMSTRONGREFSETPROPERTYT_H
#define OMSTRONGREFSETPROPERTYT_H

#include "OMAssertions.h"
#include "OMStoredSetIndex.h"
#include "OMStrongReferenceSetIter.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMStrongReferenceSetProperty>.
template <typename ReferencedObject>
OMStrongReferenceSetProperty<ReferencedObject>::
                    OMStrongReferenceSetProperty(const OMPropertyId propertyId,
                                                 const char* name)
: OMContainerProperty<ReferencedObject>(propertyId,
                                        SF_STRONG_OBJECT_REFERENCE_SET,
                                        name)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::"
                                               "OMStrongReferenceSetProperty");
}

  // @mfunc Destructor.
template <typename ReferencedObject>
OMStrongReferenceSetProperty<ReferencedObject>::
                                            ~OMStrongReferenceSetProperty(void)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::"
                                              "~OMStrongReferenceSetProperty");

}

  // @mfunc Save this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm Client context for callbacks.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::save(
                                                     void* clientContext) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::save");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());
  OMStoredObject* s = container->store();

  const char* propertyName = name();

  // create a set index
  //
  size_t count = _set.count();
  OMStoredSetIndex* index = new OMStoredSetIndex(count);
  index->setHighWaterMark(localKey());
  ASSERT("Valid heap pointer", index != 0);
  size_t position = 0;

  // Iterate over the set saving each element. The index entries
  // are written in order of their unique keys.
  //
  OMSetIterator<OMUniqueObjectIdentification,
                OMSetElement<OMStrongObjectReference<ReferencedObject>,
                             ReferencedObject> > iterator(_set, OMBefore);
  while (++iterator) {

    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject>& element = iterator.value();

    // enter into the index
    //
    index->insert(position,
                  element.localKey(),
                  element.referenceCount(),
                  element.identification());

    // save the object
    //
    element.save(clientContext);

    position = position + 1;

  }

  // save the set index
  //
  ASSERT("Valid set index", index->isValid());
  s->save(index, name());
  delete index;

  // make an entry in the property index
  //
  s->write(_propertyId,
           _storedForm,
           (void *)propertyName,
           strlen(propertyName) + 1);

}

  // @mfunc Close this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::close(void)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::close");

  OMSetIterator<OMUniqueObjectIdentification,
                OMSetElement<OMStrongObjectReference<ReferencedObject>,
                             ReferencedObject> > iterator(_set, OMBefore);
  while (++iterator) {
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject>& element = iterator.value();
    element.close();
  }
}

  // @mfunc Detach this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::detach(void)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::detach");

  OMSetIterator<OMUniqueObjectIdentification,
                OMSetElement<OMStrongObjectReference<ReferencedObject>,
                             ReferencedObject> > iterator(_set, OMBefore);
  while (++iterator) {
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject>& element = iterator.value();
    element.detach();
  }
}

  // @mfunc Restore this <c OMStrongReferenceSetProperty>, the external
  //        (persisted) size of the <c OMStrongReferenceSetProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The external (persisted) size of the
  //         <c OMStrongReferenceSetProperty>.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::restore(
                                                           size_t externalSize)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::restore");

  // get the name of the set index stream
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
  OMStoredSetIndex* setIndex = 0;
  store->restore(setIndex, name());
  ASSERT("Valid set index", setIndex->isValid());
  setLocalKey(setIndex->highWaterMark());

  // Iterate over the index restoring the elements of the set.
  // Since the index entries are stored on disk in order of their
  // unique keys this loop is the worst cast order of insertion. This
  // code will eventually be replaced by code that inserts the keys in
  // "binary search" order. That is the middle key is inserted first
  // then (recursively) all the keys below the middle key followed by
  // (recursively) all the keys above the middle key.
  //
  size_t entries = setIndex->entries();
  size_t context = 0;
  OMUInt32 localKey;
  OMUInt32 count;
  OMUniqueObjectIdentification key;
  for (size_t i = 0; i < entries; i++) {
    setIndex->iterate(context, localKey, count, key);
    char* name = elementName(localKey);
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject> newElement(this, name, localKey, key);
    newElement.restore();
    _set.insert(newElement);
    delete [] name;
    name = 0; // for BoundsChecker
  }
  delete setIndex;
  setPresent();
}

  // @mfunc The number of <p ReferencedObject>s in this
  //        <c OMStrongReferenceSetProperty>.
  //   @this const
template <typename ReferencedObject>
size_t OMStrongReferenceSetProperty<ReferencedObject>::count(void) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::count");

  return _set.count();
}

  // @mfunc Get the size of this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @rdesc The size of this <c OMStrongReferenceSetProperty>.
  //     @this const
template <typename ReferencedObject>
size_t OMStrongReferenceSetProperty<ReferencedObject>::getSize(void) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::getSize");
  OBSOLETE("OMStrongReferenceSetProperty<ReferencedObject>::count");

  return count();
}

  // @mfunc Insert <p object> into this
  //        <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to insert.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::insert");

  PRECONDITION("Valid object", object != 0);

  // Set the set to contain the new object
  //
  const size_t localKey = nextLocalKey();
  char* name = elementName(localKey);
  OMUniqueObjectIdentification key = object->identification();

  OMSetElement<OMStrongObjectReference<ReferencedObject>,
               ReferencedObject> newElement(this, name, localKey, key);
  newElement.setValue(object);
  _set.insert(newElement);
  setPresent();
  delete [] name;

  //POSTCONDITION("Optional property is present", isPresent());
}

  // @mfunc Append the given <p ReferencedObject> <p object> to
  //        this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::appendValue");
  PRECONDITION("Valid object", object != 0);

  OBSOLETE("OMStrongReferenceSetProperty<ReferencedObject>::insert");
  insert(object);
}

  // @mfunc Remove the <p ReferencedObject> identified by
  //        <p identification> from this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The unique identification of the object to be removed, the
  //         search key.
  //   @rdesc A pointer to the removed <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject*
OMStrongReferenceSetProperty<ReferencedObject>::remove(
                            const OMUniqueObjectIdentification& identification)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::remove");

  PRECONDITION("Object is present", contains(identification));

  OMSetElement<OMStrongObjectReference<ReferencedObject>,
               ReferencedObject>* element = 0;
  bool found = _set.find(identification, &element);
  ASSERT("Object found", found);
  ReferencedObject* result = element->setValue(0);
  _set.remove(identification);

  POSTCONDITION("Object is not present", !contains(identification));

  return result;
}

  // @mfunc Remove <p object> from this
  //        <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  OMUniqueObjectIdentification identification = object->identification();
  remove(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
}

  // @mfunc Does this <c OMStrongReferenceSetProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
bool OMStrongReferenceSetProperty<ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::containsValue");
  PRECONDITION("Valid object", object != 0);

  bool result = _set.contains(object->identification());
  return result;
}

  // @mfunc Does this <c OMStrongReferenceSetProperty> contain a
  //        <p ReferencedObject> identified by <p identification>?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The unique identification of the desired object, the search key.
  //   @rdesc True if the object is found, false otherwise.
template <typename ReferencedObject>
bool OMStrongReferenceSetProperty<ReferencedObject>::contains(
                      const OMUniqueObjectIdentification& identification) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::contains");

  return _set.contains(identification);
}

  // @mfunc Find the <p ReferencedObject> in this
  //        <c OMStrongReferenceSetProperty> identified by
  //        <p identification>.  If the object is found it is returned
  //        in <p object> and the result is true. If the element is
  //        not found the result is false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The unique identification of the desired object, the search key.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @rdesc True if the object is found, false otherwise.
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceSetProperty<ReferencedObject>::find(
                            const OMUniqueObjectIdentification& identification,
                            ReferencedObject*& object) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::find");

  OMSetElement<OMStrongObjectReference<ReferencedObject>,
               ReferencedObject>* element = 0;

  bool result = _set.find(identification, &element);
  if (result) {
    object = element->getValue();
  }

  return result;
}

  // @mfunc Is this <c OMStrongReferenceSetProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @rdesc True if this <c OMStrongReferenceSetProperty> is void,
  //          false otherwise. 
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceSetProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::isVoid");

  bool result = true;

  OMSetIterator<OMUniqueObjectIdentification,
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject> > iterator(_set, OMBefore);
  while (++iterator) {
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject>& element = iterator.value();
    ReferencedObject* object = element.getValue();
    if (object != 0) {
      result = false;
      break;
    }
  }
  return result;
}

  // @mfunc Remove this optional <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::remove(void)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::remove");

  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}
  // @mfunc The size of the raw bits of this
  //        <c OMStrongReferenceSetProperty>. The size is given in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @rdesc The size of the raw bits of this
  //          <c OMStrongReferenceSetProperty> in bytes.
  //   @this const
template<typename ReferencedObject>
size_t OMStrongReferenceSetProperty<ReferencedObject>::bitsSize(void) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::bitsSize");

  return sizeof(ReferencedObject*) * count();
}

  // @mfunc Get the raw bits of this <c OMStrongReferenceSetProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
  //   @this const
template<typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::getBits(OMByte* bits,
                                                             size_t size) const
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::getBits");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject** p = (const ReferencedObject**)bits;

  OMSetIterator<OMUniqueObjectIdentification,
                OMSetElement<OMStrongObjectReference<ReferencedObject>,
                             ReferencedObject> > iterator(_set, OMBefore);
  while (++iterator) {
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject>& element = iterator.value();
    *p++ = element.getValue();
  }
}

  // @mfunc Set the raw bits of this
  //        <c OMStrongReferenceSetProperty>. The raw bits are
  //        copied from the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
template<typename ReferencedObject>
void OMStrongReferenceSetProperty<ReferencedObject>::setBits(
                                                            const OMByte* bits,
                                                            size_t size)
{
  TRACE("OMStrongReferenceSetProperty<ReferencedObject>::setBits");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  ReferencedObject** p = (ReferencedObject**)bits;

  OMSetIterator<OMUniqueObjectIdentification,
                OMSetElement<OMStrongObjectReference<ReferencedObject>,
                             ReferencedObject> > iterator(_set, OMBefore);
  while (++iterator) {
    OMSetElement<OMStrongObjectReference<ReferencedObject>,
                 ReferencedObject>& element = iterator.value();
    element.setValue(*p++);
  }
}

#endif

