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
#ifndef OMPROPERTY_H
#define OMPROPERTY_H

#include "OMDataTypes.h"

#include <stddef.h>

// The following stored form values are used to denote the on-disk
// representation of a given property.
//
const OMStoredForm SF_DATA                                   = 0x82;
const OMStoredForm SF_DATA_STREAM                            = 0x42;
const OMStoredForm SF_STRONG_OBJECT_REFERENCE                = 0x22;
const OMStoredForm SF_STRONG_OBJECT_REFERENCE_VECTOR         = 0x32;
const OMStoredForm SF_STRONG_OBJECT_REFERENCE_SET            = 0x3A;
const OMStoredForm SF_WEAK_OBJECT_REFERENCE                  = 0x02;
const OMStoredForm SF_WEAK_OBJECT_REFERENCE_VECTOR           = 0x12;
const OMStoredForm SF_WEAK_OBJECT_REFERENCE_SET              = 0x1A;
const OMStoredForm SF_WEAK_OBJECT_REFERENCE_STORED_OBJECT_ID = 0x03;
const OMStoredForm SF_UNIQUE_OBJECT_ID                       = 0x86;
const OMStoredForm SF_OPAQUE_STREAM                          = 0x40;

class OMFile;
class OMStoredObject;
class OMStorable;
class OMPropertySet;
class OMPropertyDefinition;
class OMType;

  // @class Abstract base class for persistent properties supported by
  //        the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMProperty(const OMPropertyId propertyId,
             const OMStoredForm storedForm,
             const wchar_t* name);

    // @cmember Temporary pseudo-constructor for clients which provide
    //          a property definition.
  void initialize(const OMPropertyDefinition* definition);

    // @cmember Destructor.
  virtual ~OMProperty(void);

    // @cmember Save this <c OMProperty>.
  virtual void save(void) const = 0;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMProperty>, the external (persisted)
    //          size of the <c OMProperty> is <p externalSize>.
  virtual void restore(size_t externalSize) = 0;

    // @cmember The <c OMPropertyDefinition> defining this <c OMProperty>.
  const OMPropertyDefinition* definition(void) const;

    // @cmember The name of this <c OMProperty>.
  const wchar_t* name(void) const;

    // @cmember The property id of this <c OMProperty>.
  OMPropertyId propertyId(void) const;

    // @cmember The <c OMPropertySet> containing this <c OMProperty>.
  const OMPropertySet* propertySet(void) const;

    // @cmember Inform this <c OMProperty> that it is a member of
    //          the <c OMPropertySet> <p propertySet>.
  void setPropertySet(const OMPropertySet* propertySet);

    // @cmember The address of this <c OMProperty> object.
  OMProperty* address(void);

  // Optional property interface

    // @cmember Is this <c OMProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Is this an optional property ? 
  bool isOptional(void) const;

    // @cmember Is this optional property present ?
  bool isPresent(void) const;

    // @cmember Remove this optional <c OMProperty>.
  virtual void removeProperty(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this <c OMProperty>. The
    //          size is given in bytes.
  virtual size_t bitsSize(void) const = 0;

    // @cmember Get the raw bits of this <c OMProperty>. The raw bits
    //          are copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const = 0;

    // @cmember Set the raw bits of this <c OMProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size) = 0;

    // @cmember The value of this <c OMProperty> as an <c OMStorable>.
    //          If this <c OMProperty> does not represent an <c OMStorable>
    //          then the value returned is 0.
  virtual OMStorable* storable(void) const;

    // @cmember The stored form of this <c OMProperty>.
  OMStoredForm storedForm(void) const;

    // @cmember The type of this <c OMProperty>.
  const OMType* type(void) const;

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const = 0;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const = 0;

protected:
  // @access Protected members.

    // @cmember Set the bit that indicates that this optional <c OMProperty>
    //          is present.
  void setPresent(void);

    // @cmember Clear the bit that indicates that this optional <c OMProperty>
    //          is present.
  void clearPresent(void);

    // @cmember The <c OMStorable> that contains this <c OMProperty>.
  OMStorable* container(void) const;

    // @cmember The <c OMStoredObject> that contains the persisted
    //          representation of this <c OMProperty>.
  OMStoredObject* store(void) const;

    // @cmember The <c OMFile> that contains the persisted
    //          representation of this <c OMProperty>.
  OMFile* file(void) const;

  virtual const wchar_t* storedName(void) const;

  OMPropertyId _propertyId;
  OMStoredForm _storedForm;
  wchar_t* _storedName;
  const wchar_t* _name;

private:

  const OMPropertySet* _propertySet; // The PropertySet that contains
                                     // this property
  const OMPropertyDefinition* _definition;
  bool _isOptional;
  bool _isPresent;

    // OMProperty can't be assigned - declare but don't define
  OMProperty& operator = (const OMProperty& rhs);

    // OMProperty can't be copied - declare but don't define
  OMProperty(const OMProperty& rhs);

};

  // @class Abstract base class for simple (data) persistent
  //        properties supported by the Object Manager.
  //   @base public | <c OMProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMSimpleProperty : public OMProperty {
public:
  // @access Public members. 

    // @cmember Constructor.
  OMSimpleProperty(const OMPropertyId propertyId,
                   const wchar_t* name,
                   size_t valueSize);

    // @cmember Constructor.
  OMSimpleProperty(const OMPropertyId propertyId, const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMSimpleProperty(void);

    // @cmember Save this <c OMSimpleProperty>.
  virtual void save(void) const;

    // @cmember Restore this <c OMSimpleProperty>, the external (persisted)
    //          size of the <c OMSimpleProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The size of this <c OMSimpleProperty>.
  size_t size(void) const;

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMSimpleProperty>. The size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember The raw bits of this <c OMSimpleProperty>.
  virtual OMByte* bits(void) const;

    // @cmember Get the raw bits of this <c OMSimpleProperty>. The raw
    //          bits are copied to the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMSimpleProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Set the size of this <c OMSimpleProperty> to <p newSize> bytes.
  void setSize(size_t newSize);

  // Copying.

  virtual void shallowCopyTo(OMProperty* destination) const;

  virtual void deepCopyTo(OMProperty* destination,
                          void* clientContext) const;

protected:
  // @access Protected members.

    // @cmember Get the value of this <c OMSimpleProperty>.
  void get(void* value, size_t valueSize) const;

    // @cmember Set the value of this <c OMSimpleProperty>.
  void set(const void* value, size_t valueSize);

  size_t _size;
  unsigned char* _bits;

private:

    // OMSimpleProperty can't be assigned - declare but don't define
  OMSimpleProperty& operator = (const OMSimpleProperty& rhs);

    // OMSimpleProperty can't be copied - declare but don't define
  OMSimpleProperty(const OMSimpleProperty& rhs);

};

#endif
