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
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in this set.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::OMStrongReferenceSetProperty(
                                              const OMPropertyId propertyId,
                                              const char* name,
                                              const OMPropertyId keyPropertyId)
: OMContainerProperty<ReferencedObject>(propertyId,
                                        SF_STRONG_OBJECT_REFERENCE_SET,
                                        name),
  _keyPropertyId(keyPropertyId)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::"
                                               "OMStrongReferenceSetProperty");
}

  // @mfunc Destructor.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::~OMStrongReferenceSetProperty(
                                                                          void)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::"
                                              "~OMStrongReferenceSetProperty");

}

  // @mfunc Save this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::save(void) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::save");

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
  size_t keySize = sizeof(UniqueIdentification);
  ASSERT("Valid key size", keySize <= (OMKeySize)~0);
  OMStoredSetIndex* index = new OMStoredSetIndex(
                                              count,
                                              _keyPropertyId,
                                              static_cast<OMKeySize>(keySize));
  ASSERT("Valid heap pointer", index != 0);
  index->setFirstFreeKey(localKey());
  size_t position = 0;

  // Iterate over the set saving each element. The index entries
  // are written in order of their unique keys.
  //
  SetIterator iterator(_set, OMBefore);
  while (++iterator) {

    SetElement& element = iterator.value();

    // enter into the index
    //
    UniqueIdentification key = element.identification();
    index->insert(position,
                  element.localKey(),
                  element.referenceCount(),
                  &key);

    // save the object
    //
    element.save();

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
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::close(void)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::close");

  SetIterator iterator(_set, OMBefore);
  while (++iterator) {
    SetElement& element = iterator.value();
    element.close();
  }
}

  // @mfunc Detach this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::detach(void)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::detach");

  SetIterator iterator(_set, OMBefore);
  while (++iterator) {
    SetElement& element = iterator.value();
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
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::restore(size_t externalSize)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::restore");

  PRECONDITION("Consistent property size", externalSize == strlen(name()) + 1);

  // get the name of the set index stream
  //
  char* propertyName = new char[externalSize];
  ASSERT("Valid heap pointer", propertyName != 0);
  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  store->read(_propertyId, _storedForm, propertyName, externalSize);
  ASSERT("Consistent property name", strcmp(propertyName, name()) == 0);
  delete [] propertyName;

  // restore the index
  //
  OMStoredSetIndex* setIndex = 0;
  store->restore(setIndex, name());
  ASSERT("Valid set index", setIndex->isValid());
  ASSERT("Consistent key sizes",
                          setIndex->keySize() == sizeof(UniqueIdentification));
  ASSERT("Consistent key property ids",
                                  setIndex->keyPropertyId() == _keyPropertyId);
  setLocalKey(setIndex->firstFreeKey());

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
  UniqueIdentification key;
  for (size_t i = 0; i < entries; i++) {
    setIndex->iterate(context, localKey, count, &key);
    char* name = elementName(localKey);
    SetElement newElement(this, name, localKey, count, key);
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
template <typename UniqueIdentification, typename ReferencedObject>
size_t
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::count(void) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::count");

  return _set.count();
}

  // @mfunc Get the size of this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @rdesc The size of this <c OMStrongReferenceSetProperty>.
  //     @this const
template <typename UniqueIdentification, typename ReferencedObject>
size_t
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::getSize(void) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::getSize");
  OBSOLETE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                        "ReferencedObject>::count");

  return count();
}

  // @mfunc Insert <p object> into this
  //        <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to insert.
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::insert");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is not present", !containsValue(object));

  // Set the set to contain the new object
  //
  const size_t localKey = nextLocalKey();
  char* name = elementName(localKey);
  UniqueIdentification key = object->identification();
  ASSERT("Valid identification", isValidIdentification(key));

  SetElement newElement(this, name, localKey, 1/*tjb*/, key);
  newElement.setValue(object);
  _set.insert(newElement);
  setPresent();
  delete [] name;

  POSTCONDITION("Object is present", containsValue(object));
  //POSTCONDITION("Optional property is present", isPresent());
}

  // @mfunc If it is not already present, insert <p object> into this
  //        <c OMStrongReferenceSetProperty> and return true,
  //        otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to insert.
  //   @rdesc True if the object was inserted, false if it was already present.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::ensurePresent(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::ensurePresent");

  PRECONDITION("Valid object", object != 0);

  // tjb - Current cost is 2 * O(lg N) this should be halved to
  //       O(lg N) by implementing an ensurePresent() on OMSet.
  bool present = containsValue(object);
  if (!present) {
    insert(object);
  }
  POSTCONDITION("Object is present", containsValue(object));
  return !present;
}

  // @mfunc Append the given <p ReferencedObject> <p object> to
  //        this <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::appendValue");
  PRECONDITION("Valid object", object != 0);

  OBSOLETE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                        "ReferencedObject>::insert");
  insert(object);
  POSTCONDITION("Object is present", containsValue(object));
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
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::remove(
                                    const UniqueIdentification& identification)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::remove");

  PRECONDITION("Object is present", contains(identification));

  SetElement* element = 0;
  bool found = _set.find(identification, &element);
  ASSERT("Object found", found);
  ReferencedObject* result = element->setValue(0);
  _set.remove(identification);

  POSTCONDITION("Object is not present", !contains(identification));

  return result;
}

  // @mfunc If it is present, remove the <p ReferencedObject> identified by
  //        <p identification> from this <c OMStrongReferenceSetProperty>
  //        and return true, otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to remove.
  //   @rdesc True if the object was removed, false if it was already absent.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::ensureAbsent(
                                    const UniqueIdentification& identification)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::ensureAbsent");

  SetElement* element = 0;
  bool result = _set.find(identification, &element);
  if (result) {
    element->setValue(0);
    _set.remove(identification);
  }

  POSTCONDITION("Object is not present", !contains(identification));
  return result;
}

  // @mfunc Remove <p object> from this
  //        <c OMStrongReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  UniqueIdentification identification = object->identification();
  remove(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
}

  // @mfunc If it is present, remove <p object> from this
  //        <c OMStrongReferenceSetProperty> and return true,
  //        otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to remove.
  //   @rdesc True if the object was removed, false if it was already absent.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::ensureAbsent(
                                                const ReferencedObject* object)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::ensureAbsent");

  PRECONDITION("Valid object", object != 0);

  UniqueIdentification identification = object->identification();
  bool result = ensureAbsent(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
  return result;
}

  // @mfunc Does this <c OMStrongReferenceSetProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::containsValue");
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
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::contains(
                              const UniqueIdentification& identification) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::contains");

  return _set.contains(identification);
}

  // @mfunc The <p ReferencedObject> in this
  //        <c OMStrongReferenceSetProperty> identified by
  //        <p identification>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The unique identification of the desired object, the search key.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::value(
                              const UniqueIdentification& identification) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::value");

  PRECONDITION("Object is present", contains(identification));

  SetElement* element = 0;

  _set.find(identification, &element);
  ReferencedObject* result = element->getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
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
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::find(
                                    const UniqueIdentification& identification,
                                    ReferencedObject*& object) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::find");

  SetElement* element = 0;

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
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::isVoid(void) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::isVoid");

  bool result = true;

  SetIterator iterator(_set, OMBefore);
  while (++iterator) {
    SetElement& element = iterator.value();
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
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::remove(void)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::remove");

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
template <typename UniqueIdentification, typename ReferencedObject>
size_t
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::bitsSize(void) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::bitsSize");

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
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::getBits(OMByte* bits,
                                                        size_t size) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::getBits");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject** p = (const ReferencedObject**)bits;

  SetIterator iterator(_set, OMBefore);
  while (++iterator) {
    SetElement& element = iterator.value();
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
template <typename UniqueIdentification, typename ReferencedObject>
void
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::setBits(const OMByte* bits,
                                                        size_t size)
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::setBits");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  size_t count = size / sizeof(ReferencedObject*);
  ReferencedObject** p = (ReferencedObject**)bits;

  for (size_t i = 0; i < count; i++) {
    ReferencedObject* object = p[i];
    insert(object);
  }

}

template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetProperty<UniqueIdentification,
                             ReferencedObject>::isValidIdentification(
                                                UniqueIdentification& id) const
{
  TRACE("OMStrongReferenceSetProperty<UniqueIdentification, "
                                     "ReferencedObject>::"
                                                      "isValidIdentification");

  bool result = false;
  OMByte* p = reinterpret_cast<OMByte*>(&id);
  for (size_t i = 0; i < sizeof(UniqueIdentification); i++) {
    if (p[i] != 0) {
      result = true;
      break;
    }
  }
  return result;
}

#endif

