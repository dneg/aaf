//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMOBJECTREFERENCET_H
#define OMOBJECTREFERENCET_H

#include "OMAssertions.h"
#include "OMProperty.h"
#include "OMPropertySet.h"
#include "OMStorable.h"
#include "OMFile.h"
#include "OMPropertyTable.h"
#include "OMUtilities.h"
#include "OMStrongReferenceSet.h"

#if 1 // HACK4MEIP2
#include "OMUniqueObjectIdentType.h"
#endif

// class OMWeakObjectReference
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMWeakObjectReference

  // @mfunc Constructor.
template<typename Key>
OMWeakObjectReference<Key>::OMWeakObjectReference(void)
: OMObjectReference(),
  _identification(nullOMUniqueObjectIdentification),
  _targetTag(nullOMPropertyTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference<Key>::OMWeakObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMWeakObjectReference>.
template<typename Key>
OMWeakObjectReference<Key>::OMWeakObjectReference(OMProperty* property)
: OMObjectReference(property),
  _identification(nullOMUniqueObjectIdentification),
  _targetTag(nullOMPropertyTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference<Key>::OMWeakObjectReference");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMWeakObjectReference>.
  //   @parm The unique key of this <c OMWeakObjectReference>.
  //   @parm A tag identifying the <c OMStrongReferenceSetProperty>
  //         in which the target resides.
template<typename Key>
OMWeakObjectReference<Key>::OMWeakObjectReference(
                                   OMProperty* property,
                                   Key identification,
                                   OMPropertyTag targetTag)
: OMObjectReference(property),
  _identification(identification),
  _targetTag(targetTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference<Key>::OMWeakObjectReference");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMWeakObjectReference> to copy.
template<typename Key>
OMWeakObjectReference<Key>::OMWeakObjectReference(
                                              const OMWeakObjectReference& rhs)
: OMObjectReference(rhs),
  _identification(rhs._identification),
  _targetTag(rhs._targetTag),
  _targetSet(0)
{
  TRACE("OMWeakObjectReference<Key>::OMWeakObjectReference");
}

  // @mfunc Destructor.
template<typename Key>
OMWeakObjectReference<Key>::~OMWeakObjectReference(void)
{
}

  // @mfunc Is this <c OMWeakObjectReference> void ?
  //   @rdesc True if this <c OMWeakObjectReference> is void,
  //          false otherwise.
  //   @this const
template<typename Key>
bool OMWeakObjectReference<Key>::isVoid(void) const
{
  TRACE("OMWeakObjectReference<Key>::isVoid");
  bool result = OMObjectReference::isVoid();
  if (result) {
    if (_identification != nullOMUniqueObjectIdentification) {
      ASSERT("Valid containing property", _property != 0);
      OMFile* file = _property->propertySet()->container()->file();
      OMPropertyTable* table = file->referencedProperties();
      if (!table->isValid(_targetTag)) {
        result = true;
      } else {
        result = false;
      }
    }
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMWeakObjectReference> to be assigned.
  //   @rdesc The <c OMWeakObjectReference> resulting from the assignment.
template<typename Key>
OMWeakObjectReference<Key>&
OMWeakObjectReference<Key>::operator= (const OMWeakObjectReference& rhs)
{
  TRACE("OMWeakObjectReference<Key>::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }
  OMObjectReference::operator=(rhs);
  _identification = rhs._identification;
  _targetTag = rhs._targetTag;
  _targetSet = 0;
  return *this;
}

  // @mfunc Equality.
  //   @parm The <c OMWeakObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
  //   @this const
template<typename Key>
bool
OMWeakObjectReference<Key>::operator== (const OMWeakObjectReference& rhs) const
{
  TRACE("OMWeakObjectReference<Key>::operator==");

  return OMObjectReference::operator==(rhs) &&
         (_identification == rhs._identification);
}

  // @mfunc Save this <c OMWeakObjectReference>.
  //   @this const
template<typename Key>
void OMWeakObjectReference<Key>::save(void) const
{
  TRACE("OMWeakObjectReference<Key>::save");
  PRECONDITION("Valid identification",
                          _identification != nullOMUniqueObjectIdentification);

  // tjb nothing to do ?
}

  // @mfunc Close this <c OMWeakObjectReference>.
template<typename Key>
void OMWeakObjectReference<Key>::close(void)
{
  TRACE("OMWeakObjectReference<Key>::close");

}

  // @mfunc Detach this <c OMWeakObjectReference>.
template<typename Key>
void OMWeakObjectReference<Key>::detach(void)
{
  TRACE("OMWeakObjectReference<Key>::detach");
}

  // @mfunc Restore this <c OMWeakObjectReference>.
template<typename Key>
void OMWeakObjectReference<Key>::restore(void)
{
  TRACE("OMWeakObjectReference<Key>::restore");

  PRECONDITION("Reference not already set", _pointer == 0);
  PRECONDITION("Valid identification",
                          _identification != nullOMUniqueObjectIdentification);

  // tjb nothing to do ?

}

  // @mfunc Get the value of this <c OMWeakObjectReference>.
  //        The value is a pointer to the referenced <c OMStorable>.
  //   @rdesc  A pointer to the referenced <c OMStorable>.
  //   @this const
template<typename Key>
OMStorable* OMWeakObjectReference<Key>::getValue(void) const
{
  TRACE("OMWeakObjectReference<Key>::getValue");

  OMWeakObjectReference* nonConstThis =
                                      const_cast<OMWeakObjectReference*>(this);

  if ((_pointer == 0) &&
      (_identification != nullOMUniqueObjectIdentification)) {
    OMStorable* object = 0;
    set()->find(&nonConstThis->_identification, object);
    nonConstThis->_pointer = object;
  }
#if 1 // HACK4MEIP2
  if ((_pointer == 0) &&
      (_identification != nullOMUniqueObjectIdentification)) {
    // We failed to resolve the reference as an object id, try again as a label
    // We should only come here for KLV encoded files.
    OMUniqueObjectIdentification bid;
    bid = _identification;
    if (hostByteOrder() != bigEndian) {
	  OMUniqueObjectIdentificationType::instance()->reorder(
                                               reinterpret_cast<OMByte*>(&bid),
                                               sizeof(bid));
    }
    OMKLVKey k;
    memcpy(&k, &bid, sizeof(OMKLVKey));
    OMUniqueObjectIdentification id;
    convert(id, k);
    nonConstThis->_identification = id;
    OMStorable* object = 0;
    set()->find(&nonConstThis->_identification, object);
    nonConstThis->_pointer = object;
  }
#endif
  // If the following assertion is violated we have a dangling weak
  // reference.  The reference illegally designates an object that is
  // not present in the target set.  Code elsewhere prevents the
  // removal of objects that are weakly referenced hence a dangling
  // reference is an assertion violation rather than a run-time error.
  //
  POSTCONDITION("Object found",
                   IMPLIES(_identification != nullOMUniqueObjectIdentification,
                           _pointer != 0));
  return _pointer;
}

  // @mfunc Set the value of this <c OMWeakObjectReference>.
  //        The value is a pointer to the referenced <c OMStorable>.
  //   @parm TBS
  //   @parm A pointer to the new <c OMStorable>.
  //   @rdesc A pointer to previous <c OMStorable>, if any.
template<typename Key>
OMStorable* OMWeakObjectReference<Key>::setValue(
                            const Key& identification,
                            const OMStorable* value)
{
  TRACE("OMWeakObjectReference<Key>::setValue");

  PRECONDITION("Valid container property", _property != 0);

  ASSERT("Valid identification",
      IMPLIES(value != 0, identification != nullOMUniqueObjectIdentification));
  ASSERT("Valid identification",
      IMPLIES(value == 0, identification == nullOMUniqueObjectIdentification));

  OMStorable* oldObject = _pointer;
  _pointer = const_cast<OMStorable*>(value);
  _identification = identification;

#if defined(OM_VALIDATE_WEAK_REFERENCES)
#if 0
  ASSERT("Consistent source and target",
                    IMPLIES(_pointer != 0, set()->contains(&_identification)));
#endif
#endif

  POSTCONDITION("Element properly set", _pointer == value);
  return oldObject;
}

template<typename Key>
OMStrongReferenceSet*
OMWeakObjectReference<Key>::targetSet(const OMProperty* property,
                                 OMPropertyTag targetTag)
{
  TRACE("OMWeakObjectReference<Key>::targetSet");

  ASSERT("Valid containing property", property != 0);
  OMFile* file = property->propertySet()->container()->file();
  OMPropertyTable* table = file->referencedProperties();
  ASSERT("Valid target tag", table->isValid(targetTag));
  const OMPropertyId* targetPath = table->valueAt(targetTag);
  ASSERT("Valid target path", validPropertyPath(targetPath));

  OMProperty* set = file->findProperty(targetPath);

  OMStrongReferenceSet* result = dynamic_cast<OMStrongReferenceSet*>(set);

  POSTCONDITION("Valid result", result != 0);
  return result;
}

template<typename Key>
const Key&
OMWeakObjectReference<Key>::identification(void) const
{
  return _identification;
}

template<typename Key>
void OMWeakObjectReference<Key>::setTargetTag(OMPropertyTag targetTag)
{
  _targetTag = targetTag;
}

template<typename Key>
OMStrongReferenceSet* OMWeakObjectReference<Key>::set(void) const
{
  TRACE("OMWeakObjectReference<Key>::set");

  if (_targetSet == 0) {
    OMWeakObjectReference* nonConstThis =
                                      const_cast<OMWeakObjectReference*>(this);
    nonConstThis->_targetSet = targetSet(_property, _targetTag);
  }

  POSTCONDITION("Valid result", _targetSet != 0);
  return _targetSet;
}

#endif
