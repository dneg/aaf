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

// @doc OMEXTERNAL
#ifndef OMWEAKREFPROPERTY_H
#define OMWEAKREFPROPERTY_H

#include "OMDataTypes.h"
#include "OMProperty.h"
#include "OMWeakReference.h"
#include "OMObjectReference.h"

  // @class Persistent weak reference (pointer to shared object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMWeakReference>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakReferenceProperty : public OMWeakReference {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceProperty(const OMPropertyId propertyId,
                          const wchar_t* name,
                          const wchar_t* targetName,
                          const OMPropertyId keyPropertyId);

    // @cmember Constructor.
  OMWeakReferenceProperty(const OMPropertyId propertyId,
                          const wchar_t* name,
                          const OMPropertyId keyPropertyId,
                          const OMPropertyId* targetPropertyPath);

    // @cmember Destructor.
  virtual ~OMWeakReferenceProperty(void);

    // @cmember Get the value of this <c OMWeakReferenceProperty>.
  virtual void getValue(ReferencedObject*& object) const;

    // @cmember set the value of this <c OMWeakReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

    // @cmember Clear the value of this <c OMWeakReferenceProperty>.
  virtual ReferencedObject* clearValue(void);

    // @cmember Assignment operator.
  OMWeakReferenceProperty<ReferencedObject>& operator =
                                              (const ReferencedObject* value);

    // @cmember Dereference operator.
  ReferencedObject* operator -> (void);

    // @cmember Dereference operator.
  const ReferencedObject* operator -> (void) const;

    // @cmember Type conversion. Convert an
    //          <c OMWeakReferenceProperty> into a pointer to the
    //          referenced (pointed to) <p ReferencedObject>.
  operator ReferencedObject*() const;

    // @cmember Save this <c OMWeakReferenceProperty>.
  virtual void save(void) const;

    // @cmember close this <c OMWeakReferenceProperty>.
  virtual void close(void);

    // @cmember The number of objects contained within this
    //          <c OMWeakReferenceProperty> if any.
  virtual OMUInt64 objectCount(void) const;

    // @cmember Restore this <c OMWeakReferenceProperty>, the external
    //          (persisted) size of the <c OMWeakReferenceProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember Is this <c OMWeakReferenceProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Get the raw bits of this <c OMWeakReferenceProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMWeakReferenceProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Get the value of this <c OMWeakReferenceProperty>.
  virtual OMObject* getObject(void) const;

    // @cmember set the value of this <c OMWeakReferenceProperty>.
  virtual OMObject* setObject(const OMObject* object);

  virtual OMWeakObjectReference& reference(void) const;

    // @cmember The <c OMStrongReferenceSet> in which the object
    //          referenced by this <c OMWeakReferenceProperty>
    //          must reside.
  virtual OMStrongReferenceSet* targetSet(void) const;

  virtual OMPropertyId keyPropertyId(void) const;

  virtual OMPropertyTag targetTag(void) const;

  virtual void setTargetTag(OMPropertyTag targetTag);

  virtual void clearTargetTag(void) const;

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

private:

  OMPropertyId* targetPropertyPath(void) const;

  OMWeakObjectReference _reference;
  OMPropertyTag _targetTag;
  const wchar_t* _targetName;
  OMPropertyId* _targetPropertyPath;
  OMPropertyId _keyPropertyId;
  OMStrongReferenceSet* _targetSet;

    // OMWeakReferenceProperty can't be assigned - declare but don't define
  OMWeakReferenceProperty& operator = (const OMWeakReferenceProperty& rhs);

    // OMWeakReferenceProperty can't be copied - declare but don't define
  OMWeakReferenceProperty(const OMWeakReferenceProperty& rhs);

};

#include "OMWeakRefPropertyT.h"

#endif
