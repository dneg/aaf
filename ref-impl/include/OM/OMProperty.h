// @doc OMEXTERNAL
#ifndef OMPROPERTY_H
#define OMPROPERTY_H

#include "OMPortability.h"
#include "OMDataTypes.h"

#include <stddef.h>

// The following stored form values are used to denote the on-disk
// representation of a given property.
//
const int SF_DATA                           = 0;
const int SF_DATA_STREAM                    = 1;
const int SF_STRONG_OBJECT_REFERENCE        = 2;
const int SF_STRONG_OBJECT_REFERENCE_VECTOR = 3;
const int SF_STRONG_OBJECT_REFERENCE_SET    = 4;
const int SF_WEAK_OBJECT_REFERENCE          = 5;
const int SF_WEAK_OBJECT_REFERENCE_VECTOR   = 6;
const int SF_WEAK_OBJECT_REFERENCE_SET      = 7;


class OMStoredObject;
class OMStorable;
class OMPropertySet;
class OMType;

  // @class Abstract base class for persistent properties supported by
  //        the Object Manager.
class OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMProperty(const OMPropertyId propertyId,
             const int storedForm,
             const char* name);

    // Temporary pseudo-constructor.
  void initialize(const OMPropertyId propertyId,
                  const char* name,
                  OMType* type,
                  const bool isOptional = false);

    // @cmember Destructor.
  virtual ~OMProperty(void);

    // @cmember Save this <c OMProperty>.
    // @this const
  virtual void save(void) const = 0;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Restore this <c OMProperty>, the external (persisted)
    //          size of the <c OMProperty> is <p externalSize>.
  virtual void restore(size_t externalSize) = 0;

    // @cmember The name of this <c OMProperty>.
    // @this const 
  const char* name(void) const;

    // @cmember The property id of this <c OMProperty>.
    // @this const
  OMPropertyId propertyId(void) const;

    // @cmember Inform this <c OMProperty> that it is a member of
    //          the <c OMPropertySet> <p propertySet>.
  void setPropertySet(const OMPropertySet* propertySet);

    // @cmember The address of this <c OMProperty> object.
  OMProperty* address(void);

    // @cmember Detach the <c OMStorable> object with the given
    //          <p key> from this <c OMProperty>. This <c OMProperty>
    //          must no longer attempt to access the <c OMStorable> with
    //          the given <p key>.
  virtual void detach(const OMStorable* object, const size_t key);

  // Direct property access interface

    // @cmember The size of the raw bits of this <c OMProperty>. The
    //          size is given in bytes.
  virtual size_t bitsSize(void) const = 0;

    // @cmember Get the raw bits of this <c OMProperty>. The raw bits
    //          are copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const = 0;

protected:

  int _propertyId;
  int _storedForm; // The on-disk representation used (one of the SF_* values)
  const char* _name;
  const OMPropertySet* _propertySet; // The PropertySet that contains
                                     // this property
  const OMType* _type;

};

// @doc OMINTERNAL

  // @class Abstract base class for persistent reference properties
  //        supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMProperty>
template <typename ReferencedObject>
class OMReferenceProperty : public OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMReferenceProperty(const OMPropertyId propertyId,
                      const int storedForm,
                      const char* name);

    // @cmember Destructor.
  virtual ~OMReferenceProperty(void);

    // @cmember Set the value of this <c OMReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

    // @cmember Get the value of this <c OMReferenceProperty>.
    //   @this const
  virtual void getValue(ReferencedObject*& object) const = 0;

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMReferenceProperty>. The size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMReferenceProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

protected:
  // @access Protected members.

    // @cmember Load the persisted representation of this
    //          <c OMReferenceProperty> into memory.
  virtual void load(void) = 0;

    // @cmember Is the persisted representation of this
    //          <c OMReferenceProperty> loaded ?
    // @this const
  virtual bool isLoaded(void) const;

    // @cmember Set the bit that indicates that the persisted
    //          representation of this <c OMReferenceProperty> is loaded.
  virtual void setLoaded(void);

    // @cmember Clear the bit that indicates that the persisted
    //          representation of this <c OMReferenceProperty> is loaded.
  virtual void clearLoaded(void);

  virtual ReferencedObject* pointer(void) const;

  ReferencedObject* _pointer; // The referenced object

