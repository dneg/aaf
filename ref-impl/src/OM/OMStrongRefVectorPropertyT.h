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
#include "OMFile.h"

#define CHUNKSIZE (10)

template <typename ReferencedObject>
OMStrongReferenceVectorProperty<ReferencedObject>::
                 OMStrongReferenceVectorProperty(const OMPropertyId propertyId,
                                                 const char* name)
: OMContainerProperty(propertyId, SF_STRONG_OBJECT_REFERENCE_VECTOR, name),
  _vector(0), _sizeOfVector(CHUNKSIZE), _size(0), _loaded(0), _keys(0)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::"
                                            "OMStrongReferenceVectorProperty");
  PRECONDITION("Valid size", _sizeOfVector >= 0);

  _vector = new const ReferencedObject*[_sizeOfVector];
  ASSERT("Valid heap pointer", _vector != 0);

  _loaded = new bool[_sizeOfVector];
  ASSERT("Valid heap pointer", _loaded != 0);

  _keys = new OMUInt32[_sizeOfVector];
  ASSERT("Valid heap pointer", _keys != 0);

  for (size_t i = 0; i < _sizeOfVector; i++) {
    _vector[i] = 0;
    _loaded[i] = true;
    _keys[i] = 0;
  }
}

template <typename ReferencedObject>
OMStrongReferenceVectorProperty<ReferencedObject>::
                                         ~OMStrongReferenceVectorProperty(void)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::"
                                           "~OMStrongReferenceVectorProperty");
  for (size_t i = 0; i < _size; i++) {
    if (_vector[i] != 0) {
      ReferencedObject* oldObject = const_cast<ReferencedObject*>(_vector[i]);
      oldObject->clearContainingObject();
      oldObject->clearContainingProperty();
      _vector[i] = 0;
    }
  }
  delete [] _vector;
  _vector = 0;
  delete [] _loaded;
  _loaded = 0;
  delete [] _keys;
  _keys = 0;
  delete [] _propertyName;
  _propertyName = 0;
}

  // @mfunc Save this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm  The <c OMStoredObject> on which to save this
  //          <c OMStrongReferenceVectorProperty>.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::save(void) const
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
  OMStoredVectorIndex* index = new OMStoredVectorIndex(_size);
  ASSERT("Valid heap pointer", index != 0);
  index->setHighWaterMark(_size);
  size_t current = 0;
  size_t position = 0;

  // Iterate over the vector saving each element
  //
  for (size_t i = 0; i < _size; i++) {

    // enter into the index
    //
    index->insert(position, current);

    // save the object
    //
    if (isElementLoaded(i)) {

      // Either the object does not exist in the file or the object
      // exists in the file and has been loaded. In either case the
      // object is saved. If the object has been loaded, we assume, in
      // the absence of a dirty bit, that it has been changed.

      ASSERT("Non-void strong reference", _vector[i] != 0);

      _vector[i]->save();
    }

    current = current + 1;
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
  for (size_t i = 0; i < _size; i++) {
    if (_vector[i] != 0) {
      const_cast<ReferencedObject*>(_vector[i])->close();
    }
    setElementLoaded(i);
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
  OMFile::OMLoadMode loadMode = _propertySet->container()->file()->loadMode();

  // get the name of the vector index stream
  //
  delete [] _propertyName;
  _propertyName = new char[externalSize];
  ASSERT("Valid heap pointer", _propertyName != 0);
  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  store->read(_propertyId, _storedForm, _propertyName, externalSize);
  ASSERT("Consistent property size",
                                    externalSize == strlen(_propertyName) + 1);
  ASSERT("Consistent property name", strcmp(_propertyName, name()) == 0);

  // restore the index
  //
  OMStoredVectorIndex* vectorIndex = 0;
  store->restore(vectorIndex, _propertyName);
  ASSERT("Valid vector index", vectorIndex->isValid());

  // Iterate over the index restoring the elements of the vector
  //
  size_t entries = vectorIndex->entries();
  size_t context = 0;
  OMUInt32 key;
  for (size_t i = 0; i < entries; i++) {
    vectorIndex->iterate(context, key);
    setElementKey(i, key);
    ASSERT("Correct initial loaded state", isElementLoaded(i));
    ASSERT("Reference not already set", _vector[i] == 0);
    clearElementLoaded(i);
    if (loadMode == OMFile::eagerLoad) {
      loadElement(i);
    }
  }
  delete vectorIndex;
  setPresent();
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

  size = _size;
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

  return _size;
}

  // @mfunc Set the value of the <p ReferencedObject> at
  //        position <p index> in this
  //        <c OMStrongReferenceVectorProperty>.
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
                                                const ReferencedObject* value,
                                                const size_t index)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::setValueAt");
  PRECONDITION("Valid index", (index >= 0) && (index <= _size));
  
  if (index == _size) {
    // this is an append
    if (_size == _sizeOfVector) {
      // this append requires us to grow by the chunk size
      grow(CHUNKSIZE);
    }
    _size = _size + 1;
  }

  // Orphan the old object
  //
  ReferencedObject* oldObject = const_cast<ReferencedObject*>(_vector[index]);
  if (oldObject != 0) {
    oldObject->clearContainingObject();
    oldObject->clearContainingProperty();
  }

  // Set the vector to contain the new object
  //
  _vector[index] = value;

  // Adopt the new object
  //
  ReferencedObject* newObject = const_cast<ReferencedObject*>(_vector[index]);
  if (newObject != 0) {
    newObject->setContainingObject(_propertySet->container());
    char* objectName = elementName(name(), index);
    newObject->setName(objectName);
    delete [] objectName;
    objectName = 0;

    newObject->setContainingProperty(this);
  }
  setPresent();
  setElementLoaded(index);
  POSTCONDITION("Element properly inserted", _vector[index] == value);
  return oldObject;
}

  // @mfunc Get the value of the <p ReferencedObject> at
  //        position <p index> in this
  //        <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @parm The position from which to get the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::getValueAt(
                                                      ReferencedObject*& value,
                                                      const size_t index) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::getValueAt");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid index", ((index >= 0) && (index < _size)));

  if (!isElementLoaded(index)) {
    OMStrongReferenceVectorProperty<ReferencedObject>* nonConstThis =
          const_cast<OMStrongReferenceVectorProperty<ReferencedObject>*>(this);
    nonConstThis->loadElement(index);
  }
  ASSERT("Loaded", isElementLoaded(index));

  value = const_cast<ReferencedObject*>(_vector[index]);
}

  // @mfunc Append the given <p ReferencedObject> <p value> to
  //        this <c OMStrongReferenceVectorProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::appendValue(
                                                const ReferencedObject*& value)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::appendValue");
  PRECONDITION("Reasonable size", _size <= _sizeOfVector);

  setValueAt(value, _size);

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
  FORALL(i, _sizeOfVector,
    PRECONDITION("Property is void", _vector[i] == 0));
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::grow(
                                                     size_t additionalElements)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::grow");
  PRECONDITION("Valid size increment", additionalElements > 0);
  PRECONDITION("Vector is full", _sizeOfVector == _size);

  // Save size and old array
  //
  size_t oldSizeOfVector = _sizeOfVector;
  const ReferencedObject** oldVector = _vector;
  bool* oldLoaded = _loaded;
  OMUInt32* oldKeys = _keys;
  
  // New size
  //
  _sizeOfVector = _sizeOfVector + additionalElements;
  
  // Allocate new array
  //
  _vector = new const ReferencedObject*[_sizeOfVector];
  ASSERT("Valid heap pointer", _vector != 0);

  _loaded = new bool[_sizeOfVector];
  ASSERT("Valid heap pointer", _loaded != 0);

  _keys = new OMUInt32[_sizeOfVector];
  ASSERT("Valid heap pointer", _keys != 0);

  // Copy over all elements from the old array
  //
  for (size_t i = 0; i < oldSizeOfVector; i++) {
    _vector[i] = oldVector[i];
    _loaded[i] = oldLoaded[i];
    _keys[i] = oldKeys[i];
  }

  // Initialize the new elements
  //
  for (size_t j = oldSizeOfVector; j < _sizeOfVector; j++) {
    _vector[j] = 0;
    _loaded[j] = true;
    _keys[j] = 0;
  }

  // Delete the old array
  //
  delete [] oldVector;
  delete [] oldLoaded;
  delete [] oldKeys;

  POSTCONDITION("Size properly increased",
                _sizeOfVector == oldSizeOfVector + additionalElements);

}

  // @mfunc Load the persisted representation of the element of
  //        this <c OMStrongReferenceVectorProperty> given by <p index>
  //        into memory.
  //   @parm The index of the element to load.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::loadElement(
                                                            const size_t index)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::loadElement");
  PRECONDITION("Valid index", (index >= 0) && (index <= _size));
  PRECONDITION("Not already loaded", !isElementLoaded(index));
  PRECONDITION("Valid storage name", validString(_propertyName));

  //   compute the storage name for this element
  //
  char* storageName = elementName(_propertyName, elementKey(index));

  // open the sub-storage
  //
  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  OMStoredObject* subStorage = store->open(storageName);

  // restore contents from the sub-storage
  //
  OMStorable* object = OMStorable::restoreFrom(_propertySet->container(),
                                               storageName,
                                               *subStorage);
  ReferencedObject* referencedObject = dynamic_cast<ReferencedObject*>(object);

  delete [] storageName;
  
  // place a pointer to the newly restored object in the vector
  //
  setValueAt(referencedObject, index);

  setElementLoaded(index);
  POSTCONDITION("Property properly loaded", isElementLoaded(index));
}

  // @mfunc Is the persisted representation of the element of this
  //        <c OMStrongReferenceVectorProperty> given by <p index>
  //        loaded ?
  //   @parm The index of the element.
  //   @rdesc False if the element at <p index> has a persisted
  //          representation exists that has not yet been loaded, true
  //          otherwise.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @this const
