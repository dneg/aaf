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
#ifndef OMSTRONGREFPROPERTY_H
#define OMSTRONGREFPROPERTY_H

#include "OMDataTypes.h"
#include "OMProperty.h"
#include "OMStrongReference.h"
#include "OMObjectReference.h"

  // @class Persistent strong reference (contained object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMStrongReference>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMStrongReferenceProperty : public OMStrongReference {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceProperty(const OMPropertyId propertyId,
                            const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceProperty(void);

    // @cmember Get the value of this <c OMStrongReferenceProperty>.
  virtual void getValue(ReferencedObject*& object) const;

    // @cmember Set the value of this <c OMStrongReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

    // @cmember Clear the value of this <c OMStrongReferenceProperty>.
  virtual ReferencedObject* clearValue(void);

    // @cmember Assignment operator.
  OMStrongReferenceProperty<ReferencedObject>& operator =
                                              (const ReferencedObject* value);

    // @cmember Dereference operator.
  ReferencedObject* operator -> (void);

    // @cmember Dereference operator.
  const ReferencedObject* operator -> (void) const;

    // @cmember Type conversion. Convert an
    //          <c OMStrongReferenceProperty> into a pointer to the
    //          referenced (contained) <p ReferencedObject>.
  operator ReferencedObject*() const;

    // @cmember Save this <c OMStrongReferenceProperty>.
  virtual void save(void) const;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMStrongReferenceProperty>, the external
    //          (persisted) size of the <c OMStrongReferenceProperty>
    //          is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of objects contained within this
    //          <c OMStrongReferenceProperty> if any.
  virtual OMUInt64 objectCount(void) const;

  // Optional property interface

    // @cmember Is this <c OMStrongReferenceProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMStrongReferenceProperty>.
  virtual void removeProperty(void);

    // @cmember Get the raw bits of this <c OMStrongReferenceProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMStrongReferenceProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Get the value of this <c OMStrongReferenceProperty>.
  virtual OMObject* getObject(void) const;

    // @cmember set the value of this <c OMStrongReferenceProperty>.
  virtual OMObject* setObject(const OMObject* object);

    // @cmember Clear the value of this <c OMStrongReferenceProperty>.
  virtual OMObject* clearObject(void);

    // @cmember The value of this <c OMStrongReferenceProperty>
    //          as an <c OMStorable>.
  virtual OMStorable* storable(void) const;

  virtual OMStrongObjectReference& reference(void) const;

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

private:

  OMStrongObjectReference _reference;

    // OMStrongReferenceProperty can't be assigned - declare but don't define
  OMStrongReferenceProperty& operator = (const OMStrongReferenceProperty& rhs);

    // OMStrongReferenceProperty can't be copied - declare but don't define
  OMStrongReferenceProperty(const OMStrongReferenceProperty& rhs);

};

#include "OMStrongRefPropertyT.h"

#endif