private:

    // False if a persisted representation exists that has not yet
    // been loaded, true otherwise.
    //
  bool _loaded;

};

  // @class Persistent strong reference (contained object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMReferenceProperty>
template <typename ReferencedObject>
class OMStrongReferenceProperty :
                                 public OMReferenceProperty<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceProperty(void);

    // @cmember Get the value of this <c OMStrongReferenceProperty>.
    //   @this const
  virtual void getValue(ReferencedObject*& object) const;

    // @cmember Set the value of this <c OMStrongReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

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
    //   @this const
  operator ReferencedObject*(void) const;

    // @cmember Save this <c OMStrongReferenceProperty>.
    // @this const
  virtual void save(void) const;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Restore this <c OMStrongReferenceProperty>, the external
    //          (persisted) size of the <c OMStrongReferenceProperty>
    //          is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember Detach the <c OMStorable> object with the given
    //          <p key> from this <c OMStrongReferenceProperty>. This
    //          <c OMStrongReferenceProperty> must no longer attempt
    //          to access the <c OMStorable> with the given <p key>.
  virtual void detach(const OMStorable* object, const size_t key);

protected:
  // @access Protected members.

    // @cmember Load the persisted representation of this
    //          <c OMStrongReferenceProperty> into memory.
  virtual void load(void);

private:

    // The name of the storage containing the persisted representation
    // of the referenced object.
    //
  char* _storageName;

};

  // @class Persistent weak reference (pointer to shared object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMReferenceProperty>
template <typename ReferencedObject>
class OMWeakReferenceProperty : public OMReferenceProperty<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMWeakReferenceProperty(void);

    // @cmember Get the value of this <c OMWeakReferenceProperty>.
    //   @this const
  virtual void getValue(ReferencedObject*& object) const;

    // @cmember set the value of this <c OMWeakReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

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
    //   @this const
  operator ReferencedObject*(void) const;

    // @cmember Save this <c OMWeakReferenceProperty>.
    // @this const
  virtual void save(void) const;

    // @cmember close this <c OMWeakReferenceProperty>.
  virtual void close(void);

    // @cmember Restore this <c OMWeakReferenceProperty>, the external
    //          (persisted) size of the <c OMWeakReferenceProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember Detach the <c OMStorable> object with the given
    //          <p key> from this <c OMWeakReferenceProperty>. This
    //          <c OMWeakReferenceProperty> must no longer attempt
    //          to access the <c OMStorable> with the given <p key>.
  virtual void detach(const OMStorable* object, const size_t key);

protected:
  // @access Protected members.

    // @cmember Load the persisted representation of this
    //          <c OMWeakReferenceProperty> into memory.
  virtual void load(void);

private:
  char* _pathName;
};

  // @class Abstract base class for simple (data) persistent
  //        properties supported by the Object Manager.
  //   @base public | <c OMProperty>
class OMSimpleProperty : public OMProperty {
public:
  // @access Public members. 

    // @cmember Constructor.
  OMSimpleProperty(const OMPropertyId propertyId,
                   const char* name,
                   size_t valueSize);

    // @cmember Constructor.
  OMSimpleProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMSimpleProperty(void);

    // @cmember Save this <c OMSimpleProperty>.
    //   @this const
  virtual void save(void) const;

    // @cmember Restore this <c OMSimpleProperty>, the external (persisted)
    //          size of the <c OMSimpleProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The size of this <c OMSimpleProperty>.
    //   @this const
  size_t size(void) const;

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMSimpleProperty>. The size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this <c OMSimpleProperty>. The raw
    //          bits are copied to the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

protected:
  void get(void* value, size_t valueSize) const;
  void set(const void* value, size_t valueSize);

  size_t _size;
  unsigned char* _bits;
};

  // @class Fixed size simple (data) persistent
  //        properties supported by the Object Manager.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @base public | <c OMSimpleProperty>