template <typename ReferencedObject>
bool OMStrongReferenceVectorProperty<ReferencedObject>::isElementLoaded(
                                                      const size_t index) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::isElementLoaded");
  PRECONDITION("Valid index", (index >= 0) && (index < _size));
  return _loaded[index];
}

  // @mfunc  Set the bit that indicates that the persisted
  //         representation of the element of this
  //         <c OMStrongReferenceVectorProperty> given by <p index> is loaded.
  //   @parm The index of the element.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template<typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::setElementLoaded(
                                                            const size_t index)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::setElementLoaded");
  PRECONDITION("Valid index", (index >= 0) && (index <= _size));
  
  if (index == _size) {
    // this is an append
    if (_size == _sizeOfVector) {
      // this append requires us to grow by the chunk size
      grow(CHUNKSIZE);
    }
    _size = _size + 1;
  }
  _loaded[index] = true;
}

  // @mfunc Clear the bit that indicates that the persisted
  //        representation of the element of this
  //        <c OMStrongReferenceVectorProperty> given by <p index> is loaded.
  //   @parm the index of the element.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template<typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::clearElementLoaded(
                                                            const size_t index)
{
  TRACE(
      "OMStrongReferenceVectorProperty<ReferencedObject>::clearElementLoaded");
  PRECONDITION("Valid index", (index >= 0) && (index <= _size));
  
  if (index == _size) {
    // this is an append
    if (_size == _sizeOfVector) {
      // this append requires us to grow by the chunk size
      grow(CHUNKSIZE);
    }
    _size = _size + 1;
  }
  _loaded[index] = false;
}

  // @mfunc The key of the element of this
  //        <c OMStrongReferenceVectorProperty> given by <p index>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The index of the element.
  //   @rdesc The key of the element at <p index>.
  //   @this const
