#ifndef OMSTORABLE_H
#define OMSTORABLE_H

#include "OMTypes.h"

#include "OMPropertySet.h"

class OMStoredObject;
class OMFile;

// Abstract base class for all objects that may be stored by the
// Object Manager.
//
class OMStorable {

public:

  OMStorable(void);

  virtual ~OMStorable(void);

  virtual const OMClassId& classId(void) const = 0;
  
// private:
  
  void setContainingObject(const OMStorable* containingObject);

  void clearContainingObject(void);

  void setContainingProperty(const OMProperty* containingProperty,
                             const size_t key);

  void clearContainingProperty(void);
  
  void setName(const char* name);

  // Save this object.
  //
  void save(void) const;

  void close(void);

  // Restore an object, of unknown sub-class, from `s'.
  //
  static OMStorable* restoreFrom(const OMStorable* container,
                                 const char* name,
                                 OMStoredObject& s);
  
  virtual OMFile* file(void) const;

  const char* pathName(void) const;

  // The stored representation of this object.
  //
  OMStoredObject* store(void) const;

  void setStore(OMStoredObject* store);

  // Is this object attached to a file ?

  virtual bool attached(void);

private:

  // Restore the contents of an object, of known sub-class, from `s'.
  //
  void restoreContentsFrom(OMStoredObject& s);

  char* makePathName(void);

  const char* name(void) const;

protected:

  OMStorable* containingObject(void) const;

  OMPropertySet _persistentProperties;

private:
  const OMStorable* _containingObject;
  char* _name;
  char* _pathName;

  OMProperty* _containingProperty;
  size_t _key;

  OMStoredObject* _store;

};

#define OMDECLARE_STORABLE(className)                           \
public:                                                         \
  virtual const OMClassId& classId(void) const                  \
    {                                                           \
      return *_classId;                                         \
    }                                                           \
                                                                \
private:                                                        \
  static const OMClassId* const _classId;                       \
public:

#define OMDEFINE_STORABLE(className, classId)                   \
const OMClassId* const className::_classId =                    \
reinterpret_cast<const OMClassId* const>(&classId);

#endif