template <typename PropertyType>
class OMFixedSizeProperty : public OMSimpleProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMFixedSizeProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMFixedSizeProperty(void);

    // @cmember Get the value of this <c OMFixedSizeProperty>.
    //   @this const
  void getValue(PropertyType& value) const;

    // @cmember Set the value of this <c OMFixedSizeProperty>.
  void setValue(const PropertyType& value);

    // @cmember Assignment operator.
  OMFixedSizeProperty<PropertyType>& operator = (const PropertyType& value);

    // @cmember Type conversion. Convert an <c OMFixedSizeProperty>
    //          into a <p PropertyType>.
    //   @this const
  operator PropertyType(void) const;

    // @cmember "Address of" operator.
  PropertyType* operator &(void);

    // @cmember Restore this <c OMFixedSizeProperty>, the external
    //          (persisted) size of the <c OMFixedSizeProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

};

  // @class Variable size simple (data) persistent
  //        properties supported by the Object Manager.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @base public | <c OMSimpleProperty>
template <typename PropertyType>
class OMVariableSizeProperty : public OMSimpleProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVariableSizeProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMVariableSizeProperty(void);

    // @cmember Get the value of this <c OMVariableSizeProperty>.
    //   @this const
  void getValue(PropertyType* value, size_t valueSize) const;

    // @cmember Set the value of this <c OMVariableSizeProperty>. The
    //          value is set by copying <p valueSize> bytes from the
    //          address <p value> into the <c OMVariableSizeProperty>.
  void setValue(const PropertyType* value, size_t valueSize);

    // @cmember Get the value of the item at position <p index> in this
    //          <c OMVariableSizeProperty>. The value is obtained by copying
    //          a single item of type PropertyType from this
    //          <c OMVariableSizeProperty> at position <p index>.
    //   @this const
  void getValueAt(PropertyType* value, const size_t index) const;

    // @cmember Set the value of the item at position <p index> in this
    //          <c OMVariableSizeProperty>. The value is set by copying
    //          a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at position <p index>.
  void setValueAt(const PropertyType* value, const size_t index);

    // @cmember Get the value of this <c OMvariableSizeProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMVariableSizeProperty>. The buffer is at address
    //          <p buffer> and is <p bufferSize> bytes in size.
    //          Copying only takes place if the buffer is large enough.
    //   @this const
  bool copyToBuffer(PropertyType* buffer, size_t bufferSize) const;

    // @cmember Restore this <c OMVariableSizeProperty>, the external
    //          (persisted) size of the <c OMVariableSizeProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of items in this this <c OMVariableSizeProperty>.
    //   @this const 
  size_t count(void) const;

};

  // @class Abstract base class for persistent collection properties
  //        supported by the Object Manager.
  //   @base public | <c OMProperty>
class OMCollectionProperty : public OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMCollectionProperty(const OMPropertyId propertyId,
                       const int storedForm,
                       const char* name);

    // @cmember Destructor.
  virtual ~OMCollectionProperty(void);

};

  // @class Persistent collections of strong reference (contained object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMCollectionProperty>
