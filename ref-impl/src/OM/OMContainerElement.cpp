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
#include "OMContainerElement.h"

#include "OMAssertions.h"

// class OMStrongReferenceVectorElement
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMStrongReferenceVectorElement

  // @mfunc Constructor.
OMStrongReferenceVectorElement::OMStrongReferenceVectorElement(void)
: OMContainerElement<OMStrongObjectReference>(),
  _localKey(0)
{
  TRACE("OMStrongReferenceVectorElement::OMStrongReferenceVectorElement");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> (a vector property) that contains this
  //         <c OMStrongReferenceVectorElement>.
  //   @parm The name of this <c OMStrongReferenceVectorElement>.
  //   @parm The local key of this <c OMStrongReferenceVectorElement> within
  //         it's vector.
OMStrongReferenceVectorElement::OMStrongReferenceVectorElement(
                                                          OMProperty* property,
                                                          const wchar_t* name,
                                                          OMUInt32 localKey)
: OMContainerElement<OMStrongObjectReference>(
                                      OMStrongObjectReference(property, name)),
  _localKey(localKey)
{
  TRACE("OMStrongReferenceVectorElement::OMStrongReferenceVectorElement");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMStrongReferenceVectorElement> to copy.
OMStrongReferenceVectorElement::OMStrongReferenceVectorElement(
                                     const OMStrongReferenceVectorElement& rhs)
: OMContainerElement<OMStrongObjectReference>(rhs),
  _localKey(rhs._localKey)
{
  TRACE("OMStrongReferenceVectorElement::OMStrongReferenceVectorElement");
}

  // @mfunc Destructor.
OMStrongReferenceVectorElement::~OMStrongReferenceVectorElement(void)
{
  TRACE("OMStrongReferenceVectorElement::~OMStrongReferenceVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMStrongReferenceVectorElement> to be assigned.
  //   @rdesc The <c OMStrongReferenceVectorElement> resulting from
  //          the assignment.
OMStrongReferenceVectorElement&
OMStrongReferenceVectorElement::operator= (
                                     const OMStrongReferenceVectorElement& rhs)
{
  TRACE("OMStrongReferenceVectorElement::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMContainerElement<OMStrongObjectReference>::operator=(rhs);
  _localKey = rhs._localKey;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMStrongReferenceVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
bool OMStrongReferenceVectorElement::operator== (
                               const OMStrongReferenceVectorElement& rhs) const
{
  TRACE("OMStrongReferenceVectorElement::operator==");

  bool result = OMContainerElement<OMStrongObjectReference>::operator==(rhs);
  if (result) {
    if (_localKey != rhs._localKey) {
      result = false;
    }
  }
  return result;
}

  // @mfunc Set the value of this <c OMOMStrongReferenceVectorElement>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
OMStorable* OMStrongReferenceVectorElement::setValue(const OMStorable* value)
{
  TRACE("OMStrongReferenceVectorElement::setValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  OMStorable* result = _reference.setValue(value);
  return result;
}

  // @mfunc The local key of this <c OMStrongReferenceVectorElement>.
  //   @rdesc The local key of this <c OMStrongReferenceVectorElement>.
  //   @this const
OMUInt32 OMStrongReferenceVectorElement::localKey(void) const
{
  TRACE("OMStrongReferenceVectorElement::localKey");

  return  _localKey;
}

// class OMStrongReferenceSetElement
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMStrongReferenceSetElement

  // @mfunc Constructor.
OMStrongReferenceSetElement::OMStrongReferenceSetElement(void)
: OMStrongReferenceVectorElement(),
  _identification(0),
  _identificationSize(0),
  _referenceCount(0xffff /* sticky */)
{
  TRACE("OMStrongReferenceSetElement::OMStrongReferenceSetElement");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMStrongReferenceSetElement>.
  //   @parm The name of this <c OMStrongReferenceSetElement>.
  //   @parm The local key of this <c OMStrongReferenceSetElement> within
  //         it's set.
  //   @parm The unique key of this <c OMStrongReferenceSetElement>.
  //   @parm TBS
OMStrongReferenceSetElement::OMStrongReferenceSetElement(
                                                     OMProperty* property,
                                                     const wchar_t* name,
                                                     OMUInt32 localKey,
                                                     OMUInt32 referenceCount,
                                                     void* identification,
                                                     size_t identificationSize)
: OMStrongReferenceVectorElement(property, name, localKey),
  _identification(0),
  _identificationSize(identificationSize),
  _referenceCount(referenceCount)
{
  TRACE("OMStrongReferenceSetElement::OMStrongReferenceSetElement");

  _identification = new OMByte[identificationSize];
  ASSERT("Valid heap pointer", _identification != 0);
  memcpy(_identification, identification, _identificationSize);
}

  // @mfunc Copy constructor.
  //   @parm The <c OMStrongReferenceSetElement> to copy.
OMStrongReferenceSetElement::OMStrongReferenceSetElement(
                                        const OMStrongReferenceSetElement& rhs)
: OMStrongReferenceVectorElement(rhs),
  _identification(0),
  _identificationSize(rhs._identificationSize),
  _referenceCount(rhs._referenceCount)
{
  TRACE("OMStrongReferenceSetElement::OMStrongReferenceSetElement");

  delete [] reinterpret_cast<OMByte*>(_identification);
  _identification = new OMByte[_identificationSize];
  ASSERT("Valid heap pointer", _identification != 0);
  memcpy(_identification, rhs._identification, _identificationSize);
}

  // @mfunc Destructor.
OMStrongReferenceSetElement::~OMStrongReferenceSetElement(void)
{
  TRACE("OMStrongReferenceSetElement::~OMStrongReferenceSetElement");
  delete [] reinterpret_cast<OMByte*>(_identification);
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMStrongReferenceSetElement> to be assigned.
  //   @rdesc The <c OMStrongReferenceSetElement> resulting from
  //          the assignment.
OMStrongReferenceSetElement&
OMStrongReferenceSetElement::operator= (const OMStrongReferenceSetElement& rhs)
{
  TRACE("OMStrongReferenceSetElement::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMStrongReferenceVectorElement::operator=(rhs);
  _identificationSize = rhs._identificationSize;
  delete [] reinterpret_cast<OMByte*>(_identification);
  _identification = 0; // for BoundsChecker
  _identification = new OMByte[_identificationSize];
  ASSERT("Valid heap pointer", _identification != 0);
  memcpy(_identification, rhs._identification, _identificationSize);
  _referenceCount = rhs._referenceCount;

  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMStrongReferenceSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
bool OMStrongReferenceSetElement::operator== (
                                  const OMStrongReferenceSetElement& rhs) const
{
  TRACE("OMStrongReferenceSetElement::operator==");

  bool result;

  if ((_identification != 0) && (rhs._identification != 0)) {
    if (memcmp(_identification,
               rhs._identification,
               _identificationSize) == 0) {
      result = true;
    } else {
      result = false;
    }
  } else if ((_identification == 0) && (rhs._identification == 0)) {
    result = true;
  } else {
    result = false;
  }


  ASSERT("Consistent",
                      IMPLIES(result, _referenceCount == rhs._referenceCount));

#if defined (OM_DEBUG)
  bool check = OMStrongReferenceVectorElement::operator==(rhs);
#endif
  ASSERT("Consistent", IMPLIES(result, check));

  return result;
}

  // @mfunc Set the value of this <c OMContainerElement>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
OMStorable* OMStrongReferenceSetElement::setValue(void* identification,
                                                  const OMStorable* value)
{
  TRACE("OMStrongReferenceSetElement::setValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  delete [] reinterpret_cast<OMByte*>(_identification);
  _identification = 0 ; // for BoundsChecker
  _identification = new OMByte[_identificationSize];
  ASSERT("Valid heap pointer", _identification != 0);
  memcpy(_identification, identification, _identificationSize);
  return _reference.setValue(value);
}

  // @mfunc The unique key of this <c OMStrongReferenceSetElement>.
  //   @rdesc  The unique key of this <c OMStrongReferenceSetElement>.
void* OMStrongReferenceSetElement::identification(void) const
{
  TRACE("OMStrongReferenceSetElement::identification");

  return _identification;
}

  // @mfunc The count of weak references to this
  //        <c OMStrongReferenceSetElement>.
  //   @rdesc The count of weak references.
OMUInt32 OMStrongReferenceSetElement::referenceCount(void) const
{
  TRACE("OMStrongReferenceSetElement::referenceCount");

  return _referenceCount;
}

// class OMWeakReferenceVectorElement
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMWeakReferenceVectorElement

  // @mfunc Constructor.
OMWeakReferenceVectorElement::OMWeakReferenceVectorElement(void)
: OMContainerElement<OMWeakObjectReference>()
{
  TRACE("OMWeakReferenceVectorElement::OMWeakReferenceVectorElement");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMWeakReferenceVectorElement>.
  //   @parm The unique key of this <c OMWeakReferenceVectorElement>.
  //   @parm A tag identifying the <c OMStrongReferenceVectorProperty>
  //         in which the target resides.
OMWeakReferenceVectorElement::OMWeakReferenceVectorElement(
                                   OMProperty* property,
                                   OMUniqueObjectIdentification identification,
                                   OMPropertyTag targetTag)
: OMContainerElement<OMWeakObjectReference>(
  OMWeakObjectReference(property, identification, targetTag))
{
  TRACE("OMWeakReferenceVectorElement::OMWeakReferenceVectorElement");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMWeakReferenceVectorElement> to copy.
OMWeakReferenceVectorElement::OMWeakReferenceVectorElement(
                                       const OMWeakReferenceVectorElement& rhs)
: OMContainerElement<OMWeakObjectReference>(rhs)
{
  TRACE("OMWeakReferenceVectorElement::OMWeakReferenceVectorElement");
}

  // @mfunc Destructor.
OMWeakReferenceVectorElement::~OMWeakReferenceVectorElement(void)
{
  TRACE("OMWeakReferenceVectorElement::~OMWeakReferenceVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMWeakReferenceVectorElement> to be assigned.
  //   @rdesc The <c OMWeakReferenceVectorElement> resulting from
  //          the assignment.
OMWeakReferenceVectorElement&
OMWeakReferenceVectorElement::operator= (
                                       const OMWeakReferenceVectorElement& rhs)
{
  TRACE("OMWeakReferenceVectorElement::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMContainerElement<OMWeakObjectReference>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMWeakReferenceVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
bool OMWeakReferenceVectorElement::operator== (
                                 const OMWeakReferenceVectorElement& rhs) const
{
  TRACE("OMWeakReferenceVectorElement::operator==");

  bool result = OMContainerElement<OMWeakObjectReference>::operator==(rhs);

  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceVectorElement>.
  //   @parm TBS
  //   @parm A pointer to the new <c OMStorable>.
  //   @rdesc A pointer to previous <c OMStorable>, if any.
OMStorable* OMWeakReferenceVectorElement::setValue(
                            const OMUniqueObjectIdentification& identification,
                            const OMStorable* value)
{
  TRACE("OMWeakReferenceVectorElement::setValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  return _reference.setValue(identification, value);
}

  // @mfunc The unique key of this <c OMWeakReferenceVectorElement>.
  //   @rdesc  The unique key of this <c OMWeakReferenceVectorElement>.
OMUniqueObjectIdentification
OMWeakReferenceVectorElement::identification(void) const
{
  TRACE("OMWeakReferenceVectorElement::identification");

  return _reference.identification();
}

// class OMWeakReferenceSetElement
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMWeakReferenceSetElement

  // @mfunc Constructor.
OMWeakReferenceSetElement::OMWeakReferenceSetElement(void)
: OMContainerElement<OMWeakObjectReference>()
{
  TRACE("OMWeakReferenceSetElement::OMWeakReferenceSetElement");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMWeakReferenceSetElement>.
  //   @parm The unique key of this <c OMWeakReferenceSetElement>.
  //   @parm A tag identifying the <c OMStrongReferenceSetProperty>
  //         in which the target resides.
OMWeakReferenceSetElement::OMWeakReferenceSetElement(
                                   OMProperty* property,
                                   OMUniqueObjectIdentification identification,
                                   OMPropertyTag targetTag)
: OMContainerElement<OMWeakObjectReference>(
  OMWeakObjectReference(property, identification, targetTag))
{
  TRACE("OMWeakReferenceSetElement::OMWeakReferenceSetElement");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMWeakReferenceSetElement> to copy.
OMWeakReferenceSetElement::OMWeakReferenceSetElement(
                                          const OMWeakReferenceSetElement& rhs)
: OMContainerElement<OMWeakObjectReference>(rhs)
{
  TRACE("OMWeakReferenceSetElement::OMWeakReferenceSetElement");
}

  // @mfunc Destructor.
OMWeakReferenceSetElement::~OMWeakReferenceSetElement(void)
{
  TRACE("OMWeakReferenceSetElement::~OMWeakReferenceSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMWeakReferenceSetElement> to be assigned.
  //   @rdesc The <c OMWeakReferenceSetElement> resulting from
  //          the assignment.
OMWeakReferenceSetElement&
OMWeakReferenceSetElement::operator= (const OMWeakReferenceSetElement& rhs)
{
  TRACE("OMWeakReferenceSetElement::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMContainerElement<OMWeakObjectReference>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMWeakReferenceSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
bool OMWeakReferenceSetElement::operator== (
                                    const OMWeakReferenceSetElement& rhs) const
{
  TRACE("OMWeakReferenceSetElement::operator==");

  bool result = OMContainerElement<OMWeakObjectReference>::operator==(rhs);

  return result;
}

  // @mfunc Set the value of this <c OMWeakReferenceSetElement>.
  //   @parm TBS
  //   @parm A pointer to the new <c OMStorable>.
  //   @rdesc A pointer to previous <c OMStorable>, if any.
OMStorable* OMWeakReferenceSetElement::setValue(
                            const OMUniqueObjectIdentification& identification,
                            const OMStorable* value)
{
  TRACE("OMWeakReferenceSetElement::setValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  return _reference.setValue(identification, value);
}

  // @mfunc The unique key of this <c OMWeakReferenceSetElement>.
  //   @rdesc  The unique key of this <c OMWeakReferenceSetElement>.
OMUniqueObjectIdentification
OMWeakReferenceSetElement::identification(void) const
{
  TRACE("OMWeakReferenceSetElement::identification");

  return _reference.identification();
}
