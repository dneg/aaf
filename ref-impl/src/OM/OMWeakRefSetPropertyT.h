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
#ifndef OMWEAKREFSETPROPERTYT_H
#define OMWEAKREFSETPROPERTYT_H

#include "OMAssertions.h"
#include "OMWeakReferenceSetIter.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceSetProperty>.
  //   @parm The name of the the <c OMProperty> instance (a set property)
  //         in which the objects referenced by the elements of this
  //         <c OMWeakReferenceSetProperty> reside.
template <typename ReferencedObject>
OMWeakReferenceSetProperty<ReferencedObject>::
                   OMWeakReferenceSetProperty(const OMPropertyId propertyId,
                                              const wchar_t* name,
                                              const wchar_t* targetName,
                                              const OMPropertyId keyPropertyId)
: OMContainerProperty<ReferencedObject>(propertyId,
                                        SF_WEAK_OBJECT_REFERENCE_SET,
                                        name),
  _targetTag(nullOMPropertyTag),
  _targetName(targetName),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::"
                                                 "OMWeakReferenceSetProperty");
}

  // @mfunc Destructor.
template <typename ReferencedObject>
OMWeakReferenceSetProperty<ReferencedObject>::~OMWeakReferenceSetProperty(void)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::"
                                                "~OMWeakReferenceSetProperty");

  delete [] _targetPropertyPath;
}

  // @mfunc Save this <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::save");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  OMPropertyTag tag = targetTag();

  // create a set index
  //
  size_t count = _set.count();
  OMUniqueObjectIdentification* index = 0;
  if (count > 0) {
    index = new OMUniqueObjectIdentification[count];
    ASSERT("Valid heap pointer", index != 0);
  }
  size_t position = 0;

  // Iterate over the set saving each element. The index entries
  // are written in order of their unique keys.
  //
  SetIterator iterator(_set, OMBefore);
  while (++iterator) {

    SetElement& element = iterator.value();

    // enter into the index
    //
    index[position] = element.identification();

    // save the object
    //
    element.save();

    position = position + 1;

  }

  // save the set index
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

  clearTargetTag();
}

  // @mfunc Close this <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::close(void)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::close");

  SetIterator iterator(_set, OMBefore);
  while (++iterator) {
    SetElement& element = iterator.value();
    element.close();
  }
}

  // @mfunc Detach this <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::detach(void)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::detach");

  SetIterator iterator(_set, OMBefore);
  while (++iterator) {
    SetElement& element = iterator.value();
    element.detach();
  }
}

  // @mfunc Restore this <c OMWeakReferenceSetProperty>, the external
  //        (persisted) size of the <c OMWeakReferenceSetProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The external (persisted) size of the
  //         <c OMWeakReferenceSetProperty>.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::restore(
                                                           size_t externalSize)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::restore");

  // get the name of the set index stream
  //
  restoreName(externalSize);

  // restore the index
  //
  OMUniqueObjectIdentification* setIndex = 0;
  size_t entries;
  OMPropertyTag tag;
  OMPropertyId keyPropertyId;
  store()->restore(storedName(),
                   setIndex,
                   entries,
                   tag,
                   keyPropertyId);

  ASSERT("Valid set index", IMPLIES(entries != 0, setIndex != 0));
  ASSERT("Valid set index", IMPLIES(entries == 0, setIndex == 0));
  ASSERT("Consistent key property ids", keyPropertyId == _keyPropertyId);
  _targetTag = tag;

  // Iterate over the index restoring the elements of the set.
  // Since the index entries are stored on disk in order of their
  // unique keys this loop is the worst cast order of insertion. This
  // code will eventually be replaced by code that inserts the keys in
  // "binary search" order. That is the middle key is inserted first
  // then (recursively) all the keys below the middle key followed by
  // (recursively) all the keys above the middle key.
  //
  for (size_t i = 0; i < entries; i++) {
    OMUniqueObjectIdentification key = setIndex[i];
    SetElement newElement(this, key, _targetTag);
    newElement.restore();
    _set.insert(newElement);
   }
  delete [] setIndex;
  setPresent();
}

  // @mfunc The number of <p ReferencedObject>s in this
  //        <c OMWeakReferenceSetProperty>.
  //   @this const