template <typename ReferencedObject>
class OMStrongReferenceVectorProperty : public OMCollectionProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceVectorProperty(const OMPropertyId propertyId,
                                  const char* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceVectorProperty(void);

    // @cmember Save this <c OMStrongReferenceVectorProperty>.
    // @this const
  virtual void save(void) const;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Restore this <c OMStrongReferenceVectorProperty>, the
    //          external (persisted) size of the
    //          <c OMStrongReferenceVectorProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember Get the size of this <c OMStrongReferenceVectorProperty>.
    //   @this const
  void getSize(size_t& size) const;

    // @cmember Get the size of this <c OMStrongReferenceVectorProperty>.
    //   @this const
  size_t getSize(void) const;

    // @cmember Set the value of the <p OMReferencedObject> at
    //          position <p index> in this
    //          <c OMStrongReferenceVectorProperty>.
  ReferencedObject* setValueAt(const ReferencedObject* value,
                               const size_t index);

    // @cmember Get the value of the <p OMReferencedObject> at
    //          position <p index> in this
    //          <c OMStrongReferenceVectorProperty>.
    //   @this const
  void getValueAt(ReferencedObject*& value, const size_t index) const;

    // @cmember Appened the given <p OMReferencedObject> <p value> to
    //          this <c OMStrongReferenceVectorProperty>.
  void appendValue(const ReferencedObject*& value);

    // @cmember Detach the <c OMStorable> object with the given
    //          <p key> from this <c OMStrongReferenceVectorProperty>.
    //          This <c OMStrongReferenceVectorProperty> must no longer
    //          attempt to access the <c OMStorable> with the given <p key>.
  virtual void detach(const OMStorable* object, const size_t key);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMStrongReferenceVectorProperty>. The size is given
    //          in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMStrongReferenceVectorProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

protected:
  // @access Protected members.

    // @cmember Load the persisted representation of the element of
    //          this <c OMStrongReferenceVectorProperty> given by <p index>
    //          into memory.
  virtual void loadElement(const size_t index);

    // @cmember Is the persisted representation of the element of this
    //          <c OMStrongReferenceVectorProperty> given by <p index>
    //          loaded ?
    // @this const
  virtual bool isElementLoaded(const size_t index) const;

    // @cmember Set the bit that indicates that the persisted
    //          representation of the element of this
    //          <c OMStrongReferenceVectorProperty> given by <p index>
    //          is loaded.
  virtual void setElementLoaded(const size_t index);

    // @cmember Clear the bit that indicates that the persisted
    //          representation of the element of this
    //          <c OMStrongReferenceVectorProperty> given by <p index>
    //          is loaded.
  virtual void clearElementLoaded(const size_t index);

    // @cmember The key of the element of this
    //          <c OMStrongReferenceVectorProperty> given by <p index>.
    // @this const
  virtual OMUInt32 elementKey(const size_t key) const;

    // @cmember Set the key of the element of this
    //          <c OMStrongReferenceVectorProperty> given by <p index>
    //          to <p key>.
  virtual void setElementKey(const size_t index, const OMUInt32 key);

private:

  void grow(size_t additionalElements);

  static char* elementName(const char* vectorPropertyName, size_t vectorIndex);

  const ReferencedObject** _vector;
  size_t _sizeOfVector; // Actual size
  size_t _size;         // Number of elements in use

    // _loaded[i] is false if a persisted representation of _vector[i]
    // exists that has not yet been loaded, true otherwise.
    //
  bool* _loaded;

    // _keys[i] is the key for _vector[i].
    //
  OMUInt32* _keys;

    // The name of the index describing the persisted representation
    // of _vector. Also the prefix for the storages containing the
    // persisted representation of the objects within _vector.
    //
  char* _propertyName;

};

  // @class Abstract base class for persistent character string
  //        properties supported by the Object Manager.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @base public | <c OMVariableSizeProperty>
template <typename CharacterType>
class OMCharacterStringProperty :
                                 public OMVariableSizeProperty<CharacterType> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMCharacterStringProperty(const OMPropertyId propertyId, const char * name);

    // @cmember Destructor.
  virtual ~OMCharacterStringProperty(void);

    // @cmember Type conversion. Convert an
    //          <c OMCharacterStringProperty> into a
    //          string of <p CharacterType> characters.
  operator const CharacterType* (void);

    // @cmember Type conversion. Convert an
    //          <c OMCharacterStringProperty> into a
    //          string of <p CharacterType> characters.
    //   @this const
  operator const CharacterType* (void) const;

    // @cmember Assign the string <p characterString> to this
    //          <c OMCharacterStringProperty>.
  void assign(const CharacterType* characterString);

    // @cmember The length of this <c OMCharacterStringProperty> in
    //          characters (not counting the null terminating character).
    //   @this const
  size_t length(void) const;

    // @cmember Utility function for computing the length, in
    //          characters, of the string of <p CharacterType>
    //          characters <p characterString>.
  static size_t stringLength(const CharacterType* characterString);

private:
  // hide, declare but don't define
  operator CharacterType* (void);
};

  // @class Persistent character strings supported by the Object Manager.
  //   @base public | <c OMCharacterStringProperty>
class OMStringProperty : public OMCharacterStringProperty<char> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStringProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMStringProperty(void);

    // @cmember Assignment operator.
  OMStringProperty& operator = (const char* value);

};

  // @class Persistent wide character strings supported by the Object Manager.
  //   @base public | <c OMCharacterStringProperty>
class OMWideStringProperty : public OMCharacterStringProperty<wchar_t> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWideStringProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMWideStringProperty(void);

    // @cmember Assignment operator.
  OMWideStringProperty& operator = (const wchar_t* value);

};

#include "OMPropertyT.h"

#endif
