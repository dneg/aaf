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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStorable

#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMFile.h"
#include "OMClassFactory.h"
#include "OMDataTypes.h"
#include "OMProperty.h"
#include "OMPropertySetIterator.h"
#include "OMUtilities.h"

#include "OMAssertions.h"

#include <string.h>

OMStorable::OMStorable(void)
: _persistentProperties(),
  _container(0),
  _name(0),
  _store(0),
  _exists(false),
  _classFactory(0),
  _definition(0)
{
  TRACE("OMStorable::OMStorable");
  _persistentProperties.setContainer(this);
}

OMStorable::~OMStorable(void)
{
  TRACE("OMStorable::~OMStorable");

  // This assertion will trigger if an attempt is made to delete an
  // object that is still attached. That is, the assertion detects an
  // attempt to create a dangling strong reference.
  //
  PRECONDITION("Object not attached", !attached());

  delete [] _name;
  _name = 0;
}

  // @mfunc Set the <c OMClassDefinition> defining this <c OMStorable>.
  //   @parm TBS
void OMStorable::setDefinition(const OMClassDefinition* definition)
{
  TRACE("OMStorable::setDefinition");
  PRECONDITION("Valid definition", definition != 0);
  PRECONDITION("No previous definition", _definition == 0);
  _definition = definition;
}

  // @mfunc The <c OMClassDefinition> defining this <c OMStorable>.
  //   @rdesc TBS
const OMClassDefinition* OMStorable::definition(void) const
{
  TRACE("OMStorable::definition");
  const OMClassDefinition* result = _definition;
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
  POSTCONDITION("Valid result", result != 0);
#endif
  return result;
}

  // @mfunc Save this <c OMStorable>.
  //   @this const
void OMStorable::save(void) const
{
  TRACE("OMStorable::save");
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
  PRECONDITION("Valid class definition", definition() != 0);
#endif

  bool opened = false;
  if (_store == 0 ) {
    opened = true;
  }

  OMStorable* nonConstThis = const_cast<OMStorable*>(this);
  store()->save(*nonConstThis);

  // Temporary brute force solution to the Microsoft Structured
  // Storage built in limit on the number of open storage elements
  // (IStorages and IStreams) caused by use of a fixed size internal
  // heap.
  // We take care to close _store here only if it was opened above.
  // We don't want to close any IStreams (or their enclosing IStorages)
  // that were opened on demand as closing them would lose important
  // state information, such as the current seek position.
  //
  if (opened) {
    ASSERT("Valid store", _store != 0);
    _store->close();
    delete _store;
    nonConstThis->_store = 0;
  }
  nonConstThis->_exists = true;
}

  // @mfunc Close this <c OMStorable>.
void OMStorable::close(void)
{
  TRACE("OMStorable::close");
  PRECONDITION("Object is persistent", persistent());

  if (_store != 0) {
    OMPropertySetIterator iterator(_persistentProperties, OMBefore);
    while (++iterator) {
      OMProperty* p = iterator.property();
      ASSERT("Valid property", p != 0);
      if (!p->isOptional() || p->isPresent()) {
        p->close();
      }
    }

    _store->close();
    delete _store;
    _store = 0;
  } // else silently ignore unsaved object
  _exists = false;

  POSTCONDITION("Closed", _store == 0);
}

  // @mfunc Restore the contents of an <c OMStorable> (of unknown
  //        sub-class).
void OMStorable::restoreContents(void)
{
  TRACE("OMStorable::restoreContents");

  store()->restore(_persistentProperties);

  // Temporary brute force solution to the Microsoft Structured
  // Storage built in limit on the number of open storage elements
  // (IStorages and IStreams) caused by use of a fixed size internal
  // heap.
  //
  ASSERT("Valid store", _store != 0);
  _store->close();
  delete _store;
  _store = 0;
  _exists = true;
}

  // @mfunc Attach this <c OMStorable>.
  //   @parm The containining <c OMStorable>.
  //   @parm The name to be given to this <c OMStorable>.
void OMStorable::attach(const OMStorable* container, const wchar_t* name)
{
  TRACE("OMStorable::attach");
  // tjb PRECONDITION("Not attached", !attached());
  PRECONDITION("Valid container", container != 0);
  PRECONDITION("Valid container", container != this);
  PRECONDITION("Valid name", validWideString(name));

  _container = container;
  setName(name);

  POSTCONDITION("Attached", attached());
}

  // @mfunc Detach this <c OMStorable>.
void OMStorable::detach(void)
{
  TRACE("OMStorable::detach");

  if (_store != 0) {
    OMPropertySetIterator iterator(_persistentProperties, OMBefore);
    while (++iterator) {
      OMProperty* p = iterator.property();
      ASSERT("Valid property", p != 0);
      p->detach();
    }

    // Once incremental saving (dirty bit) is implemented we'll need
    // to deal with any persisted representation of this unattached
    // OMStorable.

    _store->close();
    delete _store;
    _store = 0;
  }

  _container = 0;

  delete [] _name;
  _name = 0;
  _exists = false;

  PRECONDITION("Detached", !attached());
}

  // @mfunc The name of this <c OMStorable>.
  //   @rdesc The name of this <c OMStorable>.
  //   @this const
