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

// @doc OMINTERNAL
#include "OMObjectReference.h"

// class OMObjectReference
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMObjectReference

  // @mfunc Constructor.
OMObjectReference::OMObjectReference(void)
: _property(0),
  _pointer(0)
{
  TRACE("OMObjectReference::OMObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMObjectReference>.
OMObjectReference::OMObjectReference(OMProperty* property)
: _property(property),
  _pointer(0)
{
  TRACE("OMObjectReference::OMObjectReference");

  POSTCONDITION("non-void", !isVoid());
}

  // @mfunc Copy constructor.
  //   @parm The <c OMObjectReference> to copy.
OMObjectReference::OMObjectReference(const OMObjectReference& rhs)
: _property(rhs._property),
  _pointer(rhs._pointer)
{
  TRACE("OMObjectReference::OMObjectReference");
}

  // @mfunc Destructor.
OMObjectReference::~OMObjectReference(void)
{
  TRACE("OMObjectReference::~OMObjectReference");
}

  // @mfunc Is this <c OMObjectReference> void ?
  //   @rdesc True if this <c OMObjectReference> is void,
  //          false otherwise. 
  //   @this const
bool OMObjectReference::isVoid(void) const
{
  bool result;
  if ((_property == 0) && (_pointer == 0)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMObjectReference> to be assigned.
  //   @rdesc The <c OMObjectReference> resulting from the assignment. 
OMObjectReference& OMObjectReference::operator= (const OMObjectReference& rhs)
{
  TRACE("OMObjectReference::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }
  _property = rhs._property;
  _pointer = rhs._pointer;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
  //   @this const
bool OMObjectReference::operator== (const OMObjectReference& rhs) const
{
  bool result;

  if ((_property == rhs._property) && (_pointer == rhs._pointer)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The value of this <c OMObjectReference> as a pointer.
  //        This function provides low-level access. If the object exits
  //        but has not yet been loaded then the value returned is 0.
  //   @rdesc A pointer to the referenced object, if any, otherwise 0.
  //   @this const
OMStorable* OMObjectReference::pointer(void) const
{
  TRACE("OMObjectReference::pointer");

  return _pointer;
}

// class OMStrongObjectReference

  // @mfunc Constructor.
OMStrongObjectReference::OMStrongObjectReference(void)
: OMObjectReference(),
  _isLoaded(true),
  _name(0)
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMStrongObjectReference>.
  //   @parm The name of this <c OMStrongObjectReference>.
OMStrongObjectReference::OMStrongObjectReference(OMProperty* property,
                                                 const wchar_t* name)
: OMObjectReference(property),
  _isLoaded(true),
  _name(saveWideString(name))
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");
  POSTCONDITION("non-void", !isVoid());
}

  // @mfunc Copy constructor.
  //   @parm The <c OMStrongObjectReference> to copy.
OMStrongObjectReference::OMStrongObjectReference(
                                            const OMStrongObjectReference& rhs)
: OMObjectReference(rhs),
  _isLoaded(rhs._isLoaded),
  _name(0)
{
  TRACE("OMStrongObjectReference::OMStrongObjectReference");

  if (rhs._name != 0) {
    _name = saveWideString(rhs._name);
  }
}

  // @mfunc Destructor.
OMStrongObjectReference::~OMStrongObjectReference(void)
{
  TRACE("OMStrongObjectReference::~OMStrongObjectReference");

  delete [] _name;
  _name = 0;
}

  // @mfunc Is this <c OMStrongObjectReference> void ?
  //   @rdesc True if this <c OMStrongObjectReference> is void,
  //          false otherwise. 
  //   @this const
bool OMStrongObjectReference::isVoid(void) const
{
  bool result = OMObjectReference::isVoid();
  if (result) {
    if ((_isLoaded) && (_name == 0)) {
      result = true;
    } else {
      result = false;
    }
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMStrongObjectReference> to be assigned.
  //   @rdesc The <c OMStrongObjectReference> resulting from the assignment. 
OMStrongObjectReference&
OMStrongObjectReference::operator= (const OMStrongObjectReference& rhs)
{
  OMObjectReference::operator=(rhs);
  _isLoaded = rhs._isLoaded;
  delete [] _name;
  _name = 0; // for BoundsChecker
  if (rhs._name != 0) {
    _name = saveWideString(rhs._name);
  } else {
    _name = 0;
  }
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMStrongObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
  //   @this const
bool
OMStrongObjectReference::operator== (const OMStrongObjectReference& rhs) const
{
  bool result = OMObjectReference::operator==(rhs);
  if (result) {
    if (_isLoaded == rhs._isLoaded) {
      if ((_name != 0) && (rhs._name != 0)) {
        if (compareWideString(_name, rhs._name) == 0) {
          result = true;
        } else {
          result = false;
        }
      } else if ((_name == 0) && (rhs._name == 0)) {
        result = true;
      } else {
        result = false;
      }
    } else {
      result = false;
    }
  }
  return result;
}

  // @mfunc Save this <c OMStrongObjectReference>.
  //   @this const
void OMStrongObjectReference::save(void) const
{
  TRACE("OMStrongObjectReference::save");

  if (isLoaded()) {

    // Either the object does not exist in the file or the object
    // exists in the file and has been loaded. In either case the
    // object is saved. If the object has been loaded, we assume, in
    // the absence of a dirty bit, that it has been changed.

    ASSERT("Non-void strong reference", _pointer != 0);

    // Notify the client that we are about to save the object and then
    // save the object.
    //
    ASSERT("Valid containing property", _property != 0);
    OMFile* file = _property->propertySet()->container()->file();
    ASSERT("Valid file", file != 0);
    _pointer->onSave(file->clientOnSaveContext());
    _pointer->save();
  }

}

  // @mfunc Close this <c OMStrongObjectReference>.
void OMStrongObjectReference::close(void)
{
  TRACE("OMStrongObjectReference::close");

  if (_pointer != 0) {
    _pointer->close();
  }

  // So that an attempt to access an unloaded object after the file
  // has been closed results in a void reference (there is an
  // inaccessible persisted representation of the object).
  //
  setLoaded();
}

  // @mfunc Detach this <c OMStrongObjectReference>.
void OMStrongObjectReference::detach(void)
{
  TRACE("OMStrongObjectReference::detach");

  if (_pointer != 0) {
    _pointer->detach();
  }

  // So that an attempt to access an unloaded object that has been
  // detached results in a void reference (there is an inaccessible
  // persisted representation of the object).
  //
  setLoaded();
}

  // @mfunc Restore this <c OMStrongObjectReference>.
void OMStrongObjectReference::restore(void)
{
  TRACE("OMStrongObjectReference::restore");

  PRECONDITION("Correct initial loaded state", isLoaded());
  PRECONDITION("Reference not already set", _pointer == 0);
  PRECONDITION("Valid containing property", _property != 0);

  OMFile::OMLoadMode loadMode =
                     _property->propertySet()->container()->file()->loadMode();

  clearLoaded();
  if (loadMode == OMFile::eagerLoad) {
    load();
  }
}

  // @mfunc Get the value of this <c OMStrongObjectReference>.
  //        The value is a pointer to the <c ReferencedObject>.
  //   @rdesc  A pointer to the <p ReferencedObject>.
  //   @this const
OMStorable* OMStrongObjectReference::getValue(void) const
{
  TRACE("OMStrongObjectReference::getValue");

  OMStrongObjectReference* nonConstThis =
                  const_cast<OMStrongObjectReference*>(this);

  if (!isLoaded()) {
    nonConstThis->load();
  }
  ASSERT("Loaded", isLoaded());

  OMStorable* result = _pointer;

  return result;
}

  // @mfunc Set the value of this <c OMStrongObjectReference>.
  //        The value is a pointer to the <c OMStorable>.
  //   @parm A pointer to the new <p OMStorable>.
  //   @rdesc A pointer to previous <p OMStorable>, if any.
OMStorable* OMStrongObjectReference::setValue(const OMStorable* value)
{
  TRACE("OMStrongObjectReference::setValue");

  PRECONDITION("Valid container property", _property != 0);

  // Detach the old object
  //
  OMStorable* oldObject = _pointer;
  if (oldObject != 0) {
    oldObject->detach();
  }

  // Set the element to contain the new object
  //
  _pointer = const_cast<OMStorable*>(value);
  OMStorable* newObject = _pointer;

  // Attach the new object
  //
  if (newObject != 0) {
    OMStorable* container = _property->propertySet()->container();
    newObject->attach(container, _name);
  }
  setLoaded();
  POSTCONDITION("Element properly set", _pointer == newObject);
  return oldObject;
}

  // @mfunc Is this <c OMStrongObjectReference> in the loaded state. If false
  //        there is a persisted representation of this
  //        <c OMStrongObjectReference> that can be loaded.
  //   @this const
bool OMStrongObjectReference::isLoaded(void) const
{
  TRACE("OMStrongObjectReference::isLoaded");

  return _isLoaded;
}

  // @mfunc Put this <c OMStrongObjectReference> into the loaded state.
void OMStrongObjectReference::setLoaded(void)
{
  TRACE("OMStrongObjectReference::setLoaded");

  _isLoaded = true;
}

  // @mfunc Remove this <c OMStrongObjectReference> from the loaded state.
void OMStrongObjectReference::clearLoaded(void)
{
  TRACE("OMStrongObjectReference::clearLoaded");

  _isLoaded = false;
}

  // @mfunc Load the persisted representation of this
  //        <c OMStrongObjectReference>.
void OMStrongObjectReference::load(void)
{
  TRACE("OMStrongObjectReference::load");

  PRECONDITION("Not already loaded", !isLoaded());
  PRECONDITION("Valid container property", _property != 0);

  // open the sub-storage
  //
  OMStorable* containingObject = _property->propertySet()->container();
  OMStoredObject* store = containingObject->store();
  ASSERT("Valid store", store != 0);

  OMStoredObject* subStorage = store->open(_name);

  // restore contents from the sub-storage
  //
  OMStorable* object = OMStorable::restoreFrom(containingObject,
                                               _name,
                                               *subStorage);
  ASSERT("Object properly restored", object != 0);

  // place a pointer to the newly restored object in this element
  //
  setValue(object);

  setLoaded();

  // notify the client that the object has just been restored
  //
  ASSERT("Valid containing property", _property != 0);
  OMFile* file = _property->propertySet()->container()->file();
  ASSERT("Valid file", file != 0);
  _pointer->onRestore(file->clientOnSaveContext());

  POSTCONDITION("Property properly loaded", isLoaded());
}
