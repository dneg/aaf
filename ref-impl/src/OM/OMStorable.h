// @doc OMINTERNAL
#ifndef OMSTORABLE_H
#define OMSTORABLE_H

#include "OMDataTypes.h"

#include "OMPropertySet.h"

class OMStoredObject;
class OMFile;

// @class Abstract base class for all objects that may be stored by
//        the Object Manager.
//
class OMStorable {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStorable(void);

    // @cmember Destructor.
  virtual ~OMStorable(void);

    // @cmember The stored object identifier for the class of this
    //          <c OMStorable>.
    //   @this const
  virtual const OMClassId& classId(void) const = 0;
  
// private:

    // @cmember Inform this <c OMStorable> that it is contained
    //          (owned) by the <c OMStorable> <p containingObject>.
  void setContainingObject(const OMStorable* containingObject);

    // @cmember Inform this <c OMStorable> that it is no longer contained.
  void clearContainingObject(void);

    // @cmember Inform this <c OMStorable> that it is contained
    //          within the <c OMProperty> <p containingProperty>.
  void setContainingProperty(const OMProperty* containingProperty,
                             const size_t key);

    // @cmember Inform this <c OMStorable> that it is no longer
    //          contained within any <c OMProperty>.
  void clearContainingProperty(void);
  
    // @cmember Give this <c OMStorable> a name.
  void setName(const char* name);

    // @cmember Save this <c OMStorable>.
    //   @this const
  void save(void) const;

    // @cmember Close this <c OMStorable>.
  void close(void);

    // @cmember Restore an <c OMStorable> (of unknown sub-class) from
    // the stored representation <p s>.
  static OMStorable* restoreFrom(const OMStorable* container,
                                 const char* name,
                                 OMStoredObject& s);
  
    // @cmember The <c OMFile> in which this <c OMStorable> has a
    //          persistent representation.
  virtual OMFile* file(void) const;

  const char* pathName(void) const;

    // @cmember The stored representation of this <c OMStorable>.
    //   @this const
  OMStoredObject* store(void) const;

    // @cmember Inform this <c OMStorable> where it should store its
    //          persistent representation.
  void setStore(OMStoredObject* store);

    // @cmember Is this <c OMStorable> attached to (owned by) another
    //          <c OMStorable> ?
  virtual bool attached(void) const;

    // @cmember Is this <c OMStorable> associated with an <c OMFile> ?
  virtual bool inFile(void) const;

    // @cmember Is this <c OMStorable> a persistent object ?
    //          Persistent objects are associated with a persistent
    //          store (disk file).
  virtual bool persistent(void) const;

    // @cmember This object's property set.
  virtual OMPropertySet* propertySet(void);

private:
  // @access Private members.

    // @cmember Restore the contents of an <c OMStorable> (of unknown
    //          sub-class) from the stored representation <p s>.
  void restoreContentsFrom(OMStoredObject& s);

  char* makePathName(void);

protected:
  // @access Protected members.

    // @cmember The name of this <c OMStorable>.
  const char* name(void) const;

    // @cmember The <c OMStorable> that contains (owns) this
    //          <c OMStorable>.
    //   @this const
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
  virtual const OMClassId& classId(void) const;

#define OMDEFINE_STORABLE(className, objectId)                  \
                                                                \
const OMClassId& className::classId(void) const                 \
{                                                               \
  return *reinterpret_cast<const OMClassId* const>(&objectId);  \
}

#endif