const wchar_t* OMStorable::name(void) const
{
  TRACE("OMStorable::name");
  return _name;
}

  // @mfunc Give this <c OMStorable> a name.
  //   @parm The name to be given to this <c OMStorable>.
void OMStorable::setName(const wchar_t* name)
{
  TRACE("OMStorable::setName");
  PRECONDITION("Valid name", validWideString(name));
  delete [] _name;
  _name = 0; // for BoundsChecker
  _name = saveWideString(name);
}

  // @mfunc The <c OMFile> in which this <c OMStorable> has a
  //        persistent representation.
  //   @rdesc The <c OMFile> in which this <c OMStorable> has a
  //          persistent representation.
  //   @this const
OMFile* OMStorable::file(void) const
{
  TRACE("OMStorable::file");
  PRECONDITION("Valid containing object", container() != 0);
  return container()->file();
}

  // @mfunc Find the <c OMStorable> named <p objectName> contained
  //        within this <c OMStorable>.
  //   @parm The name of the object.
  //   @rdesc The object.
  //   @this const
OMStorable* OMStorable::find(const wchar_t* objectName) const
{
  TRACE("OMStorable::find");

  OMProperty* p = findProperty(objectName);
  ASSERT("Valid property", p != 0);
  OMStorable* result = p->storable();
  ASSERT("Valid object", result != 0);
  return result;
}

OMStorable* OMStorable::find(OMPropertyId propertyId) const
{
  TRACE("OMStorable::find");

  OMProperty* p = findProperty(propertyId);
  ASSERT("Valid property", p != 0);
  OMStorable* result = p->storable();
  ASSERT("Valid object", result != 0);
  return result;
}

  // @mfunc Find the <c OMProperty> named <p propertyName> contained
  //        within this <c OMStorable>.
  //   @parm The name of the property.
  //   @rdesc The property.
  //   @this const
OMProperty* OMStorable::findProperty(const wchar_t* propertyName) const
{
  TRACE("OMStorable::findProperty");

  OMProperty* result = _persistentProperties.get(propertyName);
  return result;
}

OMProperty* OMStorable::findProperty(OMPropertyId propertyId) const
{
  TRACE("OMStorable::findProperty");

  OMProperty* result = _persistentProperties.get(propertyId);
  return result;
}

  // @mfunc Is this <c OMStorable> the root of the object
  //        containment hierarchy.
  //   @rdesc True if this is the root object, false otherwise.
  //   @this const
bool OMStorable::isRoot(void) const
{
  TRACE("OMStorable::isRoot");
  return false;
}

  // @mfunc The stored representation of this <c OMStorable>.
  //   @rdesc The <c OMStoredObject> representing this <c OMStorable>.
  //   @this const
OMStoredObject* OMStorable::store(void) const
{
  TRACE("OMStorable::store");

  if (_store == 0) {
    ASSERT("Valid container", container() != 0);
    OMStorable* nonConstThis = const_cast<OMStorable*>(this);
    if (_exists) {
      nonConstThis->_store = container()->store()->open(name());
    } else {
      nonConstThis->_store = container()->store()->create(name());
    }
  }
  POSTCONDITION("Valid store", _store != 0);
  return _store;
}

  // @mfunc Inform this <c OMStorable> where it should store its
  //        persistent representation.
  //   @parm The <c OMStoredObject> on which this <c OMStorable>
  //         should be persisted.
void OMStorable::setStore(OMStoredObject* store)
{
  TRACE("OMStorable::setStore");
  PRECONDITION("Valid store", store != 0);
  PRECONDITION("No previous valid store", _store == 0);
  _store = store;
}

  // @mfunc Is this <c OMStorable> attached to (owned by) another
  //        OMStorable ?
  //   @rdesc True if this <c OMStorable> is attached, false
  //          otherwise.
  //   @this const
