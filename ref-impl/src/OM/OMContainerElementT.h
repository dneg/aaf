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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL

#include "OMAssertions.h"

// class OMVectorElement<ReferencedObject>
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMVectorElement

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::OMVectorElement(void)
: _pointer(0)
{
  TRACE("OMVectorElement<ReferencedObject>::OMVectorElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::OMVectorElement(
                                               const ReferencedObject* pointer)
: _pointer(const_cast<ReferencedObject*>(pointer))
{
  TRACE("OMVectorElement<ReferencedObject>::OMVectorElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMVectorElement> to copy.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::OMVectorElement(
                                  const OMVectorElement<ReferencedObject>& rhs)
{
  TRACE("OMVectorElement<ReferencedObject>::OMVectorElement");

  _pointer = rhs._pointer;
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::~OMVectorElement(void)
{
  TRACE("OMVectorElement<ReferencedObject>::~OMVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMVectorElement> to be assigned.
  //   @rdesc The <c OMVectorElement> resulting from the assignment.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>&
OMVectorElement<ReferencedObject>::operator= (
                                  const OMVectorElement<ReferencedObject>& rhs)
{
  TRACE("OMVectorElement<ReferencedObject>::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  _pointer = rhs._pointer;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
template <typename ReferencedObject>
bool OMVectorElement<ReferencedObject>::operator== (
                            const OMVectorElement<ReferencedObject>& rhs) const
{
  TRACE("OMVectorElement<ReferencedObject>::operator==");

  bool result;
  if (_pointer == rhs._pointer) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Get the value of this <c OMVectorElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMVectorElement<ReferencedObject>::getValue(void) const
{
  TRACE("OMVectorElement<ReferencedObject>::getValue");

  // No lazy loding for non-persistent objects
  return pointer();
}

  // @mfunc Set the value of this <c OMVectorElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
template <typename ReferencedObject>
ReferencedObject* OMVectorElement<ReferencedObject>::setValue(
                                                 const ReferencedObject* value)
{
  TRACE("OMVectorElement<ReferencedObject>::setValue");

  ReferencedObject* oldPointer = _pointer;
  _pointer = const_cast<ReferencedObject*>(value);
  return oldPointer;
}

  // @mfunc The value of this <c OMVectorElement> as a pointer.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @rdesc A pointer to the <p ReferencedObject>, if loaded.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMVectorElement<ReferencedObject>::pointer(void) const
{
  TRACE("OMVectorElement<ReferencedObject>::pointer");

  return _pointer;
}

// class OMSetElement<UniqueIdentification, ReferencedObject>
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSetElement

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement(void)
: OMVectorElement<ReferencedObject>(0)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement(
                                               const ReferencedObject* pointer)
: OMVectorElement<ReferencedObject>(pointer)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMSetElement> to copy.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement(
                                     const OMSetElement<UniqueIdentification,
                                                        ReferencedObject>& rhs)
: OMVectorElement<ReferencedObject>(rhs)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::~OMSetElement(void)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::~OMSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMSetElement> to be assigned.
  //   @rdesc The <c OMSetElement> resulting from the assignment.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>&
OMSetElement<UniqueIdentification, ReferencedObject>::operator= (
                                     const OMSetElement<UniqueIdentification,
                                                        ReferencedObject>& rhs)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMVectorElement<ReferencedObject>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool OMSetElement<UniqueIdentification, ReferencedObject>::operator== (
                               const OMSetElement<UniqueIdentification,
                                                  ReferencedObject>& rhs) const
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::operator==");

  bool result = OMVectorElement<ReferencedObject>::operator==(rhs);

  return result;
}

  // @mfunc The unique key of this <c OMSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @rdesc  The unique key of this <c OMSetElement>.
template <typename UniqueIdentification, typename ReferencedObject>
const UniqueIdentification&
OMSetElement<UniqueIdentification, ReferencedObject>::identification(
                                                                    void) const
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::"
                                                             "identification");
  ReferencedObject* object = getValue();
  ASSERT("Valid object", object!= 0);
  return object->identification();
}

// class OMContainerElement<ObjectReference>
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMContainerElement

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
template <typename ObjectReference>
OMContainerElement<ObjectReference>::OMContainerElement(void)
: _reference()
{
  TRACE("OMContainerElement<ObjectReference>::OMContainerElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @parm The <p ObjectReference> for this <c OMContainerElement>.
template <typename ObjectReference>
OMContainerElement<ObjectReference>::OMContainerElement(
                                              const ObjectReference& reference)
: _reference(reference)
{
  TRACE("OMContainerElement<ObjectReference>::OMContainerElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @parm The <c OMContainerElement> to copy.
template <typename ObjectReference>
OMContainerElement<ObjectReference>::OMContainerElement(
                                const OMContainerElement<ObjectReference>& rhs)
: _reference(rhs._reference)
{
  TRACE("OMContainerElement<ObjectReference>::OMContainerElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
template <typename ObjectReference>
OMContainerElement<ObjectReference>::~OMContainerElement(void)
{
  TRACE("OMContainerElement<ObjectReference>::~OMContainerElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @parm The <c OMContainerElement> to be assigned.
  //   @rdesc The <c OMContainerElement> resulting from the assignment.
template <typename ObjectReference>
OMContainerElement<ObjectReference>&
OMContainerElement<ObjectReference>::operator= (
                                const OMContainerElement<ObjectReference>& rhs)
{
  TRACE("OMContainerElement<ObjectReference>::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  _reference = rhs._reference;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @parm The <c OMContainerElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
template <typename ObjectReference>
bool OMContainerElement<ObjectReference>::operator== (
                          const OMContainerElement<ObjectReference>& rhs) const
{
  TRACE("OMContainerElement<ObjectReference>::operator==");

  bool result;
  if (_reference == rhs._reference) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The contained ObjectReference.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @rdesc The contained <p ObjectReference>.
template <typename ObjectReference>
ObjectReference& OMContainerElement<ObjectReference>::reference(void)
{
  TRACE("OMContainerElement<ObjectReference>::reference");

  return _reference;
}

  // @mfunc Set the contained ObjectReference.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @parm The new contained <p ObjectReference>.
template <typename ObjectReference>
void
OMContainerElement<ObjectReference>::setReference(
                                              const ObjectReference& reference)
{
  TRACE("OMContainerElement<ObjectReference>::setReference");

  _reference = reference;
}

  // @mfunc Save this <c OMContainerElement>.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
template <typename ObjectReference>
void OMContainerElement<ObjectReference>::save(void)
{
  TRACE("OMContainerElement<ObjectReference>::save");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  _reference.save();
}

  // @mfunc Close this <c OMContainerElement>.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
template <typename ObjectReference>
void OMContainerElement<ObjectReference>::close(void)
{
  TRACE("OMContainerElement<ObjectReference>::close");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  _reference.close();
}

  // @mfunc Detach this <c OMContainerElement>.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
template <typename ObjectReference>
void OMContainerElement<ObjectReference>::detach(void)
{
  TRACE("OMContainerElement<ObjectReference>::detach");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  _reference.detach();
}

  // @mfunc Restore this <c OMContainerElement>.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
template <typename ObjectReference>
void OMContainerElement<ObjectReference>::restore(void)
{
  TRACE("OMContainerElement<ObjectReference>::restore");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  _reference.restore();
}

  // @mfunc Get the value of this <c OMContainerElement>.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ObjectReference>
OMStorable* OMContainerElement<ObjectReference>::getValue(void) const
{
  TRACE("OMContainerElement<ObjectReference>::getValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  OMStorable* result = _reference.getValue();
  return result;
}

  // @mfunc The value of this <c OMContainerElement> as a pointer.
  //        This function provides low-level access. If the object exits
  //        but has not yet been loaded then the value returned is 0.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @rdesc A pointer to the reference <c OMStorable>, if loaded.
template <typename ObjectReference>
OMStorable* OMContainerElement<ObjectReference>::pointer(void) const
{
  TRACE("OMContainerElement<ObjectReference>::pointer");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  OMStorable* result = _reference.pointer();
  return result;
}
