// @doc OMINTERNAL
#include "OMStorable.h"
#include "OMStoredObject.h"
#include "OMFile.h"
#include "OMClassFactory.h"
#include "OMObjectDirectory.h"
#include "OMTypes.h"
#include "OMProperty.h"

#include "OMAssertions.h"

#include <string.h>

OMStorable::OMStorable(void)
: _persistentProperties(), _containingObject(0), _name(0),
  _pathName(0), _containingProperty(0), _key(0), _store(0)
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
  PRECONDITION("Object not referenced", _containingProperty == 0);

  delete [] _name;
  _name = 0;
  delete [] _pathName;
  _pathName = 0;
}

  // @mfunc Save this <c OMStorable>.
  //   @this const
void OMStorable::save(void) const
{
  TRACE("OMStorable::save");
  
  //_file->objectDirectory()->insert(pathName(), this);
  store()->save(classId());
  store()->save(_persistentProperties);
}

  // @mfunc Close this <c OMStorable>.
void OMStorable::close(void)
{
  TRACE("OMStorable::close");
  PRECONDITION("Object is persistent", persistent());

  if (_store != 0) {
    size_t context = 0;
    for (size_t i = 0; i < _persistentProperties.count(); i++)
    {
      OMProperty* p = 0;
      _persistentProperties.iterate(context, p);
      ASSERT("Valid property", p != 0);
      p->close();
    }

    _store->close();
    delete _store;
    _store = 0;
  } // else silently ignore unsaved object

  POSTCONDITION("Closed", _store == 0);
}

  // @mfunc Restore the contents of an <c OMStorable> (of unknown
  //        sub-class) from the stored representation <p s>.
  //   @parm The <c OMStoredObject> from which to restore this
  //   <c OMStorable>.
void OMStorable::restoreContentsFrom(OMStoredObject& s)
{
  TRACE("OMStorable::restoreContentsFrom");
  s.restore(_persistentProperties);
}

  // @mfunc Restore an <c OMStorable> (of unknown sub-class) from
  //        the stored representation <p s>.
  //   @parm The <c OMStorable> that will contain (own) the newly
  //   restored <c OMStorable>.
  //   @parm The name to be given the newly restored <c OMStorable>.
  //   @parm The <c OMStoredObject> from which to restore this
  //   <c OMStorable>.
OMStorable* OMStorable::restoreFrom(const OMStorable* containingObject,
                                    const char* name,
                                    OMStoredObject& s)
{
  TRACE("OMStorable::restoreFrom");
  OMClassId cid;
  s.restore(cid);
  OMFile* f = containingObject->file();
  OMStorable* object = f->classFactory()->create(cid);
  ASSERT("Registered class id", object != 0);
  // give the object a parent, no orphans allowed
  object->setContainingObject(containingObject);
  // give the object a name, all new objects need a name and so here
  // we baptize them
  object->setName(name);
  object->setStore(&s);
  f->objectDirectory()->insert(object->pathName(), object);
  object->restoreContentsFrom(s);
  return object;
}

  // @mfunc  The <c OMStorable> that contains (owns) this
  //          <c OMStorable>.
  //   @rdesc The containing <c OMStorable>.
  //   @this const
OMStorable* OMStorable::containingObject(void) const
{
  TRACE("OMStorable::containingObject");

  return const_cast<OMStorable*>(_containingObject);
}

  // @mfunc Inform this <c OMStorable> that it is contained
  //        (owned) by the <c OMStorable> <p containingObject>.
  //   @parm The containing <c OMStorable>.
void OMStorable::setContainingObject(const OMStorable* containingObject)
{
  TRACE("OMStorable::setContainingObject");
  //PRECONDITION("No valid old containing object", _containingObject == 0);
  PRECONDITION("Valid new containing object", containingObject != 0);
  _containingObject = containingObject;
  delete [] _pathName;
  _pathName = 0;
}

  // @mfunc Inform this <c OMStorable> that it is no longer contained.
void OMStorable::clearContainingObject(void)
{
  TRACE("OMStorable::clearContainingObject");
  _containingObject = 0;
}

  // @mfunc Inform this <c OMStorable> that it is contained
  //        within the <c OMProperty> <p containingProperty>.
  //   @parm The containing <c OMProperty>.
  //   @parm A key used to be used by this <c OMStorable> to identify
  //         itself in future transactions with the given <c OMProperty>.
