// @doc

#ifndef OMPROPERTY_H
#define OMPROPERTY_H

#include "OMPortability.h"
#include <stddef.h>

const int TID_NONE                           = 0;
const int TID_PROPERTY_DATA                  = 0;
const int TID_STRONG_OBJECT_REFERENCE        = 1;
const int TID_STRONG_OBJECT_REFERENCE_VECTOR = 2;
const int TID_WEAK_OBJECT_REFERENCE          = 3;

class OMStoredObject;
class OMStorable;

  // @class Abstract base class for persistent properties supported by
  //        the Object Manager.
class OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMProperty(int pid, const char* name);

    // @cmember Constructor.
  OMProperty(int pid, const int type, const char* name);

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
  int pid(void) const;

    // @cmember Inform this <c OMProperty> that it is a property of
    //          the <c OMStorable> object <p containingObject>.
  void setContainingObject(const OMStorable* containingObject);

    // @cmember The address of this <c OMProperty> object.
  OMProperty* address(void);

protected:
  int _pid;
  int _type;
  const char* _name;
  // The object that contains this property
  //
  const OMStorable* _containingObject;
};

template <typename ReferencedObject>
class OMReferenceProperty : public OMProperty {
public:
  OMReferenceProperty(int pid, const int type, const char* name);
  virtual ~OMReferenceProperty(void);
  virtual void setValue(const ReferencedObject*& object);
  virtual void getValue(ReferencedObject*& object) const = 0;
protected:
  virtual ReferencedObject* pointer(void) const;

  ReferencedObject* _pointer; // The referenced object
  char* _pathName;  // needed by both strong and weak references ?
};

template <typename ReferencedObject>
class OMStrongReferenceProperty :
                                 public OMReferenceProperty<ReferencedObject> {
public:
  OMStrongReferenceProperty(int pid, const char* name);
  virtual ~OMStrongReferenceProperty(void);
  virtual void getValue(ReferencedObject*& object) const;
  virtual void setValue(const ReferencedObject*& object);
  OMStrongReferenceProperty<ReferencedObject>& operator =
                                              (const ReferencedObject*& value);
  operator ReferencedObject*(void) const;
  virtual void saveTo(OMStoredObject& s) const;
  virtual void restoreFrom(OMStoredObject& s, size_t size);
};

template <typename ReferencedObject>
class OMWeakReferenceProperty : public OMReferenceProperty<ReferencedObject> {
public:
  OMWeakReferenceProperty(int pid, const char* name);
  virtual ~OMWeakReferenceProperty(void);
  virtual void getValue(ReferencedObject*& object) const;
  OMWeakReferenceProperty<ReferencedObject>& operator =
                                              (const ReferencedObject*& value);
  operator ReferencedObject*(void) const;
  virtual void saveTo(OMStoredObject& s) const;
  virtual void restoreFrom(OMStoredObject& s, size_t size);
};

class OMSimpleProperty : public OMProperty {
public:
  OMSimpleProperty(int pid, const char* name, size_t valueSize);
  OMSimpleProperty(int pid, const char* name);
  virtual ~OMSimpleProperty(void);
  virtual void saveTo(OMStoredObject& s) const;
  size_t size(void) const;
protected:
  void get(void* value, size_t valueSize) const;
  void set(const void* value, size_t valueSize);

  size_t _size;
  unsigned char* _bits;
};

template <typename PropertyType>
class OMFixedSizeProperty : public OMSimpleProperty {
public:
  OMFixedSizeProperty(int pid, const char* name);
  virtual ~OMFixedSizeProperty(void);
  void getValue(PropertyType& value) const;
  void setValue(const PropertyType& value);
  OMFixedSizeProperty<PropertyType>& operator = (const PropertyType& value);
  operator PropertyType(void) const;
  PropertyType* operator &(void);
  virtual void restoreFrom(OMStoredObject& s, size_t size);
};

template <typename PropertyType>
class OMVariableSizeProperty : public OMSimpleProperty {
public:
  OMVariableSizeProperty(int pid, const char* name);
  virtual ~OMVariableSizeProperty(void);
  void getValue(PropertyType* value, size_t valueSize) const;
  void setValue(const PropertyType* value, size_t valueSize);
  bool copyToBuffer(PropertyType* buffer, size_t bufferSize) const;
  virtual void restoreFrom(OMStoredObject& s, size_t size);
};

class OMCollectionProperty : public OMProperty {
public:
  OMCollectionProperty(int pid, const int type, const char* name);
  virtual ~OMCollectionProperty(void);
};

template <typename ReferencedObject>
class OMStrongReferenceVectorProperty : public OMCollectionProperty {
public:
  OMStrongReferenceVectorProperty(int pid, const char* name);
  virtual ~OMStrongReferenceVectorProperty(void);
  virtual void saveTo(OMStoredObject& s) const;
  virtual void restoreFrom(OMStoredObject& s, size_t size);
  void getSize(size_t& size) const;
  void setValueAt(const ReferencedObject*& value, const size_t index);
  void getValueAt(ReferencedObject*& value, const size_t index) const;
  void appendValue(const ReferencedObject*& value);
private:

  void grow(size_t additionalElements);

  static char* elementName(const char* vectorPropertyName, size_t vectorIndex);

  const ReferencedObject** _vector;
  size_t _sizeOfVector; // Actual size
  size_t _size;         // Number of elements in use
};

template <typename CharacterType>
class OMCharacterStringProperty :
                                 public OMVariableSizeProperty<CharacterType> {
public:
  OMCharacterStringProperty(int pid, const char * name);
  virtual ~OMCharacterStringProperty(void);
  operator const CharacterType* (void);
  operator const CharacterType* (void) const;
  void assign(const CharacterType* characterString);
  size_t length(void) const; // string length (not counting the null)
  static size_t stringLength(const CharacterType* characterString);
private:
  // hide, declare but don't define
  operator CharacterType* (void);
};

class OMStringProperty : public OMCharacterStringProperty<char> {
public:
  OMStringProperty(int pid, const char* name);
  virtual ~OMStringProperty(void);
  OMStringProperty& operator = (const char* value);
};

class OMWideStringProperty : public OMCharacterStringProperty<wchar_t> {
public:
  OMWideStringProperty(int pid, const char* name);
  virtual ~OMWideStringProperty(void);
  OMWideStringProperty& operator = (const wchar_t* value);
};

#include "OMPropertyT.h"

#endif