template <typename ReferencedObject>
size_t OMWeakReferenceSetProperty<ReferencedObject>::count(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::count");

  return _set.count();
}

  // @mfunc Get the size of this <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //     @rdesc The size of this <c OMWeakReferenceSetProperty>.
  //     @this const
template <typename ReferencedObject>
size_t OMWeakReferenceSetProperty<ReferencedObject>::getSize(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::getSize");
  OBSOLETE("OMWeakReferenceSetProperty<ReferencedObject>::count");

  return count();
}

  // @mfunc Insert <p object> into this
  //        <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to insert.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::insert(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::insert");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is not present", !containsValue(object));
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());
#endif
  // Set the set to contain the new object
  //
  OMUniqueObjectIdentification key = object->identification();
  SetElement newElement(this, key, _targetTag);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  newElement.reference().setTargetTag(targetTag());
#endif
  newElement.setValue(object);
  _set.insert(newElement);
  setPresent();

  POSTCONDITION("Object is present", containsValue(object));
  //POSTCONDITION("Optional property is present", isPresent());
}

  // @mfunc If it is not already present, insert <p object> into this
  //        <c OMWeakReferenceSetProperty> and return true,
  //        otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to insert.
  //   @rdesc True if the object was inserted, false if it was already present.
template <typename ReferencedObject>
bool OMWeakReferenceSetProperty<ReferencedObject>::ensurePresent(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::ensurePresent");

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
  //        this <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::appendValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::appendValue");
  PRECONDITION("Valid object", object != 0);

  OBSOLETE("OMWeakReferenceSetProperty<ReferencedObject>::insert");
  insert(object);
  POSTCONDITION("Object is present", containsValue(object));
}

  // @mfunc Remove the <p ReferencedObject> identified by
  //        <p identification> from this <c OMWeakReferenceSetProperty>.
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
OMWeakReferenceSetProperty<ReferencedObject>::remove(
                            const OMUniqueObjectIdentification& identification)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::remove");

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
  //        <p identification> from this <c OMWeakReferenceSetProperty>
  //        and return true, otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to remove.
  //   @rdesc True if the object was removed, false if it was already absent.
template <typename ReferencedObject>
bool OMWeakReferenceSetProperty<ReferencedObject>::ensureAbsent(
                            const OMUniqueObjectIdentification& identification)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::ensureAbsent");

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
  //        <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::removeValue(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::removeValue");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Object is present", containsValue(object));

  OMUniqueObjectIdentification identification = object->identification();
  remove(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
}

  // @mfunc If it is present, remove <p object> from this
  //        <c OMWeakReferenceSetProperty> and return true,
  //        otherwise return false.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The object to remove.
  //   @rdesc True if the object was removed, false if it was already absent.
template <typename ReferencedObject>
bool OMWeakReferenceSetProperty<ReferencedObject>::ensureAbsent(
                                                const ReferencedObject* object)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::ensureAbsent");

  PRECONDITION("Valid object", object != 0);

  OMUniqueObjectIdentification identification = object->identification();
  bool result = ensureAbsent(identification);

  POSTCONDITION("Object is not present", !containsValue(object));
  return result;
}

  // @mfunc Does this <c OMWeakReferenceSetProperty> contain
  //        <p object> ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
bool OMWeakReferenceSetProperty<ReferencedObject>::containsValue(
                                          const ReferencedObject* object) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::containsValue");
  PRECONDITION("Valid object", object != 0);

  bool result = _set.contains(object->identification());
  return result;
}

  // @mfunc Does this <c OMWeakReferenceSetProperty> contain a
  //        <p ReferencedObject> identified by <p identification>?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The unique identification of the desired object, the search key.
  //   @rdesc True if the object is found, false otherwise.
