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

    // @cmember The persisted value of this property is its name.
    //          Write the property name and enter it into the property index.
  void saveName(void) const;

    // @cmember The persisted value of this property is its name.
    //          Read (and check) the property name.
  void restoreName(size_t size);

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

protected:
  // @access Protected members.

    // @cmember Get the value of this <c OMSimpleProperty>.
  void get(void* value, size_t valueSize) const;

    // @cmember Set the value of this <c OMSimpleProperty>.
  void set(const void* value, size_t valueSize);

  size_t _size;
  unsigned char* _bits;
};

#endif