void OMStorable::setContainingProperty(const OMProperty* containingProperty,
                                       const size_t key)
{
  TRACE("OMStorable::setContainingProperty");
  PRECONDITION("Object not already attached", _containingProperty == 0);
  PRECONDITION("Valid property", containingProperty != 0);

  _containingProperty = const_cast<OMProperty*>(containingProperty);
  _key = key;

  POSTCONDITION("Object properly attached", _containingProperty != 0);
}

  // @mfunc Inform this <c OMStorable> that it is no longer
  //        contained within any <c OMProperty>.
void OMStorable::clearContainingProperty(void)
{
  TRACE("OMStorable::clearContainingProperty");
  _containingProperty = 0;
}

  // @mfunc The name of this <c OMStorable>.
  //   @rdesc The name of this <c OMStorable>.
  //   @this const
const char* OMStorable::name(void) const
{
  TRACE("OMStorable::name");
  return _name;
}

  // @mfunc Give this <c OMStorable> a name.
  //   @parm The name to be given to this <c OMStorable>.
void OMStorable::setName(const char* name)
{
  TRACE("OMStorable::setName");
  PRECONDITION("Valid name", validString(name));
  delete [] _name;
  _name = 0; // for BoundsChecker
  _name = new char[strlen(name) + 1];
  ASSERT("Valid heap pointer", _name != 0);
  strcpy(_name, name);
  delete [] _pathName;
  _pathName = 0;
}

  // @mfunc The <c OMFile> in which this <c OMStorable> has a
  //        persistent representation.
  //   @rdesc The <c OMFile> in which this <c OMStorable> has a
  //          persistent representation.
  //   @this const
OMFile* OMStorable::file(void) const
{
  TRACE("OMStorable::file");
  PRECONDITION("Valid containing object", _containingObject != 0);
  return _containingObject->file();
}

const char* OMStorable::pathName(void) const
{
  TRACE("OMStorable::pathName");

  if (_pathName == 0) {
    OMStorable* nonConstThis = const_cast<OMStorable*>(this);
    nonConstThis->_pathName = nonConstThis->makePathName();
  }
  ASSERT("Valid path name", validString(_pathName));
  return _pathName;
}

  // @mfunc The stored representation of this <c OMStorable>.
  //   @rdesc The <c OMStoredObject> representing this <c OMStorable>.
  //   @this const
OMStoredObject* OMStorable::store(void) const
{
  TRACE("OMStorable::store");

  if (_store == 0) {
    OMStorable* container = containingObject();
    ASSERT("Valid container", container != 0);
    OMStorable* nonConstThis = const_cast<OMStorable*>(this);
    nonConstThis->_store = container->store()->create(name());
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

  if (_containingProperty != 0) {
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
  OMStorable* container = containingObject();
  if (container != 0) {
    result = container->inFile();
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
  OMStorable* container = containingObject();
  if (container != 0) {
    result = container->persistent();
  } else {
    result = false;
  }
  return result;
}

  // @mfunc This object's property set.
  //   @rdesc A pointer to this object's <c OMPropertySet>.
OMPropertySet* OMStorable::propertySet(void)
{
  TRACE("OMStorable::propertySet");

  return &_persistentProperties;
}

char* OMStorable::makePathName(void)
{
  TRACE("OMStorable::makePathName");
  // Don't need to compute path name each time, should save once computed.

  // All objects must have a name.
  //
  ASSERT("Object has a name", validString(name()));

  // By definition the root object is the one with no container.
  // Check that the root object is called "/".
  //
  ASSERT("Root object properly named",
                   IMPLIES(containingObject() == 0, strcmp(name(), "/") == 0));

  // Only the root object is called "/".
  // Check that if the object is called "/" it is the root object (has
  // no container).
  //
  ASSERT("Non-root object properly named",
                   IMPLIES(strcmp(name(), "/") == 0, containingObject() == 0));
  
  char* result = 0;
  OMStorable* cont = containingObject();
  if (cont != 0) {
    if (cont->containingObject() != 0) {
      // general case
      result = new char[strlen(cont->pathName()) + strlen(name()) + 1 + 1];
      ASSERT("Valid heap pointer", result != 0);
      strcpy(result, cont->pathName());
      strcat(result, "/");
      strcat(result, name());
    } else {
      // child of root
      result = new char[strlen(cont->pathName()) + strlen(name()) + 1];
      ASSERT("Valid heap pointer", result != 0);
      strcpy(result, cont->pathName());
      strcat(result, name());
    }
  } else {
    // root
    result = new char[strlen(name()) + 1];
    ASSERT("Valid heap pointer", result != 0);
    strcpy(result, name());
  }

  POSTCONDITION("Valid result", validString(result));
  return result;

}
