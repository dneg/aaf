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
#ifndef OMSTRONGREFPROPERTYT_H
#define OMSTRONGREFPROPERTYT_H

#include "OMAssertions.h"
#include "OMFile.h"

template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::OMStrongReferenceProperty(
                                                 const OMPropertyId propertyId,
                                                 const char* name)
: OMReferenceProperty<ReferencedObject>(propertyId,
                                        SF_STRONG_OBJECT_REFERENCE,
                                        name),
  _storageName(0)
{
  TRACE(
     "OMStrongReferenceProperty<ReferencedObject>::OMStrongReferenceProperty");
}

template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::~OMStrongReferenceProperty(void)
{
  TRACE(
    "OMStrongReferenceProperty<ReferencedObject>::~OMStrongReferenceProperty");
  if (_pointer != 0) {
    _pointer->clearContainingObject();
    _pointer->clearContainingProperty();
  }
  delete _storageName;
  _storageName = 0;
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

  if (!isLoaded()) {
    OMStrongReferenceProperty<ReferencedObject>* nonConstThis =
                const_cast<OMStrongReferenceProperty<ReferencedObject>*>(this);
    nonConstThis->load();
  }
  ASSERT("Loaded", isLoaded());

  object = _pointer;
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

  ReferencedObject* oldPointer = _pointer;

  // Orphan the old object
  //
  if (_pointer != 0) {
    _pointer->clearContainingObject();
    _pointer->clearContainingProperty();
  }

  // Set the reference to contain the new object
  //
  OMReferenceProperty<ReferencedObject>::setValue(object);

  // Adopt the new object
  //
  if (_pointer != 0) {
    _pointer->setContainingObject(_propertySet->container());
    _pointer->setName(name());
    _pointer->setContainingProperty(this);
  }
  setLoaded();
  setPresent();
  return oldPointer;
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
  return pointer();
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
  return pointer();
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

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());

  // Write the strong reference (and an associated index entry).
  //
  OMStoredObject* s = container->store();
  const char* propertyName = name();
  s->write(_propertyId,
           _storedForm,
           (void *)propertyName,
           strlen(propertyName) + 1);

  // save the object
  //
  if (isLoaded()) {

    // Either the object does not exist in the file or the object
    // exists in the file and has been loaded. In either case the
    // object is saved. If the object has been loaded, we assume, in
    // the absence of a dirty bit, that it has been changed.

    OMStorable* object = pointer();
    ASSERT("Non-void strong reference", object != 0);

    object->save();
  }
}

  // @mfunc Close this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::close(void)
{
  if (!isOptional() || isPresent()) {
    OMStorable* object = pointer();
    if (object != 0) {
      object->close();
    }
  }
  setLoaded();
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
  OMFile::OMLoadMode loadMode = _propertySet->container()->file()->loadMode();

  // retrieve sub-storage name
  //
  delete [] _storageName;
  _storageName = new char[externalSize];
  ASSERT("Valid heap pointer", _storageName != 0);

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  store->read(_propertyId, _storedForm, _storageName, externalSize);
  ASSERT("Consistent property size", externalSize == strlen(_storageName) + 1);
  ASSERT("Consistent property name", strcmp(_storageName, name()) == 0);

  ASSERT("Correct initial loaded state", isLoaded());
  ASSERT("Reference not already set", _pointer == 0);
  clearLoaded();
  if (loadMode == OMFile::eagerLoad) {
    load();
  }
  setPresent();

  POSTCONDITION("Consistent load mode",
                           IMPLIES(loadMode == OMFile::eagerLoad, isLoaded()));
  POSTCONDITION("consistent load mode",
                           IMPLIES(loadMode == OMFile::lazyLoad, !isLoaded()));
}

  // @mfunc Remove this optional <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template<typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::remove(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::remove");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  PRECONDITION("Property is void", _pointer == 0);
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc Load the persisted representation of this
  //        <c OMStrongReferenceProperty> into memory.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template<typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::load(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::load");
  PRECONDITION("Not already loaded", !isLoaded());
  PRECONDITION("Valid storage name", validString(_storageName));

  // open the sub-storage
  //
  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  OMStoredObject* subStorage = store->open(_storageName);

  // restore contents from the sub-storage
  //
  OMStorable* object = OMStorable::restoreFrom(_propertySet->container(),
                                               name(),
                                               *subStorage);
  ReferencedObject* referencedObject = dynamic_cast<ReferencedObject*>(object);

  // set the value of this property to point to the newly created object
  //
  setValue(referencedObject);

  setLoaded();

  POSTCONDITION("Property properly loaded", isLoaded());
}

#endif
