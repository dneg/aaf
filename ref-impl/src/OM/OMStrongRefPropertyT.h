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

template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::OMStrongReferenceProperty(
                                                 const OMPropertyId propertyId,
                                                 const char* name)
: OMReferenceProperty<ReferencedObject>(propertyId,
                                        SF_STRONG_OBJECT_REFERENCE,
                                        name),
  _reference(this, name)
{
  TRACE(
     "OMStrongReferenceProperty<ReferencedObject>::OMStrongReferenceProperty");
}

template <typename ReferencedObject>
OMStrongReferenceProperty<ReferencedObject>::~OMStrongReferenceProperty(void)
{
  TRACE(
    "OMStrongReferenceProperty<ReferencedObject>::~OMStrongReferenceProperty");
  _reference.setValue(0);
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

  ReferencedObject* result = _reference.getValue();

  object = result;
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

  setPresent();
  return _reference.setValue(object);

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
  return _reference.getValue();
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
  return _reference.getValue();
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
  //   @parm Client context for callbacks.
  //   @this const
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::save(
                                                     void* clientContext) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::save");

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());

  // Write the index entry.
  //
  OMStoredObject* s = container->store();
  const char* propertyName = name();
  s->write(_propertyId,
           _storedForm,
           (void *)propertyName,
           strlen(propertyName) + 1);

  _reference.save(clientContext);

}

  // @mfunc Close this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::close(void)
{
  if (!isOptional() || isPresent()) {
    _reference.close();
  }
}

  // @mfunc Detach this <c OMStrongReferenceProperty>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
template <typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::detach(void)
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::detach");
  if (!isOptional() || isPresent()) {
    _reference.detach();
  }
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

  // retrieve sub-storage name
  //
  char* storageName = new char[externalSize];
  ASSERT("Valid heap pointer", storageName != 0);

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  store->read(_propertyId, _storedForm, storageName, externalSize);
  ASSERT("Consistent property size", externalSize == strlen(storageName) + 1);
  ASSERT("Consistent property name", strcmp(storageName, name()) == 0);
  delete [] storageName;

  _reference.restore();

  setPresent();

}

  // @mfunc  Is this <c OMStrongReferenceProperty> void ?
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @rdesc True if this <c OMStrongReferenceProperty> is void, false
  //          otherwise
  //   @this const
template<typename ReferencedObject>
bool OMStrongReferenceProperty<ReferencedObject>::isVoid(void) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::isVoid");

  bool result;

  if (_reference.getValue() == 0) {
    result = true;
  } else {
    result = false;
  }

  return result;
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
  PRECONDITION("Property is void", isVoid());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc Get the raw bits of this <c OMStrongReferenceProperty>.
  //        The raw bits are copied to the buffer at address <p bits>
  //        which is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
  //   @this const
template<typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::getBits(
                                                      OMByte* bits,
                                                      size_t ANAME(size)) const
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::getBits");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  ReferencedObject* pointer;
  getValue(pointer);

  memcpy(bits, &pointer, bitsSize());
}

  // @mfunc Set the raw bits of this <c OMStrongReferenceProperty>. The raw
  //        bits are copied from the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm size_t | size | The size of the buffer.
template<typename ReferencedObject>
void OMStrongReferenceProperty<ReferencedObject>::setBits(const OMByte* bits,
                                                          size_t ANAME(size))
{
  TRACE("OMStrongReferenceProperty<ReferencedObject>::getBits");
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size >= bitsSize());

  const ReferencedObject* p = *(const ReferencedObject**)bits;
  setValue(p);
}

#endif