template <typename ReferencedObject>
bool OMWeakReferenceSetProperty<ReferencedObject>::contains(
                      const OMUniqueObjectIdentification& identification) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::contains");

  return _set.contains(identification);
}

  // @mfunc The <p ReferencedObject> in this
  //        <c OMWeakReferenceSetProperty> identified by
  //        <p identification>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The unique identification of the desired object, the search key.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceSetProperty<ReferencedObject>::value(
                     const OMUniqueObjectIdentification& identification) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::value");

  PRECONDITION("Object is present", contains(identification));

  SetElement* element = 0;

  _set.find(identification, &element);
  ReferencedObject* result = element->getValue();

  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc Find the <p ReferencedObject> in this
  //        <c OMWeakReferenceSetProperty> identified by
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
bool OMWeakReferenceSetProperty<ReferencedObject>::find(
                            const OMUniqueObjectIdentification& identification,
                            ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::find");

  SetElement* element = 0;

  bool result = _set.find(identification, &element);
  if (result) {
    object = element->getValue();
  }

  return result;
}

  // @mfunc Is this <c OMWeakReferenceSetProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @rdesc True if this <c OMWeakReferenceSetProperty> is void,
  //          false otherwise. 
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceSetProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::isVoid");

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

  // @mfunc Remove this optional <c OMWeakReferenceSetProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
template <typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::remove(void)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::remove");

  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}
  // @mfunc The size of the raw bits of this
  //        <c OMWeakReferenceSetProperty>. The size is given in bytes.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @rdesc The size of the raw bits of this
  //          <c OMWeakReferenceSetProperty> in bytes.
  //   @this const
template<typename ReferencedObject>
size_t OMWeakReferenceSetProperty<ReferencedObject>::bitsSize(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::bitsSize");

  return sizeof(ReferencedObject*) * count();
}

  // @mfunc Get the raw bits of this <c OMWeakReferenceSetProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
  //   @this const
template<typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::getBits(OMByte* bits,
                                                             size_t size) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::getBits");

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
  //        <c OMWeakReferenceSetProperty>. The raw bits are
  //        copied from the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and <c OMUnique>.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
template<typename ReferencedObject>
void OMWeakReferenceSetProperty<ReferencedObject>::setBits(const OMByte* bits,
                                                           size_t size)
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::setBits");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  size_t count = size / sizeof(ReferencedObject*);
  ReferencedObject** p = (ReferencedObject**)bits;

  for (size_t i = 0; i < count; i++) {
    ReferencedObject* object = p[i];
    insert(object);
  }

}

template<typename ReferencedObject>
OMPropertyTag
OMWeakReferenceSetProperty<ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceSetProperty<ReferencedObject>* nonConstThis =
               const_cast<OMWeakReferenceSetProperty<ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                  file()->referencedProperties()->insert(targetPropertyPath());
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

template<typename ReferencedObject>
OMPropertyId*
OMWeakReferenceSetProperty<ReferencedObject>::targetPropertyPath(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::targetPropertyPath");

  PRECONDITION("Valid target name", validWideString(_targetName));

  if (_targetPropertyPath == 0) {
    OMWeakReferenceSetProperty<ReferencedObject>* nonConstThis =
               const_cast<OMWeakReferenceSetProperty<ReferencedObject>*>(this);
    nonConstThis->_targetPropertyPath = file()->path(_targetName);
  }
  POSTCONDITION("Valid result", _targetPropertyPath != 0);
  return _targetPropertyPath;
}

template<typename ReferencedObject>
void
OMWeakReferenceSetProperty<ReferencedObject>::clearTargetTag(void) const
{
  TRACE("OMWeakReferenceSetProperty<ReferencedObject>::clearTargetTag");

  OMWeakReferenceSetProperty<ReferencedObject>* nonConstThis =
               const_cast<OMWeakReferenceSetProperty<ReferencedObject>*>(this);

  nonConstThis->_targetTag = nullOMPropertyTag;
  delete [] nonConstThis->_targetPropertyPath;
  nonConstThis->_targetPropertyPath = 0;
}

#endif

