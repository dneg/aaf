// @doc OMEXTERNAL
#ifndef OMPROPERTY_H
#define OMPROPERTY_H

#include "OMPortability.h"
#include "OMTypes.h"

#include <stddef.h>

const int TID_DATA                           = 0;
const int TID_STRONG_OBJECT_REFERENCE        = 1;
const int TID_STRONG_OBJECT_REFERENCE_VECTOR = 2;
const int TID_WEAK_OBJECT_REFERENCE          = 3;
const int TID_DATA_STREAM                    = 4;

class OMStoredObject;
class OMStorable;
class OMPropertySet;

  // @class Abstract base class for persistent properties supported by
  //        the Object Manager.
class OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMProperty(const OMPropertyId propertyId, const int type, const char* name);

    // @cmember Destructor.
  virtual ~OMProperty(void);

    // @cmember Save this <c OMProperty> to the <c OMStoredObject>
    //          <p s>.
    // @this const
  virtual void saveTo(OMStoredObject& s) const = 0;

    // @cmember Restore this <c OMProperty> from the
    //          <c OMStoredObject> <p s>, the size of the <c OMProperty>
    //          is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size) = 0;

    // @cmember The name of this <c OMProperty>.
    // @this const 
  const char* name(void) const;

    // @cmember The property id of this <c OMProperty>.
    // @this const
  const OMPropertyId propertyId(void) const;

    // @cmember Inform this <c OMProperty> that it is a member of
    //          the <c OMPropertySet> <p propertySet>.
  void setPropertySet(const OMPropertySet* propertySet);

    // @cmember The address of this <c OMProperty> object.
  OMProperty* address(void);

  virtual void detach(const OMStorable* object, const size_t key);

protected:
  int _propertyId;
  int _type;
  const char* _name;
  // The PropertySet that contains this property
  //
  const OMPropertySet* _propertySet;
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
                      const int type,
                      const char* name);

    // @cmember Destructor.
  virtual ~OMReferenceProperty(void);

    // @cmember Set the value of this <c OMReferenceProperty>.
  virtual void setValue(const ReferencedObject*& object);

    // @cmember Get the value of this <c OMReferenceProperty>.
    //   @this const
  virtual void getValue(ReferencedObject*& object) const = 0;

protected:
  virtual ReferencedObject* pointer(void) const;

  ReferencedObject* _pointer; // The referenced object
  char* _pathName;  // needed by both strong and weak references ?
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
  virtual void setValue(const ReferencedObject*& object);

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

    // @cmember Save this <c OMStrongReferenceProperty> to the
    //          <c OMStoredObject> <p s>.
    // @this const
  virtual void saveTo(OMStoredObject& s) const;

    // @cmember Restore this <c OMStrongReferenceProperty> from the
    //          <c OMStoredObject> <p s>, the size of the
    //          <c OMStrongReferenceProperty> is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size);

  virtual void detach(const OMStorable* object, const size_t key);

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

    // @cmember Save this <c OMWeakReferenceProperty> to the
    //          <c OMStoredObject> <p s>.
    // @this const
  virtual void saveTo(OMStoredObject& s) const;

    // @cmember Restore this <c OMWeakReferenceProperty> from the
    //          <c OMStoredObject> <p s>, the size of the
    //          <c OMWeakReferenceProperty> is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size);

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

    // @cmember Save this <c OMSimpleProperty> to the
    //          <c OMStoredObject> <p s>.
    //   @this const
  virtual void saveTo(OMStoredObject& s) const;

    // @cmember The size of this <c OMSimpleProperty>.
    //   @this const
  size_t size(void) const;

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

    // @cmember Restore this <c OMFixedSizeProperty> from the
    //          <c OMStoredObject> <p s>, the size of the
    //          <c OMFixedSizeProperty> is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size);

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

    // @cmember Get the value of this <c OMvariableSizeProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMVariableSizeProperty>. The buffer is at address
    //          <p buffer> and is <p bufferSize> bytes in size.
    //          Copying only takes place if the buffer is large enough.
    //   @this const
  bool copyToBuffer(PropertyType* buffer, size_t bufferSize) const;

    // @cmember Restore this <c OMVariableSizeProperty> from the
    //          <c OMStoredObject> <p s>, the size of the
    //          <c OMVariableSizeProperty> is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size);

};

  // @class Abstract base class for persistent collection properties
  //        supported by the Object Manager.
  //   @base public | <c OMProperty>
class OMCollectionProperty : public OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMCollectionProperty(const OMPropertyId propertyId,
                       const int type,
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

    // @cmember Save this <c OMStrongReferenceVectorProperty> to the
    //          <c OMStoredObject> <p s>.
    // @this const
  virtual void saveTo(OMStoredObject& s) const;

    // @cmember Restore this <c OMStrongReferenceVectorProperty> from
    //          the <c OMStoredObject> <p s>, the size of the
    //          <c OMStrongReferenceVectorProperty> is <p size>.
  virtual void restoreFrom(OMStoredObject& s, size_t size);

    // @cmember Get the size of this <c OMStrongReferenceVectorProperty>.
    //   @this const
  void getSize(size_t& size) const;

    // @cmember Get the size of this <c OMStrongReferenceVectorProperty>.
    //   @this const
  size_t getSize(void) const;

    // @cmember Set the value of the <p OMReferencedObject> at
    //          position <p index> in this
    //          <c OMStrongReferenceVectorProperty>.
  void setValueAt(const ReferencedObject* value, const size_t index);

    // @cmember Get the value of the <p OMReferencedObject> at
    //          position <p index> in this
    //          <c OMStrongReferenceVectorProperty>.
    //   @this const
  void getValueAt(ReferencedObject*& value, const size_t index) const;

    // @cmember Appened the given <p OMReferencedObject> <p value> to
    //          this <c OMStrongReferenceVectorProperty>.
  void appendValue(const ReferencedObject*& value);

  virtual void detach(const OMStorable* object, const size_t key);

private:

  void grow(size_t additionalElements);

  static char* elementName(const char* vectorPropertyName, size_t vectorIndex);

  const ReferencedObject** _vector;
  size_t _sizeOfVector; // Actual size
  size_t _size;         // Number of elements in use
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
   //           characters <p characterString>.
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