template<typename ReferencedObject>
OMUInt32 OMStrongReferenceVectorProperty<ReferencedObject>::elementKey(
                                                      const size_t index) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::elementKey");
  PRECONDITION("Valid index", (index >= 0) && (index < _size));

  return _keys[index];
}

  // @mfunc Set the key of the element of this
  //        <c OMStrongReferenceVectorProperty> given by <p index>
  //        to <p key>.
  //   @parm The index of the element.
  //   @parm The new key.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template<typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::setElementKey(
                                                     const size_t index,
                                                     const OMUInt32 key)
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::setElementKey");
  PRECONDITION("Valid index", (index >= 0) && (index <= _size));

  if (index == _size) {
    // this is an append
    if (_size == _sizeOfVector) {
      // this append requires us to grow by the chunk size
      grow(CHUNKSIZE);
    }
    _size = _size + 1;
  }
  _keys[index] = key;
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

  return sizeof(ReferencedObject*) * _size;
}

  // @mfunc Get the raw bits of this <c OMStrongReferenceVectorProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
  //   @this const
template<typename ReferencedObject>
void OMStrongReferenceVectorProperty<ReferencedObject>::getBits(
                                                         OMByte* bits,
                                                         size_t size) const
{
  TRACE("OMStrongReferenceVectorProperty<ReferencedObject>::getBits");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  OMStrongReferenceVectorProperty<ReferencedObject>* nonConstThis =
          const_cast<OMStrongReferenceVectorProperty<ReferencedObject>*>(this);
  for (size_t i = 0; i < _size; i++) {
    if (!isElementLoaded(i)) {
      nonConstThis->loadElement(i);
    }
  }

  memcpy(bits, _vector, bitsSize());
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
