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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMWeakReferenceProperty
#ifndef OMWEAKREFPROPERTYT_H
#define OMWEAKREFPROPERTYT_H

#include "OMAssertions.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceProperty>.
  //   @parm The name (as a string) of the the <c OMProperty> instance
  //         (a set property) in which the object referenced by this
  //         <c OMWeakReferenceProperty> resides.
  //   @parm The id of the property by which the <p ReferencedObject>
  //         is uniquely identified (the key).
template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty(
                                              const OMPropertyId propertyId,
                                              const wchar_t* name,
                                              const wchar_t* targetName,
                                              const OMPropertyId keyPropertyId)
: OMWeakReference(propertyId, name),
  _reference(),
  _targetTag(nullOMPropertyTag),
  _targetName(targetName),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty");

  _reference = OMWeakObjectReference<ReferencedObject>(this);
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWeakReferenceProperty>.
  //   @parm The id of the property by which the <p ReferencedObject>
  //         is uniquely identified (the key).
  //   @parm The name (as a list of pids) of the the <c OMProperty> instance
  //         (a set property) in which the object referenced by this
  //         <c OMWeakReferenceProperty> resides.
template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty(
                                        const OMPropertyId propertyId,
                                        const wchar_t* name,
                                        const OMPropertyId keyPropertyId,
                                        const OMPropertyId* targetPropertyPath)
: OMWeakReference(propertyId, name),
  _reference(),
  _targetTag(nullOMPropertyTag),
  _targetName(0),
  _targetPropertyPath(0),
  _keyPropertyId(keyPropertyId)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::OMWeakReferenceProperty");

  _reference = OMWeakObjectReference<ReferencedObject>(this);
  _targetPropertyPath = savePropertyPath(targetPropertyPath);
}

template <typename ReferencedObject>
OMWeakReferenceProperty<ReferencedObject>::~OMWeakReferenceProperty(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::~OMWeakReferenceProperty");

  delete [] _targetPropertyPath;
}

  // @mfunc Get the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to a <p ReferencedObject> by reference.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::getValue(
                                               ReferencedObject*& object) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getValue");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  OMStorable* p = _reference.getValue();
  if (p != 0) {
    ReferencedObject* result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
    object = result;
  } else {
    object = 0;
  }
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

  PRECONDITION("Valid object", object != 0);
#if defined(OM_VALIDATE_WEAK_REFERENCES)
  PRECONDITION("Source container object attached to file",
                                                        container()->inFile());
  PRECONDITION("Target object attached to file", object->inFile());
  PRECONDITION("Source container object and target object in same file",
                                        container()->file() == object->file());

  _reference.setTargetTag(targetTag());
#endif
  OMStorable* p = _reference.setValue(object);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  } else {
    result = 0;
  }
  setPresent();
  return result;
}

  // @mfunc Clear the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to the old <p ReferencedObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
ReferencedObject* OMWeakReferenceProperty<ReferencedObject>::clearValue(void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::clearValue");

  OMStorable* p = _reference.setValue(0);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  } else {
    result = 0;
  }
  return result;
}

  // @mfunc Assignment operator.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc The result of the assignment.
  //   @parm A pointer to a <p ReferencedObject> by value.
template <typename ReferencedObject>
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
template <typename ReferencedObject>
ReferencedObject*
OMWeakReferenceProperty<ReferencedObject>::operator -> (void)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::operator ->");

  return _reference.getValue();
}

template <typename ReferencedObject>
const ReferencedObject*
OMWeakReferenceProperty<ReferencedObject>::operator -> (void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::operator ->");

  return _reference.getValue();
}

  // @mfunc Type conversion. Convert an
  //        <c OMWeakReferenceProperty> into a pointer to the
  //        referenced (pointed to) <p ReferencedObject>.
  //   @rdesc The result of the conversion as a value of type
  //          pointer to <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
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
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::save(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::save");

  PRECONDITION("Non-void weak reference", !_reference.isVoid());

  OMPropertyTag tag = targetTag();

  const OMUniqueObjectIdentification& id = _reference.identification();
  store()->save(_propertyId, _storedForm, id, tag, _keyPropertyId);

  _reference.save();
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
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::restore(
                                                    size_t ANAME(externalSize))
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::restore");

  OMUniqueObjectIdentification id;
  OMPropertyTag tag;
  ASSERT("Sizes match", (sizeof(tag) + sizeof(OMPropertyId) +
                         sizeof(OMKeySize) + sizeof(id)) == externalSize);
  OMPropertyId keyPropertyId;
  store()->restore(_propertyId, _storedForm, id, tag, keyPropertyId);
  ASSERT("Consistent key property ids", keyPropertyId == _keyPropertyId);
  _targetTag = tag;
  _reference = OMWeakObjectReference<ReferencedObject>(this, id, _targetTag);
  _reference.restore();
  setPresent();
}

  // @mfunc  Is this <c OMWeakReferenceProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc True if this <c OMWeakReferenceProperty> is void, false
  //          otherwise
  //   @this const
