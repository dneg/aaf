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
#ifndef OMWEAKREFPROPERTYT_H
#define OMWEAKREFPROPERTYT_H

#include "OMAssertions.h"

template<typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty(
                                                 const OMPropertyId propertyId,
                                                 const char* name)
: OMReferenceProperty<ReferencedObject>(propertyId,
                                        SF_WEAK_OBJECT_REFERENCE,
                                        name), _pathName(0)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty");
}

template<typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::~OMWeakReferenceProperty(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::~OMWeakReferenceProperty");
  delete [] _pathName;
  _pathName = 0;
}

  // @mfunc Get the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @this const
template<typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::getValue(
                                               ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getValue");

  if (_pointer == 0) {
    if (_pathName != 0) {
      // We're implementing "conceptual constness" and not "bitwise constness"
      // so cast away the constness of this.
      //
      OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);
      
      // lookup _pathName in the object directory
      //
      OMStorable* obj;
      OMFile* file = _propertySet->container()->file();
      bool found = file->objectDirectory()->lookup(_pathName, obj);
      // Since this prototype does not currently implement lazy
      // loading all weak references should be resolvable as the
      // referenced object must have been loaded. Check this with an
      // assertion.
      //
      ASSERT("Weak reference resolved", found);
      if (found) {
        nonConstThis->_pointer = dynamic_cast<ReferencedObject*>(obj);
        ASSERT("Consistent object names",
                                 strcmp(_pathName, _pointer->pathName()) == 0);
      } else {
        // This code can only execute for the "object level" and finer
        // granularities of lazy loading.  Since this prototype does
        // not currently implement lazy loading we shouldn't get here.
        // Check this with an assertion.
        //
        ASSERT("Unsupported code not reachable", false);

        // open the named storage
        //
        OMFile* file =_propertySet->container()->file();
        OMStoredObject* root = file->rootStoredObject();
        OMStoredObject* storage = root->openStoragePath(_pathName);

        // restore object contents from the storage
        //
        OMStorable* referencedObject = OMStorable::restoreFrom(
                                                     _propertySet->container(),
                                                     name(),
                                                     *storage);
        nonConstThis->_pointer =
                             dynamic_cast<ReferencedObject*>(referencedObject);

        // close the storage
        //
        storage->close();  // tjb unreachable close
        delete storage;
      }
    }
  }
  object = _pointer;
}

  // @mfunc Set the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceProperty<ReferencedObject>::setValue(
                                               const ReferencedObject* object)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::setValue");

  delete [] _pathName;
  _pathName = 0;
  return OMReferenceProperty<ReferencedObject>::setValue(object);
}

  // @mfunc Assignment operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The result of the assignment.
  //   @parm A pointer to a <p ReferencedObject> by value.
template<typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>&
OMWeakReferenceProperty<ReferencedObject>::operator = (
                                                 const ReferencedObject* value)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::operator =");

  setValue(value);
  return *this;
}

  // @mfunc Dereference operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>
  //   @rdesc A pointer to a <p ReferencedObject> by value.
template<typename ReferencedObject>
ReferencedObject*
OMWeakReferenceProperty<ReferencedObject>::operator -> (void)
{
  return pointer();
}

template<typename ReferencedObject>
const ReferencedObject*
OMWeakReferenceProperty<ReferencedObject>::operator -> (void) const
{
  return pointer();
}

  // @mfunc Type conversion. Convert an
  //        <c OMWeakReferenceProperty> into a pointer to the
  //        referenced (pointed to) <p ReferencedObject>.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p ReferencedObject>.
  //   @this const
template<typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::operator ReferencedObject* () const
{
  TRACE(
      "OMWeakReferenceProperty<ReferencedObject>::operator ReferencedObject*");

  ReferencedObject* result;
  getValue(result);
  return result;
}

  // @mfunc Save this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @this const
template<typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::save");

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());
  OMStoredObject* s = container->store();

  PRECONDITION("Valid weak reference",
                                      IMPLIES(pointer() == 0, _pathName != 0));
  PRECONDITION("Valid weak reference",
                                      IMPLIES(pointer() != 0, _pathName == 0));

  const char* pathName = 0;
  OMStorable* object = pointer();
  if (object != 0) {
    pathName = object->pathName();
  } else {
    pathName = _pathName;
  }
  ASSERT("Non-void weak reference", pathName != 0);

  // save this name as the value of this property
  s->write(_propertyId,
           _storedForm,
           (void *)pathName,
           strlen(pathName) + 1);

}

  // @mfunc Close this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::close(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::close");
  // tjb
}

  // @mfunc Restore this <c OMWeakReferenceProperty>, the external
  //        (persisted) size of the <c OMWeakReferenceProperty> is
  //        <p externalSize>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm The external (persisted) size of the <c OMWeakReferenceProperty>.
template<typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::restore(size_t externalSize)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::restore");

  delete [] _pathName;
  // read the pathname from the file and save it in this weak reference
  _pathName = new char[externalSize];
  ASSERT("Valid heap pointer", _pathName != 0);

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  store->read(_propertyId, _storedForm, _pathName, externalSize);
  _pointer = 0;
}

  // @mfunc Load the persisted representation of this
  //        <c OMWeakReferenceProperty> into memory.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template<typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::load(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::load");
  ASSERT("Not yet implemented", false);
}

#endif
