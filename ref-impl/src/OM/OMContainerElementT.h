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
UniqueIdentification
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
template <typename ObjectReference>
OMStorable* OMContainerElement<ObjectReference>::pointer(void) const
{
  TRACE("OMContainerElement<ObjectReference>::pointer");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  OMStorable* result = _reference.pointer();
  return result;
}

// class OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                              OMStrongReferenceSetElement(void)
: OMStrongReferenceVectorElement(),
  _referenceCount(0xffff /* sticky */)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                "OMStrongReferenceSetElement");

  memset(&_identification, 0, sizeof(_identification));
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMStrongReferenceSetElement>.
  //   @parm The name of this <c OMStrongReferenceSetElement>.
  //   @parm The local key of this <c OMStrongReferenceSetElement> within
  //         it's set.
  //   @parm The unique key of this <c OMStrongReferenceSetElement>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                   OMStrongReferenceSetElement(
                                   OMProperty* property,
                                   const wchar_t* name,
                                   OMUInt32 localKey,
                                   OMUInt32 referenceCount,
                                   UniqueIdentification identification)
: OMStrongReferenceVectorElement(property, name, localKey),
  _identification(identification),
   _referenceCount(referenceCount)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                "OMStrongReferenceSetElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceSetElement> to copy.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                   OMStrongReferenceSetElement(
                  const OMStrongReferenceSetElement<UniqueIdentification,
                                                    ReferencedObject>& rhs)
: OMStrongReferenceVectorElement(rhs),
  _identification(rhs._identification),
  _referenceCount(rhs._referenceCount)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                "OMStrongReferenceSetElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                             ~OMStrongReferenceSetElement(void)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                               "~OMStrongReferenceSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference | The type of the contained object
  //          reference
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceSetElement> to be assigned.
  //   @rdesc The <c OMStrongReferenceSetElement> resulting from
  //          the assignment.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>&
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                                    operator= (
                  const OMStrongReferenceSetElement<UniqueIdentification,
                                                    ReferencedObject>& rhs)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                                  "operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMStrongReferenceVectorElement::operator=(rhs);
  _identification = rhs._identification;
  _referenceCount = rhs._referenceCount;

  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                                   operator== (
            const OMStrongReferenceSetElement<UniqueIdentification,
                                              ReferencedObject>& rhs) const
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                                 "operator==");

  bool result;

  if (_identification == rhs._identification) {
    result = true;
  } else {
    result = false;
  }

  ASSERT("Consistent",
                      IMPLIES(result, _referenceCount == rhs._referenceCount));
  ASSERT("Consistent",
             IMPLIES(result, OMStrongReferenceVectorElement::operator==(rhs)));

  return result;
}

  // @mfunc Set the value of this <c OMContainerElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
template <typename UniqueIdentification, typename ReferencedObject>
ReferencedObject*
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::setValue(
                                                 const ReferencedObject* value)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                                   "setValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  OMStorable* p = _reference.setValue(value);
  ReferencedObject* result = 0;
  if (p != 0) {
    result = dynamic_cast<ReferencedObject*>(p);
    ASSERT("Object is correct type", result != 0);
  } else {
    result = 0;
  }
  return result;
}

  // @mfunc The unique key of this <c OMStrongReferenceSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc  The unique key of this <c OMStrongReferenceSetElement>.
template <typename UniqueIdentification, typename ReferencedObject>
UniqueIdentification
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                     identification(void) const
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                             "identification");

  return _identification;
}

  // @mfunc The count of weak references to this
  //        <c OMStrongReferenceSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc The count of weak references.
template <typename UniqueIdentification, typename ReferencedObject>
OMUInt32
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                     referenceCount(void) const
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                             "referenceCount");

  return _referenceCount;
}