template <typename ReferencedObject>
bool OMWeakReferenceProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::isVoid");

  bool result;
  const OMUniqueObjectIdentification& key = _reference.identification();
  if (key == nullOMUniqueObjectIdentification) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

  // @mfunc Get the raw bits of this <c OMWeakReferenceProperty>. The raw bits
  //        are copied to the buffer at address <p bits> which is
  //        <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
  //   @this const
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceProperty");

  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  ReferencedObject* pointer;
  getValue(pointer);

  memcpy(bits, &pointer, bitsSize());
}

  // @mfunc Set the raw bits of this <c OMWeakReferenceProperty>. The raw
  //        bits are copied from the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::setBits(const OMByte* bits,
                                                        size_t ANAME(size))
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getBits");
  OBSOLETE("methods on class OMReferenceProperty");

  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject* p = *(const ReferencedObject**)bits;
  setValue(p);
}

  // @mfunc Get the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc A pointer to an <c OMObject>.
  //   @this const
template <typename ReferencedObject>
OMObject*
OMWeakReferenceProperty<ReferencedObject>::getObject(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::getObject");

  ReferencedObject* p = 0;  
  getValue(p);
  OMObject* result = p;

  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @parm A pointer to the new <c OMObject>.
  //   @rdesc A pointer to the old <c OMObject>. If lazy
  //          loading is enabled and the referenced object was never
  //          loaded the value returned is 0.
template <typename ReferencedObject>
OMObject* OMWeakReferenceProperty<ReferencedObject>::setObject(
                                                        const OMObject* object)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::setObject");

  PRECONDITION("Valid object", object != 0);

  const ReferencedObject* p = dynamic_cast<const ReferencedObject*>(object);
  ASSERT("Object is correct type", p != 0);

  return setValue(p);
}


template <typename ReferencedObject>
OMPropertyId
OMWeakReferenceProperty<ReferencedObject>::keyPropertyId(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::keyPropertyId");

  return _keyPropertyId;
}

template <typename ReferencedObject>
void OMWeakReferenceProperty<ReferencedObject>::setTargetTag(
                                                       OMPropertyTag targetTag)
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::setTargetTag");

  _targetTag = targetTag;
}

template <typename ReferencedObject>
OMPropertyTag OMWeakReferenceProperty<ReferencedObject>::targetTag(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::targetTag");

  PRECONDITION("Property is attached to file", container()->inFile());

  OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);
  if (_targetTag == nullOMPropertyTag) {
    nonConstThis->_targetTag =
                  file()->referencedProperties()->insert(targetPropertyPath());
  }
  POSTCONDITION("Valid target property tag", _targetTag != nullOMPropertyTag);
  return _targetTag;
}

template <typename ReferencedObject>
OMPropertyId*
OMWeakReferenceProperty<ReferencedObject>::targetPropertyPath(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::targetPropertyPath");

  PRECONDITION("Valid target name", validWideString(_targetName));

  if (_targetPropertyPath == 0) {
    OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);
    nonConstThis->_targetPropertyPath = file()->path(_targetName);
  }
  POSTCONDITION("Valid result", _targetPropertyPath != 0);
  return _targetPropertyPath;
}

template <typename ReferencedObject>
void
OMWeakReferenceProperty<ReferencedObject>::clearTargetTag(void) const
{
  TRACE("OMWeakReferenceProperty<ReferencedObject>::clearTargetTag");

  OMWeakReferenceProperty<ReferencedObject>* nonConstThis =
                  const_cast<OMWeakReferenceProperty<ReferencedObject>*>(this);

  nonConstThis->_targetTag = nullOMPropertyTag;
  delete [] nonConstThis->_targetPropertyPath;
  nonConstThis->_targetPropertyPath = 0;
}

#endif