bool OMStorable::attached(void) const
{
  TRACE("OMStorable::attached");

  bool result;

  if (container() != 0) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

  // @mfunc Is this <c OMStorable> associated with an <c OMFile> ?
  //   @rdesc True if this <c OMStorable> is associated with an
  //          <c OMFile> , false otherwise.
  //   @this const
bool OMStorable::inFile(void) const
{
  TRACE("OMStorable::inFile");
  //PRECONDITION("object is attached", attached());

  bool result;

  if (container() != 0) {
    result = container()->inFile();
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMStorable> a persistent object ?
  //        Persistent objects are associated with a persistent
  //        store (disk file).
  //   @rdesc True if this <c OMStorable> is persistent, false
  //          otherwise.
  //   @this const
bool OMStorable::persistent(void) const
{
  TRACE("OMStorable::persistent");
  //PRECONDITION("object is in file", inFile());

  bool result;

  if (container() != 0) {
    result = container()->persistent();
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMStorable> dirty ?
  //        A dirty object is one that has been modified since it
  //        was last saved or that has never been saved at all.
bool OMStorable::isDirty(void) const
{
  TRACE("OMStorable::isDirty");

  // Dirty bit not yet implemented, consider all objects dirty.
  //
  return true;
}

  // @mfunc This object's property set.
  //   @rdesc A pointer to this object's <c OMPropertySet>.
OMPropertySet* OMStorable::propertySet(void)
{
  TRACE("OMStorable::propertySet");

  return &_persistentProperties;
}

  // @mfunc The <c OMClassFactory> that created this object.
  //   @rdesc The <c OMClassFactory> that created this object.
  //   @this const
OMClassFactory* OMStorable::classFactory(void) const
{
  TRACE("OMStorable::classFactory");
  PRECONDITION("Valid class factory", _classFactory != 0);

  return const_cast<OMClassFactory*>(_classFactory);
}

  // @mfunc Inform this <c OMStorable> of the <c OMClassFactory>
  //      that was used to create it.
  //   @parm The <c OMClassFactory> that was used to create
  //         this <c OMStorable>.
void OMStorable::setClassFactory(const OMClassFactory* classFactory)
{
  TRACE("OMStorable::setClassFactory");
  PRECONDITION("Valid class factory", classFactory != 0);
  //PRECONDITION("No previous valid class factory", _classFactory == 0);

  _classFactory = classFactory;

  POSTCONDITION("Valid class factory", _classFactory != 0);
}

OMStorable* OMStorable::shallowCopy(void) const
{
  TRACE("OMStorable::shallowCopy");
  OMStorable* result = shallowCopy(classFactory());

  POSTCONDITION("Valid result", result != 0);
  return result;
}

OMStorable* OMStorable::shallowCopy(const OMClassFactory* factory) const
{
  TRACE("OMStorable::shallowCopy");
  PRECONDITION("Valid class factory", factory != 0);

  const OMStoredObjectIdentification& id = classId();
  OMStorable* object = factory->create(id);
  ASSERT("Registered class id", object != 0);
  ASSERT("Valid class factory", object->classFactory() != 0);
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
  ASSERT("Valid class definition", object->definition() != 0);
#endif

  OMPropertySetIterator iterator(_persistentProperties, OMBefore);
  while (++iterator) {
    OMProperty* source = iterator.property();
    ASSERT("Valid property", source != 0);
    if (!source->isOptional() || source->isPresent()) {
      OMPropertyId pid = source->propertyId();
      OMProperty* dest = object->propertySet()->get(pid);
      source->shallowCopyTo(dest);
    }
  }

  POSTCONDITION("Valid result", object != 0);
  return object;
}

void OMStorable::deepCopyTo(OMStorable* destination,
                            void* clientContext) const
{
  TRACE("OMStorable::deepCopyTo");
  OMPropertySetIterator iterator(_persistentProperties, OMBefore);
  while (++iterator) {
    OMProperty* source = iterator.property();
    ASSERT("Valid property", source != 0);
    if (!source->isOptional() || source->isPresent()) {
      OMPropertyId pid = source->propertyId();
      OMProperty* dest = destination->propertySet()->get(pid);
      source->deepCopyTo(dest, clientContext);
    }
  }
}

  // @mfunc The number of objects contained within this <c OStorable>.
  //   @rdesc The number of objects.
OMUInt64 OMStorable::objectCount(void) const
{
  TRACE("OMStorable::objectCount");

  OMUInt64 result = 1; // Count this object
  OMPropertySetIterator iterator(_persistentProperties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    if (!p->isOptional() || p->isPresent()) {
      result = result + p->objectCount();
    }
  }
  return result;
}

  // @mfunc Inform this <c OMStorable> that it is about to be saved.
  //        The <p clientContext> passed is the one that was specified
  //        in the currently active call to <mf OMStorable::save>.
  //   @parm void *| clientContext | A context for the client.
  //   @this const
void OMStorable::onSave(void*) const
{
  // nothing to do in this default implementation
}

  // @mfunc Inform this <c OMStorable> that it has just been restored.
  //        The <p clientContext> passed is the one that was specified
  //        in the currently active call to <mf OMStorable::restore>.
  //   @parm void *| clientContext | A context for the client.
  //   @this const
void OMStorable::onRestore(void*) const
{
  // nothing to do in this default implementation
}

  // @mfunc Inform this <c OMStorable> that it has just been copied by
  //        <mf OMStorable::deepCopyTo>. The <p clientContext> passed is
  //        the one that was specified in the currently active call
  //        to <mf OMStorable::deepCopyTo>. When <mf OMStorable::onCopy>
  //        is called only the shallow portion of the deep copy to be
  //        performed by <mf OMStorable::deepCopyTo> has been completed.
  //        That is, this <c OMStorable> contains copies of all of the
  //        properties of the souce <c OMStorable> except for strong
  //        references (contained objects). Those properties wll be
  //        copied after <mf OMStorable::onCopy> returns.
  //   @parm void *| clientContext | A context for the client.
  //   @this const
void OMStorable::onCopy(void*) const
{
  // nothing to do in this default implementation
}

const OMStorable* OMStorable::container(void) const
{
  return _container;
}
