/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

// @doc OMINTERNAL
#ifndef OMSTORABLE_H
#define OMSTORABLE_H

#include "OMDataTypes.h"

#include "OMPropertySet.h"

class OMStoredObject;
class OMFile;
class OMClassFactory;

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

    // @cmember The path to this <c OMStorable> from the root of
    //          the <c OMFile> in which this <c OMStorable> resides.
    //   @this const
  const char* pathName(void) const;

    // @cmember Is this <c OMStorable> the root of the object
    //          containment hierarchy.
    //   @this const
  bool isRoot(void) const;

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

    // @cmember The <c OMClassFactory> that created this object.
    //   @this const
  OMClassFactory* classFactory(void) const;

    // @cmember Inform this <c OMStorable> of the <c OMClassFactory>
    //          that was used to create it.
  void setClassFactory(const OMClassFactory* classFactory);

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
  const OMClassFactory* _classFactory;

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
